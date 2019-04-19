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
template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::ParticleSystem(void) :
    m_active_count(0),
    m_time(0.0f),
    m_system_type(rFOUNTAIN),
    m_texture_object(nullptr),
    m_bounding_box(nullptr)
{}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::~ParticleSystem(void)
{}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE void ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::reset_particles()
{
	this->m_active_count = 0;
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE const particle_type *ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::get_particles() const
{
	if (0 == this->m_active_count)
	{
		return nullptr;
	}
	return this->m_particles;
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE const uint32_t ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::maximum_particles() const
{
	return _tsize;
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE const uint32_t ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::active_particles_count() const
{
	return this->m_active_count;
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE TextureObject *ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::get_texture_object() const
{
	return this->m_texture_object;
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE void ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::set_texture_object(TextureObject *a_texture_object)
{
	this->m_texture_object = a_texture_object;
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE BoundingBox *ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::get_bounding_box() const
{
	return this->m_bounding_box;
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE void ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::set_bounding_box(BoundingBox *a_boundin_box)
{
	this->m_bounding_box = a_boundin_box;
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE ParticleSystemBase::ESystemType ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::get_system_type() const
{
	return this->m_system_type;
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE void ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::set_system_type(ParticleSystemBase::ESystemType a_system_type)
{
	this->m_system_type = a_system_type;
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE void ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::emit_particles(const uint32_t &a_amount, const Vector3f &a_origin)
{
	uint32_t amount = a_amount;

	if ((this->m_active_count + amount) > _tsize)
	{
		amount = _tsize - this->m_active_count;
	}

	if (amount > 0)
	{
		uint32_t active_count = this->m_active_count;
		this->m_active_count += amount;
		for (; active_count < this->m_active_count; ++active_count)
		{
			this->m_particles[active_count].m_position = a_origin;
			this->m_initialize_policy(this->m_particles[active_count]);
		}
	}
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE void ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::simulate()
{
	this->m_action_policy.prepare_action();
	//  kill off all dead particles
	for (uint32_t n_cnt = 0; n_cnt < this->m_active_count;)
	{
		this->m_action_policy(this->m_particles[n_cnt]);
		if (this->m_particles[n_cnt].m_life_time <= 0)
		{
			// dead, move last particle to this particle
			//				this->m_Particles[nCnt] = this->m_Particles[this->m_ActiveCount - 1];
			//				// decrease particle count.
			//				--this->m_ActiveCount;
			// Or Re-spawn it
			this->m_particles[n_cnt].m_position = g_zero_vector3f;
			this->m_initialize_policy(this->m_particles[n_cnt]);
			n_cnt++;
		}
		else
		{
			//  move to next particle
			++n_cnt;
		}
	}
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE void ror::ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>::render()
{
	glPointSize(4.0f);
	glBegin(GL_POINTS);
	for (uint32_t i = 0; i < this->m_active_count; i++)
	{
		this->m_particles[i].m_color.call_gl_color();
		//glColor3f(0.1f,0.4f,0.9f);
		this->m_particles[i].m_position.call_gl_vertex();
	}
	glEnd();
	glPointSize(1.0f);
}

}        // namespace ror
