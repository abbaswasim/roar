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

#include "foundation/rortypes.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rorbuffers_format.hpp"
#include "rhi/rortypes.hpp"
#include <cassert>
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace rhi
{

/**
 * Describes how many buffers are created for static geometry data and with what layout
 * This should only be used for static data that won't be touched later for anything else
 * use Buffer directly. Ideally this will try to create
 *     1 buffer for positions, joints and weights,
 *     1 for all other vertex attributes,
 *     1 for index buffers,
 *     1 for instanced data
 * But depending on the config provided the buffers created migh be very different
 */
class ROAR_ENGINE_ITEM BuffersPack final
{
  public:
	FORCE_INLINE              BuffersPack(const BuffersPack &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE              BuffersPack(BuffersPack &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE BuffersPack &operator=(const BuffersPack &a_other)       = delete;         //! Copy assignment operator
	FORCE_INLINE BuffersPack &operator=(BuffersPack &&a_other) noexcept   = delete;         //! Move assignment operator
	FORCE_INLINE ~BuffersPack() noexcept                                  = default;        //! Destructor

	FORCE_INLINE BuffersPack(ror::BuffersFormat &&a_buffers_format)
	{
		this->m_buffers = std::move(a_buffers_format.m_buffer_packs[a_buffers_format.m_current_format].m_buffers);        // move the buffers over

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

	FORCE_INLINE uint32_t attribute_buffer_index(BufferSemantic a_semantic) const
	{
		return this->m_attribute_indices.at(a_semantic);
	}

	FORCE_INLINE ptrdiff_t attribute_buffer_offset(BufferSemantic a_semantic, ptrdiff_t a_bytes)
	{
		const uint32_t index = this->attribute_buffer_index(a_semantic);

		return this->m_buffers[index].offset(a_bytes);
	}

	FORCE_INLINE bool attribute_buffer_interleaved(BufferSemantic a_semantic) const
	{
		return this->m_buffers[this->attribute_buffer_index(a_semantic)].interleaved();
	}

	/**
	 * Returns a pair with buffer index and the offsets in that buffer where the data is copied
	 */
	// This will be contentious amongst threads, Since Buffer is thread safe this works asynchronously
	FORCE_INLINE void upload(BufferSemantic a_semantic, const std::vector<uint8_t> &a_data, ptrdiff_t a_offset)
	{
		const uint32_t index = this->attribute_buffer_index(a_semantic);
		this->m_buffers[index].upload(a_data, a_offset);
	}

	FORCE_INLINE void upload(size_t a_index, const std::vector<uint8_t> &a_data, ptrdiff_t a_offset)
	{
		this->m_buffers[a_index].upload(a_data, a_offset);
	}

	/**
	 * Uploads all the data in all the buffers to the GPU
	 */
	FORCE_INLINE void upload()
	{
		for (auto &buffer : this->m_buffers)
			buffer.gpu_upload();
	}

	/**
	 * Uploads the buffer for a specific semantic to the GPU
	 */
	FORCE_INLINE void upload(rhi::BufferSemantic a_semantic)
	{
		this->buffer(a_semantic).gpu_upload();
	}

	/**
	 * Uploads the buffer for a specific index to the GPU
	 */
	FORCE_INLINE void upload(size_t a_index)
	{
		this->m_buffers[a_index].gpu_upload();
	}

	/**
	 * Returns a buffer by semantic
	 */
	FORCE_INLINE rhi::Buffer<rhi::Static> &buffer(BufferSemantic a_semantic)
	{
		const uint32_t index = this->attribute_buffer_index(a_semantic);
		return this->m_buffers[index];
	}
	FORCE_INLINE const rhi::Buffer<rhi::Static> &buffer(BufferSemantic a_semantic) const
	{
		const uint32_t index = this->attribute_buffer_index(a_semantic);
		return this->m_buffers[index];
	}

	/**
	 * Returns a buffer by index, shouldn't be used if semantics are a better choice
	 */
	FORCE_INLINE rhi::Buffer<rhi::Static> &buffer(size_t a_index)
	{
		return this->m_buffers[a_index];
	}
	FORCE_INLINE const rhi::Buffer<rhi::Static> &buffer(size_t a_index) const
	{
		return this->m_buffers[a_index];
	}

	/**
	 * Sets BufferPack to be free-able, should only be called once and at the end of the program
	 */
	FORCE_INLINE void free()
	{
		assert(this->m_ready_to_free == false && "Free called on BufferPack again");
		this->m_ready_to_free = true;
	}

	FORCE_INLINE bool ready_to_free() const
	{
		return this->m_ready_to_free;
	}

  private:
	std::vector<rhi::Buffer<rhi::Static>>        m_buffers{};                   //! All buffers created for different type data
	std::unordered_map<BufferSemantic, uint32_t> m_attribute_indices{};         //! All indices for any of the ShaderSemantic type, Position and its buffer index, Normal and its buffer index etc
	bool                                         m_ready_to_free{false};        //! True when we can deallocate all buffers safely, doesn't have to be atomic, because this is end of everything
};

FORCE_INLINE BuffersPack &get_buffers_pack()
{
	// Everything in bfp constructor are r-values because I don't want to keep a copy of BuffersFormat around its a big store of Buffers
	// The following loads the format via BuffersFormatConfig's one argument ctor from buffers_format setting and then moves it into bfp
	static BuffersPack bfp{ror::BuffersFormatConfig{ror::settings().m_buffers_format}.move_buffers_format()};        // The buffers_format config file name is provided in settings.json
	return bfp;
}

}        // namespace rhi
