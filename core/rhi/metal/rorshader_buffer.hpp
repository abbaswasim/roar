// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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

#include "foundation/rormacros.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rorshader_buffer.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/rortypes.hpp"

#include <Metal/MTLBuffer.hpp>

namespace rhi
{
class ShaderBufferMetal : public ShaderBufferCrtp<ShaderBufferMetal>
{
  public:
	FORCE_INLINE                    ShaderBufferMetal()                                     = default;        //! Default constructor
	FORCE_INLINE                    ShaderBufferMetal(const ShaderBufferMetal &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                    ShaderBufferMetal(ShaderBufferMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShaderBufferMetal &operator=(const ShaderBufferMetal &a_other)             = delete;         //! Copy assignment operator
	FORCE_INLINE ShaderBufferMetal &operator=(ShaderBufferMetal &&a_other) noexcept         = default;        //! Move assignment operator
	FORCE_INLINE virtual ~ShaderBufferMetal() noexcept override                             = default;        //! Destructor

	declare_translation_unit_vtable();

	// clang-format off
	FORCE_INLINE auto platform_buffer() { return this->m_platform_buffer; }
	// clang-format on

	FORCE_INLINE void init(rhi::Device &a_device, uint32_t a_size_in_bytes)
	{
		assert(a_size_in_bytes && "Can't create a shader buffer with size 0");
		MTL::Device *device     = a_device.platform_device();
		this->m_platform_buffer = device->newBuffer(a_size_in_bytes, MTL::ResourceStorageModeManaged);
	}

	FORCE_INLINE constexpr uint8_t *map() noexcept
	{
		return static_cast<uint8_t *>(this->m_platform_buffer->contents());
	}

	FORCE_INLINE constexpr void unmap() noexcept
	{
		this->m_platform_buffer->didModifyRange(NS::Range::Make(0, this->m_platform_buffer->length()));        // Remember this is only valid for Managed mode, shared doesn't require this
	}

	FORCE_INLINE constexpr void unmap_partial(std::uintptr_t a_from, std::uintptr_t a_to) noexcept
	{
		this->m_platform_buffer->didModifyRange(NS::Range::Make(a_from, a_to));        // Remember this is only valid for Managed mode, shared doesn't require this
	}

	template <typename _type>
	FORCE_INLINE constexpr void bind(_type *a_cmd_encoder, rhi::ShaderType a_shader_stage, uint32_t a_index, uint32_t a_offset = 0) const noexcept
	{
		// auto &shader_buffer = this->shader_buffer();
		switch (a_shader_stage)
		{
			case rhi::ShaderType::vertex:
				a_cmd_encoder->setVertexBuffer(this->m_platform_buffer, a_offset, a_index);
				break;
			case rhi::ShaderType::fragment:
				a_cmd_encoder->setFragmentBuffer(this->m_platform_buffer, a_offset, a_index);
				break;
			case rhi::ShaderType::none:
			case rhi::ShaderType::mesh:
			case rhi::ShaderType::task:
			case rhi::ShaderType::tile:
			case rhi::ShaderType::compute:
			case rhi::ShaderType::ray_miss:
			case rhi::ShaderType::ray_any_hit:
			case rhi::ShaderType::ray_closest_hit:
			case rhi::ShaderType::ray_intersection:
			case rhi::ShaderType::ray_generation:
				ror::log_critical("Binding buffer to unsupported shader stage");
				break;
		}
	}

  protected:
  private:
	MTL::Buffer *m_platform_buffer{nullptr};
};

declare_rhi_render_type(ShaderBuffer);

}        // namespace rhi

#include "rhi/metal/rorshader_buffer.hh"
