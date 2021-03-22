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
	this->m_semantics.reserve(20);        // About enough for now, ror::ShaderSemantic is about 18 entries, if it grows in the future it doens't matter much
}

uint64_t Buffer::request(uint64_t a_bytes)
{
	assert(this->m_filled_size + a_bytes < this->m_size_in_bytes && "Requesting more bytes than the buffer has available");

	uint64_t offset{this->m_filled_size};

	this->m_filled_size += a_bytes;

	return offset;
}

uint64_t Buffer::upload(uint8_t &a_data, uint64_t a_length)
{
	uint64_t offset{this->request(a_length)};

	// TODO: Thats how you do copy
	// std::copy(data, data + size, mapped_data + offset);

	_upload(a_data, a_length);

	return offset;
}

uint32_t Buffer::handle()
{
	return 0;
}

void Buffer::map()
{}

void Buffer::unmap()
{}

void Buffer::size(uint64_t a_size)
{
	this->m_size_in_bytes = a_size;
}

uint64_t Buffer::size() const noexcept
{
	return this->m_size_in_bytes;
}

void Buffer::interleaved(bool a_interleaved)
{
	this->m_interleaved_local = a_interleaved;
}

bool Buffer::interleaved() const noexcept
{
	return this->m_interleaved_local;
}

void Buffer::emplace_semantic(std::pair<ShaderSemantic, uint64_t> &&a_pair)
{
	this->m_semantics.emplace_back(std::move(a_pair));
}

std::pair<ShaderSemantic, uint64_t> Buffer::semantic(size_t a_index) const noexcept
{
	return this->m_semantics[a_index];        // Not doing boundary checks delibrately
}

const std::vector<std::pair<ShaderSemantic, uint64_t>> &Buffer::semantics() const noexcept
{
	return this->m_semantics;
}
void Buffer::_upload(uint8_t &a_data, uint64_t a_length)        // TODO: To be implemented in renderer or via CRTP
{
	(void) a_data;
	(void) a_length;
}

define_translation_unit_vtable(Buffer){}

}        // namespace rhi
