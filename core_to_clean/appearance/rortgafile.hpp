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
class ROAR_ENGINE_ITEM ctgafile
{
  public:
#pragma pack(push, 1)
	typedef struct
	{
		rUsChar  m_image_ident;              // size of image identification field
		rUsChar  m_colour_map_type;          // 0 - No colour map, 1 - has colour map
		rUsChar  m_image_type;               // 0 - No Image, 1 - 8bit, 2 - uncompress RGB
		rUsShort m_colour_map_origin;        // Index of first colour map entry
		rUsShort m_colour_map_size;          // Number of colour map entries
		rUsChar  m_colour_map_esize;         // Number of bits for each entry (24)
		rUsShort m_xorigin;                  // x origin of image
		rUsShort m_yorigin;                  // y origin of image
		rUsShort m_width;                    // width of image in pixels
		rUsShort m_height;                   // height of image in pixels
		rUsChar  m_pixel_index_size;         // number of bits stored in each pixel index
		rUsChar  m_image_desc_byte;          // should always be zero
	} tga_header;
#pragma pack(pop)

	ctgafile(void);
	ctgafile(std::string &a_file_name, uint32_t &a_bpp, uint32_t &a_width, uint32_t &a_height, rUsChar **a_pixel_data);

	static bool load_tgafile(std::string &a_file_name, uint32_t &a_bpp, uint32_t &a_width, uint32_t &a_height, rUsChar **a_pixel_data);
};
}        // namespace ror
