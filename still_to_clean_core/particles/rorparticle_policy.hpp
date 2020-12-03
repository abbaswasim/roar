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
#include "rorfountain_particle.h"

namespace ror
{
template <class particle_type, class color_policy, class velocity_policy, class life_policy, class position_policy>
class ROAR_ENGINE_ITEM ParticlePolicy
{
  public:
	FORCE_INLINE ParticlePolicy(void);
	FORCE_INLINE ~ParticlePolicy(void);

	FORCE_INLINE void prepare_action();
	FORCE_INLINE void operator()(particle_type &m_particle) const;

  protected:
  private:
	color_policy    m_color_policy;
	velocity_policy m_velocity_policy;
	life_policy     m_life_policy;
	position_policy m_position_policy;
};
}        // namespace ror

#include "rorparticle_policy.hh"
