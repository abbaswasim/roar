// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

#include "core/foundation/rortypes.hpp"
#include <type_traits>

namespace rdr
{
class ROAR_ENGINE_ITEM Texture final
{
  public:
	FORCE_INLINE Texture()                           = default;                   //! Default constructor
	FORCE_INLINE Texture(const Texture &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Texture(Texture &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Texture &operator=(const Texture &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Texture &operator=(Texture &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Texture() noexcept                            = default;        //! Destructor

  protected:
  private:
};

static_assert(std::is_trivially_copyable_v<Texture>, "Texture is not trivially copyable");
static_assert(std::is_standard_layout_v<Texture>, "Texture is not standard layout");

}        // namespace rdr
