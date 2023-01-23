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

#include "graphics/rorscene.hpp"
#include "profiling/rorlog.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/crtp_interfaces/rorrenderpass.hpp"
#include "settings/rorsettings.hpp"

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
		a_renderer.render_state().bind(a_command_encoder, a_renderer.render_state().depth_state());
	else
		a_renderer.render_state().bind(a_command_encoder, a_renderer.render_state().depth_state_less_no_write());

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
}

void render_scene(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system,
                  ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
                  rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer;
	// clang-format on

	a_scene.render(a_command_encoder, a_buffer_pack, a_renderer, a_subpass);
}

void lut_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
              rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void main_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system,
               ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
               rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void depth_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void shadow_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                 rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void light_bin_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                    rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void reflection_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void refraction_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void pre_process_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                      rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void post_process_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void tone_mapping_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on
}

void forward_light_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                        rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	render_scene(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_subpass);
}

void node_transform_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                         rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void deferred_gbuffer_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                           rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void reflection_probes_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void image_based_light_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void ambient_occlusion_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void skeletal_transform_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                             rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void deferred_clustere_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void lut_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
              rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void main_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system,
               ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
               rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void depth_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void shadow_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                 rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void light_bin_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                    rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void reflection_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void refraction_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void pre_process_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                      rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void post_process_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void tone_mapping_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void forward_light_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                        rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void node_transform_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                         rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	// if (ror::settings().m_animate_cpu)
	// 	a_scene.cpu_walk_scene(a_command_encoder, a_device, a_buffer_pack, a_renderer, a_subpass, a_timer, a_event_system);
	// else
	a_scene.compute_pass_walk_scene(a_command_encoder, a_device, a_buffer_pack, a_renderer, a_subpass, a_timer, a_event_system);
}

void deferred_gbuffer_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                           rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void reflection_probes_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void image_based_light_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void ambient_occlusion_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void skeletal_transform_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                             rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

void deferred_clustere_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Rendersubpass &a_subpass)
{
	// clang-format off
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_subpass;
	// clang-format on

	ror::log_critical("{} not implemented, implement me", __FUNCTION__);
}

}        // namespace rhi
