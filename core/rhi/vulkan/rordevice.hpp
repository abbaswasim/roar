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
#include <cstddef>
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
	FORCE_INLINE virtual ~PhysicalDevice() noexcept override                       = default;

	FORCE_INLINE auto &memory_properties();

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

		this->m_memory_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
		this->m_memory_properties.pNext = nullptr;

		vkGetPhysicalDeviceMemoryProperties2(this->get_handle(), &this->m_memory_properties);
	}

  protected:
  private:
	VkPhysicalDeviceProperties        m_physical_device_properties{};        //! Physical device properties cache
	VkPhysicalDeviceMemoryProperties2 m_memory_properties{};                 //! Vulkan physical device memory properties cache, it might also contain VkPhysicalDeviceMemoryBudgetPropertiesEXT in pNext*
};

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
	FORCE_INLINE VkQueue         platform_graphics_queue();
	FORCE_INLINE VkQueue         platform_compute_queue();
	FORCE_INLINE VkQueue         platform_present_queue();
	FORCE_INLINE VkQueue         platform_transfer_queue();
	FORCE_INLINE uint32_t        platform_graphics_queue_index();
	FORCE_INLINE uint32_t        platform_compute_queue_index();
	FORCE_INLINE uint32_t        platform_present_queue_index();
	FORCE_INLINE uint32_t        platform_transfer_queue_index();
	FORCE_INLINE VkCommandPool   platform_graphics_command_pool();
	FORCE_INLINE VkCommandPool   platform_compute_command_pool();
	FORCE_INLINE VkCommandPool   platform_transfer_command_pool();
	FORCE_INLINE VkCommandBuffer platform_command_buffer();
	FORCE_INLINE Swapchain       platform_swapchain();
	FORCE_INLINE auto           &memory_properties();

  protected:
  private:
	FORCE_INLINE void destory_surface();
	FORCE_INLINE void destroy_device();

	void create_surface(void *a_window);
	void create_device();
	void create_command_pools();

	Instance       m_instance{};
	PhysicalDevice m_gpu{};
	VkDevice       m_device{nullptr};         //! Vulkan device
	VkSurfaceKHR   m_surface{nullptr};        //! Vulkan drawable surface provided by windowing system
	std::any       m_window{};                //! Platform window, on Metal its NSWindow while on Vulkan its GLFWwindow
	SwapChain      m_swapchain{};             //! Swapchain images abstraction for Vulkan

	uint32_t      m_graphics_queue_index{0};
	uint32_t      m_present_queue_index{0};
	uint32_t      m_transfer_queue_index{0};
	uint32_t      m_compute_queue_index{0};
	VkQueue       m_graphics_queue{nullptr};
	VkQueue       m_compute_queue{nullptr};
	VkQueue       m_transfer_queue{nullptr};
	VkQueue       m_present_queue{nullptr};
	VkQueue       m_sparse_queue{nullptr};
	VkQueue       m_protected_queue{nullptr};
	VkCommandPool m_graphics_command_pool{nullptr};
	VkCommandPool m_compute_command_pool{nullptr};
	VkCommandPool m_transfer_command_pool{nullptr};

	declare_translation_unit_vtable();
};

declare_rhi_render_type(Device);

}        // namespace rhi

#include "rhi/vulkan/rordevice.hh"
