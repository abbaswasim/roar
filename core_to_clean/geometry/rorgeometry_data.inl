// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2019
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

namespace ror
{
FORCE_INLINE GeometryData::GeometryData(void)
{
	this->m_vertices_array         = nullptr;
	this->m_skinned_vertices_array = nullptr;
	this->m_skinned_normals_array  = nullptr;
	this->m_normals_array          = nullptr;
	this->m_texture_coords_array   = nullptr;
	this->m_vertex_weights_array   = nullptr;
	//this->m_VertexInfluences		= nullptr;
	this->m_vertex_influences = new std::vector<VertexInfluence *>();
}

FORCE_INLINE float32_t *GeometryData::get_vertices_array() const
{
	return m_vertices_array;
}

FORCE_INLINE void GeometryData::set_vertices_array(float32_t *a_vertices)
{
	m_vertices_array = a_vertices;
}

FORCE_INLINE float32_t *GeometryData::get_skinned_vertices_array() const
{
	return m_skinned_vertices_array;
}

FORCE_INLINE void GeometryData::set_skinned_vertices_array(float32_t *a_vertices)
{
	m_skinned_vertices_array = a_vertices;
}

FORCE_INLINE float32_t *GeometryData::get_vertex_weights_array() const
{
	return m_vertex_weights_array;
}

FORCE_INLINE void GeometryData::set_vertex_weights_array(float32_t *a_vertices_weights)
{
	m_vertex_weights_array = a_vertices_weights;
}

FORCE_INLINE float32_t *GeometryData::get_normals_array() const
{
	return m_normals_array;
}

FORCE_INLINE void GeometryData::set_normals_array(float32_t *a_normals)
{
	m_normals_array = a_normals;
}

FORCE_INLINE float32_t *GeometryData::get_texture_coords_array() const
{
	return m_texture_coords_array;
}

FORCE_INLINE void GeometryData::set_texture_coords_array(float32_t *a_texture_coods)
{
	m_texture_coords_array = a_texture_coods;
}

FORCE_INLINE uint32_t GeometryData::get_vertices_array_size() const
{
	return m_vertices_array_size;
}

FORCE_INLINE void GeometryData::set_vertices_array_size(uint32_t a_vertices_size)
{
	m_vertices_array_size = a_vertices_size;
}

FORCE_INLINE uint32_t GeometryData::get_normals_array_size() const
{
	return m_normals_array_size;
}

FORCE_INLINE void GeometryData::set_normals_array_size(uint32_t a_normals_size)
{
	m_normals_array_size = a_normals_size;
}

FORCE_INLINE uint32_t GeometryData::get_texture_coords_array_size() const
{
	return m_texture_coords_array_size;
}

FORCE_INLINE void GeometryData::set_texture_coords_array_size(uint32_t a_tex_coords_size)
{
	m_texture_coords_array_size = a_tex_coords_size;
}

}        // namespace ror
