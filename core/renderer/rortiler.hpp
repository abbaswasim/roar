// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2026
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
#include <array>
#include <cassert>

namespace ror
{

struct Tile
{
	uint32_t m_column{0u};        //! Tile horizonal index
	uint32_t m_row{0u};           //! Tile vertical index
	uint32_t m_width{1u};         //! Tile size in window/screen coordinates (points on Retina)
	uint32_t m_height{1u};        //! Tile size in window/screen coordinates (points on Retina)
	uint32_t m_left;              //! left of the tile in screen pixels
	uint32_t m_right;             //! right of the tile in screen pixels
	uint32_t m_top;               //! top of the tile in screen pixels
	uint32_t m_bottom;            //! bottom of the tile in screen pixels
};

template <size_t _width, size_t _height, size_t _tile_width, size_t _tile_height>
class ROAR_ENGINE_ITEM Tiler final
{
  public:
	FORCE_INLINE        Tiler()                             = default;        //! Default constructor
	FORCE_INLINE        Tiler(const Tiler &a_other)         = default;        //! Copy constructor
	FORCE_INLINE        Tiler(Tiler &&a_other) noexcept     = default;        //! Move constructor
	FORCE_INLINE Tiler &operator=(const Tiler &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Tiler &operator=(Tiler &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Tiler() noexcept                          = default;        //! Destructor

	// clang-format off
	// clang-format on

  protected:
  private:
	std::array<Tile, _width * _height> m_tiles{};        //! All the tiles for the screen
};

Tile get_screen_tile_rectangle(uint32_t a_full_width, uint32_t a_full_height, uint32_t a_tile_width, uint32_t a_tile_height, uint32_t a_tile_col, uint32_t a_tile_row);

}        // namespace ror

#include "rortiler.hh"
