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
FORCE_INLINE CoreString::CoreString(void) :
    m_handle(nullptr)
{}

FORCE_INLINE CoreString::CoreString(const char8_t *a_string)
{
	this->m_handle = const_cast<char8_t *>(STRING(a_string));
}

FORCE_INLINE CoreString::CoreString(CoreString &a_string)
{
	this->m_handle = a_string.m_handle;
}

FORCE_INLINE CoreString::operator const char8_t *() const
{
	return (const char8_t *) this->m_handle;
}

FORCE_INLINE CoreString &CoreString::operator=(const CoreString &a_string)
{
	if (this->m_handle != a_string.m_handle)
	{
		this->m_handle = a_string.m_handle;
	}
	return *this;
}

FORCE_INLINE CoreString &CoreString::operator=(const char8_t *a_string)
{
	this->m_handle = const_cast<char8_t *>(STRING(a_string));
}

FORCE_INLINE bool CoreString::is_empty() const
{
	return (this->m_handle == nullptr);
}

FORCE_INLINE bool CoreString::equals(const char8_t *a_string) const
{
	return (this->m_handle == a_string);
}

FORCE_INLINE bool CoreString::contains(const char8_t *a_string) const
{}

FORCE_INLINE std::string CoreString::get_string() const
{
	return std::string((const char8_t *) this->m_handle);
}

}        // namespace ror
