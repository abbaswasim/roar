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
class ROAR_ENGINE_ITEM FirstPersonCamera : public ror::CameraNode
{
  public:
	FORCE_INLINE FirstPersonCamera(void);
	virtual ~FirstPersonCamera(void);

	FORCE_INLINE void global_animate_camera(float32_t &a_delta_time){};
	FORCE_INLINE void global_look_at();
	FORCE_INLINE void global_look_at(Vector3f &a_look_at);
	FORCE_INLINE void global_update(int32_t &a_x_delta, int32_t &a_y_delta);

	FORCE_INLINE void global_update_newtonian(rUsLong a_milli_seconds);
	FORCE_INLINE void update(int32_t &a_x_delta, int32_t &a_y_delta);
	FORCE_INLINE void mouse_setup(int32_t &a_x_delta, int32_t &a_y_delta);
	FORCE_INLINE void move(float32_t &a_distance);
	FORCE_INLINE void strafe(float32_t &a_distance);
	FORCE_INLINE void elevate(float32_t &a_distance);

	//		FORCE_INLINE void		MoveLocal(float32_t &a_X, float32_t &a_Y, float32_t &a_Z, float32_t &a_Distance);
	//		FORCE_INLINE void		MoveLocal(CVector3f &a_Direction, float32_t &a_Distance);
	//		FORCE_INLINE void		MoveGlobal(float32_t &a_X, float32_t &a_Y, float32_t &a_Z, float32_t &a_Distance);
	//		FORCE_INLINE void		MoveGlobal(CVector3f &a_Direction, float32_t &a_Distance);

	FORCE_INLINE void rotate(Vector3f &a_axis, float32_t &a_angle);
	FORCE_INLINE void rotate_global(Vector3f &a_axis, float32_t &a_angle);

	//		FORCE_INLINE void		MoveLocal(float32_t &a_X, float32_t &a_Y, float32_t &a_Z, float32_t &a_Distance);
	//		FORCE_INLINE void		MoveLocal(CVector3f &a_Direction, float32_t &a_Distance);
	//		FORCE_INLINE void		MoveGlobal(float32_t &a_X, float32_t &a_Y, float32_t &a_Z, float32_t &a_Distance);
	//		FORCE_INLINE void		MoveGlobal(CVector3f &a_Direction, float32_t &a_Distance);
	//
	//		FORCE_INLINE void		RotateLocal(CVector3f &a_Axis, float32_t &a_Angle);
	//		FORCE_INLINE void		RotateGlobal(CVector3f &a_Axis, float32_t &a_Angle);

  private:
	friend ROAR_ENGINE_ITEM class World;        // Hack
};
}        // namespace ror

#include "rorfirst_person_camera.hh"
