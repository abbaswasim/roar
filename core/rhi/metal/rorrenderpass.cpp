
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

#include "rhi/metal/rorrenderpass.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include <Metal/MTLRenderPass.hpp>

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
	this->m_render_pass = MTL::RenderPassDescriptor::alloc()->init();

	// auto bgc = this->background();

	// auto &input_attachments = this->input_attachments();
	// for (size_t i = 0; i < input_attachments.size(); ++i)
	// {
	// 	auto colorAttachment = this->m_render_pass->colorAttachments()->object(i);

	// 	colorAttachment->setClearColor(MTL::ClearColor::Make(bgc.x, bgc.y, bgc.z, bgc.w));
	// 	colorAttachment->setLoadAction(to_metal_load_action(input_attachments[i].m_action));
	// 	colorAttachment->setStoreAction(to_metal_store_action(rhi::StoreAction::dont_care));
	// 	// colorAttachment->setTexture(swapchain->texture());
	// }

	// auto depth = this->m_render_pass->depthAttachment();
	// depth->clearDepth();
	// depth->setLoadAction(MTL::LoadActionClear);
	// depth->setStoreAction(MTL::StoreActionStore);
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
