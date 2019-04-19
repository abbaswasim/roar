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
#include "rormaterial.h"
#include "rortriangle.h"

namespace ror
{
class ROAR_ENGINE_ITEM CCBA;
class ROAR_ENGINE_ITEM TriangleGroup
{
  public:
	FORCE_INLINE TriangleGroup(void);
	~TriangleGroup(void);

	FORCE_INLINE Triangle *get_triangles() const;
	FORCE_INLINE void      set_triangles(Triangle *a_triangles);
	FORCE_INLINE Material *get_material() const;
	FORCE_INLINE void      set_material(Material *a_material);
	FORCE_INLINE uint32_t get_triangles_size() const;
	FORCE_INLINE void     set_triangles_size(uint32_t a_no_of_triangles);

  private:
	// Triangle Group should be able to access the data
	friend class ROAR_ENGINE_ITEM CCBA;

	Triangle *m_triangles;
	Material *m_material;
	uint32_t  m_no_of_triangles;
};
}        // namespace ror

#include "rortriangle_group.inl"
