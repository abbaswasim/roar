// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2021
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

#include "rhi/rorrhi_vertex_description.hpp"

namespace ror
{
class ROAR_ENGINE_ITEM Material;

class ROAR_ENGINE_ITEM Model
{
  public:
	FORCE_INLINE Model()                         = default;                   //! Default constructor
	FORCE_INLINE Model(const Model &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Model(Model &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Model &operator=(const Model &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Model &operator=(Model &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Model() noexcept                          = default;        //! Destructor

	struct Parts
	{
		uint32_t m_vertex_offset{0};
		uint32_t m_vertex_count{0};
		uint32_t m_index_offset{0};
		uint32_t m_index_count{0};
		uint32_t m_material_id{0};
	};

  private:
	// rhi::Buffer                        m_vertices_buffer{};        //! This doesn't work, its a list of these what about those
	// rhi::Buffer                        m_indices_buffer{};         //! This doesn't work, its a list of these what about those
	std::vector<rhi::VertexDescriptor> m_vertex_descriptors{};        //! Contains all the buffers required for attributes
	std::vector<rhi::Model::Parts>     m_parts{};
	std::vector<rhi::Material>         m_materials{};
	// uint32_t                           m_index_count{0};
	// uint32_t                           m_vertex_count{0};
};

// static_assert(std::is_trivially_copyable<Model>::value, "Model is not trivially copyable");

}        // namespace ror
