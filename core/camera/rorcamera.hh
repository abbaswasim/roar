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
#include "math/rormatrix4_functions.hpp"
#include "math/rorvector3.hpp"

namespace ror
{
void OrbitCamera::bounds(float32_t a_width, float32_t a_height)
{
	this->m_width  = a_width;
	this->m_height = a_height;
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

	this->m_perspective_z_far = -far;        // FIXME: reverse-z consideration
}

void OrbitCamera::zoom(double64_t a_zoom_delta)
{
	auto delta = (camera_sensitivity * 5.0f * static_cast<float32_t>(a_zoom_delta));

	this->m_y_fov += delta;

	if (this->m_y_fov < 1.0f)
		this->m_y_fov = 1.0f;
	else if (this->m_y_fov > 100.0f)
		this->m_y_fov = 100.0f;
}

void OrbitCamera::forward(double64_t a_zoom_delta)
{
	auto delta = (camera_sensitivity * 5.0f * static_cast<float32_t>(a_zoom_delta));

	auto forward = this->m_target - this->m_eye;

	auto translation = ror::matrix4_translation(forward * -delta);
	this->m_eye      = translation * this->m_eye;
	this->m_target   = translation * this->m_target;

	this->m_x_mag += delta;
	this->m_y_mag += delta;
}

void OrbitCamera::update_position_function(double64_t &a_x_new_position, double64_t &a_y_new_position)
{
	this->m_x_position = static_cast<float>(a_x_new_position);
	this->m_y_position = static_cast<float>(a_y_new_position);
}

void OrbitCamera::left_key_drag(double64_t &a_x_new_position, double64_t &a_y_new_position, EventModifier a_modifier)
{
	auto x_delta = this->m_x_position - a_x_new_position;
	auto y_delta = this->m_y_position - a_y_new_position;

	this->m_x_position = static_cast<float>(a_x_new_position);
	this->m_y_position = static_cast<float>(a_y_new_position);

	this->rotate(static_cast<float32_t>(x_delta), static_cast<float32_t>(y_delta), a_modifier);
}

void OrbitCamera::right_key_drag(double64_t &a_x_new_position, double64_t &a_y_new_position, EventModifier a_modifier)
{
	float scale = 1.0f;

	if (a_modifier == EventModifier::command)
		scale = 0.01f;
	else if (a_modifier == EventModifier::control)
		scale = 2.5f;

	auto x_delta = this->m_x_position - a_x_new_position;
	auto y_delta = this->m_y_position - a_y_new_position;

	auto delta_x = this->m_right * (camera_sensitivity * static_cast<float32_t>(-x_delta * scale));
	auto delta_y = this->m_up * (camera_sensitivity * static_cast<float32_t>(y_delta * scale));

	auto translation = ror::matrix4_translation(-delta_x - delta_y);

	this->m_eye        = translation * this->m_eye;
	this->m_target     = translation * this->m_target;
	this->m_x_position = static_cast<float>(a_x_new_position);
	this->m_y_position = static_cast<float>(a_y_new_position);
}

void OrbitCamera::middle_key_drag(double64_t &a_x_new_position, double64_t &a_y_new_position, EventModifier a_modifier)
{
	(void) a_x_new_position;
	(void) a_y_new_position;
	(void) a_modifier;
}

void OrbitCamera::volume(Vector3f a_minimum, Vector3f a_maximum)
{
	this->m_minimum = a_minimum;
	this->m_maximum = a_maximum;
}

void OrbitCamera::center(Vector3f a_center)
{
	this->m_center = a_center;
}

void OrbitCamera::up(Vector3f a_up)
{
	this->m_up = a_up;
}

void OrbitCamera::right(Vector3f a_right)
{
	this->m_right = a_right;
}

void OrbitCamera::forward(Vector3f a_forward)
{
	this->m_forward = a_forward;
}

void OrbitCamera::eye(Vector3f a_eye)
{
	this->m_eye = a_eye;
}

void OrbitCamera::target(Vector3f a_target)
{
	this->m_target = a_target;
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
}

void OrbitCamera::pespective_near(float32_t a_near)
{
	this->m_perspective_z_near = a_near;
}

void OrbitCamera::perspective_far(float32_t a_far)
{
	this->m_perspective_z_far = a_far;
}

void OrbitCamera::orthographic_near(float32_t a_near)
{
	this->m_orthographic_z_near = a_near;
}

void OrbitCamera::orthographic_far(float32_t a_far)
{
	this->m_orthographic_z_far = a_far;
}

void OrbitCamera::fov(float32_t a_y_fov)
{
	this->m_y_fov = a_y_fov;
}

void OrbitCamera::ratio(float32_t a_aspect_ratio)
{
	this->m_aspect_ratio = a_aspect_ratio;
}

void OrbitCamera::width(float32_t a_width)
{
	this->m_width = a_width;
}

void OrbitCamera::height(float32_t a_height)
{
	this->m_height = a_height;
}

void OrbitCamera::x_mag(float32_t a_x_mag)
{
	this->m_x_mag = a_x_mag;
}

void OrbitCamera::y_mag(float32_t a_y_mag)
{
	this->m_y_mag = a_y_mag;
}

void OrbitCamera::y_fov(float32_t a_y_fov)
{
	this->m_y_fov = a_y_fov;
}

void OrbitCamera::from_scene(bool a_from_scene)
{
	this->m_from_scene = a_from_scene;
}

void OrbitCamera::cascade_lambda(float32_t a_cascade_lambda)
{
	this->m_cascade_lambda = a_cascade_lambda;
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
                                              float32_t a_persp_near, float32_t a_persp_far,
                                              float32_t a_ortho_near, float32_t a_ortho_far,
                                              Vector3f a_center, Vector3f a_eye,
                                              Vector3f a_up, Vector3f a_right, Vector3f a_forward,
                                              Vector3f a_minimum, Vector3f a_maximum,
                                              float32_t a_y_fov, float32_t a_x_mag, float32_t a_y_mag)
{
	this->m_type                = a_type;
	this->m_width               = a_width;
	this->m_height              = a_height;
	this->m_perspective_z_near  = a_persp_near;
	this->m_perspective_z_far   = a_persp_far;
	this->m_orthographic_z_near = a_ortho_near;
	this->m_orthographic_z_far  = a_ortho_far;
	this->m_center              = a_center;
	this->m_eye                 = a_eye;
	this->m_up                  = a_up;
	this->m_right               = a_right;
	this->m_forward             = a_forward;
	this->m_minimum             = a_minimum;
	this->m_maximum             = a_maximum;
	this->m_y_fov               = a_y_fov;
	this->m_x_mag               = a_x_mag;
	this->m_y_mag               = a_y_mag;
}

FORCE_INLINE void OrbitCamera::rotate_at_origin(const ror::Matrix4f &a_rotation_matrix, const ror::Vector3f &a_origin, ror::Vector3f &a_input_output)
{
	auto eye_offset = a_input_output - a_origin;
	eye_offset      = a_rotation_matrix * eye_offset;
	a_input_output  = a_origin + eye_offset;
}

}        // namespace ror
