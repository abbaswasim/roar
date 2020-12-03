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

#pragma once

#include "math/rorvector3.h"
#include "memory_manager/rormemory_check.h"

using namespace ror;

namespace game
{
class ROAR_ENGINE_ITEM Ball
{
  public:
	enum e_direction
	{
		UP,
		DOWN
	};

	Ball(void);
	~Ball(void);
	// Draw Ball at the given (x,y,z) location (Position)
	void     draw_ball(void);
	void     update_ball(float32_t a_delta_time);
	Vector3f get_initial_position() const
	{
		return m_initial_position;
	}
	Vector3f get_fraction() const
	{
		return m_fraction;
	}
	float32_t get_fraction_factor() const
	{
		return m_fraction_factor;
	}
	Vector3f get_current_position() const
	{
		return m_current_position;
	}
	Vector3f get_initial_velocity() const
	{
		return m_initial_velocity;
	}
	Vector3f get_currnet_velocity() const
	{
		return m_current_velocity;
	}
	float32_t get_radius() const
	{
		return m_radius;
	}
	Vector3f get_acceleration() const
	{
		return m_acceleration;
	}
	e_direction get_direction() const
	{
		return m_direction;
	}

	void set_current_position(Vector3f &a_new_value)
	{
		m_current_position = a_new_value;
	}
	void set_initial_position(Vector3f &a_new_value)
	{
		m_initial_position = a_new_value;
	}
	void set_current_velocity(Vector3f &a_new_value)
	{
		m_current_velocity = a_new_value;
	}
	void set_initial_velocity(Vector3f &a_new_value)
	{
		m_initial_velocity = a_new_value;
	}
	void set_acceleration(Vector3f &a_new_value)
	{
		m_acceleration = a_new_value;
	}
	void set_fraction(Vector3f &a_new_value)
	{
		m_fraction = a_new_value;
	}
	void set_fraction_factor(float32_t &a_new_value)
	{
		m_fraction_factor = a_new_value;
	}
	void set_direction(e_direction a_new_value)
	{
		m_direction = a_new_value;
	}

  private:
	// The Initial position of the Ball in the world coordinates at t = 0
	Vector3f m_initial_position;
	// The Current position of the Ball in the world coordinates at t = T
	Vector3f m_current_position;
	// The Initial velocity of the Ball at any instance of time t = 0
	Vector3f m_initial_velocity;
	// The current velocity of the Ball at any instance of time t
	Vector3f m_current_velocity;
	// The Acceleration of the Balls at any instance
	Vector3f m_acceleration;
	// The Fraction vector of the Balls
	Vector3f m_fraction;
	// The radius of the Ball used in Collision detection
	float32_t m_radius;
	// The Frictions controlling factor
	float32_t m_fraction_factor;
	// Enum Direction either UP or Down
	e_direction m_direction;
};

}        // namespace game
