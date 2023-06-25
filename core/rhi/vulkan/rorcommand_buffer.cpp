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
#include "profiling/rorlog.hpp"
#include "rhi/vulkan/rorcommand_buffer.hpp"
#include "rhi/rorrender_command_encoder.hpp"
#include "rhi/rorcompute_command_encoder.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorrhi_macros.hpp"

namespace rhi
{

FORCE_INLINE rhi::RenderCommandEncoder CommandBuffer::render_encoder(rhi::Renderpass &a_render_pass, uint32_t a_index)
{
	(void) a_render_pass;
	(void) a_index;

	ror::log_critical("Fix me, I shouldnt be using default ctor here {}", __FUNCTION__);

	return rhi::RenderCommandEncoder{};
}

FORCE_INLINE rhi::ComputeCommandEncoder CommandBuffer::compute_encoder(rhi::Renderpass &a_render_pass, uint32_t a_index)
{
	(void) a_render_pass;
	(void) a_index;

	ror::log_critical("Fix me, I shouldnt be using default ctor here {}", __FUNCTION__);
	return rhi::ComputeCommandEncoder{};
}

}        // namespace rhi
