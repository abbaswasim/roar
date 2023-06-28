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
#include "rhi/vulkan/rordevice.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
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
		auto pixel_format = to_vulkan_pixelformat(settings.m_pixel_format);

		this->m_swapchain.release(this->m_device);
		this->m_swapchain.create(this->m_gpu.get_handle(), this->m_device, this->m_surface, pixel_format, VkExtent2D{size.x, size.y});
	};

	a_event_system.subscribe(ror::buffer_resize, resize_callback);

	ror::Event e{ror::buffer_resize, true, ror::Vector2ui{a_dimensions.x, a_dimensions.y}};
	resize_callback(e);
}

FORCE_INLINE VkDevice DeviceVulkan::platform_device()
{
	assert(this->m_device && "Vulkan device requested is null");

	return this->m_device;
}

FORCE_INLINE VkQueue DeviceVulkan::platform_queue()
{
	assert(this->m_command_queue && "Vulkan device requested is null");

	return this->m_command_queue;
}

FORCE_INLINE VkCommandBuffer DeviceVulkan::platform_command_buffer()
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

	return buffer;
}

FORCE_INLINE rhi::Swapchain DeviceVulkan::platform_swapchain()
{
	assert(this->m_swapchain.swapchain() && "Vulkan ca layer is null, can't create swapchain");

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

}        // namespace rhi
