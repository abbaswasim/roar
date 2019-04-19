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

#pragma once

#include "memory_manager/rormemory_check.h"
#include "rorvertex_influence.h"

namespace ror
{
class ROAR_ENGINE_ITEM CCBA;
class ROAR_ENGINE_ITEM GeometryData
{
  public:
	FORCE_INLINE GeometryData(void);
	~GeometryData(void);

	FORCE_INLINE float32_t *get_vertices_array() const;
	FORCE_INLINE void       set_vertices_array(float32_t *a_vertices);
	FORCE_INLINE float32_t *get_skinned_vertices_array() const;
	FORCE_INLINE void       set_skinned_vertices_array(float32_t *a_vertices);
	FORCE_INLINE float32_t *get_vertex_weights_array() const;
	FORCE_INLINE void       set_vertex_weights_array(float32_t *a_vertices_weights);
	FORCE_INLINE float32_t *get_normals_array() const;
	FORCE_INLINE void       set_normals_array(float32_t *a_normals);
	FORCE_INLINE float32_t *get_texture_coords_array() const;
	FORCE_INLINE void       set_texture_coords_array(float32_t *a_texture_coods);
	FORCE_INLINE uint32_t get_vertices_array_size() const;
	FORCE_INLINE void     set_vertices_array_size(uint32_t a_vertices_size);
	FORCE_INLINE uint32_t get_normals_array_size() const;
	FORCE_INLINE void     set_normals_array_size(uint32_t a_normals_size);
	FORCE_INLINE uint32_t get_texture_coords_array_size() const;
	FORCE_INLINE void     set_texture_coords_array_size(uint32_t a_tex_coords_size);

  private:
	// CBA should be able to access the data
	friend class ROAR_ENGINE_ITEM CCBA;

	float32_t *m_vertices_array;                // All the Vertices
	float32_t *m_skinned_vertices_array;        // All the Skinned Vertices
	float32_t *m_normals_array;                 // All the Normals
	float32_t *m_skinned_normals_array;         // All the Skinned Vertices
	float32_t *m_texture_coords_array;          // All the Texture Coordinates
	float32_t *m_vertex_weights_array;          // All the Vertex Weights
	//CoreEngine::CVertexInfluence		**m_VertexInfluences;
	std::vector<VertexInfluence *> *m_vertex_influences;        // Vertices Influences

	uint32_t m_vertices_array_size;
	uint32_t m_normals_array_size;
	uint32_t m_texture_coords_array_size;
	uint32_t m_vertex_weights_array_size;
};
}        // namespace ror

#include "rorgeometry_data.inl"
