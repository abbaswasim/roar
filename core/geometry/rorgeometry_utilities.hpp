// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2020
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

#include "foundation/rortypes.hpp"
#include "math/rorvector_functions.hpp"
#include "rhi/rortypes.hpp"

namespace ror
{
// a_clock_wise == true because its Metal's default
/**
 * @brief      Creates a Box with from minimum and maximum
 * @param      a_minimum Minimum of the box to create
 * @param      a_maximum Maximum of the box to create
 * @param      a_vertex_buffer A vector to append the vertices created to
 */
template <class _type>
FORCE_INLINE void make_box_triangles(std::vector<_type> &a_vertex_buffer, const _type &a_minimum, const _type &a_maximum, bool a_clock_wise = true);

/**
 * @brief      Creates a Box with a_size in all 3 or 2 dimensions
 * @param      a_size a 2D or 3D vector defining its size
 * @param      a_origin Origin of the Box
 * @param      a_output A vector to append the vertices created to
 */
template <class _type>
FORCE_INLINE void make_box_triangles(const _type &a_size, const _type &a_origin, std::vector<_type> &a_output, bool a_clock_wise = true);

/**
 * @brief      Creates a Box with size in all 3 or 2 dimensions
 * @param      a_size float defining its size in all 3 or 2 dimentions
 * @param      a_origin Origin of the Box
 * @param      a_output A vector to append the vertices created to
 */
template <class _type>
FORCE_INLINE void make_box_triangles(float32_t a_size, const _type &a_origin, std::vector<_type> &a_output, bool a_clock_wise = true);

/**
 * @brief      Creates a Box with size in all 3 or 2 dimensions at origin
 * @param      a_size float defining its size in all 3 or 2 dimentions
 * @param      a_output A vector to append the vertices created to
 */
template <class _type>
FORCE_INLINE void make_box_triangles(float32_t a_size, std::vector<_type> &a_output, bool a_clock_wise = true);

/**
 * @brief      Creates a Box with size in all 3 or 2 dimensions at origin
 * @param      a_size float defining its size in all 3 or 2 dimentions
 * @return     return A vector to append the vertices created to
 */
template <class _type>
FORCE_INLINE std::vector<_type> make_box_triangles(float32_t a_size, bool a_clock_wise = true);

/**
 * @brief      Creates a Box with a_size in all 3 or 2 dimensions
 * @param      a_size a 2D or 3D vector defining its size
 * @param      a_origin Origin of the Box
 * @param      a_vertex_buffer A vector to append the vertices created to
 * @param      a_index_buffer A vector to append the indices created to
 */
template <class _type, class _index_type = uint32_t>
FORCE_INLINE void make_box_triangles_indexed(const _type &a_size, const _type &a_origin,
                                             std::vector<_type>                     &a_vertex_buffer,
                                             std::vector<ror::Vector3<_index_type>> &a_index_buffer, bool a_clock_wise = true);

/**
 * @brief      Creates a Box with unit size 3 or 2 dimensions at the origin
 * @param      a_vertex_buffer A vector to append the vertices created to
 * @param      a_index_buffer A vector to append the indices created to
 */
template <class _type, class _index_type = uint32_t>
FORCE_INLINE void make_box_triangles_indexed(std::vector<_type> &a_vertex_buffer, std::vector<ror::Vector3<_index_type>> &a_index_buffer, bool a_clock_wise = true);

// TODO: Provide option for inward or outward normals

/**
    Same set of functions for creating a box but this time return only lines
 */
template <class _type>
FORCE_INLINE void make_box_lines(std::vector<_type> &a_vertex_buffer, const _type &a_minimum, const _type &a_maximum);
template <class _type>
FORCE_INLINE void make_box_lines(const _type &a_size, const _type &a_origin, std::vector<_type> &a_output);
template <class _type>
FORCE_INLINE void make_box_lines(float32_t a_size, const _type &a_origin, std::vector<_type> &a_output);
template <class _type>
FORCE_INLINE void make_box_lines(float32_t a_size, std::vector<_type> &a_output);
template <class _type>
FORCE_INLINE std::vector<_type> make_box_lines(float32_t a_size);
template <class _type, class _index_type = uint32_t>
FORCE_INLINE void make_box_lines_indexed(const _type &a_size, const _type &a_origin,
                                         std::vector<_type>                     &a_vertex_buffer,
                                         std::vector<ror::Vector2<_index_type>> &a_index_buffer);
template <class _type, class _index_type = uint32_t>
FORCE_INLINE void make_box_lines_indexed(std::vector<_type> &a_vertex_buffer, std::vector<ror::Vector2<_index_type>> &a_index_buffer);

/**
 * @brief      Creates a unit sphere with samples(=default 50) number of points
 * @param      a_samples number of points in the sphere
 * @param      a_vertex_buffer A 3D vertex buffer with given type for all the vertices
 * @param      a_index_buffer A 3D index buffer with given type for all the indices
 */
template <class _type = float32_t, class _index_type = uint32_t>
void make_sphere_triangles(std::vector<ror::Vector3<_type>>       &a_vertex_buffer,
                           std::vector<ror::Vector3<_index_type>> &a_index_buffer,
                           uint32_t                                a_samples = 50);

void make_sphere_triangles(std::vector<ror::Vector3f> &a_vertex_buffer,
                           int32_t                     a_subdivisions  = 3,
                           float32_t                   a_corner_radius = 1.0f);        // Corner radius of 1.0 creates a unit sphere

void make_cylinder_triangles(std::vector<ror::Vector3f> &a_vertex_buffer,
                             int32_t                     a_slices  = 10,
                             int32_t                     a_stackes = 1);

void make_cone_triangles(std::vector<ror::Vector3f> &a_vertex_buffer,
                         int32_t                     a_slices  = 10,
                         int32_t                     a_stackes = 1);

void make_tetrahedron_triangles(std::vector<ror::Vector3f> &a_vertex_buffer);

void make_hemisphere_triangles(std::vector<ror::Vector3f> &a_vertex_buffer,
                               int32_t                     a_slices  = 10,
                               int32_t                     a_stackes = 5);

void make_disk_triangles(std::vector<ror::Vector3f> &a_vertex_buffer,
                         ror::Vector3f               a_center = {0.0f, 0.0f, 0.0f},
                         ror::Vector3f               a_normal = {0.0f, 1.0f, 0.0f},
                         float32_t                   a_radius = 1.0,
                         int32_t                     a_slices = 10);

/**
 * @brief      Creates the a single arraow that is used for one axis but can be used for other things
 */
void create_arrow(std::vector<ror::Vector3f> &arrow_triangles_data, ror::Vector3f a_scale);

/**
 * @brief      Creates the cartesian axis, in different colors from a cylinder, hemisphere and a disk
 */
void create_axis(std::vector<std::vector<float32_t>> &debug_data, std::vector<rhi::PrimitiveTopology> &topology_data);

/**
 * @brief      Creates a sphere with samples number of points in all 3 or 2 dimensions at origin
 * @param      a_samples number of points
 * @param      a_output a vector to append the vertices created into
 */
template <class _type>
FORCE_INLINE void make_sphere_triangles(uint32_t a_samples, const _type &a_origin, std::vector<_type> &a_output);

/**
 * @brief      Creates normals for triangles
 * @param      a_vertex_buffer Vertex buffer to use for calculating normals
 * @param      a_index_buffer Optional index buffer
 * @param      a_size Amount of trianlges
 * @param      a_output Normals for each vertex are created in a_output
 */
template <class _type>
FORCE_INLINE void add_normals(const std::vector<_type> &a_vertex_buffer,
                              uint32_t a_size, std::vector<_type> &a_output,
                              const std::vector<_type> *a_index_buffer = nullptr);

/**
 * @brief      Creates colors for triangles
 * @param      a_vertex_buffer Vertex buffer to use for calculating colors
 * @param      a_index_buffer Optional index buffer
 * @param      a_size Amount of trianlges
 * @param      a_output Normals for each vertex are created in a_output
 */
template <class _type>
FORCE_INLINE void add_colors(const std::vector<_type> &a_vertex_buffer,
                             uint32_t a_size, std::vector<_type> &a_output,
                             const std::vector<_type> *a_index_buffer = nullptr);

}        // namespace ror

#include "rorgeometry_utilities.hh"
