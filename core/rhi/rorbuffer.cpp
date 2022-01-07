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

#include "rhi/rorbuffer.hpp"
#include <cassert>

namespace rhi
{
Buffer::Buffer()
{
	this->m_semantics.reserve(32);        // About enough for now, ror::ShaderSemantic is about 28 entries, if it grows in the future it doens't matter much
}

uint64_t Buffer::_offset(uint64_t a_bytes)
{
	// TODO: Make me thread safe once requested by multiple threads
	assert(this->m_filled_size + a_bytes < this->m_size_in_bytes && "Requesting more bytes than the buffer has available");

	auto offset{this->m_filled_size};

	this->m_filled_size += a_bytes;

	return offset;
}

uint64_t Buffer::offset(uint64_t a_bytes)
{
	return this->_offset(a_bytes);
}

uint8_t *Buffer::request(uint64_t a_bytes)
{
	return this->m_data.data() + this->_offset(a_bytes);
}

uint64_t Buffer::upload(uint8_t &a_data, uint64_t a_length)
{
	auto offset{this->offset(a_length)};

	assert(0);

	// TODO: Thats how you do copy
	// TODO: Need to upload data here. Into the buffer
	// std::copy(a_data, a_data + a_length, this->m_data.begin() + offset);

	this->_upload(a_data, a_length);

	return offset;
}

uint32_t Buffer::handle() noexcept
{
	return 0;
}

void Buffer::map() noexcept
{}

void Buffer::unmap() noexcept
{}

void Buffer::size(uint64_t a_size) noexcept
{
	// this->m_data.reserve(a_size);
	// TODO: Make this lazy allocated, perhaps on first _offset() call
	this->m_data.resize(a_size);
	this->m_size_in_bytes = a_size;
}

uint64_t Buffer::size() const noexcept
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

void Buffer::_upload(uint8_t &a_data, uint64_t a_length)        // TODO: To be implemented in renderer or via CRTP
{
	(void) a_data;
	(void) a_length;
}

define_translation_unit_vtable(Buffer)
{}

}        // namespace rhi
