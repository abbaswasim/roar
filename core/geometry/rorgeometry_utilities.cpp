// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2023
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

#include "rorgeometry_utilities.hpp"
#include "thirdparty/include_convhull_3d.hpp"

#define PAR_OCTASPHERE_IMPLEMENTATION
#include "par/par_octasphere.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par/par_shapes.h"

namespace ror
{
void make_sphere_triangles(std::vector<ror::Vector3f> &a_vertex_buffer, int32_t a_subdivisions, float32_t a_cornder_radius)
{
	par_octasphere_config cfg{};// = {
	    cfg.corner_radius    = a_cornder_radius;
	    cfg.width            = 0;
	    cfg.height           = 0;
	    cfg.depth            = 0;
	    cfg.num_subdivisions = a_subdivisions;
        // cfg.uv_mode          = PAR_OCTASPHERE_UV_LATLONG;
	    // cfg.normals_mode     = PAR_OCTASPHERE_NORMALS_SMOOT;
	//};

	uint32_t num_indices;
	uint32_t num_vertices;
	par_octasphere_get_counts(&cfg, &num_indices, &num_vertices);

	std::vector<ror::Vector3f> vertex_buffer{};
	std::vector<uint16_t>      indicies{};
	vertex_buffer.resize(num_vertices * sizeof(ror::Vector3f));
	indicies.resize(num_indices);

	par_octasphere_mesh mesh{};// = {
	    mesh.positions    = reinterpret_cast<float32_t *>(vertex_buffer.data());
	    mesh.normals      = nullptr;        // Don't want normals
	    mesh.texcoords    = nullptr;        // Don't want uvs
	    mesh.indices      = indicies.data();
        // mesh.num_indices  = num_indices;
        // mesh.num_vertices = num_vertices;
    //};


	// Generate vertex coordinates, ignoring normals, uv, and triangle indices.
	par_octasphere_populate(&cfg, &mesh);

	a_vertex_buffer.reserve(num_indices);
	for (uint32_t i = 0; i < num_indices; ++i)
		a_vertex_buffer.emplace_back(vertex_buffer[indicies[i]]);
}

static void shape_to_buffer(std::vector<ror::Vector3f> &a_vertex_buffer, const ror::Matrix4f &a_transform, par_shapes_mesh *a_shape)
{
	a_vertex_buffer.reserve(a_vertex_buffer.size() + (static_cast<size_t>(a_shape->ntriangles) * 3));

	for (int32_t i = 0; i < a_shape->ntriangles; ++i)
	{
		auto *ptr = a_shape->points;
		auto *ts  = a_shape->triangles;
		auto  ti  = 3 * i;
		for (int32_t j = 0; j < 3; ++j)
		{
			auto vid   = ts[ti + j];
			auto point = ror::Vector3f{ptr[vid * 3 + 0], ptr[vid * 3 + 1], ptr[vid * 3 + 2]};
			point      = a_transform * point;
			a_vertex_buffer.emplace_back(point);
		}
	}
}

void make_cylinder_triangles(std::vector<ror::Vector3f> &a_vertex_buffer,
                             int32_t                     a_slices,
                             int32_t                     a_stackes)
{
	auto *shape = par_shapes_create_cylinder(a_slices, a_stackes);

	// Make the cylinder in roar y-axis and unit diameter
	auto xform = ror::matrix4_rotation_around_x(ror::to_radians(-90.0f)) * ror::matrix4_scaling(0.5f, 0.5f, 1.0f);

	shape_to_buffer(a_vertex_buffer, xform, shape);
	delete shape;
}

void make_cone_triangles(std::vector<ror::Vector3f> &a_vertex_buffer,
                         int32_t                     a_slices,
                         int32_t                     a_stackes)
{
	auto *shape = par_shapes_create_cone(a_slices, a_stackes);

	// Make the cylinder in roar y-axis and unit diameter
	auto xform = ror::matrix4_rotation_around_x(ror::to_radians(-90.0f)) * ror::matrix4_scaling(0.5f, 0.5f, 1.0f);

	shape_to_buffer(a_vertex_buffer, xform, shape);
	delete shape;
}

void make_tetrahedron_triangles(std::vector<ror::Vector3f> &a_vertex_buffer)
{
	auto *shape = par_shapes_create_tetrahedron();
	shape_to_buffer(a_vertex_buffer, ror::Matrix4f{}, shape);
	delete shape;
}

void make_hemisphere_triangles(std::vector<ror::Vector3f> &a_vertex_buffer,
                               int32_t                     a_slices,
                               int32_t                     a_stackes)
{
	auto *shape = par_shapes_create_hemisphere(a_slices, a_stackes);
	shape_to_buffer(a_vertex_buffer, ror::Matrix4f{}, shape);
	delete shape;
}

void make_disk_triangles(std::vector<ror::Vector3f> &a_vertex_buffer,
                         ror::Vector3f               a_center,
                         ror::Vector3f               a_normal,
                         float32_t                   a_radius,
                         int32_t                     a_slices)
{
	// radius, slices, center, normal
	auto *shape = par_shapes_create_disk(a_radius, a_slices, reinterpret_cast<float32_t *>(&a_center.x), reinterpret_cast<float32_t *>(&a_normal.x));
	shape_to_buffer(a_vertex_buffer, ror::Matrix4f{}, shape);
	delete shape;
}

void create_arrow(std::vector<ror::Vector3f> &arrow_triangles_data, ror::Vector3f a_scale)
{
	std::vector<ror::Vector3f> cylinder_triangles_data{};
	std::vector<ror::Vector3f> cone_triangles_data{};
	std::vector<ror::Vector3f> disk_triangles_data{};

	make_cylinder_triangles(cylinder_triangles_data);
	make_cone_triangles(cone_triangles_data);
	make_disk_triangles(disk_triangles_data, ror::Vector3f{}, ror::Vector3f{0.0, -1.0, 0.0});

	auto smat  = ror::matrix4_scaling(0.01f * a_scale.x, 0.5f * a_scale.y, 0.01f * a_scale.z);
	auto smat2 = ror::matrix4_scaling(0.03f * a_scale.x, 0.04f * a_scale.y, 0.03f * a_scale.z);
	auto smat3 = ror::matrix4_scaling(0.015f * a_scale.x, 0.04f * a_scale.y, 0.015f * a_scale.z);
	auto tmat  = ror::matrix4_translation(0.0f * a_scale.x, 0.5f * a_scale.y, 0.0f * a_scale.z);
	auto rmat  = ror::matrix4_rotation_around_y(ror::to_radians(18.0f));

	arrow_triangles_data.reserve(cylinder_triangles_data.size() + cone_triangles_data.size() + disk_triangles_data.size());

	// Lets make an arrow from the three objects, cylinder, cone and disk
	for (auto &p : cylinder_triangles_data)
		arrow_triangles_data.emplace_back(smat * p);

	for (auto &p : cone_triangles_data)
		arrow_triangles_data.emplace_back(tmat * smat2 * p);

	for (auto &p : disk_triangles_data)
		arrow_triangles_data.emplace_back(tmat * rmat * smat3 * p);
}

void create_axis(std::vector<std::vector<float32_t>> &debug_data, std::vector<rhi::PrimitiveTopology> &topology_data)
{
	std::vector<float32_t> points_colors{};

	auto add_point = [&points_colors](ror::Vector3f &point, ror::Vector4f &color, ror::Matrix4f xform) {
		auto xpoint = xform * point;

		points_colors.push_back(xpoint.x);
		points_colors.push_back(xpoint.y);
		points_colors.push_back(xpoint.z);
		points_colors.push_back(0.0f);

		points_colors.push_back(color.x);
		points_colors.push_back(color.y);
		points_colors.push_back(color.z);
		points_colors.push_back(color.w);
	};

	std::vector<ror::Vector3f> arrow_triangles_data{};
	create_arrow(arrow_triangles_data, ror::Vector3f{1.0f});

	float32_t color_intensity = 1.0f;
	float32_t color_fade      = 0.2f;

	auto redcolor_p = ror::Vector4f{color_intensity, color_fade, color_fade, 1.0f};
	auto redcolor_n = redcolor_p * ror::Vector4f(color_fade, color_fade, color_fade, 1.0);

	auto greencolor_p = ror::Vector4f{color_fade, color_intensity, color_fade, 1.0f};
	auto greencolor_n = greencolor_p * ror::Vector4f(color_fade, color_fade, color_fade, 1.0);

	auto bluecolor_p = ror::Vector4f{color_fade, color_fade, color_intensity, 1.0f};
	auto bluecolor_n = bluecolor_p * ror::Vector4f(color_fade, color_fade, color_fade, 1.0);

	// +X
	for (auto &p : arrow_triangles_data)
		add_point(p, redcolor_p, ror::matrix4_rotation_around_z(ror::to_radians(-90.0f)));

	// -X
	for (auto &p : arrow_triangles_data)
		add_point(p, redcolor_n, ror::matrix4_rotation_around_z(ror::to_radians(90.0f)));

	// +Y
	for (auto &p : arrow_triangles_data)
		add_point(p, greencolor_p, {});

	// -Y
	for (auto &p : arrow_triangles_data)
		add_point(p, greencolor_n, ror::matrix4_rotation_around_x(ror::to_radians(180.0f)));

	// +Z
	for (auto &p : arrow_triangles_data)
		add_point(p, bluecolor_p, ror::matrix4_rotation_around_x(ror::to_radians(90.0f)));

	// -Z
	for (auto &p : arrow_triangles_data)
		add_point(p, bluecolor_n, ror::matrix4_rotation_around_x(ror::to_radians(-90.0f)));

	debug_data.emplace_back(points_colors);
	topology_data.emplace_back(rhi::PrimitiveTopology::triangles);
}

template <class _type, class _index_type>
void make_sphere_triangles(std::vector<ror::Vector3<_type>> &a_vertex_buffer, std::vector<ror::Vector3<_index_type>> &a_index_buffer, uint32_t a_samples)        // 50 samples are enough for normal use
{
	// Sphere motivations
	// https://stackoverflow.com/a/26127012 for Fibonacci sphere
	// And convex hull solution from convhull_3d https://github.com/leomccormack/convhull_3d

	_type offset    = static_cast<_type>(2.0f) / static_cast<_type>(a_samples);
	_type increment = ror_pi * (static_cast<_type>(3.0f) - std::sqrt(static_cast<_type>(5.0f)));

	ch_vertex *vertices = new ch_vertex[a_samples];

	for (uint32_t sample = 0; sample < a_samples; ++sample)
	{
		_type u = static_cast<_type>(sample);
		_type y = ((u * offset) - static_cast<_type>(1.0f)) + (offset / static_cast<_type>(2.0f));
		_type r = std::sqrt(static_cast<_type>(1.0f) - std::pow(y, static_cast<_type>(2.0f)));

		_type phi = static_cast<_type>((sample) % a_samples) * increment;

		_type x = std::cos(phi) * r;
		_type z = std::sin(phi) * r;

		vertices[sample].x = x;
		vertices[sample].y = y;
		vertices[sample].z = z;

		// Save my vertices
		a_vertex_buffer.emplace_back(ror::Vector3<_type>(x, y, z));
	}

	int32_t *face_indices = nullptr;
	int32_t  face_count;

	convhull_3d_build(vertices, static_cast<int32_t>(a_samples), &face_indices, &face_count);

	for (int32_t i = 0; i < face_count * 3; i += 3)
	{
		// Save my indices
		a_index_buffer.emplace_back(ror::Vector3<_index_type>(
		    static_cast<uint32_t>(face_indices[i]),
		    static_cast<uint32_t>(face_indices[i + 1]),
		    static_cast<uint32_t>(face_indices[i + 2])));
	}

	delete[] vertices;
	delete[] face_indices;
}

}        // namespace ror
