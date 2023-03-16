// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
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

#define ROAR_FONT_FILE_NAME_FAR "roar_icons.ttf"

#define ROAR_ICON_RANGE_START 0xe000
#define ROAR_ICON_RANGE_END 0xe003        // Update the range when more items added

// Use core/assets/fonts/unicode_to_utf8.py to generate UTF-8 for each unicode
#define ROAR_ICON_CAMERA "\xee\x80\x80"                   // U+e000
#define ROAR_ICON_DIRECTIONAL_LIGHT "\xee\x80\x81"        // U+e001
#define ROAR_ICON_POINT_LIGHT "\xee\x80\x82"              // U+e002
#define ROAR_ICON_SPOT_LIGHT "\xee\x80\x83"               // U+e003
#define ROAR_ICON_AREA_LIGHT "\xee\x80\x83"               // U+e003 //TODO: Add unique icon later and change RANGE_END
