// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
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

#include "renderer/rorrenderer.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrender_command_encoder.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_attribute.hpp"
#include "rhi/rorvertex_layout.hpp"
#include "rhi/rorshader.hpp"
#include "rordynamic_mesh.hpp"
#include <cassert>
#include <vector>

namespace ror
{

/**
   To create an interleaved vertex descriptor with postition, uv and color use something like

    // Create vertex descriptor
    rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_2};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
    rhi::VertexLayout    vlp{0, 20};                                                                                       // binding, stride, rate, multiplier, function
    rhi::VertexAttribute vat{1, 8, 1, 0, 0, 0, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2};
    rhi::VertexLayout    vlt{0, 20};
    rhi::VertexAttribute vac{2, 16, 1, 0, 0, 0, rhi::BufferSemantic::vertex_color_0, rhi::VertexFormat::uint8_4_norm};
    rhi::VertexLayout    vlc{0, 20};

    // Create a list of vertex descriptors
    std::vector<rhi::VertexAttribute> vattribs{vap, vat, vac};
    // Create layouts
    std::vector<rhi::VertexLayout>    vlayouts{vlp, vlt, vlc};

    // Create a vertex descriptor from it
    this->m_vertex_descriptor = rhi::VertexDescriptor{vattribs, vlayouts};
 */

/**
 * @brief      Initialises internal state of the mesh, must be called if init_upload is not called
 * @param      a_device        The device to use
 * @param      a_topology      Whether its a triangle or point soup etc
 */
void DynamicMesh::init(const rhi::Device &a_device, rhi::PrimitiveTopology a_topology)
{
	this->device(&a_device);

	auto &setting = ror::settings();

	this->m_vertex_buffer.init(a_device, setting.m_gui.m_vertex_buffer_size);        // By default in shared mode
	this->m_index_buffer.init(a_device, setting.m_gui.m_index_buffer_size);          // By default in shared mode

	this->m_topology = a_topology;
}

void DynamicMesh::load_texture(const rhi::Device &a_device, std::filesystem::path a_texure_path)
{
	auto &texture = ror::load_resource(a_texure_path, ror::ResourceSemantic::textures);
	rhi::read_texture_from_resource(texture, this->m_texture_image);

	this->m_texture_image.upload(a_device);
	this->m_texture_sampler.upload(a_device);
	this->m_has_texture = true;
}

void DynamicMesh::set_texture(rhi::TextureImage *a_texture, rhi::TextureSampler *a_sampler)
{
	this->m_texture_image_external   = a_texture;
	this->m_texture_sampler_external = a_sampler;
}

void DynamicMesh::setup_vertex_descriptor(rhi::VertexDescriptor *a_descriptor)
{
	if (a_descriptor)
	{
		this->m_vertex_descriptor = std::move(*a_descriptor);
		for (auto &va : this->m_vertex_descriptor.attributes())
		{
			if (va.semantics() == rhi::BufferSemantic::vertex_position)
			{
				this->m_has_positions = true;
				continue;
			}
			if (va.semantics() == rhi::BufferSemantic::vertex_index)
			{
				this->m_has_indices = true;
				continue;
			}
		}
	}
	else        // otherwise lets create a default VD with xyz, uv and rgb indexed values
	{
		// Create vertex descriptor
		rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
		rhi::VertexLayout    vlp{0, 36};                                                                                       // binding, stride, rate, multiplier, function
		rhi::VertexAttribute vat{1, 12, 1, 0, 0, 0, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2};
		rhi::VertexLayout    vlt{0, 36};
		rhi::VertexAttribute vac{2, 20, 1, 0, 0, 0, rhi::BufferSemantic::vertex_color_0, rhi::VertexFormat::float32_4};
		rhi::VertexLayout    vlc{0, 36};
		rhi::VertexAttribute vai{3, 0, 0, 0, 0, 0, rhi::BufferSemantic::vertex_index, rhi::VertexFormat::uint16_1};
		rhi::VertexLayout    vli{0, 0};

		this->m_has_positions = true;
		this->m_has_indices   = true;

		std::vector<rhi::VertexAttribute> vattribs{vap, vat, vac, vai};
		std::vector<rhi::VertexLayout>    vlayouts{vlp, vlt, vlc, vli};

		this->m_vertex_descriptor = rhi::VertexDescriptor{vattribs, vlayouts};
	}
}

void DynamicMesh::setup_shaders(const ror::Renderer &a_renderer, rhi::BlendMode a_blend_mode, std::filesystem::path a_vertex_shader, std::filesystem::path a_fragment_shader)
{
	// Create shader program
	auto vs_shader = rhi::build_shader<rhi::Shader>(*this->m_device, a_vertex_shader);
	auto fs_shader = rhi::build_shader<rhi::Shader>(*this->m_device, a_fragment_shader);

	rhi::Renderpass    *pass{nullptr};
	rhi::Rendersubpass *subpass{nullptr};

	a_renderer.get_final_pass_subpass(&pass, &subpass);

	std::vector<rhi::Shader> shaders{vs_shader, vs_shader};

	this->m_shader_program.vertex_id(0);
	this->m_shader_program.fragment_id(1);

	this->m_shader_program.build_descriptor(*this->m_device, shaders);

	// This means it is only guaranteed to render in last/main render pass
	this->m_shader_program.upload(*this->m_device, *pass, *subpass, vs_shader, fs_shader, this->m_vertex_descriptor, a_blend_mode, this->m_topology, "tri_pso", true, false, false);
}

/**
 * @brief      Sets up a generic xvz,uv,rgb vertex attribute soup.
 * @details    Sets up a generic xvz,uv,rgb vertex attribute soup with vertex and index buffer.
 *             It initialises other things required like initialising vertex and index buffers.
 *             Creates a checkers texture to be used and uses default triangle.glsl[vert/frag] shaders.
 *             Will use shader buffer from the renderer not using its specific one.
 * @param      a_device        The device to use
 * @param      a_blend_mode    The blend mode for the mesh
 * @param      a_topology      Whether its a triangle or point soup etc
 */
void DynamicMesh::init_upload(const rhi::Device &a_device, const ror::Renderer &a_renderer, rhi::BlendMode a_blend_mode, rhi::PrimitiveTopology a_topology)
{
	this->init(a_device, a_topology);

	// Do some default setup
	// Load default texture of a checker
	this->load_texture(a_device);
	this->setup_vertex_descriptor();
	this->setup_shaders(a_renderer, a_blend_mode);

	// this->m_shader_buffer.add_entry("orthographic_projection", rhi::Format::float32_4x4);
	// this->m_shader_buffer.upload(a_device, rhi::ResourceStorageOption::managed);
}

void DynamicMesh::upload_data(const uint8_t *a_vertex_data_pointer, size_t a_vertex_size_in_bytes, uint32_t a_vertex_attributes_count,
                              const uint8_t *a_indices_data_pointer, size_t a_indices_size_in_bytes, uint32_t a_indices_count)
{
	assert(this->m_device);

	const rhi::Device &a_device = *this->m_device;
	this->m_vertices_count      = a_vertex_attributes_count;

	if (this->m_has_positions)
	{
		auto &vertex_buffer_attribute = this->m_vertex_descriptor.attribute(rhi::BufferSemantic::vertex_position);
		vertex_buffer_attribute.count(a_vertex_attributes_count);

		assert(a_vertex_data_pointer && a_vertex_size_in_bytes > 0 && "No vertex attributes data provided");

		this->m_vertex_buffer.resize(a_device, a_vertex_size_in_bytes);        // Makes sure its atleast as big as vertex_buffer_size (it could be bigger)
		this->m_vertex_buffer.upload(a_vertex_data_pointer, 0, a_vertex_size_in_bytes);
		this->m_vertex_buffer.ready(true);
	}

	if (this->m_has_indices)
	{
		auto &index_buffer_attribute = this->m_vertex_descriptor.attribute(rhi::BufferSemantic::vertex_index);
		index_buffer_attribute.count(a_indices_count);

		assert(a_indices_data_pointer && a_indices_size_in_bytes > 0 && "No indices data provided, although the vertex descriptor defines indices");

		this->m_index_buffer.resize(a_device, a_indices_size_in_bytes);        // Makes sure its atleast as big as index_buffer_size (it could be bigger)
		this->m_index_buffer.upload(a_indices_data_pointer, 0, a_indices_size_in_bytes);
		this->m_index_buffer.ready(true);
	}
}

// NOTE: Make sure you have called this->upload_data(); before calling render or any other separate calls to set things up
void DynamicMesh::render(const ror::Renderer &a_renderer, rhi::RenderCommandEncoder &a_encoder)
{
	if (!this->m_visible)
		return;

	if (this->m_shader_program_external)
		a_encoder.render_pipeline_state(*this->m_shader_program_external);
	else
		a_encoder.render_pipeline_state(this->m_shader_program);        // Includes alpha blend state

	auto &attributes = this->m_vertex_descriptor.attributes();
	for (auto &attrib : attributes)
	{
		if (attrib.semantics() != rhi::BufferSemantic::vertex_index)
		{
			size_t vertex_buffer_offset = 0;                                                                // Not attrib.buffer_offset() + attrib.offset() because these are already set in attribute
			a_encoder.vertex_buffer(this->m_vertex_buffer, vertex_buffer_offset, attrib.location());        // Like Position at offset 0,index 0 and uv at offset 12, index 1 etc
			a_encoder.vertex_buffer_offset(vertex_buffer_offset, attrib.location());
		}
	}

	if (this->m_has_texture)
	{
		a_encoder.fragment_texture(this->m_texture_image, 0);
		a_encoder.fragment_sampler(this->m_texture_sampler, 0);
	}

	// These overrides the loadeded ones
	if (this->m_texture_image_external)
		a_encoder.fragment_texture(*this->m_texture_image_external, 0);

	if (this->m_texture_sampler_external)
		a_encoder.fragment_sampler(*this->m_texture_sampler_external, 0);

	auto shader_buffer = a_renderer.shader_buffer("per_view_uniform");
	shader_buffer->buffer_bind(a_encoder, rhi::ShaderStage::vertex);

	if (this->m_has_vertex_shader_buffer)
		this->m_vertex_shader_buffer.buffer_bind(a_encoder, rhi::ShaderStage::vertex);
	if (this->m_has_fragment_shader_buffer)
		this->m_fragment_shader_buffer.buffer_bind(a_encoder, rhi::ShaderStage::fragment);

	// If using own shader buffer make sure to do something like the following to fill them up
	// this->m_shader_buffer.buffer_map();
	// this->m_shader_buffer.update("some_matrix", &some_matrix);
	// this->m_shader_buffer.buffer_unmap();
	// this->m_shader_buffer.buffer_bind(a_encoder, rhi::ShaderStage::vertex);

	if (this->m_has_indices)
	{
		auto &index_buffer_attribute = this->m_vertex_descriptor.attribute(rhi::BufferSemantic::vertex_index);

		if (index_buffer_attribute.count() > 0)
			a_encoder.draw_indexed_primitives(this->m_topology,
			                                  index_buffer_attribute.count(),
			                                  index_buffer_attribute.format(),
			                                  this->m_index_buffer,
			                                  index_buffer_attribute.buffer_offset() + index_buffer_attribute.offset());
	}
	else if (this->m_has_positions)
	{
		auto &vertex_buffer_attribute = this->m_vertex_descriptor.attribute(rhi::BufferSemantic::vertex_position);
		assert(vertex_buffer_attribute.count() != 0 && "Looks like either there are not vertices in the mesh or no positions are provided");

		if (vertex_buffer_attribute.count() > 0)
			a_encoder.draw_primitives(this->m_topology, 0, vertex_buffer_attribute.count());
	}
	else
	{
		a_encoder.draw_primitives(this->m_topology, 0, this->m_vertices_count);
	}
}

// Creates a default descriptor with nothing in it. This can be used to render no attributes geometry like the fullscreen quad
rhi::VertexDescriptor create_default_descriptor()
{
	std::vector<rhi::VertexAttribute> vattribs{};
	std::vector<rhi::VertexLayout>    vlayouts{};

	rhi::VertexDescriptor vertex_descriptor{vattribs, vlayouts};

	return vertex_descriptor;
}

auto create_vertex_descriptor(const rhi::VertexAttribute &a_attribs, rhi::VertexLayout &a_layouts)
{
	std::vector<rhi::VertexAttribute> vattribs{a_attribs};
	std::vector<rhi::VertexLayout>    vlayouts{a_layouts};

	rhi::VertexDescriptor vertex_descriptor{vattribs, vlayouts};

	return vertex_descriptor;
}

auto create_vertex_descriptor(const rhi::VertexAttribute &a_attribs0, const rhi::VertexAttribute &a_attribs1, const rhi::VertexLayout &a_layouts0, const rhi::VertexLayout &a_layouts1)
{
	std::vector<rhi::VertexAttribute> vattribs{a_attribs0, a_attribs1};
	std::vector<rhi::VertexLayout>    vlayouts{a_layouts0, a_layouts1};

	rhi::VertexDescriptor vertex_descriptor{vattribs, vlayouts};

	return vertex_descriptor;
}

// Creates a float3 positions descriptor
rhi::VertexDescriptor create_p_float3_descriptor()
{
	rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vlp{0, 12};                                                                                       // binding, stride, rate, multiplier, function

	return create_vertex_descriptor(vap, vlp);
}

// Creates a float3 positions and uint16 index descriptor
rhi::VertexDescriptor create_p_float3_i_uint16_descriptor()
{
	rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vlp{0, 12};                                                                                       // binding, stride, rate, multiplier, function
	rhi::VertexAttribute vip{0, 0, 1, 0, 1, 0, rhi::BufferSemantic::vertex_index, rhi::VertexFormat::uint16_1};            // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vli{1, 0};                                                                                        // binding, stride, rate, multiplier, function

	return create_vertex_descriptor(vap, vip, vlp, vli);
}

// Creates a float3 positions, float2 uv descriptor
rhi::VertexDescriptor create_p_float4_c_float4_descriptor()
{
	rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_4};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vlp{0, 32};                                                                                       // binding, stride, rate, multiplier, function
	rhi::VertexAttribute vac{1, 16, 1, 0, 1, 0, rhi::BufferSemantic::vertex_color_0, rhi::VertexFormat::float32_4};
	rhi::VertexLayout    vlc{1, 32};

	return create_vertex_descriptor(vap, vac, vlp, vlc);
}

// Creates a float3 positions, float2 uv descriptor
rhi::VertexDescriptor create_p_float3_t_float2_descriptor()
{
	rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vlp{0, 20};                                                                                       // binding, stride, rate, multiplier, function
	rhi::VertexAttribute vat{1, 12, 1, 0, 1, 0, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2};
	rhi::VertexLayout    vlt{1, 20};

	return create_vertex_descriptor(vap, vat, vlp, vlt);
}

// Creates a float2 positions, float2 uv descriptor
rhi::VertexDescriptor create_p_float2_t_float2_descriptor()
{
	rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_2};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vlp{0, 16};                                                                                       // binding, stride, rate, multiplier, function
	rhi::VertexAttribute vat{1, 8, 1, 0, 1, 0, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2};
	rhi::VertexLayout    vlt{1, 16};

	return create_vertex_descriptor(vap, vat, vlp, vlt);
}

// Creates a float3 positions, float3 color descriptor
rhi::VertexDescriptor create_p_float3_c_float3_descriptor()
{
	rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vlp{0, 24};                                                                                       // binding, stride, rate, multiplier, function
	rhi::VertexAttribute vat{1, 12, 1, 0, 1, 0, rhi::BufferSemantic::vertex_color_0, rhi::VertexFormat::float32_3};
	rhi::VertexLayout    vlt{1, 24};

	return create_vertex_descriptor(vap, vat, vlp, vlt);
}

// Creates a float3 positions, float2 uv and uint16 index descriptor
rhi::VertexDescriptor create_p_float3_t_float2_i_uint16_descriptor()
{
	rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vlp{0, 20};                                                                                       // binding, stride, rate, multiplier, function
	rhi::VertexAttribute vat{1, 12, 1, 0, 1, 0, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2};
	rhi::VertexLayout    vlt{1, 20};
	rhi::VertexAttribute vip{0, 0, 1, 0, 2, 0, rhi::BufferSemantic::vertex_index, rhi::VertexFormat::uint16_1};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vli{2, 0};                                                                                    // binding, stride, rate, multiplier, function

	std::vector<rhi::VertexAttribute> vattribs{vap, vat, vip};
	std::vector<rhi::VertexLayout>    vlayouts{vlp, vlt, vli};

	rhi::VertexDescriptor vertex_descriptor{vattribs, vlayouts};

	return vertex_descriptor;
}

}        // namespace ror
