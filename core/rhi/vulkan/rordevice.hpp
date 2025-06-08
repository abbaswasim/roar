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
#include "rhi/vulkan/rorcommand_pool.hpp"
#include "rhi/vulkan/rordescriptor_cache.hpp"
#include "rhi/vulkan/rordescriptor_pool.hpp"
#include "rhi/vulkan/rordescriptor_set.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_object.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include <any>
#include <cstddef>
#include <typeindex>
#include <vector>

namespace ror
{
class Renderer;
}

namespace rhi
{

class VulkanValidationException : public std::exception
{
  private:
	std::string m_message;

  public:
	VulkanValidationException(const char *a_message) :
	    m_message(a_message)
	{}

	declare_translation_unit_vtable();

	const char *what() const noexcept override
	{
		return m_message.c_str();
	}
};

class Instance : public VulkanObject<VkInstance>
{
  public:
	FORCE_INLINE           Instance(const Instance &a_other)      = delete;        //! Copy constructor
	FORCE_INLINE           Instance(Instance &&a_other) noexcept  = delete;        //! Move constructor
	FORCE_INLINE Instance &operator=(const Instance &a_other)     = delete;        //! Copy assignment operator
	FORCE_INLINE Instance &operator=(Instance &&a_other) noexcept = delete;        //! Move assignment operator
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
	FORCE_INLINE virtual ~PhysicalDevice() noexcept override                       = default;

	FORCE_INLINE auto &memory_properties() const noexcept;
	FORCE_INLINE auto  samples_count() const noexcept;

	declare_translation_unit_vtable();

	void init(Instance &a_instance);

  protected:
  private:
	VkPhysicalDeviceProperties        m_physical_device_properties{};        //! Physical device properties cache
	VkPhysicalDeviceMemoryProperties2 m_memory_properties{};                 //! Vulkan physical device memory properties cache, it might also contain VkPhysicalDeviceMemoryBudgetPropertiesEXT in pNext*
};

class RenderpassVulkan;

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
	void setup_framebuffer(VkDevice a_device, rhi::RenderpassVulkan *a_renderpass);
	void release(VkDevice a_device);

	// clang-format off
	void swapchain(VkSwapchainKHR a_swapchain)                      { this->m_swapchain = a_swapchain;           }
	void swapchain_images(std::vector<VkImage> &&a_swapchains)      { this->m_swapchain_images = a_swapchains;   } 
	void swapchain_images_views(std::vector<VkImageView> &&a_views) { this->m_swapchain_images_views = a_views;  }
	void format(VkFormat a_format)                                  { this->m_format = a_format;                 }

	VkSwapchainKHR              swapchain()                         { return this->m_swapchain;                  }
	std::vector<VkImage>       &swapchain_images()                  { return this->m_swapchain_images;           }
	std::vector<VkImageView>   &swapchain_images_views()            { return this->m_swapchain_images_views;     }
	std::vector<VkFramebuffer> &swapchain_framebuffers()            { return this->m_framebuffers;               }
	VkFormat                    format()                            { return this->m_format;                     }
	void                        release()                           {}
	// clang-format on

	declare_translation_unit_vtable();

  protected:
  private:
	VkSwapchainKHR             m_swapchain{nullptr};                     //! The platform swapchain handle
	VkExtent2D                 m_extent{};                               //! The size of the swapchain, could be different that what's requested
	std::vector<VkImage>       m_swapchain_images{};                     //! Image enumurated from swapchain
	std::vector<VkImageView>   m_swapchain_images_views{};               //! Image views created from images which are enumurated from swapchain
	VkFormat                   m_format{VK_FORMAT_B8G8R8A8_SRGB};        //! Format of the swapchain, might end up different depending on what's available
	std::vector<VkFramebuffer> m_framebuffers{};                         //! These should be as many as the amount of images/image views in the swapchain
};

using Swapchain = SwapChain *;

struct DeviceQueue
{
	uint32_t           m_index{0};
	VkQueue            m_queue{nullptr};
	mutable std::mutex m_mutex{};        //! These will be problematic when I have only one queue, FIXME:
};

class DeviceVulkan : public DeviceCrtp<DeviceVulkan>
{
  public:
	FORCE_INLINE               DeviceVulkan()                                = default;        //! Default constructor
	FORCE_INLINE               DeviceVulkan(const DeviceVulkan &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE               DeviceVulkan(DeviceVulkan &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE DeviceVulkan &operator=(const DeviceVulkan &a_other)        = delete;         //! Copy assignment operator
	FORCE_INLINE DeviceVulkan &operator=(DeviceVulkan &&a_other) noexcept    = delete;         //! Move assignment operator
	FORCE_INLINE virtual ~DeviceVulkan() noexcept override                   = default;        //! Destructor

	FORCE_INLINE void  init(std::any a_platform_window, void *a_window, ror::EventSystem &a_event_system, ror::Vector2ui a_dimensions);
	FORCE_INLINE void  shutdown();
	void  swapchain_setup(ror::Renderer *a_renderer);
	FORCE_INLINE auto  platform_device() const noexcept;
	FORCE_INLINE auto  platform_graphics_queue() const noexcept;
	FORCE_INLINE auto  platform_compute_queue() const noexcept;
	FORCE_INLINE auto  platform_transfer_queue() const noexcept;
	FORCE_INLINE auto  platform_present_queue() const noexcept;
	FORCE_INLINE auto  platform_sparse_queue() const noexcept;
	FORCE_INLINE auto  platform_protected_queue() const noexcept;
	FORCE_INLINE auto  platform_graphics_queue_index() const noexcept;
	FORCE_INLINE auto  platform_compute_queue_index() const noexcept;
	FORCE_INLINE auto  platform_transfer_queue_index() const noexcept;
	FORCE_INLINE auto  platform_present_queue_index() const noexcept;
	FORCE_INLINE auto  platform_sparse_queue_index() const noexcept;
	FORCE_INLINE auto  platform_protected_queue_index() const noexcept;
	FORCE_INLINE auto  platform_graphics_command_buffer();
	FORCE_INLINE auto  platform_compute_command_buffer();
	FORCE_INLINE auto  platform_transfer_command_buffer();
	FORCE_INLINE auto  platform_swapchain();
	FORCE_INLINE auto  samples_count() const noexcept;
	FORCE_INLINE auto &memory_properties() const noexcept;
	FORCE_INLINE auto &platform_graphics_queue_mutex() const noexcept;
	FORCE_INLINE auto &platform_compute_queue_mutex() const noexcept;
	FORCE_INLINE auto &platform_transfer_queue_mutex() const noexcept;
	FORCE_INLINE auto &platform_present_queue_mutex() const noexcept;
	FORCE_INLINE auto &platform_sparse_queue_mutex() const noexcept;
	FORCE_INLINE auto &platform_protected_queue_mutex() const noexcept;
	FORCE_INLINE auto  platform_pipeline_cache() const noexcept;

	FORCE_INLINE DescriptorSetLayoutCache &descriptor_set_layout_cache() const noexcept;
	FORCE_INLINE DescriptorSetCache       &descriptor_set_cache() const noexcept;
	FORCE_INLINE DescriptorPool           &descriptor_set_pool() const noexcept;
	FORCE_INLINE VkCommandBuffer           graphics_command_buffer() const noexcept;
	FORCE_INLINE VkCommandBuffer           compute_command_buffer() const noexcept;
	FORCE_INLINE VkCommandBuffer           transfer_command_buffer() const noexcept;

  protected:
  private:
	FORCE_INLINE void destory_surface();
	FORCE_INLINE void destroy_device();

	void create_surface(void *a_window);
	void create_device();

	Instance        m_instance{};                     //! Instance from where all devices are created
	PhysicalDevice  m_gpu{};                          //! Physical device in the system
	VkDevice        m_device{nullptr};                //! Vulkan device
	VkSurfaceKHR    m_surface{nullptr};               //! Vulkan drawable surface provided by windowing system
	std::any        m_window{};                       //! Platform window, on Metal its NSWindow while on Vulkan its GLFWwindow
	SwapChain       m_swapchain{};                    //! Swapchain images abstraction for Vulkan
	DeviceQueue     m_graphics_queue{};               //! Graphics queue
	DeviceQueue     m_compute_queue{};                //! Compute queue
	DeviceQueue     m_transfer_queue{};               //! Tranfer queue
	DeviceQueue     m_present_queue{};                //! Present queue
	DeviceQueue     m_sparse_queue{};                 //! Sparse queue
	DeviceQueue     m_protected_queue{};              //! Protected queue
	VkPipelineCache m_pipeline_cache{nullptr};        //! Don't need a mutex because its internally synchronised

	mutable DescriptorSetLayoutCache m_layout_cache{};                 //! Layout cache used to create all lyouts from
	mutable DescriptorSetCache       m_desciptor_cache{};              //! Descriptor set cache used by all PSOs
	mutable DescriptorPool           m_desciptor_pool{};               //! Descriptor pool used for allocating descriptors from
	mutable CommandPool              m_graphics_command_pool{};        //! Command buffer pool used for allocating command buffers from for graphics
	mutable CommandPool              m_compute_command_pool{};         //! Command buffer pool used for allocating command buffers from for compute
	mutable CommandPool              m_transfer_command_pool{};        //! Command buffer pool used for allocating command buffers from for transfer

	declare_translation_unit_vtable();
};

declare_rhi_render_type(Device);

}        // namespace rhi

#include "rhi/vulkan/rordevice.hh"
