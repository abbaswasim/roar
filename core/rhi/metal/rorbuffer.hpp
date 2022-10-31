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

#include "foundation/rormacros.hpp"
#include "rhi/crtp_interfaces/rorbuffer.hpp"
#include "rhi/rorrhi_macros.hpp"
#include <Metal/MTLBuffer.hpp>

namespace rhi
{
class DeviceMetal;
using Device = DeviceMetal;

template <typename _type = Static>
class ROAR_ENGINE_ITEM BufferMetal : public BufferCrtp<BufferMetal<_type>, _type>
{
  public:
	FORCE_INLINE              BufferMetal()                               = default;        //! Default constructor
	FORCE_INLINE              BufferMetal(const BufferMetal &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              BufferMetal(BufferMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE BufferMetal &operator=(const BufferMetal &a_other)       = default;        //! Copy assignment operator
	FORCE_INLINE BufferMetal &operator=(BufferMetal &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE virtual ~BufferMetal() noexcept override                 = default;        //! Destructor

	void release();
	void upload(rhi::Device &a_device);
	void partial_upload(rhi::Device &a_device, size_t a_offset, size_t a_length);

	// clang-format off
	FORCE_INLINE constexpr auto* platform_buffer()  noexcept { return this->m_buffer; }
	// clang-format on

  protected:
  private:
	declare_translation_unit_vtable();

	MTL::Buffer *m_buffer{nullptr};        //! API handle to buffer
};

// Template deduction guide CATD for Buffer static
template <class _type>
BufferMetal(_type) -> BufferMetal<Static>;

// I am doing this so the rest of the system can use the CRTP implementation directly as "Buffer"
template <typename _type>
using Buffer = BufferMetal<_type>;

}        // namespace rhi

#include "rhi/metal/rorbuffer.hh"
