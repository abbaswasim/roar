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

namespace ror
{
FORCE_INLINE ccbaobject::ccbaobject(void) :
    m_cbaid(0),
    m_reference_count(1),
    m_cbasize(0)
{}

FORCE_INLINE ccbaobject::ccbaobject(uint32_t a_id, uint32_t a_cbasize, uint32_t a_reference_count) :
    m_cbaid(a_id),
    m_reference_count(a_reference_count),
    m_cbasize(a_cbasize)
{}

FORCE_INLINE ccbaobject::ccbaobject(ccbaobject &a_other) :
    m_cbaid(a_other.m_cbaid),
    m_reference_count(a_other.m_reference_count),
    m_cbasize(a_other.m_cbasize)
{}

FORCE_INLINE uint32_t ccbaobject::get_cbaid() const
{
	return this->m_cbaid;
}

FORCE_INLINE void ccbaobject::set_cbaid(uint32_t &a_cbaid)
{
	this->m_cbaid = a_cbaid;
}

FORCE_INLINE uint32_t ccbaobject::get_cbasize() const
{
	return this->m_cbasize;
}

FORCE_INLINE void ccbaobject::set_cbasize(uint32_t a_cbaid)
{
	this->m_cbasize = a_cbaid;
}

FORCE_INLINE uint32_t ccbaobject::increment_reference_count()
{
	return ++this->m_reference_count;
}

FORCE_INLINE uint32_t ccbaobject::decrement_reference_count()
{
	return --this->m_reference_count;
}

FORCE_INLINE uint32_t ccbaobject::get_reference_count() const
{
	return this->m_reference_count;
}

FORCE_INLINE void ccbaobject::set_reference_count(uint32_t &a_reference_count)
{
	this->m_reference_count = a_reference_count;
}

FORCE_INLINE CCBA *ccbaobject::get_cbapointer() const
{
	return this->m_cbapointer;
}

FORCE_INLINE void ccbaobject::set_cbapointer(CCBA *a_cbapointer)
{
	this->m_cbapointer = a_cbapointer;
}

}        // namespace ror
