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
#include "foundation/rortypes.hpp"
#include "rhi/vulkan/rordescriptor_cache.hpp"
#include "rhi/vulkan/rordescriptor_pool.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include <cassert>
#include <cstddef>
#include <mutex>
#include <vector>

namespace rhi
{
/**
 * Abstraction over Vulkan Descriptor sets. Vulkan descriptors are pointers into data that shaders can access. Mostly images and buffers.
 * Roar uses 4-descriptor sets for all the data provided to the shaders. These are split by frequency of use/update.
 * Descriptor sets need to be allocated from a pool and updated via descriptor writes. When all descriptors of a pool are no longer needed, one can free the pool.
 * If descriptor set is fully static, it can be allocated and reused each frame. But if each frame requires a different copy of the data
 * you need to double buffere both the data as well as the vulkan descriptor. Some implementations will allocate new descriptors for each frame to avoid that.
 * If a set requires both static and dynamic data, you need to double buffer the dynamic data but the static data doesn't have to be. The descriptor will also
 * needs to be double buffered.
 */
class DescriptorSet final
{
  public:
	FORCE_INLINE                DescriptorSet()                                 = default;        //! Copy constructor
	FORCE_INLINE                DescriptorSet(const DescriptorSet &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                DescriptorSet(DescriptorSet &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE DescriptorSet &operator=(const DescriptorSet &a_other)         = delete;         //! Copy assignment operator
	FORCE_INLINE DescriptorSet &operator=(DescriptorSet &&a_other) noexcept     = default;        //! Move assignment operator
	FORCE_INLINE ~DescriptorSet() noexcept                                      = default;

	void                  push_image(uint32_t a_binding, VkDescriptorImageInfo *a_image_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags = VK_SHADER_STAGE_ALL);
	void                  push_buffer(uint32_t a_binding, VkDescriptorBufferInfo *a_buffer_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags = VK_SHADER_STAGE_ALL);
	void                  push_binding(uint32_t a_binding, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags);
	void                  push_binding(uint32_t a_binding, VkDescriptorType a_type, VkDescriptorImageInfo *a_image_info, VkDescriptorBufferInfo *a_buffer_info);
	VkDescriptorSetLayout allocate(const VkDevice a_device, DescriptorSetLayoutCache &a_factory, DescriptorPool &a_pool, uint32_t a_set_id);
	void                  update(const VkDevice a_device);
	void                  update_writes();
	void                  reset_writes();

	// clang-format off
	auto &platform_descriptor() { return this->m_handle; }
	auto &bindings()            { return this->m_bindings; }
	auto &writes()              { return this->m_writes;   }
	auto  set_id()              { return this->m_set_id;   }
	// clang-format on
	// void free(const VkDevice a_device); // Not provided because bad practice, would rather free the pool directly

  protected:
  private:
	void push_binding(uint32_t a_binding, VkDescriptorImageInfo *a_image_info, VkDescriptorBufferInfo *a_buffer_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags = VK_SHADER_STAGE_ALL);

	std::vector<VkDescriptorSetLayoutBinding> m_bindings{};             //! All the bindings that makes up this descriptor
	std::vector<VkWriteDescriptorSet>         m_writes{};               //! All the writes that would need to performed when updating
	VkDescriptorSet                           m_handle{nullptr};        //! Handle to the descriptor set
	uint32_t                                  m_set_id{0};              //! Set id used in the shader for this descriptor set, by default its 0

	// VkDescriptorPool                          m_pool{nullptr};          //! Handle to the pool from where we allocated this descriptor, required if free is implemented
};

class DescriptorSetCache final
{
  public:
	FORCE_INLINE                     DescriptorSetCache()                                      = default;        //! Copy constructor
	FORCE_INLINE                     DescriptorSetCache(const DescriptorSetCache &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                     DescriptorSetCache(DescriptorSetCache &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE DescriptorSetCache &operator=(const DescriptorSetCache &a_other)              = delete;         //! Copy assignment operator
	FORCE_INLINE DescriptorSetCache &operator=(DescriptorSetCache &&a_other) noexcept          = delete;         //! Move assignment operator
	FORCE_INLINE ~DescriptorSetCache() noexcept                                                = default;

	size_t         emplace(DescriptorSet a_set);
	DescriptorSet &at(size_t a_index);

  protected:
  private:
	std::vector<DescriptorSet> m_descriptors{};        //! All the descriptors in this cache, used by app PSOs
	std::mutex                 m_mutex{};              //! Mutex to lock the cache for external synchronisation
};
}        // namespace rhi

#include "rhi/vulkan/rordescriptor_set.hh"
