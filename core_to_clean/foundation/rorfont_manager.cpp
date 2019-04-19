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

#include "rorfont_manager.h"

namespace ror
{
std::vector<std::string> *FontManager::m_fonts_installed = new std::vector<std::string>();        // This will be a leack but since the application will end anyways i don't care

FontManager::~FontManager(void)
{
	//		this->ReleaseAllFonts();
	//		rDELETE (this->m_Fonts);
}

//	int32_t	CALLBACK CFontManager::EnumerateFonts( LOGFONT *a_LogFont, TEXTMETRIC *a_TextMatric, DWORD a_Type, LPARAM a_Data )
//	{
//		std::vector<std::string> *Fonts = (std::vector<std::string>*)a_Data;
//		char8_t SomeAsciiStr[50];
//		WideCharToMultiByte(CP_ACP, 0, a_LogFont->lfFaceName, -1, SomeAsciiStr, 50, nullptr, nullptr);
//		std::string temp = SomeAsciiStr;
//		Fonts->push_back(temp);
//		return 1;
//	}

void FontManager::get_font_names(std::vector<std::string> *a_fonts)
{
	//		HWND hDskWnd= GetDesktopWindow();
	//		HDC htempDC	= GetDC(hDskWnd);
	//		HDC hMyDC	= CreateCompatibleDC(htempDC);
	//
	//		ReleaseDC(hDskWnd,htempDC);
	//		EnumFontFamilies(hMyDC,nullptr,(FONTENUMPROC)EnumerateFonts,(LPARAM)a_Fonts);
	//		DeleteDC(hMyDC);
}

}        // namespace ror
