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
#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/vulkan/rorbuffer.hpp"
#include "rhi/vulkan/rordevice.hpp"
#include "rhi/vulkan/rorrender_command_encoder.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "settings/rorsettings.hpp"
#include <cassert>

namespace rhi
{

FORCE_INLINE RenderCommandEncoder::~RenderCommandEncoderVulkan() noexcept
{
	if (this->m_command_buffer)
	{
		// this->m_encoder->endEncoding();
		// this->m_encoder->release();
	}
}

FORCE_INLINE void RenderCommandEncoder::viewport(ror::Vector4d a_viewport_rectangle, ror::Vector2d a_near_far) const noexcept
{
	const VkViewport viewports{
	    .x        = static_cast<float32_t>(a_viewport_rectangle.x),
	    .y        = static_cast<float32_t>(a_viewport_rectangle.y),
	    .width    = static_cast<float32_t>(a_viewport_rectangle.z),
	    .height   = static_cast<float32_t>(a_viewport_rectangle.w),
	    .minDepth = static_cast<float32_t>(a_near_far.x),
	    .maxDepth = static_cast<float32_t>(a_near_far.y),
	};

	vkCmdSetViewport(this->m_command_buffer, 0, 1, &viewports);
}

FORCE_INLINE void RenderCommandEncoder::scissor(ror::Vector4ui a_scissor_rectangle) const noexcept
{
	VkRect2D rect{
	    .offset.x      = ror::static_cast_safe<int32_t>(a_scissor_rectangle.x),
	    .offset.y      = ror::static_cast_safe<int32_t>(a_scissor_rectangle.y),
	    .extent.width  = a_scissor_rectangle.z,
	    .extent.height = a_scissor_rectangle.w};

	vkCmdSetScissor(this->m_command_buffer, 0, 1, &rect);
}

FORCE_INLINE void RenderCommandEncoder::front_facing_winding(rhi::PrimitiveWinding a_winding) const noexcept
{
	if (ror::settings().m_vulkan.m_extended_dynamic_state)
		vkCmdSetFrontFace(this->m_command_buffer, to_vulkan_winding(a_winding));
}

FORCE_INLINE void RenderCommandEncoder::depth_stencil_state(const rhi::RenderstateDepth &a_depth_stencil) const noexcept
{
	(void) a_depth_stencil;
	ror::log_critical("Not sure what to do with {}", __FUNCTION__);
	// this->m_encoder->setDepthStencilState(a_depth_stencil.depth_state());
}

FORCE_INLINE void RenderCommandEncoder::cull_mode(rhi::PrimitiveCullMode a_cull_mode) const noexcept
{
	if (ror::settings().m_vulkan.m_extended_dynamic_state)
	{
		VkCullModeFlags cull_mode{to_vulkan_cull_mode(a_cull_mode)};
		vkCmdSetCullMode(this->m_command_buffer, cull_mode);
	}
}

FORCE_INLINE void RenderCommandEncoder::vertex_buffer(const rhi::BufferHybrid<rhi::Buffer, rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	(void) a_buffer;
	(void) a_offset;
	(void) a_index;

	ror::log_critical("Not sure what to do with this yet {}", __FUNCTION__);
}

FORCE_INLINE void RenderCommandEncoder::fragment_buffer(const rhi::BufferHybrid<rhi::Buffer, rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	(void) a_buffer;
	(void) a_offset;
	(void) a_index;

	ror::log_critical("Not sure what to do with this yet {}", __FUNCTION__);
}

FORCE_INLINE void RenderCommandEncoder::tile_buffer(const rhi::BufferHybrid<rhi::Buffer, rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	(void) a_buffer;
	(void) a_offset;
	(void) a_index;

	ror::log_critical("Not sure what to do with this yet {}", __FUNCTION__);
}

FORCE_INLINE void RenderCommandEncoder::vertex_buffer(const rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	auto buffer = a_buffer.platform_buffer();
	auto offset = static_cast<VkDeviceSize>(a_offset);

	vkCmdBindVertexBuffers(this->m_command_buffer, a_index, 1, &buffer, &offset);
}

FORCE_INLINE void RenderCommandEncoder::vertex_buffer_offset(const rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	// There is no concept of vertex buffer offset in Vulkan so using setVertexBuffer instead
	this->vertex_buffer(a_buffer, a_offset, a_index);
}

// Don't need to do anything here because descriptors are already bound
FORCE_INLINE constexpr void RenderCommandEncoder::fragment_buffer(const rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	(void) a_buffer;
	(void) a_offset;
	(void) a_index;
}

// Don't need to do anything here because descriptors are already bound
FORCE_INLINE void RenderCommandEncoder::fragment_buffer_offset(const uintptr_t a_offset, uint32_t a_index) const noexcept
{
	(void) a_offset;
	(void) a_index;
}

// Don't need to do anything here because descriptors are already bound
FORCE_INLINE constexpr void RenderCommandEncoder::tile_buffer(const rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept
{
	(void) a_buffer;
	(void) a_offset;
	(void) a_index;
}

// Don't need to do anything in these methods because descriptors are already bound
FORCE_INLINE constexpr void RenderCommandEncoder::fragment_texture(const rhi::TextureImage &a_texture, uint32_t a_index) const noexcept
{
	(void) a_texture;
	(void) a_index;
}

FORCE_INLINE constexpr void RenderCommandEncoder::vertex_texture(const rhi::TextureImage &a_texture, uint32_t a_index) const noexcept
{
	(void) a_texture;
	(void) a_index;
}

FORCE_INLINE constexpr void RenderCommandEncoder::tile_texture(const rhi::TextureImage &a_texture, uint32_t a_index) const noexcept
{
	(void) a_texture;
	(void) a_index;
}

FORCE_INLINE constexpr void RenderCommandEncoder::fragment_sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) const noexcept
{
	(void) a_sampler;
	(void) a_index;
}

FORCE_INLINE constexpr void RenderCommandEncoder::vertex_sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) const noexcept
{
	(void) a_sampler;
	(void) a_index;
}

FORCE_INLINE constexpr void RenderCommandEncoder::tile_sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) const noexcept
{
	(void) a_sampler;
	(void) a_index;
}

FORCE_INLINE void RenderCommandEncoder::draw_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_vertex_start, uint32_t a_vertex_count) const noexcept
{
	(void) a_topology;
	vkCmdDraw(this->m_command_buffer, a_vertex_count, 1, a_vertex_start, 0);
}

FORCE_INLINE void RenderCommandEncoder::draw_primitives_instanced(rhi::PrimitiveTopology a_topology, uint32_t a_vertex_start, uint32_t a_vertex_count, uint32_t a_instance_count) const noexcept
{
	(void) a_topology;
	vkCmdDraw(this->m_command_buffer, a_vertex_count, a_instance_count, a_vertex_start, 0);
}

FORCE_INLINE void RenderCommandEncoder::draw_indexed_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_index_count, rhi::Format a_format, rhi::Buffer &a_indices, uintptr_t a_offset) const noexcept
{
	(void) a_topology;
	vkCmdBindIndexBuffer(this->m_command_buffer, a_indices.platform_buffer(), a_offset, to_vulkan_indexformat(a_format));
	vkCmdDrawIndexed(this->m_command_buffer, a_index_count, 1, 0, 0, 0);
}

FORCE_INLINE void RenderCommandEncoder::draw_indexed_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_index_count, rhi::Format a_format, rhi::BufferHybrid<rhi::Buffer, rhi::Static> &a_indices, uintptr_t a_offset) const noexcept
{
	(void) a_topology;
	vkCmdBindIndexBuffer(this->m_command_buffer, a_indices.platform_buffer(), a_offset, to_vulkan_indexformat(a_format));
	vkCmdDrawIndexed(this->m_command_buffer, a_index_count, 1, 0, 0, 0);
}

FORCE_INLINE constexpr void RenderCommandEncoder::end_encoding() noexcept
{
	// vk_end_command_buffer(this->m_command_buffer);        // Maybe this is not the right idea, if we have an outter command buffer that needs to be closed down first
	// At the moment this is done in CommandBufferVulkan::commit
}

FORCE_INLINE constexpr void RenderCommandEncoder::release() const noexcept
{
	// this->m_encoder->release();
}

FORCE_INLINE void RenderCommandEncoder::triangle_fill_mode(rhi::TriangleFillMode a_fill_mode) const noexcept
{
	if (ror::settings().m_vulkan.m_extended_dynamic_state3)
		vkCmdSetPolygonModeEXT(this->m_command_buffer, to_vulkan_triangle_fill_mode(a_fill_mode));
}
}        // namespace rhi
