// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
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
#include "rhi/rorcommand_buffer.hpp"
#include "rhi/rorcompute_command_encoder.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"

namespace rhi
{

void compute_dispatch(rhi::CommandBuffer &a_command_buffer, const ror::Vector3ui &a_dispatch_size, const ror::Vector3ui &a_threadroup_size,
                      const rhi::Program &a_compute_pso, const std::vector<const rhi::TextureImage *> &a_images, const std::vector<const rhi::TextureSampler *> &a_samplers, rhi::ShaderBuffer &a_shader_buffer, std::function<void()> a_completion_handler);

// Very heavy weight dispatch that creates its own command buffer and waits untill its completed
void compute_dispatch_and_wait(rhi::Device &a_device, const ror::Vector3ui &a_dispatch_size, const ror::Vector3ui &a_threadroup_size,
                               const rhi::Program &a_compute_pso, const std::vector<const rhi::TextureImage *> &a_images, const std::vector<const rhi::TextureSampler *> &a_samplers, rhi::ShaderBuffer &a_shader_buffer, std::function<void()> a_completion_handler);

// Slightly less heavy weight dispatch which returns the command buffer you need to wait and release later
rhi::CommandBuffer compute_dispatch_create_command_buffer(rhi::Device &a_device, const ror::Vector3ui &a_dispatch_size, const ror::Vector3ui &a_threadroup_size,
                                                          const rhi::Program &a_compute_pso, const std::vector<const rhi::TextureImage *> &a_images, const std::vector<const rhi::TextureSampler *> &a_samplers, rhi::ShaderBuffer &a_shader_buffer, std::function<void()> a_completion_handler);

}        // namespace rhi
