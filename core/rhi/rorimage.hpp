// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2021
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

#include "foundation/rormacros.hpp"

namespace rdr
{
class ROAR_ENGINE_ITEM Image final
{
  public:
	FORCE_INLINE Image()                         = default;                   //! Default constructor
	FORCE_INLINE Image(const Image &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Image(Image &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Image &operator=(const Image &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Image &operator=(Image &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Image() noexcept                          = default;        //! Destructor

	uint32_t width() const;
	uint32_t height() const
	{
		return this->m_height;
	}

	uint32_t bpp() const
	{
		return this->m_bpp;
	}

  protected:
  private:
	uint32_t  m_width{0};
	uint32_t  m_height{0};
	uint32_t  m_bpp{4};
};

static_assert(std::is_trivially_copyable_v<Image>, "Image is not trivially copyable");
static_assert(std::is_standard_layout_v<Image>, "Image is not standard layout");

}        // namespace rdr
