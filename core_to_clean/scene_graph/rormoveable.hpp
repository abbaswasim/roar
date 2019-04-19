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

#include "foundation/rormacros.h"
#include "math/rorvector3.h"

namespace ror
{
// This CMoveable can also be used for static Objects in the world
class ROAR_ENGINE_ITEM Moveable
{
  public:
	// Not Intended to be used on its own
	// Dtor
	virtual ~Moveable(void);

	// Update of the Movable, can be specialized by derived classes
	FORCE_INLINE virtual void update_moveable(float32_t a_delta_time);
	FORCE_INLINE void         draw_local_axis();
	FORCE_INLINE void         draw_world_axis();

	FORCE_INLINE bool get_use_gravity() const;
	FORCE_INLINE void set_use_gravity(bool a_gravity);
	FORCE_INLINE bool get_in_motion() const;
	FORCE_INLINE void set_in_motion(bool a_in_motion);
	FORCE_INLINE float32_t get_time() const;
	FORCE_INLINE void      set_time(float32_t a_time);
	FORCE_INLINE const Vector3f *get_acceleration() const;
	FORCE_INLINE void            set_acceleration(Vector3f &a_acceleration);
	FORCE_INLINE const Vector3f *get_position() const;
	FORCE_INLINE void            set_position(Vector3f &a_position);
	FORCE_INLINE const Vector3f *get_velocity() const;
	FORCE_INLINE void            set_velocity(Vector3f &a_velocity);
	FORCE_INLINE const Vector3f *get_forward_vector() const;
	FORCE_INLINE void            set_forward_vector(Vector3f &a_forward);
	FORCE_INLINE const Vector3f *get_up_vector() const;
	FORCE_INLINE void            set_up_vector(Vector3f &a_up);
	FORCE_INLINE const Vector3f *get_right_vector() const;
	FORCE_INLINE void            set_right_vector(Vector3f &a_right);

  protected:
	// Ctor
	FORCE_INLINE Moveable(void);

	// Data Needed for Movable Object
	Vector3f *m_velocity;            // Velocity of the Object
	Vector3f *m_position;            // Position in the World
	Vector3f *m_acceleration;        // Acceleration/Deceleration

	// TODO : Keep track of the changes in FUR Vectors and update, Create Align
	Vector3f *m_forward_vector;        // Forward Vector of the Object (in Local Coordinates)
	Vector3f *m_up_vector;             // Up Vector of the Object (in Local Coordinates)
	Vector3f *m_right_vector;          // Right Vector of the Object (in Local Coordinates)

	float32_t m_time;        // Is Needed or NOT?????

	bool m_in_motion;          // Handy for Turning On and Off Motion
	bool m_use_gravity;        // Handy for Turning On and Off gravity
};
}        // namespace ror

#include "rormoveable.inl"
