
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
#include "rhi/rorrenderpass.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include "rhi/rorrender_command_encoder.hpp"
#include "rhi/rorcompute_command_encoder.hpp"

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

	auto  bgc            = this->background();
	auto &render_targets = this->render_targets();

	// There is no concept of subpass in metal so we create a render pass for each subpass, PLS and Merging is done via single encoder instead
	auto render_supasses = this->subpasses();
	this->m_render_passes.reserve(render_supasses.size());

	for (auto &subpass : render_supasses)
	{
		auto mtl_render_pass = MTL::RenderPassDescriptor::alloc()->init();
		assert(mtl_render_pass && "Can't create MTL::RenderPassDescriptor");

		mtl_render_pass->setRenderTargetWidth(this->dimensions().x);
		mtl_render_pass->setRenderTargetHeight(this->dimensions().y);

		(void) subpass;

		auto   &render_target_attachments = this->render_targets();
		int32_t depth_index               = -1;
		for (size_t i = 0; i < render_target_attachments.size(); ++i)
		{
			if (is_pixel_format_depth_format(render_targets[i].m_target_reference.get().format()))
			{
				if (depth_index == -1)
				{
					depth_index = ror::static_cast_safe<int32_t>(i);
					continue;
				}
				else
				{
					assert(0 && "Too many depth render targets provided");
				}
			}

			auto color_attachment = mtl_render_pass->colorAttachments()->object(i);

			color_attachment->setClearColor(MTL::ClearColor::Make(bgc.x, bgc.y, bgc.z, bgc.w));
			color_attachment->setLoadAction(to_metal_load_action(render_targets[i].m_load_action));
			color_attachment->setStoreAction(to_metal_store_action(render_targets[i].m_store_action));
			color_attachment->setTexture(render_targets[i].m_target_reference.get().platform_handle());
		}

		if (depth_index != -1)
		{
			auto depth = mtl_render_pass->depthAttachment();
			depth->setClearDepth(ror::settings().m_depth_clear);
			depth->clearDepth();
			depth->setLoadAction(to_metal_load_action(render_targets[static_cast<uint32_t>(depth_index)].m_load_action));
			depth->setStoreAction(to_metal_store_action(render_targets[static_cast<uint32_t>(depth_index)].m_store_action));

			subpass.has_depth(true);
		}
		else
		{
			ror::log_info("No depth textures created for this renderpass {}", subpass.name().c_str());
		}

		this->m_render_passes.emplace_back(mtl_render_pass);
	}
}

MTL::RenderCommandEncoder *RenderpassMetal::render_encoder(MTL::CommandBuffer *a_command_buffer)
{
	// Hack to get the main pass
	ror::log_critical("All render passes are of size {}", this->m_render_passes.size());
	return a_command_buffer->renderCommandEncoder(this->m_render_passes[0]);
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
