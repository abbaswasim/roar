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
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "math/rormatrix3.hpp"
#include "math/rormatrix4.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "settings/rorsettings.hpp"
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

const float32_t camera_sensitivity = 0.05f;
const float32_t camera_speed       = 1.0f;

enum class CameraType
{
	perspective,
	orthographic
};

enum class CameraMode
{
	fps,
	orbit
};

class ROAR_ENGINE_ITEM OrbitCamera final
{
  public:
	FORCE_INLINE              OrbitCamera()                               = default;        //! Default constructor
	FORCE_INLINE              OrbitCamera(const OrbitCamera &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE              OrbitCamera(OrbitCamera &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE OrbitCamera &operator=(const OrbitCamera &a_other)       = delete;         //! Copy assignment operator
	FORCE_INLINE OrbitCamera &operator=(OrbitCamera &&a_other) noexcept   = delete;         //! Move assignment operator
	FORCE_INLINE ~OrbitCamera() noexcept                                  = default;        //! Destructor

	FORCE_INLINE void bounds(float32_t a_width, float32_t a_height);
	FORCE_INLINE void volume(Vector3f a_minimum, Vector3f a_maximum);
	FORCE_INLINE void zoom(double64_t a_zoom_delta);
	FORCE_INLINE void mode(CameraMode a_mode);
	FORCE_INLINE void type(CameraType a_type);
	FORCE_INLINE void near(float32_t a_near);
	FORCE_INLINE void far(float32_t a_far);
	FORCE_INLINE void fov(float32_t a_far);
	FORCE_INLINE void ratio(float32_t a_far);
	FORCE_INLINE void width(float32_t a_far);
	FORCE_INLINE void height(float32_t a_far);
	void              init(EventSystem &a_event_system);
	void              enable();
	void              disable();
	void              update();
	void              upload(rhi::Device &a_device);

	// clang-format off
	FORCE_INLINE constexpr auto& shader_buffer() const noexcept { return this->m_shader_buffer; } // TODO: Fix how you do this, just call bind directly on this instead
	FORCE_INLINE constexpr auto& shader_buffer()       noexcept { return this->m_shader_buffer; } // TODO: Fix how you do this, just call bind directly on this iFORCE_INLINE nstead
	FORCE_INLINE constexpr auto& view()          const noexcept { return this->m_view;          }
	FORCE_INLINE constexpr auto& projection()    const noexcept { return this->m_projection;    }
	// clang-format on

  private:
	void fill_shader_buffer();
	void reset();
	void setup();
	void update_vectors();
	void update_view();
	void update_normal();
	void update_projection();
	void rotate(float32_t a_x_rotation, float32_t a_y_rotation);

	FORCE_INLINE void update_position_function(double64_t &a_x_delta, double64_t &a_y_delta);
	FORCE_INLINE void left_key_drag(double64_t &a_x_delta, double64_t &a_y_delta);
	FORCE_INLINE void middle_key_drag(double64_t &a_x_delta, double64_t &a_y_delta);
	FORCE_INLINE void right_key_drag(double64_t &a_x_delta, double64_t &a_y_delta);
	FORCE_INLINE void forward(double64_t a_zoom_delta);

	Matrix4f      m_view{};                                 //! View matrix
	Matrix4f      m_projection{};                           //! Projection matrix
	Matrix4f      m_view_projection{};                      //! View projection matrix
	Matrix4f      m_inverse_projection{};                   //! Inverse of Projection matrix
	Matrix4f      m_inverse_view_projection{};              //! Inverse of View projection matrix
	Matrix3f      m_normal{};                               //! Normal matrix
	Vector3f      m_center{0.0f, 0.0f, 0.0f};               //! Target position in worldspace
	Vector3f      m_eye{0.0f, 0.0f, 1.0f};                  //! Eye position in worldspace
	Vector3f      m_right{1.0f, 0.0f, 0.0f};                //! Right vector in camera's frame of reference
	Vector3f      m_up{0.0f, 1.0f, 0.0f};                   //! Up vector in camera's frame of reference
	Vector3f      m_forward{0.0f, 0.0f, -1.0f};             //! Forward vector in camera's frame of reference
	Vector3f      m_minimum{-50.0f, -50.0f, -50.0f};        //! Minimum bound of the bounding volume that the camera will always make sure to see fully
	Vector3f      m_maximum{50.0f, 50.0f, 50.0f};           //! Maximum bound of the bounding volume that the camera will always make sure to see fully
	float32_t     m_x_position{0.0f};                       //! Previous cusor position of the mouse pointer
	float32_t     m_y_position{0.0f};                       //! Previous cusor position of the mouse pointer
	float32_t     m_y_fov{60.0f};                           //! Y-FOV of the camera
	float32_t     m_z_near{0.1f};                           //! z-near of the camera
	float32_t     m_z_far{1000.0f};                         //! z-far of the camera
	float32_t     m_aspect_ratio{1.0f};                     //! Aspect ratio of the camera
	float32_t     m_width{1024.0f};                         //! Width of the rectangle it needs to fill
	float32_t     m_height{768.0f};                         //! Height of the rectangle it needs to fill
	CameraMode    m_mode{CameraMode::orbit};                //! Default orbit camera
	CameraType    m_type{CameraType::perspective};          //! Default perspective camera
	EventSystem  *m_event_system{nullptr};                  //! A non-owning alias of the event system to not have to keep moving this around
	EventCallback m_move_callback{};                        //! Drag lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_drag_callback{};                        //! Drag lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_resize_callback{};                      //! Resize lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_zoom_callback{};                        //! Zoom lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_frambuffer_resize_callback{};           //! Framebuffer resize lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_mode_callback{};                        //! Mode lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_reset_callback{};                       //! Reset lambda function that will be used to subscribe and unsubscribe this camera with event system

	rhi::ShaderBuffer m_shader_buffer{"per_view_uniforms",
	                                  rhi::ShaderBufferType::ubo,
	                                  rhi::Layout::std140,
	                                  settings().per_view_uniform_set(),
	                                  settings().per_view_uniform_binding()};        //! Shader buffer for per_view_uniform UBO
};
}        // namespace ror

#include "rorcamera.hh"
