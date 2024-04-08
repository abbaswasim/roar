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
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"

namespace rhi
{

class ProgramVulkan;
class CommandBufferVulkan;

declare_rhi_render_type(Program);

class ComputeCommandEncoderVulkan final
{
  public:
	FORCE_INLINE                              ComputeCommandEncoderVulkan(const ComputeCommandEncoderVulkan &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                              ComputeCommandEncoderVulkan(ComputeCommandEncoderVulkan &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ComputeCommandEncoderVulkan &operator=(const ComputeCommandEncoderVulkan &a_other)                       = default;        //! Copy assignment operator
	FORCE_INLINE ComputeCommandEncoderVulkan &operator=(ComputeCommandEncoderVulkan &&a_other) noexcept                   = default;        //! Move assignment operator
	FORCE_INLINE ~ComputeCommandEncoderVulkan() noexcept;
	FORCE_INLINE explicit ComputeCommandEncoderVulkan(rhi::CommandBufferVulkan &a_encoder);

	FORCE_INLINE constexpr void buffer(rhi::BufferHybrid<rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) noexcept;
	FORCE_INLINE constexpr void buffer(rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) noexcept;
	FORCE_INLINE constexpr void texture(const rhi::TextureImage &a_texture, uint32_t a_index) noexcept;
	FORCE_INLINE constexpr void sampler(const rhi::TextureSampler &a_sampler, uint32_t a_index) noexcept;
	FORCE_INLINE constexpr void dispatch_threads(ror::Vector3ui a_threads_per_grid, ror::Vector3ui a_threads_per_threadgroup) noexcept;
	FORCE_INLINE constexpr void end_encoding() noexcept;
	FORCE_INLINE constexpr void release() const noexcept;
	FORCE_INLINE constexpr void release() noexcept;

	void compute_pipeline_state(const rhi::Program &a_compute_pipeline_state) noexcept;

  protected:
  private:
	FORCE_INLINE    ComputeCommandEncoderVulkan() = default;        //! Default constructor
	VkCommandBuffer m_command_buffer{nullptr};                      //! The command buffer used to encode the encoder commands
};

declare_rhi_render_type(ComputeCommandEncoder);

}        // namespace rhi

#include "rhi/vulkan/rorcompute_command_encoder.hh"
