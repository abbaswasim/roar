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
#include "foundation/rorhash.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorbuffer_view.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"
#include <cassert>
#include <string>
#include <vector>

namespace ror
{
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
	FORCE_INLINE       Mesh()                             = default;        //! Default constructor
	FORCE_INLINE       Mesh(const Mesh &a_other)          = default;        //! Copy constructor
	FORCE_INLINE       Mesh(Mesh &&a_other) noexcept      = default;        //! Move constructor
	FORCE_INLINE Mesh &operator=(const Mesh &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Mesh &operator=(Mesh &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Mesh() noexcept                         = default;        //! Destructor

	// Provides hash for a specific part of the mesh
	hash_64_t hash() const;
	hash_64_t vertex_hash(size_t a_primitive_index) const noexcept;
	hash_64_t fragment_hash(size_t a_primitive_index) const noexcept;
	hash_64_t program_hash(size_t a_primitive_index) const noexcept;
	void      generate_hash();
	size_t    primitives_count() const noexcept;
	void      update();
	void      upload(rhi::Device &a_device);

	// clang-format off
	FORCE_INLINE constexpr auto  weights_count() const noexcept  { return this->m_morph_weights.size();        }
	FORCE_INLINE constexpr auto &shader_buffer() const noexcept  { return this->m_morph_weights_shader_buffer; }
	// clang-format on

	// TODO: Flatten this into 'Mesh' into 'Models' etc to see if I get cache locallity
	using BoundingBoxAllocator = rhi::BufferAllocator<ror::BoundingBoxf>;

	// NOTE: m_parts, m_primitive_types and m_has_indices_states are not BufferAllocated,
	// because they will not be sent into the GPU, so don't need them in a big buffer
	// TODO: Although to save on its allocation costs, one can BufferAllocate those as well

	std::vector<hash_64_t>                                  m_vertex_hashes{};                           //! All the parts has a specific hash of its vertex shaders due to its VertexDescriptors etc
	std::vector<hash_64_t>                                  m_fragment_hashes{};                         //! All the parts has a specific hash of its fragment shaders due to its VertexDescriptors + Material etc
	std::vector<hash_64_t>                                  m_program_hashes{};                          //! All the parts has a specific hash of its program due to a combination of vertex and fragment hashes
	std::vector<rhi::VertexDescriptor>                      m_attribute_vertex_descriptors{};            //! All the parts that makes up the mesh, each part requires a VertexDescription(attributes and layouts)
	std::vector<std::vector<rhi::VertexDescriptor>>         m_morph_targets_vertex_descriptors{};        //! All the parts that makes up the mesh, each part requires a VertexDescription(attributes and layouts) for morph targets
	std::vector<rhi::PrimitiveTopology>                     m_primitive_types{};                         //! Should be init with rhi::PrimitiveTopology::triangle
	std::vector<bool>                                       m_has_indices_states{};                      //! Should be init with false
	std::vector<float32_t, rhi::BufferAllocator<float32_t>> m_morph_weights{};                           //! Optional morph weights provided per mesh
	std::vector<ror::BoundingBoxf, BoundingBoxAllocator>    m_bounding_boxes{};                          //! This is per part
	std::vector<int32_t, rhi::BufferAllocator<int32_t>>     m_material_indices{};                        //! Should be init with -1 and might not have valid values after load, Maybe add a default material
	std::vector<int32_t, rhi::BufferAllocator<int32_t>>     m_program_indices{};                         //! Should be init with -1 but should have valid values when fully loaded
	rhi::ShaderBuffer                                       m_morph_weights_shader_buffer{};             //! ShaderBuffers for joint_transforms within the skinning shader
	int32_t                                                 m_skin_index{-1};                            //! If the mesh has Skin their index is saved here, Should be init with -1
	uint64_t                                                m_hash{0};                                   //! Hash of this mesh depending on most of its properties
	std::string                                             m_name{"generic_mesh"};                      //! Name of this mesh
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
