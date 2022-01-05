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

#include "rorcache.hpp"
#include <functional>
#include <utility>

namespace ror
{
template <class _key, class _type, bool _thread_safe, class _hasher>
bool Cache<_key, _type, _thread_safe, _hasher>::insert(_key a_key, _type a_value)
{
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	auto result = this->m_cache.emplace(a_key, a_value);
	return result.second;
}

template <class _key, class _type, bool _thread_safe, class _hasher>
_type Cache<_key, _type, _thread_safe, _hasher>::remove(_key a_key)
{
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	auto to_be_erased = this->m_cache.at(a_key);
	this->m_cache.erase(a_key);

	return to_be_erased->second;
}

/**
 * Returns a flag and value in the map looked up by key.
 * It will always return a value and bool even if the value didn't exist, in which case bool=false
 * The reason I am returning _type and not iterator is because iterator could be invalidated
 * by another thread insert in the meantime.
 */
template <class _key, class _type, bool _thread_safe, class _hasher>
std::pair<_type, bool> Cache<_key, _type, _thread_safe, _hasher>::find(_key a_key)
{
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	auto iter = this->m_cache.find(a_key);

	return iter != this->m_cache.end() ?
			   std::make_pair(iter->second, true) :
			   std::make_pair(_type{}, false);
}

}        // namespace ror
