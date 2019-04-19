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

#include "rorboids.h"
#include "utility/rorutility.h"

namespace game
{
/************************************************************************/
/*                                                                      */
/************************************************************************/
Boid::Boid()
{
	m_position.x = ror::Utility::rand(-10.0f, 10.0f);
	m_position.y = ror::Utility::rand(0.0f, 10.0f);
	m_position.z = ror::Utility::rand(-10.0f, 10.0f);
	m_color.x    = ror::Utility::rand(0.0f, 1.0f);
	m_color.y    = ror::Utility::rand(0.0f, 1.0f);
	m_color.z    = ror::Utility::rand(0.0f, 1.0f);

	m_velocity.x = ror::Utility::rand(-1.0f, 1.0f);
	m_velocity.y = ror::Utility::rand(-1.0f, 1.0f);
	m_velocity.z = ror::Utility::rand(-1.0f, 1.0f);

	m_velocity.x = 0.0f;
	m_velocity.y = 0.0f;
	m_velocity.z = 0.0f;

	m_position.x = 20.0f;
	/*
		m_Position.y = 0.0f;
		m_Position.z = 0.0f;
		*/
	m_perching       = false;
	m_perching_timer = (int32_t) ror::Utility::rand(0.0f, 50.0f);

	m_model = ror::ccbamanager::get_instance()->get_model("D:/ColladaSamples/Assets/Models/Hawk.CBA");
	//assert(m_Model != nullptr);
}

Boid::Boid(float32_t a_x, float32_t a_y, float32_t a_z)
{
	m_position.x = a_x;
	m_position.y = a_y;
	m_position.z = a_z;

	m_color.x = ror::Utility::rand(0.0f, 1.0f);
	m_color.y = ror::Utility::rand(0.0f, 1.0f);
	m_color.z = ror::Utility::rand(0.0f, 1.0f);

	m_velocity.x = ror::Utility::rand(-1.0f, 1.0f);
	;
	m_velocity.y = ror::Utility::rand(-1.0f, 1.0f);
	;
	m_velocity.z = ror::Utility::rand(-1.0f, 1.0f);
	;

	m_velocity.x = 0.0f;
	m_velocity.y = 0.0f;
	m_velocity.z = 0.0f;

	m_perching       = false;
	m_perching_timer = (int32_t) ror::Utility::rand(0.0f, 10.0f);
}

Boids::~Boids()
{
	rDELETEARRAY(m_boids);
}

void Boid::draw()
{
	Vector3f direction = m_velocity;
	Vector3f up(0.0f, -1.0f, 0.0f);
	Vector3f right;
	Matrix4f matrix;

	direction.normalize();
	right.cross_product(direction, up);
	up.cross_product(direction, right);

	matrix.set_forward_vector(direction);
	matrix.set_up_vector(up);
	matrix.set_right_vector(right);

	glPushMatrix();
	glTranslatef(m_position.x, m_position.y, m_position.z);
	glMultMatrixf(matrix.m_values);

	if (nullptr != this->m_model)
		this->m_model->draw();
	else
		draw_boid();

	glPopMatrix();
}

void Boid::draw_boid()
{
	glColor3f(m_color.x, m_color.y, m_color.z);
	glPushMatrix();
	glScalef(5, 5, 5);
	glBegin(GL_QUADS);
	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, 0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, 0.1f);

	glVertex3f(-0.1f, 0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, -0.1f);

	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, 0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, 0.1f);

	glVertex3f(0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, -0.1f);

	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, 0.1f, 0.1f);

	glVertex3f(0.1f, 0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, -0.1f);
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, -0.1f);
	glEnd();
	glPopMatrix();
}

void Boid::update()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
Boids::Boids()
{
	m_target_point = Vector3f(-5.0f, 4.0f, 0.0f);
	m_first_update = true;
}

void Boids::draw_boids()
{
	for (int32_t i = 0; i < NUMBEROFBOIDS; i++)
	{
		m_boids[i].draw();
	}
}

void Boids::update_boids()
{
	if (this->m_first_update)
	{
		this->m_first_update = false;
		m_boids              = rNew Boid[NUMBEROFBOIDS];
		return;
	}
	Vector3f  v1, v2, v3, v4, v5;
	Vector3f  wind;
	float32_t m1, m2, m3, m4, m5;

	v1 = v2 = v3 = v4 = v5 = Vector3f(0.0f, 0.0f, 0.0f);
	m1 = m2 = m3 = m4 = m5 = 1.0f;
	//m5 = -1.0f;
	wind = Vector3f(-0.01f, 0.0f, 0.0f);

	for (int32_t index = 0; index < NUMBEROFBOIDS; index++)
	{
		v1 = boids_tendency_to_center_of_mass(index) * m1;
		v2 = boids_keep_small_distance(index) * m2;
		v3 = boids_tendency_to_match_velocity(index) * m3;
		v4 = boids_limit_position(index) * m4;
		//v5 = BoidsTendencyToParticularPoint(index) * m5;

		if (m_boids[index].m_perching)
		{
			if (m_boids[index].m_perching_timer > 0)
			{
				m_boids[index].m_perching_timer--;
				continue;
			}
			else
				m_boids[index].m_perching = false;
		}

		m_boids[index].m_velocity = m_boids[index].m_velocity + v1 + v2 + v3 + v4 + v5 + wind;
		boids_limit_velocity(index);
		m_boids[index].m_position = m_boids[index].m_position + m_boids[index].m_velocity;
	}
}

Vector3f Boids::boids_tendency_to_center_of_mass(int32_t a_index)
{
	Vector3f pc_j(0.0f, 0.0f, 0.0f);
	for (int32_t index = 0; index < NUMBEROFBOIDS; index++)
	{
		if (index != a_index)
		{
			pc_j += m_boids[index].m_position;
		}
	}
	pc_j = (pc_j / (NUMBEROFBOIDS - 1));

	return (pc_j - m_boids[a_index].m_position) / 100.0f;
}

Vector3f Boids::boids_keep_small_distance(int32_t a_index)
{
	Vector3f center(0.0f, 0.0f, 0.0f);
	for (int32_t index = 0; index < NUMBEROFBOIDS; index++)
	{
		if (index != a_index)
		{
			float32_t distance1 = m_boids[index].m_position.distance(m_boids[a_index].m_position);
			if (distance1 < 2.0f)
			{
				center = center - (m_boids[index].m_position - m_boids[a_index].m_position);
			}
		}
	}
	return center;
}

Vector3f Boids::boids_tendency_to_match_velocity(int32_t a_index)
{
	Vector3f pc_j(0.0f, 0.0f, 0.0f);
	;
	for (int32_t index = 0; index < NUMBEROFBOIDS; index++)
	{
		if (index != a_index)
		{
			pc_j += m_boids[index].m_velocity;
		}
	}
	pc_j = (pc_j / (NUMBEROFBOIDS - 1));
	return (pc_j - m_boids[a_index].m_velocity) / 1.0f;
}

Vector3f Boids::boids_limit_position(int32_t a_index)
{
	float32_t xmin, xmax, ymin, ymax, zmin, zmax, ground_level;

	xmin = ymin = zmin = -200.0f;
	xmax = ymax = zmax = 200.0f;

	ymin         = 0.001f;
	ground_level = -2;

	float32_t arbitrary_amount = 10.0f;
	Vector3f  v(0.0f, 0.0f, 0.0f);

	if (m_boids[a_index].m_position.x < xmin)
		v.x = arbitrary_amount;
	else if (m_boids[a_index].m_position.x > xmax)
		v.x = -arbitrary_amount;

	if (m_boids[a_index].m_position.y < ymin)
		v.y = arbitrary_amount;
	else if (m_boids[a_index].m_position.y > ymax)
		v.y = -arbitrary_amount;

	if (m_boids[a_index].m_position.z < zmin)
		v.z = arbitrary_amount;
	else if (m_boids[a_index].m_position.z > zmax)
		v.z = -arbitrary_amount;

	if (m_boids[a_index].m_position.y < ground_level)
	{
		m_boids[a_index].m_position.y     = ground_level;
		m_boids[a_index].m_perching       = true;
		m_boids[a_index].m_perching_timer = ror::Utility::rand_int(0, 10);
	}
	return v;
}

void Boids::boids_limit_velocity(int32_t a_index)
{
	float32_t limit_range = 0.2f;

	float32_t vector_magnitude = m_boids[a_index].m_velocity.length();
	if (vector_magnitude > limit_range)
	{
		m_boids[a_index].m_velocity = (m_boids[a_index].m_velocity / vector_magnitude) * limit_range;
	}
}

Vector3f Boids::boids_tendency_to_particular_point(int32_t a_index)
{
	return (m_target_point - m_boids[a_index].m_position) / 100;        // 0.01 Percent
}

}        // namespace game
