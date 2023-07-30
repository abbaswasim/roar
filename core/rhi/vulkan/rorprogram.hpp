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
#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rorprogram.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"

namespace ror
{
class Model;
}

namespace rhi
{
class Rendersubpass;
class BuffersPack;

typedef struct VkGraphicsPipeline_T *GraphicsPipelineState;
typedef struct VkComputePipeline_T  *ComputePipelineState;

class ProgramVulkan : public ProgramCrtp<ProgramVulkan>
{
  public:
	FORCE_INLINE                ProgramVulkan(const ProgramVulkan &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                ProgramVulkan(ProgramVulkan &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ProgramVulkan &operator=(const ProgramVulkan &a_other)         = default;        //! Copy assignment operator
	FORCE_INLINE ProgramVulkan &operator=(ProgramVulkan &&a_other) noexcept     = default;        //! Move assignment operator
	FORCE_INLINE virtual ~ProgramVulkan() noexcept override                     = default;        //! Destructor

	FORCE_INLINE ProgramVulkan(int32_t a_vert_id, int32_t a_frag_id) :
	    ProgramCrtp(a_vert_id, a_frag_id)
	{}

	FORCE_INLINE explicit ProgramVulkan(int32_t a_compute_id) :
	    ProgramCrtp(a_compute_id)
	{}

	void upload(rhi::Device &a_device, const std::vector<rhi::Shader> &a_shaders, const ror::Model &a_model, uint32_t a_mesh_index, uint32_t a_prim_index, const rhi::Rendersubpass &a_subpass, bool a_premultiplied_alpha);
	void upload(rhi::Device &a_device, const std::vector<rhi::Shader> &a_shaders, rhi::BuffersPack &a_buffer_pack, bool a_premultiplied_alpha);
	void upload(rhi::Device &a_device, const rhi::Shader &a_vs_shader, const rhi::Shader &a_fs_shader, const rhi::VertexDescriptor &a_vertex_descriptor, rhi::BlendMode a_blend_mode,
	            rhi::PrimitiveTopology a_toplogy, const char *a_pso_name, bool a_subpass_has_depth, bool a_is_depth_shadow, bool a_premultiplied_alpha);
	void release(rhi::Device &a_device);

	FORCE_INLINE constexpr auto *compute_pipeline_state() const noexcept
	{
		if (std::holds_alternative<ComputePipelineState>(this->m_pipeline_state))
			return reinterpret_cast<VkPipeline>(std::get<ComputePipelineState>(this->m_pipeline_state));

		return static_cast<VkPipeline>(nullptr);
	}

	FORCE_INLINE constexpr auto *render_pipeline_state() const noexcept
	{
		if (std::holds_alternative<GraphicsPipelineState>(this->m_pipeline_state))
			return reinterpret_cast<VkPipeline>(std::get<GraphicsPipelineState>(this->m_pipeline_state));

		return static_cast<VkPipeline>(nullptr);
	}

  protected:
	FORCE_INLINE ProgramVulkan() = default;        //! Default constructor

  private:
	declare_translation_unit_vtable();

	std::variant<GraphicsPipelineState, ComputePipelineState> m_pipeline_state{};        //! This program will contain either Render or Compute pipeline state
};

declare_rhi_render_type(Program);

}        // namespace rhi

#include "rhi/vulkan/rorprogram.hh"
