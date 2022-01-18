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

#include "rorrandom.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE Random<_type>::Random(_type a_min, _type a_max)
{
#ifdef CONSTANT_SEED
	this->m_engine = std::make_unique<std::mt19937>(1);
#else
	this->m_engine = std::make_unique<std::mt19937>(this->m_device());
#endif
	this->reset(a_min, a_max);
}

template <class _type>
FORCE_INLINE void Random<_type>::reset(_type a_min, _type a_max)
{
	this->_reset(a_min, a_max, std::is_integral<_type>{});
}

template <class _type>
FORCE_INLINE void Random<_type>::_reset(_type a_min, _type a_max, std::true_type)
{
	this->m_distribution = std::make_unique<std::uniform_int_distribution<_type>>(a_min, a_max);
}

template <class _type>
FORCE_INLINE void Random<_type>::_reset(_type a_min, _type a_max, std::false_type)
{
	this->m_distribution = std::make_unique<std::uniform_real_distribution<_type>>(a_min, a_max);
}

template <class _type>
FORCE_INLINE _type Random<_type>::next() const
{
	return this->_next(std::is_integral<_type>{});
}

template <class _type>
FORCE_INLINE _type Random<_type>::_next(std::true_type) const
{
	return (*this->m_distribution)(*this->m_engine);
}

template <class _type>
FORCE_INLINE _type Random<_type>::_next(std::false_type) const
{
	return (*this->m_distribution)(*this->m_engine);
}

}        // namespace ror
