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

#include "rorcompiler_workarounds.hpp"
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

using ::int16_t;
using ::int32_t;
using ::int64_t;
using ::int8_t;

using ::uint16_t;
using ::uint32_t;
using ::uint64_t;
using ::uint8_t;

using ::intptr_t;
using ::uintptr_t;

using ::intmax_t;
using ::uintmax_t;

using ::ptrdiff_t;
using ::size_t;

#if (__cplusplus < 202001L)        // TODO: Change this to 2020 when available
using char8_t = int8_t;
#endif

using uchar8_t  = uint8_t;
using uchar16_t = uint16_t;
using uchar32_t = uint32_t;

using float32_t   = float;              // Just for consistancy reasons, also compile time checked for size
using double64_t  = double;             // Just for consistancy reasons, also compile time checked for size

// Can't use double128 because I can't gurantee this, avoid using this unless absolutely sure that a 64bit version will be ok
// using double128_t = long double;        // Just for consistancy reasons, also compile time checked for size

// Types for hashing
using hash_32_t = uint32_t;
using hash_64_t = uint64_t;

struct hash_128_t
{
	hash_64_t low;
	hash_64_t high;
};

// Check sizes you are getting are what you expect
static_assert(sizeof(char8_t) == 1, "char8_t is not 1 byte");
static_assert(sizeof(char16_t) == 2, "char16_t is not 2 bytes");
static_assert(sizeof(char32_t) == 4, "char32_t is not 4 bytes");

static_assert(sizeof(uchar8_t) == 1, "uchar8_t is not 1 byte");
static_assert(sizeof(uchar16_t) == 2, "uchar16_t is not 2 bytes");
static_assert(sizeof(uchar32_t) == 4, "uchar32_t is not 4 bytes");

static_assert(sizeof(int8_t) == 1, "int8_t is not 1 byte");
static_assert(sizeof(int16_t) == 2, "int16_t is not 2 bytes");
static_assert(sizeof(int32_t) == 4, "int32_t is not 4 bytes");
static_assert(sizeof(int64_t) == 8, "int64_t is not 8 bytes");

static_assert(sizeof(uint8_t) == 1, "uint8_t is not 1 byte");
static_assert(sizeof(uint16_t) == 2, "uint16_t is not 2 bytes");
static_assert(sizeof(uint32_t) == 4, "uint32_t is not 4 bytes");
static_assert(sizeof(uint64_t) == 8, "uint64_t is not 8 bytes");

static_assert(sizeof(uint64_t) == sizeof(size_t), "size_t is not same as uint64_t");
static_assert(sizeof(ptrdiff_t) == sizeof(size_t), "size_t is not same as ptrdiff_t");
static_assert(sizeof(intptr_t) == sizeof(size_t), "size_t is not same as intptr_t");        // This will fail on 32bit

static_assert(sizeof(float32_t) == 4, "uint32_t is not 4 bytes");
static_assert(sizeof(double64_t) == 8, "double64_t is not 8 bytes");

static_assert(sizeof(hash_128_t) == 16, "hash_128_t is not 16 bytes");

#if 1

static_assert(std::alignment_of_v<char8_t> == 1, "char8_t is not 1 bytes aligned");
static_assert(std::alignment_of_v<char16_t> == 2, "char16_t is not 2 bytes aligned");
static_assert(std::alignment_of_v<char32_t> == 4, "char32_t is not 4 bytes aligned");

static_assert(std::alignment_of_v<uchar8_t> == 1, "uchar8_t is not 1 bytes aligned");
static_assert(std::alignment_of_v<uchar16_t> == 2, "uchar16_t is not 2 bytes aligned");
static_assert(std::alignment_of_v<uchar32_t> == 4, "uchar32_t is not 4 bytes aligned");

static_assert(std::alignment_of_v<int8_t> == 1, "int8_t is not 1 bytes aligned");
static_assert(std::alignment_of_v<int16_t> == 2, "int16_t is not 2 bytes aligned");
static_assert(std::alignment_of_v<int32_t> == 4, "int32_t is not 4 bytes aligned");
static_assert(std::alignment_of_v<int64_t> == 8, "int64_t is not 8 bytes aligned");
static_assert(std::alignment_of_v<size_t> == 8, "size_t is not 8 bytes aligned");

static_assert(std::alignment_of_v<uint8_t> == 1, "uint8_t is not 1 bytes aligned");
static_assert(std::alignment_of_v<uint16_t> == 2, "uint16_t is not 2 bytes aligned");
static_assert(std::alignment_of_v<uint32_t> == 4, "uint32_t is not 4 bytes aligned");
static_assert(std::alignment_of_v<uint64_t> == 8, "uint64_t is not 8 bytes aligned");

static_assert(std::alignment_of_v<float32_t> == 4, "float32_t is not 4 bytes aligned");
static_assert(std::alignment_of_v<double64_t> == 8, "double64_t is not 8 bytes aligned");

static_assert(std::alignment_of_v<intptr_t> == 8, "intptr_t is not 8 bytes aligned");          // This will fail on 32bit
static_assert(std::alignment_of_v<ptrdiff_t> == 8, "ptrdiff_t is not 8 bytes aligned");        // This will fail on 32bit

static_assert(std::alignment_of_v<hash_128_t> == 8, "has_128_t is not 16 bytes aligned");

#else

static_assert(std::alignment_of<char8_t>() == 1, "char8_t is not 1 bytes aligned");
static_assert(std::alignment_of<char16_t>() == 2, "char16_t is not 2 bytes aligned");
static_assert(std::alignment_of<char32_t>() == 4, "char32_t is not 4 bytes aligned");

static_assert(std::alignment_of<uchar8_t>() == 1, "uchar8_t is not 1 bytes aligned");
static_assert(std::alignment_of<uchar16_t>() == 2, "uchar16_t is not 2 bytes aligned");
static_assert(std::alignment_of<uchar32_t>() == 4, "uchar32_t is not 4 bytes aligned");

static_assert(std::alignment_of<int8_t>() == 1, "int8_t is not 1 bytes aligned");
static_assert(std::alignment_of<int16_t>() == 2, "int16_t is not 2 bytes aligned");
static_assert(std::alignment_of<int32_t>() == 4, "int32_t is not 4 bytes aligned");
static_assert(std::alignment_of<int64_t>() == 8, "int64_t is not 8 bytes aligned");
static_assert(std::alignment_of<size_t>() == 8, "size_t is not 8 bytes aligned");

static_assert(std::alignment_of<uint8_t>() == 1, "uint8_t is not 1 bytes aligned");
static_assert(std::alignment_of<uint16_t>() == 2, "uint16_t is not 2 bytes aligned");
static_assert(std::alignment_of<uint32_t>() == 4, "uint32_t is not 4 bytes aligned");
static_assert(std::alignment_of<uint64_t>() == 8, "uint64_t is not 8 bytes aligned");

static_assert(std::alignment_of<float32_t>() == 4, "float32_t is not 4 bytes aligned");
static_assert(std::alignment_of<double64_t>() == 8, "double64_t is not 8 bytes aligned");

static_assert(std::alignment_of<intptr_t>() == 8, "intptr_t is not 8 bytes aligned");          // This will fail on 32bit
static_assert(std::alignment_of<ptrdiff_t>() == 8, "ptrdiff_t is not 8 bytes aligned");        // This will fail on 32bit

static_assert(std::alignment_of<hash_128_t>() == 8, "has_128_t is not 16 bytes aligned");

#endif

// Loads data at 8 bytes aligned address
static_assert(alignof(std::vector<uint8_t>) == 8, "Bytes vector not aligned to 8 bytes");
static_assert(alignof(std::vector<int8_t>) == 8, "Bytes vector not aligned to 8 bytes");
static_assert(alignof(std::vector<char8_t>) == 8, "Bytes vector not aligned to 8 bytes");
static_assert(alignof(std::vector<char>) == 8, "Bytes vector not aligned to 8 bytes");

namespace ror
{
template <class _type>
using ror_precision = typename std::conditional<std::is_same<_type, double64_t>::value, double64_t, float32_t>::type;
}        // namespace ror
