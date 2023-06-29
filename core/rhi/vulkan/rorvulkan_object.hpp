// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
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
#include "rhi/vulkan/rorvulkan_common.hpp"

namespace rhi
{

template <typename _type>
class VulkanObject
{
  public:
	FORCE_INLINE               VulkanObject()                                = default;        //! Default constructor
	FORCE_INLINE               VulkanObject(const VulkanObject &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               VulkanObject(VulkanObject &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE VulkanObject &operator=(const VulkanObject &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE VulkanObject &operator=(VulkanObject &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE virtual ~VulkanObject() noexcept                            = default;        //! Destructor

	// Will/Should be called by all derived classes to initialize m_handle, it can't be default initialized
	FORCE_INLINE VulkanObject(_type handle) :
	    m_handle(handle)
	{}

	FORCE_INLINE _type get_handle()
	{
		return m_handle;
	}

	FORCE_INLINE void set_handle(_type a_handle)
	{
		this->m_handle = a_handle;
	}

	FORCE_INLINE void reset()
	{
		this->m_handle = nullptr;
	}

  protected:        // Not using protected instead providing accessors
  private:
	_type m_handle{VK_NULL_HANDLE};        // Handle to object initilised with null
};

}        // namespace rhi
