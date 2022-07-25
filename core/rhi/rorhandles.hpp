// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

namespace rhi
{
/**
 * This defines a type safe handle class
 * Instead of just defining engine wide handles as uin64_t which are not type safe
 * there is a big chance of using a texture handle for a material handle etc
 * RoarHandle defines a public m_handle member variable which can be used directly
 */
#define srfy(x) #x

#if defined(ROR_BUILD_TYPE_DEBUG)
template <class _handle, class _type>
class ROAR_ENGINE_ITEM RoarHandle final
{
  public:
	FORCE_INLINE             RoarHandle()                              = default;        //! Default constructor
	FORCE_INLINE             RoarHandle(const RoarHandle &a_other)     = default;        //! Copy constructor
	FORCE_INLINE             RoarHandle(RoarHandle &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RoarHandle &operator=(const RoarHandle &a_other)      = default;        //! Copy assignment operator
	FORCE_INLINE RoarHandle &operator=(RoarHandle &&a_other) noexcept  = default;        //! Move assignment operator
	FORCE_INLINE ~RoarHandle() noexcept                                = default;        //! Destructor

	FORCE_INLINE explicit RoarHandle(_type a_handle) :
	    m_handle(a_handle)
	{}

	bool operator==(const RoarHandle &a_other) const
	{
		return this->m_handle == a_other.m_handle;
	}

	bool operator==(const _type &a_other) const
	{
		return this->m_handle == a_other;
	}

	bool operator!=(const RoarHandle &a_other) const
	{
		return this->m_handle != a_other.m_handle;
	}

	bool operator!=(const _type a_other) const
	{
		return this->m_handle != a_other;
	}

	constexpr operator _type() const
	{
		return this->m_handle;
	}

  protected:
  private:
	_type m_handle;
};

#	define create_handle(handle, _type)                                                             \
		struct type_##handle                                                                         \
		{};                                                                                          \
		using handle = rhi::RoarHandle<type_##handle, _type>;                                        \
		static_assert(std::is_trivially_copyable_v<handle>, srfy(handle is not trivially copyable)); \
		static_assert(std::is_standard_layout_v<handle>, srfy(handle is not standard layout))
#else
#	define create_handle(handle, _type) \
		using handle = _type;
#endif

create_handle(TextureImageHandle, int32_t);
create_handle(TextureSamplerHandle, uint32_t);
create_handle(TextureHandle, int32_t);
create_handle(TextureAPIHandle, int32_t);

}        // namespace rhi
