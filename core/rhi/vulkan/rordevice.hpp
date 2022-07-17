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

#include "rhi/crtp_interfaces/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include <any>
// #include <vulkan/include/vulkan/vulkan_core.h>

namespace cfg
{
}

namespace rhi
{
class DeviceVulkan : public DeviceCrtp<DeviceVulkan>
{
  public:
	FORCE_INLINE               DeviceVulkan()                                = default;        //! Default constructor
	FORCE_INLINE               DeviceVulkan(const DeviceVulkan &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               DeviceVulkan(DeviceVulkan &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE DeviceVulkan &operator=(const DeviceVulkan &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE DeviceVulkan &operator=(DeviceVulkan &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE virtual ~DeviceVulkan() noexcept override                   = default;        //! Destructor

	declare_translation_unit_vtable();

	FORCE_INLINE VkDevice platform_device();
	void                  init(std::any a_window);

  protected:
  private:
	VkDevice m_device{};        //! Vulkan device
};

declare_rhi_render_type(Device);

}        // namespace rhi

#include "rhi/vulkan/rordevice.hh"
