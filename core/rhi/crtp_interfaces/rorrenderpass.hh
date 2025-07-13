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
#include "foundation/rorsystem.hpp"
#include "math/rorvector4.hpp"
#include "rhi/crtp_interfaces/rorrenderpass.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorcommand_buffer.hpp"
#include "rhi/rorcompute_command_encoder.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrender_command_encoder.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include <cassert>

namespace rhi
{
template <class _type>
FORCE_INLINE constexpr void bind_input_textures(_type &a_encoder, rhi::Rendersubpass::RenderTargets &a_render_inputs, uint32_t a_starting_index)
{
	for (auto &render_input : a_render_inputs)
	{
		assert(render_input.m_render_output && "Render reference shouldn't be null");
		auto &tr = render_input.m_render_output->m_target_reference.get();        // NOTE: Might need to add frequency incase these are also buffered,
		                                                                          // but usually textures are updated and used in the same frame, and second frame GPU side is free to reuse it
		tr.bind(a_encoder, render_input.m_stage, a_starting_index++);
	}
}

template <class _type>
FORCE_INLINE void Rendersubpass::bind_render_inputs(_type &a_encoder)
{
	auto     render_inputs  = this->rendered_inputs();
	uint32_t starting_index = 15;                                                          // TODO: Abstract out this starting index
	assert(render_inputs.size() < 5 && "Fix the starting index for render inputs");        // 5 because there might be other textures/inputs earlier as well
	bind_input_textures(a_encoder, render_inputs, starting_index);
}

template <class _type>
FORCE_INLINE void Rendersubpass::bind_input_attachments(_type &a_encoder)
{
	auto     render_inputs  = this->input_attachments();
	uint32_t starting_index = 20;                                                          // TODO: Abstract out this starting index
	assert(render_inputs.size() < 5 && "Fix the starting index for render inputs");        // 5 because its used after render_inputs so need enough space
	bind_input_textures(a_encoder, render_inputs, starting_index);
}

template <class _type>
FORCE_INLINE void Rendersubpass::bind_buffer_inputs(_type &a_encoder, const ror::Renderer &a_renderer)        // _type can be rhi::RenderCommandEncoder or rhi::ComputeCommandEncoder
{
	auto buffer_inputs = this->buffer_inputs();
	for (auto &buffer_input : buffer_inputs)
	{
		assert(buffer_input.m_render_output && "Render reference shouldn't be null");
		auto &brs             = buffer_input.m_render_output->m_target_reference.get();
		auto  frequency       = brs[0].frequency();        // Check the first index in the shader buffers array, there must be one
		auto  frequency_index = get_renderer_frequency_index(a_renderer, frequency);
		brs[frequency_index].buffer_bind(a_encoder, buffer_input.m_stage);
	}
}

// NOTE: Enable me if ever needed per render target enabling
// Only possible for compute encoders, for render encoders its done at render pass creation time
// FORCE_INLINE void Rendersubpass::bind_render_targets(rhi::ComputeCommandEncoder &a_encoder, rhi::Renderpass &a_render_pass)
// {
// 	uint32_t starting_index = 0;        // TODO: Abstract out this starting index, this is only valid for compute anyways
// 	auto &renderpass_rts = a_render_pass.render_targets();
// 	auto &subpass_rt_indices        = this->render_targets();

// 	for (auto &srti : subpass_rt_indices)
// 	{
// 		assert(srti < renderpass_rts.size() && "Render target out of bound");
// 		auto &rt = renderpass_rts[srti].m_target_reference.get();
// 		rt.bind(a_encoder, rhi::ShaderStage::compute, starting_index++);
// 	}
// }

// Looks like at least metal doesn't support render_buffers from non-compute and compute requires these to be enabled here, not in renderpass creation time
template <class _type>
FORCE_INLINE void RenderpassCrtp<_type>::bind_render_targets(rhi::ComputeCommandEncoder &a_encoder)
{
	uint32_t starting_index = 0;        // TODO: Make sure shader is patched with this index for the whole render pass
	auto     render_targets = this->render_targets();
	for (auto &render_target : render_targets)
	{
		auto &rt = render_target.m_target_reference.get();
		rt.bind(a_encoder, rhi::ShaderStage::compute, starting_index++);
	}
}

// Looks like at least metal doesn't support render_buffers from non-compute and compute requires these to be enabled here, not in renderpass creation time
template <class _type>
FORCE_INLINE void RenderpassCrtp<_type>::bind_render_buffers(rhi::ComputeCommandEncoder &a_encoder, const ror::Renderer &a_renderer)
{
	auto buffer_inputs = this->render_buffers();
	for (auto &buffer_input : buffer_inputs)
	{
		auto &brs             = buffer_input.m_target_reference.get();
		auto  frequency       = brs[0].frequency();
		auto  frequency_index = get_renderer_frequency_index(a_renderer, frequency);
		brs[frequency_index].buffer_bind(a_encoder, rhi::ShaderStage::compute);
	}
}

template <class _type>
void RenderpassCrtp<_type>::setup(rhi::RenderCommandEncoder &a_command_encoder)
{
	auto         &setting = ror::settings();
	ror::Vector4d viewport{0, 0, static_cast<double64_t>(this->m_dimensions.x), static_cast<double64_t>(this->m_dimensions.y)};

	a_command_encoder.viewport(viewport, {0.0, 1.0});        // TODO: Understand what does the near and far mean for metal render passes
	a_command_encoder.cull_mode(this->m_cull_mode);
	a_command_encoder.front_facing_winding(setting.m_primitive_winding);

	// Inside render pass
	// std::vector<uint32_t>      m_parent_ids{};                                   //! All passes that need to complete before this can run
	// Renderpasses               m_parents{};                                      //! All passes that need to complete before this can run
	// bool                       m_enabled{true};                                  //! Enabled by default unless a "disabled":true is found in the config

	// // std::vector<Rendersubpass> m_subpasses{};                                    //! All the subpasses in this render pass
	// // std::vector<RenderTarget>  m_render_targets{};                               //! Output attachments (images)
	// // std::vector<RenderBuffer>  m_render_buffers{};                               //! Output attachments (buffers)
	// // ror::Vector4f              m_background{0.14f, 0.14f, 0.14f, 1.0f};          //! Background color of this render pass we will use to clear it with
	// // ror::Vector2ui             m_dimensions{1024, 768};                          //! Dimensions for this renderpass if provided will override frame graph dimensions
	// // ror::Vector4i              m_viewport{0, 0, 1024, 768};                      //! Viewport for this renderpass if provided will override frame graph viewport
	// // rhi::PrimitiveCullMode     m_cull_mode{rhi::PrimitiveCullMode::back};        //! Enabled by default unless a "disabled":true is found in the config

	// Inside render subpass
	// std::string              m_name{};                                               //! Debug name of this render pass
	// rhi::RenderpassTechnique m_technique{rhi::RenderpassTechnique::fragment};        //! Will this render pass be excuted in fragment or compute
	// rhi::RenderpassState     m_state{rhi::RenderpassState::transient};               //! Do I need to pre-run this once or required every frame
	// int32_t                  m_program_id{};                                         //! A program id that could be used to execute this pass or will use the content PSOs
	// bool                     m_debug_output{false};                                  //! Whether debug output is required
	// bool                     m_has_depth{false};                                     //! Whether there is a depth buffer required and attached

	// // rhi::RenderpassType      m_type{rhi::RenderpassType::main};                      //! Is it a shadow, reflection etc or main pass etc
	// // RenderTargets            m_input_attachments{};                                  //! References to input attachments from other subpasses, different from m_render_inputs
	// // RenderTargets            m_rendered_inputs{};                                    //! References to texture outputs from other attachments that can be sampled by this subpass as a texture, like shadow map inputs
	// // BufferTargets            m_buffer_inputs{};                                      //! References to buffer outputs from other attachments that can be read by this subpass as buffer inputs
}

template <class _type>
void RenderpassCrtp<_type>::setup(rhi::ComputeCommandEncoder &a_command_encoder, const ror::Renderer &a_renderer)
{
	auto &setting = ror::settings();

	(void) setting;
	(void) a_command_encoder;

	// Lets bind all render targets in one go for each render pass coming later, this saves us having to do this for each render pass
	this->bind_render_targets(a_command_encoder);
	this->bind_render_buffers(a_command_encoder, a_renderer);
}

void lut_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
              rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void main_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
               rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void depth_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void shadow_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                 rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void light_bin_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                    rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void reflection_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void refraction_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void pre_process_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                      rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void post_process_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void tone_mapping_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void forward_light_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                        rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void node_transform_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                         rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void deferred_gbuffer_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                           rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void reflection_probes_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void image_based_light_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void ambient_occlusion_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void skeletal_transform_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                             rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void deferred_clustered_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                             rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void image_based_light_lut_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                                rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);

void lut_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
              rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void main_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
               rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void depth_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void shadow_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                 rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void light_bin_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                    rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void reflection_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void refraction_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                     rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void pre_process_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                      rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void post_process_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void tone_mapping_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                       rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void forward_light_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                        rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void node_transform_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                         rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void deferred_gbuffer_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                           rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void reflection_probes_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void image_based_light_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void ambient_occlusion_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                            rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void skeletal_transform_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                             rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void deferred_clustered_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                             rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);
void image_based_light_lut_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system,
                                rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass);

}        // namespace rhi
