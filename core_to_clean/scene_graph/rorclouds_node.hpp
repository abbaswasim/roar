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

#include "rorgeometry_node.h"

namespace ror
{
class ROAR_ENGINE_ITEM CloudsNode : public ror::GeometryNode
{
  public:
	FORCE_INLINE CloudsNode(void);
	virtual ~CloudsNode(void);

	virtual void global_draw();
	virtual void global_update(rUsLong a_mili_seconds);

	FORCE_INLINE float32_t get_scrolling_speed() const;
	FORCE_INLINE void      set_scrolling_speed(float32_t a_scrolling_speed);

  protected:
	Vector3f  m_scrolling_position;
	float32_t m_scrolling_speed;
};
}        // namespace ror

#include "rorclouds_node.inl"
