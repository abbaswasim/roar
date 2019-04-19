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

#include "math/rorvector3.h"
#include "memory_manager/rormemory_check.h"

namespace rendering
{
class ROAR_ENGINE_ITEM IRenderingDevice;
class ROAR_ENGINE_ITEM RenderingState
{
  public:
	enum shade_mode
	{
		rGL_SMOOTH,
		rGL_FLAT,
		rGL_SHADE_MODEL_MAX
	};
	enum blend_mode
	{
		rGL_ZERO,
		rGL_ONE,
		rGL_ONE_MINUS_SRC_COLOR,
		rGL_ONE_MINUS_DST_COLOR,
		rGL_ONE_MINUS_SRC_ALPHA,
		rGL_ONE_MINUS_DST_ALPHA,
		rGL_SRC_ALPHA,
		rGL_DST_ALPHA,
		rGL_SRC_COLOR,
		rGL_DST_COLOR,
		rGL_SRC_ALPHA_SATURATE,
		rGL_BLEND_MODE_MAX
	};

	enum depth_mode
	{
		rGL_NEVER,
		rGL_LESS,
		rGL_EQUAL,
		rGL_LEQUAL,
		rGL_GREATER,
		rGL_NOTEQUAL,
		rGL_GEQUAL,
		rGL_ALWAYS,
		rGL_DEPTH_MODE_MAX
	};

	enum hint_mode
	{
		rGL_FASTEST,
		rGL_NICEST,
		rGL_DONT_CARE,
		rGL_HINT_MODE_MAX
	};

	enum hint_target
	{
		rGL_FOG_HINT,
		rGL_LINE_SMOOTH_HINT,
		rGL_PERSPECTIVE_CORRECTION_HINT,
		rGL_POINT_SMOOTH_HINT,
		rGL_POLYGON_SMOOTH_HINT,
		rGL_HINT_TARGET_MAX
	};

	enum state
	{
		rGL_ALPHATEST            = 1 << 0,
		rGL_ALPHA_TEST           = 1 << 1,
		rGL_AUTO_NORMAL          = 1 << 2,
		rGL_BLEND                = 1 << 3,
		rGL_COLOR_LOGIC_OP       = 1 << 4,
		rGL_COLOR_MATERIAL       = 1 << 5,
		rGL_CULL_FACE            = 1 << 6,
		rGL_DEPTH_TEST           = 1 << 7,
		rGL_DITHER               = 1 << 8,
		rGL_FOG                  = 1 << 9,
		rGL_LIGHTING             = 1 << 10,
		rGL_LINE_SMOOTH          = 1 << 11,
		rGL_LINE_STIPPLE         = 1 << 12,
		rGL_NORMALIZE            = 1 << 13,
		rGL_POINT_SMOOTH         = 1 << 14,
		rGL_POLYGON_OFFSET_FILL  = 1 << 15,
		rGL_POLYGON_OFFSET_LINE  = 1 << 16,
		rGL_POLYGON_OFFSET_POINT = 1 << 17,
		rGL_POLYGON_SMOOTH       = 1 << 18,
		rGL_POLYGON_STIPPLE      = 1 << 19,
		rGL_SCISSOR_TEST         = 1 << 20,
		rGL_STENCIL_TEST         = 1 << 21,
		rGL_TEXTURE_1D           = 1 << 22,
		rGL_TEXTURE_2D           = 1 << 23,
		rGL_TEXTURE_GEN_Q        = 1 << 24,
		rGL_TEXTURE_GEN_R        = 1 << 25,
		rGL_TEXTURE_GEN_S        = 1 << 26,
		rGL_TEXTURE_GEN_T        = 1 << 27,
		rGL_STATES_MAX
	};

	RenderingState(void);
	~RenderingState(void);

  private:
	// RenderingDevice can access directly
	friend class ROAR_ENGINE_ITEM rendering::IRenderingDevice;

	ror::Color3f m_back_color;              // Background Color
	ror::Color3f m_default_color;           // Current Drawing Color
	float32_t    m_depth_resolution;        // Depth of z-buffer

	shade_mode  m_shade_mode;
	blend_mode  m_source_blend_mode;
	blend_mode  m_distination_blend_mode;
	depth_mode  m_depth_mode;
	hint_mode   m_hint_mode;
	hint_target m_hint_target;
	state       m_renderer_state;

	/*
		glAlphaFunc, glBlendFunc, glClipPlane, glColorMaterial,
		glCullFace,
		glDepthFunc, glDepthRange, glEnableClientState, glFog,
		glGet, glIsEnabled, glLight, glLightModel, glLineWidth,
		glLineStipple, glLogicOp, glMap1, glMap2, glMaterial,
		glNormal, glPointSize, glPolygonMode,	glPolygonOffset,
		glPolygonStipple, glScissor, glStencilFunc, glStencilOp,
		glTexGen, glTexImage1D, glTexImage2D
		*/
};
}        // namespace rendering
