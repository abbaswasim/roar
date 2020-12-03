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

#include "rorentities.h"

namespace ror
{
bool                          ror::Entities::m_registered = false;
Stack<OGLShaderProgram *>     ror::Entities::m_shaders_stack;        // = rNew CStack<COGLShaderProgram*>();
Vector3f                      ror::Entities::m_origin_offset       = g_zero_vector3f;
ror::Entities::ERenderingPass ror::Entities::m_current_render_pass = ror::Entities::rSHADOWING_PASS;

Entities::~Entities(void)
{
	rDELETE(this->m_bound);
	rDELETE(this->m_shaders);
	rDELETE(this->m_tree_bound);
	rDELETE(this->m_world_transformation);
	rDELETE(this->m_local_transformation);

	//		if (nullptr != this->m_ShadersStack)
	//		{
	//			rDELETE (this->m_ShadersStack);
	//		}
}

void Entities::set_current_render_pass(ror::Entities::ERenderingPass a_render_pass)
{
	m_current_render_pass = a_render_pass;
}

ror::Entities::ERenderingPass Entities::get_current_render_pass()
{
	return m_current_render_pass;
}

}        // namespace ror
