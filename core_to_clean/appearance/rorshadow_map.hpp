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

#include "foundation/rorsingleton.h"
#include "math/rormatrix4.h"
#include "memory_manager/rormemory_check.h"

namespace ror
{
class ROAR_ENGINE_ITEM ShadowMap : public Singleton<ShadowMap>
{
  public:
	FORCE_INLINE uint32_t get_shadow_map_id() const;
	FORCE_INLINE void     set_shadow_map_id(uint32_t a_shadow_map_id);
	FORCE_INLINE uint32_t get_shadow_map_size() const;
	FORCE_INLINE void     set_shadow_map_size(uint32_t a_shadow_map_size);

	FORCE_INLINE void start();
	FORCE_INLINE void setup_pass1();
	FORCE_INLINE void setup_pass2();
	FORCE_INLINE void setup_pass3(Matrix4f &a_projection);
	FORCE_INLINE void end() const;

  private:
	FORCE_INLINE ShadowMap(void);
	~ShadowMap(void);

	// Make CSingleton<CShadowMap> as friend
	friend class Singleton<ShadowMap>;

	uint32_t m_shadow_map_id;          // Texture ID used for Depth Texture
	int32_t  m_view_port[4];           // Viewport Buffer
	uint32_t m_shadow_map_size;        // Shadow Map size

	Matrix4f m_light_projection_matrix;        // Light's Compact Projection Matrix
	Matrix4f m_light_view_matrix;              // Light's Compact Projection Matrix
};
}        // namespace ror

#include "rorshadow_map.hh"
