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
#include <any>

#include <Metal/Metal.hpp>

namespace rhi
{

// Utility function to get CAMetalLayer from Objective-C code
void *get_metal_layer(std::any     a_window,               // NSWindow
                      double       a_width,                // Width of the layer
                      double       a_height,               // Height of the layer
                      void        *a_device,               // MTLDevice pointer
                      unsigned int a_pixel_format);        // MTLPixelFormat, usually BGRA8Unorm
// Utility function to get next drawable from Objective-C code
void *next_drawable(void *layer);

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
	FORCE_INLINE              DeviceMetal()                               = default;        //! Default constructor
	FORCE_INLINE              DeviceMetal(const DeviceMetal &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              DeviceMetal(DeviceMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE DeviceMetal &operator=(const DeviceMetal &a_other)       = default;        //! Copy assignment operator
	FORCE_INLINE DeviceMetal &operator=(DeviceMetal &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE virtual ~DeviceMetal() noexcept override                 = default;        //! Destructor

	FORCE_INLINE MTL::Device *platform_device();
	FORCE_INLINE void         init(std::any a_window);

  protected:
  private:
	MTL::Device       *m_device{nullptr};                //! Metal device pointer for MacOS metal targets
	MTL::CommandQueue *m_command_queue;                  //! MTLCommandQueue where all the commands will be submitted, I think I only need one
	std::any           m_window{};                       //! Platform window, on Metal its NSWindow while on Vulkan its GLFWwindow
	void              *m_ca_metal_layer{nullptr};        //! Actually CAMetalLayer*

	declare_translation_unit_vtable();
};

declare_rhi_render_type(Device);

}        // namespace rhi

#include "rhi/metal/rordevice.hh"
