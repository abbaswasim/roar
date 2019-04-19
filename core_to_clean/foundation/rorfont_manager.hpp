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

#include "rorfont.h"
#include "rorsingleton.h"

namespace ror
{
class ROAR_ENGINE_ITEM FontManager : public ror::Singleton<FontManager>
{
  public:
	FORCE_INLINE ror::Font *create_font(std::string a_font_name = "Courier New", int32_t a_height = 12, bool a_bold = true, bool a_italic = false);
	FORCE_INLINE ror::Font *get_font(uint32_t a_index);
	FORCE_INLINE int32_t get_font(ror::Font *a_font);
	FORCE_INLINE void    release_font(uint32_t a_index);
	FORCE_INLINE void    release_font(ror::Font *a_font);
	FORCE_INLINE void    release_all_fonts();
	FORCE_INLINE ror::Font *get_default_font() const;
	FORCE_INLINE void       set_default_font(ror::Font *a_font);

  protected:
	FORCE_INLINE bool add_font(ror::Font *a_font);

	//static int32_t	CALLBACK			EnumerateFonts(LOGFONT *a_LogFont, TEXTMETRIC *a_TextMatric, DWORD a_Type, LPARAM a_Data);
	void get_font_names(std::vector<std::string> *a_fonts);

  private:
	FORCE_INLINE FontManager(void);
	virtual ~FontManager(void);

	// Make CSingleton<CFontManager> as friend
	friend class Singleton<FontManager>;

	typedef std::vector<ror::Font *> fonts_vector;

	fonts_vector *m_fonts;               // All the fonts
	ror::Font *   m_default_font;        // Default Active Font

	static std::vector<std::string> *m_fonts_installed;
};
}        // namespace ror

#include "rorfont_manager.inl"
