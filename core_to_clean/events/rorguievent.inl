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
FORCE_INLINE cguievent::cguievent(void)
{
	this->m_source = nullptr;
}

FORCE_INLINE cguievent::~cguievent(void)
{}

FORCE_INLINE cguievent::cguievent(cguielement *a_guielement)
{
	this->m_source = a_guielement;
}

FORCE_INLINE cguievent::cguievent(const cguievent &a_other)
{
	this->operator=(a_other);
}

FORCE_INLINE cguievent &cguievent::operator=(const cguievent &a_other)
{
	if (this != &a_other)
	{
		this->m_source = a_other.m_source;
	}
	return *this;
}

FORCE_INLINE cguielement *cguievent::get_source() const
{
	return this->m_source;
}

FORCE_INLINE void cguievent::set_source(cguielement *a_source)
{
	this->m_source = a_source;
}

}        // namespace ror
