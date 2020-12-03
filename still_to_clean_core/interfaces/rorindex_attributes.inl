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
FORCE_INLINE IndexAttributes::IndexAttributes(void) :
    m_offset(0),
    m_no_of_attributes(0),
    m_ibodata(nullptr),
    m_indices_minimum(0),
    m_indices_maximum(1)
{}

FORCE_INLINE IndexAttributes::~IndexAttributes(void)
{
	m_ibodata = nullptr;
	//rDELETEARRAY (m_IBOData);
}

FORCE_INLINE uint32_t IndexAttributes::get_offset() const
{
	return m_offset;
}

FORCE_INLINE void IndexAttributes::set_offset(uint32_t a_offset)
{
	m_offset = a_offset;
}

FORCE_INLINE uint32_t IndexAttributes::get_no_of_attribute() const
{
	return m_no_of_attributes;
}

FORCE_INLINE void IndexAttributes::set_no_of_attribute(uint32_t &a_no_of_attribute)
{
	m_no_of_attributes = a_no_of_attribute;
}

FORCE_INLINE rUsShort *IndexAttributes::get_ibodata() const
{
	return m_ibodata;
}

FORCE_INLINE void IndexAttributes::set_ibodata(rUsShort *a_ibodata)
{
	//rDELETEARRAY (m_IBOData);
	m_ibodata = a_ibodata;
}

FORCE_INLINE uint32_t IndexAttributes::get_indices_minimum() const
{
	return m_indices_minimum;
}

FORCE_INLINE void IndexAttributes::set_indices_minimum(uint32_t &a_indices_min)
{
	m_indices_minimum = a_indices_min;
}

FORCE_INLINE uint32_t IndexAttributes::get_indices_maximum() const
{
	return m_indices_maximum;
}

FORCE_INLINE void IndexAttributes::set_indices_maximum(uint32_t &a_indices_max)
{
	m_indices_maximum = a_indices_max;
}

}        // namespace ror
