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
class ROAR_ENGINE_ITEM ParticleSystemObject
{
  public:
	FORCE_INLINE ParticleSystemObject(void);
	~ParticleSystemObject(void);

	FORCE_INLINE uint32_t get_reference_count() const;
	FORCE_INLINE void     set_reference_count(uint32_t &a_reference_count);
	FORCE_INLINE uint32_t increment_reference_count();
	FORCE_INLINE uint32_t decrement_reference_count();

	FORCE_INLINE ParticleSystemBase *get_system_pointer() const;
	FORCE_INLINE void                set_system_pointer(ParticleSystemBase *a_system_pointer);

  private:
	uint32_t            m_reference_count;        // How many times this Particle system is Used
	ParticleSystemBase *m_system_pointer;         // Pointer to the data of the system, ParticleSystemObject takes ownership
};
}        // namespace ror

#include "rorparticle_system_object.hh"
