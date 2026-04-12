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
#include "foundation/rorutilities.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
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

void LineSoup::reset()
{
	/* // Most probably not needed but incase seeing rubbish on screen enable this
	const size_t vertex_buffer_size = this->m_vertex_buffer.size_in_bytes();

	uint8_t *vertex_buffer = reinterpret_cast<uint8_t *>(this->m_vertex_buffer.map());

	vertex_buffer = reinterpret_cast<uint8_t *>(this->m_vertex_buffer.map());
	std::memset(reinterpret_cast<void *>(vertex_buffer), 0, vertex_buffer_size);
	*/

	this->m_lines_count = 0;
}

void LineSoup::init(const rhi::Device &a_device)
{
	this->m_device = &a_device;

	// Create vertex descriptor
	rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vlp{0, vertex_descriptor_size};                                                                   // binding, stride, rate, multiplier, function
	rhi::VertexAttribute vac{1, 12, 1, 0, 1, 0, rhi::BufferSemantic::vertex_color_0, rhi::VertexFormat::float32_4};
	rhi::VertexLayout    vlc{1, vertex_descriptor_size};

	std::vector<rhi::VertexAttribute> vattribs{vap, vac};
	std::vector<rhi::VertexLayout>    vlayouts{vlp, vlc};

	this->m_vertex_descriptor = rhi::VertexDescriptor{vattribs, vlayouts};
}

void LineSoup::upload(const rhi::Device &a_device, const rhi::Renderpass &a_target_pass, const rhi::Rendersubpass &a_target_subpass)
{
	this->init(a_device);

	auto vertex_buffer_size{10000u};

	// Create shader program
	// TODO: Needs to fix the set and binding
	auto vs_shader = rhi::build_shader<rhi::Shader>(a_device, "line_soup.glsl.vert");
	auto fs_shader = rhi::build_shader<rhi::Shader>(a_device, "line_soup.glsl.frag");

	// Only requires the following descriptors
	// layout(std140, set = 0, binding = 20) uniform per_view_uniform; name and set and binding needs changing according to API

	uint32_t lines_soup_buffer_binding{0};
	this->m_shader_program.build_descriptor(a_device, nullptr, lines_soup_buffer_binding, nullptr, nullptr, 0);
	this->m_shader_program.upload(a_device, a_target_pass, a_target_subpass, vs_shader, fs_shader, this->m_vertex_descriptor, rhi::BlendMode::blend, rhi::PrimitiveTopology::lines, "line_soup_pso", false, false, false, false);

	this->m_vertex_buffer.init(a_device, vertex_buffer_size);        // By default in shared mode
}

uint32_t LineSoup::push_lines(const uint8_t *a_data, uint32_t a_lines_count, uint32_t a_lines_index)
{
	assert(this->m_device);

	const rhi::Device &a_device = *this->m_device;

	const size_t vertex_buffer_size           = this->m_vertex_buffer.size_in_bytes();
	const size_t total_lines                  = this->m_lines_count + a_lines_count;
	const size_t vertex_buffer_new_lines_size = total_lines * vertex_descriptor_size * 2;
	const size_t vertex_buffer_lines_size     = this->m_lines_count * vertex_descriptor_size * 2;
	uint8_t     *vertex_buffer_backup{nullptr};

	uint8_t *vertex_buffer = reinterpret_cast<uint8_t *>(this->m_vertex_buffer.map());

	if (vertex_buffer_new_lines_size >= vertex_buffer_size && this->m_lines_count < a_lines_index)
	{
		vertex_buffer_backup = new uint8_t[vertex_buffer_lines_size];

		std::memcpy(vertex_buffer_backup, reinterpret_cast<uint8_t *>(vertex_buffer), vertex_buffer_lines_size);
		this->m_vertex_buffer.unmap();

		this->m_vertex_buffer.resize(a_device, vertex_buffer_new_lines_size);        // Makes sure its atleast as big as vertex_buffer_new_size (it could be bigger)

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
	if (!this->m_visible || this->m_lines_count == 0u)
		return;

	this->setup_render_state(a_device, a_encoder);

	size_t vertexBufferOffset = 0;

	a_encoder.vertex_buffer_offset(this->m_vertex_buffer, vertexBufferOffset, 0);
	a_encoder.vertex_buffer_offset(this->m_vertex_buffer, vertexBufferOffset, 1);
	a_encoder.draw_primitives(rhi::PrimitiveTopology::lines, static_cast_safe<uint32_t>(vertexBufferOffset), this->m_lines_count * 2);
}

#define push_box_points(data_vector, color)     \
	ror::push_point(data_vector, l_b_f, color); \
	ror::push_point(data_vector, l_t_f, color); \
	ror::push_point(data_vector, l_t_f, color); \
	ror::push_point(data_vector, r_t_f, color); \
	ror::push_point(data_vector, r_t_f, color); \
	ror::push_point(data_vector, r_b_f, color); \
	ror::push_point(data_vector, r_b_f, color); \
	ror::push_point(data_vector, l_b_f, color); \
	ror::push_point(data_vector, l_b_b, color); \
	ror::push_point(data_vector, l_t_b, color); \
	ror::push_point(data_vector, l_t_b, color); \
	ror::push_point(data_vector, r_t_b, color); \
	ror::push_point(data_vector, r_t_b, color); \
	ror::push_point(data_vector, r_b_b, color); \
	ror::push_point(data_vector, r_b_b, color); \
	ror::push_point(data_vector, l_b_b, color); \
	ror::push_point(data_vector, l_b_f, color); \
	ror::push_point(data_vector, l_b_b, color); \
	ror::push_point(data_vector, l_t_f, color); \
	ror::push_point(data_vector, l_t_b, color); \
	ror::push_point(data_vector, r_t_f, color); \
	ror::push_point(data_vector, r_t_b, color); \
	ror::push_point(data_vector, r_b_f, color); \
	ror::push_point(data_vector, r_b_b, color); \
	(void) 0

#define push_points()                                                                                       \
	const uint32_t         lines_count = 12;                                                                \
	std::vector<float32_t> data{};                                                                          \
	data.reserve(lines_count * 2 * 4);                                                                      \
                                                                                                            \
	push_box_points(data, a_color);                                                                         \
	a_lines_index = this->push_lines(reinterpret_cast<uint8_t *>(data.data()), lines_count, a_lines_index); \
	(void) 0

uint32_t LineSoup::push_box(const ror::BoundingBoxf &a_box, uint32_t a_lines_index, ror::Vector4f a_color)
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

	push_points();

	return a_lines_index;
}

uint32_t LineSoup::push_box(const ror::Vector3f a_corners[8], uint32_t a_lines_index, ror::Vector4f a_color, bool a_mark_center)
{
	// First 4 corners are the nearest the next 4 are the furthest
	ror::Vector3f l_b_f{a_corners[0]};
	ror::Vector3f l_t_f{a_corners[1]};
	ror::Vector3f r_t_f{a_corners[2]};
	ror::Vector3f r_b_f{a_corners[3]};

	ror::Vector3f l_b_b{a_corners[4]};
	ror::Vector3f l_t_b{a_corners[5]};
	ror::Vector3f r_t_b{a_corners[6]};
	ror::Vector3f r_b_b{a_corners[7]};

	uint32_t               lines_count = a_mark_center ? 12 + 19 : 12;
	std::vector<float32_t> data{};
	data.reserve(lines_count * 2 * 4);

	push_box_points(data, a_color);

	if (a_mark_center)
	{
		// Front/back, left/right, top/bottom crosses
		ror::push_point(data, l_b_f, a_color);
		ror::push_point(data, r_t_f, a_color);
		ror::push_point(data, l_t_f, a_color);
		ror::push_point(data, r_b_f, a_color);
		ror::push_point(data, l_b_b, a_color);
		ror::push_point(data, r_t_b, a_color);
		ror::push_point(data, l_t_b, a_color);
		ror::push_point(data, r_b_b, a_color);
		ror::push_point(data, l_t_f, a_color);
		ror::push_point(data, r_t_b, a_color);
		ror::push_point(data, r_t_f, a_color);
		ror::push_point(data, l_t_b, a_color);
		ror::push_point(data, l_b_f, a_color);
		ror::push_point(data, r_b_b, a_color);
		ror::push_point(data, r_b_f, a_color);
		ror::push_point(data, l_b_b, a_color);
		ror::push_point(data, l_t_f, a_color);
		ror::push_point(data, l_b_b, a_color);
		ror::push_point(data, l_b_f, a_color);
		ror::push_point(data, l_t_b, a_color);
		ror::push_point(data, r_t_f, a_color);
		ror::push_point(data, r_b_b, a_color);
		ror::push_point(data, r_b_f, a_color);
		ror::push_point(data, r_t_b, a_color);

		// Front to Back center line
		ror::push_point(data, ((((l_b_f + r_t_f) / 2.0f) + ((l_t_f + r_b_f) / 2.0f)) / 2.0f), a_color);
		ror::push_point(data, ((((l_b_b + r_t_b) / 2.0f) + ((l_t_b + r_b_b) / 2.0f)) / 2.0f), a_color);

		// Top to Bottom center line
		ror::push_point(data, ((((l_t_f + r_t_b) / 2.0f) + ((r_t_f + l_t_b) / 2.0f)) / 2.0f), a_color);
		ror::push_point(data, ((((l_b_f + r_b_b) / 2.0f) + ((r_b_f + l_b_b) / 2.0f)) / 2.0f), a_color);

		// Left to Right center line
		ror::push_point(data, ((((l_t_f + l_b_b) / 2.0f) + ((l_b_f + l_t_b) / 2.0f)) / 2.0f), a_color);
		ror::push_point(data, ((((r_t_f + r_b_b) / 2.0f) + ((r_b_f + r_t_b) / 2.0f)) / 2.0f), a_color);

		// Middle crosses
		ror::push_point(data, l_b_f, a_color);
		ror::push_point(data, r_t_b, a_color);

		ror::push_point(data, l_t_f, a_color);
		ror::push_point(data, r_b_b, a_color);

		ror::push_point(data, l_b_b, a_color);
		ror::push_point(data, r_t_f, a_color);

		ror::push_point(data, l_t_b, a_color);
		ror::push_point(data, r_b_f, a_color);
	}

	a_lines_index = this->push_lines(reinterpret_cast<uint8_t *>(data.data()), lines_count, a_lines_index);
	return a_lines_index;
}

uint32_t LineSoup::push_cross(const ror::Vector3f &a_center, const float32_t a_size, uint32_t a_lines_index, ror::Vector4f a_color)
{
	const uint32_t         lines_count{18 + 12};        // 18 for cross and 12 for box
	const float32_t        half_size{a_size / 2.0f};
	const float32_t        twentyth_size{a_size / 20.0f};
	const float32_t        thirtyth_size{a_size / 30.0f};
	std::vector<float32_t> data{};
	data.reserve(lines_count * 2 * 4);

	ror::Vector3f xm{a_center.x - half_size, a_center.y, a_center.z};
	ror::Vector3f xp{a_center.x + half_size, a_center.y, a_center.z};

	ror::Vector3f ym{a_center.x, a_center.y - half_size, a_center.z};
	ror::Vector3f yp{a_center.x, a_center.y + half_size, a_center.z};

	ror::Vector3f zm{a_center.x, a_center.y, a_center.z - half_size};
	ror::Vector3f zp{a_center.x, a_center.y, a_center.z + half_size};

	// X-Axis
	ror::push_point(data, xm, ror::tint_red4f);
	ror::push_point(data, a_center, ror::tint_red4f);

	ror::push_point(data, a_center, ror::red4f);
	ror::push_point(data, xp, ror::red4f);

	// Y-Axis
	ror::push_point(data, ym, ror::tint_green4f);
	ror::push_point(data, a_center, ror::tint_green4f);

	ror::push_point(data, a_center, ror::green4f);
	ror::push_point(data, yp, ror::green4f);

	// Z-Axis
	ror::push_point(data, zm, ror::tint_blue4f);
	ror::push_point(data, a_center, ror::tint_blue4f);

	ror::push_point(data, a_center, ror::blue4f);
	ror::push_point(data, zp, ror::blue4f);

	// X-Axis Arrows
	ror::push_point(data, xm, ror::tint_red4f);
	ror::push_point(data, xm + ror::Vector3f{twentyth_size, +twentyth_size, 0.0f}, ror::tint_red4f);
	ror::push_point(data, xm, ror::tint_red4f);
	ror::push_point(data, xm + ror::Vector3f{twentyth_size, -twentyth_size, 0.0f}, ror::tint_red4f);

	ror::push_point(data, xp, ror::red4f);
	ror::push_point(data, xp - ror::Vector3f{twentyth_size, -twentyth_size, 0.0f}, ror::red4f);
	ror::push_point(data, xp, ror::red4f);
	ror::push_point(data, xp - ror::Vector3f{twentyth_size, +twentyth_size, 0.0f}, ror::red4f);

	// Y-Axis Arrows
	ror::push_point(data, ym, ror::tint_green4f);
	ror::push_point(data, ym + ror::Vector3f{+twentyth_size, twentyth_size, 0.0f}, ror::tint_green4f);
	ror::push_point(data, ym, ror::tint_green4f);
	ror::push_point(data, ym + ror::Vector3f{-twentyth_size, twentyth_size, 0.0f}, ror::tint_green4f);

	ror::push_point(data, yp, ror::green4f);
	ror::push_point(data, yp - ror::Vector3f{-twentyth_size, twentyth_size, 0.0f}, ror::green4f);
	ror::push_point(data, yp, ror::green4f);
	ror::push_point(data, yp - ror::Vector3f{+twentyth_size, twentyth_size, 0.0f}, ror::green4f);

	// Z-Axis Arrows
	ror::push_point(data, zm, ror::tint_blue4f);
	ror::push_point(data, zm + ror::Vector3f{+twentyth_size, 0.0f, twentyth_size}, ror::tint_blue4f);
	ror::push_point(data, zm, ror::tint_blue4f);
	ror::push_point(data, zm + ror::Vector3f{-twentyth_size, 0.0f, twentyth_size}, ror::tint_blue4f);

	ror::push_point(data, zp, ror::blue4f);
	ror::push_point(data, zp - ror::Vector3f{-twentyth_size, 0.0f, twentyth_size}, ror::blue4f);
	ror::push_point(data, zp, ror::blue4f);
	ror::push_point(data, zp - ror::Vector3f{+twentyth_size, 0.0f, twentyth_size}, ror::blue4f);

	auto bmin = a_center - ror::Vector3f{thirtyth_size};
	auto bmax = a_center + ror::Vector3f{thirtyth_size};

	ror::Vector3f l_b_f = bmin;
	ror::Vector3f l_t_f{bmin.x, bmax.y, bmin.z};
	ror::Vector3f r_t_f{bmax.x, bmax.y, bmin.z};
	ror::Vector3f r_b_f{bmax.x, bmin.y, bmin.z};

	ror::Vector3f l_b_b{bmin.x, bmin.y, bmax.z};
	ror::Vector3f l_t_b{bmin.x, bmax.y, bmax.z};
	ror::Vector3f r_t_b{bmax.x, bmax.y, bmax.z};
	ror::Vector3f r_b_b{bmax.x, bmin.y, bmax.z};

	// Tiny box in the middle to work out what color this cross was
	push_box_points(data, a_color);

	a_lines_index = this->push_lines(reinterpret_cast<uint8_t *>(data.data()), lines_count, a_lines_index);

	return a_lines_index;
}

std::vector<float32_t> create_wire_sphere(const ror::Vector3f &a_center, float32_t a_radius, uint32_t a_segments, const ror::Vector4f &a_color)
{
	std::vector<float32_t> lines{};

	if (a_segments < 3 || a_radius <= 0.0f)
		return lines;

	// We build the sphere using latitude and longitude rings.
	// Latitude: from top to bottom
	// Longitude: around the sphere

	const uint32_t lat_segments = a_segments;
	const uint32_t lon_segments = a_segments * 2;

	auto point_on_sphere = [&](float32_t theta, float32_t phi) -> ror::Vector3f {
		// theta = latitude angle   [0, ror_pi]
		// phi   = longitude angle  [0, 2pi]
		const float32_t sin_theta = std::sin(theta);
		const float32_t cos_theta = std::cos(theta);
		const float32_t sin_phi   = std::sin(phi);
		const float32_t cos_phi   = std::cos(phi);

		return ror::Vector3f{a_center.x + a_radius * sin_theta * cos_phi,
		                     a_center.y + a_radius * cos_theta,
		                     a_center.z + a_radius * sin_theta * sin_phi};
	};

	// 1. Latitude rings
	for (uint32_t lat = 1; lat < lat_segments; ++lat)
	{
		const float32_t theta = ror::ror_pi * static_cast<float32_t>(lat) / static_cast<float32_t>(lat_segments);

		for (uint32_t lon = 0; lon < lon_segments; ++lon)
		{
			const float32_t phi0 = 2.0f * ror::ror_pi * static_cast<float32_t>(lon) / static_cast<float32_t>(lon_segments);
			const float32_t phi1 = 2.0f * ror::ror_pi * static_cast<float32_t>(lon + 1) / static_cast<float32_t>(lon_segments);

			const ror::Vector3f p0 = point_on_sphere(theta, phi0);
			const ror::Vector3f p1 = point_on_sphere(theta, phi1);

			ror::push_point(lines, p0, a_color);
			ror::push_point(lines, p1, a_color);
		}
	}

	// 2. Longitude lines
	for (uint32_t lon = 0; lon < lon_segments; ++lon)
	{
		const float32_t phi = 2.0f * ror::ror_pi * static_cast<float32_t>(lon) / static_cast<float32_t>(lon_segments);

		for (uint32_t lat = 0; lat < lat_segments; ++lat)
		{
			const float32_t theta0 = ror::ror_pi * static_cast<float32_t>(lat) / static_cast<float32_t>(lat_segments);
			const float32_t theta1 = ror::ror_pi * static_cast<float32_t>(lat + 1) / static_cast<float32_t>(lat_segments);

			const ror::Vector3f p0 = point_on_sphere(theta0, phi);
			const ror::Vector3f p1 = point_on_sphere(theta1, phi);

			ror::push_point(lines, p0, a_color);
			ror::push_point(lines, p1, a_color);
		}
	}

	return lines;
}

uint32_t LineSoup::push_sphere(const ror::BoundingSpheref &a_sphere, uint32_t a_lines_index, uint32_t a_segments, ror::Vector4f a_color)
{
	auto sphere_lines    = create_wire_sphere(a_sphere.center(), a_sphere.radius(), a_segments, a_color);
	return a_lines_index = this->push_lines(reinterpret_cast<uint8_t *>(sphere_lines.data()), static_cast_safe<uint32_t>(sphere_lines.size() / 14u), a_lines_index);
}

}        // namespace ror
