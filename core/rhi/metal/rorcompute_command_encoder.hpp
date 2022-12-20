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

#include <Metal/MTLBuffer.hpp>
#include <Metal/MTLCommandBuffer.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>

namespace rhi
{

class ProgramMetal;
declare_rhi_render_type(Program);

class ComputeCommandEncoderMetal final
{
  public:
	FORCE_INLINE                             ComputeCommandEncoderMetal(const ComputeCommandEncoderMetal &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                             ComputeCommandEncoderMetal(ComputeCommandEncoderMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ComputeCommandEncoderMetal &operator=(const ComputeCommandEncoderMetal &a_other)                      = default;        //! Copy assignment operator
	FORCE_INLINE ComputeCommandEncoderMetal &operator=(ComputeCommandEncoderMetal &&a_other) noexcept                  = default;        //! Move assignment operator
	FORCE_INLINE ~ComputeCommandEncoderMetal() noexcept;
	FORCE_INLINE explicit ComputeCommandEncoderMetal(MTL::ComputeCommandEncoder *a_encoder);

	FORCE_INLINE constexpr void compute_pipeline_state(const rhi::Program &a_compute_pipeline_state) noexcept;
	FORCE_INLINE constexpr void buffer(rhi::BufferHybrid<rhi::Static> &a_buffer, uintptr_t a_offset, uint32_t a_index) noexcept;
	FORCE_INLINE constexpr void buffer(rhi::Buffer &a_buffer, uintptr_t a_offset, uint32_t a_index) noexcept;
	FORCE_INLINE constexpr void texture(rhi::TextureImage &a_texture, uint32_t a_index) noexcept;
	FORCE_INLINE constexpr void sampler(rhi::TextureSampler &a_sampler, uint32_t a_index) noexcept;
	FORCE_INLINE constexpr void dispatch_threads(ror::Vector3ui a_threads_per_grid, ror::Vector3ui a_threads_per_threadgroup) noexcept;
	FORCE_INLINE constexpr void end_encoding() noexcept;
	FORCE_INLINE constexpr void release() noexcept;

  protected:
  private:
	FORCE_INLINE                ComputeCommandEncoderMetal() = default;        //! Default constructor
	MTL::ComputeCommandEncoder *m_encoder{nullptr};
};

declare_rhi_render_type(ComputeCommandEncoder);

}        // namespace rhi

#include "rhi/metal/rorcompute_command_encoder.hh"
