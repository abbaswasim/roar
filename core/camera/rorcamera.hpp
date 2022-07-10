// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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

#include "event_system/rorevent_system.hpp"
#include "foundation/rortypes.hpp"
#include "math/rormatrix4.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorvector4.hpp"
#include <functional>

/*  OrbitCamera usage
 *  After window creation call camera_init();
 *  and set visual volume to limit the camera to a specific bounding box
 *  by calling glfw_camera_visual_volume(min, max);
 *  use glfw_camera_update() to update the viewport and get updated mvp
 */

#define USE_GLFW

namespace ror
{

enum class CameraType
{
	perspective,
	orthographic
};

class ROAR_ENGINE_ITEM OrbitCamera final
{
  public:
	FORCE_INLINE              OrbitCamera()                               = default;        //! Default constructor
	FORCE_INLINE              OrbitCamera(const OrbitCamera &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              OrbitCamera(OrbitCamera &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE OrbitCamera &operator=(const OrbitCamera &a_other)       = default;        //! Copy assignment operator
	FORCE_INLINE OrbitCamera &operator=(OrbitCamera &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE ~OrbitCamera() noexcept                                  = default;        //! Destructor

	FORCE_INLINE void       set_bounds(int32_t a_width, int32_t a_height);
	FORCE_INLINE void       get_bounds(int32_t &a_width, int32_t &a_height);
	FORCE_INLINE void       set_visual_volume(Vector3f a_minimum, Vector3f a_maximum);
	FORCE_INLINE Matrix4f   get_model_view_projection();
	FORCE_INLINE Matrix4f   get_view_projection();
	FORCE_INLINE Matrix4f   get_model();
	FORCE_INLINE Matrix4f   get_normal();
	FORCE_INLINE Vector3f   get_from();
	FORCE_INLINE void       zoom_by(double64_t a_zoom_delta);
	FORCE_INLINE void       type(CameraType a_type);
	FORCE_INLINE CameraType type();
	FORCE_INLINE void       z_near(float32_t a_near);
	FORCE_INLINE float32_t  z_near();
	FORCE_INLINE void       z_far(float32_t a_far);
	FORCE_INLINE float32_t  z_far();
	FORCE_INLINE void       y_fov(float32_t a_far);
	FORCE_INLINE float32_t  y_fov();
	FORCE_INLINE void       aspect_ratio(float32_t a_far);
	FORCE_INLINE float32_t  aspect_ratio();
	FORCE_INLINE void       width(int32_t a_far);
	FORCE_INLINE int32_t    width();
	FORCE_INLINE void       height(int32_t a_far);
	FORCE_INLINE int32_t    height();
	void                    init(EventSystem &a_event_system);
	void                    enable();
	void                    disable();

  private:
	FORCE_INLINE void left_key_function(double64_t &a_x_delta, double64_t &a_y_delta);
	FORCE_INLINE void middle_key_function(double64_t &a_x_delta, double64_t &a_y_delta);
	FORCE_INLINE void right_key_function(double64_t &a_x_delta, double64_t &a_y_delta);

	FORCE_INLINE void look_at();

	Matrix4f                     m_view{};
	Matrix4f                     m_projection{};
	Matrix4f                     m_model_view_projection{};                //! Model view projection matrix
	Matrix4f                     m_view_projection{};                      //! View projection matrix
	Matrix4f                     m_model{};                                //! Model matrix
	Matrix4f                     m_normal{};                               //! Normal matrix
	Vector3f                     m_to{0.0f, 0.0f, 0.0f};                   //! Looking to direction
	Vector3f                     m_from{0.0f, 0.0f, -10.0f};               //! From position
	Vector3f                     m_up{0.0f, 1.0f, 0.0f};                   //! Up vector to orient itself
	Vector3f                     m_minimum{0.0f, 0.0f, 0.0f};              //! Minimum bound of the bounding volume that the camera will always make sure to see fully
	Vector3f                     m_maximum{100.0f, 100.0f, 100.0f};        //! Maximum bound of the bounding volume that the camera will always make sure to see fully
	float32_t                    m_bounding_sphere_radius{100.0f};         //! Could also describe volume as a sphere with a radius
	double64_t                   m_camera_depth{25.0f};                    //! How far does the camera see
	double64_t                   m_zooming_depth{0.0f};                    //! How far does the camera move
	float32_t                    m_x_rotation{0.0f};                       //! Euler angle of rotation around x-axis
	float32_t                    m_z_rotation{0.0f};                       //! Euler angle of rotation around z-axis
	float32_t                    m_x_translation{0.0f};                    //! Translation in x-coordinates
	float32_t                    m_y_translation{0.0f};                    //! Translation in y-coordinates
	float32_t                    m_y_fov{60.0f};                           //! Y-FOV of the camera
	float32_t                    m_z_near{0.1f};                           //! z-near of the camera
	float32_t                    m_z_far{1000.0f};                         //! z-far of the camera
	float32_t                    m_aspect_ratio{1.0f};                     //! Aspect ratio of the camera
	int32_t                      m_width{800};                             //! Width of the rectangle it needs to fill
	int32_t                      m_height{600};                            //! Height of the rectangle it needs to fill
	CameraType                   m_type{CameraType::perspective};          //! Default perspective camera
	EventSystem                 *m_event_system{nullptr};                  //! A non-owning alias of the event system to not have to keep moving this around
	std::function<void(Event &)> m_drag_callback{};                        //! Drag lambda function that will be used to subscribe and unsubscribe this camera with event system
	std::function<void(Event &)> m_resize_callback{};                      //! Resize lambda function that will be used to subscribe and unsubscribe this camera with event system
	std::function<void(Event &)> m_zoom_callback{};                        //! Zoom lambda function that will be used to subscribe and unsubscribe this camera with event system
	std::function<void(Event &)> m_frambuffer_resize_callback{};           //! Framebuffer resize lambda function that will be used to subscribe and unsubscribe this camera with event system
};
}        // namespace ror

#include "rorcamera.hh"
