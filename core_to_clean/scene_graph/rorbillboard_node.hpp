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

#include "opengl/rorogl_texture_manager.h"
#include "rorentities.h"

namespace ror
{
class ROAR_ENGINE_ITEM BillboardNode : public ror::Entities
{
  public:
	FORCE_INLINE BillboardNode(void);
	virtual ~BillboardNode(void);

	// Functions for Treating it as Drawable Object of the World
	virtual void global_draw();
	virtual void global_update(rUsLong a_milli_seconds){};
	virtual void global_prepair(){};
	virtual void global_check_collision(Entities *a_other_entity){};
	virtual void global_update_newtonian(rUsLong a_milli_seconds){};

	FORCE_INLINE void set_orientation();
	FORCE_INLINE void load_geometry(std::string a_file_name);
	FORCE_INLINE Matrix4f *get_model_view_matrix() const;
	FORCE_INLINE void      set_model_view_matrix(Matrix4f *a_modelview_matrix);

  protected:
	TextureObject *m_texture_object;
	Matrix4f *     m_rotation_matrix;        // The orientation matrix for Billboard

	static Matrix4f m_model_view_matrix;
};
}        // namespace ror

#include "rorbillboard_node.inl"
