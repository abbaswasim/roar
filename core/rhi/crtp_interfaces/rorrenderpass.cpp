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

#include "bounds/rorbounding.hpp"
#include "camera/rorcamera.hpp"
#include "camera/rorshadow_data.hpp"
#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rorsystem.hpp"
#include "graphics/rorlight.hpp"
#include "graphics/rorline_soup.hpp"
#include "graphics/rorscene.hpp"
#include "gui/rorgui.hpp"
#include "math/rormatrix4.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include "profiling/rorlog.hpp"
#include "renderer/rorrenderer.hpp"
#include "renderer/rortiler.hpp"
#include "rhi/crtp_interfaces/rorrenderpass.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <vector>

namespace rhi
{

define_translation_unit_vtable(RenderOutput)
{}

define_translation_unit_vtable(RenderTarget)
{}

define_translation_unit_vtable(RenderBuffer)
{}

size_t get_renderer_frequency_index(const ror::Renderer &a_renderer, rhi::ShaderBufferFrequency a_frequency)
{
	return a_renderer.calculate_shader_buffer_index(a_frequency);
}

void Rendersubpass::setup(rhi::RenderCommandEncoder &a_command_encoder, ror::Renderer &a_renderer)
{
	if (this->has_depth())
		a_command_encoder.depth_stencil_state(a_renderer.render_state().depth_state());
	else
		a_command_encoder.depth_stencil_state(a_renderer.render_state().depth_state_less_no_write());

	this->bind_render_inputs(a_command_encoder);
	this->bind_buffer_inputs(a_command_encoder, a_renderer);
	this->bind_input_attachments(a_command_encoder);
}

void Rendersubpass::setup(rhi::ComputeCommandEncoder &a_command_encoder, ror::Renderer &a_renderer)
{
	(void) a_command_encoder;
	(void) a_renderer;

	// do compute setup
	this->bind_render_inputs(a_command_encoder);
	this->bind_buffer_inputs(a_command_encoder, a_renderer);
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
	(void) a_command_encoder; (void) a_scene; (void) a_job_system; (void) a_event_system; (void) a_buffer_pack; (void) a_device; (void) a_timer; (void) a_renderer; (void) a_pass; (void) a_subpass;
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

ror::Tile get_pick_tile(ror::Renderer &a_renderer, rhi::Renderpass &a_pass)
{
	const auto tile_origin     = a_renderer.pick_target();
	const auto tile_dimensions = a_pass.dimensions();

	ror::Tile rect{};
	rect.m_left   = static_cast<uint32_t>((tile_origin.x));
	rect.m_top    = static_cast<uint32_t>((tile_origin.y));
	rect.m_right  = static_cast<uint32_t>(rect.m_left + tile_dimensions.x);
	rect.m_bottom = static_cast<uint32_t>(rect.m_top + tile_dimensions.y);

	return rect;
}

ror::Matrix4f clipspace_crop_matrix(ror::Renderer &a_renderer, rhi::Renderpass &a_pass)
{
	const auto full_dimensions = a_renderer.dimensions();
	auto       rect            = get_pick_tile(a_renderer, a_pass);

	const auto l = (2.0f * static_cast<float32_t>(rect.m_left)) / full_dimensions.x - 1.0f;
	const auto r = (2.0f * static_cast<float32_t>(rect.m_right)) / full_dimensions.x - 1.0f;
	const auto t = 1.0f - (2.0f * static_cast<float32_t>(rect.m_top)) / full_dimensions.y;
	const auto b = 1.0f - (2.0f * static_cast<float32_t>(rect.m_bottom)) / full_dimensions.y;

	return ror::clipspace_crop_matrix(l, r, b, t);
}

void render_pick_tile(ror::Renderer &a_renderer, const ror::OrbitCamera &a_camera)
{
	auto main_pass = a_renderer.get_pass(rhi::RenderpassType::forward_light);
	auto pick_pass = a_renderer.get_pass(rhi::RenderpassType::pick);

	assert(main_pass && "Can't find main pass");
	assert(pick_pass && "Can't find pick pass");

	auto tile = get_pick_tile(a_renderer, *pick_pass);

	const auto  full_dimensions = main_pass->dimensions();
	const auto  full_width      = full_dimensions.x;
	const auto  full_height     = full_dimensions.y;
	const auto  fovyRadians     = ror::to_radians(a_camera.y_fov());
	const auto &cameraPos       = a_camera.eye();
	const auto &cameraRight     = a_camera.right();
	const auto &cameraUp        = a_camera.up();
	const auto &cameraForward   = a_camera.forward();

	auto q = ror::project_screen_rectangle_to_world(ror::Vector4f{static_cast<float32_t>(tile.m_left), static_cast<float32_t>(tile.m_top),
	                                                              static_cast<float32_t>(tile.m_right), static_cast<float32_t>(tile.m_bottom)},
	                                                ror::Vector4ui{0, 0, full_width, full_height}, fovyRadians, 1.0f,        // chosen depth of 1.0f, just after the near plane
	                                                cameraPos, cameraRight, cameraUp, cameraForward);

	auto &ls = ror::line_soup();

	static uint32_t rect{0xFFFFFFFF};

	const uint32_t         lines_count = 4;
	std::vector<float32_t> data{};
	data.reserve(lines_count * 2 * 4);

	ror::Vector3f tl{std::get<0>(q)};
	ror::Vector3f tr{std::get<1>(q)};
	ror::Vector3f br{std::get<2>(q)};
	ror::Vector3f bl{std::get<3>(q)};

	ror::push_point(data, tl, ror::yellow4f);
	ror::push_point(data, tr, ror::yellow4f);

	ror::push_point(data, tr, ror::yellow4f);
	ror::push_point(data, br, ror::yellow4f);

	ror::push_point(data, br, ror::yellow4f);
	ror::push_point(data, bl, ror::yellow4f);

	ror::push_point(data, bl, ror::yellow4f);
	ror::push_point(data, tl, ror::yellow4f);

	rect = ls.push_lines(reinterpret_cast<uint8_t *>(data.data()), lines_count, rect);
}

void pick_pass(rhi::RenderCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system,
               ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
               rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
{
	(void) a_job_system;
	(void) a_timer;

	// TODO: FIXME: Disable blending etc
	auto &camera = a_scene.current_camera();
	camera.upload(a_renderer);

	const auto tile_dimensions = a_pass.dimensions();
	auto       crop            = rhi::clipspace_crop_matrix(a_renderer, a_pass);
	auto       projection      = crop * camera.projection();
	auto       view            = camera.view();
	auto       view_projection = projection * view;

	ror::Matrix4f projection_inverse{};
	ror::Matrix4f view_projection_inverse{};

	auto invertable = projection.inverse(projection_inverse);
	assert(invertable && "Can't invert cropped projection matrix");
	invertable = view_projection.inverse(view_projection_inverse);
	assert(invertable && "Can't invert cropped view_projection matrix");
	(void) invertable;

	{
		auto per_view_uniform = a_renderer.shader_buffer("per_view_uniform");

		per_view_uniform->buffer_map();

		per_view_uniform->update("projection_mat4", &projection.m_values);
		per_view_uniform->update("view_projection_mat4", &view_projection.m_values);
		per_view_uniform->update("inverse_projection_mat4", &projection_inverse.m_values);
		per_view_uniform->update("inverse_view_projection_mat4", &view_projection_inverse.m_values);

		ror::Vector4ui viewport{0, 0, tile_dimensions.x, tile_dimensions.y};
		per_view_uniform->update("viewport", &viewport.x);

		per_view_uniform->buffer_unmap();
	}

	a_scene.render_simple(a_device, a_command_encoder, a_buffer_pack, a_renderer, a_pass, a_subpass, a_event_system);
	a_renderer.signal_pick();

	a_pass.enabled(false);
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
	// TODO: Need to do front face culling
	// TODO: Disable depth clipping so occluder objects near the light are pancaked to z=0, find out how its done in vulkan and metal https://therealmjp.github.io/posts/shadow-maps/ (Disabling Z Clipping)
	/*

	  // Make sure this is only done for 3D objects and not objects like plane which will disappear
	  glCullFace(GL_FRONT);
	  RenderSceneToDepthMap();
	  glCullFace(GL_BACK); // don't forget to reset original culling face

	*/

	static uint32_t cascade_index = 0;

	a_renderer.update_cascade_per_view_uniform(cascade_index);        // The renderer knows about current frame, pass, renderpass as it uses it here to work out which per_view uniform to update

	render_scene(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);

	cascade_index = (cascade_index + 1) % ror::cascade_count;
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

void draw_canonical()
{
	auto    &ls = ror::line_soup(0);
	uint32_t box_id{0xFFFFFFFF};
	uint32_t sphere_id{0xFFFFFFFF};

	uint32_t sphereC_id{0xFFFFFFFF};

	static std::unordered_map<std::string, bool> options = {{"Box", false},
	                                                        {"Sphere", false},
	                                                        {"SphereC", false}};
	if (ImGui::Begin("Canonical test"))
	{
		for (auto &op_it : options)
			ImGui::Checkbox(op_it.first.c_str(), &op_it.second);
	}
	ImGui::End();

	ror::Vector3f        min{-2, -2, -2};
	ror::Vector3f        max{2, 2, 2};
	ror::BoundingBoxf    box{};
	ror::BoundingSpheref sphere{};
	ror::BoundingSpheref sphereC{};

	box.create_from_min_max(min, max);

	sphere.set_center(box.center());
	sphere.set_radius(box.diagonal() / 2.0f);

	sphereC.set_center(box.center());
	sphereC.set_radius(2.0f);

	if (options["Box"])
		box_id = ls.push_box(box, box_id, ror::red4f);

	if (options["Sphere"])
		sphere_id = ls.push_sphere(sphere, sphere_id, 10, ror::green4f);

	if (options["SphereC"])
		sphereC_id = ls.push_sphere(sphereC, sphereC_id, 10, ror::red4f);
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
	camera.upload(a_renderer);

	static bool setup{true};
	static bool should_render_scene{true};
	if (setup)
	{
		a_event_system.subscribe(ror::keyboard_z_command_click, [](ror::Event &) {
			should_render_scene = !should_render_scene;
		});
		setup = false;
	}

	if (should_render_scene)
		render_scene(a_command_encoder, a_scene, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, a_renderer, a_pass, a_subpass);
	else
	{
		const auto &per_view_uniforms = a_renderer.shader_buffer("per_view_uniform");        // Shared per_view_uniform amongst all cameras
		const auto &per_frame_uniform = a_renderer.shader_buffer("per_frame_uniform");
		// const auto &weights_shader_buffer = a_renderer.shader_buffer("morphs_weights");

		// Vertex shader bindings
		per_view_uniforms->buffer_bind(a_command_encoder, rhi::ShaderStage::vertex);

		// Fragment shader bindings
		per_view_uniforms->buffer_bind(a_command_encoder, rhi::ShaderStage::fragment);
		// Fragment shader bindings per frame
		per_frame_uniform->buffer_bind(a_command_encoder, rhi::ShaderStage::fragment);
	}

	// Lets render all the renderer dynamic mesh, usually it only has a skybox cubmap
	// TODO: Check why the order of this and the one above matters, if scene dynamic meshes are rendererd first nothing shows
	// This doesn't render anymore as its using sampler2D but now shadow depth is sampler2DArray
	for (auto &dynamic_mesh : a_renderer.dynamic_meshes())
		dynamic_mesh->render(a_device, a_renderer, a_command_encoder);

	// draw_canonical();

	for (size_t i = 0; i < ror::line_soups_count; ++i)
	{
		auto &lin_soup = ror::line_soup(i);
		lin_soup.render(a_device, a_command_encoder);
	}

	// Lets update the UI elements
	auto &setting = ror::settings();

	ror::gui().draw_gui_windows(a_scene.current_camera(), a_event_system);
	ror::gui().end_frame();

	if (setting.m_generate_gui_mesh && setting.m_gui.m_visible)
		ror::gui().render(a_device, a_renderer, a_command_encoder);

	if (ror::settings().m_draw_pick_tile)
		render_pick_tile(a_renderer, camera);
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

void pick_pass(rhi::ComputeCommandEncoder &a_command_encoder, ror::Scene &a_scene, ror::JobSystem &a_job_system,
               ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
               rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer, rhi::Renderpass &a_pass, rhi::Rendersubpass &a_subpass)
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
	a_command_encoder.compute_pipeline_state(a_device, compute_pso);
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
