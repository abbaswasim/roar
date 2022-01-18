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

#include "roar.hpp"
#include <memory>
#include <random>
#include <type_traits>

namespace ror
{
template <class _type, bool>
class ROAR_ENGINE_ITEM RandomType
{};

template <class _type>
class ROAR_ENGINE_ITEM RandomType<_type, true>
{
  public:
	std::unique_ptr<std::uniform_int_distribution<_type>> m_distribution{};
};

template <class _type>
class ROAR_ENGINE_ITEM RandomType<_type, false>
{
  public:
	std::unique_ptr<std::uniform_real_distribution<_type>> m_distribution{};
};

template <class _type>
class ROAR_ENGINE_ITEM Random final : public std::conditional<std::is_integral<_type>::value,
															  RandomType<_type, true>,
															  RandomType<_type, false>>::type
{
  public:
	FORCE_INLINE Random(_type a_min = std::numeric_limits<_type>::min(),
						_type a_max = std::numeric_limits<_type>::max());        //! Default constructor
	FORCE_INLINE Random(const Random &a_other)     = delete;                     //! Copy constructor
	FORCE_INLINE Random(Random &&a_other) noexcept = delete;                     //! Move constructor
	FORCE_INLINE Random &operator=(const Random &a_other) = delete;              //! Copy assignment operator
	FORCE_INLINE Random &operator=(Random &&a_other) noexcept = delete;          //! Move assignment operator
	FORCE_INLINE ~Random() noexcept                           = default;         //! Destructor

	FORCE_INLINE void reset(_type a_min = std::numeric_limits<_type>::min(), _type a_max = std::numeric_limits<_type>::max());
	FORCE_INLINE _type next() const;

  protected:
  private:
	FORCE_INLINE void _reset(_type a_min, _type a_max, std::true_type);
	FORCE_INLINE void _reset(_type a_min, _type a_max, std::false_type);

	FORCE_INLINE _type _next(std::true_type) const;
	FORCE_INLINE _type _next(std::false_type) const;

	std::random_device            m_device{};        //!< Could be static
	std::unique_ptr<std::mt19937> m_engine{};        //!< Could be static
};

using Randomd = Random<double64_t>;
using Randomf = Random<float32_t>;
using Randomi = Random<int32_t>;

}        // namespace ror

#include "rorrandom.hh"
