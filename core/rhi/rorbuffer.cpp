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

#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rorbuffer.hpp"
#include <cassert>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string_view>

namespace rhi
{
Buffer::Buffer()
{
	this->m_semantics.reserve(32);        // About enough for now, ror::ShaderSemantic is about 28 entries, if it grows in the future it doens't matter much
}

ptrdiff_t Buffer::_offset(ptrdiff_t a_bytes)
{
	assert(a_bytes > 0 && "Requested bytes must be positive");

	assert(this->m_filled_size + a_bytes < this->m_size_in_bytes && "Requesting more bytes than the buffer has available");

	this->m_filled_size += a_bytes;
	this->m_filled_size = static_cast<ptrdiff_t>(ror::align8(static_cast<uint64_t>(this->m_filled_size)));

	return this->m_filled_size - a_bytes;
}

ptrdiff_t Buffer::offset(ptrdiff_t a_bytes)
{
	// Do the allocation
	return this->_offset(a_bytes);
}

uint8_t *Buffer::request(ptrdiff_t a_bytes)
{
	return this->m_data.data() + this->_offset(a_bytes);
}

// Assumes data space is already allocated in this buffer so do request()/offset() and then multiple uploads() or request_upload() for bulk allocation and upload
void Buffer::upload(const uint8_t *a_data, size_t a_size, ptrdiff_t a_offset)
{
	assert(a_size <= std::numeric_limits<ptrdiff_t>::max() && "Buffer data too big for diff calculations");

	std::copy(a_data, a_data + a_size, this->m_data.data() + a_offset);
}

void Buffer::upload(const std::vector<uint8_t> &a_data, ptrdiff_t a_offset)
{
	// Do the allocation
	assert(a_data.size() <= std::numeric_limits<ptrdiff_t>::max() && "Buffer data too big for diff calculations");

	std::copy(a_data.begin(), a_data.end(), this->m_data.begin() + a_offset);
}

uint32_t Buffer::handle() noexcept
{
	return 0;
}

void Buffer::map() noexcept
{}

void Buffer::unmap() noexcept
{}

void Buffer::size(ptrdiff_t a_size) noexcept
{
	assert(a_size > 0 && "Buffer size must be positive");

	// TODO: Make this lazy allocated, perhaps on first _offset() call
	this->m_data.resize(static_cast<size_t>(a_size));
	this->m_size_in_bytes = a_size;
}

ptrdiff_t Buffer::size() const noexcept
{
	return this->m_size_in_bytes;
}

void Buffer::interleaved(bool a_interleaved) noexcept
{
	this->m_interleaved_local = a_interleaved;
}

bool Buffer::interleaved() const noexcept
{
	return this->m_interleaved_local;
}

void Buffer::emplace_semantic(std::pair<BufferSemantic, uint64_t> &&a_pair)
{
	this->m_semantics.emplace_back(std::move(a_pair));
}

std::pair<BufferSemantic, uint64_t> Buffer::semantic(size_t a_index) const noexcept
{
	assert(a_index < this->m_semantics.size());

	return this->m_semantics[a_index];
}

const std::vector<std::pair<BufferSemantic, uint64_t>> &Buffer::semantics() const noexcept
{
	return this->m_semantics;
}

void Buffer::_upload()        // TODO: To be implemented in renderer or via CRTP
{
	ror::log_critical("Implement me _upload");
}

void Buffer::_partial_upload(ptrdiff_t a_offset, ptrdiff_t a_length)        // TODO: To be implemented in renderer or via CRTP
{
	(void) a_offset;
	(void) a_length;

	ror::log_critical("Implement me _partial_upload");
}

void Buffer::gpu_upload() noexcept
{
	this->_upload();
}

define_translation_unit_vtable(Buffer)
{}

}        // namespace rhi
