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

#include "foundation/rorcompiler_workarounds.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/vulkan/rorcommand_buffer.hpp"
#include "rhi/vulkan/rorcompute_command_encoder.hpp"
#include "rhi/vulkan/rordevice.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include <cassert>

namespace rhi
{

FORCE_INLINE ComputeCommandEncoder::~ComputeCommandEncoderVulkan() noexcept
{
	if (this->m_command_buffer)
	{
		// this->m_encoder->endEncoding();
		// this->m_encoder->release();
	}
}

FORCE_INLINE ComputeCommandEncoder::ComputeCommandEncoderVulkan(rhi::CommandBufferVulkan &a_encoder) :
    m_command_buffer(a_encoder.platform_compute_command_buffer())
{
	vk_begin_command_buffer(this->m_command_buffer);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::buffer(rhi::BufferHybrid<rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) noexcept
{
	(void) a_buffer;
	(void) a_offset;
	(void) a_index;

	// vkCmdBindVertexBuffers(this->m_command_buffer,0, 0, a_buffer.platform_buffer(), VK_WHOLE_SIZE);
	// this->m_encoder->setBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::buffer(rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) noexcept
{
	(void) a_buffer;
	(void) a_offset;
	(void) a_index;

	// this->m_encoder->setBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::texture(const rhi::TextureImage &a_texture, uint32_t a_index) noexcept
{
	(void) a_texture;
	(void) a_index;
	// this->m_encoder->setTexture(a_texture.platform_handle(), a_index);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) noexcept
{
	(void) a_sampler;
	(void) a_index;
	// this->m_encoder->setSamplerState(a_sampler.platform_handle(), a_index);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::dispatch_threads(ror::Vector3ui a_threads_per_grid, ror::Vector3ui a_threads_per_threadgroup) noexcept
{
	(void) a_threads_per_grid;
	(void) a_threads_per_threadgroup;
	// MTL::Size threads_per_grid        = MTL::Size::Make(a_threads_per_grid.x, a_threads_per_grid.y, a_threads_per_grid.z);
	// MTL::Size threads_per_threadgroup = MTL::Size::Make(a_threads_per_threadgroup.x, a_threads_per_threadgroup.y, a_threads_per_threadgroup.z);

	// this->m_encoder->dispatchThreads(threads_per_grid, threads_per_threadgroup);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::end_encoding() noexcept
{
	assert(this->m_command_buffer != nullptr && "Command buffer isn't valid");
	vk_end_command_buffer(this->m_command_buffer);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::release() noexcept
{
	// Need to be done some other way for vulkan
	// this->m_encoder->release();
}

FORCE_INLINE constexpr void ComputeCommandEncoder::release() const noexcept
{
	// Need to be done some other way for vulkan
	// this->m_encoder->release();
}

}        // namespace rhi
