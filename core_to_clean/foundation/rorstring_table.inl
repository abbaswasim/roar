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
FORCE_INLINE StringTablePredicate::StringTablePredicate(void)
{}

FORCE_INLINE StringTablePredicate::~StringTablePredicate(void)
{}

FORCE_INLINE bool StringTablePredicate::operator()(const char8_t *a_pointer1, const char8_t *a_pointer2) const
{
	int32_t return_value = strcmp(a_pointer1, a_pointer2);
	if (return_value < 0)
	{
		return true;
	}
	return false;
}

FORCE_INLINE StringTable::StringTable(void)
{}

FORCE_INLINE const char8_t *StringTable::get_string(const char8_t *a_string)
{
	character_pointer_set::iterator found;
	found = this->m_strings.find(a_string);
	if (found != this->m_strings.end())
	{
		return (*found);
	}
	int32_t  length     = (int32_t) strlen(a_string);
	char8_t *new_string = new char8_t[length + 1];
	strcpy(new_string, a_string);
	this->m_strings.insert(new_string);
	return new_string;
}

FORCE_INLINE const char8_t *StringTable::get_string(const std::string &a_string)
{
	return get_string(a_string.c_str());
}

}        // namespace ror
