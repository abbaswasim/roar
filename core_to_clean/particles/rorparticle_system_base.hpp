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

#include "bounds/rorbounding_box.h"
#include "opengl/rorogl_texture_manager.h"
#include "rorfountain_particle.h"
#include "rorparticle.h"
#include "rorparticle_policy.h"

namespace ror
{
class ROAR_ENGINE_ITEM ParticleSystemBase
{
  public:
	enum ESystemType
	{
		rSMOKE,
		rENGINE_EXHAUST_SMOKE,
		rFOUNTAIN,
		rBLOOD,
		rRAIN,
		rFOG,
		rCLOUDS
	};

	FORCE_INLINE ParticleSystemBase(void);
	FORCE_INLINE ~ParticleSystemBase(void);

	virtual void           simulate()                                                         = 0;
	virtual void           render()                                                           = 0;
	virtual void           reset_particles()                                                  = 0;
	virtual const uint32_t maximum_particles() const                                          = 0;
	virtual const uint32_t active_particles_count() const                                     = 0;
	virtual void           emit_particles(const uint32_t &a_amount, const Vector3f &a_origin) = 0;
	virtual TextureObject *get_texture_object() const                                         = 0;
	virtual void           set_texture_object(TextureObject *a_texture_object)                = 0;
	virtual BoundingBox *  get_bounding_box() const                                           = 0;
	virtual void           set_bounding_box(BoundingBox *a_boundin_box)                       = 0;
	virtual void           load_from_file(std::string a_file_name)                            = 0;
	virtual ESystemType    get_system_type() const                                            = 0;
	virtual void           set_system_type(ESystemType a_system_type)                         = 0;

  protected:
};
}        // namespace ror

#include "rorparticle_system_base.inl"
