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

#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/metal/rorcommand_buffer.hpp"
#include "rhi/rorcompute_command_encoder.hpp"
#include "rhi/rorrender_command_encoder.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"

#include <Metal/MTLPixelFormat.hpp>
#include <Metal/MTLRenderPass.hpp>
#include <Metal/MTLResource.hpp>
#include <Metal/MTLTexture.hpp>
#include <variant>

namespace rhi
{
define_translation_unit_vtable(RenderpassMetal)
{}

MTL::LoadAction to_metal_load_action(rhi::LoadAction a_action)
{
	switch (a_action)
	{
		case rhi::LoadAction::clear:
			return MTL::LoadActionClear;
		case rhi::LoadAction::load:
			return MTL::LoadActionLoad;
		case rhi::LoadAction::dont_care:
			return MTL::LoadActionDontCare;
	}
}

MTL::StoreAction to_metal_store_action(rhi::StoreAction a_action)
{
	switch (a_action)
	{
		case rhi::StoreAction::store:
			return MTL::StoreActionStore;
		case rhi::StoreAction::discard:
		case rhi::StoreAction::dont_care:
			return MTL::StoreActionDontCare;
	}
}

void RenderpassMetal::upload(rhi::Device &a_device)
{
	(void) a_device;
	// For subpass/programmable blending rendering (only available in iOS) you would use a single command encoder created from the first render pass
	// For your geometry pass the render targets "store" becomes "dont_care" and set storageMode to "memoryLess" == transient in Roar

	auto  bgc                       = this->background();
	auto &renderpass_render_targets = this->render_targets();
	auto &render_supasses           = this->subpasses();

	this->m_render_passes.clear();
	this->m_render_passes.reserve(render_supasses.size());

	// There is no concept of subpass in metal so we create a render pass for each subpass, PLS and Merging is done via single encoder instead
	for (auto &subpass : render_supasses)
	{
		if (subpass.technique() != rhi::RenderpassTechnique::compute)
		{
			auto mtl_render_pass = MTL::RenderPassDescriptor::alloc()->init();
			assert(mtl_render_pass && "Can't create MTL::RenderPassDescriptor");

			mtl_render_pass->setRenderTargetWidth(this->dimensions().x);
			mtl_render_pass->setRenderTargetHeight(this->dimensions().y);

			uint32_t color_index            = 0;
			auto     subpass_render_targets = subpass.render_targets();
			for (size_t i = 0; i < subpass_render_targets.size(); ++i)
			{
				auto &render_target = renderpass_render_targets[subpass_render_targets[i]];
				if (is_pixel_format_depth_format(render_target.m_target_reference.get().format()))
				{
					auto depth_attachment = mtl_render_pass->depthAttachment();

					depth_attachment->setClearDepth(ror::settings().m_depth_clear);
					depth_attachment->setLoadAction(to_metal_load_action(render_target.m_load_action));
					depth_attachment->setStoreAction(to_metal_store_action(render_target.m_store_action));
					depth_attachment->setTexture(render_target.m_target_reference.get().platform_handle());
				}
				else
				{
					auto color_attachment = mtl_render_pass->colorAttachments()->object(color_index++);

					color_attachment->setClearColor(MTL::ClearColor::Make(bgc.x, bgc.y, bgc.z, bgc.w));
					color_attachment->setLoadAction(to_metal_load_action(render_target.m_load_action));
					color_attachment->setStoreAction(to_metal_store_action(render_target.m_store_action));
					color_attachment->setTexture(render_target.m_target_reference.get().platform_handle());
				}
			}

			this->m_render_passes.emplace_back(mtl_render_pass);
		}
		else
		{
			auto mtl_compute_pass = MTL::ComputePassDescriptor::alloc()->init();
			assert(mtl_compute_pass && "Can't create MTL::ComputePassDescriptor");

			mtl_compute_pass->setDispatchType(MTL::DispatchType::DispatchTypeConcurrent);

			// Can't attach render_targets to compute and can't attach render_buffers here, needs to be done in PSO generation time

			this->m_render_passes.emplace_back(mtl_compute_pass);
		}
	}
}

size_t RenderpassMetal::platform_renderpass_count()
{
	return this->m_render_passes.size();
}

MTL::RenderPassDescriptor *RenderpassMetal::platform_renderpass(uint32_t a_index)
{
	if (std::holds_alternative<MTL::RenderPassDescriptor *>(this->m_render_passes[a_index]))
		return std::get<MTL::RenderPassDescriptor *>(this->m_render_passes[a_index]);

	return nullptr;
}

MTL::ComputePassDescriptor *RenderpassMetal::platform_computepass(uint32_t a_index)
{
	if (std::holds_alternative<MTL::ComputePassDescriptor *>(this->m_render_passes[a_index]))
		return std::get<MTL::ComputePassDescriptor *>(this->m_render_passes[a_index]);

	return nullptr;
}

rhi::RenderCommandEncoder RenderpassMetal::render_encoder(rhi::CommandBuffer &a_command_buffer, uint32_t a_index)
{
	return rhi::RenderCommandEncoder{a_command_buffer.platform_command_buffer()->renderCommandEncoder(this->platform_renderpass(a_index))};
}

rhi::ComputeCommandEncoder RenderpassMetal::compute_encoder(rhi::CommandBuffer &a_command_buffer, uint32_t a_index)
{
	return rhi::ComputeCommandEncoder{a_command_buffer.platform_command_buffer()->computeCommandEncoder(this->platform_computepass(a_index))};
}

}        // namespace rhi

// Difference between renderpass vs renderpipeline color attachments
/*
class RenderPipelineColorAttachmentDescriptor
{
public:
    MTL::PixelFormat                                      pixelFormat() const;
    bool                                                  blendingEnabled() const;
    MTL::BlendFactor                                      sourceRGBBlendFactor() const;
    MTL::BlendFactor                                      destinationRGBBlendFactor() const;
    MTL::BlendOperation                                   rgbBlendOperation() const;
    MTL::BlendFactor                                      sourceAlphaBlendFactor() const;
    MTL::BlendFactor                                      destinationAlphaBlendFactor() const;
    MTL::BlendOperation                                   alphaBlendOperation() const;
    MTL::ColorWriteMask                                   writeMask() const;
};

class RenderPassColorAttachmentDescriptor
{
public:
    class Texture*                               texture() const;
    NS::UInteger                                 level() const;
    NS::UInteger                                 slice() const;
    NS::UInteger                                 depthPlane() const;
    class Texture*                               resolveTexture() const;
    NS::UInteger                                 resolveLevel() const;
    NS::UInteger                                 resolveSlice() const;
    NS::UInteger                                 resolveDepthPlane() const;
    MTL::LoadAction                              loadAction() const;
    MTL::StoreAction                             storeAction() const;
    MTL::StoreActionOptions                      storeActionOptions() const;
    MTL::ClearColor                              clearColor() const;
};
*/
