// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2025
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

#include "bounds/rorbounding.hpp"
#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rorutilities.hpp"
#include "math/rorvector3.hpp"
#include "rorline_soup.hpp"
#include <cassert>
#include <cstddef>
#include <vector>

namespace ror
{

static const auto vertex_descriptor_size{28};

LineSoup::LineSoup()
{
	this->m_show_toggle_callback = [this](ror::Event &e) {
		(void) e;
		this->m_visible = !this->m_visible;
	};
}

void LineSoup::install_input_handlers()
{
	if (this->m_event_system)
	{
		this->m_event_system->subscribe_early(keyboard_a_click, this->m_show_toggle_callback);
	}
}

void LineSoup::uninstall_input_handlers()
{
	if (this->m_event_system)
	{
		this->m_event_system->unsubscribe(keyboard_a_click, this->m_show_toggle_callback);
	}
}

void LineSoup::init(const rhi::Device &a_device, ror::EventSystem &a_event_system)
{
	this->m_device       = &a_device;
	this->m_event_system = &a_event_system;

	this->install_input_handlers();

	// Create vertex descriptor
	rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vlp{0, vertex_descriptor_size};                                                                   // binding, stride, rate, multiplier, function
	rhi::VertexAttribute vac{1, 12, 1, 0, 1, 0, rhi::BufferSemantic::vertex_color_0, rhi::VertexFormat::float32_4};
	rhi::VertexLayout    vlc{1, vertex_descriptor_size};

	std::vector<rhi::VertexAttribute> vattribs{vap, vac};
	std::vector<rhi::VertexLayout>    vlayouts{vlp, vlc};

	this->m_vertex_descriptor = rhi::VertexDescriptor{vattribs, vlayouts};
}

void LineSoup::upload(const rhi::Device &a_device, const ror::Renderer &a_renderer)
{
	auto vertex_buffer_size{10000u};

	// Create shader program
	// TODO: Needs to fix the set and binding
	auto vs_shader = rhi::build_shader<rhi::Shader>(a_device, "line_soup.glsl.vert");
	auto fs_shader = rhi::build_shader<rhi::Shader>(a_device, "line_soup.glsl.frag");

	// Needs final pass stuff
	rhi::Renderpass    *pass{nullptr};
	rhi::Rendersubpass *subpass{nullptr};

	a_renderer.get_final_pass_subpass(&pass, &subpass);

	// Only requires the following descriptors
	// layout(std140, set = 0, binding = 20) uniform per_view_uniform; name and set and binding needs changing according to API

	uint32_t lines_soup_buffer_binding{0};
	this->m_shader_program.build_descriptor(a_device, nullptr, lines_soup_buffer_binding, nullptr, nullptr, 0);
	this->m_shader_program.upload(a_device, *pass, *subpass, vs_shader, fs_shader, this->m_vertex_descriptor, rhi::BlendMode::blend, rhi::PrimitiveTopology::lines, "line_soup_pso", false, false, false);

	this->m_vertex_buffer.init(a_device, vertex_buffer_size);        // By default in shared mode
}

uint32_t LineSoup::push_lines(const uint8_t *a_data, uint32_t a_lines_count, uint32_t a_lines_index)
{
	assert(this->m_device);

	const rhi::Device &a_device = *this->m_device;

	size_t   vertex_buffer_size       = this->m_vertex_buffer.size_in_bytes();
	size_t   total_lines              = this->m_lines_count + a_lines_count;
	size_t   vertex_buffer_new_size   = total_lines * vertex_descriptor_size * 2;
	size_t   vertex_buffer_lines_size = this->m_lines_count * vertex_descriptor_size * 2;
	uint8_t *vertex_buffer_backup{nullptr};

	uint8_t *vertex_buffer = reinterpret_cast<uint8_t *>(this->m_vertex_buffer.map());

	if (vertex_buffer_new_size >= vertex_buffer_size && this->m_lines_count < a_lines_index)
	{
		vertex_buffer_backup = new uint8_t[vertex_buffer_lines_size];

		std::memcpy(vertex_buffer_backup, reinterpret_cast<uint8_t *>(vertex_buffer), vertex_buffer_lines_size);
		this->m_vertex_buffer.unmap();

		this->m_vertex_buffer.resize(a_device, vertex_buffer_new_size);        // Makes sure its atleast as big as vertex_buffer_new_size (it could be bigger)

		vertex_buffer = reinterpret_cast<uint8_t *>(this->m_vertex_buffer.map());
		std::memcpy(reinterpret_cast<void *>(vertex_buffer), vertex_buffer_backup, vertex_buffer_lines_size);

		delete[] vertex_buffer_backup;
	}

	auto ret = 0u;
	if (this->m_lines_count > a_lines_index)        // Replace mode
	{
		ret = a_lines_index;
		vertex_buffer += a_lines_index * vertex_descriptor_size * 2;
	}

	else
	{
		vertex_buffer += vertex_buffer_lines_size;        // Append mode
		ret = this->m_lines_count;
		this->m_lines_count += a_lines_count;
	}

	std::memcpy(reinterpret_cast<void *>(vertex_buffer), a_data, a_lines_count * vertex_descriptor_size * 2);

	this->m_vertex_buffer.unmap();

	this->m_vertex_buffer.ready(true);

	return ret;
}

void LineSoup::setup_render_state(const rhi::Device &a_device, rhi::RenderCommandEncoder &a_encoder)
{
	a_encoder.cull_mode(rhi::PrimitiveCullMode::none);                               // No face culling, no triangles
	a_encoder.front_facing_winding(rhi::PrimitiveWinding::counter_clockwise);        // This might not be needed
	a_encoder.vertex_buffer(this->m_vertex_buffer, 0, 0);                            // Position at index 0 offset 0 because its provided in attribute
	a_encoder.vertex_buffer(this->m_vertex_buffer, 0, 1);                            // Color at index 1 offset 0 because its provided in attribute
	a_encoder.render_pipeline_state(a_device, this->m_shader_program);               // Include alpha blend state, also calls bind descriptors (important for vulkan)
}

void LineSoup::render(const rhi::Device &a_device, rhi::RenderCommandEncoder &a_encoder)
{
	if (!this->m_visible)
		return;

	this->setup_render_state(a_device, a_encoder);

	size_t vertexBufferOffset = 0;

	a_encoder.vertex_buffer_offset(this->m_vertex_buffer, vertexBufferOffset, 0);
	a_encoder.vertex_buffer_offset(this->m_vertex_buffer, vertexBufferOffset, 1);
	a_encoder.draw_primitives(rhi::PrimitiveTopology::lines, static_cast_safe<uint32_t>(vertexBufferOffset), this->m_lines_count * 2);
}

uint32_t LineSoup::push_box(ror::BoundingBoxf &a_box, uint32_t a_lines_index, ror::Vector4f a_color)
{
	auto bmin = a_box.minimum();
	auto bmax = a_box.maximum();

	ror::Vector3f l_b_f = bmin;
	ror::Vector3f l_t_f{bmin.x, bmax.y, bmin.z};
	ror::Vector3f r_t_f{bmax.x, bmax.y, bmin.z};
	ror::Vector3f r_b_f{bmax.x, bmin.y, bmin.z};

	ror::Vector3f l_b_b{bmin.x, bmin.y, bmax.z};
	ror::Vector3f l_t_b{bmin.x, bmax.y, bmax.z};
	ror::Vector3f r_t_b{bmax.x, bmax.y, bmax.z};
	ror::Vector3f r_b_b{bmax.x, bmin.y, bmax.z};

	const uint32_t         lines_count = 12;
	std::vector<float32_t> data{};
	data.reserve(lines_count * 2 * 4);        // 2 points per line, 4 floats each point (x, y, z, c)

	// Font 4 lines
	ror::push_point(data, l_b_f, a_color);
	ror::push_point(data, l_t_f, a_color);

	ror::push_point(data, l_t_f, a_color);
	ror::push_point(data, r_t_f, a_color);

	ror::push_point(data, r_t_f, a_color);
	ror::push_point(data, r_b_f, a_color);

	ror::push_point(data, r_b_f, a_color);
	ror::push_point(data, l_b_f, a_color);

	// Back 4 lines
	ror::push_point(data, l_b_b, a_color);
	ror::push_point(data, l_t_b, a_color);

	ror::push_point(data, l_t_b, a_color);
	ror::push_point(data, r_t_b, a_color);

	ror::push_point(data, r_t_b, a_color);
	ror::push_point(data, r_b_b, a_color);

	ror::push_point(data, r_b_b, a_color);
	ror::push_point(data, l_b_b, a_color);

	// Side 4 lines
	ror::push_point(data, l_b_f, a_color);
	ror::push_point(data, l_b_b, a_color);

	ror::push_point(data, l_t_f, a_color);
	ror::push_point(data, l_t_b, a_color);

	ror::push_point(data, r_t_f, a_color);
	ror::push_point(data, r_t_b, a_color);

	ror::push_point(data, r_b_f, a_color);
	ror::push_point(data, r_b_b, a_color);

	this->push_lines(reinterpret_cast<uint8_t *>(data.data()), lines_count, a_lines_index);

	return a_lines_index;
}

}        // namespace ror
