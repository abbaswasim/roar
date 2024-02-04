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

#include "foundation/rorhash.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "rhi/vulkan/rordescriptor_pool.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace rhi
{
struct DescriptorLayoutInfoHash
{
	std::size_t operator()(const VkDescriptorSetLayoutCreateInfo &a_info) const noexcept
	{
		hash_64_t hash{a_info.sType};
		ror::hash_combine_64(hash, reinterpret_cast<size_t>(a_info.pNext));
		ror::hash_combine_64(hash, a_info.flags);
		ror::hash_combine_64(hash, a_info.bindingCount);

		for (size_t i = 0; i < a_info.bindingCount; ++i)
		{
			auto &binding = a_info.pBindings[i];

			ror::hash_combine_64(hash, binding.binding);
			ror::hash_combine_64(hash, binding.descriptorType);
			ror::hash_combine_64(hash, binding.descriptorCount);
			ror::hash_combine_64(hash, binding.stageFlags);
			ror::hash_combine_64(hash, reinterpret_cast<size_t>(binding.pImmutableSamplers));
		}

		return static_cast<size_t>(hash);
	}
};

class DescriptorSetLayoutFactory final
{
  public:
	FORCE_INLINE                             DescriptorSetLayoutFactory()                                              = default;        //! Copy constructor
	FORCE_INLINE                             DescriptorSetLayoutFactory(const DescriptorSetLayoutFactory &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                             DescriptorSetLayoutFactory(DescriptorSetLayoutFactory &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE DescriptorSetLayoutFactory &operator=(const DescriptorSetLayoutFactory &a_other)                      = delete;         //! Copy assignment operator
	FORCE_INLINE DescriptorSetLayoutFactory &operator=(DescriptorSetLayoutFactory &&a_other) noexcept                  = delete;         //! Move assignment operator
	FORCE_INLINE ~DescriptorSetLayoutFactory() noexcept                                                                = default;

	VkDescriptorSetLayout make_layout(const VkDevice &a_device, const VkDescriptorSetLayoutCreateInfo &descriptor_set_layout_createinfo);
	VkDescriptorSetLayout make_layout(const VkDevice &a_device, std::vector<VkDescriptorSetLayoutBinding> &a_bindings);

	void destroy(const VkDevice a_device);

  protected:
  private:
	using LayoutCache = std::unordered_map<VkDescriptorSetLayoutCreateInfo, VkDescriptorSetLayout, DescriptorLayoutInfoHash>;

	LayoutCache m_layouts{};        //! All the layouts known to the engine
	std::mutex  m_mutex;            //! Mutex to synchronise the factory accros threads
};

}        // namespace rhi
