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
#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rorrenderpass.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/vulkan/rordevice.hpp"

namespace rhi
{
declare_rhi_render_type(RenderCommandEncoder);
declare_rhi_render_type(ComputeCommandEncoder);

class RenderpassVulkan : public RenderpassCrtp<RenderpassVulkan>
{
  public:
	FORCE_INLINE                   RenderpassVulkan()                                    = default;        //! Default constructor
	FORCE_INLINE                   RenderpassVulkan(const RenderpassVulkan &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                   RenderpassVulkan(RenderpassVulkan &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderpassVulkan &operator=(const RenderpassVulkan &a_other)            = default;        //! Copy assignment operator
	FORCE_INLINE RenderpassVulkan &operator=(RenderpassVulkan &&a_other) noexcept        = default;        //! Move assignment operator
	FORCE_INLINE virtual ~RenderpassVulkan() noexcept override                           = default;        //! Destructor

	declare_translation_unit_vtable();

	void          upload(rhi::Device &a_device);
	VkRenderPass  platform_renderpass(uint32_t a_index) const;
	VkRenderPass  platform_computepass(uint32_t a_index) const;
	VkFramebuffer platform_framebuffer() const;

	size_t                     platform_renderpass_count();
	rhi::RenderCommandEncoder  render_encoder(rhi::CommandBuffer &a_command_buffer, uint32_t a_index);
	rhi::ComputeCommandEncoder compute_encoder(rhi::CommandBuffer &a_command_buffer, uint32_t a_index);
	rhi::ComputeCommandEncoder compute_encoder(rhi::CommandBuffer &a_command_buffer, VkRenderPassCreateInfo *a_pass_descriptor);

	FORCE_INLINE constexpr void make_final_pass(const rhi::Device &a_device, rhi::Swapchain a_surface, uint32_t a_index)
	{
		(void) a_surface;
		(void) a_index;
		(void) a_device;

		// This will only be effective for the first few frames untill all the framebuffers for each swapchain image are created
		// If the render pass used to do that changes over the course of the run it will have to be reset and start again
		// a_surface->setup_framebuffer(a_device.platform_device(), a_index, this);
	}

	void execute(rhi::CommandBuffer &a_command_buffer, ror::Scene &a_scene, rhi::Swapchain a_surface,
	             ror::JobSystem &a_job_system, ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
	             rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer);

  protected:
  private:
	VkRenderPass  m_render_pass{nullptr};        //! Platform render pass descriptors
	VkFramebuffer m_framebuffer{nullptr};        //! Platform framebuffer required for vulkan render passes
};

declare_rhi_render_type(Renderpass);
}        // namespace rhi

#include "rhi/vulkan/rorrenderpass.hh"
