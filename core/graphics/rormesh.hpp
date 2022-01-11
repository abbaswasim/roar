// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021-2022
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

#include "bounds/rorbounding.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorbuffer_view.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"
#include <vector>

namespace ror
{
constexpr uint32_t max_morph_targets{8};                                                                   // TODO: Read from config somewhere
constexpr uint32_t max_vertex_attributes{enum_to_type_cast(rhi::AttributeIndex::vertex_index) + 1};        // Anything that can be a vertex attribute is until vertex_index

#define item(_enum) _enum##_index
#define item_value(_enum)

// NOTE: Its not an enum class
enum AttributeIndices
{
	describe_buffer_semantics(item)
};

#undef item
#undef item_value

class ROAR_ENGINE_ITEM Mesh final
{
  public:
	FORCE_INLINE       Mesh()                        = default;             //! Default constructor
	FORCE_INLINE       Mesh(const Mesh &a_other)     = default;             //! Copy constructor
	FORCE_INLINE       Mesh(Mesh &&a_other) noexcept = default;             //! Move constructor
	FORCE_INLINE Mesh &operator=(const Mesh &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Mesh &operator=(Mesh &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Mesh() noexcept                         = default;        //! Destructor

	// TODO: Flatten this into 'Mesh' into 'Models' etc to see if I get cache locallity
	using BoundingBoxAllocator = rhi::BufferAllocator<ror::BoundingBoxf>;

	// NOTE: m_parts, m_primitive_types and m_has_indices_states are not BufferAllocated,
	// because they will not be sent into the GPU, so don't need them int a big buffer
	// TODO: Although to save on its allocation costs, one can BufferAllocate those as well

	std::vector<rhi::VertexDescriptor>                                m_attribute_vertex_descriptors{};            //! All the parts that makes up the mesh, each part requires a VertexDescription(attributes and layouts)
	std::vector<std::array<rhi::VertexDescriptor, max_morph_targets>> m_morph_targets_vertex_descriptors{};        //! All the parts that makes up the mesh, each part requires a VertexDescription(attributes and layouts)
	std::vector<rhi::PrimitiveTopology>                               m_primitive_types{};                         //! Its here and not in Drawable for memory locality reasons, should be init with rhi::PrimitiveTopology::triangle
	std::vector<bool>                                                 m_has_indices_states{};                      //! Its here and not in Drawable for memory locality reasons, should be init with false
	std::vector<float32_t, rhi::BufferAllocator<float32_t>>           m_morph_weights{};                           //! Optional morph weights provided per mesh
	std::vector<ror::BoundingBoxf, BoundingBoxAllocator>              m_bounding_boxes{};                          //! Its here and not in Drawable for memory locality reasons, this is per part
	std::vector<int32_t, rhi::BufferAllocator<int32_t>>               m_material_indices{};                        //! Its here and not in Drawable for memory locality reasons, should be init with -1
};

}        // namespace ror

namespace rhi
{
// Define type to semantic for buffers allocator
define_type_to_shader_semantics(ror::Mesh)
{
	return rhi::BufferSemantic::mesh_data;
}

define_type_to_shader_semantics(ror::BoundingBoxf)
{
	return rhi::BufferSemantic::bounding_box_data;
}

define_type_to_shader_semantics(int32_t)
{
	return rhi::BufferSemantic::index_data;
}

define_type_to_shader_semantics(float32_t)
{
	return rhi::BufferSemantic::vertex_morph_weight;
}
}        // namespace rhi
