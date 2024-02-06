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

#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "math/rorvector2.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/vulkan/rordescriptor_cache.hpp"
#include "rhi/vulkan/rordescriptor_set.hpp"
#include "rhi/vulkan/rordevice.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include "settings/rorsettings.hpp"
#include <any>

namespace rhi
{

FORCE_INLINE void *resize_ca_vulkan_layer(std::any a_window, VkDevice a_device, ror::Vector2ui a_window_dimensions, uint32_t a_pixel_format)
{
	(void) a_window;
	(void) a_device;
	(void) a_window_dimensions;
	(void) a_pixel_format;

	void *ca_vulkan_layer = {nullptr};        // get_vulkan_layer(a_window,
	//  static_cast<float32_t>(a_window_dimensions.x),
	//	                                       static_cast<float32_t>(a_window_dimensions.y),
	//	                                       a_device,
	//	                                       a_pixel_format);

	if (!ca_vulkan_layer)
	{
		ror::log_critical("Can't acquire CA Vulkan layer");
		exit(1);
	}

	assert(0);
	return ca_vulkan_layer;
}

// This is not inside the ctor above because by the time Application ctor chain is finished the window in UnixApp is not initialized yet
FORCE_INLINE void DeviceVulkan::init(std::any a_platform_window, void *a_window, ror::EventSystem &a_event_system, ror::Vector2ui a_dimensions)
{
	this->m_gpu.init(this->m_instance);

	this->create_surface(a_window);
	this->create_device();

	if (!this->m_device)
	{
		ror::log_critical("Can't create vulkan device");
		exit(1);
	}

	this->m_window = a_platform_window;
	auto &settings = ror::settings();

	auto resize_callback = [this, a_platform_window, &settings](ror::Event &a_event) {
		auto size         = a_event.get_payload<ror::Vector2ui>();
		auto pixel_format = to_vulkan_pixelformat(settings.m_window.m_pixel_format);

		this->m_swapchain.release(this->m_device);
		this->m_swapchain.create(this->m_gpu.get_handle(), this->m_device, this->m_surface, pixel_format, VkExtent2D{size.x, size.y});
	};

	a_event_system.subscribe(ror::buffer_resize, resize_callback);

	ror::Event e{ror::buffer_resize, true, ror::Vector2ui{a_dimensions.x, a_dimensions.y}};
	resize_callback(e);
}

FORCE_INLINE auto DeviceVulkan::platform_device() const noexcept
{
	assert(this->m_device && "Vulkan device requested is null");

	return this->m_device;
}

FORCE_INLINE auto DeviceVulkan::platform_pipeline_cache() const noexcept
{
	assert(this->m_pipeline_cache && "Vulkan pipeline cache requested is null");

	return this->m_pipeline_cache;
}

FORCE_INLINE DescriptorSetLayoutCache &DeviceVulkan::descriptor_set_layout_cache() const noexcept
{
	return this->m_layout_cache;
}

FORCE_INLINE DescriptorSetCache &DeviceVulkan::descriptor_set_cache() const noexcept
{
	return this->m_desciptor_cache;
}

FORCE_INLINE DescriptorPool &DeviceVulkan::descriptor_set_pool() const noexcept
{
	return this->m_desciptor_pool;
}

FORCE_INLINE auto DeviceVulkan::platform_graphics_queue() const noexcept
{
	assert(this->m_graphics_queue.m_queue && "Vulkan graphics queue requested is null");

	return this->m_graphics_queue.m_queue;
}

FORCE_INLINE auto DeviceVulkan::platform_compute_queue() const noexcept
{
	assert(this->m_compute_queue.m_queue && "Vulkan compute queue requested is null");

	return this->m_compute_queue.m_queue;
}

FORCE_INLINE auto DeviceVulkan::platform_transfer_queue() const noexcept
{
	assert(this->m_transfer_queue.m_queue && "Vulkan transfer queue requested is null");

	return this->m_transfer_queue.m_queue;
}

FORCE_INLINE auto DeviceVulkan::platform_present_queue() const noexcept
{
	assert(this->m_present_queue.m_queue && "Vulkan present queue requested is null");

	return this->m_present_queue.m_queue;
}

FORCE_INLINE auto DeviceVulkan::platform_sparse_queue() const noexcept
{
	assert(this->m_sparse_queue.m_queue && "Vulkan sparse queue requested is null");

	return this->m_sparse_queue.m_queue;
}

FORCE_INLINE auto DeviceVulkan::platform_protected_queue() const noexcept
{
	assert(this->m_protected_queue.m_queue && "Vulkan protected queue requested is null");

	return this->m_protected_queue.m_queue;
}

FORCE_INLINE auto DeviceVulkan::platform_graphics_queue_index() const noexcept
{
	assert(this->m_graphics_queue.m_queue && "Vulkan graphics queue requested is null");

	return this->m_graphics_queue.m_index;
}

FORCE_INLINE auto DeviceVulkan::platform_compute_queue_index() const noexcept
{
	assert(this->m_compute_queue.m_queue && "Vulkan compute queue requested is null");

	return this->m_compute_queue.m_index;
}

FORCE_INLINE auto DeviceVulkan::platform_transfer_queue_index() const noexcept
{
	assert(this->m_transfer_queue.m_queue && "Vulkan transfer queue requested is null");

	return this->m_transfer_queue.m_index;
}

FORCE_INLINE auto DeviceVulkan::platform_present_queue_index() const noexcept
{
	assert(this->m_present_queue.m_queue && "Vulkan present queue requested is null");

	return this->m_present_queue.m_index;
}

FORCE_INLINE auto DeviceVulkan::platform_sparse_queue_index() const noexcept
{
	assert(this->m_sparse_queue.m_queue && "Vulkan sparse queue requested is null");

	return this->m_sparse_queue.m_index;
}

FORCE_INLINE auto DeviceVulkan::platform_protected_queue_index() const noexcept
{
	assert(this->m_protected_queue.m_queue && "Vulkan protected queue requested is null");

	return this->m_protected_queue.m_index;
}

FORCE_INLINE auto &DeviceVulkan::platform_graphics_queue_mutex() const noexcept
{
	return this->m_graphics_queue.m_mutex;
}

FORCE_INLINE auto &DeviceVulkan::platform_compute_queue_mutex() const noexcept
{
	return this->m_compute_queue.m_mutex;
}

FORCE_INLINE auto &DeviceVulkan::platform_transfer_queue_mutex() const noexcept
{
	return this->m_transfer_queue.m_mutex;
}

FORCE_INLINE auto &DeviceVulkan::platform_present_queue_mutex() const noexcept
{
	return this->m_present_queue.m_mutex;
}

FORCE_INLINE auto &DeviceVulkan::platform_sparse_queue_mutex() const noexcept
{
	return this->m_sparse_queue.m_mutex;
}

FORCE_INLINE auto &DeviceVulkan::platform_protected_queue_mutex() const noexcept
{
	return this->m_protected_queue.m_mutex;
}

FORCE_INLINE auto DeviceVulkan::platform_command_buffer()
{
	VkCommandBuffer buffer{nullptr};

	if constexpr (ror::get_build() == ror::BuildType::build_debug)
	{
		// VkCommandBuffer command_buffer_descriptor = nullptr;// MTL::CommandBufferDescriptor::alloc()->init();

		// command_buffer_descriptor->setErrorOptions(MTL::CommandBufferErrorOptionEncoderExecutionStatus);

		// buffer = this->platform_queue()->commandBuffer(command_buffer_descriptor);

		// command_buffer_descriptor->release();
	}
	else
	{
		// buffer = this->platform_queue()->commandBuffer();
	}

	assert(0);
	return buffer;
}

FORCE_INLINE auto DeviceVulkan::platform_swapchain()
{
	assert(this->m_swapchain.swapchain() && "Vulkan swapchain is null, can't create swapchain");

	return &this->m_swapchain;
}

FORCE_INLINE void DeviceVulkan::destory_surface()
{
	vk_destroy_surface(this->m_instance.get_handle(), this->m_surface);
}

FORCE_INLINE void DeviceVulkan::destroy_device()
{
	vk_destroy_device(this->m_device);
}

FORCE_INLINE auto &PhysicalDevice::memory_properties() const noexcept
{
	return this->m_memory_properties;
}

FORCE_INLINE auto PhysicalDevice::samples_count() const noexcept
{
	return get_sample_count(this->m_physical_device_properties);
}

FORCE_INLINE auto &DeviceVulkan::memory_properties() const noexcept
{
	return this->m_gpu.memory_properties();
}

FORCE_INLINE auto DeviceVulkan::samples_count() const noexcept
{
	return this->m_gpu.samples_count();
}

}        // namespace rhi
