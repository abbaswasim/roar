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

#include "appearance/rorlight.h"
#include "particles/rorparticle_system.h"
#include "scene_graph/rorentities.h"

namespace ror
{
template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
class ROAR_ENGINE_ITEM ParticlesNode : public ror::Entities, public ror::ParticleSystem<_tsize, particle_type, initialize_policy, action_policy>
{
  public:
	FORCE_INLINE ParticlesNode(void);
	FORCE_INLINE ~ParticlesNode(void);

	// Functions for Treating it as Drawable Object of the World
	virtual void global_draw();
	virtual void global_update(rUsLong a_mili_seconds);
	;
	virtual void global_prepair(){};
	virtual void global_check_collision(Entities *a_other_entity){};
	virtual void global_update_newtonian(rUsLong a_milli_seconds){};

  protected:
};

typedef ParticlesNode<1000, Particle,
                      ParticlePolicy<        //Initialize Policy
                          Particle,
                          FountainColorInitialize<Particle>,
                          FountainVelocityInitialize<Particle>,
                          FountainLifeInitialize<Particle>,
                          FountainPositionInitialize<Particle>>,
                      ParticlePolicy<        //Action Policy
                          Particle,
                          FountainColorAction<Particle>,
                          FountainVelocityAction<Particle>,
                          FountainLifeAction<Particle>,
                          FountainPositionAction<Particle>>>
    fountain_system1000;

typedef ParticlesNode<500, Particle,
                      ParticlePolicy<        //Initialize Policy
                          Particle,
                          FountainColorInitialize<Particle>,
                          FountainVelocityInitialize<Particle>,
                          FountainLifeInitialize<Particle>,
                          FountainPositionInitialize<Particle>>,
                      ParticlePolicy<        //Action Policy
                          Particle,
                          FountainColorAction<Particle>,
                          FountainVelocityAction<Particle>,
                          FountainLifeAction<Particle>,
                          FountainPositionAction<Particle>>>
    fountain_system500;
}        // namespace ror

#include "rorparticles_node.inl"
