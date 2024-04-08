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

#include "event_system/rorevent_handles.hpp"
#include "graphics/rorscene.hpp"
#include "profiling/rorlog.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/crtp_interfaces/rorrenderpass.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include <vector>

namespace rhi
{

define_translation_unit_vtable(RenderOutput)
{}

define_translation_unit_vtable(RenderTarget)
{}

define_translation_unit_vtable(RenderBuffer)
{}

void Rendersubpass::setup(rhi::RenderCommandEncoder &a_command_encoder, ror::Renderer &a_renderer)
{
	if (this->has_depth())
		a_command_encoder.depth_stencil_state(a_renderer.render_state().depth_state());
	else
		a_command_encoder.depth_stencil_state(a_renderer.render_state().depth_state_less_no_write());

	this->bind_render_inputs(a_command_encoder);
	this->bind_buffer_inputs(a_command_encoder);
	this->bind_input_attachments(a_command_encoder);
}

void Rendersubpass::setup(rhi::ComputeCommandEncoder &a_command_encoder, ror::Renderer &a_renderer)
{
	(void) a_command_encoder;
	(void) a_renderer;

	// do compute setup
	this->bind_render_inputs(a_command_encoder);
	this->bind_buffer_inputs(a_command_encoder);
	this->bind_input_attachments(a_command_encoder);

	// NOTE: Also need to add bind_render_targets(), special case for compute, but that requires renderpass which is not available here
	// This is why its done in RenderpassCrt::setup instead
}

bool Rendersubpass::has_depth_attachment(const std::vector<RenderTarget> &a_renderpass_render_targets, const std::vector<uint32_t> &a_subpas_render_targets)
{
	for (auto &rti : a_subpas_render_targets)
	{
		if (is_pixel_format_depth_format(a_renderpass_render_targets[rti].m_target_reference.get().format()))
			return true;
	}

	return false;
}

void render_scene(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system,
                  ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
                  rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_job_system; (void) a_device; (void) a_timer;
	// clang-format on

	a_scene.render(a_device, a_command_encoder, a_buffer_pack, a_renderer, a_pass, a_subpass, a_event_system);
}

void lut_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
              rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void main_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system,
               ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
               rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void depth_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void shadow_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                 rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	for (auto &light : a_scene.lights())
	{
		if (light.m_type == ror::Light::LightType::directional)        // TODO: Check all shadow casting lights
		{
			ror::Matrix4f  *view_projection{nullptr};
			ror::Matrix4f  *projection{nullptr};
			ror::Matrix4f  *view{nullptr};
			ror::Vector3f  *position{nullptr};
			ror::Vector4ui *viewport;

			ror::Matrix4f _view_projection{};
			ror::Matrix4f _projection{};
			ror::Matrix4f _view{};

			// TODO: This doesn't work at the moment because of no per pass per view uniforms
			light.get_transformations(&view_projection, &projection, &view, &position, &viewport);
			a_renderer.update_per_view_uniform(*view, *projection, *viewport, *position);
			render_scene(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);
			break;
		}
	}
}

void light_bin_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                    rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void reflection_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void refraction_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void pre_process_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                      rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void post_process_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void tone_mapping_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	// ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void forward_light_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                        rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	// Proper synchronisation of uniforms is needed
	// TODO: Need to find a way for sharing uniforms across multiple render passes and multiple frames
	// TODO: This doesn't work at the moment because of no per pass per view uniforms
	// When per-pass and per-frame versions of uniforms are added, this could also be done at scene::update time
	auto &camera = a_scene.current_camera();
	camera.update(a_renderer);

	render_scene(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);
}

void node_transform_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                         rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void deferred_gbuffer_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                           rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void reflection_probes_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void image_based_light_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void image_based_light_lut_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                                rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void ambient_occlusion_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void skeletal_transform_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                             rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void deferred_clustered_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                             rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Render pass {} not implemented, implement me", __FUNCTION__);
}

void lut_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
              rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void main_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system,
               ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
               rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void depth_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void shadow_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                 rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void light_bin_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                    rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void reflection_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void refraction_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void pre_process_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                      rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void post_process_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void tone_mapping_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void forward_light_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                        rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void node_transform_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                         rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	// if (ror::settings().m_animate_cpu)
	// 	a_scene.cpu_walk_scene(a_command_encoder, a_device, a_buffer_pack, a_renderer, a_subpass, a_timer, a_event_system);
	// else
	a_scene.compute_pass_walk_scene(a_command_encoder, a_device, a_buffer_pack, a_renderer, a_subpass, a_timer, a_event_system);
}

void deferred_gbuffer_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                           rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void reflection_probes_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void reflection_probes_pass_lut(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                                rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void image_based_light_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void image_based_light_lut_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                                rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_pass;
	// clang-format on

	auto program_id = a_subpass.program_id();

	assert(program_id != -1 && "No program provided for image_based_light_lut_pass");

	auto &compute_pso       = a_renderer.programs()[static_cast<size_t>(program_id)];
	auto  per_frame_uniform = a_renderer.shader_buffer("per_frame_uniform");

	// Encode the pipeline state object and its parameters.
	a_command_encoder.compute_pipeline_state(compute_pso);
	per_frame_uniform->buffer_bind(a_command_encoder, rhi::ShaderStage::compute);

	auto lut_image_index = a_renderer.brdf_integration_lut_index();
	assert(lut_image_index != -1 && "Can't run a lut pass on no image available");

	auto &lut_image = a_renderer.images()[static_cast<size_t>(lut_image_index)];

	// Calculate a threadgroup size.
	uint32_t max_thread_group_size = 32;
	// Encode the compute command.
	a_command_encoder.dispatch_threads({lut_image.width(), lut_image.height(), 1}, {max_thread_group_size, max_thread_group_size, 1});
}

void ambient_occlusion_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void skeletal_transform_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                             rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

void deferred_clustered_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                             rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
	// clang-format on

	ror::log_critical("Compute pass {} not implemented, implement me", __FUNCTION__);
}

}        // namespace rhi
