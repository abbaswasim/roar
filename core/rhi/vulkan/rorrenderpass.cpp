
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

#include "foundation/rormacros.hpp"
#include "rhi/vulkan/rorrenderpass.hpp"

namespace rhi
{
define_translation_unit_vtable(RenderpassVulkan)
{}

VkAttachmentLoadOp to_vulkan_load_action(rhi::LoadAction a_action)
{
	switch (a_action)
	{
		case rhi::LoadAction::clear:
			return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
		case rhi::LoadAction::load:
			return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD;
		case rhi::LoadAction::dont_care:
			return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	}
}

VkAttachmentStoreOp to_vulkan_store_action(rhi::StoreAction a_action)
{
	switch (a_action)
	{
		case rhi::StoreAction::store:
			return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
		case rhi::StoreAction::discard:
		case rhi::StoreAction::dont_care:
			return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
	}
}

void RenderpassVulkan::upload(rhi::Device &a_device)
{
	(void) a_device;
	// For subpass/programmable blending rendering (only available in iOS) you would use a single command encoder created from the first render pass
	// For your geometry pass the render targets "store" becomes "dont_care" and set storageMode to "memoryLess" == transient in Roar

	// auto  bgc            = this->background();
	auto &render_targets = this->render_targets();

	// There is no concept of subpass in vulkan so we create a render pass for each subpass, PLS and Merging is done via single encoder instead
	auto &render_supasses = this->subpasses();
	this->m_render_passes.clear();
	this->m_render_passes.reserve(render_supasses.size());

	for (auto &subpass : render_supasses)
	{
		if (subpass.technique() != rhi::RenderpassTechnique::compute)
		{
			// auto mtl_render_pass = MTL::RenderPassDescriptor::alloc()->init();
			// assert(mtl_render_pass && "Can't create MTL::RenderPassDescriptor");

			// mtl_render_pass->setRenderTargetWidth(this->dimensions().x);
			// mtl_render_pass->setRenderTargetHeight(this->dimensions().y);

			int32_t  depth_index = -1;
			// uint32_t color_index = 0;
			for (size_t i = 0; i < render_targets.size(); ++i)
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

				// auto color_attachment = mtl_render_pass->colorAttachments()->object(color_index++);

				// color_attachment->setClearColor(MTL::ClearColor::Make(bgc.x, bgc.y, bgc.z, bgc.w));
				// color_attachment->setLoadAction(to_vulkan_load_action(render_targets[i].m_load_action));
				// color_attachment->setStoreAction(to_vulkan_store_action(render_targets[i].m_store_action));
				// color_attachment->setTexture(render_targets[i].m_target_reference.get().platform_handle());
			}

			if (depth_index != -1)
			{
				// auto depth = mtl_render_pass->depthAttachment();

				// depth->setTexture(render_targets[static_cast<uint32_t>(depth_index)].m_target_reference.get().platform_handle());
				// depth->setClearDepth(ror::settings().m_depth_clear);
				// depth->setLoadAction(to_vulkan_load_action(render_targets[static_cast<uint32_t>(depth_index)].m_load_action));
				// depth->setStoreAction(to_vulkan_store_action(render_targets[static_cast<uint32_t>(depth_index)].m_store_action));

				// subpass.has_depth(true);
			}
			else
			{
				ror::log_info("No depth textures created for this renderpass {}", subpass.name().c_str());
			}

			// this->m_render_passes.emplace_back(mtl_render_pass);
		}
		else
		{
			// auto mtl_compute_pass = MTL::ComputePassDescriptor::alloc()->init();
			// assert(mtl_compute_pass && "Can't create MTL::ComputePassDescriptor");

			// mtl_compute_pass->setDispatchType(MTL::DispatchType::DispatchTypeConcurrent);

			// // Can't attach render_targets to compute and can't attach render_buffers here, needs to be done in PSO generation time

			// this->m_render_passes.emplace_back(mtl_compute_pass);
		}
	}
}

size_t RenderpassVulkan::platform_renderpass_count()
{
	return this->m_render_passes.size();
}

VkRenderPass *RenderpassVulkan::platform_renderpass(uint32_t a_index)
{
	(void) a_index;
	// if (std::holds_alternative<MTL::RenderPassDescriptor *>(this->m_render_passes[a_index]))
	// 	return std::get<MTL::RenderPassDescriptor *>(this->m_render_passes[a_index]);

	return nullptr;
}

VkRenderPass *RenderpassVulkan::platform_computepass(uint32_t a_index)
{
	(void) a_index;
	// if (std::holds_alternative<MTL::ComputePassDescriptor *>(this->m_render_passes[a_index]))
	// 	return std::get<MTL::ComputePassDescriptor *>(this->m_render_passes[a_index]);

	return nullptr;
}

rhi::RenderCommandEncoder RenderpassVulkan::render_encoder(rhi::CommandBuffer &a_command_buffer, uint32_t a_index)
{
	(void) a_index;
	return rhi::RenderCommandEncoder{a_command_buffer.platform_command_buffer()};
}

rhi::ComputeCommandEncoder RenderpassVulkan::compute_encoder(rhi::CommandBuffer &a_command_buffer, uint32_t a_index)
{
	(void) a_index;
	return rhi::ComputeCommandEncoder{a_command_buffer.platform_command_buffer()};
}

}
