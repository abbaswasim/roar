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

#pragma once

#include "core/settings/rorsettings.hpp"
#include "foundation/rormacros.hpp"
#include "math/rorvector2.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_object.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include <any>
#include <typeindex>
#include <vector>

namespace rhi
{

class Instance : public VulkanObject<VkInstance>
{
  public:
	FORCE_INLINE           Instance(const Instance &a_other)      = default;        //! Copy constructor
	FORCE_INLINE           Instance(Instance &&a_other) noexcept  = default;        //! Move constructor
	FORCE_INLINE Instance &operator=(const Instance &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Instance &operator=(Instance &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~Instance() noexcept override
	{
		vkDestroyDebugUtilsMessengerEXT(this->get_handle(), this->m_messenger, cfg::VkAllocator);
		this->m_messenger = nullptr;

		vkDestroyInstance(this->get_handle(), cfg::VkAllocator);
		this->reset();
	}

	declare_translation_unit_vtable();

	Instance();

  protected:
  private:
	VkDebugUtilsMessengerEXT m_messenger{nullptr};
};

class PhysicalDevice : public VulkanObject<VkPhysicalDevice>
{
  public:
	FORCE_INLINE                 PhysicalDevice()                                  = default;        //! Copy constructor
	FORCE_INLINE                 PhysicalDevice(const PhysicalDevice &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                 PhysicalDevice(PhysicalDevice &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE PhysicalDevice &operator=(const PhysicalDevice &a_other)          = default;        //! Copy assignment operator
	FORCE_INLINE PhysicalDevice &operator=(PhysicalDevice &&a_other) noexcept      = default;        //! Move assignment operator
	FORCE_INLINE virtual ~PhysicalDevice() noexcept override
	{
		// this->cleanup();
	}

	// void cleanup()
	// {
	// 	// Wait for stuff to finish before deleting
	// 	vkDeviceWaitIdle(this->m_device);

	// 	this->destroy_buffers();
	// 	this->destroy_uniform_buffers();

	// 	this->destroy_descriptor_set_layout();

	// 	this->destroy_sync_object();

	// 	this->cleanup_swapchain();

	// 	this->destroy_texture();
	// 	this->destroy_texture_sampler();

	// 	this->destroy_command_pools();
	// 	this->destroy_descriptor_pools();
	// 	this->destory_surface();
	// 	this->destroy_device();
	// }

	declare_translation_unit_vtable();

	void init(Instance &a_instance)
	{
		auto gpus = enumerate_general_property<VkPhysicalDevice, true>(vkEnumeratePhysicalDevices, a_instance.get_handle());

		VkPhysicalDevice physical_device{VK_NULL_HANDLE};

		for (auto gpu : gpus)
		{
			vkGetPhysicalDeviceProperties(gpu, &this->m_physical_device_properties);

			if (this->m_physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				physical_device = gpu;
				break;
			}
		}

		if (physical_device == nullptr)
		{
			ror::log_critical("Couldn't find suitable discrete physical device, falling back to integrated gpu.");
			assert(gpus.size() > 1);
			physical_device = gpus[0];
		}

		this->set_handle(physical_device);
	}

	// PhysicalDevice(VkInstance a_instance, void *a_window) :
	//     m_window(a_window)
	// {
	// 	// Order of these calls is important, don't reorder
	// 	this->create_surface(this->m_window); // Done
	// 	this->create_physical_device(); // Done
	// 	this->create_device(); // Done
	// 	this->create_swapchain(); // Done
	// 	this->create_imageviews(); // Done

	// 	this->create_descriptor_set_layout();

	// 	// Create pipeline etc, to be cleaned out later
	// 	this->create_render_pass();
	// 	this->create_graphics_pipeline();

	// 	this->create_msaa_color_buffer();
	// 	this->create_depth_buffer();
	// 	this->create_framebuffers();
	// 	this->create_command_pools();
	// 	this->create_descriptor_pools();
	// 	this->create_command_buffers();

	// 	this->create_vertex_buffers();
	// 	this->create_uniform_buffers();
	// 	this->create_texture();
	// 	this->create_descriptor_sets();

	// 	this->record_command_buffers();

	// 	this->create_sync_objects();
	// }

	// void draw_frame(bool a_update_animation)
	// {
	// 	vkWaitForFences(this->m_device, 1, &this->m_queue_fence[this->m_current_frame], VK_TRUE, UINT64_MAX);

	// 	uint32_t image_index;
	// 	VkResult swapchain_res = vkAcquireNextImageKHR(this->m_device, this->m_swapchain, UINT64_MAX, this->m_image_available_semaphore[this->m_current_frame], VK_NULL_HANDLE, &image_index);

	// 	if (swapchain_res == VK_ERROR_OUT_OF_DATE_KHR)
	// 	{
	// 		assert(0 && "This should never happen");
	// 		this->recreate_swapchain();
	// 	}
	// 	else if (swapchain_res != VK_SUCCESS && swapchain_res != VK_SUBOPTIMAL_KHR)
	// 	{
	// 		throw std::runtime_error("Acquire Next image failed or its suboptimal!");
	// 	}

	// 	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
	// 	if (this->m_queue_fence_in_flight[image_index] != VK_NULL_HANDLE)
	// 	{
	// 		vkWaitForFences(this->m_device, 1, &this->m_queue_fence_in_flight[image_index], VK_TRUE, UINT64_MAX);
	// 	}

	// 	// Mark the image as now being in use by this frame
	// 	this->m_queue_fence_in_flight[image_index] = this->m_queue_fence[this->m_current_frame];

	// 	VkSubmitInfo submit_info{};
	// 	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	// 	VkSemaphore          waitSemaphores[] = {this->m_image_available_semaphore[this->m_current_frame]};
	// 	VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	// 	submit_info.waitSemaphoreCount        = 1;
	// 	submit_info.pWaitSemaphores           = waitSemaphores;
	// 	submit_info.pWaitDstStageMask         = waitStages;
	// 	submit_info.commandBufferCount        = 1;
	// 	submit_info.pCommandBuffers           = &this->m_graphics_command_buffers[image_index];

	// 	VkSemaphore signalSemaphores[]   = {m_render_finished_semaphore[this->m_current_frame]};
	// 	submit_info.signalSemaphoreCount = 1;
	// 	submit_info.pSignalSemaphores    = signalSemaphores;

	// 	vkResetFences(this->m_device, 1, &this->m_queue_fence[this->m_current_frame]);

	// 	// Update our uniform buffers for this frame
	// 	this->update_uniform_buffer(image_index, a_update_animation);

	// 	if (vkQueueSubmit(this->m_graphics_queue, 1, &submit_info, this->m_queue_fence[this->m_current_frame]) != VK_SUCCESS)
	// 	{
	// 		throw std::runtime_error("failed to submit draw command buffer!");
	// 	}

	// 	// VkSubpassDependency dependency{};
	// 	// dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
	// 	// dependency.dstSubpass          = 0;
	// 	// dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	// 	// dependency.srcAccessMask       = 0;
	// 	// dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	// 	// dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	// 	// renderPassInfo.dependencyCount = 1;
	// 	// renderPassInfo.pDependencies   = &dependency;

	// 	VkPresentInfoKHR presentInfo{};
	// 	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	// 	presentInfo.waitSemaphoreCount = 1;
	// 	presentInfo.pWaitSemaphores    = signalSemaphores;

	// 	VkSwapchainKHR swapChains[] = {this->m_swapchain};
	// 	presentInfo.swapchainCount  = 1;
	// 	presentInfo.pSwapchains     = swapChains;
	// 	presentInfo.pImageIndices   = &image_index;
	// 	presentInfo.pResults        = nullptr;        // Optional

	// 	swapchain_res = vkQueuePresentKHR(this->m_present_queue, &presentInfo);

	// 	if (swapchain_res == VK_ERROR_OUT_OF_DATE_KHR || swapchain_res == VK_SUBOPTIMAL_KHR)
	// 	{
	// 		assert(0 && "This should never happen");
	// 		this->recreate_swapchain();
	// 	}
	// 	else if (swapchain_res != VK_SUCCESS)
	// 	{
	// 		throw std::runtime_error("Failed to present swapchain image!");
	// 	}

	// 	this->m_current_frame = (this->m_current_frame + 1) % cfg::get_number_of_buffers();
	// }

	// void recreate_swapchain()
	// {
	// 	vkDeviceWaitIdle(this->m_device);
	// 	// vkQueueWaitIdle(this->m_graphics_queue);

	// 	this->cleanup_swapchain();

	// 	this->create_swapchain();
	// 	this->create_imageviews();
	// 	this->create_render_pass();
	// 	this->create_graphics_pipeline();
	// 	this->create_msaa_color_buffer();
	// 	this->create_depth_buffer();
	// 	this->create_framebuffers();
	// 	this->create_command_buffers();

	// 	this->record_command_buffers();
	// }

  protected:
  private:
	// 	VkShaderModule create_shader_module(std::string a_shader_path)
	// 	{
	// 		utl::bytes_vector shader_code;

	// 		utl::align_load_file(a_shader_path, shader_code);

	// 		VkShaderModuleCreateInfo shader_module_info = {};
	// 		shader_module_info.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	// 		shader_module_info.pNext                    = nullptr;
	// 		shader_module_info.flags                    = 0;
	// 		shader_module_info.codeSize                 = shader_code.size();
	// 		shader_module_info.pCode                    = reinterpret_cast<uint32_t *>(shader_code.data());

	// 		VkShaderModule shader_module;
	// 		VkResult       result = vkCreateShaderModule(this->m_device, &shader_module_info, cfg::VkAllocator, &shader_module);
	// 		assert(result == VK_SUCCESS);

	// 		return shader_module;
	// 	}

	// 	void create_framebuffers()
	// 	{
	// 		this->m_framebuffers.resize(this->m_swapchain_images.size());

	// 		for (size_t i = 0; i < this->m_swapchain_image_views.size(); i++)
	// 		{
	// 			std::array<VkImageView, 3> attachments{this->m_msaa_color_image_view, this->m_depth_image_view, this->m_swapchain_image_views[i]};

	// 			VkFramebufferCreateInfo framebuffer_info = {};
	// 			framebuffer_info.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	// 			framebuffer_info.pNext                   = nullptr;
	// 			framebuffer_info.flags                   = 0;
	// 			framebuffer_info.renderPass              = this->m_render_pass;
	// 			framebuffer_info.attachmentCount         = attachments.size();
	// 			framebuffer_info.pAttachments            = attachments.data();
	// 			framebuffer_info.width                   = this->m_swapchain_extent.width;
	// 			framebuffer_info.height                  = this->m_swapchain_extent.height;
	// 			framebuffer_info.layers                  = 1;

	// 			VkResult result = vkCreateFramebuffer(this->m_device, &framebuffer_info, cfg::VkAllocator, &this->m_framebuffers[i]);
	// 			assert(result == VK_SUCCESS);
	// 		}
	// 	}

	// 	void destroy_framebuffers()
	// 	{
	// 		for (auto &framebuffer : this->m_framebuffers)
	// 		{
	// 			vkDestroyFramebuffer(this->m_device, framebuffer, cfg::VkAllocator);
	// 			framebuffer = nullptr;
	// 		}
	// 	}

	// 	void create_command_pools()
	// 	{
	// 		VkCommandPoolCreateInfo command_pool_info = {};
	// 		command_pool_info.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	// 		command_pool_info.pNext                   = nullptr;
	// 		command_pool_info.flags                   = 0;        // VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	// 		command_pool_info.queueFamilyIndex        = this->m_graphics_queue_index;

	// 		VkResult result = vkCreateCommandPool(this->m_device, &command_pool_info, cfg::VkAllocator, &this->m_graphics_command_pool);
	// 		assert(result == VK_SUCCESS);

	// 		command_pool_info.queueFamilyIndex = this->m_transfer_queue_index;

	// 		result = vkCreateCommandPool(this->m_device, &command_pool_info, cfg::VkAllocator, &this->m_transfer_command_pool);
	// 		assert(result == VK_SUCCESS);
	// 	}

	// 	void destroy_command_pools()
	// 	{
	// 		vkDestroyCommandPool(this->m_device, this->m_graphics_command_pool, cfg::VkAllocator);
	// 		vkDestroyCommandPool(this->m_device, this->m_transfer_command_pool, cfg::VkAllocator);
	// 	}

	// 	void create_descriptor_pools()
	// 	{
	// 		std::array<VkDescriptorPoolSize, 2> pool_size{};
	// 		pool_size[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	// 		pool_size[0].descriptorCount = static_cast<uint32_t>(cfg::get_number_of_buffers());        // This should be more generic, perhaps a pool per thread/per frame/per command buffer, TODO: Find out

	// 		pool_size[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	// 		pool_size[1].descriptorCount = static_cast<uint32_t>(cfg::get_number_of_buffers());        // This should be more generic, perhaps a pool per thread/per frame/per command buffer, TODO: Find out

	// 		VkDescriptorPoolCreateInfo descriptor_pool_create_info{};
	// 		descriptor_pool_create_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	// 		descriptor_pool_create_info.poolSizeCount = pool_size.size();
	// 		descriptor_pool_create_info.pPoolSizes    = pool_size.data();
	// 		descriptor_pool_create_info.maxSets       = static_cast<uint32_t>(cfg::get_number_of_buffers());
	// 		descriptor_pool_create_info.flags         = 0;        // VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT

	// 		VkResult result = vkCreateDescriptorPool(this->m_device, &descriptor_pool_create_info, cfg::VkAllocator, &this->m_descriptor_pool);
	// 		assert(result == VK_SUCCESS);
	// 	}

	// 	void destroy_descriptor_pools()
	// 	{
	// 		vkDestroyDescriptorPool(this->m_device, this->m_descriptor_pool, cfg::VkAllocator);
	// 	}

	// 	void create_descriptor_sets()
	// 	{
	// 		std::vector<VkDescriptorSetLayout> layouts(cfg::get_number_of_buffers(), this->m_descriptor_set_layout);

	// 		VkDescriptorSetAllocateInfo descriptor_set_allocate_info{};
	// 		descriptor_set_allocate_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	// 		descriptor_set_allocate_info.descriptorPool     = this->m_descriptor_pool;
	// 		descriptor_set_allocate_info.descriptorSetCount = static_cast<uint32_t>(layouts.size());
	// 		descriptor_set_allocate_info.pSetLayouts        = layouts.data();

	// 		VkResult result = vkAllocateDescriptorSets(this->m_device, &descriptor_set_allocate_info, this->m_descriptor_sets.data());
	// 		assert(result == VK_SUCCESS && "Failed to allocate descriptor sets");

	// 		// Update descriptor configuration
	// 		for (size_t i = 0; i < this->m_descriptor_sets.size(); i++)
	// 		{
	// 			// VkDescriptorImageInfo image_info{}; // Another option if we are dealing with images

	// 			VkDescriptorBufferInfo buffer_info{};
	// 			buffer_info.buffer = this->m_uniform_buffers[i];
	// 			buffer_info.offset = 0;
	// 			buffer_info.range  = VK_WHOLE_SIZE;        // sizeof(Uniforms);

	// 			VkDescriptorImageInfo image_info{};
	// 			image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	// 			image_info.imageView   = this->m_texture_image_view;
	// 			image_info.sampler     = this->m_texture_sampler;

	// 			std::array<VkWriteDescriptorSet, 2> descriptor_write{};
	// 			descriptor_write[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	// 			descriptor_write[0].dstSet           = this->m_descriptor_sets[i];
	// 			descriptor_write[0].dstBinding       = 0;        // TODO: Another hardcoded binding for descriptor
	// 			descriptor_write[0].dstArrayElement  = 0;
	// 			descriptor_write[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	// 			descriptor_write[0].descriptorCount  = 1;
	// 			descriptor_write[0].pBufferInfo      = &buffer_info;
	// 			descriptor_write[0].pImageInfo       = nullptr;        // Optional
	// 			descriptor_write[0].pTexelBufferView = nullptr;        // Optional

	// 			descriptor_write[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	// 			descriptor_write[1].dstSet           = this->m_descriptor_sets[i];
	// 			descriptor_write[1].dstBinding       = 1;        // TODO: Another hardcoded binding for descriptor
	// 			descriptor_write[1].dstArrayElement  = 0;
	// 			descriptor_write[1].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	// 			descriptor_write[1].descriptorCount  = 1;
	// 			descriptor_write[1].pBufferInfo      = nullptr;
	// 			descriptor_write[1].pImageInfo       = &image_info;
	// 			descriptor_write[1].pTexelBufferView = nullptr;        // Optional

	// 			vkUpdateDescriptorSets(this->m_device, descriptor_write.size(), descriptor_write.data(), 0, nullptr);
	// 		}
	// 	}

	// 	void create_command_buffers()
	// 	{
	// 		this->m_graphics_command_buffers.resize(this->m_framebuffers.size());

	// 		VkCommandBufferAllocateInfo command_buffer_allocation_info = {};
	// 		command_buffer_allocation_info.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	// 		command_buffer_allocation_info.pNext                       = nullptr;
	// 		command_buffer_allocation_info.commandPool                 = this->m_graphics_command_pool;
	// 		command_buffer_allocation_info.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	// 		command_buffer_allocation_info.commandBufferCount          = static_cast<uint32_t>(this->m_graphics_command_buffers.size());

	// 		VkResult result = vkAllocateCommandBuffers(this->m_device, &command_buffer_allocation_info, this->m_graphics_command_buffers.data());
	// 		assert(result == VK_SUCCESS);
	// 	}

	// 	void create_semaphore(VkSemaphore &a_semaphore)
	// 	{
	// 		VkSemaphoreCreateInfo semaphore_info = {};
	// 		semaphore_info.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	// 		semaphore_info.pNext                 = nullptr;
	// 		semaphore_info.flags                 = 0;

	// 		VkResult result = vkCreateSemaphore(this->m_device, &semaphore_info, cfg::VkAllocator, &a_semaphore);
	// 		assert(result == VK_SUCCESS);
	// 	}

	// 	void create_fence(VkFence &a_fence)
	// 	{
	// 		VkFenceCreateInfo fence_info = {};
	// 		fence_info.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	// 		fence_info.pNext             = nullptr;
	// 		fence_info.flags             = VK_FENCE_CREATE_SIGNALED_BIT;

	// 		VkResult result = vkCreateFence(this->m_device, &fence_info, cfg::VkAllocator, &a_fence);
	// 		assert(result == VK_SUCCESS);
	// 	}

	// 	void create_sync_objects()
	// 	{
	// 		for (size_t i = 0; i < cfg::get_number_of_buffers(); ++i)
	// 		{
	// 			this->create_semaphore(this->m_image_available_semaphore[i]);
	// 			this->create_semaphore(this->m_render_finished_semaphore[i]);

	// 			this->create_fence(this->m_queue_fence[i]);

	// 			this->m_queue_fence_in_flight[i] = VK_NULL_HANDLE;
	// 		}
	// 	}

	// 	void destroy_sync_object()
	// 	{
	// 		for (size_t i = 0; i < cfg::get_number_of_buffers(); ++i)
	// 		{
	// 			vkDestroyFence(this->m_device, this->m_queue_fence[i], cfg::VkAllocator);
	// 			vkDestroySemaphore(this->m_device, this->m_image_available_semaphore[i], cfg::VkAllocator);
	// 			vkDestroySemaphore(this->m_device, this->m_render_finished_semaphore[i], cfg::VkAllocator);
	// 		}
	// 	}

	// 	void create_graphics_pipeline()
	// 	{
	// 		VkShaderModule vert_shader_module;
	// 		VkShaderModule frag_shader_module;

	// 		vert_shader_module = this->create_shader_module("assets/shaders/tri.vert.spv");
	// 		frag_shader_module = this->create_shader_module("assets/shaders/tri.frag.spv");

	// 		VkPipelineShaderStageCreateInfo vert_shader_stage_info = {};
	// 		vert_shader_stage_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	// 		vert_shader_stage_info.pNext                           = nullptr;
	// 		vert_shader_stage_info.flags                           = 0;
	// 		vert_shader_stage_info.stage                           = VK_SHADER_STAGE_VERTEX_BIT;
	// 		vert_shader_stage_info.module                          = vert_shader_module;
	// 		vert_shader_stage_info.pName                           = "main";
	// 		vert_shader_stage_info.pSpecializationInfo             = nullptr;

	// 		VkPipelineShaderStageCreateInfo frag_shader_stage_info = {};
	// 		frag_shader_stage_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	// 		frag_shader_stage_info.pNext                           = nullptr;
	// 		frag_shader_stage_info.flags                           = 0;
	// 		frag_shader_stage_info.stage                           = VK_SHADER_STAGE_FRAGMENT_BIT;
	// 		frag_shader_stage_info.module                          = frag_shader_module;
	// 		frag_shader_stage_info.pName                           = "main";
	// 		frag_shader_stage_info.pSpecializationInfo             = nullptr;

	// 		VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info, frag_shader_stage_info};

	// 		// This is where you add where the vertex data is coming from
	// 		// TODO: To be abstracted later so it can be configured properly
	// 		auto vertex_attribute_descriptions = utl::get_astro_boy_vertex_attributes();
	// 		auto vertex_attribute_bindings     = utl::get_astro_boy_vertex_bindings();

	// 		VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_info = {};
	// 		pipeline_vertex_input_state_info.sType                                = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	// 		pipeline_vertex_input_state_info.pNext                                = nullptr;
	// 		pipeline_vertex_input_state_info.flags                                = 0;
	// 		pipeline_vertex_input_state_info.vertexBindingDescriptionCount        = vertex_attribute_bindings.size();
	// 		pipeline_vertex_input_state_info.pVertexBindingDescriptions           = vertex_attribute_bindings.data();
	// 		pipeline_vertex_input_state_info.vertexAttributeDescriptionCount      = vertex_attribute_descriptions.size();
	// 		pipeline_vertex_input_state_info.pVertexAttributeDescriptions         = vertex_attribute_descriptions.data();

	// 		VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_info = {};
	// 		pipeline_input_assembly_info.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	// 		pipeline_input_assembly_info.pNext                                  = nullptr;
	// 		pipeline_input_assembly_info.topology                               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	// 		pipeline_input_assembly_info.primitiveRestartEnable                 = VK_FALSE;

	// 		VkViewport viewport = {};
	// 		viewport.x          = 0.0f;
	// 		viewport.y          = 0.0f;
	// 		viewport.width      = static_cast<float32_t>(this->m_swapchain_extent.width);
	// 		viewport.height     = static_cast<float32_t>(this->m_swapchain_extent.height);
	// 		viewport.minDepth   = 0.0f;
	// 		viewport.maxDepth   = 1.0f;

	// 		VkRect2D scissor = {};
	// 		scissor.offset   = {0, 0};
	// 		scissor.extent   = this->m_swapchain_extent;

	// 		VkPipelineViewportStateCreateInfo pipeline_viewport_state_info = {};
	// 		pipeline_viewport_state_info.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	// 		pipeline_viewport_state_info.pNext                             = nullptr;
	// 		pipeline_viewport_state_info.flags                             = 0;
	// 		pipeline_viewport_state_info.viewportCount                     = 1;
	// 		pipeline_viewport_state_info.pViewports                        = &viewport;
	// 		pipeline_viewport_state_info.scissorCount                      = 1;
	// 		pipeline_viewport_state_info.pScissors                         = &scissor;

	// 		VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_info = {};
	// 		pipeline_rasterization_state_info.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	// 		pipeline_rasterization_state_info.pNext                                  = nullptr;
	// 		pipeline_rasterization_state_info.flags                                  = 0;
	// 		pipeline_rasterization_state_info.depthClampEnable                       = VK_FALSE;
	// 		pipeline_rasterization_state_info.rasterizerDiscardEnable                = VK_FALSE;
	// 		pipeline_rasterization_state_info.polygonMode                            = VK_POLYGON_MODE_FILL;
	// 		pipeline_rasterization_state_info.lineWidth                              = 1.0f;
	// 		pipeline_rasterization_state_info.cullMode                               = VK_CULL_MODE_BACK_BIT;
	// 		pipeline_rasterization_state_info.frontFace                              = VK_FRONT_FACE_COUNTER_CLOCKWISE;        // TODO: Model3d is counter clock wise fix this

	// 		pipeline_rasterization_state_info.depthBiasEnable         = VK_FALSE;
	// 		pipeline_rasterization_state_info.depthBiasConstantFactor = 0.0f;        // Optional
	// 		pipeline_rasterization_state_info.depthBiasClamp          = 0.0f;        // Optional
	// 		pipeline_rasterization_state_info.depthBiasSlopeFactor    = 0.0f;        // Optional

	// 		VkPipelineMultisampleStateCreateInfo pipeline_multisampling_state_info = {};
	// 		pipeline_multisampling_state_info.sType                                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	// 		pipeline_multisampling_state_info.pNext                                = nullptr;
	// 		pipeline_multisampling_state_info.flags                                = 0;
	// 		pipeline_multisampling_state_info.rasterizationSamples                 = this->get_sample_count();
	// 		pipeline_multisampling_state_info.sampleShadingEnable                  = (cfg::get_sample_rate_shading_enabled() ? VK_TRUE : VK_FALSE);
	// 		pipeline_multisampling_state_info.minSampleShading                     = (cfg::get_sample_rate_shading_enabled() ? cfg::get_sample_rate_shading() : 1.0f);
	// 		pipeline_multisampling_state_info.pSampleMask                          = nullptr;         // Optional
	// 		pipeline_multisampling_state_info.alphaToCoverageEnable                = VK_FALSE;        // Optional
	// 		pipeline_multisampling_state_info.alphaToOneEnable                     = VK_FALSE;        // Optional

	// 		VkPipelineDepthStencilStateCreateInfo pipeline_depth_stencil_info = {};
	// 		pipeline_depth_stencil_info.sType                                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	// 		pipeline_depth_stencil_info.pNext                                 = nullptr;
	// 		pipeline_depth_stencil_info.flags                                 = 0;
	// 		pipeline_depth_stencil_info.depthTestEnable                       = VK_TRUE;        // TODO: Depth testing should be enabled later
	// 		pipeline_depth_stencil_info.depthWriteEnable                      = VK_TRUE;
	// 		pipeline_depth_stencil_info.depthCompareOp                        = VK_COMPARE_OP_LESS_OR_EQUAL;
	// 		pipeline_depth_stencil_info.depthBoundsTestEnable                 = VK_FALSE;
	// 		pipeline_depth_stencil_info.stencilTestEnable                     = VK_FALSE;
	// 		pipeline_depth_stencil_info.front                                 = VkStencilOpState{};        // TODO: Needs fixing
	// 		pipeline_depth_stencil_info.back                                  = VkStencilOpState{};        // TODO: Needs fixing
	// 		pipeline_depth_stencil_info.minDepthBounds                        = 0.0f;
	// 		pipeline_depth_stencil_info.maxDepthBounds                        = 1.0f;

	// 		VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment_info = {};
	// 		pipeline_color_blend_attachment_info.blendEnable                         = VK_FALSE;
	// 		pipeline_color_blend_attachment_info.srcColorBlendFactor                 = VK_BLEND_FACTOR_ONE;         // Optional
	// 		pipeline_color_blend_attachment_info.dstColorBlendFactor                 = VK_BLEND_FACTOR_ZERO;        // Optional
	// 		pipeline_color_blend_attachment_info.colorBlendOp                        = VK_BLEND_OP_ADD;             // Optional
	// 		pipeline_color_blend_attachment_info.srcAlphaBlendFactor                 = VK_BLEND_FACTOR_ONE;         // Optional
	// 		pipeline_color_blend_attachment_info.dstAlphaBlendFactor                 = VK_BLEND_FACTOR_ZERO;        // Optional
	// 		pipeline_color_blend_attachment_info.alphaBlendOp                        = VK_BLEND_OP_ADD;             // Optional
	// 		pipeline_color_blend_attachment_info.colorWriteMask                      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	// 		/*
	// 		  // Could do simple alpha blending with this
	// 		  colorBlendAttachment.blendEnable = VK_TRUE;
	// 		  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	// 		  colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	// 		  colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	// 		  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	// 		  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	// 		  colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	// 		*/

	// 		VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_info = {};
	// 		pipeline_color_blend_state_info.sType                               = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	// 		pipeline_color_blend_state_info.pNext                               = nullptr;
	// 		pipeline_color_blend_state_info.flags                               = 0;
	// 		pipeline_color_blend_state_info.logicOpEnable                       = VK_FALSE;
	// 		pipeline_color_blend_state_info.logicOp                             = VK_LOGIC_OP_COPY;
	// 		pipeline_color_blend_state_info.attachmentCount                     = 1;
	// 		pipeline_color_blend_state_info.pAttachments                        = &pipeline_color_blend_attachment_info;
	// 		pipeline_color_blend_state_info.blendConstants[0]                   = 0.0f;
	// 		pipeline_color_blend_state_info.blendConstants[1]                   = 0.0f;
	// 		pipeline_color_blend_state_info.blendConstants[2]                   = 0.0f;
	// 		pipeline_color_blend_state_info.blendConstants[3]                   = 0.0f;

	// 		VkDynamicState dynamic_states[] = {
	// 		    VK_DYNAMIC_STATE_VIEWPORT,
	// 		    // VK_DYNAMIC_STATE_CULL_MODE_EXT,
	// 		    // VK_DYNAMIC_STATE_FRONT_FACE_EXT,
	// 		    VK_DYNAMIC_STATE_LINE_WIDTH};

	// 		VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_info = {};
	// 		pipeline_dynamic_state_info.sType                            = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	// 		pipeline_dynamic_state_info.pNext                            = nullptr;
	// 		pipeline_dynamic_state_info.flags                            = 0;
	// 		pipeline_dynamic_state_info.dynamicStateCount                = 2;
	// 		pipeline_dynamic_state_info.pDynamicStates                   = dynamic_states;

	// 		VkPipelineLayoutCreateInfo pipeline_layout_info = {};
	// 		pipeline_layout_info.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	// 		pipeline_layout_info.pNext                      = nullptr;
	// 		pipeline_layout_info.flags                      = 0;
	// 		pipeline_layout_info.setLayoutCount             = 1;
	// 		pipeline_layout_info.pSetLayouts                = &this->m_descriptor_set_layout;
	// 		pipeline_layout_info.pushConstantRangeCount     = 0;              // Optional
	// 		pipeline_layout_info.pPushConstantRanges        = nullptr;        // Optional

	// 		VkResult result = vkCreatePipelineLayout(this->m_device, &pipeline_layout_info, cfg::VkAllocator, &this->m_pipeline_layout);
	// 		assert(result == VK_SUCCESS);

	// 		VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {};
	// 		graphics_pipeline_create_info.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	// 		graphics_pipeline_create_info.pNext                        = nullptr;
	// 		graphics_pipeline_create_info.flags                        = 0;
	// 		graphics_pipeline_create_info.stageCount                   = 2;
	// 		graphics_pipeline_create_info.pStages                      = shader_stages;
	// 		graphics_pipeline_create_info.pVertexInputState            = &pipeline_vertex_input_state_info;
	// 		graphics_pipeline_create_info.pInputAssemblyState          = &pipeline_input_assembly_info;
	// 		graphics_pipeline_create_info.pTessellationState           = nullptr;
	// 		graphics_pipeline_create_info.pViewportState               = &pipeline_viewport_state_info;
	// 		graphics_pipeline_create_info.pRasterizationState          = &pipeline_rasterization_state_info;
	// 		graphics_pipeline_create_info.pMultisampleState            = &pipeline_multisampling_state_info;
	// 		graphics_pipeline_create_info.pDepthStencilState           = &pipeline_depth_stencil_info;
	// 		graphics_pipeline_create_info.pColorBlendState             = &pipeline_color_blend_state_info;
	// 		graphics_pipeline_create_info.pDynamicState                = &pipeline_dynamic_state_info;
	// 		graphics_pipeline_create_info.layout                       = this->m_pipeline_layout;
	// 		graphics_pipeline_create_info.renderPass                   = this->m_render_pass;        // TODO: This is not created before hand
	// 		graphics_pipeline_create_info.subpass                      = 0;
	// 		graphics_pipeline_create_info.basePipelineHandle           = VK_NULL_HANDLE;
	// 		graphics_pipeline_create_info.basePipelineIndex            = -1;

	// 		result = vkCreateGraphicsPipelines(this->m_device, this->m_pipeline_cache, 1, &graphics_pipeline_create_info, cfg::VkAllocator, &this->m_graphics_pipeline);
	// 		assert(result == VK_SUCCESS);

	// 		// cleanup
	// 		vkDestroyShaderModule(this->m_device, vert_shader_module, cfg::VkAllocator);
	// 		vkDestroyShaderModule(this->m_device, frag_shader_module, cfg::VkAllocator);
	// 	}

	// 	void destroy_graphics_pipeline()
	// 	{
	// 		vkDestroyPipelineLayout(this->m_device, this->m_pipeline_layout, cfg::VkAllocator);
	// 		this->m_pipeline_layout = nullptr;

	// 		vkDestroyPipeline(this->m_device, this->m_graphics_pipeline, cfg::VkAllocator);
	// 		this->m_graphics_pipeline = nullptr;
	// 	}

	// 	void record_command_buffers()
	// 	{
	// 		for (size_t i = 0; i < this->m_graphics_command_buffers.size(); i++)
	// 		{
	// 			const VkCommandBuffer &current_command_buffer = this->m_graphics_command_buffers[i];

	// 			VkCommandBufferBeginInfo command_buffer_begin_info = {};
	// 			command_buffer_begin_info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	// 			command_buffer_begin_info.pNext                    = nullptr;
	// 			command_buffer_begin_info.flags                    = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;        // TODO: In pratice should be VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT no reuse of command buffers
	// 			command_buffer_begin_info.pInheritanceInfo         = nullptr;                                             // Optional

	// 			VkResult result = vkBeginCommandBuffer(current_command_buffer, &command_buffer_begin_info);
	// 			assert(result == VK_SUCCESS);

	// 			// VkClearValue clear_color = {{{0.028f, 0.028f, 0.03f, 1.0f}}};        // This is the color I want, but I think SRGB is making this very bright than it should be
	// 			std::array<VkClearValue, 2> clear_color_depth{};
	// 			clear_color_depth[0].color        = {{0.5f, 0.5f, 0.5f, 1.0f}};
	// 			clear_color_depth[1].depthStencil = {1.0f, 0};

	// 			VkRenderPassBeginInfo render_pass_begin_info = {};
	// 			render_pass_begin_info.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	// 			render_pass_begin_info.pNext                 = nullptr;
	// 			render_pass_begin_info.renderPass            = this->m_render_pass;
	// 			render_pass_begin_info.framebuffer           = this->m_framebuffers[i];
	// 			render_pass_begin_info.renderArea.offset     = {0, 0};
	// 			render_pass_begin_info.renderArea.extent     = this->m_swapchain_extent;
	// 			render_pass_begin_info.clearValueCount       = clear_color_depth.size();
	// 			render_pass_begin_info.pClearValues          = clear_color_depth.data();

	// 			vkCmdBeginRenderPass(current_command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);        // VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS for secondary
	// 			assert(result == VK_SUCCESS);

	// 			VkViewport viewport = {};
	// 			viewport.x          = 0.0f;
	// 			viewport.y          = 0.0f;
	// 			viewport.width      = static_cast<float>(this->m_swapchain_extent.width);
	// 			viewport.height     = static_cast<float>(this->m_swapchain_extent.height);
	// 			viewport.minDepth   = 0.0f;
	// 			viewport.maxDepth   = 1.0f;

	// 			this->update_uniform_buffer(i, true);

	// 			vkCmdBindPipeline(current_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_graphics_pipeline);
	// 			vkCmdSetViewport(current_command_buffer, 0, 1, &viewport);

	// 			VkBuffer vertexBuffers[] = {this->m_vertex_buffers[0],
	// 			                            this->m_vertex_buffers[1],
	// 			                            this->m_vertex_buffers[1],
	// 			                            this->m_vertex_buffers[1],
	// 			                            this->m_vertex_buffers[1]};

	// 			VkDeviceSize offsets[] = {astro_boy_positions_array_count * 0,
	// 			                          astro_boy_normals_array_count * 0,                                                                                                                             // Normal offset
	// 			                          astro_boy_normals_array_count * sizeof(float32_t),                                                                                                             // UV offset
	// 			                          astro_boy_normals_array_count * sizeof(float32_t) + astro_boy_uvs_array_count * sizeof(float32_t),                                                             // Weight offset
	// 			                          astro_boy_normals_array_count * sizeof(float32_t) + astro_boy_uvs_array_count * sizeof(float32_t) + astro_boy_weights_array_count * sizeof(float32_t)};        // JointID offset

	// 			vkCmdBindVertexBuffers(current_command_buffer, 0, 5, vertexBuffers, offsets);

	// 			vkCmdBindIndexBuffer(current_command_buffer, this->m_index_buffer, 0, VK_INDEX_TYPE_UINT32);

	// 			vkCmdBindDescriptorSets(current_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pipeline_layout, 0, 1, &this->m_descriptor_sets[i], 0, nullptr);

	// 			// vkCmdDraw(current_command_buffer, static_cast<uint32_t>(astro_boy_indices_array_count), 1, 0, 0);
	// 			vkCmdDrawIndexed(current_command_buffer, astro_boy_indices_array_count, 1, 0, 0, 0);

	// 			vkCmdEndRenderPass(current_command_buffer);

	// 			result = vkEndCommandBuffer(current_command_buffer);
	// 			assert(result == VK_SUCCESS);
	// 		}
	// 	}

	// 	void create_render_pass()
	// 	{
	// 		VkSampleCountFlagBits msaa_samples = this->get_sample_count();

	// 		VkAttachmentDescription color_attachment_description = {};
	// 		color_attachment_description.flags                   = 0;
	// 		color_attachment_description.format                  = this->m_swapchain_format;
	// 		color_attachment_description.samples                 = msaa_samples;
	// 		color_attachment_description.loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// 		color_attachment_description.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
	// 		color_attachment_description.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	// 		color_attachment_description.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	// 		color_attachment_description.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
	// 		color_attachment_description.finalLayout             = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;        // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// 		VkAttachmentReference color_attachment_reference = {};
	// 		color_attachment_reference.attachment            = 0;
	// 		color_attachment_reference.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// 		VkAttachmentDescription depth_attachment_description = {};
	// 		depth_attachment_description.flags                   = 0;
	// 		depth_attachment_description.format                  = VK_FORMAT_D24_UNORM_S8_UINT;
	// 		depth_attachment_description.samples                 = msaa_samples;
	// 		depth_attachment_description.loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// 		depth_attachment_description.storeOp                 = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	// 		depth_attachment_description.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	// 		depth_attachment_description.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	// 		depth_attachment_description.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
	// 		depth_attachment_description.finalLayout             = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// 		VkAttachmentReference depth_attachment_reference = {};
	// 		depth_attachment_reference.attachment            = 1;
	// 		depth_attachment_reference.layout                = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// 		VkAttachmentDescription resolved_attachment_description = {};
	// 		resolved_attachment_description.flags                   = 0;
	// 		resolved_attachment_description.format                  = this->m_swapchain_format;
	// 		resolved_attachment_description.samples                 = VK_SAMPLE_COUNT_1_BIT;
	// 		resolved_attachment_description.loadOp                  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	// 		resolved_attachment_description.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
	// 		resolved_attachment_description.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	// 		resolved_attachment_description.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	// 		resolved_attachment_description.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
	// 		resolved_attachment_description.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// 		VkAttachmentReference resolve_attachment_reference = {};
	// 		resolve_attachment_reference.attachment            = 2;
	// 		resolve_attachment_reference.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// 		VkSubpassDescription subpass_description    = {};
	// 		subpass_description.flags                   = 0;
	// 		subpass_description.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
	// 		subpass_description.inputAttachmentCount    = 0;
	// 		subpass_description.pInputAttachments       = nullptr;
	// 		subpass_description.colorAttachmentCount    = 1;
	// 		subpass_description.pColorAttachments       = &color_attachment_reference;
	// 		subpass_description.pResolveAttachments     = &resolve_attachment_reference;
	// 		subpass_description.pDepthStencilAttachment = &depth_attachment_reference;
	// 		subpass_description.preserveAttachmentCount = 0;
	// 		subpass_description.pPreserveAttachments    = nullptr;

	// 		VkSubpassDependency subpass_dependency = {};
	// 		subpass_dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
	// 		subpass_dependency.dstSubpass          = 0;
	// 		subpass_dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	// 		subpass_dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	// 		subpass_dependency.srcAccessMask       = 0;
	// 		subpass_dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	// 		subpass_dependency.dependencyFlags     = 0;

	// 		std::array<VkAttachmentDescription, 3> attachments{color_attachment_description, depth_attachment_description, resolved_attachment_description};

	// 		VkRenderPassCreateInfo render_pass_info = {};
	// 		render_pass_info.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	// 		render_pass_info.pNext                  = nullptr;
	// 		render_pass_info.flags                  = 0;
	// 		render_pass_info.attachmentCount        = attachments.size();
	// 		render_pass_info.pAttachments           = attachments.data();
	// 		render_pass_info.subpassCount           = 1;
	// 		render_pass_info.pSubpasses             = &subpass_description;
	// 		render_pass_info.dependencyCount        = 1;
	// 		render_pass_info.pDependencies          = &subpass_dependency;

	// 		VkResult result = vkCreateRenderPass(this->m_device, &render_pass_info, cfg::VkAllocator, &this->m_render_pass);
	// 		assert(result == VK_SUCCESS);
	// 	}

	// 	uint32_t find_memory_type(uint32_t a_type_filter, VkMemoryPropertyFlags a_properties)
	// 	{
	// 		VkPhysicalDeviceMemoryProperties memory_properties{};
	// 		vkGetPhysicalDeviceMemoryProperties(this->m_physical_device, &memory_properties);

	// 		for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
	// 		{
	// 			if (a_type_filter & (1 << i) && ((memory_properties.memoryTypes[i].propertyFlags & a_properties) == a_properties))
	// 			{
	// 				return i;
	// 			}
	// 		}

	// 		throw std::runtime_error("Failed to find suitable memory type!");
	// 	}

	// 	auto allocate_bind_buffer_memory(VkBuffer a_buffer, VkMemoryPropertyFlags a_properties = (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
	// 	{
	// 		VkMemoryRequirements buffer_mem_req{};
	// 		vkGetBufferMemoryRequirements(this->m_device, a_buffer, &buffer_mem_req);

	// 		VkDeviceMemory buffer_memory;

	// 		VkMemoryAllocateInfo allocation_info{};
	// 		allocation_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	// 		allocation_info.allocationSize  = buffer_mem_req.size;
	// 		allocation_info.memoryTypeIndex = find_memory_type(buffer_mem_req.memoryTypeBits, a_properties);
	// 		allocation_info.pNext           = nullptr;

	// 		VkResult result = vkAllocateMemory(this->m_device, &allocation_info, cfg::VkAllocator, &buffer_memory);
	// 		assert(result == VK_SUCCESS && "Failed to allocate vulkan buffer memory!");
	// 		assert(buffer_memory);

	// 		result = vkBindBufferMemory(this->m_device, a_buffer, buffer_memory, 0);
	// 		assert(result == VK_SUCCESS && "Failed to bind vulkan buffer memory!");

	// 		return buffer_memory;
	// 	}

	// 	auto create_buffer(size_t a_size, VkBufferUsageFlags a_usage)
	// 	{
	// 		// TODO: Change default behaviour of sharing between transfer and graphics only
	// 		std::vector<uint32_t> indicies{this->m_graphics_queue_index, this->m_transfer_queue_index};

	// 		VkBufferCreateInfo buffer_info{};

	// 		buffer_info.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	// 		buffer_info.pNext                 = nullptr;
	// 		buffer_info.flags                 = 0;
	// 		buffer_info.size                  = a_size;                            // example: 1024 * 1024 * 2;        // 2kb
	// 		buffer_info.usage                 = a_usage;                           // example: VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
	// 		buffer_info.sharingMode           = VK_SHARING_MODE_CONCURRENT;        // example: VK_SHARING_MODE_EXCLUSIVE; // TODO: Make this more variable, this has performance implications, not all resources are shared either
	// 		buffer_info.queueFamilyIndexCount = ror::static_cast_safe<uint32_t>(indicies.size());
	// 		buffer_info.pQueueFamilyIndices   = indicies.data();

	// 		VkBuffer buffer;
	// 		VkResult result = vkCreateBuffer(this->m_device, &buffer_info, cfg::VkAllocator, &buffer);

	// 		assert(result == VK_SUCCESS && "Failed to create vulkan buffer!");
	// 		assert(buffer);

	// 		return buffer;
	// 	}

	// 	void create_uniform_buffers()
	// 	{
	// 		VkDeviceSize buffer_size = sizeof(Uniforms);

	// 		for (size_t i = 0; i < this->m_uniform_buffers.size(); i++)
	// 		{
	// 			this->m_uniform_buffers[i]        = this->create_buffer(buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
	// 			this->m_uniform_buffers_memory[i] = this->allocate_bind_buffer_memory(this->m_uniform_buffers[i]);
	// 		}
	// 	}

	// 	void create_descriptor_set_layout()
	// 	{
	// 		// TODO: This is where you create a layout that works for everything like machinery or do something else
	// 		VkDescriptorSetLayoutBinding ubo_layout_binding{};
	// 		ubo_layout_binding.binding            = 0;        // This is the binding in the shader that is hardcoded at this stage
	// 		ubo_layout_binding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	// 		ubo_layout_binding.descriptorCount    = 1;
	// 		ubo_layout_binding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;        // This should also be something like VK_SHADER_STAGE_ALL or VK_SHADER_STAGE_ALL_GRAPHICS to simplify things but might have perf implications
	// 		ubo_layout_binding.pImmutableSamplers = nullptr;                           // Optional for uniforms but required for images

	// 		VkDescriptorSetLayoutBinding sampler_layout_binding{};
	// 		sampler_layout_binding.binding            = 1;
	// 		sampler_layout_binding.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	// 		sampler_layout_binding.descriptorCount    = 1;
	// 		sampler_layout_binding.pImmutableSamplers = nullptr;
	// 		sampler_layout_binding.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;

	// 		std::array<VkDescriptorSetLayoutBinding, 2> bindings{ubo_layout_binding, sampler_layout_binding};

	// 		VkDescriptorSetLayoutCreateInfo layout_info{};
	// 		layout_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	// 		layout_info.bindingCount = bindings.size();
	// 		layout_info.pBindings    = bindings.data();

	// 		VkResult result = vkCreateDescriptorSetLayout(this->m_device, &layout_info, nullptr, &this->m_descriptor_set_layout);

	// 		assert(result == VK_SUCCESS && "Failed to create descriptor set layout");
	// 	}

	// 	void destroy_descriptor_set_layout()
	// 	{
	// 		vkDestroyDescriptorSetLayout(this->m_device, this->m_descriptor_set_layout, cfg::VkAllocator);
	// 		this->m_descriptor_set_layout = nullptr;
	// 	}

	// 	void update_uniform_buffer(size_t a_index, bool a_animate)
	// 	{
	// 		ror::Matrix4f model;
	// 		ror::Matrix4f view_projection;
	// 		ror::Vector3f camera_position;

	// 		ror::Matrix4f model_matrix{ror::matrix4_rotation_around_x(ror::to_radians(-90.0f))};
	// 		ror::Matrix4f translation{ror::matrix4_translation(ror::Vector3f{0.0f, 0.0f, -(this->m_astroboy_bbox.maximum() - this->m_astroboy_bbox.minimum()).z} / 2.0f)};

	// 		ror::glfw_camera_update(view_projection, model, camera_position);

	// 		model = model_matrix * translation * model;

	// 		Uniforms *uniform_data;
	// 		vkMapMemory(this->m_device, this->m_uniform_buffers_memory[a_index], 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void **>(&uniform_data));

	// 		uniform_data->model           = model;
	// 		uniform_data->view_projection = ror::vulkan_clip_correction * view_projection;

	// 		auto skinning_matrices = this->animate(a_animate);
	// 		memcpy(uniform_data->joints_matrices[0].m_values, skinning_matrices[0].m_values, 44 * sizeof(float) * 16);

	// 		vkUnmapMemory(this->m_device, this->m_uniform_buffers_memory[a_index]);
	// 	}

	// 	void destroy_uniform_buffers()
	// 	{
	// 		for (size_t i = 0; i < this->m_uniform_buffers.size(); i++)
	// 		{
	// 			vkDestroyBuffer(this->m_device, this->m_uniform_buffers[i], cfg::VkAllocator);
	// 			vkFreeMemory(this->m_device, this->m_uniform_buffers_memory[i], cfg::VkAllocator);
	// 			this->m_uniform_buffers[i]        = nullptr;
	// 			this->m_uniform_buffers_memory[i] = nullptr;
	// 		}
	// 	}

	// 	auto begin_single_use_cmd_buffer()
	// 	{
	// 		VkCommandBufferAllocateInfo command_buffer_allocate_info{};
	// 		command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	// 		command_buffer_allocate_info.pNext              = nullptr;
	// 		command_buffer_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	// 		command_buffer_allocate_info.commandPool        = this->m_transfer_command_pool;
	// 		command_buffer_allocate_info.commandBufferCount = 1;

	// 		VkCommandBuffer staging_command_buffer;
	// 		VkResult        result = vkAllocateCommandBuffers(this->m_device, &command_buffer_allocate_info, &staging_command_buffer);
	// 		assert(result == VK_SUCCESS);

	// 		VkCommandBufferBeginInfo command_buffer_begin_info{};
	// 		command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	// 		command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	// 		vkBeginCommandBuffer(staging_command_buffer, &command_buffer_begin_info);

	// 		return staging_command_buffer;
	// 	}

	// 	void end_single_use_cmd_buffer(VkCommandBuffer a_command_buffer)
	// 	{
	// 		vkEndCommandBuffer(a_command_buffer);

	// 		VkSubmitInfo staging_submit_info{};
	// 		staging_submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	// 		staging_submit_info.commandBufferCount = 1;
	// 		staging_submit_info.pCommandBuffers    = &a_command_buffer;

	// 		vkQueueSubmit(this->m_transfer_queue, 1, &staging_submit_info, VK_NULL_HANDLE);
	// 		vkQueueWaitIdle(this->m_transfer_queue);        // TODO: Should be improved in the future

	// 		vkFreeCommandBuffers(this->m_device, this->m_transfer_command_pool, 1, &a_command_buffer);
	// 	}

	// 	void copy_from_staging_buffers_to_buffers(std::vector<std::pair<VkBuffer, size_t>> &a_source, std::vector<VkBuffer> &a_destination)
	// 	{
	// 		VkCommandBuffer staging_command_buffer = this->begin_single_use_cmd_buffer();

	// 		// if (a_source.size() != a_destination.size())
	// 		// ror::log_critical("Copying from different size a_source to a_destination, something won't be copied correctly");

	// 		// TODO: Could be done in one go
	// 		for (size_t i = 0; i < a_source.size(); ++i)
	// 		{
	// 			VkBufferCopy buffer_image_copy_region{};
	// 			buffer_image_copy_region.srcOffset = 0;        // Optional
	// 			buffer_image_copy_region.dstOffset = 0;        // Optional
	// 			buffer_image_copy_region.size      = a_source[i].second;

	// 			vkCmdCopyBuffer(staging_command_buffer, a_source[i].first, a_destination[i], 1, &buffer_image_copy_region);
	// 		}

	// 		this->end_single_use_cmd_buffer(staging_command_buffer);
	// 	}

	// 	void transition_image_layout(VkImage a_image, VkFormat a_format, VkImageLayout a_old_layout, VkImageLayout a_new_layout, uint32_t a_mip_levels)
	// 	{
	// 		(void) a_format;

	// 		VkCommandBuffer command_buffer = this->begin_single_use_cmd_buffer();

	// 		VkPipelineStageFlags source_stage;
	// 		VkPipelineStageFlags destination_stage;

	// 		VkImageMemoryBarrier barrier{};
	// 		barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	// 		barrier.oldLayout                       = a_old_layout;
	// 		barrier.newLayout                       = a_new_layout;
	// 		barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	// 		barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	// 		barrier.image                           = a_image;
	// 		barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
	// 		barrier.subresourceRange.baseMipLevel   = 0;
	// 		barrier.subresourceRange.levelCount     = a_mip_levels;
	// 		barrier.subresourceRange.baseArrayLayer = 0;
	// 		barrier.subresourceRange.layerCount     = 1;
	// 		barrier.srcAccessMask                   = 0;        // TODO
	// 		barrier.dstAccessMask                   = 0;        // TODO

	// 		if (a_old_layout == VK_IMAGE_LAYOUT_UNDEFINED && a_new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	// 		{
	// 			barrier.srcAccessMask = 0;
	// 			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	// 			source_stage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	// 			destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	// 		}
	// 		else if (a_old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && a_new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	// 		{
	// 			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	// 			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	// 			source_stage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
	// 			destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	// 		}
	// 		else
	// 		{
	// 			throw std::invalid_argument("unsupported layout transition!");
	// 		}

	// 		vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	// 		this->end_single_use_cmd_buffer(command_buffer);
	// 	}

	// 	// This is taking std::vectors instead of VkBuffer and VkImage so that we only use single cmdbuffer
	// 	void copy_from_staging_buffers_to_images(std::vector<VkBuffer> &a_source, std::vector<VkImage> &a_destination, utl::TextureImage a_texture)
	// 	{
	// 		VkCommandBuffer staging_command_buffer = this->begin_single_use_cmd_buffer();

	// 		// if (a_source.size() != a_destination.size())
	// 		// 	ror::log_critical("Copying from different size a_source to a_destination, something won't be copied correctly");

	// 		// TODO: Could this be done in one go?, i.e. remove this loop?
	// 		for (size_t i = 0; i < a_source.size(); ++i)
	// 		{
	// 			std::vector<VkBufferImageCopy> buffer_image_copy_regions{};

	// 			for (uint32_t j = 0; j < a_texture.get_mip_levels(); j++)
	// 			{
	// 				VkBufferImageCopy buffer_image_copy_region{};

	// 				buffer_image_copy_region.bufferRowLength   = 0;
	// 				buffer_image_copy_region.bufferImageHeight = 0;

	// 				buffer_image_copy_region.bufferOffset                    = a_texture.m_mips[j].m_offset;
	// 				buffer_image_copy_region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
	// 				buffer_image_copy_region.imageSubresource.mipLevel       = j;
	// 				buffer_image_copy_region.imageSubresource.baseArrayLayer = 0;
	// 				buffer_image_copy_region.imageSubresource.layerCount     = 1;
	// 				buffer_image_copy_region.imageOffset                     = {0, 0, 0};
	// 				buffer_image_copy_region.imageExtent                     = {a_texture.m_mips[j].m_width, a_texture.m_mips[j].m_height, 1};

	// 				buffer_image_copy_regions.push_back(buffer_image_copy_region);
	// 			}

	// 			vkCmdCopyBufferToImage(staging_command_buffer, a_source[i], a_destination[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	// 			                       utl::static_cast_safe<uint32_t>(buffer_image_copy_regions.size()), buffer_image_copy_regions.data());
	// 		}

	// 		this->end_single_use_cmd_buffer(staging_command_buffer);
	// 	}

	// 	void create_vertex_buffers()
	// 	{
	// 		constexpr size_t index_buffer_size         = astro_boy_indices_array_count * sizeof(uint32_t);
	// 		constexpr size_t positions_buffer_size     = astro_boy_positions_array_count * sizeof(float32_t);
	// 		constexpr size_t normals_buffer_size       = astro_boy_normals_array_count * sizeof(float32_t);
	// 		constexpr size_t uvs_buffer_size           = astro_boy_uvs_array_count * sizeof(float32_t);
	// 		constexpr size_t weights_buffer_size       = astro_boy_weights_array_count * sizeof(float32_t);
	// 		constexpr size_t joints_buffer_size        = astro_boy_joints_array_count * sizeof(uint32_t);
	// 		constexpr size_t non_positions_buffer_size = normals_buffer_size + uvs_buffer_size + joints_buffer_size + weights_buffer_size;

	// 		std::vector<std::pair<VkBuffer, size_t>> staging_buffers{};
	// 		std::vector<VkDeviceMemory>              staging_buffers_memory{};
	// 		staging_buffers.resize(3);
	// 		staging_buffers_memory.resize(3);

	// 		staging_buffers[0].first = this->create_buffer(positions_buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	// 		staging_buffers[1].first = this->create_buffer(non_positions_buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	// 		staging_buffers[2].first = this->create_buffer(index_buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

	// 		staging_buffers[0].second = positions_buffer_size;
	// 		staging_buffers[1].second = non_positions_buffer_size;
	// 		staging_buffers[2].second = index_buffer_size;

	// 		staging_buffers_memory[0] = this->allocate_bind_buffer_memory(staging_buffers[0].first);        // default of VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	// 		staging_buffers_memory[1] = this->allocate_bind_buffer_memory(staging_buffers[1].first);        // default of VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	// 		staging_buffers_memory[2] = this->allocate_bind_buffer_memory(staging_buffers[2].first);        // default of VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

	// 		uint8_t *position_data;
	// 		vkMapMemory(this->m_device, staging_buffers_memory[0], 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void **>(&position_data));
	// 		memcpy(position_data, astro_boy_positions, positions_buffer_size);        // Positions
	// 		vkUnmapMemory(this->m_device, staging_buffers_memory[0]);

	// 		uint8_t *non_position_data;
	// 		vkMapMemory(this->m_device, staging_buffers_memory[1], 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void **>(&non_position_data));
	// 		memcpy(non_position_data, astro_boy_normals, normals_buffer_size);        // Normals

	// 		non_position_data += normals_buffer_size;
	// 		memcpy(non_position_data, astro_boy_uvs, uvs_buffer_size);        // UVs

	// 		non_position_data += uvs_buffer_size;
	// 		memcpy(non_position_data, astro_boy_weights, weights_buffer_size);        // Weights

	// 		non_position_data += weights_buffer_size;
	// 		memcpy(non_position_data, astro_boy_joints, joints_buffer_size);        // JoinIds

	// 		vkUnmapMemory(this->m_device, staging_buffers_memory[1]);

	// 		uint8_t *index_data;
	// 		vkMapMemory(this->m_device, staging_buffers_memory[2], 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void **>(&index_data));
	// 		memcpy(index_data, astro_boy_indices, index_buffer_size);        // Indices

	// 		vkUnmapMemory(this->m_device, staging_buffers_memory[2]);

	// 		// Here copy from staging buffers into vbo and ibo
	// 		this->m_vertex_buffers[0] = this->create_buffer(positions_buffer_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
	// 		this->m_vertex_buffers[1] = this->create_buffer(non_positions_buffer_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
	// 		this->m_index_buffer      = this->create_buffer(index_buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

	// 		this->m_vertex_buffer_memory[0] = this->allocate_bind_buffer_memory(this->m_vertex_buffers[0], VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	// 		this->m_vertex_buffer_memory[1] = this->allocate_bind_buffer_memory(this->m_vertex_buffers[1], VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	// 		this->m_index_buffer_memory     = this->allocate_bind_buffer_memory(this->m_index_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// 		std::vector<VkBuffer> astro_boy_buffers{this->m_vertex_buffers[0], this->m_vertex_buffers[1], this->m_index_buffer};

	// 		this->copy_from_staging_buffers_to_buffers(staging_buffers, astro_boy_buffers);

	// 		// Cleanup staging buffers
	// 		for (size_t i = 0; i < staging_buffers.size(); ++i)
	// 		{
	// 			vkDestroyBuffer(this->m_device, staging_buffers[i].first, cfg::VkAllocator);
	// 			vkFreeMemory(this->m_device, staging_buffers_memory[i], cfg::VkAllocator);

	// 			staging_buffers[i].first  = nullptr;
	// 			staging_buffers_memory[i] = nullptr;
	// 		}

	// 		this->m_astroboy_bbox.create_from_min_max(ror::Vector3f(astro_boy_bounding_box[0], astro_boy_bounding_box[1], astro_boy_bounding_box[2]),
	// 		                                          ror::Vector3f(astro_boy_bounding_box[3], astro_boy_bounding_box[4], astro_boy_bounding_box[5]));

	// 		ror::glfw_camera_visual_volume(this->m_astroboy_bbox.minimum(), this->m_astroboy_bbox.maximum());
	// 	}

	// 	void destroy_buffers()
	// 	{
	// 		vkDestroyBuffer(this->m_device, this->m_vertex_buffers[0], cfg::VkAllocator);
	// 		vkDestroyBuffer(this->m_device, this->m_vertex_buffers[1], cfg::VkAllocator);
	// 		vkDestroyBuffer(this->m_device, this->m_index_buffer, cfg::VkAllocator);

	// 		vkFreeMemory(this->m_device, this->m_vertex_buffer_memory[0], cfg::VkAllocator);
	// 		vkFreeMemory(this->m_device, this->m_vertex_buffer_memory[1], cfg::VkAllocator);
	// 		vkFreeMemory(this->m_device, this->m_index_buffer_memory, cfg::VkAllocator);

	// 		this->m_vertex_buffers[0] = nullptr;
	// 		this->m_vertex_buffers[1] = nullptr;
	// 		this->m_index_buffer      = nullptr;
	// 	}

	// 	VkImage create_image(uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling, VkImageUsageFlags a_usage, uint32_t a_mip_levels, VkSampleCountFlagBits a_samples_count = VK_SAMPLE_COUNT_1_BIT)
	// 	{
	// 		VkImageCreateInfo image_info{};
	// 		image_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	// 		image_info.imageType     = VK_IMAGE_TYPE_2D;
	// 		image_info.extent.width  = a_width;
	// 		image_info.extent.height = a_height;
	// 		image_info.extent.depth  = 1;
	// 		image_info.mipLevels     = a_mip_levels;
	// 		image_info.arrayLayers   = 1;
	// 		image_info.format        = a_format;
	// 		image_info.tiling        = a_tiling;
	// 		image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	// 		image_info.usage         = a_usage;
	// 		image_info.samples       = a_samples_count;        // VK_SAMPLE_COUNT_1_BIT;
	// 		image_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
	// 		image_info.flags         = 0;        // Optional

	// 		VkImage  image;
	// 		VkResult result = vkCreateImage(this->m_device, &image_info, nullptr, &image);
	// 		assert(result == VK_SUCCESS);

	// 		return image;
	// 	}

	// 	void destroy_image(VkImage a_image)
	// 	{
	// 		vkDestroyImage(this->m_device, a_image, cfg::VkAllocator);
	// 	}

	// 	// TODO: This and allocate_bind_buffer_memory() should be one function
	// 	auto allocate_bind_image_memory(VkImage a_image, VkMemoryPropertyFlags a_properties)
	// 	{
	// 		VkMemoryRequirements image_mem_requirements;
	// 		vkGetImageMemoryRequirements(this->m_device, a_image, &image_mem_requirements);

	// 		VkMemoryAllocateInfo allocation_info{};
	// 		allocation_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	// 		allocation_info.pNext           = nullptr;
	// 		allocation_info.allocationSize  = image_mem_requirements.size;
	// 		allocation_info.memoryTypeIndex = find_memory_type(image_mem_requirements.memoryTypeBits, a_properties);

	// 		VkDeviceMemory image_memory;
	// 		VkResult       result = vkAllocateMemory(this->m_device, &allocation_info, cfg::VkAllocator, &image_memory);
	// 		assert(result == VK_SUCCESS && "Failed to allocate vulkan image memory!");
	// 		assert(image_memory);

	// 		result = vkBindImageMemory(this->m_device, a_image, image_memory, 0);

	// 		assert(result == VK_SUCCESS && "Failed to bind vulkan image memory!");

	// 		return image_memory;
	// 	}

	// 	void create_texture()
	// 	{
	// 		utl::TextureImage texture = utl::read_texture_from_file("./assets/astroboy/astro_boy_uastc.ktx2");
	// 		// Texture texture = utl::read_texture_from_file("./assets/astroboy/astro_boy.jpg");

	// 		VkBuffer       staging_buffer{};
	// 		VkDeviceMemory staging_buffer_memory{};

	// 		staging_buffer        = this->create_buffer(texture.m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	// 		staging_buffer_memory = this->allocate_bind_buffer_memory(staging_buffer);

	// 		uint8_t *texture_data;
	// 		vkMapMemory(this->m_device, staging_buffer_memory, 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void **>(&texture_data));
	// 		memcpy(texture_data, texture.m_data.data(), texture.m_size);
	// 		vkUnmapMemory(this->m_device, staging_buffer_memory);

	// 		this->m_texture_image        = this->create_image(texture.get_width(), texture.get_height(), texture.get_format(), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, texture.get_mip_levels());
	// 		this->m_texture_image_memory = this->allocate_bind_image_memory(this->m_texture_image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	// 		this->m_texture_image_view   = this->create_image_view(this->m_texture_image, texture.get_format(), VK_IMAGE_ASPECT_COLOR_BIT, texture.get_mip_levels());

	// 		std::vector<VkImage>  texture_images{this->m_texture_image};
	// 		std::vector<VkBuffer> source_textures{staging_buffer};

	// 		this->transition_image_layout(this->m_texture_image, texture.get_format(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, texture.get_mip_levels());
	// 		this->copy_from_staging_buffers_to_images(source_textures, texture_images, texture);
	// 		this->transition_image_layout(this->m_texture_image, texture.get_format(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, texture.get_mip_levels());
	// 		this->create_texture_sampler(static_cast<float32_t>(texture.get_mip_levels()));

	// 		// Cleanup staging buffers
	// 		vkDestroyBuffer(this->m_device, staging_buffer, cfg::VkAllocator);
	// 		vkFreeMemory(this->m_device, staging_buffer_memory, cfg::VkAllocator);

	// 		staging_buffer        = nullptr;
	// 		staging_buffer_memory = nullptr;
	// 	}

	// 	void destroy_texture()
	// 	{
	// 		vkDestroyImageView(this->m_device, this->m_texture_image_view, cfg::VkAllocator);

	// 		this->destroy_image(this->m_texture_image);
	// 		vkFreeMemory(this->m_device, this->m_texture_image_memory, cfg::VkAllocator);
	// 	}

	// 	void create_texture_sampler(float a_mip_levels)
	// 	{
	// 		VkSamplerCreateInfo sampler_info{};

	// 		sampler_info.sType     = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	// 		sampler_info.magFilter = VK_FILTER_LINEAR;
	// 		sampler_info.minFilter = VK_FILTER_LINEAR;

	// 		sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	// 		sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	// 		sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	// 		sampler_info.anisotropyEnable = VK_FALSE;
	// 		sampler_info.maxAnisotropy    = 1.0f;

	// 		sampler_info.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	// 		sampler_info.unnormalizedCoordinates = VK_FALSE;

	// 		sampler_info.compareEnable = VK_FALSE;
	// 		sampler_info.compareOp     = VK_COMPARE_OP_ALWAYS;
	// 		sampler_info.mipmapMode    = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	// 		sampler_info.mipLodBias    = 0.0f;
	// 		sampler_info.minLod        = 0.0f;
	// 		sampler_info.maxLod        = a_mip_levels;

	// 		VkResult result = vkCreateSampler(this->m_device, &sampler_info, nullptr, &this->m_texture_sampler);
	// 		assert(result == VK_SUCCESS);
	// 	}

	// 	VkSampleCountFlagBits get_sample_count()
	// 	{
	// 		// Get color and depth sample count flags
	// 		VkSampleCountFlags counts = this->m_physical_device_properties.limits.framebufferColorSampleCounts &
	// 		                            this->m_physical_device_properties.limits.framebufferDepthSampleCounts;

	// 		VkSampleCountFlagBits required = static_cast<VkSampleCountFlagBits>(cfg::get_multisample_count());        // FIXME: Dangerous, if vulkan header changes or sample count isn't 2's power

	// 		// Return if required available otherwise go lower until an alternative is available
	// 		do
	// 		{
	// 			if (counts & required)
	// 				return required;

	// 			required = static_cast<VkSampleCountFlagBits>(required >> 2);

	// 		} while (required);

	// 		// No choice but to return no MSAA
	// 		return VK_SAMPLE_COUNT_1_BIT;
	// 	}

	// 	void destroy_texture_sampler()
	// 	{
	// 		vkDestroySampler(this->m_device, this->m_texture_sampler, cfg::VkAllocator);
	// 	}

	// 	void destroy_render_pass()
	// 	{
	// 		vkDestroyRenderPass(this->m_device, this->m_render_pass, cfg::VkAllocator);
	// 		this->m_render_pass = nullptr;
	// 	}

	// 	void cleanup_swapchain()
	// 	{
	// 		// TODO: Expolore how does the 'oldSwapChain' argument works to be more efficient
	// 		this->destroy_framebuffers();

	// 		// Rather than destroying command pool we are destroying command buffers themselves, TODO: I thought clearing the pool was faster
	// 		vkFreeCommandBuffers(this->m_device, this->m_graphics_command_pool, static_cast<uint32_t>(this->m_graphics_command_buffers.size()), this->m_graphics_command_buffers.data());
	// 		this->m_graphics_command_buffers.clear();

	// 		this->destroy_render_pass();
	// 		this->destroy_graphics_pipeline();
	// 		this->destroy_imageviews();
	// 		this->destroy_depth_buffer();
	// 		this->destroy_msaa_color_buffer();
	// 		this->destroy_swapchain();
	// 	}

	// 	void create_depth_buffer()
	// 	{
	// 		// TODO: Called multiple times, should be cached
	// 		VkSampleCountFlagBits samples = this->get_sample_count();

	// 		VkFormat depth_format      = VK_FORMAT_D24_UNORM_S8_UINT;        // TODO: Make more generic and flexible
	// 		this->m_depth_image        = this->create_image(this->m_swapchain_extent.width, this->m_swapchain_extent.height, VK_FORMAT_D24_UNORM_S8_UINT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 1, samples);
	// 		this->m_depth_image_memory = this->allocate_bind_image_memory(this->m_depth_image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	// 		this->m_depth_image_view   = this->create_image_view(this->m_depth_image, depth_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	// 	}

	// 	void create_msaa_color_buffer()
	// 	{
	// 		// TODO: Called multiple times, should be cached
	// 		VkSampleCountFlagBits samples = this->get_sample_count();

	// 		this->m_msaa_color_image        = this->create_image(this->m_swapchain_extent.width, this->m_swapchain_extent.height, this->m_swapchain_format, VK_IMAGE_TILING_OPTIMAL,
	// 		                                                     VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, 1, samples);
	// 		this->m_msaa_color_image_memory = this->allocate_bind_image_memory(this->m_msaa_color_image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	// 		this->m_msaa_color_image_view   = this->create_image_view(this->m_msaa_color_image, this->m_swapchain_format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	// 	}

	// 	void destroy_msaa_color_buffer()
	// 	{
	// 		this->destroy_image(this->m_msaa_color_image);
	// 		this->destroy_imageview(this->m_msaa_color_image_view);
	// 		vkFreeMemory(this->m_device, this->m_msaa_color_image_memory, cfg::VkAllocator);
	// 	}

	// 	void destroy_depth_buffer()
	// 	{
	// 		this->destroy_image(this->m_depth_image);
	// 		this->destroy_imageview(this->m_depth_image_view);
	// 		vkFreeMemory(this->m_device, this->m_depth_image_memory, cfg::VkAllocator);
	// 	}

	VkPhysicalDeviceProperties m_physical_device_properties;
	// 	std::vector<VkFramebuffer>   m_framebuffers;
	// 	std::vector<VkCommandBuffer> m_graphics_command_buffers;
	// 	std::vector<VkCommandBuffer> m_compute_command_buffers;
	// 	std::vector<VkCommandBuffer> m_transfer_command_buffers;
	// 	VkPipeline                   m_graphics_pipeline{nullptr};
	// 	VkPipelineLayout             m_pipeline_layout{nullptr};
	// 	VkDescriptorSetLayout        m_descriptor_set_layout{nullptr};
	// 	VkDescriptorPool             m_descriptor_pool{nullptr};
	// 	std::vector<VkDescriptorSet> m_descriptor_sets{cfg::get_number_of_buffers()};
	// 	VkPipelineCache              m_pipeline_cache{nullptr};
	// 	VkRenderPass                 m_render_pass{nullptr};
	// 	VkCommandPool                m_graphics_command_pool{nullptr};
	// 	VkCommandPool                m_transfer_command_pool{nullptr};
	// 	VkSemaphore                  m_image_available_semaphore[cfg::get_number_of_buffers()];
	// 	VkSemaphore                  m_render_finished_semaphore[cfg::get_number_of_buffers()];
	// 	VkFence                      m_queue_fence[cfg::get_number_of_buffers()];
	// 	VkFence                      m_queue_fence_in_flight[cfg::get_number_of_buffers()];
	// 	uint32_t                     m_current_frame{0};
	// 	VkBuffer                     m_vertex_buffers[2];                                           // Temporary buffers for Astro_boy geometry
	// 	VkBuffer                     m_index_buffer{nullptr};                                       // Temporary buffers for Astro_boy geometry
	// 	VkDeviceMemory               m_vertex_buffer_memory[2];                                     // Temporary vertex memory buffers for Astro_boy geometry
	// 	VkDeviceMemory               m_index_buffer_memory{nullptr};                                // Temporary index memory buffers for Astro_boy geometry
	// 	std::vector<VkBuffer>        m_uniform_buffers{cfg::get_number_of_buffers()};               // Uniforms buffers for all frames in flight
	// 	std::vector<VkDeviceMemory>  m_uniform_buffers_memory{cfg::get_number_of_buffers()};        // Uniforms buffers memory for all frames in flight
	// 	VkImage                      m_msaa_color_image{nullptr};                                   // Color image used for unresolved MSAA RT
	// 	VkDeviceMemory               m_msaa_color_image_memory{nullptr};
	// 	VkImageView                  m_msaa_color_image_view{nullptr};
	// 	VkImage                      m_depth_image{nullptr};
	// 	VkImageView                  m_depth_image_view{nullptr};
	// 	VkDeviceMemory               m_depth_image_memory{nullptr};
	// 	VkImage                      m_texture_image{nullptr};
	// 	VkDeviceMemory               m_texture_image_memory{nullptr};
	// 	VkImageView                  m_texture_image_view{nullptr};
	// 	VkSampler                    m_texture_sampler{nullptr};
	// 	cfg::BoundingBox             m_astroboy_bbox{};

};        // namespace vkd

class ROAR_ENGINE_ITEM SwapChain final
{
  public:
	FORCE_INLINE            SwapChain()                             = default;        //! Default constructor
	FORCE_INLINE            SwapChain(const SwapChain &a_other)     = default;        //! Copy constructor
	FORCE_INLINE            SwapChain(SwapChain &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE SwapChain &operator=(const SwapChain &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE SwapChain &operator=(SwapChain &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~SwapChain() noexcept                      = default;        //! Destructor

	void create(VkPhysicalDevice a_physical_device, VkDevice a_device, VkSurfaceKHR a_surface, VkFormat swapchain_format, VkExtent2D a_swapchain_extent);
	void release(VkDevice a_device);

	// clang-format off
	void swapchain(VkSwapchainKHR a_swapchain)                      { this->m_swapchain = a_swapchain;           }
	void swapchain_images(std::vector<VkImage> &&a_swapchains)      { this->m_swapchain_images = a_swapchains;   } 
	void swapchain_images_views(std::vector<VkImageView> &&a_views) { this->m_swapchain_images_views = a_views;  }
	void format(VkFormat a_format)                                  { this->m_format = a_format;                 }

	VkSwapchainKHR            swapchain()                           { return this->m_swapchain;                  }
	std::vector<VkImage>     &swapchain_images()                    { return this->m_swapchain_images;           }
	std::vector<VkImageView> &swapchain_images_views()              { return this->m_swapchain_images_views;     }
	VkFormat                  format()                              { return this->m_format;                     }
	void                      release()                             {}
	// clang-format on

	declare_translation_unit_vtable();

  protected:
  private:
	VkSwapchainKHR           m_swapchain{nullptr};
	std::vector<VkImage>     m_swapchain_images;
	std::vector<VkImageView> m_swapchain_images_views;
	VkFormat                 m_format{VK_FORMAT_B8G8R8A8_SRGB};
};

using Swapchain = SwapChain *;

class DeviceVulkan : public DeviceCrtp<DeviceVulkan>
{
  public:
	FORCE_INLINE               DeviceVulkan()                                = default;        //! Default constructor
	FORCE_INLINE               DeviceVulkan(const DeviceVulkan &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               DeviceVulkan(DeviceVulkan &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE DeviceVulkan &operator=(const DeviceVulkan &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE DeviceVulkan &operator=(DeviceVulkan &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE virtual ~DeviceVulkan() noexcept override                   = default;        //! Destructor

	FORCE_INLINE void            init(std::any a_platform_window, void *a_window, ror::EventSystem &a_event_system, ror::Vector2ui a_dimensions);
	FORCE_INLINE VkDevice        platform_device();
	FORCE_INLINE VkQueue         platform_queue();
	FORCE_INLINE VkCommandBuffer platform_command_buffer();
	FORCE_INLINE Swapchain       platform_swapchain();
	FORCE_INLINE void            destory_surface();
	FORCE_INLINE void            destroy_device();

	void create_surface(void *a_window);
	void create_device();

  protected:
  private:
	Instance       m_instance{};
	PhysicalDevice m_gpu{};
	VkDevice       m_device{nullptr};         //! Vulkan device
	VkSurfaceKHR   m_surface{nullptr};        //! Vulkan drawable surface provided by windowing system
	VkQueue        m_command_queue;           //! CommandQueue where all the commands will be submitted, I think I only need one
	std::any       m_window{};                //! Platform window, on Metal its NSWindow while on Vulkan its GLFWwindow
	SwapChain      m_swapchain{};             //! Swapchain images abstraction for Vulkan

	uint32_t m_graphics_queue_index{0};
	uint32_t m_present_queue_index{0};
	uint32_t m_transfer_queue_index{0};
	uint32_t m_compute_queue_index{0};
	VkQueue  m_graphics_queue{nullptr};
	VkQueue  m_compute_queue{nullptr};
	VkQueue  m_transfer_queue{nullptr};
	VkQueue  m_present_queue{nullptr};
	VkQueue  m_sparse_queue{nullptr};
	VkQueue  m_protected_queue{nullptr};

	declare_translation_unit_vtable();
};

declare_rhi_render_type(Device);

}        // namespace rhi

#include "rhi/vulkan/rordevice.hh"
