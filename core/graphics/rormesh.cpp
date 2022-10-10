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

#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"

namespace ror
{
hash_64_t Mesh::hash() const
{
	return this->m_hash;
}

hash_64_t Mesh::vertex_hash(size_t a_primitive_index) const noexcept
{
	return this->m_vertex_hashes[a_primitive_index];
}

hash_64_t Mesh::fragment_hash(size_t a_primitive_index) const noexcept
{
	return this->m_fragment_hashes[a_primitive_index];
}

hash_64_t Mesh::program_hash(size_t a_primitive_index) const noexcept
{
	return this->m_program_hashes[a_primitive_index];
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

size_t Mesh::primitives_count() const noexcept
{
	return this->m_attribute_vertex_descriptors.size();
}

void Mesh::update()
{
	auto mapping = this->m_morph_weights_shader_buffer.map();
	std::memcpy(mapping, m_morph_weights.data(), sizeof(float32_t) * this->weights_count());

	this->m_morph_weights_shader_buffer.unmap();
}

void Mesh::upload(rhi::Device &a_device)
{
	// Will create UBO for something like the following
	/*
	  const uint weights_count = 1;

	  layout(set = 0, binding = 1) uniform morph_weight
	  {
	      float morph_weights[weights_count];
	  } in_morph_weights;
	*/

	auto &shader_buffer = this->m_morph_weights_shader_buffer.shader_buffer();
	shader_buffer.add_entry("morph_weights", rhi::VertexFormat::float32_1, static_cast_safe<uint32_t>(this->weights_count()));

	this->m_morph_weights_shader_buffer.init(a_device, sizeof(float32_t) * static_cast_safe<uint32_t>(this->weights_count()));

	if constexpr (ror::get_build() == ror::BuildType::build_debug)
	{
		auto entries = shader_buffer.entries();
		(void) entries;
		assert(entries.size() == 1 && "More entries in morph weights UBO than expected");
		assert(entries[0]->m_offset == 0 && "Invalid offset for morph weights UBO entry");
		assert(entries[0]->m_size == sizeof(float32_t) && "Invalid size for morph weights UBO entry");
		// Can't trust stride in this case because of float to vec4 correction for glsl
		// assert(entries[0]->m_stride == sizeof(float32_t) * this->weights_count() && "Invalid stride for morph weights UBO entry");
	}

	this->update();
}
}        // namespace ror
