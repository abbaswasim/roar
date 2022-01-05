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

#include "foundation/rorutilities.hpp"
#include "roar.hpp"
#include <unordered_map>

namespace ror
{

/**
 * Thread Safe unordered_map used as a Cache. A very simple wrapper
 * std::unordered_map isn't thread safe like other STL containers
 * This class tries to limit its interface to very few methods, all thread safe
 */
template <class _key, class _type, bool _thread_safe = false, class _hasher = std::hash<_key>>
class ROAR_ENGINE_ITEM Cache final : ConditionalMutex<_thread_safe>
{
  public:
	FORCE_INLINE        Cache()                         = default;            //! Default constructor
	FORCE_INLINE        Cache(const Cache &a_other)     = default;            //! Copy constructor
	FORCE_INLINE        Cache(Cache &&a_other) noexcept = default;            //! Move constructor
	FORCE_INLINE Cache &operator=(const Cache &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Cache &operator=(Cache &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Cache() noexcept                          = default;        //! Destructor

	// NOTE: Don't add methods like begin() or end() which makes thread safety impossible
	bool                   insert(_key a_key, _type a_value);
	_type                  remove(_key a_key);
	std::pair<_type, bool> find(_key a_key);

  protected:
  private:
	std::unordered_map<_key, _type, _hasher> m_cache{};
};
}        // namespace ror

#include "rorcache.hh"
