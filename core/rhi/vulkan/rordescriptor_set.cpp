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
#include "rhi/vulkan/rordescriptor_set.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include "settings/rorsettings.hpp"
#include <mutex>

bool operator==(const VkDescriptorSetLayoutBinding &a_left, const VkDescriptorSetLayoutBinding &a_right)
{
	if (a_left.binding == a_right.binding &&
	    a_left.descriptorType == a_right.descriptorType &&
	    a_left.descriptorCount == a_right.descriptorCount &&
	    a_left.stageFlags == a_right.stageFlags &&
	    a_left.pImmutableSamplers == a_right.pImmutableSamplers)        // TODO: Not sure if this is right. confirmm
		return true;

	return false;
}

bool operator!=(const VkDescriptorSetLayoutBinding &a_left, const VkDescriptorSetLayoutBinding &a_right)
{
	return !(a_left == a_right);
}

bool operator==(const VkDescriptorSetLayoutCreateInfo &a_left, const VkDescriptorSetLayoutCreateInfo &a_right)
{
	if (a_left.bindingCount == a_right.bindingCount &&
	    a_left.sType == a_right.sType &&        // Maybe should be ignored, since they will always be the same
	    a_left.pNext == a_right.pNext &&        // Maybe should be ignored, since they will always be the same
	    a_left.flags == a_right.flags)          // Maybe should be ignored, since they will always be the same
	{
		for (size_t i = 0; i < a_left.bindingCount; ++i)
		{
			auto &left_binding  = a_left.pBindings[i];
			auto &right_binding = a_right.pBindings[i];

			if (left_binding != right_binding)
				return false;
		}

		return true;
	}

	return false;
}

namespace rhi
{

VkDescriptorSetLayout DescriptorSetLayoutFactory::make_layout(const VkDevice &a_device, const VkDescriptorSetLayoutCreateInfo &a_descriptor_set_layout_createinfo)
{
	std::lock_guard<std::mutex> lock{this->m_mutex};

	auto iter = this->m_layouts.find(a_descriptor_set_layout_createinfo);
	if (iter != this->m_layouts.end())
	{
		return iter->second;
	}
	else
	{
		VkDescriptorSetLayout layout = vk_create_descriptor_set_layout(a_device, a_descriptor_set_layout_createinfo);

		this->m_layouts[a_descriptor_set_layout_createinfo] = layout;

		return layout;
	}
}

void DescriptorSetLayoutFactory::destroy(const VkDevice a_device)
{
	for (auto &layout : this->m_layouts)
		vkDestroyDescriptorSetLayout(a_device, layout.second, cfg::VkAllocator);
}

VkDescriptorSetLayout DescriptorSetLayoutFactory::make_layout(const VkDevice &a_device, std::vector<VkDescriptorSetLayoutBinding> &a_bindings)
{
	VkDescriptorSetLayoutCreateInfo descriptor_set_layout_createinfo = vk_create_descriptor_set_layout_info(a_bindings);

	return make_layout(a_device, descriptor_set_layout_createinfo);
}

void DescriptorSet::push_binding(uint32_t a_binding, VkDescriptorImageInfo *a_image_info, VkDescriptorBufferInfo *a_buffer_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags)
{
	VkDescriptorSetLayoutBinding descriptor_set_layout_binding = vk_create_descriptor_set_layout_binding(a_binding, a_type, a_stage_flags);
	VkWriteDescriptorSet         write_descriptor_set          = vk_create_write_descriptor_set(nullptr, a_binding, a_type, a_image_info, a_buffer_info);

	this->m_bindings.push_back(descriptor_set_layout_binding);
	this->m_writes.push_back(write_descriptor_set);
}

void DescriptorSet::push_image(uint32_t a_binding, VkDescriptorImageInfo *a_image_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags)
{
	this->push_binding(a_binding, a_image_info, nullptr, a_type, a_stage_flags);
}

void DescriptorSet::push_buffer(uint32_t a_binding, VkDescriptorBufferInfo *a_buffer_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags)
{
	this->push_binding(a_binding, nullptr, a_buffer_info, a_type, a_stage_flags);
}

void DescriptorSet::allocate(const VkDevice a_device, DescriptorSetLayoutFactory &a_factory, DescriptorPool &a_pool)
{
	auto descriptor_layout = a_factory.make_layout(a_device, this->m_bindings);
	this->m_handle         = a_pool.allocate(a_device, descriptor_layout);

	this->update(a_device);
}

void DescriptorSet::update(const VkDevice a_device)
{
	for (VkWriteDescriptorSet &write : this->m_writes)
		write.dstSet = this->m_handle;

	vkUpdateDescriptorSets(a_device, static_cast<uint32_t>(this->m_writes.size()), this->m_writes.data(), 0, nullptr);
}

// void DescriptorSet::free(const VkDevice a_device)
// {
// 	vkFreeDescriptorSets(a_device, this->m_pool, 1, &this->m_handle);
// }

}        // namespace rhi
