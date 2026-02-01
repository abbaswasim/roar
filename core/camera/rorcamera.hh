// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

#include "camera/rorcamera.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorvector3.hpp"

namespace ror
{
void OrbitCamera::bounds(float32_t a_width, float32_t a_height)
{
	this->m_width  = a_width;
	this->m_height = a_height;

	this->update_projection();
	this->update_view();
}

void OrbitCamera::set_from_parameters()
{
	this->update_projection();
	this->update_view(this->m_up);
}

void OrbitCamera::set_far_to_scene(ror::BoundingBoxf a_scene_bounds)
{
	auto scene_bbox_points_ws = a_scene_bounds.corners();        // Initially in world space but converted to ls in next line

	auto far = 0.0f;
	for (auto &scene_bbox_point : scene_bbox_points_ws)
	{
		scene_bbox_point = this->m_view * scene_bbox_point;
		far              = std::min(far, scene_bbox_point.z);
	}

	this->m_z_far = -far;        // FIXME: reverse-z consideration

	this->update_projection();
	this->update_view(this->m_up);
}

void OrbitCamera::zoom(double64_t a_zoom_delta)
{
	auto delta = (camera_sensitivity * 10.0f * static_cast<float32_t>(a_zoom_delta));

	this->m_y_fov += delta;

	if (this->m_y_fov < 1.0f)
		this->m_y_fov = 1.0f;
	else if (this->m_y_fov > 100.0f)
		this->m_y_fov = 100.0f;

	this->update_projection();
}

void OrbitCamera::forward(double64_t a_zoom_delta)
{
	auto delta = (camera_sensitivity * 10.0f * static_cast<float32_t>(a_zoom_delta));

	auto translation = ror::matrix4_translation(this->m_forward * delta);
	this->m_view *= translation;

	translation = ror::matrix4_translation(this->m_forward * -delta);
	this->m_eye = translation * this->m_eye;

	this->m_x_mag += delta;
	this->m_y_mag += delta;

	this->update_projection();
	this->update_vectors();
}

void OrbitCamera::update_position_function(double64_t &a_x_new_position, double64_t &a_y_new_position)
{
	this->m_x_position = static_cast<float>(a_x_new_position);
	this->m_y_position = static_cast<float>(a_y_new_position);
}

void OrbitCamera::left_key_drag(double64_t &a_x_new_position, double64_t &a_y_new_position, float32_t a_scale)
{
	auto x_delta = this->m_x_position - a_x_new_position;
	auto y_delta = this->m_y_position - a_y_new_position;

	this->m_x_position = static_cast<float>(a_x_new_position);
	this->m_y_position = static_cast<float>(a_y_new_position);

	this->rotate(static_cast<float32_t>(x_delta * a_scale), static_cast<float32_t>(y_delta * a_scale));
}

void OrbitCamera::right_key_drag(double64_t &a_x_new_position, double64_t &a_y_new_position, float32_t a_scale)
{
	auto x_delta = this->m_x_position - a_x_new_position;
	auto y_delta = this->m_y_position - a_y_new_position;

	this->m_x_position = static_cast<float>(a_x_new_position);
	this->m_y_position = static_cast<float>(a_y_new_position);

	auto delta_x = this->m_right * (camera_sensitivity * static_cast<float32_t>(-x_delta * a_scale));
	auto delta_y = this->m_up * (camera_sensitivity * static_cast<float32_t>(y_delta * a_scale));

	auto translation = ror::matrix4_translation(delta_x + delta_y);
	this->m_view *= translation;

	translation = ror::matrix4_translation(-delta_x - delta_y);
	this->m_eye = translation * this->m_eye;

	translation    = ror::matrix4_translation(-delta_x - delta_y);
	this->m_center = translation * this->m_center;

	this->update_vectors();
}

void OrbitCamera::middle_key_drag(double64_t &a_x_new_position, double64_t &a_y_new_position, float32_t a_scale)
{
	(void) a_x_new_position;
	(void) a_y_new_position;
	(void) a_scale;
}

void OrbitCamera::volume(Vector3f a_minimum, Vector3f a_maximum)
{
	this->m_minimum = a_minimum;
	this->m_maximum = a_maximum;

	this->setup();
}

void OrbitCamera::center(Vector3f a_center)
{
	this->m_center = a_center;

	this->setup();
}

void OrbitCamera::up(Vector3f a_up)
{
	this->m_up = a_up;

	this->setup();
}

void OrbitCamera::right(Vector3f a_right)
{
	this->m_right = a_right;

	this->setup();
}

void OrbitCamera::forward(Vector3f a_forward)
{
	this->m_forward = a_forward;

	this->setup();
}

void OrbitCamera::eye(Vector3f a_eye)
{
	this->m_eye = a_eye;

	this->setup();
}

void OrbitCamera::target(Vector3f a_target)
{
	this->m_target = a_target;

	this->setup();
}

void OrbitCamera::mode(CameraMode a_mode)
{
	this->m_mode = a_mode;
}

void Camera::type(CameraType a_type)
{
	this->m_type = a_type;
}

void OrbitCamera::type(CameraType a_type)
{
	Camera::type(a_type);

	this->update_projection();
}

void OrbitCamera::near(float32_t a_near)
{
	this->m_z_near = a_near;

	this->update_projection();
}

void OrbitCamera::far(float32_t a_far)
{
	this->m_z_far = a_far;

	this->update_projection();
}

void OrbitCamera::fov(float32_t a_y_fov)
{
	this->m_y_fov        = a_y_fov;
	this->m_y_fov_target = a_y_fov;

	this->update_projection();
}

void OrbitCamera::ratio(float32_t a_aspect_ratio)
{
	this->m_aspect_ratio = a_aspect_ratio;

	this->update_projection();
}

void OrbitCamera::width(float32_t a_width)
{
	this->m_width = a_width;

	this->update_projection();
}

void OrbitCamera::height(float32_t a_height)
{
	this->m_height = a_height;

	this->update_projection();
}

void OrbitCamera::x_mag(float32_t a_x_mag)
{
	this->m_x_mag = a_x_mag;

	this->update_projection();
}

void OrbitCamera::y_mag(float32_t a_y_mag)
{
	this->m_y_mag = a_y_mag;

	this->update_projection();
}

void OrbitCamera::y_fov(float32_t a_y_fov)
{
	this->m_y_fov        = a_y_fov;
	this->m_y_fov_target = a_y_fov;

	this->update_projection();
}

void OrbitCamera::from_scene(bool a_from_scene)
{
	this->m_from_scene = a_from_scene;
}

FORCE_INLINE constexpr auto &OrbitCamera::frustum_corners(size_t a_index) const
{
	assert(a_index < cascade_count && "Cascade index out of bound");
	return this->m_frustums[a_index].corners();
}

FORCE_INLINE constexpr auto &OrbitCamera::frustum_bounding_box(size_t a_index) const
{
	assert(a_index < cascade_count && "Cascade index out of bound");
	return this->m_frustums[a_index].box();
}

FORCE_INLINE constexpr auto &OrbitCamera::frustum_center(size_t a_index) const
{
	assert(a_index < cascade_count && "Cascade index out of bound");
	return this->m_frustums[a_index].center();
}

FORCE_INLINE void OrbitCamera::set_parameters(CameraType a_type, float32_t a_width, float32_t a_height,
                                              float32_t a_near, float32_t a_far,
                                              Vector3f a_center, Vector3f a_eye,
                                              Vector3f a_up, Vector3f a_right, Vector3f a_forward,
                                              Vector3f a_minimum, Vector3f a_maximum,
                                              float32_t a_y_fov, float32_t a_x_mag, float32_t a_y_mag)
{
	this->m_type         = a_type;
	this->m_width        = a_width;
	this->m_height       = a_height;
	this->m_z_near       = a_near;
	this->m_z_far        = a_far;
	this->m_center       = a_center;
	this->m_eye          = a_eye;
	this->m_up           = a_up;
	this->m_right        = a_right;
	this->m_forward      = a_forward;
	this->m_minimum      = a_minimum;
	this->m_maximum      = a_maximum;
	this->m_y_fov        = a_y_fov;
	this->m_y_fov_target = a_y_fov;
	this->m_x_mag        = a_x_mag;
	this->m_y_mag        = a_y_mag;
}

}        // namespace ror
