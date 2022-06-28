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

namespace ror
{
void OrbitCamera::look_at()
{
	Vector3f diagonal = (this->m_maximum - this->m_minimum);

	this->m_bounding_sphere_radius = diagonal.length() / 2.0f;

	this->m_to   = this->m_minimum + ((this->m_maximum - this->m_minimum) / 2.0f);
	this->m_from = this->m_to;

	this->m_camera_depth = this->m_to.z - (this->m_bounding_sphere_radius / sin(ror::to_radians(this->m_fov / 2)));

	this->m_from.z = static_cast<float32_t>(-(this->m_camera_depth + this->m_zooming_depth));        // Truncation happening here, make sure it doesn't matter

	this->m_view = ror::make_look_at(this->m_from, this->m_to, this->m_up);

	this->m_projection = ror::make_perspective(ror::to_radians(this->m_fov), static_cast<float>(this->m_width) / static_cast<float>(this->m_height), 0.1f, 1000.0f);
	// this->m_projection.perspective(this->m_fov, 1.0f, 0.1f, 1000.0f);

	// Make infinite projections matrix
	float a_z_near = 0.1f;
	float epsilon  = 0.00000024f;

	this->m_projection.m_values[10] = epsilon - 1.0f;
	this->m_projection.m_values[14] = (epsilon - 2.0f) * a_z_near;

	// Apply mouse movement transformation on top
	Matrix4f transformation = identity_matrix4f;
	Matrix4f rotation_x;
	Matrix4f rotation_z;
	Matrix4f translation0;
	Matrix4f translation1;
	Matrix4f translation;
	Vector3f min_to = -this->m_to;

	translation0 = ror::matrix4_translation(min_to);
	rotation_x   = ror::matrix4_rotation_around_x(this->m_x_rotation / 4.0f);
	rotation_z   = ror::matrix4_rotation_around_z(this->m_z_rotation / 4.0f);
	translation1 = ror::matrix4_translation(this->m_to);

	translation = ror::matrix4_translation(this->m_x_translation, this->m_y_translation, 0.0f);

	transformation = (translation1 * rotation_x * rotation_z * translation0);

	this->m_view_projection       = this->m_projection * this->m_view * translation;
	this->m_model                 = transformation;
	this->m_model_view_projection = this->m_view_projection * this->m_model;

	// Also calculate normal matrix
	// TODO: Create normal matrix for xforming normals from transpose of inverse of model-view (top 3x3) without translations Transpose and Inverse removes non-uniform scale from it
}

void OrbitCamera::set_bounds(int32_t a_width, int32_t a_height)
{
	this->m_width  = a_width;
	this->m_height = a_height;
}

void OrbitCamera::get_bounds(int32_t &a_width, int32_t &a_height)
{
	a_width  = this->m_width;
	a_height = this->m_height;
}

Matrix4f OrbitCamera::get_model()
{
	return this->m_model;
}

Matrix4f OrbitCamera::get_normal()
{
	return this->m_normal;
}

Vector3f OrbitCamera::get_from()
{
	return this->m_from;
}

Matrix4f OrbitCamera::get_view_projection()
{
	return this->m_view_projection;
}

Matrix4f OrbitCamera::get_model_view_projection()
{
	return this->m_model_view_projection;
}

void OrbitCamera::zoom_by(double64_t a_zoom_delta)
{
	this->m_zooming_depth += a_zoom_delta;

	auto abs_camera_depth = abs(this->m_camera_depth);
	abs_camera_depth      = abs_camera_depth - fmax(1.0f, abs_camera_depth * 0.1f);

	if (this->m_zooming_depth > abs_camera_depth)
		this->m_zooming_depth = abs_camera_depth;

	// Now update the MVP and the likes
	this->look_at();
}

void OrbitCamera::left_key_function(double64_t &a_x_delta, double64_t &a_y_delta)
{
	this->m_x_rotation += static_cast<float32_t>(0.05f * static_cast<float32_t>(a_y_delta));
	this->m_z_rotation += static_cast<float32_t>(0.05f * static_cast<float32_t>(a_x_delta));
}

void OrbitCamera::middle_key_function(double64_t &a_x_delta, double64_t &a_y_delta)
{
	this->m_x_translation += static_cast<float32_t>(0.05f * static_cast<float32_t>(a_x_delta));
	this->m_y_translation -= static_cast<float32_t>(0.05f * static_cast<float32_t>(a_y_delta));
}

void OrbitCamera::right_key_function(double64_t &a_x_delta, double64_t &a_y_delta)
{
	(void) a_y_delta;
	this->m_zooming_depth += static_cast<float32_t>(0.05f * static_cast<float32_t>(a_x_delta));

	auto abs_camera_depth = abs(this->m_camera_depth);
	abs_camera_depth      = abs_camera_depth - fmax(1.0f, abs_camera_depth * 0.1f);

	if (this->m_zooming_depth > abs_camera_depth)
		this->m_zooming_depth = abs_camera_depth;
}

void OrbitCamera::set_visual_volume(Vector3f a_minimum, Vector3f a_maximum)
{
	this->m_minimum = a_minimum;
	this->m_maximum = a_maximum;

	this->look_at();
}
}        // namespace ror
