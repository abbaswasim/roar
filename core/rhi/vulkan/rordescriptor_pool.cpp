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

#include "rhi/vulkan/rordescriptor_pool.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include "settings/rorsettings.hpp"

namespace rhi
{

const size_t pools_types_count{11};

FORCE_INLINE constexpr auto pool_index(uint32_t a_index)
{
	// clang-format off
	switch (a_index)
	{
		case 0: return VK_DESCRIPTOR_TYPE_SAMPLER;
		case 1: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		case 2: return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		case 3: return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		case 4: return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
		case 5: return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
		case 6: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		case 7: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		case 8: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		case 9: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
		case 10:return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	}
	// clang-format on

	assert(0 && "Unknown descriptor implement me");

	return VK_DESCRIPTOR_TYPE_SAMPLER;
}

auto make_pool(VkDevice a_device)
{
	auto pool_size = ror::settings().m_vulkan.m_descriptor_pool_size;

	std::vector<VkDescriptorPoolSize> sizes{};
	sizes.reserve(pools_types_count);

	VkDescriptorPoolSize size{};
	size.descriptorCount = pool_size;

	for (uint32_t i = 0; i < pools_types_count; ++i)
	{
		size.type = pool_index(i);
		sizes.push_back(size);
	}

	VkDescriptorPoolCreateInfo descriptor_pool_create_info = {};

	descriptor_pool_create_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptor_pool_create_info.flags         = 0;
	descriptor_pool_create_info.maxSets       = pool_size;        // Maximum number of descriptors that can be allocated from this pool, in my case same as the indiviual size
	descriptor_pool_create_info.poolSizeCount = static_cast<uint32_t>(sizes.size());
	descriptor_pool_create_info.pPoolSizes    = sizes.data();

	VkDescriptorPool pool;

	vkCreateDescriptorPool(a_device, &descriptor_pool_create_info, cfg::VkAllocator, &pool);

	return pool;
}

void DescriptorPool::init(const VkDevice a_device)
{
	this->m_pools.emplace_back(make_pool(a_device));
}

void DescriptorPool::reset(const VkDevice a_device)
{
	for (auto &pool : this->m_pools)
		vkResetDescriptorPool(a_device, pool, 0);

	// Also re-create them standard ones
	this->init(a_device);
}

void DescriptorPool::destroy(const VkDevice a_device)
{
	for (auto &pool : this->m_pools)
		vkDestroyDescriptorPool(a_device, pool, cfg::VkAllocator);
}

VkDescriptorSet DescriptorPool::allocate(const VkDevice a_device, VkDescriptorSetLayout a_layout)
{
	auto pool = this->m_pools.back();

	VkResult        result;
	VkDescriptorSet descriptor_set = vk_allocate_descriptor_set(a_device, a_layout, pool, 1u, &result);

	if (result != VK_SUCCESS && descriptor_set == nullptr)
	{
		if (result == VK_ERROR_FRAGMENTED_POOL || result == VK_ERROR_OUT_OF_POOL_MEMORY)
		{
			this->init(a_device);

			pool           = this->m_pools.back();
			descriptor_set = vk_allocate_descriptor_set(a_device, a_layout, pool, 1u, &result);
			check_return_status(result, "vkAllocateDescriptorSet");
		}
		else
		{
			check_return_status(result, "vkAllocateDescriptorSet");
		}
	}

	return descriptor_set;
}

}        // namespace rhi
