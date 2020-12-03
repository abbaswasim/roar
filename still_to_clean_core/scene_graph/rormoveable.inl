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
FORCE_INLINE Moveable::Moveable(void)
{
	this->m_velocity     = rNew     Vector3f(/*0.0f,0.0f,0.0f*/);
	this->m_position     = rNew     Vector3f();
	this->m_acceleration = rNew Vector3f();

	this->m_forward_vector = rNew Vector3f();
	this->m_up_vector      = rNew      Vector3f();
	this->m_right_vector   = rNew   Vector3f();

	this->m_in_motion   = false;
	this->m_use_gravity = false;
	this->m_time        = 0.0f;
}

FORCE_INLINE void Moveable::update_moveable(float32_t a_delta_time)
{
	if (this->m_in_motion)
	{
		*this->m_velocity += *this->m_acceleration * a_delta_time;
		*this->m_position += *this->m_velocity * a_delta_time;
	}
}

FORCE_INLINE bool Moveable::get_use_gravity() const
{
	return this->m_use_gravity;
}

FORCE_INLINE void Moveable::set_use_gravity(bool a_gravity)
{
	this->m_use_gravity = a_gravity;

	if (this->m_use_gravity)
		this->m_acceleration->y = -9.80f;
	else
		this->m_acceleration->y = 0.0f;
}

FORCE_INLINE bool Moveable::get_in_motion() const
{
	return this->m_in_motion;
}

FORCE_INLINE void Moveable::set_in_motion(bool a_in_motion)
{
	this->m_in_motion = a_in_motion;
}

FORCE_INLINE float32_t Moveable::get_time() const
{
	return this->m_time;
}

FORCE_INLINE void Moveable::set_time(float32_t a_time)
{
	this->m_time = a_time;
}

FORCE_INLINE const Vector3f *Moveable::get_acceleration() const
{
	return this->m_acceleration;
}

FORCE_INLINE void Moveable::set_acceleration(Vector3f &a_acceleration)
{
	*this->m_acceleration = a_acceleration;
}

FORCE_INLINE const Vector3f *Moveable::get_position() const
{
	return this->m_position;
}

FORCE_INLINE void Moveable::set_position(Vector3f &a_position)
{
	*this->m_position = a_position;
}

FORCE_INLINE const Vector3f *Moveable::get_velocity() const
{
	return this->m_velocity;
}

FORCE_INLINE void Moveable::set_velocity(Vector3f &a_velocity)
{
	*this->m_velocity = a_velocity;
}

FORCE_INLINE const Vector3f *Moveable::get_forward_vector() const
{
	return this->m_forward_vector;
}

FORCE_INLINE void Moveable::set_forward_vector(Vector3f &a_forward)
{
	*this->m_forward_vector = a_forward;
}

FORCE_INLINE const Vector3f *Moveable::get_up_vector() const
{
	return this->m_up_vector;
}

FORCE_INLINE void Moveable::set_up_vector(Vector3f &a_up)
{
	*this->m_up_vector = a_up;
}

FORCE_INLINE const Vector3f *Moveable::get_right_vector() const
{
	return this->m_right_vector;
}

FORCE_INLINE void Moveable::set_right_vector(Vector3f &a_right)
{
	*this->m_right_vector = a_right;
}

FORCE_INLINE void Moveable::draw_local_axis()
{
#ifdef OPENGL_RENDERER

	static Vector3f right, up, forward, origion;
	float32_t       scale = 2.0;

	right   = *this->m_right_vector * scale;
	up      = *this->m_up_vector * scale;
	forward = *this->m_forward_vector * scale;

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_COLOR_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);

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
#endif
}

FORCE_INLINE void Moveable::draw_world_axis()
{
#ifdef OPENGL_RENDERER

	static Vector3f right, up, forward;
	float32_t       scale = 1.0;

	right   = *this->m_right_vector * scale;
	up      = *this->m_up_vector * scale;
	forward = *this->m_forward_vector * scale;

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_COLOR_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	this->m_position->call_gl_vertex();
	(*this->m_position + right).call_gl_vertex();

	glColor3f(0.0f, 1.0f, 0.0f);
	this->m_position->call_gl_vertex();
	(*this->m_position + up).call_gl_vertex();

	glColor3f(0.0f, 0.0f, 1.0f);
	this->m_position->call_gl_vertex();
	(*this->m_position + forward).call_gl_vertex();

	glEnd();
	glPopMatrix();
	glPopAttrib();
#endif
}

}        // namespace ror
