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

#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rortypes.hpp"

namespace rhi
{
/**
 * A view of the device buffer
 */
class ROAR_ENGINE_ITEM BufferView
{
  public:
	FORCE_INLINE             BufferView()                              = default;        //! Constructor
	FORCE_INLINE             BufferView(const BufferView &a_other)     = default;        //! Copy constructor
	FORCE_INLINE             BufferView(BufferView &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE BufferView &operator=(const BufferView &a_other) = default;             //! Copy assignment operator
	FORCE_INLINE BufferView &operator=(BufferView &&a_other) noexcept = default;         //! Move assignment operator
	FORCE_INLINE ~BufferView() noexcept                               = default;         //! Destructor

	FORCE_INLINE uint32_t       index() noexcept;
	FORCE_INLINE decltype(auto) element(size_t a_index) noexcept;
	FORCE_INLINE size_t         data(size_t a_item_index) noexcept
	{
		(void) a_item_index;
		// const auto &bp = rhi::get_buffers_pack();
		// const auto &b  = bp.buffer(static_cast<size_t>(this->m_buffer_index));
		// return b.ptr(a_item_index * m_stride) + this->m_offset;
		return 0;
	}

	// private:
	size_t  m_offset{0};                        // Start of the data
	size_t  m_stride{0};                        // Start of the next value
	size_t  m_count{0};                         // How much of the data we need
	int32_t m_buffer_index{-1};                 // Where the data is
	Format  m_format{Format::float32_4};        // What is the size in bytes and whether its scalar, vector or matrix type
};

}        // namespace rhi
