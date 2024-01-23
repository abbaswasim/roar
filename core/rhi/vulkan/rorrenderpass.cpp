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
#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rorrenderpass.hpp"
#include "rhi/vulkan/rorrenderpass.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include <cassert>
#include <cstddef>
#include <unordered_map>
#include <utility>

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

static void attachments_create_utility(const RenderTarget &a_render_target, uint32_t a_rti_index, VkSampleCountFlagBits a_samples,
                                       std::vector<VkAttachmentDescription> &a_attachments_descriptions,
                                       std::vector<VkAttachmentReference>   &a_attachments_references,
                                       VkAttachmentLoadOp a_load_op, VkAttachmentStoreOp a_store_op,
                                       VkImageLayout a_initial_layout, VkImageLayout a_final_layout)
{
	if (a_render_target.m_type == rhi::RenderOutputType::depth)
	{
		assert(is_pixel_format_depth_format(a_render_target.m_target_reference.get().format()) && "Invalid depth render target format");
		assert(a_final_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL && "Invalid depth render target format final image layout");
	}

	auto format                   = a_render_target.m_target_reference.get().format();
	auto vkformat                 = to_vulkan_pixelformat(format);
	auto render_target_attachment = vk_create_attachment_description(vkformat, a_samples, a_load_op, a_store_op,
	                                                                 VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
	                                                                 a_initial_layout, a_final_layout);

	auto render_target_reference = vk_create_attachment_reference(a_rti_index, a_final_layout);

	a_attachments_descriptions.emplace_back(std::move(render_target_attachment));
	a_attachments_references.emplace_back(std::move(render_target_reference));
}

void RenderpassVulkan::upload(rhi::Device &a_device)
{
	auto  samples                   = a_device.samples_count();
	auto &renderpass_render_targets = this->render_targets();

	// Unlike Metal where there is no concept of subpass so we created a render pass for each subpass, In Vulkan we have first class subpasses
	auto &render_supasses = this->subpasses();

	// TODO: Whats left to use from Metal still, but this perhaps goes into framebuffer
	// auto  bgc            = this->background();
	// color_attachment->setClearColor(MTL::ClearColor::Make(bgc.x, bgc.y, bgc.z, bgc.w));
	// depth->setClearDepth(ror::settings().m_depth_clear);

	uint32_t                             rti_index = 0;
	std::vector<VkAttachmentDescription> renderpass_render_target_attachments_descriptions{};
	std::vector<VkAttachmentReference>   renderpass_render_target_attachments_references{};
	std::vector<VkSubpassDescription>    subpasses_descriptions;
	std::vector<VkSubpassDependency>     subpasses_dependencies;
	std::vector<VkImageView>             framebuffer_attachments;

	renderpass_render_target_attachments_descriptions.reserve(renderpass_render_targets.size());
	renderpass_render_target_attachments_references.reserve(renderpass_render_targets.size());
	framebuffer_attachments.reserve(renderpass_render_targets.size());
	subpasses_descriptions.reserve(render_supasses.size());
	subpasses_dependencies.reserve(render_supasses.size());

	// Create vulkan render target descriptions and references and framebuffer attachments
	VkImageLayout initial_layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkImageLayout final_layout   = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	for (auto &render_target : renderpass_render_targets)
	{
		if (is_pixel_format_depth_format(render_target.m_target_reference.get().format()))
		{
			initial_layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			final_layout   = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}
		else
		{
			initial_layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			final_layout   = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		VkAttachmentLoadOp  load_op{to_vulkan_load_action(render_target.m_load_action)};
		VkAttachmentStoreOp store_op{to_vulkan_store_action(render_target.m_store_action)};

		attachments_create_utility(render_target, rti_index, samples, renderpass_render_target_attachments_descriptions, renderpass_render_target_attachments_references, load_op, store_op, initial_layout, final_layout);

		framebuffer_attachments.emplace_back(render_target.m_target_reference.get().image_view());

		rti_index++;
	}

	// Collect all input attachment descriptions and references
	std::vector<std::pair<uint32_t, const RenderTarget *>> input_attachments_references{};
	initial_layout = VK_IMAGE_LAYOUT_UNDEFINED;
	final_layout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	for (auto &subpass : render_supasses)
	{
		auto &subpass_render_targets = subpass.input_attachments();
		for (auto &render_target : subpass_render_targets)
		{
			VkAttachmentLoadOp  load_op{VK_ATTACHMENT_LOAD_OP_DONT_CARE};        // TODO: DONT_CARE for both load and store for input attachments doesn't make sense
			VkAttachmentStoreOp store_op{VK_ATTACHMENT_STORE_OP_DONT_CARE};

			assert(render_target.m_render_output != nullptr && "Render output reference can't be null");
			attachments_create_utility(*render_target.m_render_output, rti_index, samples, renderpass_render_target_attachments_descriptions, renderpass_render_target_attachments_references, load_op, store_op, initial_layout, final_layout);
			input_attachments_references.emplace_back(std::make_pair(rti_index, render_target.m_render_output));

			// TODO: Understand why am I doing this. shouldn't this only contain all the render targets and not subpass attachments
			framebuffer_attachments.emplace_back(render_target.m_render_output->m_target_reference.get().image_view());

			rti_index++;
		}
	}

	// Copies of per subpass data to be kept alive until vk_create_render_pass is called later
	std::vector<std::vector<VkAttachmentReference>> resident_subpass_input_attachments{};
	std::vector<std::vector<VkAttachmentReference>> resident_subpass_color_attachments_references{};

	for (auto &subpass : render_supasses)
	{
		auto &subpass_render_targets = subpass.render_targets();

		std::vector<VkAttachmentReference> subpass_input_attachments{};
		std::vector<VkAttachmentReference> subpass_color_attachments_references{};
		VkAttachmentReference             *subpass_depth_attachment_reference{nullptr};
		VkAttachmentReference             *subpass_resolve_attachment_reference{nullptr};

		if (subpass.technique() != rhi::RenderpassTechnique::compute)
		{
			// subpass.input_attachments returns RenderTargets which is std::vector<RenderOutputRef<RenderTarget>>;
			for (auto &input_attachment : subpass.input_attachments())
			{
				int32_t input_attachment_index{-1};

				for (auto &input_attachment_reference : input_attachments_references)
					if (input_attachment.m_render_output == input_attachment_reference.second)
						input_attachment_index = ror::static_cast_safe<int32_t>(input_attachment_reference.first);

				assert(input_attachment_index != -1 && "Can't find the reference to input attachment created earlier");

				auto attachment_reference = renderpass_render_target_attachments_references[static_cast<size_t>(input_attachment_index)];
				subpass_input_attachments.emplace_back(attachment_reference);
			}

			resident_subpass_input_attachments.emplace_back(std::move(subpass_input_attachments));

			for (size_t i = 0; i < subpass_render_targets.size(); ++i)
			{
				assert(subpass_render_targets[i] < renderpass_render_targets.size() && "Subpass render target index out of bound");
				assert(subpass_render_targets[i] < renderpass_render_target_attachments_references.size() && "Subpass render target index out of bound");

				auto &rendertarget = renderpass_render_targets[subpass_render_targets[i]];

				if (rendertarget.m_type == rhi::RenderOutputType::depth && subpass_depth_attachment_reference == nullptr)
					subpass_depth_attachment_reference = &renderpass_render_target_attachments_references[subpass_render_targets[i]];
				else if (rendertarget.m_type == rhi::RenderOutputType::resolve && subpass_resolve_attachment_reference == nullptr)
					subpass_resolve_attachment_reference = &renderpass_render_target_attachments_references[subpass_render_targets[i]];
				else if (rendertarget.m_type == rhi::RenderOutputType::color)
					subpass_color_attachments_references.emplace_back(renderpass_render_target_attachments_references[subpass_render_targets[i]]);
				else
					assert(0 && "Implement other types");
			}

			resident_subpass_color_attachments_references.emplace_back(std::move(subpass_color_attachments_references));

			subpasses_descriptions.emplace_back(vk_create_subpass_description(VK_PIPELINE_BIND_POINT_GRAPHICS, resident_subpass_input_attachments.back(), resident_subpass_color_attachments_references.back(),
			                                                                  subpass_depth_attachment_reference, subpass_resolve_attachment_reference));
		}
		else
		{
			ror::log_critical("No concept of Vulkan Renderpass for compute render pass, not creating anything, find a solution");
			// TODO: This doesn't work. Although it binds to compute binding point, the problem is there is no concept of compute render pass at the moment in vulkan
			// subpasses_descriptions.emplace_back(vk_create_subpass_description(VK_PIPELINE_BIND_POINT_COMPUTE, input_attachments, subpass_color_attachments_references,
			//                                                                   subpass_depth_attachment_reference, subpass_resolve_attachment_reference));
		}
	}

	// Create subpass dependencies
	uint32_t             src_subpass     = VK_SUBPASS_EXTERNAL;
	uint32_t             dst_subpass     = 0;
	VkPipelineStageFlags src_stage_mask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	VkAccessFlags        src_access_mask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	VkPipelineStageFlags dst_stage_mask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;        // VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	VkAccessFlags        dst_access_mask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	for (size_t i = 0; i < subpasses_descriptions.size(); ++i)
	{
		auto dependency = vk_create_subpass_dependency(src_subpass, dst_subpass, src_stage_mask, dst_stage_mask, src_access_mask, dst_access_mask, VK_DEPENDENCY_BY_REGION_BIT);
		subpasses_dependencies.emplace_back(std::move(dependency));

		dst_access_mask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		dst_stage_mask  = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		src_subpass     = dst_subpass;
		dst_subpass++;
	}

	// TODO: Remove the check ones compute path is defined
	if (subpasses_descriptions.size() > 0)
	{
		VkExtent2D dimensions{};
		dimensions.width  = this->dimensions().x;
		dimensions.height = this->dimensions().y;

		this->m_render_pass = vk_create_render_pass(a_device.platform_device(), std::move(renderpass_render_target_attachments_descriptions), std::move(subpasses_descriptions), std::move(subpasses_dependencies));
		this->m_framebuffer = vk_create_framebuffer(a_device.platform_device(), this->m_render_pass, framebuffer_attachments, dimensions);
	}
}

size_t RenderpassVulkan::platform_renderpass_count()
{
	return 1ul;
}

VkRenderPass RenderpassVulkan::platform_renderpass(uint32_t a_index)
{
	(void) a_index;
	return this->m_render_pass;
}

VkRenderPass RenderpassVulkan::platform_computepass(uint32_t a_index)
{
	(void) a_index;
	return this->m_render_pass;
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

void RenderpassVulkan::execute(rhi::CommandBuffer &a_command_buffer, ror::Scene &a_scene, rhi::Swapchain a_surface, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                               rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer)
{
	renderpass_execute(*this, a_command_buffer, a_scene, a_surface, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer);
}
}        // namespace rhi
