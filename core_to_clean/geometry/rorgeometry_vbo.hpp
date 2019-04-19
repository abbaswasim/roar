// Roar Source Code
// Wasim Abbas
// http://wazim.i8.com
// Copyright (c) 2009-2010
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Version: 1.0.0 (08 July 2008)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////NOT USED YET////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "memory_manager/rormemory_check.h"
#include "opengl/rorogl_index_buffer.h"
#include "opengl/rorogl_vertex_buffer.h"

namespace ror
{
// This class is not used (Deprecated)
class ROAR_ENGINE_ITEM CCBA;
class ROAR_ENGINE_ITEM GeometryVBO
{
  public:
	FORCE_INLINE GeometryVBO(void);
	~GeometryVBO(void);

	FORCE_INLINE uint32_t get_id() const;
	FORCE_INLINE void     set_id(uint32_t a_id);

  private:
	friend ROAR_ENGINE_ITEM class CCBA;

	uint32_t         m_id;                  // For checking in the Scene Graph
	uint32_t         m_no_of_groups;        // Total number of IBOs in the Group
	OGLVertexBuffer *m_vbo;                 // VBO
	OGLIndexBuffer * m_ibo;                 // IBO
};
}        // namespace ror

#include "rorgeometry_vbo.inl"
