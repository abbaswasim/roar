// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2024
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

class CommandPool final
{
  public:
	FORCE_INLINE              CommandPool()                               = default;        //! Copy constructor
	FORCE_INLINE              CommandPool(const CommandPool &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE              CommandPool(CommandPool &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE CommandPool &operator=(const CommandPool &a_other)       = delete;         //! Copy assignment operator
	FORCE_INLINE CommandPool &operator=(CommandPool &&a_other) noexcept   = delete;         //! Move assignment operator
	FORCE_INLINE ~CommandPool() noexcept                                  = default;

	void            init(const VkDevice a_device, uint32_t a_family_index);
	void            reset(const VkDevice a_device, uint32_t a_family_index);
	VkCommandBuffer allocate(const VkDevice a_device);
	void            destroy(const VkDevice a_device);

  protected:
  private:
	VkCommandPool m_pool{};         //! Array of all types of pools
	std::mutex    m_mutex{};        //! Mutex to synchronise the pools
};

}        // namespace rhi

#include "rhi/vulkan/rorcommand_pool.hh"
