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

#include "rhi/vulkan/rorcommand_pool.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include <mutex>

namespace rhi
{

auto make_pool(VkDevice a_device, uint32_t a_family_index)
{
	VkCommandPool pool = vk_create_command_pools(a_device, a_family_index, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	return pool;
}

void CommandPool::init(const VkDevice a_device, uint32_t a_family_index)
{
	std::unique_lock<std::mutex> lock{this->m_mutex};

	this->m_pool = make_pool(a_device, a_family_index);
}

void CommandPool::reset(const VkDevice a_device, uint32_t a_family_index)
{
	{
		std::unique_lock<std::mutex> lock{this->m_mutex};

		vkResetCommandPool(a_device, this->m_pool, 0);
	}

	// Also re-create the standard one
	this->init(a_device, a_family_index);
}

void CommandPool::destroy(const VkDevice a_device)
{
	std::unique_lock<std::mutex> lock{this->m_mutex};

	vk_destroy_command_pools(a_device, this->m_pool);
}

VkCommandBuffer CommandPool::allocate(const VkDevice a_device)
{
	std::unique_lock<std::mutex> lock{this->m_mutex};

	return vk_allocate_command_buffer(a_device, this->m_pool);
}

}        // namespace rhi
