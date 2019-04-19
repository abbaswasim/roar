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
FORCE_INLINE ParticlesNode<_tsize, particle_type, initialize_policy, action_policy>::ParticlesNode(void)
{}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE ParticlesNode<_tsize, particle_type, initialize_policy, action_policy>::~ParticlesNode(void)
{}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE void ParticlesNode<_tsize, particle_type, initialize_policy, action_policy>::global_draw()
{
	this->render();
}

template <uint32_t _tsize, class particle_type, class initialize_policy, class action_policy>
FORCE_INLINE void ParticlesNode<_tsize, particle_type, initialize_policy, action_policy>::global_update(rUsLong a_mili_seconds)
{
	glDisable(GL_LIGHTING);
	this->simulate();
	glEnable(GL_LIGHTING);
}
//	FORCE_INLINE void CParticlesNode::GlobalUpdate(rUsLong a_MilliSeconds)
//	{}
//
//	FORCE_INLINE void CParticlesNode::GlobalPrepair()
//	{}
//
//	FORCE_INLINE void CParticlesNode::GlobalCheckCollision( CEntities *a_OtherEntity )
//	{}
//
}        // namespace ror
