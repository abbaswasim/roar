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

#include "foundation/rormacros.hpp"
#include "rhi/crtp_interfaces/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"

#include <Metal/MTLDevice.hpp>

namespace rhi
{
/*
  Metal multi-threaded command buffer filling advice
In a multithreaded app, it’s advisable to break your overall task into subtasks that can be encoded separately.
Create a command buffer for each chunk of work, then call the enqueue method on these command buffer objects to establish the order of execution.
Fill each buffer object (using multiple threads) and commit them.
The command queue automatically schedules and executes these command buffers as they become available.
 */
class DeviceMetal : public DeviceCrtp<DeviceMetal>
{
  public:
	FORCE_INLINE              DeviceMetal();                                                //! Default constructor
	FORCE_INLINE              DeviceMetal(const DeviceMetal &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              DeviceMetal(DeviceMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE DeviceMetal &operator=(const DeviceMetal &a_other)       = default;        //! Copy assignment operator
	FORCE_INLINE DeviceMetal &operator=(DeviceMetal &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE virtual ~DeviceMetal() noexcept override                 = default;        //! Destructor

	FORCE_INLINE MTL::Device *platform_device();

  protected:
  private:
	MTL::Device *m_device{nullptr};             //! Metal device pointer for MacOS metal targets
	// void        *m_metal_layer{nullptr};        // Actually CAMetalLayer*

	declare_translation_unit_vtable();
};

declare_rhi_render_type(Device);

}        // namespace rhi

#include "rhi/metal/rordevice.hh"
