// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2019
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the 'Software'),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version: 1.0.0

#pragma once

#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorprojectroot.hpp"

// Using third party xxHash implementation
#include "thirdparty/xxHash/xxhash.h"
#include <cstddef>

// TODO: Check performance of this with std::hash, in some tests with std::string std::hash performs better

namespace ror
{
FORCE_INLINE hash_32_t hash_32(const void *a_input, size_t a_size)
{
	static const uint32_t seed = static_cast<uint32_t>(get_default_project_root().hash());        // Truncation from uint64_t to uint32_t is ok here
	return XXH32(a_input, a_size, seed);
}

FORCE_INLINE hash_32_t hash_32(const std::string& a_input)
{
	return hash_32(a_input.c_str(), a_input.size());
}

FORCE_INLINE hash_64_t hash_64(const void *a_input, size_t a_size)
{
	static const uint64_t seed = get_default_project_root().hash();
	return XXH64(a_input, a_size, seed);
}

FORCE_INLINE hash_64_t hash_64(const std::string& a_input)
{
	return hash_64(a_input.c_str(), a_input.size());
}

FORCE_INLINE hash_128_t hash_128(const void *a_input, size_t a_size)
{
	auto hs = XXH3_128bits(a_input, a_size);
	return {hs.low64, hs.high64};
}

FORCE_INLINE hash_128_t hash_128(const std::string& a_input)
{
	return hash_128(a_input.c_str(), a_input.size());
}

// The following is based on boost hash_combine
// Not sure about result of this guy. test it
FORCE_INLINE void hash_combine_32(hash_32_t &a_output_hash, hash_32_t a_input_hash)
{
	a_output_hash ^= a_input_hash + 0x9e3779b9 + (a_output_hash << 6) + (a_output_hash >> 2);
}

FORCE_INLINE void hash_combine_64(hash_64_t &a_output_hash, hash_64_t a_input_hash)
{
	a_output_hash ^= a_input_hash + 0x9e3779b9 + (a_output_hash << 6) + (a_output_hash >> 2);
}

FORCE_INLINE void hash_combine_128(hash_128_t &a_output_hash, hash_128_t a_input_hash)
{
	hash_combine_64(a_output_hash.low, a_input_hash.low);
	hash_combine_64(a_output_hash.high, a_input_hash.high);
}

/**
 * Use this to create hash from data not known at once, this is statefull function, be-aware, its NOT reinterant and NOT thread-safe
 * Instead of building a cliend side buffer and adding all your data and then calling hash_32 on that
 * Its better to use the xxHash streaming functionality
 * the a_size parameter has multiple roles
 * a_size == 0 means return the so far generated hash
 * a_size < 0 means reset the streamer to default setting, ready to accept data
 * a_size > 0 means incoming data with length == a_size

 * Therefore one would have to use the function like:
 * hash_stream_32(nullptr, -1);
 * hash_stream_32(data, 20);
 * hash_stream_32(data, 10);
 * hash_stream_32(data, 40);
 * auto hash = hash_stream_32(data, 0);
 */
FORCE_INLINE hash_32_t hash_stream_32(const void *a_input, int32_t a_size)
{
	static XXH32_state_t *const state = XXH32_createState();
	assert(state && "xxHash state can't be initialized can't continue");
	static const uint32_t seed = static_cast<uint32_t>(get_default_project_root().hash());        // Truncation from uint64_t to uint32_t is ok here

	if (a_size > 0)
	{
		auto res = XXH32_update(state, a_input, static_cast<size_t>(a_size));
		assert(res != XXH_ERROR && "hash_stream_32 update failed");
		(void) res;
	}
	else if (a_size == 0)
	{
		XXH32_hash_t const hash = XXH32_digest(state);
		return hash;
	}
	else
	{
		auto res = XXH32_reset(state, seed);
		assert(res != XXH_ERROR && "hash_stream_64 reset failed");
		(void) res;
	}

	return 0;
}

/**
 * Use this to create hash from data not known at once, this is statefull function, be-aware, its NOT reinterant and NOT thread-safe
 * Instead of building a cliend side buffer and adding all your data and then calling hash_64 on that
 * Its better to use the xxHash streaming functionality
 * the a_size parameter has multiple roles
 * a_size == 0 means return the so far generated hash
 * a_size < 0 means reset the streamer to default setting, ready to accept data
 * a_size > 0 means incoming data with length == a_size

 * Therefore one would have to use the function like:
 * hash_stream_64(nullptr, -1);
 * hash_stream_64(data, 20);
 * hash_stream_64(data, 10);
 * hash_stream_64(data, 40);
 * auto hash = hash_stream_64(nullptr, 0);
 */
FORCE_INLINE hash_64_t hash_stream_64(const void *a_input, int32_t a_size)
{
	static XXH64_state_t *const state = XXH64_createState();
	assert(state && "xxHash state can't be initialized can't continue");
	static const uint64_t seed = get_default_project_root().hash();        // Truncation from uint64_t to uint32_t is ok here

	// log_critical("seed = {}",seed);
	if (a_size > 0)
	{
		auto res = XXH64_update(state, a_input, static_cast<size_t>(a_size));
		assert(res != XXH_ERROR && "hash_stream_64 update failed");
		(void) res;
	}
	else if (a_size == 0)
	{
		XXH64_hash_t const hash = XXH64_digest(state);
		return hash;
	}
	else
	{
		auto res = XXH64_reset(state, seed);
		assert(res != XXH_ERROR && "hash_stream_64 reset failed");
		(void) res;
	}

	return 0;
}

}        // namespace ror
