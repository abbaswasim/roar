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
class ROAR_ENGINE_ITEM CCBA;
class ROAR_ENGINE_ITEM IndexCountPair
{
  public:
	FORCE_INLINE IndexCountPair(void);
	~IndexCountPair(void);

	FORCE_INLINE void operator++();

  private:
	friend class ROAR_ENGINE_ITEM CCBA;

	uint32_t m_position_index;         // Positions Index in the pair
	uint32_t m_normal_index;           // Normals Index in the pair
	uint32_t m_tex_coord_index;        // TexCoords Index in the pair
	uint32_t m_count;                  // Count Number of times Occurs
	uint32_t m_index;                  // m_Index is used later where this Set is saved in the Arrays
};
}        // namespace ror

#include "rorindex_count_pair.inl"
