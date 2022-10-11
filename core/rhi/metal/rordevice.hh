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
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/metal/rordevice.hpp"
#include "rhi/metal/rormetal_common.hpp"
#include "settings/rorsettings.hpp"
#include <any>

#include <Metal/MTLCommandQueue.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>


namespace rhi
{

// This is not inside the ctor above because by the time Application ctor chain is finished the window in UnixApp is not initialized yet
FORCE_INLINE void DeviceMetal::init(std::any a_window)
{
	this->m_device = MTL::CreateSystemDefaultDevice();
	if (!this->m_device)
	{
		ror::log_critical("Can't create metal device");
		exit(1);
	}

	auto    &settings     = ror::settings();
	uint32_t pixel_format = ror::static_cast_safe<uint32_t>(to_metal_pixelformat(settings.m_pixel_format));

	this->m_window         = a_window;
	this->m_ca_metal_layer = get_metal_layer(a_window,
	                                         static_cast<float32_t>(settings.m_window_dimensions.z),
	                                         static_cast<float32_t>(settings.m_window_dimensions.w),
	                                         this->m_device,
	                                         pixel_format);

	if (!this->m_ca_metal_layer)
	{
		ror::log_critical("Can't acquire CA Metal layer");
		exit(1);
	}

	this->m_command_queue = this->m_device->newCommandQueue();

	if (!this->m_command_queue)
	{
		ror::log_critical("Can't create Metal command queue");
		exit(1);
	}
}

FORCE_INLINE MTL::Device *DeviceMetal::platform_device()
{
	assert(this->m_device && "Metal device requested is null");
	return this->m_device;
}

FORCE_INLINE MTL::CommandQueue *DeviceMetal::platform_queue()
{
	assert(this->m_command_queue && "Metal device requested is null");
	return this->m_command_queue;
}

FORCE_INLINE CA::MetalDrawable *DeviceMetal::platform_swapchain()
{
	assert(this->m_ca_metal_layer && "Metal ca layer is null, can't create swapchain");
	return reinterpret_cast<CA::MetalDrawable *>(next_drawable(this->m_ca_metal_layer));
}
}        // namespace rhi
