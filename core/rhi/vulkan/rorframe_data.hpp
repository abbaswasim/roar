// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2025
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

#include "rhi/crtp_interfaces/rorframe_data.hpp"
#include "rhi/rorrhi_macros.hpp"

namespace rhi
{
class FrameDataVulkan : public FrameDataCrtp<FrameDataVulkan>
{
  public:
	FORCE_INLINE                  FrameDataVulkan(const FrameDataVulkan &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                  FrameDataVulkan(FrameDataVulkan &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE FrameDataVulkan &operator=(const FrameDataVulkan &a_other)           = default;        //! Copy assignment operator
	FORCE_INLINE FrameDataVulkan &operator=(FrameDataVulkan &&a_other) noexcept       = default;        //! Move assignment operator
	FORCE_INLINE virtual ~FrameDataVulkan() noexcept override                         = default;        //! Destructor

  protected:
	FORCE_INLINE FrameDataVulkan() = default;        //! Default constructor
  private:
	DescriptorSetCache *m_descriptor_sets{nullptr};         //! An alias of the main descriptor cache
	VkCommandBuffer     m_render_command_buffers{};         //! Command buffer used for rendering
	VkCommandBuffer     m_compute_command_buffers{};        //! Command buffer used for compute dispatches
	VkSemaphore         m_acquire_semaphore{};              //! Acquire semaphore used to acquire images from swapchain
	VkSemaphore         m_present_semaphore{};              //! Present semaphore used to present images from swapchain onto display
	VkFence             m_submit_fence;                     //! Fence used to waiting on queue submits
};

declare_rhi_render_type(FrameData);

}        // namespace rhi

#include "rhi/vulkan/rorframe_data.hh"
