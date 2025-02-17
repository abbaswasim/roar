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
#include "rhi/metal/rorcommand_buffer.hpp"
#include "rhi/metal/rorcompute_command_encoder.hpp"
#include "rhi/metal/rordevice.hpp"
#include "rhi/metal/rormetal_common.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rortypes.hpp"
#include <Metal/MTLTypes.hpp>
#include <cassert>

namespace rhi
{

FORCE_INLINE ComputeCommandEncoder::~ComputeCommandEncoderMetal() noexcept
{
	if (this->m_encoder)
	{
		// this->m_encoder->endEncoding();
		// this->m_encoder->release();
	}
}

FORCE_INLINE ComputeCommandEncoder::ComputeCommandEncoderMetal(MTL::ComputeCommandEncoder *a_encoder) :
    m_encoder(a_encoder)
{}

FORCE_INLINE constexpr void ComputeCommandEncoder::buffer(const rhi::BufferHybrid<rhi::Buffer, rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	this->m_encoder->setBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::buffer(const rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	this->m_encoder->setBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::texture(const rhi::TextureImage &a_texture, uint32_t a_index) const noexcept
{
	this->m_encoder->setTexture(a_texture.platform_handle(), a_index);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) const noexcept
{
	this->m_encoder->setSamplerState(a_sampler.platform_handle(), a_index);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::dispatch_threads(const ror::Vector3ui &a_threads_per_grid, const ror::Vector3ui &a_threads_per_threadgroup) const noexcept
{
	// NOTE: threads_pre_grid are the total number of threads in all dimensions
	// and threads_per_threadgroup is the size of the threadgroup in vulkan terminology
	MTL::Size threads_per_grid        = MTL::Size::Make(a_threads_per_grid.x, a_threads_per_grid.y, a_threads_per_grid.z);
	MTL::Size threads_per_threadgroup = MTL::Size::Make(a_threads_per_threadgroup.x, a_threads_per_threadgroup.y, a_threads_per_threadgroup.z);

	this->m_encoder->dispatchThreads(threads_per_grid, threads_per_threadgroup);
}

FORCE_INLINE constexpr void ComputeCommandEncoder::end_encoding() const noexcept
{
	this->m_encoder->endEncoding();
}

FORCE_INLINE constexpr void ComputeCommandEncoder::release() const noexcept
{
	this->m_encoder->release();
}

}        // namespace rhi
