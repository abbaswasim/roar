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

#include "profiling/rorlog.hpp"
#include "graphics/rorbuffer.hpp"

namespace ror
{
template <typename _type>
Buffer<_type>::Buffer()
{
	this->m_semantics.reserve(32);        // About enough for now, ror::ShaderSemantic is about 28 entries, if it grows in the future it doens't matter much
	this->m_mutex = std::make_shared<std::mutex>();
}

template <typename _type>
template <typename U, std::enable_if_t<std::is_same<U, Static>::value, bool>>
ptrdiff_t Buffer<_type>::_offset(ptrdiff_t a_bytes)
{
	assert(a_bytes > 0 && "Requested bytes must be positive");

	std::lock_guard<std::mutex> mtx(*this->m_mutex);

	if (this->m_filled_size + a_bytes > this->m_size_in_bytes)
		ror::log_critical("Requesting {} bytes but size is {}", (this->m_filled_size + a_bytes), m_size_in_bytes);

	assert(this->m_filled_size + a_bytes <= this->m_size_in_bytes && "Requesting more bytes than available in the static buffer");

	auto offset = this->m_filled_size;

	this->m_filled_size += a_bytes;
	this->m_filled_size = static_cast<ptrdiff_t>(ror::align8(static_cast<uint64_t>(this->m_filled_size)));        // Aligning next offset for the most common denominator, other types are fine

	return offset;
}

template <typename _type>
template <typename U, std::enable_if_t<std::is_same<U, Dynamic>::value, bool>>
ptrdiff_t Buffer<_type>::_offset(ptrdiff_t a_bytes)
{
	assert(a_bytes > 0 && "Requested bytes must be positive");

	std::lock_guard<std::mutex> mtx(*this->m_mutex);

	if (this->m_filled_size + a_bytes >= this->m_size_in_bytes)
	{
		const size_t buffer_increment = ror::get_settings().get<uint32_t>("buffer_increment");
		assert(buffer_increment > 0 && "Setting doesn't contain valid buffer_increment");
		this->m_data.resize(this->m_data.size() + buffer_increment);
		this->m_size_in_bytes = ror::static_cast_safe<ptrdiff_t>(this->m_data.size());
	}

	auto offset = this->m_filled_size;

	this->m_filled_size += a_bytes;
	this->m_filled_size = static_cast<ptrdiff_t>(ror::align8(static_cast<uint64_t>(this->m_filled_size)));        // Aligning next offset for the most common denominator, other types are fine

	return offset;
}

template <typename _type>
ptrdiff_t Buffer<_type>::offset(ptrdiff_t a_bytes)
{
	// Do the allocation
	return this->_offset(a_bytes);
}

// Request is only defined for Static buffer, we can't return a pointer of a dynamic buffer
template <typename _type>
template <typename U, std::enable_if_t<std::is_same<U, Static>::value, bool>>
uint8_t *Buffer<_type>::request(ptrdiff_t a_bytes)
{
	// This is only defined for Static buffers because this->m_data.data() might be invalid when _offset returns with a different size
	return this->m_data.data() + this->_offset(a_bytes);
}

// Assumes data space is already allocated in this buffer so do request()/offset() and then multiple uploads() or request_upload() for bulk allocation and upload
template <typename _type>
void Buffer<_type>::upload(const uint8_t *a_data, size_t a_size, ptrdiff_t a_offset)
{
	assert(a_size <= std::numeric_limits<ptrdiff_t>::max() && "Buffer data too big for diff calculations");

	std::copy(a_data, a_data + a_size, this->m_data.data() + a_offset);
}

template <typename _type>
void Buffer<_type>::upload(const std::vector<uint8_t> &a_data, ptrdiff_t a_offset)
{
	// Do the allocation
	assert(a_data.size() <= std::numeric_limits<ptrdiff_t>::max() && "Buffer data too big for diff calculations");

	std::copy(a_data.begin(), a_data.end(), this->m_data.begin() + a_offset);
}

template <typename _type>
uint32_t Buffer<_type>::handle() noexcept
{
	return 0;
}

template <typename _type>
void Buffer<_type>::map() noexcept
{}

template <typename _type>
void Buffer<_type>::unmap() noexcept
{}

template <typename _type>
void Buffer<_type>::size(ptrdiff_t a_size) noexcept
{
	assert(a_size > 0 && "Buffer size must be positive");
	assert(this->m_size_in_bytes == 0 && "Buffer<_type>::resize is called again, this needs synchronising if this is happening for good reason");

	// Probably better to make this lazy allocated, perhaps on first _offset() call
	this->m_data.resize(static_cast<size_t>(a_size));
	this->m_size_in_bytes = a_size;
}

template <typename _type>
ptrdiff_t Buffer<_type>::size() const noexcept
{
	return this->m_size_in_bytes;
}

template <typename _type>
void Buffer<_type>::interleaved(bool a_interleaved) noexcept
{
	this->m_interleaved_local = a_interleaved;
}

template <typename _type>
bool Buffer<_type>::interleaved() const noexcept
{
	return this->m_interleaved_local;
}

template <typename _type>
void Buffer<_type>::emplace_semantic(std::pair<rhi::BufferSemantic, uint64_t> &&a_pair)
{
	this->m_semantics.emplace_back(std::move(a_pair));
}

template <typename _type>
std::pair<rhi::BufferSemantic, uint64_t> Buffer<_type>::semantic(size_t a_index) const noexcept
{
	assert(a_index < this->m_semantics.size());

	return this->m_semantics[a_index];
}

template <typename _type>
const std::vector<std::pair<rhi::BufferSemantic, uint64_t>> &Buffer<_type>::semantics() const noexcept
{
	return this->m_semantics;
}

template <typename _type>
void Buffer<_type>::_upload()        // TODO: To be implemented in renderer or via CRTP
{
	ror::log_critical("Implement me _upload");
}

template <typename _type>
void Buffer<_type>::_partial_upload(ptrdiff_t a_offset, ptrdiff_t a_length)        // TODO: To be implemented in renderer or via CRTP
{
	(void) a_offset;
	(void) a_length;

	ror::log_critical("Implement me _partial_upload");
}

template <typename _type>
void Buffer<_type>::gpu_upload() noexcept
{
	this->_upload();
}

template <typename _type>
const std::vector<uint8_t> &Buffer<_type>::data() const
{
	return this->m_data;
}

}        // namespace rhi
