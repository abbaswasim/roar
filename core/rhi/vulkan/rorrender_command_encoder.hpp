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

#include "foundation/rormacros.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector4.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorrenderstate.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"

namespace rhi
{
class ProgramMetal;

declare_rhi_render_type(Program);

class RenderCommandEncoderVulkan final
{
  public:
	FORCE_INLINE                             RenderCommandEncoderVulkan(const RenderCommandEncoderVulkan &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                             RenderCommandEncoderVulkan(RenderCommandEncoderVulkan &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderCommandEncoderVulkan &operator=(const RenderCommandEncoderVulkan &a_other)                      = default;        //! Copy assignment operator
	FORCE_INLINE RenderCommandEncoderVulkan &operator=(RenderCommandEncoderVulkan &&a_other) noexcept                  = default;        //! Move assignment operator
	FORCE_INLINE ~RenderCommandEncoderVulkan() noexcept;
	FORCE_INLINE explicit RenderCommandEncoderVulkan(VkCommandBuffer a_command_buffer);

	FORCE_INLINE constexpr void viewport(ror::Vector4d a_viewport_rectangle, ror::Vector2d a_near_far) const noexcept;
	FORCE_INLINE constexpr void scissor(ror::Vector4ui a_scissor_rectangle) const noexcept;
	FORCE_INLINE constexpr void front_facing_winding(rhi::PrimitiveWinding a_winding) const noexcept;
	FORCE_INLINE constexpr void depth_stencil_state(const rhi::RenderstateDepth &a_depth_stencil) const noexcept;
	FORCE_INLINE constexpr void cull_mode(rhi::PrimitiveCullMode a_cull_mode) const noexcept;
	FORCE_INLINE constexpr void render_pipeline_state(const rhi::Program &a_render_pipeline_state) const noexcept;
	FORCE_INLINE constexpr void vertex_buffer(rhi::BufferHybrid<rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void fragment_buffer(rhi::BufferHybrid<rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void tile_buffer(rhi::BufferHybrid<rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void vertex_buffer(rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void vertex_buffer_offset(uintptr_t a_offset, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void fragment_buffer(rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void fragment_buffer_offset(uintptr_t a_offset, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void tile_buffer(rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void fragment_texture(const rhi::TextureImage &a_texture, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void vertex_texture(const rhi::TextureImage &a_texture, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void tile_texture(const rhi::TextureImage &a_texture, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void fragment_sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void vertex_sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void tile_sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) const noexcept;
	FORCE_INLINE constexpr void draw_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_vertex_start, uint32_t a_vertex_count) const noexcept;
	FORCE_INLINE constexpr void draw_primitives_instanced(rhi::PrimitiveTopology a_topology, uint32_t a_vertex_start, uint32_t a_vertex_count, uint32_t a_instance_count) const noexcept;
	FORCE_INLINE constexpr void draw_indexed_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_index_count, rhi::Format a_format, rhi::Buffer &a_indices, uintptr_t a_offset) const noexcept;
	FORCE_INLINE constexpr void draw_indexed_primitives(rhi::PrimitiveTopology a_topology, uint32_t a_index_count, rhi::Format a_format, rhi::BufferHybrid<rhi::Static> &a_indices, uintptr_t a_offset) const noexcept;
	FORCE_INLINE constexpr void end_encoding() const noexcept;
	FORCE_INLINE constexpr void release() const noexcept;
	FORCE_INLINE constexpr void triangle_fill_mode(rhi::TriangleFillMode a_fill_mode) const noexcept;

	// clang-format off
	FORCE_INLINE constexpr auto platform_encoder() noexcept { return this->m_command_buffer; }
	// clang-format on

	// FIXME: Put me back into private
	FORCE_INLINE RenderCommandEncoderVulkan() = default;        //! Default constructor
  protected:
  private:
	// FORCE_INLINE               RenderCommandEncoderVulkan() = default;        //! Default constructor
	VkCommandBuffer m_command_buffer{nullptr};
};

declare_rhi_render_type(RenderCommandEncoder);

}        // namespace rhi

#include "rhi/vulkan/rorrender_command_encoder.hh"
