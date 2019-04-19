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

#include "rorparticle_system_manager.h"

namespace ror
{
ParticleSystemManager::~ParticleSystemManager(void)
{
	rDELETE(this->m_particle_systems);
}

ParticleSystemBase *ParticleSystemManager::load_particle_system(std::string a_file_name, ParticleSystemBase::ESystemType a_system_type)
{
	// Before trying to create a new System check if its already in the Systems loaded
	particle_system_objects_map::iterator iter = this->m_particle_systems->find(a_file_name);
	if (iter != this->m_particle_systems->end())
	{
		iter->second->increment_reference_count();
		return iter->second->get_system_pointer();
	}

	// Otherwise we load it from disk
	ParticleSystemBase *temp_system = nullptr;

	switch (a_system_type)
	{
		case ParticleSystemBase::rSMOKE:
			break;
		case ParticleSystemBase::rFOUNTAIN:
			temp_system = rNew fountain_system500();
			temp_system->set_system_type(ParticleSystemBase::rFOUNTAIN);
			break;
		case ParticleSystemBase::rBLOOD:
			break;
		case ParticleSystemBase::rRAIN:
			break;
		case ParticleSystemBase::rFOG:
			break;
		case ParticleSystemBase::rCLOUDS:
			break;
		case ParticleSystemBase::rENGINE_EXHAUST_SMOKE:
			break;
		default:
			break;
	}

	temp_system->load_from_file(a_file_name);
	temp_system->emit_particles(400, Vector3f(0.0f, 0.0f, 0.0f));

	ParticleSystemObject *new_system_object = rNew ParticleSystemObject();
	new_system_object->set_system_pointer(temp_system);

	this->m_particle_systems->insert(std::make_pair(std::string(a_file_name), new_system_object));

	return temp_system;
}

void ParticleSystemManager::release_particle_system(ParticleSystemBase *a_system_pointer)
{
	if (nullptr != a_system_pointer)
	{
		particle_system_objects_map::iterator iter;
		for (iter = this->m_particle_systems->begin(); iter != this->m_particle_systems->end(); ++iter)
		{
			if (a_system_pointer == iter->second->get_system_pointer())
			{
				if (iter->second->decrement_reference_count() == 0)
				{
					rDELETE(a_system_pointer);
					rDELETE(iter->second);
					this->m_particle_systems->erase(iter);
				}
				return;
			}
		}
	}
}

void ParticleSystemManager::release_all_particle_systems()
{
	particle_system_objects_map::iterator iter;        // = m_ParticleSystems->begin();
	for (iter = this->m_particle_systems->begin(); iter != this->m_particle_systems->end(); ++iter)
	{
		ParticleSystemBase *temp_pointer = iter->second->get_system_pointer();
		rDELETE(iter->second);
		//rDELETE (TempPointer); // Can't Delete this one since its deleted in the scene graph
	}
	this->m_particle_systems->clear();
}

}        // namespace ror
