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

#include "foundation/rortypes.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"
#include "rhi/rortypes.hpp"

namespace ror
{
void Mesh::update_primitive_hash(size_t a_primitive_id, size_t a_skin_count, hash_64_t a_material_hash)
{
	auto &vertex_attribute_descriptor              = this->m_attribute_vertex_descriptors[a_primitive_id];
	auto &morph_target_vertex_attribute_descriptor = this->m_morph_targets_vertex_descriptors[a_primitive_id];
	auto &vertex_hash                              = this->m_vertex_hashes[a_primitive_id];
	auto &fragment_hash                            = this->m_fragment_hashes[a_primitive_id];
	auto &program_hash                             = this->m_program_hashes[a_primitive_id];

	// Setup vertex hash
	vertex_hash = vertex_attribute_descriptor.hash_64();

	for (auto &attrib : morph_target_vertex_attribute_descriptor)
		hash_combine_64(vertex_hash, attrib.hash_64());

	// Only check if we have weights
	auto weights_count = this->m_morph_weights.size();

	if (weights_count > 0)
		hash_combine_64(vertex_hash, hash_64(&weights_count, sizeof(weights_count)));

	if (this->m_skin_index != -1)
		hash_combine_64(vertex_hash, a_skin_count);

	// Setup material hash
	// Adding material index here because if material is the same for specific vertex attributes then we use the same shader
	fragment_hash = vertex_hash;
	if (this->m_material_indices[a_primitive_id] != -1)
		hash_combine_64(fragment_hash, a_material_hash);

	// Setup program hash
	program_hash = vertex_hash;
	hash_combine_64(program_hash, fragment_hash);
}

void Mesh::resize(size_t a_primitives_count)
{
	this->m_attribute_vertex_descriptors.resize(a_primitives_count);
	this->m_morph_targets_vertex_descriptors.resize(a_primitives_count);
	this->m_primitive_types.resize(a_primitives_count);
	this->m_has_indices_states.resize(a_primitives_count);
	this->m_vertex_hashes.resize(a_primitives_count);
	this->m_fragment_hashes.resize(a_primitives_count);
	this->m_program_hashes.resize(a_primitives_count);

	// The last 2 are important and definitely needs reserving because these are BufferAllocated
	this->m_bounding_boxes.resize(a_primitives_count);
	this->m_material_indices.resize(a_primitives_count);
	this->m_program_indices.resize(a_primitives_count);
}

void Mesh::generate_hash()
{
	for (size_t i = 0; i < this->m_vertex_hashes.size(); ++i)
		hash_combine_64(this->m_hash, this->vertex_hash(i));

	for (size_t i = 0; i < this->m_fragment_hashes.size(); ++i)
		hash_combine_64(this->m_hash, this->fragment_hash(i));

	for (size_t i = 0; i < this->m_program_hashes.size(); ++i)
		hash_combine_64(this->m_hash, this->program_hash(i));
}

void Mesh::update()
{}

void Mesh::upload(rhi::Device &a_device)
{
	(void) a_device;

	if (this->weights_count() > 0)
	{
		if constexpr (ror::get_build() == ror::BuildType::build_debug)
		{}

		this->update();
	}
}

void Mesh::mesh_data(size_t a_primitive_index, const std::vector<std::tuple<rhi::BufferSemantic, rhi::VertexFormat, uint8_t *, uint32_t, uint32_t>> &a_data, rhi::BuffersPack &a_buffers_pack)
{
	auto &vertex_descriptor = this->m_attribute_vertex_descriptors[a_primitive_index];

	std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>> attribs_data{};

	for (auto &data : a_data)
	{
		auto [buffer_semantic, vertex_format, data_pointer, size, stride] = data;

		vertex_descriptor.add(buffer_semantic, vertex_format, &a_buffers_pack);

		std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer, size, stride};
		attribs_data.emplace(buffer_semantic, std::move(data_tuple));
	}

	vertex_descriptor.upload(attribs_data, &a_buffers_pack);
}

void Mesh::update_mesh_data(size_t a_primitive_index, std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>> &a_attribs_data, rhi::BuffersPack &a_buffers_pack)
{
	auto &vertex_descriptor = this->m_attribute_vertex_descriptors[a_primitive_index];

	vertex_descriptor.update(a_attribs_data, &a_buffers_pack);
}
}        // namespace ror
