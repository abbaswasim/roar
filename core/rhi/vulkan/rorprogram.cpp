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
#include "foundation/rorutilities.hpp"
#include "graphics/rorlight.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rorscene.hpp"
#include "profiling/rorlog.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_attribute.hpp"
#include "rhi/rorvertex_description.hpp"
#include "rhi/rorvertex_layout.hpp"
#include "rhi/vulkan/rordescriptor_set.hpp"
#include "rhi/vulkan/rorprogram.hpp"
#include "rhi/vulkan/rorshader.hpp"
#include "rhi/vulkan/rortexture.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include "settings/rorsettings.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace rhi
{
define_translation_unit_vtable(ProgramVulkan)
{}

using VulkanDescriptorVector = std::vector<std::pair<VkVertexInputAttributeDescription, VkVertexInputBindingDescription>>;

shader_resources_map read_shader_inputs(const std::vector<unsigned int> &a_spirv);

void ProgramVulkan::release(const rhi::Device &a_device)
{
	auto *device = a_device.platform_device();

	VkPipeline pipeline{this->render_pipeline_state()};

	if (pipeline == nullptr)
		pipeline = this->compute_pipeline_state();

	if (pipeline)
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

// static auto get_default_vertex_descriptor(rhi::BuffersPack &a_buffer_pack)
// {
// 	auto                 &setting = ror::settings();
// 	rhi::VertexDescriptor descriptor{};

// 	for (auto att_pair : setting.m_default_vertex_descriptor.attributes)
// 		descriptor.add(att_pair.first, att_pair.second, &a_buffer_pack);

// 	return descriptor;
// }

static auto get_default_vertex_descriptor(const shader_resources_map &a_vertex_reflection)
{
	assert(a_vertex_reflection.size() == 1 && "Not expected number of sets in the reflection");
	rhi::VertexDescriptor descriptor{};

	std::vector<rhi::VertexAttribute> attribs;
	std::vector<rhi::VertexLayout>    layouts;

	for (auto &sets : a_vertex_reflection)
	{
		uint32_t semantic       = 0;
		uint32_t semantic_index = 0;
		for (auto &input : sets.second)
		{
			assert(input.m_type == ShaderResourceType::input && "Shader resource not input");
			// These attributes and layouts doesn't have to be perfect, these are just dummy to make append_to_vulkan_vertex_descriptor work
			rhi::VertexAttribute va{input.m_location, 0, 1, 0, input.m_location, 0, static_cast<rhi::BufferSemantic>(semantic), input.m_format};
			rhi::VertexLayout    vl{input.m_location, vertex_format_to_bytes(input.m_format)};

			attribs.emplace_back(va);
			layouts.emplace_back(vl);

			semantic = 1u << semantic_index;
			semantic_index++;
		}
	}

	return rhi::VertexDescriptor{attribs, layouts};
}

// static auto get_default_vlk_vertex_descriptor(rhi::BuffersPack &a_buffer_pack)
// {
// 	auto vertex_descriptor = get_default_vertex_descriptor(a_buffer_pack);

// 	VulkanDescriptorVector attributes;

// 	for (auto &attrib : vertex_descriptor.attributes())
// 		append_to_vulkan_vertex_descriptor(attributes, attrib, vertex_descriptor.layout(attrib.semantics()));

// 	return attributes;
// }

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

static auto get_vulkan_vertex_descriptor(const rhi::Shader &a_vertex_shader)
{
	shader_resources_map vertex_shader_reflection = read_shader_inputs(a_vertex_shader.spirv());

	auto vertex_descriptor = get_default_vertex_descriptor(vertex_shader_reflection);

	VulkanDescriptorVector attributes;

	for (auto &attrib : vertex_descriptor.attributes())
		append_to_vulkan_vertex_descriptor(attributes, attrib, vertex_descriptor.layout(attrib.semantics()));

	return attributes;
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

static auto create_compute_pipeline(const rhi::Device &a_device,
                                    const rhi::Shader &a_compute_shader,
                                    VkPipelineLayout   a_pipeline_layout)

{
	auto                            device = a_device.platform_device();
	auto                            pipeline_cache{a_device.platform_pipeline_cache()};
	VkPipelineShaderStageCreateInfo compute_shader_stage_info = vk_create_pipeline_shader_stage(VK_SHADER_STAGE_COMPUTE_BIT, "main", a_compute_shader.module());

	auto compute_pipeline = vk_create_compute_pipeline(device, pipeline_cache, a_pipeline_layout, compute_shader_stage_info);

	return reinterpret_cast<ComputePipelineState>(compute_pipeline);
}

static auto create_fragment_render_pipeline(const rhi::Device            &a_device,
                                            const rhi::Shader            &a_vertex_shader,
                                            const rhi::Shader            &a_fragment_shader,
                                            const rhi::Renderpass        &a_renderpass,
                                            const rhi::Rendersubpass     &a_render_subpass,
                                            const VulkanDescriptorVector &a_vulkan_descriptor,
                                            VkPipelineLayout              a_pipeline_layout,
                                            rhi::BlendMode                a_blend_mode,
                                            rhi::PrimitiveTopology        a_topology,
                                            const char                   *a_label,
                                            bool                          a_depth,
                                            bool                          a_premultiplied_alpha = false,
                                            rhi::PrimitiveCullMode        a_cull_mode           = rhi::PrimitiveCullMode::back,
                                            rhi::PrimitiveWinding         a_winding             = rhi::PrimitiveWinding::counter_clockwise)
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

	std::vector<VkDynamicState> dynamic_states = {
	    VK_DYNAMIC_STATE_VIEWPORT,
	    VK_DYNAMIC_STATE_SCISSOR};
	// VK_DYNAMIC_STATE_LINE_WIDTH};

	if (setting.m_vulkan.m_extended_dynamic_state)
	{
		dynamic_states.emplace_back(VK_DYNAMIC_STATE_CULL_MODE_EXT);
		dynamic_states.emplace_back(VK_DYNAMIC_STATE_FRONT_FACE_EXT);
	}

	if (setting.m_vulkan.m_extended_dynamic_state3)
		dynamic_states.emplace_back(VK_DYNAMIC_STATE_POLYGON_MODE_EXT);

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
	                                                           a_pipeline_layout,
	                                                           render_pass,
	                                                           static_cast<uint32_t>(render_subpass));

	return reinterpret_cast<GraphicsPipelineState>(graphics_pipeline);
}

static rhi::VertexFormat spirv_type_to_vertex_format(const spirv_cross::SPIRType &a_spirv_type)
{
	// clang-format off
	if (a_spirv_type.basetype == spirv_cross::SPIRType::SByte)
	{
		switch (a_spirv_type.vecsize)
		{
		case 1: return rhi::VertexFormat::int8_1;
		case 2: return rhi::VertexFormat::int8_2;
		case 3: return rhi::VertexFormat::int8_3;
		case 4: return rhi::VertexFormat::int8_4;
		}
	}
	else if (a_spirv_type.basetype == spirv_cross::SPIRType::UByte)
	{
		switch (a_spirv_type.vecsize)
		{
		case 1: return rhi::VertexFormat::uint8_1;
		case 2: return rhi::VertexFormat::uint8_2;
		case 3: return rhi::VertexFormat::uint8_3;
		case 4: return rhi::VertexFormat::uint8_4;
		}
	}
	else if (a_spirv_type.basetype == spirv_cross::SPIRType::Short)
	{
		switch (a_spirv_type.vecsize)
		{
		case 1: return rhi::VertexFormat::int16_1;
		case 2: return rhi::VertexFormat::int16_2;
		case 3: return rhi::VertexFormat::int16_3;
		case 4: return rhi::VertexFormat::int16_4;
		}
	}
	else if (a_spirv_type.basetype == spirv_cross::SPIRType::UShort)
	{
		switch (a_spirv_type.vecsize)
		{
		case 1: return rhi::VertexFormat::uint16_1;
		case 2: return rhi::VertexFormat::uint16_2;
		case 3: return rhi::VertexFormat::uint16_3;
		case 4: return rhi::VertexFormat::uint16_4;
		}
	}
	else if (a_spirv_type.basetype == spirv_cross::SPIRType::Int)
	{
		switch (a_spirv_type.vecsize)
		{
		case 1: return rhi::VertexFormat::int32_1;
		case 2: return rhi::VertexFormat::int32_2;
		case 3: return rhi::VertexFormat::int32_3;
		case 4: return rhi::VertexFormat::int32_4;
		}
	}
	else if (a_spirv_type.basetype == spirv_cross::SPIRType::UInt)
	{
		switch (a_spirv_type.vecsize)
		{
		case 1: return rhi::VertexFormat::uint32_1;
		case 2: return rhi::VertexFormat::uint32_2;
		case 3: return rhi::VertexFormat::uint32_3;
		case 4: return rhi::VertexFormat::uint32_4;
		}
	}
	else if (a_spirv_type.basetype == spirv_cross::SPIRType::Float)
	{
		switch (a_spirv_type.vecsize)
		{
		case 1: return rhi::VertexFormat::float32_1;
		case 2: return rhi::VertexFormat::float32_2;
		case 3: return rhi::VertexFormat::float32_3;
		case 4: return rhi::VertexFormat::float32_4;
		}
	}
	else if (a_spirv_type.basetype == spirv_cross::SPIRType::Double)
	{
		switch (a_spirv_type.vecsize)
		{
		case 1: return rhi::VertexFormat::float64_1;
		case 2: return rhi::VertexFormat::float64_2;
		case 3: return rhi::VertexFormat::float64_3;
		case 4: return rhi::VertexFormat::float64_4;
		}
	}
	// clang-format on

	// This just means its not a vertex input that we support
	return rhi::VertexFormat::invalid;
}

void append_resource(const spirv_cross::Compiler  &a_compiler,
                     shader_resources_map         &a_resources,
                     const spirv_resources_vector &a_shader_resources,
                     ShaderResourceType            a_type)
{
	const auto descriptor_sets_count = ror::settings().m_vulkan.m_descriptor_sets_size;

	for (auto &resource : a_shader_resources)
	{
		ShaderResource shader_resource{};

		shader_resource.m_type     = a_type;
		shader_resource.m_name     = resource.name;
		shader_resource.m_set      = a_compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
		shader_resource.m_binding  = a_compiler.get_decoration(resource.id, spv::DecorationBinding);
		shader_resource.m_location = a_compiler.get_decoration(resource.id, spv::DecorationLocation);
		shader_resource.m_format   = spirv_type_to_vertex_format(a_compiler.get_type_from_variable(resource.id));

		if (a_type == ShaderResourceType::input)
		{
			assert(shader_resource.m_set == 0 && "Input must only be in set 0");
		}
		assert(shader_resource.m_set < descriptor_sets_count && "Descriptor set value not valid");
		(void) descriptor_sets_count;

		a_resources[shader_resource.m_set].emplace_back(shader_resource);
	}
}

// This is a macro and not a function because spirv_cross::CompilerGLSL has a deleted copy/move ctr in StreamBuffer
// Probably worth cacheing the compiler at some point
#define set_spirv_cross_compiler(spirv)                             \
	spirv_cross::CompilerGLSL compiler{a_spirv};                    \
	auto                      opts = compiler.get_common_options(); \
	opts.enable_420pack_extension  = true;                          \
	compiler.set_common_options(opts);                              \
	(void) 0

shader_resources_map read_shader_resources(const std::vector<unsigned int> &a_spirv)
{
	auto descriptor_sets_count = ror::settings().m_vulkan.m_descriptor_sets_size;

	// Provides compiler
	set_spirv_cross_compiler(a_spirv);

	// using shader_resources_map   = std::unordered_map<uint32_t, std::vector<ShaderResource>>;
	shader_resources_map resources;

	// Prime the only descriptor sets we support
	for (uint32_t i = 0; i < descriptor_sets_count; ++i)
		resources[i] = std::vector<ShaderResource>{};

	auto shader_resources = compiler.get_shader_resources();

	append_resource(compiler, resources, shader_resources.sampled_images, ShaderResourceType::sampled_image);
	append_resource(compiler, resources, shader_resources.uniform_buffers, ShaderResourceType::uniform_buffer);
	append_resource(compiler, resources, shader_resources.storage_buffers, ShaderResourceType::storage_buffer);
	append_resource(compiler, resources, shader_resources.storage_images, ShaderResourceType::storage_image);

	assert(shader_resources.separate_images.size() == 0 && "Don't support separate images");
	assert(shader_resources.separate_samplers.size() == 0 && "Don't support separate_samplers");
	assert(shader_resources.subpass_inputs.size() == 0 && "Don't support input attachments yet FIXME");
	assert(shader_resources.push_constant_buffers.size() == 0 && "Don't support push constant yet FIXME");
	// assert(shader_resources.stage_inputs.size() == 0 && "Don't support stage inputs yet FIXME"); // this is handled separately
	// assert(shader_resources.stage_outputs.size() == 0 && "Don't support stage output yet FIXME");
	// assert(shader_resources.builtin_inputs.size() == 0 && "Don't support builtin inputs yet FIXME");
	// assert(shader_resources.builtin_outputs.size() == 0 && "Don't support builtin outputs yet FIXME");

	// Don't support these for this usecase
	// append_resource(compiler, resources, shader_resources.subpass_inputs, ShaderResourceType::input_attachment);
	// append_resource(compiler, resources, shader_resources.separate_images, ShaderResourceType::separate_image);
	// append_resource(compiler, resources, shader_resources.separate_samplers, ShaderResourceType::separate_sampler);
	// append_resource(compiler, resources, shader_resources.stage_inputs, ShaderResourceType::input);
	// append_resource(compiler, resources, shader_resources.stage_outputs, ShaderResourceType::Output);
	// append_resource(compiler, resources, shader_resources.push_constant_buffers, ShaderResourceType::PushConstant);

	// Not of Resource type can't use append_resource but I also don't need them
	// append_resource(compiler, resources, shader_resources.builtin_inputs, ShaderResourceType::input);
	// append_resource(compiler, resources, shader_resources.builtin_outputs, ShaderResourceType::Output);

	return resources;
}

shader_resources_map read_shader_inputs(const std::vector<unsigned int> &a_spirv)
{
	// Provides compiler
	set_spirv_cross_compiler(a_spirv);

	// Prime the only descriptor set we support for inputs
	shader_resources_map resources;
	resources[0] = std::vector<ShaderResource>{};

	auto shader_resources = compiler.get_shader_resources();

	append_resource(compiler, resources, shader_resources.stage_inputs, ShaderResourceType::input);
	// append_resource(compiler, resources, shader_resources.stage_outputs, ShaderResourceType::input);        // Do I need this? will find out some day

	return resources;
}

const ShaderResource *shader_resource(const std::string &a_name, const std::vector<ShaderResource> &a_reflection)
{
	for (const auto &resource : a_reflection)
	{
		if (resource.m_name == a_name)
		{
			return &resource;
		}
	}

	return nullptr;
}

const ShaderResource *shader_resource(const std::string &a_name, const std::vector<ShaderResource> &a_vertex_reflection, const std::vector<ShaderResource> &a_fragment_reflection, const std::vector<ShaderResource> &a_compute_reflection)
{
	const ShaderResource *resource = shader_resource(a_name, a_vertex_reflection);

	if (!resource)
		resource = shader_resource(a_name, a_fragment_reflection);

	if (!resource)
		resource = shader_resource(a_name, a_compute_reflection);

	return resource;
}

void collect_unique_resources(std::unordered_map<uint32_t, ShaderResource *> &a_unique_resources, shader_resources_map &a_shader_reflection)
{
	for (auto &set_bindings : a_shader_reflection)
	{
		for (auto &resource : set_bindings.second)
		{
			auto inserted = a_unique_resources.insert(std::make_pair(static_cast<uint32_t>(ror::pair(resource.m_set, resource.m_binding)), &resource));
			if (!inserted.second)
			{
				// Reference https://community.khronos.org/t/are-sets-bindings-unique-per-stage-or-per-program/107123
				assert(resource.m_name == inserted.first->second->m_name && "Shaders has conflicting variables of the same set and binding, all set and bindings combinations accross PSO must be unique for a sepcific variable");
			}
		}
	}
}

void get_resource_bindings(const std::vector<rhi::Shader> &a_shaders, int32_t a_vertex_id, int32_t a_fragment_id, int32_t a_compute_id, int32_t a_tile_id, int32_t a_mesh_id,
                           shader_resources_map &vertex_shader_reflection,
                           shader_resources_map &fragment_shader_reflection,
                           shader_resources_map &compute_shader_reflection)
{
	auto sets_count = ror::settings().m_vulkan.m_descriptor_sets_size;

	// Need to prime these incase nothing below exists in the shader, makes the rest of the code easy to write
	for (uint32_t i = 0; i < sets_count; ++i)
	{
		vertex_shader_reflection[i]   = std::vector<ShaderResource>{};
		fragment_shader_reflection[i] = std::vector<ShaderResource>{};
		compute_shader_reflection[i]  = std::vector<ShaderResource>{};
	}

	if (a_vertex_id != -1)
	{
		auto vertex_shader       = a_shaders[static_cast<size_t>(a_vertex_id)];
		vertex_shader_reflection = read_shader_resources(vertex_shader.spirv());
	}

	if (a_fragment_id != -1)
	{
		auto fragment_shader       = a_shaders[static_cast<size_t>(a_fragment_id)];
		fragment_shader_reflection = read_shader_resources(fragment_shader.spirv());
	}

	if (a_compute_id != -1)
	{
		auto compute_shader       = a_shaders[static_cast<size_t>(a_compute_id)];
		compute_shader_reflection = read_shader_resources(compute_shader.spirv());
	}

	if (a_tile_id != -1 || a_mesh_id != -1)
	{
		assert(0 && "Don't support mesh or tile vulkan shaders yet");
	}
}

shader_resources_map get_unique_resource_bindings(const std::vector<rhi::Shader> &a_shaders, int32_t a_vertex_id, int32_t a_fragment_id, int32_t a_compute_id, int32_t a_tile_id, int32_t a_mesh_id)
{
	shader_resources_map                           vertex_shader_reflection{};
	shader_resources_map                           fragment_shader_reflection{};
	shader_resources_map                           compute_shader_reflection{};
	shader_resources_map                           unique_bindings{};
	std::unordered_map<uint32_t, ShaderResource *> unique_resources{};

	get_resource_bindings(a_shaders, a_vertex_id, a_fragment_id, a_compute_id, a_tile_id, a_mesh_id, vertex_shader_reflection, fragment_shader_reflection, compute_shader_reflection);

	collect_unique_resources(unique_resources, vertex_shader_reflection);
	collect_unique_resources(unique_resources, fragment_shader_reflection);
	collect_unique_resources(unique_resources, compute_shader_reflection);

	for (auto &ur : unique_resources)
		unique_bindings[ur.second->m_set].push_back(*ur.second);

	return unique_bindings;
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
FORCE_INLINE void add_material_component_descriptor(const ror::Material::Component<_type> &a_component, const ShaderResource *a_shader_resource,
                                                    const std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>               &a_textures,
                                                    const std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>>     &a_images,
                                                    const std::vector<rhi::TextureSampler, rhi::BufferAllocator<rhi::TextureSampler>> &a_samplers,
                                                    uint32_t &a_binding, DescriptorSet &a_set)
{
	if (a_component.m_texture != -1 && a_shader_resource)
	{
		// assert(a_shader_resource && "Material has texture but it doesn't exist in the shader");
		// assert(a_shader_resource->m_binding == a_binding && a_shader_resource->m_type == ShaderResourceType::sampled_image && "Material binding doesn't match its shader binding");
		// (void) a_shader_resource;

		auto &texture = a_textures[ror::static_cast_safe<size_t>(a_component.m_texture)];
		assert(texture.texture_image() != -1);
		auto &image   = a_images[ror::static_cast_safe<size_t>(texture.texture_image())];
		auto &sampler = a_samplers[texture.texture_sampler()];

		VkDescriptorImageInfo image_info{vk_create_descriptor_image_info(image.platform_image_view(), sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
		a_set.push_image(a_binding++, &image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
	}
}

void ProgramVulkan::allocate_descriptor(const VkDevice a_device, DescriptorSetLayoutCache &a_layout_cache, DescriptorPool &a_descriptor_pool, DescriptorSetCache &a_descriptor_cache, DescriptorSet &a_set, uint32_t a_set_id)
{
	assert(a_set_id < 4 && "Only support upto 4 descriptors");

	auto descriptor_set_layout = a_set.allocate(a_device, a_layout_cache, a_descriptor_pool, a_set_id);
	auto descriptor_index      = a_descriptor_cache.emplace(std::move(a_set));

	this->m_platform_descriptors[a_set_id]        = descriptor_index;
	this->m_platform_descriptor_layouts[a_set_id] = descriptor_set_layout;
}

DescriptorSet &ProgramVulkan::descriptor(DescriptorSetCache &a_descriptor_cache, uint32_t a_set_id)
{
	assert(a_set_id < 4 && "Only support upto 4 descriptors");

	return a_descriptor_cache.at(this->m_platform_descriptors[a_set_id]);
}

void ProgramVulkan::allocate_empty_descriptors(const VkDevice a_device, DescriptorSetLayoutCache &a_layout_cache, DescriptorPool &a_descriptor_pool, DescriptorSetCache &a_descriptor_cache)
{
	auto descriptor_sets_count = ror::settings().m_vulkan.m_descriptor_sets_size;

	for (size_t i = 0; i < descriptor_sets_count; ++i)
	{
		if (this->m_platform_descriptor_layouts[i] == nullptr)
		{
			DescriptorSet set{};
			this->allocate_descriptor(a_device, a_layout_cache, a_descriptor_pool, a_descriptor_cache, set, static_cast<uint32_t>(i));
		}
	}
}

// Builds a descriptor for all environment textures etc. Its always in set 0 but takes that as argument
void ProgramVulkan::environment_descriptor_set(const ror::Renderer &a_renderer, shader_resources_map &shaders_reflection, DescriptorSet &a_set, bool &a_allocate)
{
	auto &renderer_images   = a_renderer.images();
	auto  renderer_samplers = a_renderer.samplers();
	auto &env               = a_renderer.current_environment();
	auto &setting           = ror::settings();

	auto brdf_integration_sampler_binding = setting.brdf_integration_sampler_binding();
	auto skybox_sampler_binding           = setting.skybox_sampler_binding();
	auto irradiance_sampler_binding       = setting.irradiance_sampler_binding();
	auto radiance_sampler_binding         = setting.radiance_sampler_binding();

	const auto brdf_integration_sampler_str{"brdf_integration_sampler"};
	const auto skybox_sampler_str{"skybox_sampler"};
	const auto irradiance_sampler_str{"irradiance_sampler"};
	const auto radiance_sampler_str{"radiance_sampler"};

	const size_t set_0_id = 0;

	auto brdf_integration_sampler_rsrc = shader_resource(brdf_integration_sampler_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto skybox_sampler_rsrc           = shader_resource(skybox_sampler_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto irradiance_sampler_rsrc       = shader_resource(irradiance_sampler_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto radiance_sampler_rsrc         = shader_resource(radiance_sampler_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);

	auto has_brdf_integration_sampler = brdf_integration_sampler_rsrc != nullptr;
	auto has_skybox_sampler           = skybox_sampler_rsrc != nullptr;
	auto has_irradiance_sampler       = irradiance_sampler_rsrc != nullptr;
	auto has_radiance_sampler         = radiance_sampler_rsrc != nullptr;

	auto &brdf_integration_image = renderer_images[ror::static_cast_safe<size_t>(env.brdf_integration())];
	auto &skybox_image           = renderer_images[ror::static_cast_safe<size_t>(env.skybox())];
	auto &irradiance_image       = renderer_images[ror::static_cast_safe<size_t>(env.irradiance())];
	auto &radiance_image         = renderer_images[ror::static_cast_safe<size_t>(env.radiance())];
	auto &shared_sampler         = renderer_samplers[ror::static_cast_safe<size_t>(env.skybox_sampler())];

	VkDescriptorImageInfo brdf_integration_image_info{vk_create_descriptor_image_info(brdf_integration_image.platform_image_view(), shared_sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
	VkDescriptorImageInfo skybox_image_info{vk_create_descriptor_image_info(skybox_image.platform_image_view(), shared_sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
	VkDescriptorImageInfo irradiance_image_info{vk_create_descriptor_image_info(irradiance_image.platform_image_view(), shared_sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
	VkDescriptorImageInfo radiance_image_info{vk_create_descriptor_image_info(radiance_image.platform_image_view(), shared_sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};

	if (has_brdf_integration_sampler)
	{
		assert(brdf_integration_sampler_rsrc->m_binding == brdf_integration_sampler_binding &&
		       brdf_integration_sampler_rsrc->m_type == ShaderResourceType::sampled_image && "Buffer data and shader descriptors doesn't match");
		a_set.push_image(brdf_integration_sampler_binding, &brdf_integration_image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
		a_allocate = true;
	}

	if (has_skybox_sampler)
	{
		assert(skybox_sampler_rsrc->m_binding == skybox_sampler_binding &&
		       skybox_sampler_rsrc->m_type == ShaderResourceType::sampled_image && "Buffer data and shader descriptors doesn't match");
		a_set.push_image(skybox_sampler_binding, &skybox_image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
		a_allocate = true;
	}

	if (has_irradiance_sampler)
	{
		assert(irradiance_sampler_rsrc->m_binding == irradiance_sampler_binding &&
		       irradiance_sampler_rsrc->m_type == ShaderResourceType::sampled_image && "Buffer data and shader descriptors doesn't match");
		a_set.push_image(irradiance_sampler_binding, &irradiance_image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
		a_allocate = true;
	}

	if (has_radiance_sampler)
	{
		assert(radiance_sampler_rsrc->m_binding == radiance_sampler_binding &&
		       radiance_sampler_rsrc->m_type == ShaderResourceType::sampled_image && "Buffer data and shader descriptors doesn't match");
		a_set.push_image(radiance_sampler_binding, &radiance_image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
		a_allocate = true;
	}
}

// This one can be used when only descriptor writes needs updating and not the whole descriptor needs building
void ProgramVulkan::environment_descriptor_set_update(const rhi::Device &a_device, const ror::Renderer &a_renderer)
{
	// Should be getting the descriptor set from the list it should be number 0
	assert(this->m_platform_descriptors.size() && "There are no descriptors in the program yet");

	auto          &descriptor_cache  = a_device.descriptor_set_cache();
	DescriptorSet &set               = descriptor_cache.at(this->m_platform_descriptors[0]);        // Environment stuff is in descriptor set 0 based on frequency based architecture
	auto          &renderer_images   = a_renderer.images();
	auto           renderer_samplers = a_renderer.samplers();
	auto          &env               = a_renderer.current_environment();
	auto          &setting           = ror::settings();

	auto brdf_integration_sampler_binding = setting.brdf_integration_sampler_binding();
	auto skybox_sampler_binding           = setting.skybox_sampler_binding();
	auto irradiance_sampler_binding       = setting.irradiance_sampler_binding();
	auto radiance_sampler_binding         = setting.radiance_sampler_binding();

	auto &brdf_integration_image = renderer_images[ror::static_cast_safe<size_t>(env.brdf_integration())];
	auto &skybox_image           = renderer_images[ror::static_cast_safe<size_t>(env.skybox())];
	auto &irradiance_image       = renderer_images[ror::static_cast_safe<size_t>(env.irradiance())];
	auto &radiance_image         = renderer_images[ror::static_cast_safe<size_t>(env.radiance())];
	auto &shared_sampler         = renderer_samplers[ror::static_cast_safe<size_t>(env.skybox_sampler())];

	VkDescriptorImageInfo brdf_integration_image_info{vk_create_descriptor_image_info(brdf_integration_image.platform_image_view(), shared_sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
	VkDescriptorImageInfo skybox_image_info{vk_create_descriptor_image_info(skybox_image.platform_image_view(), shared_sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
	VkDescriptorImageInfo irradiance_image_info{vk_create_descriptor_image_info(irradiance_image.platform_image_view(), shared_sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
	VkDescriptorImageInfo radiance_image_info{vk_create_descriptor_image_info(radiance_image.platform_image_view(), shared_sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};

	set.push_binding_write(brdf_integration_sampler_binding, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &brdf_integration_image_info, nullptr);
	set.push_binding_write(skybox_sampler_binding, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &skybox_image_info, nullptr);
	set.push_binding_write(irradiance_sampler_binding, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &irradiance_image_info, nullptr);
	set.push_binding_write(radiance_sampler_binding, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &radiance_image_info, nullptr);

	set.update_writes();
}

// TODO: Do per pass optimisation of the layout, is that an optimisation or we need it.
// It should use *material, *skin
/**
 * @brief      Builds descriptor set layout for the PSO via reflection unlike build_descriptor
 * @details    A very complicated method to create a complete descriptor set layout for this PSO by doing shader reflection for the descriptors I support.
 * @param      a_device             Non-optional ror::Device
 * @param      a_renderer           Non-optional ror::Renderer
 * @param      a_shaders            Non-optional rhi::Shaders vector of the shaders that this program is suppose to read from
 * @param      a_need_shadow_map    Non-optional Does the descriptor uses shadow map or not
 * @param      a_with_environment   Non-optional Do we want to use environment for this program or not

               All other parameters are optional or can be false.
               To get those and send it into this function the following methods can be used:

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
void ProgramVulkan::build_descriptor(const rhi::Device &a_device, const ror::Renderer &a_renderer, const std::vector<rhi::Shader> &a_shaders, const ror::Scene *a_scene,
                                     const ror::Material                                                               *a_material,
                                     const std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>               *a_textures,
                                     const std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>>     *a_images,
                                     const std::vector<rhi::TextureSampler, rhi::BufferAllocator<rhi::TextureSampler>> *a_samplers,
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

	// clang-format off
	assert(per_frame_uniform_set == 0 && "per_frame_uniform_set should be zero");                     (void) per_frame_uniform_set;
	assert(nodes_model_set == 0 && "nodes_model_set should be zero");                                 (void) nodes_model_set;
	assert(nodes_offset_set == 0 && "nodes_offset_set should be zero");                               (void) nodes_offset_set;
	assert(shadow_map_sampler_set == 0 && "shadow_map_sampler_set should be zero");                   (void) shadow_map_sampler_set;
	assert(brdf_integration_sampler_set == 0 && "brdf_integration_sampler_set should be zero");       (void) brdf_integration_sampler_set;
	assert(skybox_sampler_set == 0 && "skybox_sampler_set should be zero");                           (void) skybox_sampler_set;
	assert(irradiance_sampler_set == 0 && "irradiance_sampler_set should be zero");                   (void) irradiance_sampler_set;
	assert(radiance_sampler_set == 0 && "radiance_sampler_set should be zero");                       (void) radiance_sampler_set;
	assert(directional_light_set == 0 && "directional_light_set should be zero");                     (void) directional_light_set;
	assert(point_light_set == 0 && "point_light_set should be zero");                                 (void) point_light_set;
	assert(spot_light_set == 0 && "spot_light_set should be zero");                                   (void) spot_light_set;
	assert(area_light_set == 0 && "area_light_set should be zero");                                   (void) area_light_set;
	assert(morphs_weights_set == 0 && "morph_weights_set should be zero");                            (void) morphs_weights_set;
	assert(per_view_uniform_set == 1 && "per_view_uniform_set should be one");                        (void) per_view_uniform_set;
	assert(joint_offset_uniform_set == 2 && "joint_offset_uniform_set should be two");                (void) joint_offset_uniform_set;
	assert(joint_inverse_bind_matrices_set == 2 && "inverse_bind_matrices_set should be two");        (void) joint_inverse_bind_matrices_set;
	assert(material_factors_uniform_set == 3 && "material_factors_uniform_set should be three");      (void) material_factors_uniform_set;
	assert(material_samplers_uniform_set == 3 && "material_samplers_uniform_set should be three");    (void) material_samplers_uniform_set;
	// clang-format on

	// To make it reinterant clear the current layouts
	this->clear_descriptor();

	const auto per_view_ubo_str{"per_view_uniform"};
	const auto per_frame_ubo_str{"per_frame_uniform"};
	const auto model_ubo_str{"nodes_models"};
	const auto offset_ubo_str{"nodes_offsets"};
	const auto weights_ubo_str{"morphs_weights"};
	const auto node_xform_input_ubo_str{"node_transform_input"};
	const auto node_xform_output_ubo_str{"node_transform_output"};
	const auto animations_ubo_str{"animations"};
	const auto anim_sampler_input_ubo_str{"animations_sampler_input"};
	const auto anim_sampler_output_ubo_str{"animations_sampler_output"};
	const auto current_anim_ubo_str{"current_animations"};
	const auto shadow_map_sampler_str{"shadow_map_sampler"};
	const auto directional_light_ubo_str{"directional_light_uniform"};
	const auto point_light_ubo_str{"point_light_uniform"};
	const auto spot_light_ubo_str{"spot_light_uniform"};
	const auto area_light_ubo_str{"area_light_uniform"};
	const auto joints_offset_ubo_str{"joint_offset_uniform"};
	const auto joints_IBM_ubo_str{"joint_inverse_bind_matrices"};
	const auto material_factors_ubo_str{"material_factors"};
	const auto base_color_sampler_str{"base_color_sampler"};
	const auto diffuse_color_sampler_str{"diffuse_color_sampler"};
	const auto specular_glossyness_sampler_str{"specular_glossyness_sampler"};
	const auto emissive_sampler_str{"emissive_sampler"};
	const auto anisotropy_sampler_str{"anisotropy_sampler"};
	const auto transmission_sampler_str{"transmission_sampler"};
	const auto sheen_color_sampler_str{"sheen_color_sampler"};
	const auto sheen_roughness_sampler_str{"sheen_roughness_sampler"};
	const auto clearcoat_normal_sampler_str{"clearcoat_normal_sampler"};
	const auto clearcoat_sampler_str{"clearcoat_sampler"};
	const auto clearcoat_roughness_sampler_str{"clearcoat_roughness_sampler"};
	const auto occlusion_sampler_str{"occlusion_sampler"};
	const auto roughness_sampler_str{"roughness_sampler"};
	const auto metallic_sampler_str{"metallic_sampler"};
	const auto height_sampler_str{"height_sampler"};
	const auto normal_sampler_str{"normal_sampler"};
	const auto bent_normal_sampler_str{"bent_normal_sampler"};
	const auto subsurface_color_sampler_str{"subsurface_color_sampler"};

	// Don't need to test/asser these, an exception will be through if don't exist
	auto per_view_ubo            = a_renderer.shader_buffer(per_view_ubo_str);
	auto per_frame_ubo           = a_renderer.shader_buffer(per_frame_ubo_str);
	auto model_ubo               = a_renderer.shader_buffer(model_ubo_str);
	auto offset_ubo              = a_renderer.shader_buffer(offset_ubo_str);
	auto weights_ubo             = a_renderer.shader_buffer(weights_ubo_str);
	auto node_xform_input_ubo    = a_renderer.shader_buffer(node_xform_input_ubo_str);
	auto node_xform_output_ubo   = a_renderer.shader_buffer(node_xform_output_ubo_str);
	auto animations_ubo          = a_renderer.shader_buffer(animations_ubo_str);
	auto anim_sampler_input_ubo  = a_renderer.shader_buffer(anim_sampler_input_ubo_str);
	auto anim_sampler_output_ubo = a_renderer.shader_buffer(anim_sampler_output_ubo_str);
	auto current_anim_ubo        = a_renderer.shader_buffer(current_anim_ubo_str);

	const size_t set_0_id = 0;
	const size_t set_1_id = 1;
	const size_t set_2_id = 2;
	const size_t set_3_id = 3;

	shader_resources_map shaders_reflection{get_unique_resource_bindings(a_shaders, this->vertex_id(), this->fragment_id(), this->compute_id(), this->tile_id(), this->mesh_id())};

	auto per_frame_ubo_rsrc               = shader_resource(per_frame_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto model_ubo_rsrc                   = shader_resource(model_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto offset_ubo_rsrc                  = shader_resource(offset_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto weights_ubo_rsrc                 = shader_resource(weights_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto node_xform_input_ubo_rsrc        = shader_resource(node_xform_input_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto node_xform_output_ubo_rsrc       = shader_resource(node_xform_output_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto animations_ubo_rsrc              = shader_resource(animations_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto anim_sampler_input_ubo_rsrc      = shader_resource(anim_sampler_input_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto anim_sampler_output_ubo_rsrc     = shader_resource(anim_sampler_output_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto current_anim_ubo_rsrc            = shader_resource(current_anim_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto shadow_map_sampler_rsrc          = shader_resource(shadow_map_sampler_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto directional_light_ubo_rsrc       = shader_resource(directional_light_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto point_light_ubo_rsrc             = shader_resource(point_light_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto spot_light_ubo_rsrc              = shader_resource(spot_light_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto area_light_ubo_rsrc              = shader_resource(area_light_ubo_str, shaders_reflection[set_0_id], shaders_reflection[set_0_id], shaders_reflection[set_0_id]);
	auto per_view_ubo_rsrc                = shader_resource(per_view_ubo_str, shaders_reflection[set_1_id], shaders_reflection[set_1_id], shaders_reflection[set_1_id]);
	auto joints_offset_ubo_rsrc           = shader_resource(joints_offset_ubo_str, shaders_reflection[set_2_id], shaders_reflection[set_2_id], shaders_reflection[set_2_id]);
	auto joints_IBM_ubo_rsrc              = shader_resource(joints_IBM_ubo_str, shaders_reflection[set_2_id], shaders_reflection[set_2_id], shaders_reflection[set_2_id]);
	auto material_factors_ubo_rsrc        = shader_resource(material_factors_ubo_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto base_color_sampler_rsrc          = shader_resource(base_color_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto diffuse_color_sampler_rsrc       = shader_resource(diffuse_color_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto specular_glossyness_sampler_rsrc = shader_resource(specular_glossyness_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto emissive_sampler_rsrc            = shader_resource(emissive_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto anisotropy_sampler_rsrc          = shader_resource(anisotropy_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto transmission_sampler_rsrc        = shader_resource(transmission_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto sheen_color_sampler_rsrc         = shader_resource(sheen_color_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto sheen_roughness_sampler_rsrc     = shader_resource(sheen_roughness_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto clearcoat_normal_sampler_rsrc    = shader_resource(clearcoat_normal_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto clearcoat_sampler_rsrc           = shader_resource(clearcoat_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto clearcoat_roughness_sampler_rsrc = shader_resource(clearcoat_roughness_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto occlusion_sampler_rsrc           = shader_resource(occlusion_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto roughness_sampler_rsrc           = shader_resource(roughness_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto metallic_sampler_rsrc            = shader_resource(metallic_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto height_sampler_rsrc              = shader_resource(height_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto normal_sampler_rsrc              = shader_resource(normal_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto bent_normal_sampler_rsrc         = shader_resource(bent_normal_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);
	auto subsurface_color_sampler_rsrc    = shader_resource(subsurface_color_sampler_str, shaders_reflection[set_3_id], shaders_reflection[set_3_id], shaders_reflection[set_3_id]);

	auto has_per_view_ubo            = per_view_ubo_rsrc != nullptr;
	auto has_per_frame_ubo           = per_frame_ubo_rsrc != nullptr;
	auto has_model_ubo               = model_ubo_rsrc != nullptr;
	auto has_offset_ubo              = offset_ubo_rsrc != nullptr;
	auto has_weights_ubo             = weights_ubo_rsrc != nullptr;
	auto has_node_xform_input_ubo    = node_xform_input_ubo_rsrc != nullptr;
	auto has_node_xform_output_ubo   = node_xform_output_ubo_rsrc != nullptr;
	auto has_animations_ubo          = animations_ubo_rsrc != nullptr;
	auto has_anim_sampler_input_ubo  = anim_sampler_input_ubo_rsrc != nullptr;
	auto has_anim_sampler_output_ubo = anim_sampler_output_ubo_rsrc != nullptr;
	auto has_current_anim_ubo        = current_anim_ubo_rsrc != nullptr;
	auto has_shadow_map_sampler      = shadow_map_sampler_rsrc != nullptr;
	auto has_directional_light_ubo   = directional_light_ubo_rsrc != nullptr;
	auto has_point_light_ubo         = point_light_ubo_rsrc != nullptr;
	auto has_spot_light_ubo          = spot_light_ubo_rsrc != nullptr;
	auto has_area_light_ubo          = area_light_ubo_rsrc != nullptr;
	auto has_joints_offset_ubo       = joints_offset_ubo_rsrc != nullptr;
	auto has_joints_IBM_ubo          = joints_IBM_ubo_rsrc != nullptr;
	auto has_material_factors_ubo    = material_factors_ubo_rsrc != nullptr;

	const ror::Light *directional_light{nullptr};
	const ror::Light *point_light{nullptr};
	const ror::Light *spot_light{nullptr};
	const ror::Light *area_light{nullptr};

	if (a_scene)
	{
		directional_light = a_scene->directional_light();
		point_light       = a_scene->point_light();
		spot_light        = a_scene->spot_light();
		area_light        = a_scene->area_light();
	}

	// Set 0 is the majority of per frame constant stuff, like per frame uniform, lights, shadow map environment textures, morph weights etc
	{
		DescriptorSet set{};
		bool          allocate{false};

		if (has_per_frame_ubo)
		{
			assert(per_frame_ubo_rsrc->m_binding == per_frame_uniform_binding &&
			       per_frame_ubo_rsrc->m_type == ShaderResourceType::uniform_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo per_frame_buffer_info{vk_create_descriptor_buffer_info(per_frame_ubo->platform_buffer())};
			set.push_buffer(per_frame_uniform_binding, &per_frame_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
			allocate = true;
		}

		if (has_model_ubo)
		{
			assert(model_ubo_rsrc->m_binding == nodes_model_binding &&
			       model_ubo_rsrc->m_type == ShaderResourceType::storage_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo nodes_model_buffer_info{vk_create_descriptor_buffer_info(model_ubo->platform_buffer())};
			set.push_buffer(nodes_model_binding, &nodes_model_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
			allocate = true;
		}

		if (has_offset_ubo)
		{
			assert(offset_ubo_rsrc->m_binding == nodes_offset_binding &&
			       offset_ubo_rsrc->m_type == ShaderResourceType::uniform_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo offset_buffer_info{vk_create_descriptor_buffer_info(offset_ubo->platform_buffer())};
			set.push_buffer(nodes_offset_binding, &offset_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
			allocate = true;
		}

		if (has_node_xform_input_ubo)
		{
			assert(node_xform_input_ubo_rsrc->m_type == ShaderResourceType::storage_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo info{vk_create_descriptor_buffer_info(node_xform_input_ubo->platform_buffer())};
			set.push_buffer(node_xform_input_ubo_rsrc->m_binding, &info, VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
			allocate = true;
		}

		if (has_node_xform_output_ubo)
		{
			assert(node_xform_output_ubo_rsrc->m_type == ShaderResourceType::storage_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo info{vk_create_descriptor_buffer_info(node_xform_output_ubo->platform_buffer())};
			set.push_buffer(node_xform_output_ubo_rsrc->m_binding, &info, VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
			allocate = true;
		}

		if (has_animations_ubo)
		{
			assert(animations_ubo_rsrc->m_type == ShaderResourceType::storage_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo info{vk_create_descriptor_buffer_info(animations_ubo->platform_buffer())};
			set.push_buffer(animations_ubo_rsrc->m_binding, &info, VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
			allocate = true;
		}

		if (has_anim_sampler_input_ubo)
		{
			assert(anim_sampler_input_ubo_rsrc->m_type == ShaderResourceType::storage_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo info{vk_create_descriptor_buffer_info(anim_sampler_input_ubo->platform_buffer())};
			set.push_buffer(anim_sampler_input_ubo_rsrc->m_binding, &info, VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
			allocate = true;
		}

		if (has_anim_sampler_output_ubo)
		{
			assert(anim_sampler_output_ubo_rsrc->m_type == ShaderResourceType::storage_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo info{vk_create_descriptor_buffer_info(anim_sampler_output_ubo->platform_buffer())};
			set.push_buffer(anim_sampler_output_ubo_rsrc->m_binding, &info, VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
			allocate = true;
		}

		if (has_current_anim_ubo)
		{
			assert(current_anim_ubo_rsrc->m_type == ShaderResourceType::storage_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo info{vk_create_descriptor_buffer_info(current_anim_ubo->platform_buffer())};
			set.push_buffer(current_anim_ubo_rsrc->m_binding, &info, VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
			allocate = true;
		}

		// Shadow map
		if (a_need_shadow_map && has_shadow_map_sampler)
		{
			assert(shadow_map_sampler_rsrc->m_binding == shadow_map_sampler_binding &&
			       shadow_map_sampler_rsrc->m_type == ShaderResourceType::sampled_image && "Buffer data and shader descriptors doesn't match");
			VkDescriptorImageInfo shadow_map_image_info{vk_create_descriptor_image_info(shadow_map_image.platform_image_view(), shadow_map_sampler.platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
			set.push_image(shadow_map_sampler_binding, &shadow_map_image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
			allocate = true;
		}

		// Environment
		if (a_with_environment)
		{
			this->environment_descriptor_set(a_renderer, shaders_reflection, set, allocate);
		}

		// Lights
		if (directional_light && has_directional_light_ubo)
		{
			assert(directional_light_ubo_rsrc->m_binding == directional_light_binding &&
			       directional_light_ubo_rsrc->m_type == ShaderResourceType::uniform_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo direction_light_info{vk_create_descriptor_buffer_info(directional_light->shader_buffer().platform_buffer())};
			set.push_buffer(directional_light_binding, &direction_light_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
			allocate = true;
		}

		if (point_light && has_point_light_ubo)
		{
			assert(point_light_ubo_rsrc->m_binding == point_light_binding &&
			       point_light_ubo_rsrc->m_type == ShaderResourceType::uniform_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo point_light_info{vk_create_descriptor_buffer_info(point_light->shader_buffer().platform_buffer())};
			set.push_buffer(point_light_binding, &point_light_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
			allocate = true;
		}

		if (spot_light && has_spot_light_ubo)
		{
			assert(spot_light_ubo_rsrc->m_binding == spot_light_binding &&
			       spot_light_ubo_rsrc->m_type == ShaderResourceType::uniform_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo spot_light_info{vk_create_descriptor_buffer_info(spot_light->shader_buffer().platform_buffer())};
			set.push_buffer(spot_light_binding, &spot_light_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
			allocate = true;
		}

		if (area_light && has_area_light_ubo)
		{
			assert(area_light_ubo_rsrc->m_binding == area_light_binding &&
			       area_light_ubo_rsrc->m_type == ShaderResourceType::uniform_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo area_light_info{vk_create_descriptor_buffer_info(area_light->shader_buffer().platform_buffer())};
			set.push_buffer(area_light_binding, &area_light_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
			allocate = true;
		}

		if (has_weights_ubo)
		{
			assert(weights_ubo_rsrc->m_binding == morphs_weights_binding &&
			       weights_ubo_rsrc->m_type == ShaderResourceType::storage_buffer && "Buffer data and shader descriptors doesn't match");
			VkDescriptorBufferInfo weights_info{vk_create_descriptor_buffer_info(weights_ubo->platform_buffer())};
			set.push_buffer(morphs_weights_binding, &weights_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
			allocate = true;
		}

		// Now lets allocate set 0
		if (allocate)
			this->allocate_descriptor(device, layout_cache, descriptor_pool, descriptor_cache, set, 0);
	}

	// Set 1 is per view uniform
	{
		if (has_per_view_ubo)
		{
			DescriptorSet set{};

			assert(per_view_ubo_rsrc->m_binding == per_view_uniform_binding &&
			       per_view_ubo_rsrc->m_type == ShaderResourceType::uniform_buffer && "Buffer data and shader descriptors doesn't match");

			VkDescriptorBufferInfo per_view_buffer_info{vk_create_descriptor_buffer_info(per_view_ubo->platform_buffer())};
			set.push_buffer(per_view_uniform_binding, &per_view_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

			// Now lets allocate set 1
			this->allocate_descriptor(device, layout_cache, descriptor_pool, descriptor_cache, set, 1);
		}
	}

	// Set 2 is mostly skin data, which doesn't change as often as meterials
	if (a_skin)
	{
		DescriptorSet set{};

		assert(has_joints_offset_ubo && has_joints_IBM_ubo && "Asking for Skin descriptor but the shader doesn't have skins data");
		(void) has_joints_offset_ubo;
		(void) has_joints_IBM_ubo;

		VkDescriptorBufferInfo joint_offset_uniform_buffer_info{vk_create_descriptor_buffer_info(a_skin->joint_offset_shader_buffer().platform_buffer())};
		VkDescriptorBufferInfo joint_inverse_bind_matrices_uniform_buffer_info{vk_create_descriptor_buffer_info(a_skin->joint_inverse_bind_shader_buffer().platform_buffer())};

		assert(joints_offset_ubo_rsrc->m_binding == joint_offset_uniform_binding &&
		       joints_offset_ubo_rsrc->m_type == ShaderResourceType::uniform_buffer && "Buffer data and shader descriptors doesn't match");
		assert(joints_IBM_ubo_rsrc->m_binding == joint_inverse_bind_matrices_binding &&
		       joints_IBM_ubo_rsrc->m_type == ShaderResourceType::uniform_buffer && "Buffer data and shader descriptors doesn't match");

		set.push_buffer(joint_offset_uniform_binding, &joint_offset_uniform_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		set.push_buffer(joint_inverse_bind_matrices_binding, &joint_inverse_bind_matrices_uniform_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

		// Now lets allocate set 2
		this->allocate_descriptor(device, layout_cache, descriptor_pool, descriptor_cache, set, 2);
	}

	// Set 3 is Material factors and texture samplers
	if (a_material)
	{
		uint32_t      binding = material_samplers_uniform_binding;
		DescriptorSet set{};

		assert(a_textures && a_images && a_samplers && "If material has textures, images and samplers are required too");
		assert(has_material_factors_ubo && "Material descriptor requested but no material factors found in shader");
		assert(material_factors_ubo_rsrc->m_binding == material_factors_uniform_binding &&
		       material_factors_ubo_rsrc->m_type == ShaderResourceType::uniform_buffer && "Buffer data and shader descriptors doesn't match");

		(void) has_material_factors_ubo;

		if (has_material_factors_ubo)
		{
			VkDescriptorBufferInfo material_factors_uniform_buffer_info{vk_create_descriptor_buffer_info(a_material->shader_buffer().platform_buffer())};
			set.push_buffer(material_factors_uniform_binding, &material_factors_uniform_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		}

		auto &model_textures = *a_textures;
		auto &model_images   = *a_images;
		auto &model_samplers = *a_samplers;

		add_material_component_descriptor(a_material->m_base_color, base_color_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_diffuse_color, diffuse_color_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_specular_glossyness, specular_glossyness_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_emissive, emissive_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_anisotropy, anisotropy_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_transmission, transmission_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_sheen_color, sheen_color_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_sheen_roughness, sheen_roughness_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_clearcoat_normal, clearcoat_normal_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_clearcoat, clearcoat_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_clearcoat_roughness, clearcoat_roughness_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_metallic, metallic_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_roughness, roughness_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_occlusion, occlusion_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_normal, normal_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_bent_normal, bent_normal_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_height, height_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);
		add_material_component_descriptor(a_material->m_subsurface_color, subsurface_color_sampler_rsrc, model_textures, model_images, model_samplers, binding, set);

		// Now lets allocate set 3
		this->allocate_descriptor(device, layout_cache, descriptor_pool, descriptor_cache, set, 3);
	}

	this->allocate_empty_descriptors(device, layout_cache, descriptor_pool, descriptor_cache);
}

void ProgramVulkan::build_descriptor(const rhi::Device &a_device, const ror::Renderer &a_renderer, const std::vector<rhi::Shader> &a_shaders, bool a_need_shadow_map, bool a_with_environment)
{
	this->build_descriptor(a_device, a_renderer, a_shaders, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, a_need_shadow_map, a_with_environment);
}

void ProgramVulkan::build_descriptor(const rhi::Device &a_device, const rhi::ShaderBuffer *a_shader_buffer, uint32_t buffer_binding,
                                     const rhi::TextureImage *a_image, const rhi::TextureSampler *a_sampler, uint32_t a_texture_binding)
{
	// To make it reinterant clear the current layouts
	this->clear_descriptor();

	// TODO: Should I do reflection here as well? don't think so because its so specialised method.
	auto  device           = a_device.platform_device();
	auto &descriptor_pool  = a_device.descriptor_set_pool();
	auto &layout_cache     = a_device.descriptor_set_layout_cache();
	auto &descriptor_cache = a_device.descriptor_set_cache();

	// Set 0 is shader buffer, would probably be something like per view
	{
		DescriptorSet set{};
		if (a_shader_buffer)
		{
			VkDescriptorBufferInfo per_view_buffer_info{vk_create_descriptor_buffer_info(a_shader_buffer->platform_buffer())};
			set.push_buffer(buffer_binding, &per_view_buffer_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		}

		if (a_image && a_sampler)
		{
			VkDescriptorImageInfo image_info{vk_create_descriptor_image_info(a_image->platform_image_view(), a_sampler->platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
			set.push_image(a_texture_binding, &image_info, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL);
		}

		// Now lets allocate the only set
		this->allocate_descriptor(device, layout_cache, descriptor_pool, descriptor_cache, set, 0);
	}

	this->allocate_empty_descriptors(device, layout_cache, descriptor_pool, descriptor_cache);
}

void ProgramVulkan::clear_descriptor()
{
	// this->updated_m_platform_descriptors.clear();
	// this->updated_m_platform_descriptor_layouts.clear();
	// this->updated_m_platform_descriptors.resize(4);               // Supporting only 4 sets for now
	// this->updated_m_platform_descriptor_layouts.resize(4);        // Supporting only 4 sets for now

	for (size_t i = 0; i < 4; ++i)
		this->m_platform_descriptor_layouts[i] = nullptr;
}

// Fully reflected descriptor building that requires push_binding with actual buffer and images later
void ProgramVulkan::build_descriptor(const rhi::Device &a_device, const std::vector<rhi::Shader> &a_shaders)
{
	auto  device                = a_device.platform_device();
	auto &descriptor_pool       = a_device.descriptor_set_pool();
	auto &layout_cache          = a_device.descriptor_set_layout_cache();
	auto &descriptor_cache      = a_device.descriptor_set_cache();
	auto  descriptor_sets_count = ror::settings().m_vulkan.m_descriptor_sets_size;

	// To make it reinterant clear the current layouts
	this->clear_descriptor();

	shader_resources_map       shaders_reflection{get_unique_resource_bindings(a_shaders, this->vertex_id(), this->fragment_id(), this->compute_id(), this->tile_id(), this->mesh_id())};
	std::vector<DescriptorSet> sets{descriptor_sets_count};

	for (uint32_t i = 0; i < descriptor_sets_count; ++i)
	{
		auto &reflected_set = shaders_reflection[i];
		auto &set           = sets[i];
		bool  allocate{false};

		for (auto &set_binding : reflected_set)
		{
			assert(set_binding.m_set == i && "Set id doesn't match the set number in the vector");

			if (set_binding.m_type == ShaderResourceType::uniform_buffer ||
			    set_binding.m_type == ShaderResourceType::storage_buffer ||
			    set_binding.m_type == ShaderResourceType::sampled_image ||
			    set_binding.m_type == ShaderResourceType::storage_image)
			{
				// NOTE: This type of descriptor setup also requires calling
				// push_binding(a_binding, a_type, VkDescriptorImageInfo *, VkDescriptorBufferInfo *)
				// later at some point, when we know the buffers and/or images
				set.push_binding(set_binding.m_binding, static_cast<VkDescriptorType>(set_binding.m_type), VK_SHADER_STAGE_ALL);
				allocate = true;
			}
			else
			{
				assert(0 && "Don't support this resource type, implement me");
			}
		}

		// Now lets allocate the only set
		if (allocate)
			this->allocate_descriptor(device, layout_cache, descriptor_pool, descriptor_cache, set, i);

		// Max frames in flight needs consideration because there needs to be a pool per frame
	}

	this->allocate_empty_descriptors(device, layout_cache, descriptor_pool, descriptor_cache);
}

// Each ShaderBuffer or TextureImage/TextureSampler comes with set and binding which can be used to link the resources with descriptors
void ProgramVulkan::update_descriptor(const rhi::Device &a_device, const ror::Renderer &a_renderer,
                                      descriptor_update_type &a_buffers_images,
                                      bool                    a_use_environment)
{
	auto  device                = a_device.platform_device();
	auto &descriptor_cache      = a_device.descriptor_set_cache();
	auto &renderer_images       = a_renderer.images();
	auto  renderer_samplers     = a_renderer.samplers();
	auto &env                   = a_renderer.current_environment();
	auto &setting               = ror::settings();
	auto  descriptor_sets_count = ror::settings().m_vulkan.m_descriptor_sets_size;

	if (a_use_environment)
	{
		auto brdf_integration_sampler_binding = setting.brdf_integration_sampler_binding();
		auto skybox_sampler_binding           = setting.skybox_sampler_binding();
		auto irradiance_sampler_binding       = setting.irradiance_sampler_binding();
		auto radiance_sampler_binding         = setting.radiance_sampler_binding();

		auto &brdf_integration_image = renderer_images[ror::static_cast_safe<size_t>(env.brdf_integration())];
		auto &skybox_image           = renderer_images[ror::static_cast_safe<size_t>(env.skybox())];
		auto &irradiance_image       = renderer_images[ror::static_cast_safe<size_t>(env.irradiance())];
		auto &radiance_image         = renderer_images[ror::static_cast_safe<size_t>(env.radiance())];
		auto &shared_sampler         = renderer_samplers[ror::static_cast_safe<size_t>(env.skybox_sampler())];

		const auto env_set = 0u;

		rhi::descriptor_variant brdf_texture       = std::make_pair(&brdf_integration_image, &shared_sampler);
		rhi::descriptor_variant skybox_texture     = std::make_pair(&skybox_image, &shared_sampler);
		rhi::descriptor_variant irradiance_texture = std::make_pair(&irradiance_image, &shared_sampler);
		rhi::descriptor_variant radiance_texture   = std::make_pair(&radiance_image, &shared_sampler);

		a_buffers_images[env_set].emplace_back(std::make_pair(brdf_texture, brdf_integration_sampler_binding));
		a_buffers_images[env_set].emplace_back(std::make_pair(skybox_texture, skybox_sampler_binding));
		a_buffers_images[env_set].emplace_back(std::make_pair(irradiance_texture, irradiance_sampler_binding));
		a_buffers_images[env_set].emplace_back(std::make_pair(radiance_texture, radiance_sampler_binding));
	}

	assert(this->m_platform_descriptors.size() && "There are no descriptors in the program yet");

	using texture_pair = std::pair<const rhi::TextureImage *, const rhi::TextureSampler *>;

	for (uint32_t i = 0; i < descriptor_sets_count; ++i)
	{
		DescriptorSet               *set_ptr{nullptr};
		rhi::descriptor_data_vector *set_data_ptr{nullptr};

		for (auto &pd : this->m_platform_descriptors)        // Lets find the set we have in this program
		{
			DescriptorSet &set = descriptor_cache.at(pd);
			if (set.set_id() == i)
			{
				set_ptr = &set;

				if (set_ptr->bindings().size())
					set_data_ptr = &a_buffers_images.at(i);

				break;
			}
		}

		if (!set_ptr || !set_data_ptr)
			continue;

		assert(set_ptr->bindings().size() && "There are no bindings in the set we are trying to update");
		assert(set_ptr->writes().size() == 0 && "There are already writes, can't re-create them. First call reset_writes() on the descriptor");

		for (auto layout_binding : set_ptr->bindings())
		{
			rhi::descriptor_data_pair *binding_data{nullptr};
			for (auto &s : *set_data_ptr)        // Lets find the binding we have in the data
			{
				if (s.second == layout_binding.binding)
				{
					binding_data = &s;
					break;
				}
			}

			assert(binding_data && "Can't find binding data for this binding");

			auto binding_id = binding_data->second;

			if (layout_binding.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
			{
				texture_pair &texture = std::get<texture_pair>(binding_data->first);

				auto image   = texture.first;
				auto sampler = texture.second;

				assert(image && sampler && "Image and sampler can't be found");

				VkDescriptorImageInfo image_info{vk_create_descriptor_image_info(image->platform_image_view(), sampler->platform_handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)};
				set_ptr->push_binding_write(binding_id, layout_binding.descriptorType, &image_info, nullptr);        // Creates a write descriptor for the descriptor set
			}
			else if (layout_binding.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE)
			{
				auto image = std::get<const rhi::TextureImage *>(binding_data->first);

				assert(image && "Image can't be found");

				VkDescriptorImageInfo image_info{vk_create_descriptor_image_info(image->platform_image_view(), VK_NULL_HANDLE, VK_IMAGE_LAYOUT_GENERAL)};
				set_ptr->push_binding_write(binding_id, layout_binding.descriptorType, &image_info, nullptr);        // Creates a write descriptor for the descriptor set
			}
			else if (layout_binding.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ||
			         layout_binding.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
			{
				const rhi::ShaderBuffer *buffer = std::get<const rhi::ShaderBuffer *>(binding_data->first);

				assert(buffer && "Buffer can't be found");

				VkDescriptorBufferInfo buffer_info{vk_create_descriptor_buffer_info(buffer->platform_buffer())};
				set_ptr->push_binding_write(binding_id, layout_binding.descriptorType, nullptr, &buffer_info);        // Creates a write descriptor for the descriptor set
			}
		}

		set_ptr->update_writes();
		set_ptr->update(device);
	}
}

// This one is used along side the version of build_descriptor which is used for things like materials, where the linking is already done, just need to call an update on it
void ProgramVulkan::update_descriptor(const rhi::Device &a_device)
{
	auto  device                = a_device.platform_device();
	auto &descriptor_cache      = a_device.descriptor_set_cache();
	auto  descriptor_sets_count = ror::settings().m_vulkan.m_descriptor_sets_size;

	for (uint32_t i = 0; i < descriptor_sets_count; ++i)
		this->descriptor(descriptor_cache, i).update(device);
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

	try
	{
		VkPipelineLayoutCreateInfo pipeline_layout_info = vk_create_pipeline_layout_state(this->platform_descriptors_layouts());
		this->m_pipeline_layout                         = vk_create_pipeline_layout(device, pipeline_layout_info);
		this->m_pipeline_state                          = create_fragment_render_pipeline(a_device, vs, fs, a_renderpass, a_subpass, vlk_vertex_descriptor, this->m_pipeline_layout, material.m_blend_mode,
		                                                                                  mesh.primitive_type(a_prim_index), mesh.name().c_str(), a_subpass.has_depth(), a_premultiplied_alpha, cull_mode, winding);
	}
	catch (VulkanValidationException &vve)
	{
		ror::log_critical("Validation error occured with message {}", vve.what());
		vs.print_source_unconditional();
		fs.print_source_unconditional();
	}
}

// This one is used in Renderer::upload, with default vertex descriptor
void ProgramVulkan::upload(const rhi::Device &a_device, const rhi::Renderpass &a_pass, const rhi::Rendersubpass &a_subpass, const std::vector<rhi::Shader> &a_shaders, rhi::BuffersPack &a_buffer_pack, bool a_premultiplied_alpha)
{
	auto *device = a_device.platform_device();
	assert(device);
	(void) device;
	(void) a_buffer_pack;

	// TODO: Add support for non-mesh vertex and fragment pipelines, would require a RenderpassType as a must

	this->release(a_device);

	auto vs_id = this->vertex_id();
	auto fs_id = this->fragment_id();
	auto cs_id = this->compute_id();

	// TODO: Add support for single fragment shader programs
	assert(((vs_id >= 0 && fs_id >= 0) || cs_id >= 0) && "Invalid vs shader id");

	VkPipelineLayoutCreateInfo pipeline_layout_info = vk_create_pipeline_layout_state(this->platform_descriptors_layouts());
	this->m_pipeline_layout                         = vk_create_pipeline_layout(device, pipeline_layout_info);

	if (vs_id >= 0 && fs_id >= 0)
	{
		const auto &vs = a_shaders[static_cast<size_t>(vs_id)];
		const auto &fs = a_shaders[static_cast<size_t>(fs_id)];
		if (vs.module() != nullptr && fs.module() != nullptr)
		{
			// These are dynamic states so default are ok here
			rhi::PrimitiveCullMode cull_mode{rhi::PrimitiveCullMode::back};
			rhi::PrimitiveWinding  winding{rhi::PrimitiveWinding::counter_clockwise};

			auto vlk_vertex_descriptor = get_vulkan_vertex_descriptor(vs);

			try
			{
				this->m_pipeline_state = create_fragment_render_pipeline(a_device, vs, fs, a_pass, a_subpass, vlk_vertex_descriptor, this->m_pipeline_layout, rhi::BlendMode::blend,
				                                                         rhi::PrimitiveTopology::triangles, "GlobalRenderPassPipeline", true, a_premultiplied_alpha, cull_mode, winding);
			}
			catch (VulkanValidationException &vve)
			{
				ror::log_critical("Validation error occured with message {}", vve.what());
				vs.print_source_unconditional();
				fs.print_source_unconditional();
			}
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

		try
		{
			this->m_pipeline_state = create_compute_pipeline(a_device, cs, this->m_pipeline_layout);
		}
		catch (VulkanValidationException &vve)
		{
			ror::log_critical("Validation error occured with message {}", vve.what());
			cs.print_source_unconditional();
		}

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

	try
	{
		VkPipelineLayoutCreateInfo pipeline_layout_info = vk_create_pipeline_layout_state(this->platform_descriptors_layouts());
		this->m_pipeline_layout                         = vk_create_pipeline_layout(device, pipeline_layout_info);
		this->m_pipeline_state                          = create_fragment_render_pipeline(a_device, a_vs_shader, a_fs_shader, a_renderpass, a_subpass, vlk_vertex_descriptor, this->m_pipeline_layout, a_blend_mode, a_toplogy, a_pso_name, a_subpass_has_depth, a_premultiplied_alpha);
	}
	catch (VulkanValidationException &vve)
	{
		ror::log_critical("Validation error occured with message {}", vve.what());
		a_vs_shader.print_source_unconditional();
		a_fs_shader.print_source_unconditional();
	}
}

// Used by debug geometry and environment gemoetry etc
void ProgramVulkan::upload(const rhi::Device &a_device, rhi::Renderpass &a_renderpass, rhi::Rendersubpass &a_subpass, const rhi::VertexDescriptor &a_vertex_descriptor, const std::vector<rhi::Shader> &a_shaders,
                           rhi::BlendMode a_blend_mode, rhi::PrimitiveTopology a_toplogy, const char *a_pso_name, bool a_subpass_has_depth, bool a_is_depth_shadow, bool a_premultiplied_alpha)
{
	auto device = a_device.platform_device();
	auto vs_id  = this->vertex_id();
	auto fs_id  = this->fragment_id();
	auto cs_id  = this->compute_id();

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

		try
		{
			VkPipelineLayoutCreateInfo pipeline_layout_info = vk_create_pipeline_layout_state(this->platform_descriptors_layouts());
			this->m_pipeline_layout                         = vk_create_pipeline_layout(device, pipeline_layout_info);
			this->m_pipeline_state                          = create_compute_pipeline(a_device, cs, this->m_pipeline_layout);
		}
		catch (VulkanValidationException &vve)
		{
			ror::log_critical("Validation error occured with message {}", vve.what());
			cs.print_source_unconditional();
		}

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
	auto        device = a_device.platform_device();
	auto        cs_id  = this->compute_id();
	const auto &cs     = a_shaders[static_cast<size_t>(cs_id)];
	assert(device);

	if (cs.module() == nullptr)
	{
		ror::log_critical("Compute function can't be null or empty");
		return;
	}

	this->release(a_device);

	try
	{
		VkPipelineLayoutCreateInfo pipeline_layout_info = vk_create_pipeline_layout_state(this->platform_descriptors_layouts());
		this->m_pipeline_layout                         = vk_create_pipeline_layout(device, pipeline_layout_info);
		this->m_pipeline_state                          = create_compute_pipeline(a_device, cs, this->m_pipeline_layout);
	}
	catch (VulkanValidationException &vve)
	{
		ror::log_critical("Validation error occured with message {}", vve.what());
		cs.print_source_unconditional();
	}

	if (!this->compute_pipeline_state())
	{
		ror::log_critical("Vulkan compute program creation failed with error");
		return;
	}
}
}        // namespace rhi
