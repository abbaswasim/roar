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

#include "foundation/rormacros.hpp"
#include "graphics/rorlight.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rorscene.hpp"
#include "profiling/rorlog.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_layout.hpp"
#include "rhi/vulkan/rordescriptor_set.hpp"
#include "rhi/vulkan/rorprogram.hpp"
#include "rhi/vulkan/rorshader.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include "settings/rorsettings.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace rhi
{
define_translation_unit_vtable(ProgramVulkan)
{}

using VulkanDescriptorVector = std::vector<std::pair<VkVertexInputAttributeDescription, VkVertexInputBindingDescription>>;

void ProgramVulkan::release(const rhi::Device &a_device)
{
	auto *device = a_device.platform_device();

	VkPipeline pipeline{this->render_pipeline_state()};

	if (pipeline == nullptr)
		pipeline = this->compute_pipeline_state();

	assert(pipeline && "Neither render not compute pipeline found in program vulkan");

	vk_destroy_pipeline(device, pipeline);
}

static void append_to_vulkan_vertex_descriptor(VulkanDescriptorVector     &a_attributes,
                                               const rhi::VertexAttribute &attrib, const rhi::VertexLayout &layout)
{
	assert(layout.binding() == attrib.binding() && "In vulkan layout and attribute stride must match");

	VkVertexInputAttributeDescription vertex_descriptor_attribute;
	VkVertexInputBindingDescription   vertex_descriptor_binding;

	vertex_descriptor_attribute.location = attrib.location();
	vertex_descriptor_attribute.binding  = attrib.binding();
	vertex_descriptor_attribute.format   = to_vulkan_vertexformat(attrib.format());
	vertex_descriptor_attribute.offset   = attrib.offset();

	vertex_descriptor_binding.binding   = layout.binding();
	vertex_descriptor_binding.stride    = layout.stride();
	vertex_descriptor_binding.inputRate = to_vulkan_step_function(layout.step_function());

	a_attributes.emplace_back(std::make_pair(vertex_descriptor_attribute, vertex_descriptor_binding));
}

static auto get_default_vertex_descriptor(rhi::BuffersPack &a_buffer_pack)
{
	auto                 &setting = ror::settings();
	rhi::VertexDescriptor descriptor{};

	for (auto att_pair : setting.m_default_vertex_descriptor.attributes)
		descriptor.add(att_pair.first, att_pair.second, &a_buffer_pack);

	return descriptor;
}

static auto get_default_vlk_vertex_descriptor(rhi::BuffersPack &a_buffer_pack)
{
	auto vertex_descriptor = get_default_vertex_descriptor(a_buffer_pack);

	VulkanDescriptorVector attributes;

	for (auto &attrib : vertex_descriptor.attributes())
		append_to_vulkan_vertex_descriptor(attributes, attrib, vertex_descriptor.layout(attrib.semantics()));

	return attributes;
}

static auto walk_vertex_descriptor_attributes(VulkanDescriptorVector &a_attributes, const rhi::VertexDescriptor &a_vertex_descriptor, bool a_depth_shadow)
{
	// Lets read all the descriptors we might need to use for normal attributes + morph target attributes
	const auto &attributes = a_vertex_descriptor.attributes();
	for (auto &attrib : attributes)
	{
		if (is_attribute_required_in_pass(attrib.semantics(), a_depth_shadow))
			append_to_vulkan_vertex_descriptor(a_attributes, attrib, a_vertex_descriptor.layout(attrib.semantics()));
	}
}

static auto get_vulkan_vertex_descriptor(const std::vector<ror::Mesh, rhi::BufferAllocator<ror::Mesh>> &a_meshes, uint32_t a_mesh_index, uint32_t a_prim_index, bool a_depth_shadow)
{
	const auto &mesh                     = a_meshes[a_mesh_index];
	const auto &vertex_descriptor        = mesh.vertex_descriptor(a_prim_index);
	const auto &morph_vertex_descriptors = mesh.target_descriptor(a_prim_index);

	// Lets read all the descriptors we might need to use for normal attributes + morph target attributes
	std::vector<std::reference_wrapper<const rhi::VertexDescriptor>> descriptors{std::cref(vertex_descriptor)};
	for (const auto &morph_descriptor : morph_vertex_descriptors)
		descriptors.emplace_back(morph_descriptor);

	VulkanDescriptorVector vulkan_descriptor{};
	vulkan_descriptor.reserve(descriptors.size() * 10);        // Upper limit of how many attributes one can have

	for (auto &descriptor : descriptors)
		walk_vertex_descriptor_attributes(vulkan_descriptor, descriptor.get(), a_depth_shadow);

	return vulkan_descriptor;
}

static auto get_vulkan_vertex_descriptor(const rhi::VertexDescriptor &a_vertex_descriptor, bool a_depth_shadow)
{
	VulkanDescriptorVector vulkan_descriptor{};

	walk_vertex_descriptor_attributes(vulkan_descriptor, a_vertex_descriptor, a_depth_shadow);

	return vulkan_descriptor;
}

// Since Vulkan needs an index instead of the actual thing, searching it via index in the subpasses
auto get_subpass_index(const rhi::Renderpass &a_renderpass, const rhi::Rendersubpass &a_subpass)
{
	auto index{0};
	for (auto &subpass : a_renderpass.subpasses())
	{
		if (&subpass == &a_subpass)
			return index;

		index++;
	}

	return -1;
}

static auto create_compute_pipeline(const rhi::Device                        &a_device,
                                    const rhi::Shader                        &a_compute_shader,
                                    const std::vector<VkDescriptorSetLayout> &a_vulkan_descriptors_layouts)

{
	auto                            device = a_device.platform_device();
	auto                            pipeline_cache{a_device.platform_pipeline_cache()};
	VkPipelineShaderStageCreateInfo compute_shader_stage_info = vk_create_pipeline_shader_stage(VK_SHADER_STAGE_COMPUTE_BIT, "main", a_compute_shader.module());
	VkPipelineLayoutCreateInfo      pipeline_layout_info      = vk_create_pipeline_layout_state(a_vulkan_descriptors_layouts);
	VkPipelineLayout                pipeline_layout           = vk_create_pipeline_layout(device, pipeline_layout_info);

	auto compute_pipeline = vk_create_compute_pipeline(device, pipeline_cache, pipeline_layout, compute_shader_stage_info);

	return reinterpret_cast<ComputePipelineState>(compute_pipeline);
}

static auto create_fragment_render_pipeline(const rhi::Device                        &a_device,
                                            const rhi::Shader                        &a_vertex_shader,
                                            const rhi::Shader                        &a_fragment_shader,
                                            const rhi::Renderpass                    &a_renderpass,
                                            const rhi::Rendersubpass                 &a_render_subpass,
                                            const VulkanDescriptorVector             &a_vulkan_descriptor,
                                            const std::vector<VkDescriptorSetLayout> &a_vulkan_descriptors_layouts,
                                            rhi::BlendMode                            a_blend_mode,
                                            rhi::PrimitiveTopology                    a_topology,
                                            const char                               *a_label,
                                            bool                                      a_depth,
                                            bool                                      a_premultiplied_alpha = false,
                                            rhi::PrimitiveCullMode                    a_cull_mode           = rhi::PrimitiveCullMode::back,
                                            rhi::PrimitiveWinding                     a_winding             = rhi::PrimitiveWinding::counter_clockwise)
{
	(void) a_label;

	auto                                           device                 = a_device.platform_device();
	auto                                          &setting                = ror::settings();
	VkShaderModule                                 vert_shader_module     = a_vertex_shader.module();
	VkShaderModule                                 frag_shader_module     = a_fragment_shader.module();
	VkPipelineShaderStageCreateInfo                vert_shader_stage_info = vk_create_pipeline_shader_stage(VK_SHADER_STAGE_VERTEX_BIT, "main", vert_shader_module);
	VkPipelineShaderStageCreateInfo                frag_shader_stage_info = vk_create_pipeline_shader_stage(VK_SHADER_STAGE_FRAGMENT_BIT, "main", frag_shader_module);
	std::vector<VkPipelineShaderStageCreateInfo>   shader_stages          = {vert_shader_stage_info, frag_shader_stage_info};
	std::vector<VkVertexInputAttributeDescription> vertex_attribute_descriptions{};
	std::vector<VkVertexInputBindingDescription>   vertex_attribute_bindings{};
	auto                                           dimensions = a_renderpass.dimensions();
	auto                                           fillmode   = to_vulkan_triangle_fill_mode(setting.m_vulkan.m_polygon_mode);
	auto                                           samples    = a_device.samples_count();
	auto                                           alpha_to_coverage{false};
	auto                                           alpha_to_one{false};
	auto                                           blend_mode = (a_blend_mode == rhi::BlendMode::blend ? true : false);
	auto                                           pipeline_cache{a_device.platform_pipeline_cache()};
	VkRenderPass                                   render_pass{a_renderpass.platform_renderpass(0)};
	int32_t                                        render_subpass{get_subpass_index(a_renderpass, a_render_subpass)};

	assert(render_subpass != -1 && "Subpass searching via index failed");

	// NOTE: As this mentions Front face winding and cull mode are dynamic states so doesn't require to be set later
	std::vector<VkDynamicState> dynamic_states = {
	    VK_DYNAMIC_STATE_VIEWPORT,
	    VK_DYNAMIC_STATE_SCISSOR,
	    VK_DYNAMIC_STATE_CULL_MODE_EXT,
	    VK_DYNAMIC_STATE_FRONT_FACE_EXT,
	    VK_DYNAMIC_STATE_LINE_WIDTH};

	vertex_attribute_descriptions.reserve(a_vulkan_descriptor.size() / 2);
	vertex_attribute_bindings.reserve(a_vulkan_descriptor.size() / 2);

	for (auto &vk_desc : a_vulkan_descriptor)
	{
		vertex_attribute_descriptions.emplace_back(vk_desc.first);
		vertex_attribute_bindings.emplace_back(vk_desc.second);
	}

	VkPipelineVertexInputStateCreateInfo   pipeline_vertex_input_state_info           = vk_create_pipeline_vertex_input_state(vertex_attribute_descriptions, vertex_attribute_bindings);
	VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_info         = vk_create_pipeline_input_assembly_state(to_vulkan_primitive_topoloy(a_topology));
	VkViewport                             viewport                                   = vk_create_viewport(0.0f, 0.0f, dimensions.x, dimensions.y, 0.0f, setting.m_depth_clear);
	VkRect2D                               scissor                                    = vk_create_scissor(0.0f, 0.0f, dimensions.x, dimensions.y);
	VkPipelineViewportStateCreateInfo      pipeline_viewport_state_info               = vk_create_viewport_state(viewport, scissor);
	VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_info          = vk_create_pipeline_rasterisation_state(fillmode, to_vulkan_cull_mode(a_cull_mode), to_vulkan_winding(a_winding));
	VkPipelineMultisampleStateCreateInfo   pipeline_multisampling_state_info          = vk_create_pipeline_multisample_state(samples, alpha_to_coverage, alpha_to_one);
	VkPipelineDepthStencilStateCreateInfo  pipeline_depth_stencil_state_info          = vk_create_depth_stencil_state(a_depth, a_depth, VK_COMPARE_OP_LESS_OR_EQUAL, VK_FALSE);
	VkPipelineColorBlendAttachmentState    pipeline_color_blend_attachment_state_info = vk_create_color_blend_attachment_state(blend_mode, a_premultiplied_alpha);
	VkPipelineColorBlendStateCreateInfo    pipeline_color_blend_state_info            = vk_create_color_blend_state(pipeline_color_blend_attachment_state_info);
	VkPipelineDynamicStateCreateInfo       pipeline_dynamic_state_info                = vk_create_dynamic_state(dynamic_states);
	VkPipelineLayoutCreateInfo             pipeline_layout_info                       = vk_create_pipeline_layout_state(a_vulkan_descriptors_layouts);
	VkPipelineLayout                       pipeline_layout                            = vk_create_pipeline_layout(device, pipeline_layout_info);

	VkPipeline graphics_pipeline = vk_create_graphics_pipeline(device, pipeline_cache,
	                                                           shader_stages,
	                                                           pipeline_vertex_input_state_info,
	                                                           pipeline_input_assembly_state_info,
	                                                           pipeline_viewport_state_info,
	                                                           pipeline_rasterization_state_info,
	                                                           pipeline_multisampling_state_info,
	                                                           pipeline_depth_stencil_state_info,
	                                                           pipeline_color_blend_state_info,
	                                                           pipeline_dynamic_state_info,
	                                                           pipeline_layout,
	                                                           render_pass,
	                                                           static_cast<uint32_t>(render_subpass));

	return reinterpret_cast<GraphicsPipelineState>(graphics_pipeline);
}

/*
// Roar uses the following sets and bindings in rough order, this is from low frequency to high frequency for a frequencey based descriptor set design
layout(set = 0, binding = 0) uniform per_frame_uniform;
layout(set = 0, binding = 1) buffer  nodes_models;                                   // Nodes model matrix updated once per frame
layout(set = 0, binding = 2) uniform nodes_offsets;                                  // Nodes offset goes hand in hand with nodes_models, this is the index of the matrix in the array
layout(set = 0, binding = 3) uniform sampler2D shadow_map_sampler;
layout(set = 0, binding = 4) uniform sampler2D brdf_integration_sampler;
layout(set = 0, binding = 5) uniform samplerCube skybox_sampler;
layout(set = 0, binding = 6) uniform samplerCube irradiance_sampler;
layout(set = 0, binding = 7) uniform samplerCube radiance_sampler;
layout(set = 0, binding = 8) uniform directional_light_uniform;
layout(set = 0, binding = 9) uniform point_light_uniform;
layout(set = 0, binding = 10) uniform spot_light_uniform;
layout(set = 0, binding = 11) buffer morphs_weights;                                  // All the morph weights in the scene are uploaded at once, hence per frame

layout(set = 1, binding = 0) uniform per_view_uniform;

layout(set = 2, binding = 0) buffer joint_offset_uniform
layout(set = 2, binding = 1) buffer joint_inverse_bind_matrices

layout(set = 3, binding = 0) uniform material_factors;
layout(set = 3, binding = 1) uniform highp sampler2D base_color_sampler;
layout(set = 3, binding = 2) uniform highp sampler2D emissive_sampler;
layout(set = 3, binding = 3) uniform highp sampler2D metallic_sampler;
layout(set = 3, binding = 4) uniform highp sampler2D roughness_sampler;
layout(set = 3, binding = 5) uniform highp sampler2D occlusion_sampler;
layout(set = 3, binding = 6) uniform highp sampler2D normal_sampler;
*/
template <typename _type>
FORCE_INLINE void add_material_component_descriptor(const ror::Material::Component<_type>                                             &a_component,
                                                    const std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>               &a_textures,
                                                    const std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>>     &a_images,
                                                    const std::vector<rhi::TextureSampler, rhi::BufferAllocator<rhi::TextureSampler>> &a_samplers,
                                                    uint32_t a_binding, DescriptorSet &a_set)
{
	if (a_component.m_texture != -1)
	{
		auto &texture = a_textures[ror::static_cast_safe<size_t>(a_component.m_texture)];
		assert(texture.texture_image() != -1);
		auto &image   = a_images[ror::static_cast_safe<size_t>(texture.texture_image())];
		auto &sampler = a_samplers[texture.texture_sampler()];

		VkDescriptorImageInfo image_info{vk_create_descriptor_image_info(image.platform_image_view(), sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
		a_set.push_image(a_binding, &image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
	}
}

void ProgramVulkan::allocate_descriptor(const VkDevice a_device, DescriptorSetLayoutCache &a_layout_cache, DescriptorPool &a_descriptor_pool, DescriptorSetCache &a_descriptor_cache, DescriptorSet &a_set)
{
	auto layout           = a_set.allocate(a_device, a_layout_cache, a_descriptor_pool);
	auto descriptor_index = a_descriptor_cache.emplace(std::move(a_set));
	this->m_platform_descriptors.push_back(descriptor_index);
	this->m_platform_descriptor_layouts.push_back(layout);
}

// TODO: Do per pass optimisation of the layout, is that an optimisatios or we need it.
// It should use *material, *skin
/**
 * @brief      Builds descriptor set layout for the PSO
 * @details    A very complicated method to create a complete descriptor set layout for this PSO
 * @param      a_device             Non-optional ror::Device
 * @param      a_renderer           Non-optional ror::Renderer
 * @param      a_per_view_ubo       Non-optional stuff that has camera/projection etc information. Per view needs to be provided by clients, dynamic mesh can have its own while gui has its own
                                    Alternative is to use per_view_ubo = a_renderer.shader_buffer("per_view_uniform"), but this doesn't work for gui

               All other parameters are optional or can be false. I am not using renderer and scene directly as one argument because getting those at places where I need to call this method isn't always possible.
               To get those and send it into this function the following methods can be used:

               auto  per_frame_ubo     = a_renderer.shader_buffer("per_frame_uniform");
               auto  model_ubo         = a_renderer.shader_buffer("nodes_models");
               auto  offset_ubo        = a_renderer.shader_buffer("nodes_offsets");
               auto  weights_ubo       = a_renderer.shader_buffer("morphs_weights");
               auto  directional_light = (a_scene ? a_scene->directional_light() : nullptr);
               auto  point_light       = (a_scene ? a_scene->point_light() : nullptr);
               auto  spot_light        = (a_scene ? a_scene->spot_light() : nullptr);
               auto  area_light        = (a_scene ? a_scene->area_light() : nullptr);
               auto &skin              = a_model.skins()[static_cast<size_t>(mesh.skin_index())];
               auto &material          = materials[static_cast<size_t>(material_index)];
               auto &meshes            = a_model->meshes();
               auto &mesh              = meshes[a_mesh_index];
               auto &model_textures    = a_model->textures();
               auto &model_images      = a_model->images();
               auto &model_samplers    = a_model->samplers();
               auto &materials         = a_model->materials();
               auto  material_index    = mesh.material(a_prim_index);
 */
void ProgramVulkan::build_descriptor(const rhi::Device &a_device, const ror::Renderer &a_renderer, const rhi::ShaderBuffer *a_per_view_ubo,
                                     const rhi::ShaderBuffer *a_per_frame_ubo, const rhi::ShaderBuffer *a_model_ubo, const rhi::ShaderBuffer *a_offset_ubo, const rhi::ShaderBuffer *a_weights_ubo,
                                     const ror::Light *directional_light, const ror::Light *point_light, const ror::Light *spot_light, const ror::Light *area_light,
                                     const ror::Material                                                               *a_material,
                                     const std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>               *a_textures,
                                     const std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>>     *a_images,
                                     const std::vector<rhi::TextureSampler, rhi::BufferAllocator<rhi::TextureSampler>> *a_samplers,
                                     const rhi::TextureImage *a_image, const rhi::TextureSampler *a_sampler,
                                     const ror::Skin *a_skin, bool a_need_shadow_map, bool a_with_environment)
{
	auto &setting                             = ror::settings();
	auto  device                              = a_device.platform_device();
	auto &descriptor_pool                     = a_device.descriptor_set_pool();
	auto &layout_cache                        = a_device.descriptor_set_layout_cache();
	auto &descriptor_cache                    = a_device.descriptor_set_cache();
	auto &renderer_images                     = a_renderer.images();
	auto  renderer_samplers                   = a_renderer.samplers();
	auto  shadow_map                          = a_renderer.get_shadow_texture();
	auto  per_frame_uniform_set               = setting.per_frame_uniform_set();
	auto  nodes_model_set                     = setting.nodes_model_set();
	auto  nodes_offset_set                    = setting.nodes_offset_set();
	auto  shadow_map_sampler_set              = setting.shadow_map_sampler_set();
	auto  brdf_integration_sampler_set        = setting.brdf_integration_sampler_set();
	auto  skybox_sampler_set                  = setting.skybox_sampler_set();
	auto  irradiance_sampler_set              = setting.irradiance_sampler_set();
	auto  radiance_sampler_set                = setting.radiance_sampler_set();
	auto  directional_light_set               = setting.directional_light_set();
	auto  point_light_set                     = setting.point_light_set();
	auto  spot_light_set                      = setting.spot_light_set();
	auto  area_light_set                      = setting.area_light_set();
	auto  morphs_weights_set                  = setting.morph_weights_set();
	auto  per_view_uniform_set                = setting.per_view_uniform_set();
	auto  joint_offset_uniform_set            = setting.skin_joints_set();
	auto  joint_inverse_bind_matrices_set     = setting.joint_inverse_bind_set();
	auto  material_factors_uniform_set        = setting.material_factors_set();
	auto  material_samplers_uniform_set       = setting.material_samplers_set();
	auto  per_frame_uniform_binding           = setting.per_frame_uniform_binding();
	auto  nodes_model_binding                 = setting.nodes_model_binding();
	auto  nodes_offset_binding                = setting.nodes_offset_binding();
	auto  shadow_map_sampler_binding          = setting.shadow_map_sampler_binding();
	auto  brdf_integration_sampler_binding    = setting.brdf_integration_sampler_binding();
	auto  skybox_sampler_binding              = setting.skybox_sampler_binding();
	auto  irradiance_sampler_binding          = setting.irradiance_sampler_binding();
	auto  radiance_sampler_binding            = setting.radiance_sampler_binding();
	auto  directional_light_binding           = setting.directional_light_binding();
	auto  point_light_binding                 = setting.point_light_binding();
	auto  spot_light_binding                  = setting.spot_light_binding();
	auto  area_light_binding                  = setting.area_light_binding();
	auto  morphs_weights_binding              = setting.morph_weights_binding();
	auto  per_view_uniform_binding            = setting.per_view_uniform_binding();
	auto  joint_offset_uniform_binding        = setting.skin_joints_binding();
	auto  joint_inverse_bind_matrices_binding = setting.joint_inverse_bind_binding();
	auto  material_factors_uniform_binding    = setting.material_factors_binding();
	auto  material_samplers_uniform_binding   = setting.material_samplers_binding();

	assert(shadow_map && shadow_map->texture_image() >= 0 && "Shadow index out of bound");

	auto &shadow_map_image   = renderer_images[static_cast<size_t>(shadow_map->texture_image())];
	auto &shadow_map_sampler = renderer_samplers[static_cast<size_t>(shadow_map->texture_sampler())];

	assert(per_frame_uniform_set == 0 && "per_frame_uniform_set should be zero");
	assert(nodes_model_set == 0 && "nodes_model_set should be zero");
	assert(nodes_offset_set == 0 && "nodes_offset_set should be zero");
	assert(shadow_map_sampler_set == 0 && "shadow_map_sampler_set should be zero");
	assert(brdf_integration_sampler_set == 0 && "brdf_integration_sampler_set should be zero");
	assert(skybox_sampler_set == 0 && "skybox_sampler_set should be zero");
	assert(irradiance_sampler_set == 0 && "irradiance_sampler_set should be zero");
	assert(radiance_sampler_set == 0 && "radiance_sampler_set should be zero");
	assert(directional_light_set == 0 && "directional_light_set should be zero");
	assert(point_light_set == 0 && "point_light_set should be zero");
	assert(spot_light_set == 0 && "spot_light_set should be zero");
	assert(area_light_set == 0 && "area_light_set should be zero");
	assert(morphs_weights_set == 0 && "morph_weights_set should be zero");
	assert(per_view_uniform_set == 1 && "per_view_uniform_set should be one");
	assert(joint_offset_uniform_set == 2 && "joint_offset_uniform_set should be two");
	assert(joint_inverse_bind_matrices_set == 2 && "inverse_bind_matrices_set should be two");
	assert(material_factors_uniform_set == 3 && "material_factors_uniform_set should be three");
	assert(material_samplers_uniform_set == 3 && "material_samplers_uniform_set should be three");

	(void) per_frame_uniform_set;
	(void) nodes_model_set;
	(void) nodes_offset_set;
	(void) shadow_map_sampler_set;
	(void) brdf_integration_sampler_set;
	(void) skybox_sampler_set;
	(void) irradiance_sampler_set;
	(void) radiance_sampler_set;
	(void) directional_light_set;
	(void) point_light_set;
	(void) spot_light_set;
	(void) area_light_set;
	(void) morphs_weights_set;
	(void) per_view_uniform_set;
	(void) joint_offset_uniform_set;
	(void) joint_inverse_bind_matrices_set;
	(void) material_factors_uniform_set;
	(void) material_samplers_uniform_set;

	// To make it reinterant clear the current layouts
	this->m_platform_descriptors.clear();
	this->m_platform_descriptor_layouts.clear();
	this->m_platform_descriptors.reserve(4);
	this->m_platform_descriptor_layouts.reserve(4);

	// Set 0 is the majority of per frame contstant stuff, like per frame uniform, lights, shadow map environment textures, morph weights etc
	{
		DescriptorSet set{};

		if (a_per_frame_ubo)
		{
			VkDescriptorBufferInfo per_frame_buffer_info{vk_create_descriptor_buffer_info(a_per_frame_ubo->platform_buffer())};
			set.push_buffer(per_frame_uniform_binding, &per_frame_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		}

		if (a_model_ubo)
		{
			VkDescriptorBufferInfo nodes_model_buffer_info{vk_create_descriptor_buffer_info(a_model_ubo->platform_buffer())};
			set.push_buffer(nodes_model_binding, &nodes_model_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		}

		if (a_offset_ubo)
		{
			VkDescriptorBufferInfo offset_buffer_info{vk_create_descriptor_buffer_info(a_offset_ubo->platform_buffer())};
			set.push_buffer(nodes_offset_binding, &offset_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		}

		// Shadow map
		if (a_need_shadow_map)
		{
			VkDescriptorImageInfo shadow_map_image_info{vk_create_descriptor_image_info(shadow_map_image.platform_image_view(), shadow_map_sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
			set.push_image(shadow_map_sampler_binding, &shadow_map_image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
		}

		// Environment
		if (a_with_environment)
		{
			auto &env = a_renderer.current_environment();

			auto &brdf_integration_image = renderer_images[ror::static_cast_safe<size_t>(env.brdf_integration())];
			auto &skybox_image           = renderer_images[ror::static_cast_safe<size_t>(env.skybox())];
			auto &irradiance_image       = renderer_images[ror::static_cast_safe<size_t>(env.irradiance())];
			auto &radiance_image         = renderer_images[ror::static_cast_safe<size_t>(env.radiance())];

			auto &sampler = a_renderer.samplers()[ror::static_cast_safe<size_t>(env.skybox_sampler())];

			VkDescriptorImageInfo brdf_integration_image_info{vk_create_descriptor_image_info(brdf_integration_image.platform_image_view(), sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
			VkDescriptorImageInfo skybox_image_info{vk_create_descriptor_image_info(skybox_image.platform_image_view(), sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
			VkDescriptorImageInfo irradiance_image_info{vk_create_descriptor_image_info(irradiance_image.platform_image_view(), sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
			VkDescriptorImageInfo radiance_image_info{vk_create_descriptor_image_info(radiance_image.platform_image_view(), sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};

			set.push_image(brdf_integration_sampler_binding, &brdf_integration_image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
			set.push_image(skybox_sampler_binding, &skybox_image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
			set.push_image(irradiance_sampler_binding, &irradiance_image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
			set.push_image(radiance_sampler_binding, &radiance_image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
		}

		// Lights
		if (directional_light)
		{
			VkDescriptorBufferInfo direction_light_info{vk_create_descriptor_buffer_info(directional_light->shader_buffer().platform_buffer())};
			set.push_buffer(directional_light_binding, &direction_light_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		}

		if (point_light)
		{
			VkDescriptorBufferInfo point_light_info{vk_create_descriptor_buffer_info(point_light->shader_buffer().platform_buffer())};
			set.push_buffer(point_light_binding, &point_light_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		}

		if (spot_light)
		{
			VkDescriptorBufferInfo spot_light_info{vk_create_descriptor_buffer_info(spot_light->shader_buffer().platform_buffer())};
			set.push_buffer(spot_light_binding, &spot_light_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		}

		if (area_light)
		{
			VkDescriptorBufferInfo area_light_info{vk_create_descriptor_buffer_info(area_light->shader_buffer().platform_buffer())};
			set.push_buffer(area_light_binding, &area_light_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		}

		if (a_weights_ubo)
		{
			VkDescriptorBufferInfo weights_info{vk_create_descriptor_buffer_info(a_weights_ubo->platform_buffer())};
			set.push_buffer(morphs_weights_binding, &weights_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		}

		// Now lets allocate set 0
		if (a_per_frame_ubo || a_model_ubo || a_offset_ubo ||
		    a_need_shadow_map || a_with_environment ||
		    directional_light || point_light || spot_light || area_light ||
		    a_weights_ubo)
			this->allocate_descriptor(device, layout_cache, descriptor_pool, descriptor_cache, set);
	}

	// Set 1 is per view uniform
	{
		assert(a_per_view_ubo && "Per view ubo can't be nullptr");
		DescriptorSet set{};

		// Its unconditional and must always be there
		VkDescriptorBufferInfo per_view_buffer_info{vk_create_descriptor_buffer_info(a_per_view_ubo->platform_buffer())};
		set.push_buffer(per_view_uniform_binding, &per_view_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

		// Now lets allocate set 1
		this->allocate_descriptor(device, layout_cache, descriptor_pool, descriptor_cache, set);
	}

	// Set 2 is mostly skin data, which doesn't change as often as meterials
	if (a_skin)
	{
		DescriptorSet set{};

		VkDescriptorBufferInfo joint_offset_uniform_buffer_info{vk_create_descriptor_buffer_info(a_skin->joint_offset_shader_buffer().platform_buffer())};
		VkDescriptorBufferInfo joint_inverse_bind_matrices_uniform_buffer_info{vk_create_descriptor_buffer_info(a_skin->joint_inverse_bind_shader_buffer().platform_buffer())};

		set.push_buffer(joint_offset_uniform_binding, &joint_offset_uniform_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		set.push_buffer(joint_inverse_bind_matrices_binding, &joint_inverse_bind_matrices_uniform_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

		// Now lets allocate set 2
		this->allocate_descriptor(device, layout_cache, descriptor_pool, descriptor_cache, set);
	}

	// Set 3 is Material factors and texture samplers
	{
		uint32_t      binding = material_samplers_uniform_binding;
		DescriptorSet set{};

		if (a_material)
		{
			assert(a_textures && a_images && a_samplers && "If material is provided textures, images and samplers are required too");

			VkDescriptorBufferInfo material_factors_uniform_buffer_info{vk_create_descriptor_buffer_info(a_material->shader_buffer().platform_buffer())};
			set.push_buffer(material_factors_uniform_binding, &material_factors_uniform_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

			auto &model_textures = *a_textures;
			auto &model_images   = *a_images;
			auto &model_samplers = *a_samplers;

			add_material_component_descriptor(a_material->m_base_color, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_diffuse_color, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_specular_glossyness, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_emissive, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_anisotropy, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_transmission, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_sheen_color, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_sheen_roughness, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_clearcoat_normal, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_clearcoat, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_clearcoat_roughness, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_metallic, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_roughness, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_occlusion, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_normal, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_bent_normal, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_height, model_textures, model_images, model_samplers, binding++, set);
			add_material_component_descriptor(a_material->m_subsurface_color, model_textures, model_images, model_samplers, binding++, set);
		}
		else if (a_image && a_sampler)
		{
			VkDescriptorImageInfo image_info{vk_create_descriptor_image_info(a_image->platform_image_view(), a_sampler->platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
			set.push_image(binding++, &image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
		}

		// Now lets allocate set 3
		if (binding != material_samplers_uniform_binding)        // Which means at least one image was inserted
			this->allocate_descriptor(device, layout_cache, descriptor_pool, descriptor_cache, set);
	}
}

// Used in Scene::upload and Scene::push_shader_updates mostly
void ProgramVulkan::upload(const rhi::Device &a_device, const rhi::Renderpass &a_renderpass, const rhi::Rendersubpass &a_subpass, const std::vector<rhi::Shader> &a_shaders,
                           const ror::Model &a_model, uint32_t a_mesh_index, uint32_t a_prim_index, bool a_premultiplied_alpha)
{
	auto        is_depth_shadow = (a_subpass.type() == rhi::RenderpassType::depth || a_subpass.type() == rhi::RenderpassType::shadow);
	auto       *device          = a_device.platform_device();
	const auto &mesh            = a_model.meshes()[a_mesh_index];
	const auto &materials       = a_model.materials();
	auto        material_index  = mesh.material(a_prim_index);

	// These are dynamic states so default are ok here
	auto winding   = rhi::PrimitiveWinding::counter_clockwise;
	auto cull_mode = rhi::PrimitiveCullMode::back;

	assert(device);
	assert(material_index != -1 && "Material index can't be -1");
	auto &material = materials[static_cast<size_t>(material_index)];

	this->release(a_device);

	auto vs_id = this->vertex_id();
	auto fs_id = this->fragment_id();
	auto cs_id = this->compute_id();

	assert((vs_id >= 0 || cs_id >= 0) && "Invalid vs shader id");
	assert((fs_id >= 0 || cs_id >= 0) && "Invalid fs shader id");

	if (vs_id < 0 || fs_id < 0)
	{
		ror::log_critical("No other pipeline supported yet, only vertex and fragment based");
		return;
	}

	const auto &vs = a_shaders[static_cast<size_t>(vs_id)];
	const auto &fs = a_shaders[static_cast<size_t>(fs_id)];

	// TODO: Create compute pipelines
	if (cs_id != -1)
	{
		ror::log_critical("Add mesh based compute shaders support");
	}

	if (vs.module() == nullptr)
	{
		ror::log_critical("Vertex module can't be null or empty");
		return;
	}

	auto vlk_vertex_descriptor = get_vulkan_vertex_descriptor(a_model.meshes(), a_mesh_index, a_prim_index, is_depth_shadow);
	this->m_pipeline_state     = create_fragment_render_pipeline(a_device, vs, fs, a_renderpass, a_subpass, vlk_vertex_descriptor, this->platform_descriptors_layouts(), material.m_blend_mode,
	                                                             mesh.primitive_type(a_prim_index), mesh.name().c_str(), a_subpass.has_depth(), a_premultiplied_alpha, cull_mode, winding);
}

// This one is used in Renderer::upload, with default vertex descriptor
void ProgramVulkan::upload(const rhi::Device &a_device, const rhi::Renderpass &a_pass, const rhi::Rendersubpass &a_subpass, const std::vector<rhi::Shader> &a_shaders, rhi::BuffersPack &a_buffer_pack, bool a_premultiplied_alpha)
{
	auto *device = a_device.platform_device();
	assert(device);
	(void) device;

	// TODO: Add support for non-mesh vertex and fragment pipelines, would require a RenderpassType as a must

	this->release(a_device);

	auto vs_id = this->vertex_id();
	auto fs_id = this->fragment_id();
	auto cs_id = this->compute_id();

	// TODO: Add support for single fragment shader programs
	assert(((vs_id >= 0 && fs_id >= 0) || cs_id >= 0) && "Invalid vs shader id");

	if (vs_id >= 0 && fs_id >= 0)
	{
		const auto &vs = a_shaders[static_cast<size_t>(vs_id)];
		const auto &fs = a_shaders[static_cast<size_t>(fs_id)];
		if (vs.module() != nullptr && fs.module() != nullptr)
		{
			// These are dynamic states so default are ok here
			rhi::PrimitiveCullMode cull_mode{rhi::PrimitiveCullMode::back};
			rhi::PrimitiveWinding  winding{rhi::PrimitiveWinding::counter_clockwise};

			auto vlk_vertex_descriptor = get_default_vlk_vertex_descriptor(a_buffer_pack);
			this->m_pipeline_state     = create_fragment_render_pipeline(a_device, vs, fs, a_pass, a_subpass, vlk_vertex_descriptor, this->platform_descriptors_layouts(), rhi::BlendMode::blend,
			                                                             rhi::PrimitiveTopology::triangles, "GlobalRenderPassPipeline", true, a_premultiplied_alpha, cull_mode, winding);
		}
	}
	else
	{
		const auto &cs = a_shaders[static_cast<size_t>(cs_id)];

		if (cs.module() == nullptr)
		{
			ror::log_critical("Compute function can't be null or empty");
			return;
		}

		this->m_pipeline_state = create_compute_pipeline(a_device, cs, this->platform_descriptors_layouts());

		if (!this->compute_pipeline_state())
		{
			ror::log_critical("Vulkan compute program creation failed with error:");
			return;
		}
	}
}

// Multiple uses, gui, some dynaminc mesh, and ProgramVulkan::upload below
void ProgramVulkan::upload(const rhi::Device &a_device, const rhi::Renderpass &a_renderpass, const rhi::Rendersubpass &a_subpass, const rhi::Shader &a_vs_shader, const rhi::Shader &a_fs_shader,
                           const rhi::VertexDescriptor &a_vertex_descriptor, rhi::BlendMode a_blend_mode, rhi::PrimitiveTopology a_toplogy, const char *a_pso_name,
                           bool a_subpass_has_depth, bool a_is_depth_shadow, bool a_premultiplied_alpha)
{
	auto *device = a_device.platform_device();
	assert(device);

	if (a_vs_shader.module() == nullptr)
	{
		ror::log_critical("Vertex function can't be null or empty");
		return;
	}

	if (a_fs_shader.module() == nullptr)
	{
		ror::log_critical("Fragment function can't be null or empty");
		return;
	}

	this->release(a_device);

	auto vlk_vertex_descriptor = get_vulkan_vertex_descriptor(a_vertex_descriptor, a_is_depth_shadow);
	this->m_pipeline_state     = create_fragment_render_pipeline(a_device, a_vs_shader, a_fs_shader, a_renderpass, a_subpass, vlk_vertex_descriptor, this->platform_descriptors_layouts(), a_blend_mode, a_toplogy, a_pso_name, a_subpass_has_depth, a_premultiplied_alpha);
}

// Used by debug geometry and environment gemoetry etc
void ProgramVulkan::upload(const rhi::Device &a_device, rhi::Renderpass &a_renderpass, rhi::Rendersubpass &a_subpass, const rhi::VertexDescriptor &a_vertex_descriptor, const std::vector<rhi::Shader> &a_shaders,
                           rhi::BlendMode a_blend_mode, rhi::PrimitiveTopology a_toplogy, const char *a_pso_name, bool a_subpass_has_depth, bool a_is_depth_shadow, bool a_premultiplied_alpha)
{
	auto vs_id = this->vertex_id();
	auto fs_id = this->fragment_id();
	auto cs_id = this->compute_id();

	// TODO: Add support for single fragment shader programs
	assert(((vs_id >= 0 && fs_id >= 0) || cs_id >= 0) && "Invalid shader id");

	if (vs_id >= 0 && fs_id >= 0)
	{
		const auto &vs = a_shaders[static_cast<size_t>(vs_id)];
		const auto &fs = a_shaders[static_cast<size_t>(fs_id)];

		this->upload(a_device, a_renderpass, a_subpass, vs, fs, a_vertex_descriptor, a_blend_mode, a_toplogy, a_pso_name, a_subpass_has_depth, a_is_depth_shadow, a_premultiplied_alpha);
	}
	else
	{
		const auto &cs = a_shaders[static_cast<size_t>(cs_id)];

		if (cs.module() == nullptr)
		{
			ror::log_critical("Compute function can't be null or empty");
			return;
		}

		this->release(a_device);

		this->m_pipeline_state = create_compute_pipeline(a_device, cs, this->platform_descriptors_layouts());

		if (!this->compute_pipeline_state())
		{
			ror::log_critical("Vulkan compute program creation failed with error");
			return;
		}
	}
}

// Simple compute creation method without requiring other vertex decriptor, renderpass, renderer etc
void ProgramVulkan::upload(const rhi::Device &a_device, const std::vector<rhi::Shader> &a_shaders)
{
	auto       *device = a_device.platform_device();
	auto        cs_id  = this->compute_id();
	const auto &cs     = a_shaders[static_cast<size_t>(cs_id)];
	assert(device);

	if (cs.module() == nullptr)
	{
		ror::log_critical("Compute function can't be null or empty");
		return;
	}

	this->release(a_device);

	this->m_pipeline_state = create_compute_pipeline(a_device, cs, this->platform_descriptors_layouts());

	if (!this->compute_pipeline_state())
	{
		ror::log_critical("Vulkan compute program creation failed with error");
		return;
	}
}
}        // namespace rhi
