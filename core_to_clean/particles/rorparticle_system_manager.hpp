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

#include "foundation/rorsingleton.h"
#include "rorparticle_system_object.h"
#include "scene_graph/rorparticles_node.h"

namespace ror
{
class ROAR_ENGINE_ITEM ParticleSystemManager : public Singleton<ParticleSystemManager>
{
  public:
	ParticleSystemBase *load_particle_system(std::string a_file_name, ParticleSystemBase::ESystemType a_system_type);
	void                release_particle_system(ParticleSystemBase *a_system_pointer);
	void                release_all_particle_systems();

	FORCE_INLINE ParticleSystemBase *get_particle_system(std::string a_file_name) const;

  private:
	FORCE_INLINE ParticleSystemManager(void);
	~ParticleSystemManager(void);

	typedef std::unordered_map<std::string, ParticleSystemObject *> particle_system_objects_map;

	particle_system_objects_map *m_particle_systems;        // Particle Systems Store

	// Make CSingleton<CParticleSystemManager> as friend
	friend class Singleton<ParticleSystemManager>;
};
}        // namespace ror

#include "rorparticle_system_manager.inl"
