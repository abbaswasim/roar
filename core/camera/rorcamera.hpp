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
#include "rorfrustum.hpp"
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

const float32_t camera_sensitivity    = 0.05f;
const float32_t camera_speed          = 1.0f;
const size_t    cascade_count         = 4;
const size_t    frustum_corners_count = 8;

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

class Renderer;

class ROAR_ENGINE_ITEM Camera
{
  public:
	FORCE_INLINE         Camera()                             = default;        //! Default constructor
	FORCE_INLINE         Camera(const Camera &a_other)        = default;        //! Copy constructor
	FORCE_INLINE         Camera(Camera &&a_other) noexcept    = default;        //! Move constructor
	FORCE_INLINE Camera &operator=(const Camera &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Camera &operator=(Camera &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Camera() noexcept                           = default;        //! Destructor

	// clang-format off
	FORCE_INLINE void view(Matrix4f a_view)                  noexcept { this->m_view = a_view;                  }
	FORCE_INLINE void projection(Matrix4f a_projection)      noexcept { this->m_projection = a_projection;      }

	FORCE_INLINE constexpr auto& view()          const noexcept { return this->m_view;          }
	FORCE_INLINE constexpr auto& projection()    const noexcept { return this->m_projection;    }
	FORCE_INLINE constexpr auto& type()          const noexcept { return this->m_type;          }
	// clang-format on

	FORCE_INLINE void type(CameraType a_type);

  private:
  protected:
	Matrix4f   m_view{};                               //! View matrix
	Matrix4f   m_projection{};                         //! Projection matrix
	Matrix4f   m_view_projection{};                    //! View projection matrix
	Matrix4f   m_inverse_projection{};                 //! Inverse of Projection matrix
	Matrix4f   m_inverse_view_projection{};            //! Inverse of View projection matrix
	Matrix3f   m_normal{};                             //! Normal matrix
	CameraType m_type{CameraType::perspective};        //! Default perspective camera
};

class ROAR_ENGINE_ITEM OrbitCamera final : public Camera
{
  public:
	FORCE_INLINE              OrbitCamera()                               = default;        //! Default constructor
	FORCE_INLINE              OrbitCamera(const OrbitCamera &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE              OrbitCamera(OrbitCamera &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE OrbitCamera &operator=(const OrbitCamera &a_other)       = delete;         //! Copy assignment operator
	FORCE_INLINE OrbitCamera &operator=(OrbitCamera &&a_other) noexcept   = delete;         //! Move assignment operator
	FORCE_INLINE ~OrbitCamera() noexcept                                  = default;        //! Destructor

	using Camera::type;

	FORCE_INLINE void set_far_to_scene(ror::BoundingBoxf a_scene_bounds);
	FORCE_INLINE void bounds(float32_t a_width, float32_t a_height);
	FORCE_INLINE void volume(Vector3f a_minimum, Vector3f a_maximum);
	FORCE_INLINE void center(Vector3f a_center);
	FORCE_INLINE void up(Vector3f a_up);
	FORCE_INLINE void right(Vector3f a_right);
	FORCE_INLINE void forward(Vector3f a_forward);
	FORCE_INLINE void eye(Vector3f a_eye);
	FORCE_INLINE void target(Vector3f a_target);
	FORCE_INLINE void zoom(double64_t a_zoom_delta);
	FORCE_INLINE void mode(CameraMode a_mode);
	FORCE_INLINE void type(CameraType a_type);
	FORCE_INLINE void near(float32_t a_near);
	FORCE_INLINE void far(float32_t a_far);
	FORCE_INLINE void fov(float32_t a_far);
	FORCE_INLINE void ratio(float32_t a_far);
	FORCE_INLINE void width(float32_t a_far);
	FORCE_INLINE void height(float32_t a_far);
	FORCE_INLINE void x_mag(float32_t a_x_mag);
	FORCE_INLINE void y_mag(float32_t a_y_mag);
	FORCE_INLINE void y_fov(float32_t a_y_fov);
	FORCE_INLINE void from_scene(bool a_from_scene);
	void              init(EventSystem &a_event_system);
	void              enable();
	void              disable();
	void              update();
	void              upload(const Renderer &a_renderer);
	void              setup_frustums();
	void              orient_top();
	void              orient_bottom();
	void              orient_left();
	void              orient_right();
	void              orient_front();
	void              orient_back();
	void              draw_camera_properties();

	// clang-format off
	FORCE_INLINE constexpr auto& eye()           const noexcept { return this->m_eye;           }
	FORCE_INLINE constexpr auto& up()            const noexcept { return this->m_up;            }
	FORCE_INLINE constexpr auto& right()         const noexcept { return this->m_right;         }
	FORCE_INLINE constexpr auto& forward()       const noexcept { return this->m_forward;       }
	FORCE_INLINE constexpr auto& center()        const noexcept { return this->m_center;        }
	FORCE_INLINE constexpr auto& minimum()       const noexcept { return this->m_minimum;       }
	FORCE_INLINE constexpr auto& maximum()       const noexcept { return this->m_maximum;       }
	FORCE_INLINE constexpr auto& mode()          const noexcept { return this->m_mode;          }
	FORCE_INLINE constexpr auto& x_mag()         const noexcept { return this->m_x_mag;         }
	FORCE_INLINE constexpr auto& y_mag()         const noexcept { return this->m_y_mag;         }
	FORCE_INLINE constexpr auto& y_fov()         const noexcept { return this->m_y_fov;         }
	FORCE_INLINE constexpr auto& z_near()        const noexcept { return this->m_z_near;        }
	FORCE_INLINE constexpr auto& z_far()         const noexcept { return this->m_z_far;         }
	FORCE_INLINE constexpr auto& width()         const noexcept { return this->m_width;         }
	FORCE_INLINE constexpr auto& height()        const noexcept { return this->m_height;        }
	FORCE_INLINE constexpr auto& aspect_ratio()  const noexcept { return this->m_aspect_ratio;  }
	// clang-format on
	FORCE_INLINE constexpr auto &frustum_bounding_box(size_t a_index = 0u) const;
	FORCE_INLINE constexpr auto &frustum_corners(size_t a_index = 0u) const;
	FORCE_INLINE constexpr auto &frustum_center(size_t a_index = 0u) const;
	FORCE_INLINE void            set_parameters(CameraType a_type, float32_t a_width, float32_t a_height,
	                                            float32_t a_near, float32_t a_far,
	                                            Vector3f a_center, Vector3f a_eye,
	                                            Vector3f a_up, Vector3f a_right, Vector3f a_forward,
	                                            Vector3f a_minimum, Vector3f a_maximum,
	                                            float32_t a_y_fov, float32_t a_x_mag, float32_t a_y_mag);

  private:
	void setup();
	void update_vectors();
	void update_view();
	void update_normal();
	void update_projection();
	void rotate(float32_t a_x_rotation, float32_t a_y_rotation);

	FORCE_INLINE void update_position_function(double64_t &a_x_delta, double64_t &a_y_delta);
	FORCE_INLINE void left_key_drag(double64_t &a_x_delta, double64_t &a_y_delta, float32_t a_scale);
	FORCE_INLINE void middle_key_drag(double64_t &a_x_delta, double64_t &a_y_delta, float32_t a_scale);
	FORCE_INLINE void right_key_drag(double64_t &a_x_delta, double64_t &a_y_delta, float32_t a_scale);
	FORCE_INLINE void forward(double64_t a_zoom_delta);

	Vector3f      m_target{0.0f, 0.0f, 0.0f};            //! Target position in worldspace that moves when zooming using forward
	Vector3f      m_center{0.0f, 0.0f, 0.0f};            //! Target position in worldspace at the center of the scene, that never changes
	Vector3f      m_eye{0.0f, 0.0f, 1.0f};               //! Eye position in worldspace // TODO: This should be correctly transformed with what's in the scene file translation/rotation scale etc. Its currently not working
	Vector3f      m_right{1.0f, 0.0f, 0.0f};             //! Right vector in camera's frame of reference
	Vector3f      m_up{0.0f, 1.0f, 0.0f};                //! Up vector in camera's frame of reference
	Vector3f      m_forward{0.0f, 0.0f, -1.0f};          //! Forward vector in camera's frame of reference
	Vector3f      m_minimum{-5.0f, -5.0f, -5.0f};        //! Minimum bound of the bounding volume that the camera will always make sure to see fully
	Vector3f      m_maximum{10.0f, 10.0f, 10.0f};        //! Maximum bound of the bounding volume that the camera will always make sure to see fully
	float32_t     m_x_position{0.0f};                    //! Previous cusor position of the mouse pointer
	float32_t     m_y_position{0.0f};                    //! Previous cusor position of the mouse pointer
	float32_t     m_y_fov{60.0f};                        //! Y-FOV of the camera
	float32_t     m_z_near{0.1f};                        //! z-near of the camera
	float32_t     m_z_far{10.0f};                        //! z-far of the camera
	float32_t     m_aspect_ratio{1.0f};                  //! Aspect ratio of the camera
	float32_t     m_width{1024.0f};                      //! Width of the rectangle it needs to fill
	float32_t     m_height{768.0f};                      //! Height of the rectangle it needs to fill
	float32_t     m_x_mag{1.0f};                         //! Width of the orthographics camera
	float32_t     m_y_mag{1.0f};                         //! Height of the orthographics camera
	CameraMode    m_mode{CameraMode::orbit};             //! Default orbit camera
	bool          m_from_scene{true};                    //! Should the camera view be created from scene or either using default values for target, view and up or provided by scene file
	Frustum       m_frustums[cascade_count];             //! Frustums for all the cascades, max 4 splits
	EventSystem  *m_event_system{nullptr};               //! A non-owning alias of the event system to not have to keep moving this around
	EventCallback m_move_callback{};                     //! Drag lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_drag_callback{};                     //! Drag lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_resize_callback{};                   //! Resize lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_forward_callback{};                  //! forward lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_zoom_callback{};                     //! Zoom lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_frambuffer_resize_callback{};        //! Framebuffer resize lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_camera_mode_callback{};              //! Mode lambda function that will be used to subscribe and unsubscribe this camera with event system
	EventCallback m_frustum_callback{};                  //! Frustum snapshop lambda function that will be used to subscribe and unsubscribe this camera with event system
};
}        // namespace ror

#include "rorcamera.hh"
