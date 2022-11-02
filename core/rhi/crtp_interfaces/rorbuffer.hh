// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2022
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
#include "rhi/crtp_interfaces/rorbuffer.hpp"

namespace rhi
{
template <typename _type, typename _derived>
BufferCrtp<_type, _derived>::BufferCrtp()
{
	this->m_semantics.reserve(32);        // About enough for now, ror::ShaderSemantic is about 28 entries, if it grows in the future it doens't matter much
	this->m_mutex = std::make_shared<std::mutex>();
}

template <typename _type, typename _derived>
template <typename U, std::enable_if_t<std::is_same<U, Static>::value, bool>>
ptrdiff_t BufferCrtp<_type, _derived>::_offset(ptrdiff_t a_bytes)
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

template <typename _type, typename _derived>
template <typename U, std::enable_if_t<std::is_same<U, Dynamic>::value, bool>>
ptrdiff_t BufferCrtp<_type, _derived>::_offset(ptrdiff_t a_bytes)
{
	assert(a_bytes > 0 && "Requested bytes must be positive");

	std::lock_guard<std::mutex> mtx(*this->m_mutex);

	if (this->m_filled_size + a_bytes >= this->m_size_in_bytes)
	{
		const size_t buffer_increment = ror::settings().m_buffer_increment;
		assert(buffer_increment > 0 && "Setting doesn't contain valid buffer_increment");
		this->m_data.resize(this->m_data.size() + buffer_increment);
		this->m_size_in_bytes = ror::static_cast_safe<ptrdiff_t>(this->m_data.size());
	}

	auto offset = this->m_filled_size;

	this->m_filled_size += a_bytes;
	this->m_filled_size = static_cast<ptrdiff_t>(ror::align8(static_cast<uint64_t>(this->m_filled_size)));        // Aligning next offset for the most common denominator, other types are fine

	return offset;
}

template <typename _type, typename _derived>
ptrdiff_t BufferCrtp<_type, _derived>::offset(ptrdiff_t a_bytes)
{
	// Do the allocation
	return this->_offset(a_bytes);
}

// Request is only defined for Static buffer, we can't return a pointer of a dynamic buffer
template <typename _type, typename _derived>
template <typename U, std::enable_if_t<std::is_same<U, Static>::value, bool>>
uint8_t *BufferCrtp<_type, _derived>::request(ptrdiff_t a_bytes)
{
	// This is only defined for Static buffers because this->m_data.data() might be invalid when _offset returns with a different size
	return this->m_data.data() + this->_offset(a_bytes);
}

// Assumes data space is already allocated in this buffer so do request()/offset() and then multiple uploads() or request_upload() for bulk allocation and upload
template <typename _type, typename _derived>
void BufferCrtp<_type, _derived>::copy(const uint8_t *a_data, size_t a_size, ptrdiff_t a_offset)
{
	assert(a_size <= std::numeric_limits<size_t>::max() && "Buffer data too big for diff calculations");

	std::copy(a_data, a_data + a_size, this->m_data.data() + a_offset);
}

template <typename _type, typename _derived>
void BufferCrtp<_type, _derived>::copy(const std::vector<uint8_t> &a_data, ptrdiff_t a_offset)
{
	// Do the allocation
	assert(a_data.size() <= std::numeric_limits<size_t>::max() && "Buffer data too big for diff calculations");

	std::copy(a_data.begin(), a_data.end(), this->m_data.begin() + a_offset);
}

template <typename _type, typename _derived>
uint32_t BufferCrtp<_type, _derived>::handle() noexcept
{
	return 0;
}

template <typename _type, typename _derived>
void BufferCrtp<_type, _derived>::size(ptrdiff_t a_size) noexcept
{
	assert(a_size > 0 && "Buffer size must be positive");
	assert(this->m_size_in_bytes == 0 && "BufferCrtp<_type, _derived>::resize is called again, this needs synchronising if this is happening for good reason");

	// Probably better to make this lazy allocated, perhaps on first _offset() call
	this->m_data.resize(static_cast<size_t>(a_size));
	this->m_size_in_bytes = a_size;
}

template <typename _type, typename _derived>
ptrdiff_t BufferCrtp<_type, _derived>::size() const noexcept
{
	return this->m_size_in_bytes;
}

template <typename _type, typename _derived>
ptrdiff_t BufferCrtp<_type, _derived>::filled_size() const noexcept
{
	return this->m_filled_size;
}

template <typename _type, typename _derived>
void BufferCrtp<_type, _derived>::interleaved(bool a_interleaved) noexcept
{
	this->m_interleaved = a_interleaved;
}

template <typename _type, typename _derived>
bool BufferCrtp<_type, _derived>::interleaved() const noexcept
{
	return this->m_interleaved;
}

template <typename _type, typename _derived>
void BufferCrtp<_type, _derived>::ready(bool a_ready) noexcept
{
	this->m_ready = a_ready;
}

template <typename _type, typename _derived>
bool BufferCrtp<_type, _derived>::ready() const noexcept
{
	return this->m_ready;
}

template <typename _type, typename _derived>
void BufferCrtp<_type, _derived>::emplace_semantic(std::pair<rhi::BufferSemantic, uint64_t> &&a_pair)
{
	this->m_semantics.emplace_back(std::move(a_pair));
}

template <typename _type, typename _derived>
std::pair<rhi::BufferSemantic, uint64_t> BufferCrtp<_type, _derived>::semantic(size_t a_index) const noexcept
{
	assert(a_index < this->m_semantics.size());

	return this->m_semantics[a_index];
}

template <typename _type, typename _derived>
const std::vector<std::pair<rhi::BufferSemantic, uint64_t>> &BufferCrtp<_type, _derived>::semantics() const noexcept
{
	return this->m_semantics;
}

template <typename _type, typename _derived>
const std::vector<uint8_t> &BufferCrtp<_type, _derived>::data() const
{
	return this->m_data;
}

}        // namespace rhi
