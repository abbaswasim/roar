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

namespace ror
{
FORCE_INLINE Camera::Camera(void)
{
	m_camera_matrix         = rNew         ror::Matrix4f();
	m_camera_position       = rNew       ror::Vector3f();
	m_camera_focus_point    = rNew    ror::Vector3f();
	m_camera_forward_vector = rNew ror::Vector3f();
	m_camera_up_vector      = rNew      ror::Vector3f();
	m_camera_right_vector   = rNew   ror::Vector3f();
}

FORCE_INLINE void Camera::look_at()
{
	this->global_look_at();
}

FORCE_INLINE void Camera::look_at(Vector3f &a_look_at)
{
	this->global_look_at(a_look_at);
}

FORCE_INLINE void Camera::setup()
{
	static Vector3f temp;
	temp = *this->m_camera_focus_point - *this->m_camera_position;

	this->m_camera_right_vector->cross_product(temp, *this->m_camera_up_vector);
	this->m_camera_right_vector->normalize();

	this->m_camera_matrix->look_at(*this->m_camera_position, *this->m_camera_focus_point, *this->m_camera_up_vector);
}

FORCE_INLINE void Camera::set_camera_position(Vector3f &a_position)
{
	*this->m_camera_position = a_position;
	this->setup();
}

FORCE_INLINE const Vector3f *Camera::get_camera_position() const
{
	return this->m_camera_position;
}

// Obsolete Function
FORCE_INLINE void Camera::update_orientation_vectors()
{
	glGetFloatv(GL_MODELVIEW_MATRIX, this->m_camera_matrix->m_values);
	*this->m_camera_position       = this->m_camera_matrix->translation_vector();
	*this->m_camera_right_vector   = this->m_camera_matrix->right_vector();
	*this->m_camera_up_vector      = this->m_camera_matrix->up_vector();
	*this->m_camera_forward_vector = this->m_camera_matrix->forward_vector();

	*this->m_camera_position *= -1.0f;
}

FORCE_INLINE void Camera::update(int32_t &a_x_delta, int32_t &a_y_delta)
{
	this->global_update(a_x_delta, a_y_delta);
}

FORCE_INLINE void Camera::update_viewport()
{
	glGetIntegerv(GL_VIEWPORT, Viewport::m_values);
}

FORCE_INLINE void Camera::update_frustrum(const Matrix4f *a_combined_matrix)
{
	this->calculate_planes(a_combined_matrix, true);
	this->calculate_bounding_sphere(this->m_camera_forward_vector, this->m_camera_position);
	this->calculate_bounding_cone(this->m_camera_forward_vector, this->m_camera_position, this->m_width / 2, this->m_height / 2);
}

FORCE_INLINE void Camera::draw_local_axis()
{
#ifdef OPENGL_RENDERER

	static Vector3f right, up, forward, origion;
	float32_t       scale = 2.0;

	right   = *m_camera_right_vector * scale;
	up      = *m_camera_up_vector * scale;
	forward = *m_camera_forward_vector * scale;

	glPushMatrix();
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_COLOR_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);

	m_camera_position->call_gl_translate();
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	origion.call_gl_vertex();
	right.call_gl_vertex();

	glColor3f(0.0f, 1.0f, 0.0f);
	origion.call_gl_vertex();
	up.call_gl_vertex();

	glColor3f(0.0f, 0.0f, 1.0f);
	origion.call_gl_vertex();
	forward.call_gl_vertex();

	glEnd();

	glPopAttrib();
	glPopMatrix();
#endif
}

FORCE_INLINE void Camera::draw_bounding_sphere()
{
	glPushMatrix();
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_COLOR_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);

	glScalef(0.1f, 0.1f, 0.1f);
	ror::Utility::draw_sphere(m_frustrum_bounding_sphere->get_center().x, m_frustrum_bounding_sphere->get_center().y,
	                          m_frustrum_bounding_sphere->get_center().z, m_frustrum_bounding_sphere->get_radius(), 8, 8);
	glPopAttrib();
	glPopMatrix();
}

}        // namespace ror
