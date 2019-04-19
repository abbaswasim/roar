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
template <class particle_type, class color_policy, class velocity_policy, class life_policy, class position_policy>
FORCE_INLINE ParticlePolicy<particle_type, color_policy, velocity_policy, life_policy, position_policy>::ParticlePolicy(void)
{}

template <class particle_type, class color_policy, class velocity_policy, class life_policy, class position_policy>
FORCE_INLINE ParticlePolicy<particle_type, color_policy, velocity_policy, life_policy, position_policy>::~ParticlePolicy(void)
{}

template <class particle_type, class color_policy, class velocity_policy, class life_policy, class position_policy>
FORCE_INLINE void ParticlePolicy<particle_type, color_policy, velocity_policy, life_policy, position_policy>::prepare_action()
{
	this->m_position_policy.prepare_action();
	this->m_velocity_policy.prepare_action();
	this->m_color_policy.prepare_action();
	this->m_life_policy.prepare_action();
}

template <class particle_type, class color_policy, class velocity_policy, class life_policy, class position_policy>
FORCE_INLINE void ParticlePolicy<particle_type, color_policy, velocity_policy, life_policy, position_policy>::operator()(particle_type &m_particle) const
{
	this->m_color_policy(m_particle);
	this->m_velocity_policy(m_particle);
	this->m_life_policy(m_particle);
	this->m_position_policy(m_particle);
}

}        // namespace ror
