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
#include "utility/rorutility.h"

namespace ror
{
//template<class ParticleType, class ColorPolicy, class VelocityPolicy, class LifePolicy, class PositionPolicy>

template <class particle_type>
class FountainColorAction
{
  public:
	FORCE_INLINE FountainColorAction();
	FORCE_INLINE ~FountainColorAction();

	FORCE_INLINE void prepare_action();
	FORCE_INLINE void operator()(particle_type &m_particle) const;

  private:
};

template <class particle_type>
class FountainVelocityAction
{
  public:
	FORCE_INLINE FountainVelocityAction();
	FORCE_INLINE ~FountainVelocityAction();

	FORCE_INLINE float32_t get_acceleration() const;
	FORCE_INLINE void      set_acceleration(float32_t a_acceleration);

	FORCE_INLINE void prepare_action();
	FORCE_INLINE void operator()(particle_type &m_particle) const;

  private:
	float32_t m_acceleration;
};

template <class particle_type>
class FountainLifeAction
{
  public:
	FORCE_INLINE FountainLifeAction();
	FORCE_INLINE ~FountainLifeAction();

	FORCE_INLINE void prepare_action();
	FORCE_INLINE void operator()(particle_type &m_particle) const;

  private:
};

template <class particle_type>
class FountainPositionAction
{
  public:
	FORCE_INLINE FountainPositionAction();
	FORCE_INLINE ~FountainPositionAction();

	FORCE_INLINE void prepare_action();
	FORCE_INLINE void operator()(particle_type &m_particle) const;

  private:
};

template <class particle_type>
class FountainColorInitialize
{
  public:
	FORCE_INLINE FountainColorInitialize();
	FORCE_INLINE ~FountainColorInitialize();

	FORCE_INLINE void operator()(particle_type &m_particle) const;

  private:
};

template <class particle_type>
class FountainVelocityInitialize
{
  public:
	FORCE_INLINE FountainVelocityInitialize();
	FORCE_INLINE ~FountainVelocityInitialize();

	FORCE_INLINE float32_t get_velocity_jitter() const;
	FORCE_INLINE void      set_velocity_jitter(float32_t a_velocity_jitter);
	FORCE_INLINE void      operator()(particle_type &m_particle) const;

  private:
	float32_t m_velocity_jitter;
};

template <class particle_type>
class FountainLifeInitialize
{
  public:
	FORCE_INLINE FountainLifeInitialize();
	FORCE_INLINE ~FountainLifeInitialize();

	FORCE_INLINE void set_life_range(float32_t a_minium_life, float32_t a_maximum_life);
	FORCE_INLINE void operator()(particle_type &m_particle) const;

  private:
	float32_t m_minimum_life;        // In Seconds
	float32_t m_maximum_life;        // In Seconds
};

template <class particle_type>
class FountainPositionInitialize
{
  public:
	FORCE_INLINE FountainPositionInitialize();
	FORCE_INLINE ~FountainPositionInitialize();

	FORCE_INLINE float32_t get_position_jitter() const;
	FORCE_INLINE void      set_position_jitter(float32_t a_position_jitter);
	FORCE_INLINE void      operator()(particle_type &m_particle) const;

  private:
	float32_t m_position_jitter;        // In world Units
};
}        // namespace ror

#include "rorfountain_particle.hh"
