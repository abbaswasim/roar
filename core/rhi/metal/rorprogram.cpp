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
#include "profiling/rorlog.hpp"
#include "rhi/metal/rormetal_common.hpp"
#include "rhi/rorcommand_buffer.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_attribute.hpp"
#include "rhi/rorvertex_description.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <vector>

#include <Metal/MTLDevice.hpp>
#include <Metal/MTLPixelFormat.hpp>
#include <Metal/MTLRenderPipeline.hpp>
#include <Metal/MTLVertexDescriptor.hpp>

namespace rhi
{
define_translation_unit_vtable(ProgramMetal)
{}

/*
Metal vertex attributes to shader and buffer mapping

uint32_t pos_loc = 4
uint32_t nor_loc = 0

struct Vertex
{
    float3 normal        [[attribute(pos_loc)]];
    float3 position      [[attribute(nor_loc)]];
};

These attribute(index) in shader needs to match atIndex:index from api side at buffer binding time

[cmd_encoder setVertexBuffer:normals offset:0 atIndex:pos_loc];
[cmd_encoder setVertexBuffer:positions offset:0 atIndex:nor_loc];

Although the order or sequence doesn't matter, the idices needs to match.
You can have all attributes in one buffer with offsets for each separate attribute like the following

[cmd_encoder setVertexBuffer:positions offset:position_offset atIndex:pos_loc];
[cmd_encoder setVertexBuffer:positions offset:normals_offset atIndex:nor_loc];

The offset required for interleaved can be provided via vertex attribute or at attach time for specific vertex [[attribute(pos_loc)]]
The order of attribute and layout in vertex attribute descriptor specification time doesn't matter the "index/location" matters
*/

static auto append_to_metal_vertex_descriptor(MTL::VertexDescriptor *mtl_vertex_descriptor, const rhi::VertexAttribute &attrib, const rhi::VertexDescriptor &descriptor)
{
	auto  attribute_index             = attrib.location();        // This doesn't have to be sequential, as long as it matches for an attribute to its corresponding layout
	auto *vertex_descriptor_attribute = mtl_vertex_descriptor->attributes()->object(attribute_index);

	vertex_descriptor_attribute->setFormat(to_metal_vertexformat(attrib.format()));
	vertex_descriptor_attribute->setBufferIndex(attribute_index);        // The index in the argument table for the associated vertex buffer
	vertex_descriptor_attribute->setOffset(static_cast<unsigned long>(attrib.buffer_offset() + attrib.offset()));

	auto &layout = descriptor.layout(attrib.semantics());

	auto *vertex_descriptor_layout = mtl_vertex_descriptor->layouts()->object(attribute_index);
	vertex_descriptor_layout->setStride(layout.stride());
	vertex_descriptor_layout->setStepFunction(to_metal_step_function(layout.step_function()));

	return mtl_vertex_descriptor;
}

static auto get_default_vertex_descriptor(rhi::BuffersPack &a_buffer_pack)
{
	auto                 &setting = ror::settings();
	rhi::VertexDescriptor descriptor{};

	for (auto att_pair : setting.m_default_vertex_descriptor.attributes)
		descriptor.add(att_pair.first, att_pair.second, &a_buffer_pack);

	return descriptor;
}

static auto get_default_mtl_vertex_descriptor(rhi::BuffersPack &a_buffer_pack)
{
	auto  vertex_descriptor     = get_default_vertex_descriptor(a_buffer_pack);
	auto *mtl_vertex_descriptor = MTL::VertexDescriptor::alloc()->init();

	for (auto &attrib : vertex_descriptor.attributes())
		append_to_metal_vertex_descriptor(mtl_vertex_descriptor, attrib, vertex_descriptor);

	return mtl_vertex_descriptor;
}

static auto walk_vertex_descriptor_attributes(MTL::VertexDescriptor *a_target, const rhi::VertexDescriptor &a_vertex_descriptor, bool a_depth_shadow)
{
	// Lets read all the descriptors we might need to use for normal attributes + morph target attributes
	const auto &attributes = a_vertex_descriptor.attributes();
	for (auto &attrib : attributes)
	{
		if (is_attribute_required_in_pass(attrib.semantics(), a_depth_shadow))
			append_to_metal_vertex_descriptor(a_target, attrib, a_vertex_descriptor);
	}
}

static auto get_metal_vertex_descriptor(const std::vector<ror::Mesh, rhi::BufferAllocator<ror::Mesh>> &a_meshes, uint32_t a_mesh_index, uint32_t a_prim_index, bool a_depth_shadow)
{
	const auto &mesh                     = a_meshes[a_mesh_index];
	const auto &vertex_descriptor        = mesh.vertex_descriptor(a_prim_index);
	const auto &morph_vertex_descriptors = mesh.target_descriptor(a_prim_index);

	auto *mtl_vertex_descriptor = MTL::VertexDescriptor::alloc()->init();

	// Lets read all the descriptors we might need to use for normal attributes + morph target attributes
	std::vector<std::reference_wrapper<const rhi::VertexDescriptor>> descriptors{std::cref(vertex_descriptor)};
	for (const auto &morph_descriptor : morph_vertex_descriptors)
		descriptors.emplace_back(morph_descriptor);

	for (auto &descriptor : descriptors)
		walk_vertex_descriptor_attributes(mtl_vertex_descriptor, descriptor.get(), a_depth_shadow);

	return mtl_vertex_descriptor;
}

static MTL::VertexDescriptor *get_metal_vertex_descriptor(const rhi::VertexDescriptor &a_vertex_descriptor, bool a_depth_shadow)
{
	if (a_vertex_descriptor.attributes().size() == 0)
		return nullptr;

	auto *mtl_vertex_descriptor = MTL::VertexDescriptor::alloc()->init();

	walk_vertex_descriptor_attributes(mtl_vertex_descriptor, a_vertex_descriptor, a_depth_shadow);

	return mtl_vertex_descriptor;
}

static MTL::RenderPipelineState *create_fragment_render_pipeline(MTL::Device              *a_device,
                                                                 const rhi::Shader        &a_vertex_shader,
                                                                 const rhi::Shader        &a_fragment_shader,
                                                                 const rhi::Renderpass    &a_renderpass,
                                                                 const rhi::Rendersubpass &a_render_subpass,
                                                                 MTL::VertexDescriptor    *mtl_vertex_descriptor,
                                                                 rhi::BlendMode            a_blend_mode,
                                                                 rhi::PrimitiveTopology    a_topology,
                                                                 const char               *a_label,
                                                                 bool                      a_depth,
                                                                 bool                      a_premultiplied_alpha = false)
{
	auto      &setting                    = ror::settings();
	auto      *device                     = a_device;
	NS::Error *pError                     = nullptr;
	auto      *render_pipeline_descriptor = MTL::RenderPipelineDescriptor::alloc()->init();

	assert(device);
	assert(render_pipeline_descriptor && "Can't allocate metal render pipeline descriptor");

	const auto &vs = a_vertex_shader;
	const auto &fs = a_fragment_shader;

	if (vs.function() == nullptr)
	{
		ror::log_critical("Vertex function can't be null or empty");
		return nullptr;
	}

	render_pipeline_descriptor->setVertexFunction(vs.function());
	render_pipeline_descriptor->setFragmentFunction(fs.function());
	if (mtl_vertex_descriptor)
		render_pipeline_descriptor->setVertexDescriptor(mtl_vertex_descriptor);

	if (a_depth)
		render_pipeline_descriptor->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth32Float);
	else
		render_pipeline_descriptor->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatInvalid);

	render_pipeline_descriptor->setSupportIndirectCommandBuffers(setting.m_metal.indirect_command_buffers);

	// Why there are colorAttachments in MTLRenderPipelineDescriptors as well as MTLRenderPassDescriptor the explanation is
	// https://stackoverflow.com/questions/44118942/what-is-colorattachmentn-in-metal but it boils down to how expensive these are to change
	uint32_t attachment_index{0};
	for (auto &rt_index : a_render_subpass.render_targets())
	{
		auto            &rendertarget            = subpass_render_target(a_renderpass, a_render_subpass, rt_index);
		rhi::PixelFormat attachment_format       = rendertarget.m_target_reference.get().format();
		auto             attachment_format_metal = to_metal_pixelformat(attachment_format);

		auto colorAttachment = render_pipeline_descriptor->colorAttachments()->object(attachment_index++);

		if (is_pixel_format_depth_format(attachment_format))
		{
			render_pipeline_descriptor->setDepthAttachmentPixelFormat(attachment_format_metal);
		}
		else
		{
			colorAttachment->setPixelFormat(attachment_format_metal);
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
		}
	}

	render_pipeline_descriptor->setLabel(NS::String::string(a_label, NS::StringEncoding::UTF8StringEncoding));
	render_pipeline_descriptor->setRasterSampleCount(setting.m_multisample_count);
	render_pipeline_descriptor->setRasterizationEnabled(true);
	render_pipeline_descriptor->setInputPrimitiveTopology(to_metal_primitive_topoloy_class(a_topology));

	MTL::RenderPipelineState *pipeline_state = device->newRenderPipelineState(render_pipeline_descriptor, &pError);

	if (!pipeline_state)
	{
		assert(pError != nullptr && "Failed render pipeline creation didn't bring any errors");
		ror::log_critical("Metal render program creation failed with error: {}", pError->localizedDescription()->utf8String());
		return nullptr;
	}

	if (mtl_vertex_descriptor)
		mtl_vertex_descriptor->release();

	render_pipeline_descriptor->release();

	if (pError != nullptr)
		pError->release();

	return pipeline_state;

	// Other setting functions that can be called on RPD
	/*
	  render_pipeline_descriptor->setAlphaToCoverageEnabled();
	  render_pipeline_descriptor->setAlphaToOneEnabled();
	  render_pipeline_descriptor->setMaxVertexAmplificationCount();
	  render_pipeline_descriptor->setStencilAttachmentPixelFormat();
	  render_pipeline_descriptor->setVertexBuffers();
	  render_pipeline_descriptor->setFragmentBuffers();
	  render_pipeline_descriptor->setTessellationPartitionMode();
	  render_pipeline_descriptor->setMaxTessellationFactor();
	  render_pipeline_descriptor->setTessellationFactorScaleEnabled();
	  render_pipeline_descriptor->setTessellationFactorFormat();
	  render_pipeline_descriptor->setTessellationControlPointIndexType();
	  render_pipeline_descriptor->setTessellationFactorStepFunction();
	  render_pipeline_descriptor->setTessellationOutputWindingOrder();
	  render_pipeline_descriptor->setBinaryArchives();
	  render_pipeline_descriptor->setVertexPreloadedLibraries();
	  render_pipeline_descriptor->setFragmentPreloadedLibraries();
	  render_pipeline_descriptor->setVertexLinkedFunctions();
	  render_pipeline_descriptor->setFragmentLinkedFunctions();
	  render_pipeline_descriptor->setSupportAddingVertexBinaryFunctions();
	  render_pipeline_descriptor->setSupportAddingFragmentBinaryFunctions();
	  render_pipeline_descriptor->setMaxVertexCallStackDepth();
	  render_pipeline_descriptor->setMaxFragmentCallStackDepth();
	*/
}

void ProgramMetal::upload(const rhi::Device &a_device, const rhi::Renderpass &a_renderpass, const rhi::Rendersubpass &a_subpass, const std::vector<rhi::Shader> &a_shaders,
                          const ror::Model &a_model, uint32_t a_mesh_index, uint32_t a_prim_index, bool a_premultiplied_alpha)
{
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

	if (vs.function() == nullptr)
	{
		ror::log_critical("Vertex function can't be null or empty");
		return;
	}

	this->release();

	auto *mtl_vertex_descriptor = get_metal_vertex_descriptor(a_model.meshes(), a_mesh_index, a_prim_index, is_depth_shadow);
	this->m_pipeline_state      = create_fragment_render_pipeline(device, vs, fs, a_renderpass, a_subpass, mtl_vertex_descriptor, material.m_blend_mode, mesh.primitive_type(a_prim_index), mesh.name().c_str(), a_subpass.has_depth(), a_premultiplied_alpha);
}

void ProgramMetal::release()
{
	auto rps = this->render_pipeline_state();
	auto cps = this->compute_pipeline_state();

	if (rps)
		rps->release();

	if (cps)
		cps->release();
}

void ProgramMetal::upload(const rhi::Device &a_device, const rhi::Renderpass &a_renderpass, const rhi::Rendersubpass &a_subpass, const rhi::Shader &a_vs_shader, const rhi::Shader &a_fs_shader,
                          const rhi::VertexDescriptor &a_vertex_descriptor, rhi::BlendMode a_blend_mode, rhi::PrimitiveTopology a_toplogy, const char *a_pso_name,
                          bool a_subpass_has_depth, bool a_is_depth_shadow, bool a_premultiplied_alpha)
{
	auto *device = a_device.platform_device();

	assert(device);

	if (a_vs_shader.function() == nullptr)
	{
		ror::log_critical("Vertex function can't be null or empty");
		return;
	}

	if (a_fs_shader.function() == nullptr)
	{
		ror::log_critical("Fragment function can't be null or empty");
		return;
	}

	this->release();

	auto *mtl_vertex_descriptor = get_metal_vertex_descriptor(a_vertex_descriptor, a_is_depth_shadow);
	this->m_pipeline_state      = create_fragment_render_pipeline(device, a_vs_shader, a_fs_shader, a_renderpass, a_subpass, mtl_vertex_descriptor, a_blend_mode, a_toplogy, a_pso_name, a_subpass_has_depth, a_premultiplied_alpha);
}

void ProgramMetal::upload(const rhi::Device &a_device, rhi::Renderpass &a_renderpass, rhi::Rendersubpass &a_subpass, const rhi::VertexDescriptor &a_vertex_descriptor, const std::vector<rhi::Shader> &a_shaders,
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

		if (cs.function() == nullptr)
		{
			ror::log_critical("Compute function can't be null or empty");
			return;
		}

		// Don't need a descriptor but its possible to have one and create a compute pipeline from that
		// auto      *compute_pipeline_descriptor = MTL::ComputePipelineDescriptor::alloc()->init();
		// assert(compute_pipeline_descriptor && "Can't allocate metal compute pipeline descriptor");
		NS::Error *pError = nullptr;

		auto *device = a_device.platform_device();
		assert(device);

		this->release();

		this->m_pipeline_state = device->newComputePipelineState(cs.function(), &pError);

		if (!this->compute_pipeline_state())
		{
			ror::log_critical("Metal compute program creation failed with error: {}", pError->localizedDescription()->utf8String());
			return;
		}
	}
}

void ProgramMetal::upload(const rhi::Device &a_device, const rhi::Renderpass &a_pass, const rhi::Rendersubpass &a_subpass, const std::vector<rhi::Shader> &a_shaders, rhi::BuffersPack &a_buffer_pack, bool a_premultiplied_alpha)
{
	auto *device = a_device.platform_device();

	// TODO: Add support for non-mesh vertex and fragment pipelines, would require a RenderpassType as a must

	this->release();

	auto vs_id = this->vertex_id();
	auto fs_id = this->fragment_id();
	auto cs_id = this->compute_id();

	// TODO: Add support for single fragment shader programs
	assert(((vs_id >= 0 && fs_id >= 0) || cs_id >= 0) && "Invalid vs shader id");

	if (vs_id >= 0 && fs_id >= 0)
	{
		const auto &vs = a_shaders[static_cast<size_t>(vs_id)];
		const auto &fs = a_shaders[static_cast<size_t>(fs_id)];

		if (vs.function() != nullptr && fs.function() != nullptr)
		{
			auto *mtl_vertex_descriptor = get_default_mtl_vertex_descriptor(a_buffer_pack);
			this->m_pipeline_state      = create_fragment_render_pipeline(device, vs, fs, a_pass, a_subpass, mtl_vertex_descriptor, rhi::BlendMode::blend, rhi::PrimitiveTopology::triangles, "GlobalRenderPassPipeline", true, a_premultiplied_alpha);
		}
	}
	else
	{
		const auto &cs = a_shaders[static_cast<size_t>(cs_id)];

		if (cs.function() == nullptr)
		{
			ror::log_critical("Compute function can't be null or empty");
			return;
		}

		// Don't need a descriptor but its possible to have one and create a compute pipeline from that
		// auto      *compute_pipeline_descriptor = MTL::ComputePipelineDescriptor::alloc()->init();
		// assert(compute_pipeline_descriptor && "Can't allocate metal compute pipeline descriptor");
		NS::Error *pError = nullptr;

		assert(device);

		this->m_pipeline_state = device->newComputePipelineState(cs.function(), &pError);

		if (!this->compute_pipeline_state())
		{
			ror::log_critical("Metal compute program creation failed with error: {}", pError->localizedDescription()->utf8String());
			return;
		}
	}
}

void ProgramMetal::upload(const rhi::Device &a_device, const std::vector<rhi::Shader> &a_shaders)
{
	auto       *device = a_device.platform_device();
	auto        cs_id  = this->compute_id();
	const auto &cs     = a_shaders[static_cast<size_t>(cs_id)];

	if (cs.function() == nullptr)
	{
		ror::log_critical("Compute function can't be null or empty");
		return;
	}

	// Don't need a descriptor but its possible to have one and create a compute pipeline from that
	// auto      *compute_pipeline_descriptor = MTL::ComputePipelineDescriptor::alloc()->init();
	// assert(compute_pipeline_descriptor && "Can't allocate metal compute pipeline descriptor");
	NS::Error *pError = nullptr;

	assert(device);

	this->release();

	this->m_pipeline_state = device->newComputePipelineState(cs.function(), &pError);

	if (!this->compute_pipeline_state())
	{
		ror::log_critical("Metal compute program creation failed with error: {}", pError->localizedDescription()->utf8String());
		return;
	}
}
}        // namespace rhi
