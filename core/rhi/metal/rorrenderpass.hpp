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
#include "rhi/crtp_interfaces/rorrenderpass.hpp"
#include "rhi/metal/rorcommand_buffer.hpp"
#include "rhi/metal/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"

#include <Metal/MTLRenderPass.hpp>

namespace rhi
{
declare_rhi_render_type(RenderCommandEncoder);
declare_rhi_render_type(ComputeCommandEncoder);

class RenderpassMetal : public RenderpassCrtp<RenderpassMetal>
{
  public:
	FORCE_INLINE                  RenderpassMetal()                                   = default;        //! Default constructor
	FORCE_INLINE                  RenderpassMetal(const RenderpassMetal &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                  RenderpassMetal(RenderpassMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderpassMetal &operator=(const RenderpassMetal &a_other)           = default;        //! Copy assignment operator
	FORCE_INLINE RenderpassMetal &operator=(RenderpassMetal &&a_other) noexcept       = default;        //! Move assignment operator
	FORCE_INLINE virtual ~RenderpassMetal() noexcept override                         = default;        //! Destructor

	declare_translation_unit_vtable();

	void                        upload(rhi::Device &a_device);
	MTL::RenderPassDescriptor  *platform_renderpass(uint32_t a_index);
	MTL::ComputePassDescriptor *platform_computepass(uint32_t a_index);

	size_t                     platform_renderpass_count();
	rhi::RenderCommandEncoder  render_encoder(rhi::CommandBuffer &a_command_buffer, uint32_t a_index);
	rhi::ComputeCommandEncoder compute_encoder(rhi::CommandBuffer &a_command_buffer, uint32_t a_index);

	FORCE_INLINE constexpr void make_final_pass(rhi::Swapchain a_surface, uint32_t a_index)
	{
		auto rp = this->platform_renderpass(a_index);
		rp->colorAttachments()->object(0)->setTexture(a_surface->texture());
	}

  protected:
  private:
	std::vector<std::variant<MTL::RenderPassDescriptor *, MTL::ComputePassDescriptor *>> m_render_passes{};        //! Platform render pass descriptors, we have a list here because engine subpasses are split into render passes for Metal
};

declare_rhi_render_type(Renderpass);

}        // namespace rhi

#include "rhi/metal/rorrenderpass.hh"
