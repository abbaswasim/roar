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

#include "models/rorcba_instance.h"
#include "models/rorcba_manager.h"
#include "rorentities.h"

namespace ror
{
class ROAR_ENGINE_ITEM GeometryNode : public ror::Entities, public ror::ccbainstance
{
  public:
	FORCE_INLINE GeometryNode(void);
	virtual ~GeometryNode(void);

	// Functions for Treating it as Drawable Object of the World
	virtual void global_draw();
	virtual void global_update(rUsLong a_mili_seconds);
	virtual void global_prepair(){};
	virtual void global_check_collision(Entities *a_other_entity){};
	virtual void global_update_newtonian(rUsLong a_milli_seconds);

	// This Function uses Instanced Geometry
	FORCE_INLINE void  set_geometry(ror::CCBA *a_cba);
	FORCE_INLINE const CCBA *get_geometry() const;
	FORCE_INLINE void        setup_texture(std::string *a_texture_file_name = nullptr);

  protected:
	// This Should Never be called Directly
	FORCE_INLINE void load_geometry(std::string a_file_name);

	ror::CCBA *m_geometry_data;

	static OGLTextureManager *m_texture_manager;
	;        // Texture Manager Instance to access the textures
};
}        // namespace ror

#include "rorgeometry_node.hh"
