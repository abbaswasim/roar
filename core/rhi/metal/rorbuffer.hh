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
#include "rhi/metal/rorbuffer.hpp"
#include "rhi/metal/rordevice.hpp"

namespace rhi
{
template <typename _type>
void BufferMetal<_type>::_temp_virtual()
{}

template <typename _type>
void BufferMetal<_type>::upload(rhi::Device &a_device)
{
	/*
	For buffers in the device address space, align the offset to the data type consumed by the vertex function (which is always less than or equal to 16 bytes).
	For buffers in the constant address space, align the offset to 256 bytes in macOS. In iOS, align the offset to the maximum of either the data type consumed by the vertex function, or 4 bytes. A 16-byte alignment is safe in iOS if you don't need to consider the data type.
	 */

	// ror::log_critical("Uploading buffer to metal of size {}", this->filled_size());

	this->partial_upload(a_device, 0, static_cast<size_t>(this->filled_size()));
}

template <typename _type>
void BufferMetal<_type>::partial_upload(rhi::Device &a_device, size_t a_offset, size_t a_length)
{
	MTL::Device *device = a_device.platform_device();

	// Create the buffer once. but can be uploaded many times with updates
	if (this->m_buffer == nullptr)
		this->m_buffer      = device->newBuffer(std::max(1ul, a_length), MTL::ResourceStorageModeManaged);        // TODO: Add other modes

	memcpy(this->m_buffer->contents(), this->data().data() + a_offset, a_length);

	// Only need those for ResourceStorageModeManaged resources
	this->m_buffer->didModifyRange(NS::Range::Make(0, this->m_buffer->length()));
}
}        // namespace rhi
