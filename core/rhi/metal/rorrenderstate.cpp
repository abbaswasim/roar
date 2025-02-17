// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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

#include "foundation/rormacros.hpp"
#include "rhi/metal/rormetal_common.hpp"
#include "rhi/metal/rorrenderstate.hpp"

#include <Metal/MTLDepthStencil.hpp>

namespace rhi
{
define_translation_unit_vtable(RenderstateDepthMetal)
{}

void RenderstateDepthMetal::upload(const rhi::Device &a_device)
{
	MTL::Device                 *device                   = a_device.platform_device();
	MTL::DepthStencilDescriptor *depth_stencil_descriptor = MTL::DepthStencilDescriptor::alloc()->init();

	depth_stencil_descriptor->setDepthCompareFunction(to_metal_depth_compare_function(this->compare_function()));
	depth_stencil_descriptor->setDepthWriteEnabled(this->depth_write());
	this->m_depth_state = device->newDepthStencilState(depth_stencil_descriptor);

	depth_stencil_descriptor->release();
}

void RenderstateDepthMetal::release()
{
	if (this->m_depth_state)
		this->m_depth_state->release();
}

}        // namespace rhi
