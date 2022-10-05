
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
#include "rhi/metal/rorprogram.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_attribute.hpp"
#include "rhi/rorvertex_description.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <vector>

#include <Metal/MTLPixelFormat.hpp>
#include <Metal/MTLRenderPipeline.hpp>
#include <Metal/MTLVertexDescriptor.hpp>

namespace rhi
{
define_translation_unit_vtable(ProgramMetal)
{}

static auto get_metal_vertex_descriptor(const std::vector<ror::Mesh, rhi::BufferAllocator<ror::Mesh>> &a_meshes, uint32_t a_mesh_index, uint32_t a_prim_index, bool a_depth_shadow)
{
	const auto &mesh                     = a_meshes[a_mesh_index];
	const auto &vertex_descriptor        = mesh.m_attribute_vertex_descriptors[a_prim_index];
	const auto &morph_vertex_descriptors = mesh.m_morph_targets_vertex_descriptors[a_prim_index];

	auto *mtl_vertex_descriptor = MTL::VertexDescriptor::alloc()->init();

	std::string att_str{};

	// Lets read all the descriptors we might need to use for normal attributes + morph target attributes
	std::vector<std::reference_wrapper<const rhi::VertexDescriptor>> descriptors{std::cref(vertex_descriptor)};
	for (const auto &morph_descriptor : morph_vertex_descriptors)
		descriptors.emplace_back(morph_descriptor);

	uint32_t layout_index = 0;
	for (auto &descriptor : descriptors)
	{
		const auto &attributes = descriptor.get().attributes();
		for (auto &attribs : attributes)
		{
			if (is_attribute_required_in_pass(attribs.semantics(), a_depth_shadow))
			{
				auto *vertex_descriptor_attribute = mtl_vertex_descriptor->attributes()->object(attribs.location());
				vertex_descriptor_attribute->setFormat(to_metal_vertexformat(attribs.format()));
				vertex_descriptor_attribute->setBufferIndex(layout_index);        // attribs.buffer_index());        // This is actually the layout index in the mtl_vertex_descriptor, 0 means first layout 1 means second and so on, "The index in the argument table for the associated vertex buffer"
				vertex_descriptor_attribute->setOffset(static_cast<unsigned long>(attribs.buffer_offset()));

				auto &layout = descriptor.get().layout(attribs.semantics());

				auto *vertex_descriptor_layout = mtl_vertex_descriptor->layouts()->object(layout_index++);        // layout.binding());

				vertex_descriptor_layout->setStride(layout.stride());
				vertex_descriptor_layout->setStepFunction(to_metal_step_function(layout.step_function()));
			}
		}
	}

	return mtl_vertex_descriptor;
}

void ProgramMetal::upload(rhi::Device &a_device, const std::vector<rhi::Shader> &a_shaders, const ror::Model &a_model, uint32_t a_mesh_index, uint32_t a_prim_index, rhi::RenderpassType a_renderpass_type)
{
	auto                           is_depth_shadow            = (a_renderpass_type == rhi::RenderpassType::depth || a_renderpass_type == rhi::RenderpassType::shadow);
	auto                          &setting                    = ror::settings();
	MTL::Device                   *device                     = a_device.platform_device();
	NS::Error                     *pError                     = nullptr;
	MTL::RenderPipelineDescriptor *render_pipeline_descriptor = MTL::RenderPipelineDescriptor::alloc()->init();

	assert(device);
	assert(render_pipeline_descriptor && "Can't allocate metal render pipeline descriptor");

	auto vs_id  = this->vertex_id();
	auto fs_id  = this->fragment_id();
	auto com_id = this->compute_id();

	assert((vs_id >= 0 || com_id >= 0) && "Invalid vs shader id");
	assert((fs_id >= 0 || com_id >= 0) && "Invalid fs shader id");

	if (vs_id < 0 || fs_id < 0)
	{
		ror::log_critical("No other pipeline supported yet, only vertex and fragment based");
		return;
	}

	const auto &vs = a_shaders[static_cast<size_t>(vs_id)];
	const auto &fs = a_shaders[static_cast<size_t>(fs_id)];

	(void) com_id;        // TODO: Create compute pipelines

	if (vs.function() == nullptr)
	{
		ror::log_critical("Vertex function can't be null or empty");
		return;
	}

	auto *mtl_vertex_descriptor = get_metal_vertex_descriptor(a_model.meshes(), a_mesh_index, a_prim_index, is_depth_shadow);

	render_pipeline_descriptor->setVertexFunction(vs.function());
	render_pipeline_descriptor->setFragmentFunction(fs.function());
	render_pipeline_descriptor->setVertexDescriptor(mtl_vertex_descriptor);
	render_pipeline_descriptor->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth32Float);        // TODO: Read from settings, and understand why I not "depthbuffer = device->newTexture(depth_descriptor);"
	render_pipeline_descriptor->setSupportIndirectCommandBuffers(setting.m_metal.indirect_command_buffers);

	// TODO: Add support for how many attachments do we have is driven by render pass type
	render_pipeline_descriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
	render_pipeline_descriptor->colorAttachments()->object(0)->setRgbBlendOperation(MTL::BlendOperationAdd);
	render_pipeline_descriptor->colorAttachments()->object(0)->setAlphaBlendOperation(MTL::BlendOperationAdd);
	render_pipeline_descriptor->colorAttachments()->object(0)->setSourceRGBBlendFactor(MTL::BlendFactorOne);
	render_pipeline_descriptor->colorAttachments()->object(0)->setSourceAlphaBlendFactor(MTL::BlendFactorOne);
	render_pipeline_descriptor->colorAttachments()->object(0)->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
	render_pipeline_descriptor->colorAttachments()->object(0)->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);

	render_pipeline_descriptor->colorAttachments()->object(0)->setBlendingEnabled(false);        // TODO: get from setting or material

	render_pipeline_descriptor->setLabel(NS::String::string(a_model.meshes()[a_mesh_index].m_name.c_str(), NS::StringEncoding::UTF8StringEncoding));

	this->m_render_pipeline_state = device->newRenderPipelineState(render_pipeline_descriptor, &pError);

	if (!this->m_render_pipeline_state)
	{
		ror::log_critical("Metal program creation failed with error: {}", pError->localizedDescription()->utf8String());
		return;
	}

	mtl_vertex_descriptor->release();
	render_pipeline_descriptor->release();

	pError->release();
}
}        // namespace rhi
