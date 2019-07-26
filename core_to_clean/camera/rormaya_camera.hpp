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

#include "scene_graph/rorcamera_node.h"

namespace ror
{
class ROAR_ENGINE_ITEM World;
class ROAR_ENGINE_ITEM MayaCamera : public ror::CameraNode
{
  public:
	FORCE_INLINE MayaCamera(void);
	virtual ~MayaCamera(void);

	FORCE_INLINE void global_animate_camera(float32_t &a_delta_time){};
	FORCE_INLINE void global_look_at();
	FORCE_INLINE void global_look_at(Vector3f &a_look_at);
	FORCE_INLINE void global_update(int32_t &a_x_delta, int32_t &a_y_delta);

	FORCE_INLINE void update(int32_t &a_x_delta, int32_t &a_y_delta);

  private:
	friend ROAR_ENGINE_ITEM class World;        // Hack

	float32_t m_z_cam;
	float32_t m_x_rot;
	float32_t m_y_rot;
	float32_t m_x_trans;
	float32_t m_y_trans;
};
}        // namespace ror

#include "rormaya_camera.hh"
