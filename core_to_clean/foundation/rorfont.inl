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

#ifdef _WIN32
#	include <atltypes.h>
#endif

namespace ror
{
FORCE_INLINE Font::Font(std::string a_font_name, int32_t a_height, bool a_bold, bool a_italic)
{
	//		HFONT font;
	//
	//		this->m_TextBase = 0;
	//		this->m_TextModeList = 0;
	//		this->m_Height = a_Height;
	//
	//		this->m_TextBase = glGenLists(96);
	//		if(this->m_TextBase == 0)
	//		{
	//			MESSAGEBOX("Can't Create DisplayLists");
	//			return;
	//		}
	//
	//		font=CreateFontA(-a_Height,							//height
	//						0,									//default width,
	//						0, 0,								//angles
	//						(a_Bold ? FW_BOLD : FW_NORMAL),		//bold
	//						a_Italic,							//italic
	//						false,								//underline
	//						false,								//strikeout
	//						ANSI_CHARSET,						//character set
	//						OUT_TT_PRECIS,						//precision
	//						CLIP_DEFAULT_PRECIS,
	//						ANTIALIASED_QUALITY,				//quality
	//						FF_DONTCARE | DEFAULT_PITCH,
	//						a_FontName.c_str()//L"Courier New"//
	//			);
	//
	//		HWND tempHWND = GetActiveWindow();
	//		HDC tempDC = GetDC(tempHWND);
	//		SelectObject(tempDC, font);
	//		wglUseFontBitmaps(tempDC, 32, 96, this->m_TextBase);
	//		//CSize temp = GetTextExtent("0");
	//		int32_t widthValue;
	//		GetCharWidth32(tempDC, 'W', 'W', &widthValue);
	//		this->m_Width = (uint32_t)widthValue;
	//		DeleteObject(font);
}

FORCE_INLINE void Font::print(int32_t x, int32_t y, const char8_t *string, ...)
{
	if (string == nullptr)
		return;

	//Convert to text
	static char8_t text[256];

	va_list va;

	va_start(va, string);
	vsprintf(text, string, va);
	va_end(va);

	//Print the text
	glRasterPos2i(x, y);
	glListBase(this->m_text_base - 32);
	glCallLists((uint32_t) strlen(text), GL_UNSIGNED_BYTE, text);
}

FORCE_INLINE void Font::safe_print(int32_t x, int32_t y, const char8_t *string, ...)
{
	this->begin_text_mode();
	if (string == nullptr)
		return;

	//Convert to text
	static char8_t text[256];

	va_list va;

	va_start(va, string);
	vsprintf(text, string, va);
	va_end(va);

	//Print the text
	glRasterPos2i(x, 98 - y);
	//glWindowPos2i(x, 98-y);
	//glTranslatef(3,3,0);
	glCallLists((uint32_t) strlen(text), GL_UNSIGNED_BYTE, text);
	this->end_text_mode();
}

FORCE_INLINE void Font::font_cleanup()
{
	if (this->m_text_base != 0)
		glDeleteLists(this->m_text_base, 96);
	this->m_text_base = 0;

	if (this->m_text_mode_list != 0)
		glDeleteLists(this->m_text_mode_list, 1);
	this->m_text_mode_list = 0;
}

FORCE_INLINE Font::~Font(void)
{
	this->font_cleanup();
}

FORCE_INLINE void Font::begin_text_mode(void)
{
	if (!this->m_text_mode_list)
	{
		this->m_text_mode_list = glGenLists(1);
		glNewList(this->m_text_mode_list, GL_COMPILE);
		{
			glListBase(this->m_text_base - 32);
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, 100, 0, 100, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glDisable(GL_DEPTH_TEST);
		}
		glEndList();
	}

	glCallList(this->m_text_mode_list);
}

FORCE_INLINE void Font::end_text_mode(void)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glListBase(0);
	glEnable(GL_DEPTH_TEST);
}

FORCE_INLINE void Font::set_height(uint32_t a_height)
{
	this->m_height = a_height;
}

FORCE_INLINE uint32_t Font::get_height() const
{
	return this->m_height;
}

FORCE_INLINE void Font::set_width(uint32_t a_width)
{
	this->m_width = a_width;
}

FORCE_INLINE uint32_t Font::get_width() const
{
	return this->m_width;
}

}        // namespace ror
