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
FORCE_INLINE TextureImage::TextureImage(void) :
    m_bpp(4),
    m_width(0),
    m_height(0) /*, m_PixelData(nullptr)*/
{
	m_pixel_data = new rUsChar *;
}

FORCE_INLINE TextureImage::TextureImage(uint32_t a_bpp, uint32_t a_width, uint32_t a_height, rUsChar **a_pixel_data) :
    m_bpp(a_bpp),
    m_width(a_width),
    m_height(a_height),
    m_pixel_data(a_pixel_data)
{}

FORCE_INLINE bool TextureImage::load_image_file(std::string &a_file_name)
{
	assert(a_file_name.length() > 3 && "Can't extract Extension from file name, size is less then 3 characters");
	std::string extension = a_file_name.substr(a_file_name.length() - 3);
	if ("pcx" == extension || "PCX" == extension)
	{
		return cpcxfile::load_pcxfile(a_file_name, this->m_bpp, this->m_width, this->m_height, this->m_pixel_data);
	}
	else if ("tga" == extension || "TGA" == extension)
	{
		return ctgafile::load_tgafile(a_file_name, this->m_bpp, this->m_width, this->m_height, this->m_pixel_data);
	}
	else if ("bmp" == extension || "BMP" == extension)
	{
		return BMPFile::load_bmpfile(a_file_name, this->m_bpp, this->m_width, this->m_height, this->m_pixel_data);
	}
	return false;
}

FORCE_INLINE uint32_t TextureImage::get_bpp() const
{
	return m_bpp;
}

FORCE_INLINE void TextureImage::set_bpp(uint32_t &a_bpp)
{
	m_bpp = a_bpp;
}

FORCE_INLINE uint32_t TextureImage::get_width() const
{
	return m_width;
}

FORCE_INLINE void TextureImage::set_width(uint32_t &a_width)
{
	m_width = a_width;
}

FORCE_INLINE uint32_t TextureImage::get_height() const
{
	return m_height;
}

FORCE_INLINE void TextureImage::set_height(uint32_t &a_height)
{
	m_height = a_height;
}

FORCE_INLINE rUsChar **TextureImage::get_pixel_data() const
{
	return m_pixel_data;
}

FORCE_INLINE void TextureImage::set_pixel_data(rUsChar **a_pixel_data)
{
	m_pixel_data = a_pixel_data;
}

}        // namespace ror
