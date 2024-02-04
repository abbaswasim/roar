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
#include "rhi/vulkan/rordescriptor_factory.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include <array>
#include <vector>

namespace rhi
{

class DescriptorPool final
{
  public:
	FORCE_INLINE                 DescriptorPool()                                  = default;        //! Copy constructor
	FORCE_INLINE                 DescriptorPool(const DescriptorPool &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                 DescriptorPool(DescriptorPool &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE DescriptorPool &operator=(const DescriptorPool &a_other)          = delete;         //! Copy assignment operator
	FORCE_INLINE DescriptorPool &operator=(DescriptorPool &&a_other) noexcept      = delete;         //! Move assignment operator
	FORCE_INLINE ~DescriptorPool() noexcept                                        = default;

	void            init(const VkDevice a_device);
	void            reset(const VkDevice a_device);
	VkDescriptorSet allocate(const VkDevice a_device, VkDescriptorSetLayout a_layout);
	void            destroy(const VkDevice a_device);

	// clang-format off
	// FORCE_INLINE constexpr auto pool(VkDescriptorSetLayoutCreateInfo a_info)  { return this->m_pools[a_info].back(); }

	FORCE_INLINE constexpr auto sampler_pool()                  const noexcept { return this->m_pools.back(); }
	FORCE_INLINE constexpr auto combined_image_sampler_pool()   const noexcept { return this->m_pools.back(); }
	FORCE_INLINE constexpr auto sampled_image_pool()            const noexcept { return this->m_pools.back(); }
	FORCE_INLINE constexpr auto storage_image_pool()            const noexcept { return this->m_pools.back(); }
	FORCE_INLINE constexpr auto uniform_texel_buffer_pool()     const noexcept { return this->m_pools.back(); }
	FORCE_INLINE constexpr auto storage_texel_buffer_pool()     const noexcept { return this->m_pools.back(); }
	FORCE_INLINE constexpr auto uniform_buffer_pool()           const noexcept { return this->m_pools.back(); }
	FORCE_INLINE constexpr auto storage_buffer_pool()           const noexcept { return this->m_pools.back(); }
	FORCE_INLINE constexpr auto uniform_buffer_dynamic_pool()   const noexcept { return this->m_pools.back(); }
	FORCE_INLINE constexpr auto storage_buffer_dynamic_pool()   const noexcept { return this->m_pools.back(); }
	FORCE_INLINE constexpr auto input_attachment_pool()         const noexcept { return this->m_pools.back(); }
	// clang-format on

  protected:
  private:
	// TODO: Implement me: Ultimate goal is to create a pool per Layout Info this way we know each pools will not have any free space
	// Any other method I have expolored you risk wasting descritor space
	// You can also work out how many pools were created per layout info type and save that number to disk and load at next load
	// using PoolsCache = std::unordered_map<VkDescriptorSetLayoutCreateInfo, std::vector<VkDescriptorPool>, DescriptorLayoutInfoHash>;
	// PoolsCache m_pools{};        //! Cache of pools by descriptor layout

	std::vector<VkDescriptorPool> m_pools{};        //! Array of all types of pools
	std::mutex                    m_mutex{};        //! Mutex to synchronise the pools
};

}        // namespace rhi

#include "rhi/vulkan/rordescriptor_pool.hh"
