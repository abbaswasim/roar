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

#pragma once

#if defined(ROR_RENDER_TYPE_VULKAN)
#	include "rhi/vulkan/rorrenderpass.hpp"
#elif defined(ROR_RENDER_TYPE_METAL)
#	include "rhi/metal/rorrenderpass.hpp"
#else
#	error "Unsupported buffer platform"
#endif

namespace rhi
{

const rhi::RenderTarget &subpass_render_target(const rhi::Renderpass &a_renderpass, const rhi::Rendersubpass a_subpass, size_t a_index);

void renderpass_execute(rhi::Renderpass &a_renderpass, rhi::CommandBuffer &a_command_buffer, ror::Scene &a_scene, rhi::Swapchain a_surface,
                        ror::JobSystem &a_job_system, ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
                        rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer);

}        // namespace rhi

#include "rhi/rorrenderpass.hh"
