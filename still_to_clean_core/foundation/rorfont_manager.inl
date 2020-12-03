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
FORCE_INLINE FontManager::FontManager(void)
{
	this->get_font_names(this->m_fonts_installed);

	this->m_fonts        = new std::vector<ror::Font *>();
	this->m_default_font = this->create_font();
}

FORCE_INLINE ror::Font *FontManager::create_font(std::string a_font_name, int32_t a_height, bool a_bold, bool a_italic)
{
	std::vector<std::string>::iterator found = find(this->m_fonts_installed->begin(), this->m_fonts_installed->end(), a_font_name);
	if (found == this->m_fonts_installed->end())
	{
		a_font_name += "! Font not installed in the System...";
		MESSAGEBOXERROR(a_font_name.c_str());
		//a_FontName = "Arial"; // Load Default Font, but this will create multiple copies of Arial
		return nullptr;
	}

	ror::Font *temp = rNew ror::Font(a_font_name, a_height, a_bold, a_italic);
	if (this->add_font(temp))
	{
		return (*this->m_fonts)[this->m_fonts->size() - 1];
	}
	return nullptr;
}

FORCE_INLINE bool FontManager::add_font(ror::Font *a_font)
{
	if (!a_font)
	{
		return false;
	}

	this->m_fonts->push_back(a_font);
	return true;
}

FORCE_INLINE ror::Font *FontManager::get_font(uint32_t a_index)
{
	return (a_index >= this->m_fonts->size() && a_index < 0 ? nullptr : (*this->m_fonts)[a_index]);
}

FORCE_INLINE int32_t FontManager::get_font(ror::Font *a_font)
{
	if (!a_font)
		return -1;

	static uint32_t list_size = 0;
	list_size                 = (uint32_t) this->m_fonts->size();
	for (uint32_t i = 0; i < list_size; i++)
	{
		if ((*this->m_fonts)[i] == a_font)
		{
			return int32_t(i);
		}
	}
	return -1;
}

FORCE_INLINE void FontManager::release_font(uint32_t a_index)
{
	assert(a_index < (uint32_t) this->m_fonts->size() && "Index Out of Bound, FileName : RORFontManager.inl");
	rDELETE((*this->m_fonts)[a_index]);
	this->m_fonts->erase(this->m_fonts->begin() + a_index);
}

FORCE_INLINE void FontManager::release_font(ror::Font *a_font)
{
	this->release_font(this->get_font(a_font));
}

FORCE_INLINE void FontManager::release_all_fonts()
{
	uint32_t list_size1 = 0;
	list_size1          = (uint32_t) this->m_fonts->size();
	for (uint32_t i = 0; i < list_size1; i++)
	{
		rDELETE((*this->m_fonts)[i]);
	}
	this->m_fonts->clear();
}

FORCE_INLINE void FontManager::set_default_font(ror::Font *a_font)
{
	this->m_default_font = a_font;
}

FORCE_INLINE ror::Font *FontManager::get_default_font() const
{
	return this->m_default_font;
}

}        // namespace ror
