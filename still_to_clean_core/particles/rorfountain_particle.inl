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
template <class particle_type>
FORCE_INLINE FountainColorAction<particle_type>::FountainColorAction()
{}

template <class particle_type>
FORCE_INLINE FountainColorAction<particle_type>::~FountainColorAction()
{}

template <class particle_type>
FORCE_INLINE void FountainColorAction<particle_type>::prepare_action()
{}

template <class particle_type>
FORCE_INLINE void FountainColorAction<particle_type>::operator()(particle_type &m_particle) const
{
	if (m_particle.m_color.x > 0)
		m_particle.m_color.x -= 0.0001f;
	if (m_particle.m_color.y > 0)
		m_particle.m_color.y -= 0.0001f;
	if (m_particle.m_color.z > 0)
		m_particle.m_color.z -= 0.0001f;
}

template <class particle_type>
FountainVelocityAction<particle_type>::FountainVelocityAction() :
    m_acceleration(0.01f)
{}

template <class particle_type>
FORCE_INLINE FountainVelocityAction<particle_type>::~FountainVelocityAction()
{}

template <class particle_type>
FORCE_INLINE float32_t FountainVelocityAction<particle_type>::get_acceleration() const
{
	return this->m_acceleration;
}

template <class particle_type>
FORCE_INLINE void FountainVelocityAction<particle_type>::set_acceleration(float32_t a_acceleration)
{
	this->m_acceleration = a_acceleration;
}

template <class particle_type>
FORCE_INLINE void FountainVelocityAction<particle_type>::prepare_action()
{}

template <class particle_type>
FORCE_INLINE void FountainVelocityAction<particle_type>::operator()(particle_type &m_particle) const
{
	m_particle.m_velocity.y -= this->m_acceleration;
}

template <class particle_type>
FORCE_INLINE FountainLifeAction<particle_type>::FountainLifeAction()
{}

template <class particle_type>
FORCE_INLINE FountainLifeAction<particle_type>::~FountainLifeAction()
{}

template <class particle_type>
FORCE_INLINE void FountainLifeAction<particle_type>::prepare_action()
{}

template <class particle_type>
FORCE_INLINE void FountainLifeAction<particle_type>::operator()(particle_type &m_particle) const
{
	m_particle.m_life_time -= 0.05f;
}

template <class particle_type>
FORCE_INLINE FountainPositionAction<particle_type>::FountainPositionAction()
{}

template <class particle_type>
FORCE_INLINE FountainPositionAction<particle_type>::~FountainPositionAction()
{}

template <class particle_type>
FORCE_INLINE void FountainPositionAction<particle_type>::prepare_action()
{}

template <class particle_type>
FORCE_INLINE void FountainPositionAction<particle_type>::operator()(particle_type &m_particle) const
{
	m_particle.m_position += m_particle.m_velocity;
}

template <class particle_type>
FORCE_INLINE FountainColorInitialize<particle_type>::FountainColorInitialize()
{}

template <class particle_type>
FORCE_INLINE FountainColorInitialize<particle_type>::~FountainColorInitialize()
{}

template <class particle_type>
FORCE_INLINE void FountainColorInitialize<particle_type>::operator()(particle_type &m_particle) const
{
	m_particle.m_color = Vector3f(
	    Utility::rand(0.0f, 1.0f),
	    Utility::rand(0.0f, 1.0f),
	    Utility::rand(0.0f, 1.0f));
}

template <class particle_type>
FountainVelocityInitialize<particle_type>::FountainVelocityInitialize() :
    m_velocity_jitter(0.5f)
{}

template <class particle_type>
FORCE_INLINE FountainVelocityInitialize<particle_type>::~FountainVelocityInitialize()
{}

template <class particle_type>
FORCE_INLINE float32_t FountainVelocityInitialize<particle_type>::get_velocity_jitter() const
{
	return this->m_velocity_jitter;
}

template <class particle_type>
FORCE_INLINE void FountainVelocityInitialize<particle_type>::set_velocity_jitter(float32_t a_velocity_jitter)
{
	this->m_velocity_jitter = a_velocity_jitter;
}

template <class particle_type>
FORCE_INLINE void FountainVelocityInitialize<particle_type>::operator()(particle_type &m_particle) const
{
	m_particle.m_velocity = Vector3f(
	    Utility::rand(-this->m_velocity_jitter, this->m_velocity_jitter),
	    Utility::rand(0.0f, this->m_velocity_jitter),
	    Utility::rand(-this->m_velocity_jitter, this->m_velocity_jitter));
}

template <class particle_type>
FountainLifeInitialize<particle_type>::FountainLifeInitialize() :
    m_minimum_life(0.2f),
    m_maximum_life(5.0f)
{}

template <class particle_type>
FORCE_INLINE FountainLifeInitialize<particle_type>::~FountainLifeInitialize()
{}

template <class particle_type>
FORCE_INLINE void FountainLifeInitialize<particle_type>::set_life_range(float32_t a_minium_life, float32_t a_maximum_life)
{
	this->m_minimum_life = a_minium_life;
	this->m_maximum_life = a_maximum_life;
}

template <class particle_type>
FORCE_INLINE void FountainLifeInitialize<particle_type>::operator()(particle_type &m_particle) const
{
	m_particle.m_life_time = Utility::rand(this->m_minimum_life, this->m_maximum_life);
}

template <class particle_type>
FountainPositionInitialize<particle_type>::FountainPositionInitialize() :
    m_position_jitter(0.5f)
{}

template <class particle_type>
FORCE_INLINE FountainPositionInitialize<particle_type>::~FountainPositionInitialize()
{}

template <class particle_type>
FORCE_INLINE float32_t FountainPositionInitialize<particle_type>::get_position_jitter() const
{
	return this->m_position_jitter;
}

template <class particle_type>
FORCE_INLINE void FountainPositionInitialize<particle_type>::set_position_jitter(float32_t a_position_jitter)
{
	this->m_position_jitter = a_position_jitter;
}

template <class particle_type>
FORCE_INLINE void FountainPositionInitialize<particle_type>::operator()(particle_type &m_particle) const
{
	m_particle.m_position = Vector3f(
	    Utility::rand(-this->m_position_jitter, this->m_position_jitter),
	    Utility::rand(-this->m_position_jitter, this->m_position_jitter),
	    Utility::rand(-this->m_position_jitter, this->m_position_jitter));
}

}        // namespace ror
