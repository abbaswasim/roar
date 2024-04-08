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

#include "rhi/rorcommand_buffer.hpp"
#include "rhi/rorrenderpass.hpp"

namespace rhi
{

const rhi::RenderTarget &subpass_render_target(const rhi::Renderpass &a_renderpass, const rhi::Rendersubpass a_subpass, size_t a_index)
{
	auto &subpass_rts    = a_subpass.render_targets();
	auto &renderpass_rts = a_renderpass.render_targets();

	assert(a_index < subpass_rts.size() && "Subpass RT index out of bound");
	auto renderpass_index = subpass_rts[a_index];
	assert(renderpass_index < renderpass_rts.size() && "Renderpass RT index out of bound");

	return renderpass_rts[renderpass_index];
}

template <typename _type>
FORCE_INLINE constexpr void pass_by_type(_type &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
                                         rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	switch (a_subpass.type())
	{
	case rhi::RenderpassType::lut:                     lut_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);                    break;
	case rhi::RenderpassType::main:                    main_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);                   break;
	case rhi::RenderpassType::depth:                   depth_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);                  break;
	case rhi::RenderpassType::shadow:                  shadow_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);                 break;
	case rhi::RenderpassType::light_bin:               light_bin_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);              break;
	case rhi::RenderpassType::reflection:              reflection_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer,a_pass,  a_subpass);             break;
	case rhi::RenderpassType::refraction:              refraction_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);             break;
	case rhi::RenderpassType::pre_process:             pre_process_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);            break;
	case rhi::RenderpassType::post_process:            post_process_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);           break;
	case rhi::RenderpassType::tone_mapping:            tone_mapping_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);           break;
	case rhi::RenderpassType::forward_light:           forward_light_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);          break;
	case rhi::RenderpassType::node_transform:          node_transform_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);         break;
	case rhi::RenderpassType::deferred_gbuffer:        deferred_gbuffer_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);       break;
	case rhi::RenderpassType::reflection_probes:       reflection_probes_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);      break;
	case rhi::RenderpassType::image_based_light:       image_based_light_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);      break;
	case rhi::RenderpassType::ambient_occlusion:       ambient_occlusion_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);      break;
	case rhi::RenderpassType::skeletal_transform:      skeletal_transform_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);     break;
	case rhi::RenderpassType::deferred_clustered:      deferred_clustered_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);     break;
	case rhi::RenderpassType::image_based_light_lut:   image_based_light_lut_pass(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);  break;
	case rhi::RenderpassType::max:                     assert(0 && "There is no max render pass type");  break;
	}
	// clang-format on
}

void renderpass_execute(rhi::Renderpass &a_renderpass, rhi::CommandBuffer &a_command_buffer, ror::Scene &a_scene, rhi::Swapchain a_surface, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                        rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer)
{
	// This is true becuase on Metal subpasses turns into render passes. On vulkan it will not work
	if constexpr (ror::get_render_api() == ror::RenderType::vulkan)
	{
		assert(a_renderpass.subpasses().size() == 1 && "This execute doesn't work for Vulkan multi-subpasses. Port me.");
	}

	assert(a_renderpass.subpasses().size() == a_renderpass.platform_renderpass_count() && "Platform renderpasses are not same as subpasses count");

	auto       &subpasses         = a_renderpass.subpasses();
	uint32_t    render_pass_index = 0;
	static bool first_frame{true};
	for (auto &subpass : subpasses)
	{
		if (first_frame || subpass.state() == rhi::RenderpassState::transient)
		{
			if (subpass.technique() != rhi::RenderpassTechnique::compute)
			{
				// TODO: use renderer final pass here
				if (subpass.type() == rhi::RenderpassType::main || subpass.type() == rhi::RenderpassType::forward_light)        // TODO: HACK: Remove the forward_light bit and work out if I am last render pass then use me as main pass
					a_renderpass.make_final_pass(a_surface, render_pass_index);

				// FIXME: These command_encoders are leaking or getting destroyed before we commit the command buffer, although its working now
				// Actually, the metal encoders are autoreleased, since I am not releasing those in RenderCommandEncoder dtors
				// Maybe all we need is to make sure we have an autorelease block around this
				rhi::RenderCommandEncoder command_encoder{a_renderpass.render_encoder(a_command_buffer, render_pass_index++)};

				a_renderpass.setup(command_encoder);
				subpass.setup(command_encoder, a_renderer);

				pass_by_type(command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_renderpass, subpass);

				command_encoder.end_encoding();
			}
			else
			{
				rhi::ComputeCommandEncoder command_encoder{a_renderpass.compute_encoder(a_command_buffer, render_pass_index++)};

				a_renderpass.setup(command_encoder);
				subpass.setup(command_encoder, a_renderer);

				pass_by_type(command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_renderpass, subpass);

				command_encoder.end_encoding();
			}
		}
	}

	first_frame = false;
}
}        // namespace rhi
