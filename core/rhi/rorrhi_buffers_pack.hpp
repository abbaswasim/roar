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

#include "rhi/rorbuffers_format.hpp"
#include <unordered_map>
#include <vector>

namespace rhi
{
// template <bool>
// class HasPositions
// {};

// template <>
// class HasPositions<true>
// {
//   public:
//	ror::Vector3f m_positions;
// };

// template <class _type, bool>
// class HasPositionsSOA
// {};

// // This is probably much better for copying later on
// template <class _type>
// class HasPositionsSOA<_type, true>
// {
//   public:
//	std::vector<_type> m_positions;
// };

/**
 * Describes how many buffers are created for static geometry data and with what layout
 * This should only be used for static data that won't be touched later for anything else
 * use Buffer directly. Ideally this will try to create
 *     1 buffer for positions,
 *     1 for all other vertex attributes,
 *     1 for index buffers,
 *     1 for instanced data
 * But depending on the config provided the buffers created migh be very different
 */
class ROAR_ENGINE_ITEM BuffersPack
{
  public:
	FORCE_INLINE BuffersPack(const BuffersPack &a_other)     = delete;                    //! Copy constructor
	FORCE_INLINE BuffersPack(BuffersPack &&a_other) noexcept = delete;                    //! Move constructor
	FORCE_INLINE BuffersPack &operator=(const BuffersPack &a_other) = delete;             //! Copy assignment operator
	FORCE_INLINE BuffersPack &operator=(BuffersPack &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE virtual ~BuffersPack() noexcept                        = default;        //! Destructor

	declare_translation_unit_vtable();

	FORCE_INLINE BuffersPack(const ror::BuffersFormat &a_buffers_format)
	{
		this->m_buffers = a_buffers_format.m_buffer_packs[a_buffers_format.m_current_format].m_buffers;        // Copy the buffers over

		uint32_t i = 0;
		for (const auto &bfr : this->m_buffers)
		{
			const auto &semantics = bfr.semantics();

			for (const auto &sem : semantics)
			{
				this->m_attribute_indices.emplace(sem.first, i);        // Creating mapping here, If a specific ShaderSemantic is defined for multiple buffers, behaviour is undefined
			}
			i++;
		}
	}

	FORCE_INLINE uint32_t attribute_buffer_index(ShaderSemantic a_semantic) const
	{
		return this->m_attribute_indices.at(a_semantic);
	}

	FORCE_INLINE uint64_t attribute_buffer_offset(ShaderSemantic a_semantic, uint64_t a_bytes)
	{
		uint32_t index = this->attribute_buffer_index(a_semantic);

		return this->m_buffers[index].request(a_bytes);
	}

	FORCE_INLINE bool attribute_buffer_interleaved(ShaderSemantic a_semantic) const
	{
		return this->m_buffers[this->attribute_buffer_index(a_semantic)].interleaved();
	}

	/**
	 * Returns a pair with buffer index and the offsets in that buffer where the data is copied
	 */
	// TODO: This will be contentious amongs threads, think about how would this work asynchronously
	FORCE_INLINE std::pair<uint64_t, uint64_t> upload_data(ShaderSemantic a_semantic, uint8_t &a_data, uint64_t a_bytes)
	{
		uint32_t index = this->attribute_buffer_index(a_semantic);
		return std::make_pair(index, this->m_buffers[index].upload(a_data, a_bytes));
	}

  private:
	std::vector<Buffer>                          m_buffers{};                  //! All buffers created for different type data
	std::unordered_map<ShaderSemantic, uint32_t> m_attribute_indices{};        //! All indices for any of the ShaderSemantic type, Position and its buffer index, Normal and its buffer index etc
};

define_translation_unit_vtable(BuffersPack)
{}

// TODO: Eventually this should be owned by some higher entity and provided to model loaders and VertexDescriptions
BuffersPack &get_buffers_pack()
{
	static BuffersPack bfp{ror::get_buffers_format()};
	return bfp;
}

}        // namespace rhi
