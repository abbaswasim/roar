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
#include "rhi/rorprogram.hpp"
#include "rhi/rortypes.hpp"
#include <_types/_uint32_t.h>
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

FORCE_INLINE constexpr void RenderCommandEncoder::viewport(ror::Vector4d a_viewport_rectangle, ror::Vector2d a_near_far) noexcept
{
	this->m_encoder->setViewport(MTL::Viewport{a_viewport_rectangle.x, a_viewport_rectangle.y, a_viewport_rectangle.z, a_viewport_rectangle.w, a_near_far.x, a_near_far.y});
}

FORCE_INLINE constexpr void RenderCommandEncoder::front_facing_winding(rhi::PrimitiveWinding a_winding) noexcept
{
	this->m_encoder->setFrontFacingWinding(to_metal_winding(a_winding));
}

FORCE_INLINE constexpr void RenderCommandEncoder::cull_mode(rhi::PrimitiveCullMode a_cull_mode) noexcept
{
	this->m_encoder->setCullMode(to_metal_cull_mode(a_cull_mode));
}

FORCE_INLINE constexpr void RenderCommandEncoder::render_pipeline_state(const rhi::Program &a_render_pipeline_state) noexcept
{
	this->m_encoder->setRenderPipelineState(a_render_pipeline_state.render_pipeline_state());
}

FORCE_INLINE constexpr void RenderCommandEncoder::vertex_buffer(rhi::BufferHybrid<rhi::Static> &a_buffer, uint32_t a_offset, uint32_t a_index) noexcept
{
	this->m_encoder->setVertexBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::fragment_buffer(rhi::BufferHybrid<rhi::Static> &a_buffer, uint32_t a_offset, uint32_t a_index) noexcept
{
	this->m_encoder->setFragmentBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::tile_buffer(rhi::BufferHybrid<rhi::Static> &a_buffer, uint32_t a_offset, uint32_t a_index) noexcept
{
	this->m_encoder->setTileBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::vertex_buffer(rhi::Buffer &a_buffer, uint32_t a_offset, uint32_t a_index) noexcept
{
	this->m_encoder->setVertexBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::fragment_buffer(rhi::Buffer &a_buffer, uint32_t a_offset, uint32_t a_index) noexcept
{
	this->m_encoder->setFragmentBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::tile_buffer(rhi::Buffer &a_buffer, uint32_t a_offset, uint32_t a_index) noexcept
{
	this->m_encoder->setTileBuffer(a_buffer.platform_buffer(), a_offset, a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::fragment_texture(rhi::TextureImage &a_texture, uint32_t a_index) noexcept
{
	this->m_encoder->setFragmentTexture(a_texture.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::vertex_texture(rhi::TextureImage &a_texture, uint32_t a_index) noexcept
{
	this->m_encoder->setVertexTexture(a_texture.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::tile_texture(rhi::TextureImage &a_texture, uint32_t a_index) noexcept
{
	this->m_encoder->setTileTexture(a_texture.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::fragment_sampler(rhi::TextureSampler &a_sampler, uint32_t a_index) noexcept
{
	this->m_encoder->setFragmentSamplerState(a_sampler.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::vertex_sampler(rhi::TextureSampler &a_sampler, uint32_t a_index) noexcept
{
	this->m_encoder->setVertexSamplerState(a_sampler.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::tile_sampler(rhi::TextureSampler &a_sampler, uint32_t a_index) noexcept
{
	this->m_encoder->setTileSamplerState(a_sampler.platform_handle(), a_index);
}

FORCE_INLINE constexpr void RenderCommandEncoder::draw_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_vertex_start, uint32_t a_vertex_count)
{
	this->m_encoder->drawPrimitives(to_metal_primitive_topoloy(a_topology), a_vertex_start, a_vertex_count);
}

FORCE_INLINE constexpr void RenderCommandEncoder::draw_primitives_instanced(rhi::PrimitiveTopology a_topology, uint32_t a_vertex_start, uint32_t a_vertex_count, uint32_t a_instance_count)
{
	this->m_encoder->drawPrimitives(to_metal_primitive_topoloy(a_topology), a_vertex_start, a_vertex_count, a_instance_count);
}

FORCE_INLINE constexpr void RenderCommandEncoder::draw_indexed_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_index_count, rhi::Format a_format, rhi::Buffer &a_indices, uint32_t a_offset) noexcept
{
	this->m_encoder->drawIndexedPrimitives(to_metal_primitive_topoloy(a_topology), a_index_count, to_metal_indexformat(a_format), a_indices.platform_buffer(), a_offset);
}

FORCE_INLINE constexpr void RenderCommandEncoder::draw_indexed_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_index_count, rhi::Format a_format, rhi::BufferHybrid<rhi::Static> &a_indices, uint32_t a_offset) noexcept
{
	this->m_encoder->drawIndexedPrimitives(to_metal_primitive_topoloy(a_topology), a_index_count, to_metal_indexformat(a_format), a_indices.platform_buffer(), a_offset);
}

FORCE_INLINE constexpr void RenderCommandEncoder::end_encoding() noexcept
{
	this->m_encoder->endEncoding();
}

}        // namespace rhi
