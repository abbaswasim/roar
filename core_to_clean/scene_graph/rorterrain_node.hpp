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

#include "appearance/rortexture_image.h"
#include "rorbillboard_node.h"
#include "rorgeometry_node.h"

#define rWIDTH 512
#define rBREADTH 512

namespace ror
{
class ROAR_ENGINE_ITEM TerrainNode : public ror::GeometryNode
{
  public:
	FORCE_INLINE TerrainNode(void);
	virtual ~TerrainNode(void);

	//		enum TerrainSize
	//		{
	//			rWIDTH = 512,
	//			rBREADTH = 512
	//		};

	// Functions for Treating it as Drawable Object of the World
	virtual void global_draw();
	virtual void global_update(rUsLong a_milli_seconds){};
	virtual void global_prepair(){};
	virtual void global_check_collision(Entities *a_other_entity){};

	// This Should Never be called Directly
	FORCE_INLINE void load_geometry(std::string a_file_name);
	FORCE_INLINE void load_billboards();
	FORCE_INLINE void draw_billboards();
	FORCE_INLINE void draw_billy();
	// This Function uses Instanced Geometry
	FORCE_INLINE void set_geometry(ror::CCBA *a_cba);

  protected:
	Vector3f            m_has_grass[rWIDTH][rBREADTH];
	ror::BillboardNode *m_billboard;
};
}        // namespace ror

#include "rorterrain_node.inl"
