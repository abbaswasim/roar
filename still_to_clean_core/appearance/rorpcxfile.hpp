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

#include "memory_manager/rormemory_check.h"

namespace ror
{
class ROAR_ENGINE_ITEM cpcxfile
{
  public:
#pragma pack(push, 1)
	typedef struct
	{
		rUsChar manufacturer;
		rUsChar version;
		rUsChar encoding;
		rUsChar bits_per_pixel;

		struct
		{
			rUsShort xmin;
			rUsShort ymin;
			rUsShort xmax;
			rUsShort ymax;
		} window;

		rShort  HDpi;
		rShort  VDpi;
		rUsChar color_map[48];
		rUsChar reserved1;
		rUsChar NPlanes;
		rShort  bytes_per_line;
		rShort  palette_info;
		rShort  hscreen_size;
		rShort  vscreen_size;
		rUsChar filler[54];
	} pcxheader;

	typedef struct
	{
		union
		{
			struct
			{
				rUsChar count : 6;
				/* if these two bits are 1, then we have a start of a byte run */
				rUsChar flag1 : 1;
				rUsChar flag2 : 1;
			};
			rUsChar byte;
		};
	} pcxbyte;

	typedef struct
	{
		rUsChar r;
		rUsChar g;
		rUsChar b;
	} pcxcolour;
#pragma pack(pop)

	cpcxfile(void);
	cpcxfile(std::string &a_file_name, uint32_t &a_bpp, uint32_t &a_width, uint32_t &a_height, rUsChar **a_pixel_data);

	static bool load_pcxfile(std::string &a_file_name, uint32_t &a_bpp, uint32_t &a_width, uint32_t &a_height, rUsChar **a_pixel_data);
};
}        // namespace ror
