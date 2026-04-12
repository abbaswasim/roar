// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2025
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

#include "rortiler.hpp"

namespace ror
{

Tile get_screen_tile_rectangle(uint32_t a_full_width, uint32_t a_full_height, uint32_t a_tile_width, uint32_t a_tile_height, uint32_t a_tile_col, uint32_t a_tile_row)
{
	assert(a_full_width != 0 && a_full_height != 0 && "Full dimensions are invalid");
	assert(a_tile_width != 0 && a_tile_height != 0 && "Tile dimensions are invalid");

	const uint32_t cols = (a_full_width + a_tile_width - 1) / a_tile_width;
	const uint32_t rows = (a_full_height + a_tile_height - 1) / a_tile_height;

	a_tile_col = std::min(a_tile_col, cols > 0 ? cols - 1 : 0u);
	a_tile_row = std::min(a_tile_row, rows > 0 ? rows - 1 : 0u);

	const uint32_t x0 = a_tile_col * a_tile_width;
	const uint32_t y0 = a_tile_row * a_tile_height;
	const uint32_t x1 = std::min(x0 + a_tile_width, a_full_width);
	const uint32_t y1 = std::min(y0 + a_tile_height, a_full_height);

	return Tile{a_tile_col, a_tile_row, a_tile_width, a_tile_height, x0, x1, y0, y1};
}

}        // namespace ror
