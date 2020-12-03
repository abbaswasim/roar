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

#include "rorball.h"
#include "rormesh.h"

namespace game
{
Ball::Ball(void) :
    m_radius(0.1f),
    m_fraction_factor(0.00001f)
{
	m_initial_position.x = 0.0f, m_initial_position.y = 0.0f, m_initial_position.z = 0.0f;
	m_current_position   = m_initial_position;
	m_initial_velocity.x = 0.0f, m_initial_velocity.y = 40.0f, m_initial_velocity.z = 0.0f;
	m_acceleration.x = 0.0f, m_acceleration.y = -9.8f, m_acceleration.z = 0.0f;
	m_current_velocity = Vector3f(0.0f, 0.0f, 0.0f);
	// calculate Friction
	Vector3f normalized_velocity = m_current_velocity;
	//		float32_t Mag = vectorMag(NormalizedVelocity);
	normalized_velocity.normalize();

	// fraction is applied to opposite of velocity
	m_fraction  = Vector3f(m_fraction_factor, m_fraction_factor, m_fraction_factor);
	m_direction = UP;
}

Ball::~Ball(void)
{
}

// Draw Ball at the given (x,y,z) location  (Position)
void Ball::draw_ball(void)
{
	glPushMatrix();
	glTranslatef(m_initial_position.x, m_initial_position.y + m_radius, m_initial_position.z);
	//glTranslatef(m_CurrentPosition.x,m_CurrentPosition.y + m_Radius,m_CurrentPosition.z);
	glColor3f(0.0f, 1.0f, 0.0f);
	//glutWireSphere(m_Radius,10,10);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	//auxSolidSphere((double)m_Radius);
	//gluWire
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
}

void Ball::update_ball(float32_t a_delta_time)
{
	float32_t m_time          = a_delta_time;
	Vector3f  acceleration    = m_acceleration;
	Vector3f  initialvelocity = m_initial_velocity;
	//CVector3f currentVelocity	= m_CurrentVelocity;
	Vector3f initialposition = m_initial_position;

	Vector3f new_velocity, new_acceleration, new_position;

	//			NewPosition = Initialposition + (initialvelocity * m_Time) + ((acceleration * m_Time * m_Time) * 0.5f);
	/*
		// Calculate instantaneous velocity and update the velocity
		m_kVelocity += m_acceleration * m_Time;
		// Update the new position of the Projectile
		m_workingPos += m_kVelocity * m_Time;
		*/
	initialvelocity += acceleration * m_time;
	initialposition += initialvelocity * m_time;

	// Check for Peak Point
	//		if (abs((Initialvelocity)) < 0.10)
	//		{
	//			m_Direction = DOWN;
	//		}

	// Apply friction here as well
	Vector3f temp_velo = initialvelocity;
	temp_velo.normalize();

	temp_velo *= m_fraction_factor;

	initialvelocity -= temp_velo;

	//NewPosition = Initialposition + (initialvelocity * m_Time) + ((acceleration * m_Time * m_Time) * 0.5f);
	//acceleration
	if (initialposition.y < 0.0f /* && m_Direction == Entities::CBall::DOWN*/)
	{
		initialvelocity.y  = -initialvelocity.y;
		m_initial_velocity = initialvelocity;
		//m_Time = 0.0f;
		initialposition.y  = 0;
		m_initial_position = initialposition;
	}
	//m_Ball->SetCurrentPosition(NewPosition);
	m_initial_position = initialposition;
	m_initial_velocity = initialvelocity;
}

}        // namespace game
