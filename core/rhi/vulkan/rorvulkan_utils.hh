// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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

#include "foundation/rormacros.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"

namespace rhi
{

FORCE_INLINE void vk_destroy_swapchain(VkDevice a_device, VkSwapchainKHR a_swapchain)
{
	vkDestroySwapchainKHR(a_device, a_swapchain, cfg::VkAllocator);
	a_swapchain = nullptr;
}

FORCE_INLINE void vk_destroy_imageview(VkDevice a_device, VkImageView a_image_view)
{
	vkDestroyImageView(a_device, a_image_view, cfg::VkAllocator);
}

FORCE_INLINE void vk_destroy_surface(VkInstance a_instance, VkSurfaceKHR a_surface)
{
	vkDestroySurfaceKHR(a_instance, a_surface, nullptr);
	a_surface = nullptr;
}

FORCE_INLINE void vk_destroy_device(VkDevice a_device)
{
	vkDestroyDevice(a_device, cfg::VkAllocator);
	a_device = nullptr;
}

FORCE_INLINE void vk_destroy_pipeline(VkDevice a_device, VkPipeline a_pipeline)
{
	vkDestroyPipeline(a_device, a_pipeline, cfg::VkAllocator);
	a_pipeline = nullptr;
}

FORCE_INLINE void vk_destroy_shader_module(VkDevice a_device, VkShaderModule a_shader_module)
{
	vkDestroyShaderModule(a_device, a_shader_module, cfg::VkAllocator);
	a_shader_module = nullptr;
}

FORCE_INLINE void vk_destroy_buffer(VkDevice a_device, VkBuffer a_buffer)
{
	vkDestroyBuffer(a_device, a_buffer, cfg::VkAllocator);
	a_buffer = nullptr;
}

FORCE_INLINE void vk_destroy_command_pools(VkDevice a_device, VkCommandPool a_command_pool)
{
	vkDestroyCommandPool(a_device, a_command_pool, cfg::VkAllocator);
}

FORCE_INLINE void vk_destroy_memory(VkDevice a_device, VkDeviceMemory a_memory)
{
	vkFreeMemory(a_device, a_memory, cfg::VkAllocator);
}

}        // namespace rhi
