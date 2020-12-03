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
#include "rorbmpfile.h"
#include "rorpcxfile.h"
#include "rortgafile.h"

namespace ror
{
class ROAR_ENGINE_ITEM TextureImage
{
  public:
	FORCE_INLINE TextureImage(void);
	FORCE_INLINE TextureImage(uint32_t a_bpp, uint32_t a_width, uint32_t a_height, rUsChar **a_pixel_data);
	~TextureImage(void);

	FORCE_INLINE uint32_t get_bpp() const;
	FORCE_INLINE void     set_bpp(uint32_t &a_bpp);
	FORCE_INLINE uint32_t get_width() const;
	FORCE_INLINE void     set_width(uint32_t &a_width);
	FORCE_INLINE uint32_t get_height() const;
	FORCE_INLINE void     set_height(uint32_t &a_height);
	FORCE_INLINE rUsChar **get_pixel_data() const;
	FORCE_INLINE void      set_pixel_data(rUsChar **a_pixel_data);

	FORCE_INLINE bool load_image_file(std::string &a_file_name);

  private:
	uint32_t  m_bpp;               // Bits Per Pixel
	uint32_t  m_width;             // Width of the Image in Pixels
	uint32_t  m_height;            // Height of the Image in Pixels
	rUsChar **m_pixel_data;        // Pixels Data
};
}        // namespace ror

#include "rortexture_image.hh"
