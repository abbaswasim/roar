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

#include "rorfont.h"

namespace ror
{
void Font::font_init(std::string a_font_name, int32_t a_height, bool a_bold, bool a_italic)
{
	throw 1;
	//		HFONT font;
	//
	//		m_TextBase = glGenLists(96);
	//		this->m_Height = a_Height;
	//
	//		if(m_TextBase == 0)
	//		{
	//			MESSAGEBOX("Can't Create DisplayLists");
	//			return;
	//		}
	//
	//		font=CreateFont(-a_Height,			//height
	//						0,				//default width,
	//						0, 0,			//angles
	//						(a_Bold ? FW_BOLD : FW_NORMAL),		//bold
	//						a_Italic,			//italic
	//						false,			//underline
	//						false,			//strikeout
	//						ANSI_CHARSET,	//character set
	//						OUT_TT_PRECIS,	//precision
	//						CLIP_DEFAULT_PRECIS,
	//						ANTIALIASED_QUALITY,	//quality
	//						FF_DONTCARE | DEFAULT_PITCH,
	//						LPCWSTR(a_FontName.c_str())//L"Courier New"
	//					);
	//
	//		HWND tempHWND = GetActiveWindow();
	//		HDC tempDC = GetDC(tempHWND);
	//		SelectObject(tempDC, font);
	//		wglUseFontBitmaps(tempDC, 32, 96, m_TextBase);
}

}        // namespace ror
