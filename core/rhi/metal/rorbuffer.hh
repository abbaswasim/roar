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
#include <Metal/MTLRenderCommandEncoder.hpp>

namespace rhi
{
template <typename _type>
void BufferMetal<_type>::_temp_virtual()
{}

template <typename _type>
void BufferMetal<_type>::release()
{
	if (this->m_buffer)
		this->m_buffer->release;
}

template <typename _type>
void BufferMetal<_type>::upload(rhi::Device &a_device)
{
	/*
	For buffers in the device address space, align the offset to the data type consumed by the vertex function (which is always less than or equal to 16 bytes).
	For buffers in the constant address space, align the offset to 256 bytes in macOS. In iOS, align the offset to the maximum of either the data type consumed by the vertex function, or 4 bytes. A 16-byte alignment is safe in iOS if you don't need to consider the data type.
	 */

	// Create the buffer once. but can be uploaded many times with updates
	if (this->m_buffer == nullptr)
		this->init(a_device, static_cast<size_t>(this->filled_size()));

	this->partial_upload(0, static_cast<size_t>(this->filled_size()));

	this->ready(true);
}

template <typename _type>
void BufferMetal<_type>::reupload()
{
	assert(this->m_buffer && "Update called on null buffer");
	this->partial_upload(0, static_cast<size_t>(this->filled_size()));
	this->ready(true);
}

template <typename _type>
void BufferMetal<_type>::partial_upload(size_t a_offset, size_t a_length)
{
	std::memcpy(this->m_buffer->contents(), this->data().data() + a_offset, a_length);

	// Only need those for ResourceStorageModeManaged resources
	this->m_buffer->didModifyRange(NS::Range::Make(a_offset, this->m_buffer->length()));
}

template <typename _type>
FORCE_INLINE void BufferMetal<_type>::init(rhi::Device &a_device, size_t a_size_in_bytes)
{
	if (a_size_in_bytes == 0)
		ror::log_warn("Create a buffer of size {}", a_size_in_bytes);

	MTL::Device *device = a_device.platform_device();
	this->m_buffer      = device->newBuffer(std::max(1ul, a_size_in_bytes), MTL::ResourceStorageModeManaged);        // TODO: Add other modes
}

template <typename _type>
FORCE_INLINE constexpr auto BufferMetal<_type>::map() noexcept
{
	return static_cast<uint8_t *>(this->platform_buffer()->contents());
}

template <typename _type>
FORCE_INLINE constexpr void BufferMetal<_type>::unmap() noexcept
{
	this->m_buffer->didModifyRange(NS::Range::Make(0, this->m_buffer->length()));        // Remember this is only valid for Managed mode, shared doesn't require this
}

template <typename _type>
FORCE_INLINE constexpr void BufferMetal<_type>::unmap_partial(std::uintptr_t a_from, std::uintptr_t a_to) noexcept
{
	this->m_buffer->didModifyRange(NS::Range::Make(a_from, a_to));        // Remember this is only valid for Managed mode, shared doesn't require this
}

template <typename _type>
FORCE_INLINE constexpr void BufferMetal<_type>::bind(MTL::RenderCommandEncoder *a_cmd_encoder, rhi::ShaderType a_shader_stage, uint32_t a_index, uint32_t a_offset) const noexcept
{
	// auto &shader_buffer = this->shader_buffer();
	switch (a_shader_stage)
	{
		case rhi::ShaderType::vertex:
			a_cmd_encoder->setVertexBuffer(this->m_buffer, a_offset, a_index);
			break;
		case rhi::ShaderType::fragment:
			a_cmd_encoder->setFragmentBuffer(this->m_buffer, a_offset, a_index);
			break;
		case rhi::ShaderType::compute:
			// a_cmd_encoder->setBuffer(this->m_buffer, a_offset, a_index);
			// break;
		case rhi::ShaderType::none:
		case rhi::ShaderType::mesh:
		case rhi::ShaderType::task:
		case rhi::ShaderType::tile:
		case rhi::ShaderType::ray_miss:
		case rhi::ShaderType::ray_any_hit:
		case rhi::ShaderType::ray_closest_hit:
		case rhi::ShaderType::ray_intersection:
		case rhi::ShaderType::ray_generation:
			ror::log_critical("Binding buffer to unsupported shader stage");
			break;
	}
}
}        // namespace rhi
