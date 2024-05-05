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
#include "graphics/rorlight.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rorskin.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rorprogram.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"
#include "spirv-cross/spirv_cross.hpp"
#include "spirv-cross/spirv_glsl.hpp"

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

enum class ShaderResourceType
{
	input            = VK_DESCRIPTOR_TYPE_MAX_ENUM,
	output           = VK_DESCRIPTOR_TYPE_MAX_ENUM,
	sampled_image    = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
	storage_image    = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
	separate_image   = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
	separate_sampler = VK_DESCRIPTOR_TYPE_SAMPLER,
	uniform_buffer   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
	storage_buffer   = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	input_attachment = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
};

struct ShaderResource
{
	ShaderResourceType m_type{ShaderResourceType::input};
	uint32_t           m_set{0};
	uint32_t           m_binding{0};
	uint32_t           m_location{0};
	VertexFormat       m_format{VertexFormat::float32_3};
	std::string        m_name{};
};

using spirv_resources_vector = const spirv_cross::SmallVector<spirv_cross::Resource>;
using shader_resources_map   = std::unordered_map<uint32_t, std::vector<ShaderResource>>;

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

	void clear_descriptor();
	void allocate_descriptor(const VkDevice a_device, DescriptorSetLayoutCache &a_layout_cache, DescriptorPool &a_descriptor_pool, DescriptorSetCache &a_descriptor_cache, DescriptorSet &a_set, uint32_t a_set_id);
	void allocate_empty_descriptors(const VkDevice a_device, DescriptorSetLayoutCache &a_layout_cache, DescriptorPool &a_descriptor_pool, DescriptorSetCache &a_descriptor_cache);
	void build_descriptor(const rhi::Device &a_device, const ror::Renderer &a_renderer, const std::vector<rhi::Shader> &a_shaders, bool a_need_shadow_map, bool a_with_environment);
	void build_descriptor(const rhi::Device &a_device, const ror::Renderer &a_renderer, const std::vector<rhi::Shader> &a_shaders, const ror::Scene *a_scene,
	                      const ror::Material                                                               *a_material,
	                      const std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>               *a_textures,
	                      const std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>>     *a_images,
	                      const std::vector<rhi::TextureSampler, rhi::BufferAllocator<rhi::TextureSampler>> *a_samplers,
	                      const ror::Skin *a_skin, bool a_need_shadow_map, bool a_with_environment);
	void build_descriptor(const rhi::Device &a_device, const rhi::ShaderBuffer *a_shader_buffer, uint32_t buffer_binding,
	                      const rhi::TextureImage *a_image, const rhi::TextureSampler *a_sampler, uint32_t a_texture_binding);
	void build_descriptor(const rhi::Device &a_device, const std::vector<rhi::Shader> &a_shaders);
	void update_descriptor(const rhi::Device &a_device, const ror::Renderer &a_renderer, descriptor_update_type &a_buffers_images, bool a_use_environment);
	void update_descriptor(const rhi::Device &a_device);
	void upload(const rhi::Device &a_device, const rhi::Renderpass &a_renderpass, const rhi::Rendersubpass &a_subpass, const std::vector<rhi::Shader> &a_shaders,
	            const ror::Model &a_model, uint32_t a_mesh_index, uint32_t a_prim_index, bool a_premultiplied_alpha);
	void upload(const rhi::Device &a_device, const rhi::Renderpass &a_pass, const rhi::Rendersubpass &a_subpass, const std::vector<rhi::Shader> &a_shaders, rhi::BuffersPack &a_buffer_pack, bool a_premultiplied_alpha);
	void upload(const rhi::Device &a_device, const rhi::Renderpass &a_pass, const rhi::Rendersubpass &a_subpass, const rhi::Shader &a_vs_shader, const rhi::Shader &a_fs_shader,
	            const rhi::VertexDescriptor &a_vertex_descriptor, rhi::BlendMode a_blend_mode, rhi::PrimitiveTopology a_toplogy, const char *a_pso_name,
	            bool a_subpass_has_depth, bool a_is_depth_shadow, bool a_premultiplied_alpha);
	void upload(const rhi::Device &a_device, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass, const rhi::VertexDescriptor &a_vertex_descriptor, const std::vector<rhi::Shader> &a_shaders,
	            rhi::BlendMode a_blend_mode, rhi::PrimitiveTopology a_toplogy, const char *a_pso_name, bool a_subpass_has_depth, bool a_is_depth_shadow, bool a_premultiplied_alpha);
	void upload(const rhi::Device &a_device, const std::vector<rhi::Shader> &a_shaders);        // Easy way to create a compute pipeline without lots of arguments, currently unused, don't remove
	void release(const rhi::Device &a_device);

	DescriptorSet &descriptor(DescriptorSetCache &a_descriptor_cache, uint32_t a_set_id);

	// clang-format off
	FORCE_INLINE constexpr auto &platform_descriptors()         const noexcept { return this->m_platform_descriptors;        }
	FORCE_INLINE constexpr auto &platform_descriptors_layouts() const noexcept { return this->m_platform_descriptor_layouts; }
	FORCE_INLINE constexpr auto &pipeline_layout()              const noexcept { return this->m_pipeline_layout;             }
	// clang-format on

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
	void environment_descriptor_set(const ror::Renderer &a_renderer, shader_resources_map &shaders_reflection, DescriptorSet &a_set, bool &a_allocate);
	void environment_descriptor_set_update(const rhi::Device &a_device, const ror::Renderer &a_renderer);

	std::array<VkDescriptorSetLayout, 4>                      m_platform_descriptor_layouts{};        //! All the platform descriptor set layouts in the layouts cache
	std::array<size_t, 4>                                     m_platform_descriptors{};               //! Index of the platform descriptor set in the descriptors cache
	std::variant<GraphicsPipelineState, ComputePipelineState> m_pipeline_state{};                     //! This program will contain either Render or Compute pipeline state
	VkPipelineLayout                                          m_pipeline_layout{};                    //! The pipeline layout used to build this program
};

declare_rhi_render_type(Program);

}        // namespace rhi

#include "rhi/vulkan/rorprogram.hh"
