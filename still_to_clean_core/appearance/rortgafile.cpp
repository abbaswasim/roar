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

#include "rortgafile.h"

namespace ror
{
ctgafile::ctgafile()
{}

ctgafile::ctgafile(std::string &a_file_name, uint32_t &a_bpp, uint32_t &a_width, uint32_t &a_height, rUsChar **a_pixel_data)
{
	this->load_tgafile(a_file_name, a_bpp, a_width, a_height, a_pixel_data);
}

bool ctgafile::load_tgafile(std::string &a_file_name, uint32_t &a_bpp, uint32_t &a_width, uint32_t &a_height, rUsChar **a_pixel_data)
{
	tga_header header;
	FILE *     fp;

	fp1 = fopen(a_file_name.c_str(), "rb");
	// Introduced error to fix fopen later (use ifstream)
	if (fp)
	{
		fread(&header, 1, sizeof(tga_header), fp);
		a_width  = header.m_width;
		a_height = header.m_height;

		for (int32_t i = 0; i != header.m_image_ident; ++i)
			fgetc(fp);

		a_bpp = header.m_pixel_index_size / 8;

		*a_pixel_data = new rUsChar[a_bpp * a_width * a_height];
		assert(*a_pixel_data != nullptr && "Out of Memory");
		assert(a_pixel_data != nullptr && "Pixel data for TGA File is null");

		// Now Start Loading Different types of TGA files
		assert(header.m_image_type == 2 && "Can't Load other types of TGA then 2");
		//if( header.m_ImageType  == 2 )
		fread(*a_pixel_data, sizeof(rUsChar), a_bpp * a_width * a_height, fp);
		// To Use RGBA TGA Textures instead of BGRA, enable this code
		//			rUsChar * temp = *a_PixelData;
		//			for(int32_t Index = 0; Index < a_Bpp * a_Width * a_Height; Index+=a_Bpp)
		//			{
		//				uchar8_t t = temp[Index];
		//				temp[Index] = temp[Index+2];
		//				temp[Index+2] = t;
		//			}
		fclose(fp);
		return true;
	}
	return false;
}

}        // namespace ror
