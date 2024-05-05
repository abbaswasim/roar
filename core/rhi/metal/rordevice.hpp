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

#include "event_system/rorevent_system.hpp"
#include "foundation/rormacros.hpp"
#include "math/rorvector2.hpp"
#include "rhi/crtp_interfaces/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"
#include <any>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>

namespace ror
{
class Renderer;
}

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

// Utility function to release the a CAMetalLayer
void release_layer(void *layer);

/*
  There are two ways of multi-threading your work in Metal
  1) At the command buffer level
      Create different Command Buffers for each thread
      Call enqueue on each to establish the execution order
      Fill each from different threads and commit, no synchronisation required
      They will be executed in the enqueue order when become available
  In a typical app, an entire frame of rendering is encoded into a single command buffer, even if rendering that frame involves multiple rendering passes, compute processing functions, or blit operations.

  2) At the Command Encoder level
      Create MTLParallelRenderCommandEncoder for a command buffer
      Create MTLRenderCommandEncoder from MTLParallelRenderCommandEncoder for each thread
      call EndEncoding on all MTLRenderCommandEncoder and then MTLParallelRenderCommandEncoder
      The rendering commands are executed in the order that the subordinate encoders were created.

  Command buffer and command encoder objects are transient and designed for a single use. They are very inexpensive to allocate and deallocate, so their creation methods return autoreleased objects.

  The following objects are not transient. Reuse these objects in performance sensitive code, and avoid creating them repeatedly.

  Command queues
  Data buffers
  Textures
  Sampler states
  Libraries
  Compute states
  Render pipeline states
  Depth/stencil states
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

	FORCE_INLINE void init(std::any a_platform_window, void* a_window, ror::EventSystem &a_event_system, ror::Vector2ui a_dimensions);
	FORCE_INLINE void  swapchain_setup(ror::Renderer *){}
	FORCE_INLINE MTL::Device *platform_device() const;
	FORCE_INLINE MTL::CommandQueue *platform_queue() const;
	FORCE_INLINE MTL::CommandBuffer *platform_command_buffer();
	FORCE_INLINE CA::MetalDrawable *platform_swapchain();

  protected:
  private:
	MTL::Device       *m_device{nullptr};                //! Metal device pointer for MacOS metal targets
	MTL::CommandQueue *m_command_queue;                  //! MTLCommandQueue where all the commands will be submitted, I think I only need one
	std::any           m_window{};                       //! Platform window, on Metal its NSWindow while on Vulkan its GLFWwindow
	void              *m_ca_metal_layer{nullptr};        //! Actually CAMetalLayer*

	declare_translation_unit_vtable();
};

using Swapchain = CA::MetalDrawable *;

declare_rhi_render_type(Device);

}        // namespace rhi

#include "rhi/metal/rordevice.hh"
