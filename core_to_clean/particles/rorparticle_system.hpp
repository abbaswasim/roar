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

#include "rorparticle_system_base.h"

namespace ror
{
//class ROAR_ENGINE_ITEM CGUIElement;
template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
class ROAR_ENGINE_ITEM ParticleSystem : public ror::ParticleSystemBase
{
  public:
	FORCE_INLINE ParticleSystem(void);
	FORCE_INLINE ~ParticleSystem(void);

	FORCE_INLINE void  simulate();
	FORCE_INLINE void  render();
	FORCE_INLINE void  reset_particles();
	FORCE_INLINE const particle_type *get_particles() const;
	FORCE_INLINE const uint32_t maximum_particles() const;
	FORCE_INLINE const uint32_t active_particles_count() const;
	FORCE_INLINE void           emit_particles(const uint32_t &a_amount, const Vector3f &a_origin);
	FORCE_INLINE TextureObject *get_texture_object() const;
	FORCE_INLINE void           set_texture_object(TextureObject *a_texture_object);
	FORCE_INLINE BoundingBox *get_bounding_box() const;
	FORCE_INLINE void         set_bounding_box(BoundingBox *a_boundin_box);
	FORCE_INLINE ESystemType get_system_type() const;
	FORCE_INLINE void        set_system_type(ESystemType a_system_type);

	void load_from_file(std::string a_file_name)
	{}

  protected:
  private:
	particle_type     m_particles[_tsize];        // Particles
	initialize_policy m_initialize_policy;        // Initializers
	action_policy     m_action_policy;            // Actions
	uint32_t          m_active_count;             // Number of Alive Particles

	float32_t      m_time;                  // Total Time since the simulation of this system started
	ESystemType    m_system_type;           // System Identifier
	TextureObject *m_texture_object;        // A pointer to a texture, CParticleSystem does not take ownership
	BoundingBox *  m_bounding_box;          // The Bounding box reference to a Node BoundingBox
	                                        /*
		BlendMode blendMode //The blend mode you want to use for the particles. Smoke will probably have a different blend mode from Blood
		Array PShape shapes			//A collection of shapes, describing the shapes of the particles. The shape descriptions of the particles usually
									//consist of four positions in 3D camera-space. These four positions are used to draw the two triangles for our
									// particle. As you can see in Table 1, a particle is only stored as a single position, but it requires four positions
									//(vertices) to draw the texture-mapped shape of the particle.
		*/
};
}        // namespace ror

#include "rorparticle_system.inl"
