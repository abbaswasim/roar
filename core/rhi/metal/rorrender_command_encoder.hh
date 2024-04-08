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
#include "rhi/metal/rorbuffer.hpp"
#include "rhi/metal/rordevice.hpp"
#include "rhi/metal/rormetal_common.hpp"
#include "rhi/metal/rorrender_command_encoder.hpp"
#include "rhi/rortypes.hpp"
#include <cassert>

namespace rhi
{

FORCE_INLINE RenderCommandEncoder::~RenderCommandEncoderMetal() noexcept
{
	if (this->m_encoder)
	{
		// this->m_encoder->endEncoding();
		// this->m_encoder->release();
	}
}

FORCE_INLINE RenderCommandEncoder::RenderCommandEncoderMetal(MTL::RenderCommandEncoder *a_encoder) :
    m_encoder(a_encoder)
{}

FORCE_INLINE constexpr void RenderCommandEncoder::viewport(ror::Vector4d a_viewport_rectangle, ror::Vector2d a_near_far) const noexcept
{
	this->m_encoder->setViewport(MTL::Viewport{a_viewport_rectangle.x, a_viewport_rectangle.y, a_viewport_rectangle.z, a_viewport_rectangle.w, a_near_far.x, a_near_far.y});
}

FORCE_INLINE constexpr void RenderCommandEncoder::scissor(ror::Vector4ui a_scissor_rectangle) const noexcept
{
	this->m_encoder->setScissorRect(MTL::ScissorRect{a_scissor_rectangle.x, a_scissor_rectangle.y, a_scissor_rectangle.z, a_scissor_rectangle.w});
}

FORCE_INLINE constexpr void RenderCommandEncoder::front_facing_winding(rhi::PrimitiveWinding a_winding) const noexcept
{
	this->m_encoder->setFrontFacingWinding(to_metal_winding(a_winding));
}

FORCE_INLINE constexpr void RenderCommandEncoder::depth_stencil_state(const rhi::RenderstateDepth &a_depth_stencil) const noexcept
{
	this->m_encoder->setDepthStencilState(a_depth_stencil.depth_state());
}

FORCE_INLINE constexpr void RenderCommandEncoder::cull_mode(rhi::PrimitiveCullMode a_cull_mode) const noexcept
{
	this->m_encoder->setCullMode(to_metal_cull_mode(a_cull_mode));
}

[[noreturn]] FORCE_INLINE void RenderCommandEncoder::vertex_buffer(const rhi::BufferHybrid<rhi::Buffer, rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	(void) a_buffer;
	(void) a_offset;
	(void) a_index;

	assert(0);
	// this->m_encoder->setVertexBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

[[noreturn]] FORCE_INLINE void RenderCommandEncoder::fragment_buffer(const rhi::BufferHybrid<rhi::Buffer, rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	(void) a_buffer;
	(void) a_offset;
	(void) a_index;

	assert(0);
	// this->m_encoder->setFragmentBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::tile_buffer(const rhi::BufferHybrid<rhi::Buffer, rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	this->m_encoder->setTileBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::vertex_buffer(const rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	this->m_encoder->setVertexBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::vertex_buffer_offset(const rhi::Buffer &a_buffer, const uintptr_t a_offset, uint32_t a_index) const noexcept
{
	(void) a_buffer;
	this->m_encoder->setVertexBufferOffset(a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::fragment_buffer(const rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	this->m_encoder->setFragmentBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::fragment_buffer_offset(const uintptr_t a_offset, uint32_t a_index) const noexcept
{
	this->m_encoder->setFragmentBufferOffset(a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::tile_buffer(const rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	this->m_encoder->setTileBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::bind_descriptors(const rhi::Device &, const rhi::Program &) const noexcept
{}

FORCE_INLINE constexpr void RenderCommandEncoder::fragment_texture(const rhi::TextureImage &a_texture, uint32_t a_index) const noexcept
{
	this->m_encoder->setFragmentTexture(a_texture.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::vertex_texture(const rhi::TextureImage &a_texture, uint32_t a_index) const noexcept
{
	this->m_encoder->setVertexTexture(a_texture.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::tile_texture(const rhi::TextureImage &a_texture, uint32_t a_index) const noexcept
{
	this->m_encoder->setTileTexture(a_texture.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::fragment_sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) const noexcept
{
	this->m_encoder->setFragmentSamplerState(a_sampler.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::vertex_sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) const noexcept
{
	this->m_encoder->setVertexSamplerState(a_sampler.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::tile_sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) const noexcept
{
	this->m_encoder->setTileSamplerState(a_sampler.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::draw_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_vertex_start, uint32_t a_vertex_count) const noexcept
{
	this->m_encoder->drawPrimitives(to_metal_primitive_topoloy(a_topology), a_vertex_start, a_vertex_count);
}

FORCE_INLINE constexpr void RenderCommandEncoder::draw_primitives_instanced(rhi::PrimitiveTopology a_topology, uint32_t a_vertex_start, uint32_t a_vertex_count, uint32_t a_instance_count) const noexcept
{
	this->m_encoder->drawPrimitives(to_metal_primitive_topoloy(a_topology), a_vertex_start, a_vertex_count, a_instance_count);
}

FORCE_INLINE constexpr void RenderCommandEncoder::draw_indexed_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_index_count, rhi::Format a_format, rhi::Buffer &a_indices, uintptr_t a_offset) const noexcept
{
	this->m_encoder->drawIndexedPrimitives(to_metal_primitive_topoloy(a_topology), a_index_count, to_metal_indexformat(a_format), a_indices.platform_buffer(), a_offset);
}

FORCE_INLINE constexpr void RenderCommandEncoder::draw_indexed_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_index_count, rhi::Format a_format, rhi::BufferHybrid<rhi::Buffer, rhi::Static> &a_indices, uintptr_t a_offset) const noexcept
{
	this->m_encoder->drawIndexedPrimitives(to_metal_primitive_topoloy(a_topology), a_index_count, to_metal_indexformat(a_format), a_indices.platform_buffer(), a_offset);
}

FORCE_INLINE constexpr void RenderCommandEncoder::end_encoding() const noexcept
{
	this->m_encoder->endEncoding();
}

FORCE_INLINE constexpr void RenderCommandEncoder::release() const noexcept
{
	this->m_encoder->release();
}

FORCE_INLINE constexpr void RenderCommandEncoder::triangle_fill_mode(rhi::TriangleFillMode a_fill_mode) const noexcept
{
	this->m_encoder->setTriangleFillMode(to_metal_triangle_fill_mode(a_fill_mode));
}
}        // namespace rhi
