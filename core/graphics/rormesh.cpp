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
#include "rhi/rortypes.hpp"

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
}        // namespace ror
