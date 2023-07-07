
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
#include "graphics/rormodel.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_layout.hpp"
#include "rhi/vulkan/rorprogram.hpp"
#include "rhi/vulkan/rorshader.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace rhi
{
define_translation_unit_vtable(ProgramVulkan)
{}

void ProgramVulkan::release(rhi::Device &a_device)
{
	auto *device = a_device.platform_device();

	VkPipeline pipeline{nullptr};

	if (std::holds_alternative<ComputePipelineState>(this->m_pipeline_state))
		pipeline = reinterpret_cast<VkPipeline>(std::get<ComputePipelineState>(this->m_pipeline_state));
	else if (std::holds_alternative<GraphicsPipelineState>(this->m_pipeline_state))
		pipeline = reinterpret_cast<VkPipeline>(std::get<GraphicsPipelineState>(this->m_pipeline_state));

	vk_destroy_pipeline(device, pipeline);
}

static void append_to_vulkan_vertex_descriptor(std::vector<std::pair<VkVertexInputAttributeDescription, VkVertexInputBindingDescription>> &a_attributes,
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

	std::vector<std::pair<VkVertexInputAttributeDescription, VkVertexInputBindingDescription>> attributes;

	for (auto &attrib : vertex_descriptor.attributes())
		append_to_vulkan_vertex_descriptor(attributes, attrib, vertex_descriptor.layout(attrib.semantics()));

	return attributes;
}

static auto walk_vertex_descriptor_attributes(std::vector<std::pair<VkVertexInputAttributeDescription, VkVertexInputBindingDescription>> &a_attributes, const rhi::VertexDescriptor &a_vertex_descriptor, bool a_depth_shadow)
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

	std::vector<std::pair<VkVertexInputAttributeDescription, VkVertexInputBindingDescription>> vulkan_descriptor{};
	vulkan_descriptor.reserve(descriptors.size() * 10);        // Upper limit of how many attributes one can have

	for (auto &descriptor : descriptors)
		walk_vertex_descriptor_attributes(vulkan_descriptor, descriptor.get(), a_depth_shadow);

	return vulkan_descriptor;
}

static auto get_vulkan_vertex_descriptor(const rhi::VertexDescriptor &a_vertex_descriptor, bool a_depth_shadow)
{
	std::vector<std::pair<VkVertexInputAttributeDescription, VkVertexInputBindingDescription>> vulkan_descriptor{};

	walk_vertex_descriptor_attributes(vulkan_descriptor, a_vertex_descriptor, a_depth_shadow);

	return vulkan_descriptor;
}

// void create_graphics_pipeline()
static VkPipeline create_fragment_render_pipelineEX(VkDevice                                                                                   *a_device,
                                                    const rhi::Shader                                                                          &a_vertex_shader,
                                                    const rhi::Shader                                                                          &a_fragment_shader,
                                                    std::vector<std::pair<VkVertexInputAttributeDescription, VkVertexInputBindingDescription>> &vulkan_descriptor,
                                                    // VkVertexInputAttributeDescription *mtl_vertex_descriptor,
                                                    rhi::BlendMode         a_blend_mode,
                                                    rhi::PrimitiveTopology a_topology,
                                                    const char            *a_label,
                                                    bool                   a_depth,
                                                    bool                   a_premultiplied_alpha = false)
{
	(void) a_device;
	(void) a_vertex_shader;
	(void) a_fragment_shader;
	(void) vulkan_descriptor;
	(void) a_topology;
	(void) a_label;
	(void) a_depth;
	(void) a_blend_mode;
	(void) a_premultiplied_alpha;

	// VkShaderModule vert_shader_module;
	// VkShaderModule frag_shader_module;

	// vert_shader_module = this->create_shader_module("assets/shaders/tri.vert.spv");
	// frag_shader_module = this->create_shader_module("assets/shaders/tri.frag.spv");

	// VkPipelineShaderStageCreateInfo vert_shader_stage_info = vk_create_pipeline_shader_stage(VK_SHADER_STAGE_VERTEX_BIT, "main", vert_shader_module);
	// VkPipelineShaderStageCreateInfo frag_shader_stage_info = vk_create_pipeline_shader_stage(VK_SHADER_STAGE_FRAGMENT_BIT, "main", frag_shader_module);

	// VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info, frag_shader_stage_info};

	// // This is where you add where the vertex data is coming from
	// // TODO: To be abstracted later so it can be configured properly
	// auto vertex_attribute_descriptions = utl::get_astro_boy_vertex_attributes();
	// auto vertex_attribute_bindings     = utl::get_astro_boy_vertex_bindings();

	// VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_info = {};
	// pipeline_vertex_input_state_info.sType                                = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	// pipeline_vertex_input_state_info.pNext                                = nullptr;
	// pipeline_vertex_input_state_info.flags                                = 0;
	// pipeline_vertex_input_state_info.vertexBindingDescriptionCount        = vertex_attribute_bindings.size();
	// pipeline_vertex_input_state_info.pVertexBindingDescriptions           = vertex_attribute_bindings.data();
	// pipeline_vertex_input_state_info.vertexAttributeDescriptionCount      = vertex_attribute_descriptions.size();
	// pipeline_vertex_input_state_info.pVertexAttributeDescriptions         = vertex_attribute_descriptions.data();

	// VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_info = {};
	// pipeline_input_assembly_info.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	// pipeline_input_assembly_info.pNext                                  = nullptr;
	// pipeline_input_assembly_info.topology                               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	// pipeline_input_assembly_info.primitiveRestartEnable                 = VK_FALSE;

	// VkViewport viewport = {};
	// viewport.x          = 0.0f;
	// viewport.y          = 0.0f;
	// viewport.width      = static_cast<float32_t>(this->m_swapchain_extent.width);
	// viewport.height     = static_cast<float32_t>(this->m_swapchain_extent.height);
	// viewport.minDepth   = 0.0f;
	// viewport.maxDepth   = 1.0f;

	// VkRect2D scissor = {};
	// scissor.offset   = {0, 0};
	// scissor.extent   = this->m_swapchain_extent;

	// VkPipelineViewportStateCreateInfo pipeline_viewport_state_info = {};
	// pipeline_viewport_state_info.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	// pipeline_viewport_state_info.pNext                             = nullptr;
	// pipeline_viewport_state_info.flags                             = 0;
	// pipeline_viewport_state_info.viewportCount                     = 1;
	// pipeline_viewport_state_info.pViewports                        = &viewport;
	// pipeline_viewport_state_info.scissorCount                      = 1;
	// pipeline_viewport_state_info.pScissors                         = &scissor;

	// VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_info = {};
	// pipeline_rasterization_state_info.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	// pipeline_rasterization_state_info.pNext                                  = nullptr;
	// pipeline_rasterization_state_info.flags                                  = 0;
	// pipeline_rasterization_state_info.depthClampEnable                       = VK_FALSE;
	// pipeline_rasterization_state_info.rasterizerDiscardEnable                = VK_FALSE;
	// pipeline_rasterization_state_info.polygonMode                            = VK_POLYGON_MODE_FILL;
	// pipeline_rasterization_state_info.lineWidth                              = 1.0f;
	// pipeline_rasterization_state_info.cullMode                               = VK_CULL_MODE_BACK_BIT;
	// pipeline_rasterization_state_info.frontFace                              = VK_FRONT_FACE_COUNTER_CLOCKWISE;        // TODO: Model3d is counter clock wise fix this

	// pipeline_rasterization_state_info.depthBiasEnable         = VK_FALSE;
	// pipeline_rasterization_state_info.depthBiasConstantFactor = 0.0f;        // Optional
	// pipeline_rasterization_state_info.depthBiasClamp          = 0.0f;        // Optional
	// pipeline_rasterization_state_info.depthBiasSlopeFactor    = 0.0f;        // Optional

	// VkPipelineMultisampleStateCreateInfo pipeline_multisampling_state_info = {};
	// pipeline_multisampling_state_info.sType                                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	// pipeline_multisampling_state_info.pNext                                = nullptr;
	// pipeline_multisampling_state_info.flags                                = 0;
	// pipeline_multisampling_state_info.rasterizationSamples                 = this->get_sample_count();
	// pipeline_multisampling_state_info.sampleShadingEnable                  = (cfg::get_sample_rate_shading_enabled() ? VK_TRUE : VK_FALSE);
	// pipeline_multisampling_state_info.minSampleShading                     = (cfg::get_sample_rate_shading_enabled() ? cfg::get_sample_rate_shading() : 1.0f);
	// pipeline_multisampling_state_info.pSampleMask                          = nullptr;         // Optional
	// pipeline_multisampling_state_info.alphaToCoverageEnable                = VK_FALSE;        // Optional
	// pipeline_multisampling_state_info.alphaToOneEnable                     = VK_FALSE;        // Optional

	// VkPipelineDepthStencilStateCreateInfo pipeline_depth_stencil_info = {};
	// pipeline_depth_stencil_info.sType                                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	// pipeline_depth_stencil_info.pNext                                 = nullptr;
	// pipeline_depth_stencil_info.flags                                 = 0;
	// pipeline_depth_stencil_info.depthTestEnable                       = VK_TRUE;        // TODO: Depth testing should be enabled later
	// pipeline_depth_stencil_info.depthWriteEnable                      = VK_TRUE;
	// pipeline_depth_stencil_info.depthCompareOp                        = VK_COMPARE_OP_LESS_OR_EQUAL;
	// pipeline_depth_stencil_info.depthBoundsTestEnable                 = VK_FALSE;
	// pipeline_depth_stencil_info.stencilTestEnable                     = VK_FALSE;
	// pipeline_depth_stencil_info.front                                 = VkStencilOpState{};        // TODO: Needs fixing
	// pipeline_depth_stencil_info.back                                  = VkStencilOpState{};        // TODO: Needs fixing
	// pipeline_depth_stencil_info.minDepthBounds                        = 0.0f;
	// pipeline_depth_stencil_info.maxDepthBounds                        = 1.0f;

	// VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment_info = {};
	// pipeline_color_blend_attachment_info.blendEnable                         = VK_FALSE;
	// pipeline_color_blend_attachment_info.srcColorBlendFactor                 = VK_BLEND_FACTOR_ONE;         // Optional
	// pipeline_color_blend_attachment_info.dstColorBlendFactor                 = VK_BLEND_FACTOR_ZERO;        // Optional
	// pipeline_color_blend_attachment_info.colorBlendOp                        = VK_BLEND_OP_ADD;             // Optional
	// pipeline_color_blend_attachment_info.srcAlphaBlendFactor                 = VK_BLEND_FACTOR_ONE;         // Optional
	// pipeline_color_blend_attachment_info.dstAlphaBlendFactor                 = VK_BLEND_FACTOR_ZERO;        // Optional
	// pipeline_color_blend_attachment_info.alphaBlendOp                        = VK_BLEND_OP_ADD;             // Optional
	// pipeline_color_blend_attachment_info.colorWriteMask                      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	// /*
	//   // Could do simple alpha blending with this
	//   colorBlendAttachment.blendEnable = VK_TRUE;
	//   colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	//   colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	//   colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	//   colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	//   colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	//   colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	// */

	// VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_info = {};

	// pipeline_color_blend_state_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	// pipeline_color_blend_state_info.pNext             = nullptr;
	// pipeline_color_blend_state_info.flags             = 0;
	// pipeline_color_blend_state_info.logicOpEnable     = VK_FALSE;
	// pipeline_color_blend_state_info.logicOp           = VK_LOGIC_OP_COPY;
	// pipeline_color_blend_state_info.attachmentCount   = 1;
	// pipeline_color_blend_state_info.pAttachments      = &pipeline_color_blend_attachment_info;
	// pipeline_color_blend_state_info.blendConstants[0] = 0.0f;
	// pipeline_color_blend_state_info.blendConstants[1] = 0.0f;
	// pipeline_color_blend_state_info.blendConstants[2] = 0.0f;
	// pipeline_color_blend_state_info.blendConstants[3] = 0.0f;

	// VkDynamicState dynamic_states[] = {
	//     VK_DYNAMIC_STATE_VIEWPORT,
	//     // VK_DYNAMIC_STATE_CULL_MODE_EXT,
	//     // VK_DYNAMIC_STATE_FRONT_FACE_EXT,
	//     VK_DYNAMIC_STATE_LINE_WIDTH};

	// VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_info = {};
	// pipeline_dynamic_state_info.sType                            = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	// pipeline_dynamic_state_info.pNext                            = nullptr;
	// pipeline_dynamic_state_info.flags                            = 0;
	// pipeline_dynamic_state_info.dynamicStateCount                = 2;
	// pipeline_dynamic_state_info.pDynamicStates                   = dynamic_states;

	// VkPipelineLayoutCreateInfo pipeline_layout_info = {};
	// pipeline_layout_info.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	// pipeline_layout_info.pNext                      = nullptr;
	// pipeline_layout_info.flags                      = 0;
	// pipeline_layout_info.setLayoutCount             = 1;
	// pipeline_layout_info.pSetLayouts                = &this->m_descriptor_set_layout;
	// pipeline_layout_info.pushConstantRangeCount     = 0;              // Optional
	// pipeline_layout_info.pPushConstantRanges        = nullptr;        // Optional

	// VkResult result = vkCreatePipelineLayout(this->m_device, &pipeline_layout_info, cfg::VkAllocator, &this->m_pipeline_layout);
	// assert(result == VK_SUCCESS);

	// VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {};
	// graphics_pipeline_create_info.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	// graphics_pipeline_create_info.pNext                        = nullptr;
	// graphics_pipeline_create_info.flags                        = 0;
	// graphics_pipeline_create_info.stageCount                   = 2;
	// graphics_pipeline_create_info.pStages                      = shader_stages;
	// graphics_pipeline_create_info.pVertexInputState            = &pipeline_vertex_input_state_info;
	// graphics_pipeline_create_info.pInputAssemblyState          = &pipeline_input_assembly_info;
	// graphics_pipeline_create_info.pTessellationState           = nullptr;
	// graphics_pipeline_create_info.pViewportState               = &pipeline_viewport_state_info;
	// graphics_pipeline_create_info.pRasterizationState          = &pipeline_rasterization_state_info;
	// graphics_pipeline_create_info.pMultisampleState            = &pipeline_multisampling_state_info;
	// graphics_pipeline_create_info.pDepthStencilState           = &pipeline_depth_stencil_info;
	// graphics_pipeline_create_info.pColorBlendState             = &pipeline_color_blend_state_info;
	// graphics_pipeline_create_info.pDynamicState                = &pipeline_dynamic_state_info;
	// graphics_pipeline_create_info.layout                       = this->m_pipeline_layout;
	// graphics_pipeline_create_info.renderPass                   = this->m_render_pass;        // TODO: This is not created before hand
	// graphics_pipeline_create_info.subpass                      = 0;
	// graphics_pipeline_create_info.basePipelineHandle           = VK_NULL_HANDLE;
	// graphics_pipeline_create_info.basePipelineIndex            = -1;

	// result = vkCreateGraphicsPipelines(this->m_device, this->m_pipeline_cache, 1, &graphics_pipeline_create_info, cfg::VkAllocator, &this->m_graphics_pipeline);
	// assert(result == VK_SUCCESS);

	// // cleanup
	// vkDestroyShaderModule(this->m_device, vert_shader_module, cfg::VkAllocator);
	// vkDestroyShaderModule(this->m_device, frag_shader_module, cfg::VkAllocator);

	return VkPipeline{};
}

static auto create_fragment_render_pipeline(VkDevice                                                                                   a_device,
                                                  const rhi::Shader                                                                          &a_vertex_shader,
                                                  const rhi::Shader                                                                          &a_fragment_shader,
                                                  std::vector<std::pair<VkVertexInputAttributeDescription, VkVertexInputBindingDescription>> &vulkan_descriptor,
                                                  // VkVertexInputAttributeDescription *mtl_vertex_descriptor,
                                                  rhi::BlendMode         a_blend_mode,
                                                  rhi::PrimitiveTopology a_topology,
                                                  const char            *a_label,
                                                  bool                   a_depth,
                                                  bool                   a_premultiplied_alpha = false)
{
	(void) a_device;
	(void) a_vertex_shader;
	(void) a_fragment_shader;
	(void) vulkan_descriptor;
	(void) a_topology;
	(void) a_label;
	(void) a_blend_mode;
	(void) a_depth;
	(void) a_premultiplied_alpha;


	return GraphicsPipelineState{};

	/*
	auto      &setting                    = ror::settings();
	auto      *device                     = a_device;
	NS::Error *pError                     = nullptr;
	auto      *render_pipeline_descriptor = MTL::RenderPipelineDescriptor::alloc()->init();

	assert(device);
	assert(render_pipeline_descriptor && "Can't allocate vulkan render pipeline descriptor");

	const auto &vs = a_vertex_shader;
	const auto &fs = a_fragment_shader;

	if (vs.function() == nullptr)
	{
		ror::log_critical("Vertex function can't be null or empty");
		return nullptr;
	}

	render_pipeline_descriptor->setVertexFunction(vs.function());
	render_pipeline_descriptor->setFragmentFunction(fs.function());
	render_pipeline_descriptor->setVertexDescriptor(mtl_vertex_descriptor);

	if (a_depth)
		render_pipeline_descriptor->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth32Float);
	else
		render_pipeline_descriptor->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatInvalid);

	render_pipeline_descriptor->setSupportIndirectCommandBuffers(setting.m_vulkan.indirect_command_buffers);

	// Why there are colorAttachments in MTLRenderPipelineDescriptors as well as MTLRenderPassDescriptor the explanation is
	// https://stackoverflow.com/questions/44118942/what-is-colorattachmentn-in-vulkan but it boils down to how expensive these are to change
	// TODO: Add support for how ever many attachments we might have which is driven by render pass type
	ror::log_error("Fix colorAttachments() in render pipeline descriptor");
	auto colorAttachment = render_pipeline_descriptor->colorAttachments()->object(0);

	colorAttachment->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
	colorAttachment->setRgbBlendOperation(MTL::BlendOperationAdd);
	colorAttachment->setAlphaBlendOperation(MTL::BlendOperationAdd);
	colorAttachment->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
	// colorAttachment->setSourceAlphaBlendFactor(MTL::BlendFactorOne);
	colorAttachment->setSourceAlphaBlendFactor(MTL::BlendFactorSourceAlpha);
	colorAttachment->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
	colorAttachment->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);

	colorAttachment->setBlendingEnabled(false);

	if (a_blend_mode == rhi::BlendMode::blend)
	{
		colorAttachment->setBlendingEnabled(true);

		if (a_premultiplied_alpha)
			colorAttachment->setSourceRGBBlendFactor(MTL::BlendFactorOne);
	}

	render_pipeline_descriptor->setLabel(NS::String::string(a_label, NS::StringEncoding::UTF8StringEncoding));
	render_pipeline_descriptor->setRasterSampleCount(setting.m_multisample_count);
	render_pipeline_descriptor->setRasterizationEnabled(true);
	render_pipeline_descriptor->setInputPrimitiveTopology(to_vulkan_primitive_topoloy_class(a_topology));

	MTL::RenderPipelineState *pipeline_state = device->newRenderPipelineState(render_pipeline_descriptor, &pError);
	VkPipeline               *pipeline_state = nullptr;

	if (!pipeline_state)
	{
		assert(pError != nullptr && "Failed render pipeline creation didn't bring any errors");
		ror::log_critical("Vulkan render program creation failed with error: {}", pError->localizedDescription()->utf8String());
		return nullptr;
	}

	mtl_vertex_descriptor->release();
	render_pipeline_descriptor->release();

	if (pError != nullptr)
		pError->release();

	return pipeline_state;

	*/
}

void ProgramVulkan::upload(rhi::Device &a_device, const std::vector<rhi::Shader> &a_shaders, const ror::Model &a_model, uint32_t a_mesh_index, uint32_t a_prim_index, const rhi::Rendersubpass &a_subpass, bool a_premultiplied_alpha)
{
	(void) a_device;
	(void) a_subpass;
	(void) a_premultiplied_alpha;

	auto        is_depth_shadow = (a_subpass.type() == rhi::RenderpassType::depth || a_subpass.type() == rhi::RenderpassType::shadow);
	auto       *device          = a_device.platform_device();
	const auto &mesh            = a_model.meshes()[a_mesh_index];
	const auto &materials       = a_model.materials();
	auto        material_index  = mesh.material(a_prim_index);

	assert(device);
	assert(material_index != -1 && "Material index can't be -1");
	auto &material = materials[static_cast<size_t>(material_index)];

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
		ror::log_critical("Vertex function can't be null or empty");
		return;
	}

	auto vlk_vertex_descriptor = get_vulkan_vertex_descriptor(a_model.meshes(), a_mesh_index, a_prim_index, is_depth_shadow);
	this->m_pipeline_state     = create_fragment_render_pipeline(device, vs, fs, vlk_vertex_descriptor, material.m_blend_mode, mesh.primitive_type(a_prim_index), mesh.name().c_str(), a_subpass.has_depth(), a_premultiplied_alpha);
}

void ProgramVulkan::upload(rhi::Device &a_device, const rhi::Shader &a_vs_shader, const rhi::Shader &a_fs_shader, const rhi::VertexDescriptor & /* a_vertex_descriptor*/, rhi::BlendMode /* a_blend_mode*/,
                           rhi::PrimitiveTopology /* a_toplogy */, const char * /* a_pso_name */, bool /* a_subpass_has_depth */, bool /* a_is_depth_shadow */, bool /* a_premultiplied_alpha */)
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

	// auto *mtl_vertex_descriptor = get_vulkan_vertex_descriptor(a_vertex_descriptor, a_is_depth_shadow);
	// this->m_pipeline_state      = create_fragment_render_pipeline(device, a_vs_shader, a_fs_shader, mtl_vertex_descriptor, a_blend_mode, a_toplogy, a_pso_name, a_subpass_has_depth, a_premultiplied_alpha);
}

void ProgramVulkan::upload(rhi::Device &a_device, const std::vector<rhi::Shader> &a_shaders, rhi::BuffersPack & /* a_buffer_pack */, bool /* a_premultiplied_alpha */)
{
	auto *device = a_device.platform_device();

	// TODO: Add support for non-mesh vertex and fragment pipelines, would require a RenderpassType as a must

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
			// auto *mtl_vertex_descriptor = get_default_vlk_vertex_descriptor(a_buffer_pack);
			// this->m_pipeline_state      = create_fragment_render_pipeline(device, vs, fs, mtl_vertex_descriptor, rhi::BlendMode::blend, rhi::PrimitiveTopology::triangles, "GlobalRenderPassPipeline", true, a_premultiplied_alpha);
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

		// Don't need a descriptor but its possible to have one and create a compute pipeline from that
		// auto      *compute_pipeline_descriptor = MTL::ComputePipelineDescriptor::alloc()->init();
		// assert(compute_pipeline_descriptor && "Can't allocate vulkan compute pipeline descriptor");

		// NS::Error *pError = nullptr;

		assert(device);

		// this->m_pipeline_state = device->newComputePipelineState(cs.function(), &pError);

		if (!this->compute_pipeline_state())
		{
			ror::log_critical("Vulkan compute program creation failed with error:");
			return;
		}
	}
}
}        // namespace rhi
