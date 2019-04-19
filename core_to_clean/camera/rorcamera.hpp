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

#include "IO/rorkeyboard.h"
#include "IO/rormouse.h"
#include "rorfrustrum.h"
#include "rorviewport.h"
//#include INCLUDERENDERER

namespace ror
{
class ROAR_ENGINE_ITEM World;
class ROAR_ENGINE_ITEM Camera : public ror::Frustrum, public ror::Viewport
{
  public:
	FORCE_INLINE Camera(void);
	virtual ~Camera(void);

	// Camera Animation Utility function
	FORCE_INLINE void animate_camera(float32_t &a_delta_time);
	FORCE_INLINE void look_at();
	FORCE_INLINE void look_at(Vector3f &a_look_at);
	FORCE_INLINE void update(int32_t &a_x_delta, int32_t &a_y_delta);
	FORCE_INLINE void setup();

	FORCE_INLINE void  update_orientation_vectors();
	FORCE_INLINE void  update_viewport();
	FORCE_INLINE void  update_frustrum(const Matrix4f *a_combined_matrix);
	FORCE_INLINE void  draw_local_axis();
	FORCE_INLINE void  draw_bounding_sphere();
	FORCE_INLINE const Vector3f *get_camera_position() const;
	FORCE_INLINE void            set_camera_position(Vector3f &a_position);

  protected:
	friend ROAR_ENGINE_ITEM class World;        // Hack

	virtual void global_animate_camera(float32_t &a_delta_time)        = 0;
	virtual void global_look_at()                                      = 0;
	virtual void global_look_at(Vector3f &a_look_at)                   = 0;
	virtual void global_update(int32_t &a_x_delta, int32_t &a_y_delta) = 0;

	Matrix4f *m_camera_matrix;        // Camera Matrix Used for the Camera Settings,
	                                  // NOTE: its Different then Camera Position Matrix
	                                  // This can also be generated as Local Matrix instead of Member

	Vector3f *m_camera_position;              // Position Of the Camera
	Vector3f *m_camera_focus_point;           // Focus Point of Camera
	Vector3f *m_camera_forward_vector;        // Direction of Camera
	Vector3f *m_camera_up_vector;             // Up Vector of Camera
	Vector3f *m_camera_right_vector;          // Right Vector of Camera

	static Mouse *   m_mouse;            // Some Mouse Interactions are needed
	static Keyboard *m_key_board;        // Some Keyboard Interactions are needed so use this
};
}        // namespace ror

#include "rorcamera.inl"
