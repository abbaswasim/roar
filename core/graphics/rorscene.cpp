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
#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "geometry/rorgeometry_utilities.hpp"
#include "graphics/primitive_geometries.hpp"
#include "graphics/roranimation.hpp"
#include "graphics/rordynamic_mesh.hpp"
#include "graphics/rormesh.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rornode.hpp"
#include "graphics/rorscene.hpp"
#include "gui/rorgui.hpp"
#include "math/rormatrix3.hpp"
#include "math/rormatrix3_functions.hpp"
#include "math/rormatrix4.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorquaternion.hpp"
#include "math/rortransform.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include "math/rorvector_functions.hpp"
#include "profiling/rorlog.hpp"
#include "profiling/rortimer.hpp"
#include "renderer/rorrenderer.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorhandles.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include "shader_system/rorshader_system.hpp"
#include "shader_system/rorshader_update.hpp"
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <mutex>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

namespace ror
{

define_translation_unit_vtable(Scene)
{}

constexpr auto to_renderpasstype(uint32_t a_index)
{
	// This looks like a useless method, but if other renderpass types are introduced, It will give warnings if this method is used, instead of just casting to uint32_t
	auto rpt = static_cast<rhi::RenderpassType>(a_index);
	switch (rpt)
	{
			// clang-format off
		case rhi::RenderpassType::lut:                  return rpt;
		case rhi::RenderpassType::main:                 return rpt;
		case rhi::RenderpassType::depth:                return rpt;
		case rhi::RenderpassType::shadow:               return rpt;
		case rhi::RenderpassType::light_bin:            return rpt;
		case rhi::RenderpassType::reflection:           return rpt;
		case rhi::RenderpassType::refraction:           return rpt;
		case rhi::RenderpassType::pre_process:          return rpt;
		case rhi::RenderpassType::post_process:         return rpt;
		case rhi::RenderpassType::tone_mapping:         return rpt;
		case rhi::RenderpassType::forward_light:        return rpt;
		case rhi::RenderpassType::node_transform:       return rpt;
		case rhi::RenderpassType::deferred_gbuffer:     return rpt;
		case rhi::RenderpassType::reflection_probes:    return rpt;
		case rhi::RenderpassType::image_based_light:    return rpt;
		case rhi::RenderpassType::ambient_occlusion:    return rpt;
		case rhi::RenderpassType::skeletal_transform:   return rpt;
		case rhi::RenderpassType::deferred_clustered:   return rpt;
		case rhi::RenderpassType::image_based_light_lut:return rpt;
	    case rhi::RenderpassType::max:                  assert(0 && "Bad renderpass type index");
			// clang-format on
	}

	assert(0);
	return rpt;
};

constexpr auto renderpasstype_max()
{
	return static_cast<uint32_t>(rhi::RenderpassType::max);
}

Scene::Scene(const std::filesystem::path &a_level, ror::EventSystem &a_event_system)
{
	this->init(a_level, a_event_system);
	this->load(a_level, ResourceSemantic::scenes);
}

define_translation_unit_vtable(Scene::SceneState)
{}

void Scene::setup_cameras(ror::Renderer &a_renderer, ror::EventSystem &a_event_system)
{
	auto dims = a_renderer.dimensions();
	auto bbox = this->bounding_box();

	ror::EventCallback camera_cycle_callback = [this](ror::Event &) {
		size_t cameras_size{this->cameras().size()};

		this->current_camera().disable();

		this->m_current_camera_index++;
		this->m_current_camera_index = this->m_current_camera_index % ror::static_cast_safe<uint32_t>(cameras_size);

		this->current_camera().enable();
	};

	for (auto &cam : this->cameras())
	{
		cam.bounds(dims.x, dims.y);
		cam.volume(bbox.minimum(), bbox.maximum());
	}

	a_event_system.subscribe(ror::keyboard_c_click, camera_cycle_callback);

	// Setup cameras
	for (auto &camera : this->m_cameras)
		camera.init(a_event_system);

	this->current_camera().enable();

	this->update_from_scene_state();

	auto camera_top_view = [this](Event &) {
		auto &cam = this->current_camera();
		cam.orient_top();
	};

	auto camera_bottom_view = [this](Event &) {
		auto &cam = this->current_camera();
		cam.orient_bottom();
	};

	auto camera_left_view = [this](Event &) {
		auto &cam = this->current_camera();
		cam.orient_left();
	};

	auto camera_right_view = [this](Event &) {
		auto &cam = this->current_camera();
		cam.orient_right();
	};

	auto camera_front_view = [this](Event &) {
		auto &cam = this->current_camera();
		cam.orient_front();
	};

	auto camera_back_view = [this](Event &) {
		auto &cam = this->current_camera();
		cam.orient_back();
	};

	a_event_system.subscribe(keyboard_num8_click, camera_top_view);
	a_event_system.subscribe(keyboard_num2_click, camera_bottom_view);
	a_event_system.subscribe(keyboard_num4_click, camera_left_view);
	a_event_system.subscribe(keyboard_num6_click, camera_right_view);
	a_event_system.subscribe(keyboard_num5_click, camera_front_view);
	a_event_system.subscribe(keyboard_num0_click, camera_back_view);

	auto frustum_update = [&a_renderer, this](Event &) {
		auto &camera = this->current_camera();
		camera.setup_frustums();
		a_renderer.update_frustums_geometry(camera);
	};

	Event e;
	frustum_update(e);
	a_event_system.subscribe(keyboard_f_click, frustum_update);
}

void Scene::update_from_scene_state()
{
	if (!this->m_scene_state.m_is_valid || !settings().m_load_scene_state)
		return;

	// take the SceneData and fill stuff from it
	// Since we wrote out current camera now we read it in
	auto &camera = this->current_camera();
	camera.mode(this->m_scene_state.m_camera_mode);

	camera.set_parameters(this->m_scene_state.m_camera_type,
	                      this->m_scene_state.m_camera_width,
	                      this->m_scene_state.m_camera_height,
	                      this->m_scene_state.m_camera_z_near,
	                      this->m_scene_state.m_camera_z_far,
	                      this->m_scene_state.m_camera_center,
	                      this->m_scene_state.m_camera_eye,
	                      this->m_scene_state.m_camera_up,
	                      this->m_scene_state.m_camera_right,
	                      this->m_scene_state.m_camera_forward,
	                      this->m_scene_state.m_camera_minimum,
	                      this->m_scene_state.m_camera_maximum,
	                      this->m_scene_state.m_camera_y_fov,
	                      this->m_scene_state.m_camera_x_mag,
	                      this->m_scene_state.m_camera_y_mag);

	camera.set_from_parameters();
	camera.setup_frustums();
}

void Scene::SceneState::init(std::filesystem::path a_data_path)
{
	this->load(a_data_path, ResourceSemantic::scenes);
}

ror::Vector3f read_vector3(json a_json_object)
{
	std::array<float32_t, 3> c = a_json_object;

	return ror::Vector3f{c[0], c[1], c[2]};
}

void Scene::SceneState::load_specific()
{
	if (!settings().m_load_scene_state)
		return;

	if (this->m_json_file.contains("current_camera"))
	{
		auto &camera = this->m_json_file["current_camera"];
		if (camera.contains("center"))
			this->m_camera_center = read_vector3(camera["center"]);
		if (camera.contains("eye"))
			this->m_camera_eye = read_vector3(camera["eye"]);
		if (camera.contains("right"))
			this->m_camera_right = read_vector3(camera["right"]);
		if (camera.contains("up"))
			this->m_camera_up = read_vector3(camera["up"]);
		if (camera.contains("forward"))
			this->m_camera_forward = read_vector3(camera["forward"]);
		if (camera.contains("minimum"))
			this->m_camera_minimum = read_vector3(camera["minimum"]);
		if (camera.contains("maximum"))
			this->m_camera_maximum = read_vector3(camera["maximum"]);
		if (camera.contains("znear"))
			this->m_camera_z_near = camera["znear"];
		if (camera.contains("zfar"))
			this->m_camera_z_far = camera["zfar"];
		if (camera.contains("width"))
			this->m_camera_width = camera["width"];
		if (camera.contains("height"))
			this->m_camera_height = camera["height"];

		if (camera.contains("mode"))
		{
			auto mode = camera["mode"];
			if (mode == "fps")
				this->m_camera_mode = CameraMode::fps;
			else
				this->m_camera_mode = CameraMode::orbit;
		}
		if (camera.contains("type"))
		{
			auto type = camera["type"];
			if (type == "orthographic")
				this->m_camera_type = CameraType::orthographic;
			else
				this->m_camera_type = CameraType::perspective;
		}

		if (camera.contains("perspective"))
		{
			auto &perspective = camera["perspective"];
			if (perspective.contains("yfov"))
				this->m_camera_y_fov = perspective["yfov"];
		}

		if (camera.contains("orthographic"))
		{
			auto &orthographic = camera["orthographic"];
			if (orthographic.contains("xmag"))
				this->m_camera_x_mag = orthographic["xmag"];
			if (orthographic.contains("ymag"))
				this->m_camera_y_mag = orthographic["ymag"];
		}

		this->m_is_valid = true;
	}
}

template <typename _type>
void replace_next_at(_type a_value, std::string &a_result)
{
	auto index = a_result.find_first_of('@');
	assert(index != std::string::npos);
	std::string replacement{};
	if constexpr (std::is_arithmetic_v<_type>)
		replacement = std::to_string(a_value);
	else
		replacement = a_value;

	a_result.replace(index, 1, replacement);
}

void Scene::SceneState::write_specific()
{
	if (this->m_json_file.contains("current_camera"))
	{
		auto &camera = this->m_json_file["current_camera"];

		std::string patch_str = std::string(R"(
            [
                { "op": "add", "path": "/type", "value": "@" },
                { "op": "add", "path": "/width", "value": @ },
                { "op": "add", "path": "/height", "value": @ },
                { "op": "add", "path": "/zfar", "value": @ },
                { "op": "add", "path": "/znear", "value": @ },
                { "op": "add", "path": "/center", "value": [@, @, @] },
                { "op": "add", "path": "/eye", "value": [@, @, @] },
                { "op": "add", "path": "/up", "value": [@, @, @] },
                { "op": "add", "path": "/right", "value": [@, @, @] },
                { "op": "add", "path": "/forward", "value": [@, @, @] },
                { "op": "add", "path": "/minimum", "value": [@, @, @] },
                { "op": "add", "path": "/maximum", "value": [@, @, @] },
                { "op": "add", "path": "/mode", "value": "@" },
                { "op": "add", "path": "/perspective", "value": {"yfov":@} },
                { "op": "add", "path": "/orthographic", "value":{"xmag":@, "ymag":@} }
            ])");

		replace_next_at(this->m_camera_type == ror::CameraType::perspective ? "perspective" : "orthographic", patch_str);
		replace_next_at(this->m_camera_width, patch_str);
		replace_next_at(this->m_camera_height, patch_str);
		replace_next_at(this->m_camera_z_far, patch_str);
		replace_next_at(this->m_camera_z_near, patch_str);
		replace_next_at(this->m_camera_center.x, patch_str);
		replace_next_at(this->m_camera_center.y, patch_str);
		replace_next_at(this->m_camera_center.z, patch_str);
		replace_next_at(this->m_camera_eye.x, patch_str);
		replace_next_at(this->m_camera_eye.y, patch_str);
		replace_next_at(this->m_camera_eye.z, patch_str);
		replace_next_at(this->m_camera_up.x, patch_str);
		replace_next_at(this->m_camera_up.y, patch_str);
		replace_next_at(this->m_camera_up.z, patch_str);
		replace_next_at(this->m_camera_right.x, patch_str);
		replace_next_at(this->m_camera_right.y, patch_str);
		replace_next_at(this->m_camera_right.z, patch_str);
		replace_next_at(this->m_camera_forward.x, patch_str);
		replace_next_at(this->m_camera_forward.y, patch_str);
		replace_next_at(this->m_camera_forward.z, patch_str);
		replace_next_at(this->m_camera_minimum.x, patch_str);
		replace_next_at(this->m_camera_minimum.y, patch_str);
		replace_next_at(this->m_camera_minimum.z, patch_str);
		replace_next_at(this->m_camera_maximum.x, patch_str);
		replace_next_at(this->m_camera_maximum.y, patch_str);
		replace_next_at(this->m_camera_maximum.z, patch_str);
		replace_next_at(this->m_camera_mode == ror::CameraMode::fps ? "fps" : "orbit", patch_str);
		replace_next_at(this->m_camera_y_fov, patch_str);
		replace_next_at(this->m_camera_x_mag, patch_str);
		replace_next_at(this->m_camera_y_mag, patch_str);

		try
		{
			json patch = json::parse(patch_str);
			camera.patch_inplace(patch);
		}
		catch (const json::exception &e)
		{
			log_critical("Scene state write failed with message {}", e.what());
		}
	}
}

template <typename _node_type>
FORCE_INLINE constexpr void copy_node(_node_type a_node, rhi::ShaderBuffer &a_shader_buffer, uint32_t a_node_index, uint32_t a_stride, size_t a_parent_offset, int32_t a_parent)
{
	a_shader_buffer.update("rotation", &a_node.m_trs_transform.m_rotation.x, a_node_index, a_stride);
	a_shader_buffer.update("translation", &a_node.m_trs_transform.m_translation.x, a_node_index, a_stride);
	a_shader_buffer.update("scale", &a_node.m_trs_transform.m_scale.x, a_node_index, a_stride);
	a_shader_buffer.update("scale_inverse", &a_node.m_trs_transform.m_scale.x, a_node_index, a_stride);

	ror::Vector4i parent_index{a_node.m_parent, 0, 0, 0};

	if (parent_index.x != -1)
		parent_index.x += static_cast_safe<int32_t>(a_parent_offset);
	else if (a_parent_offset != 0)
		parent_index.x = a_parent;

	a_shader_buffer.update("parent_index", &parent_index.x, a_node_index, a_stride);
}

auto copy_node_transforms(ror::Scene &a_scene, rhi::ShaderBuffer &a_input_buffer)
{
	auto &nodes      = a_scene.nodes();
	auto &nodes_data = a_scene.nodes_side_data();

	a_input_buffer.buffer_map();

	auto stride = a_input_buffer.stride("trs_transform_input");

	uint32_t node_index    = 0;
	uint32_t parent_offset = 0;
	for (auto &node : nodes)
	{
		copy_node(node, a_input_buffer, node_index, stride, parent_offset, -1);
		node_index++;
	}

	int32_t parent_index = 0;
	for (auto &node : nodes_data)
	{
		if (node.m_model != -1)
		{
			parent_offset = node_index;

			auto &model_nodes = a_scene.models()[static_cast_safe<size_t>(node.m_model)].nodes();

			for (uint32_t model_node_index = 0; model_node_index < model_nodes.size(); ++model_node_index)
			{
				auto &n = model_nodes[model_node_index];
				copy_node(n, a_input_buffer, node_index, stride, parent_offset, parent_index);
				node_index++;
			}
		}

		parent_index++;
	}

	a_input_buffer.buffer_unmap();
	return node_index;
}

auto &find_trs_buffer(const ror::Renderer &a_renderer, const rhi::Rendersubpass &a_subpass)
{
	auto &input_buffers = a_subpass.buffer_inputs();
	auto &trs_buffers   = input_buffers[0].m_render_output->m_target_reference.get();                  // The 0 mean, we are looking for the first input buffer which is the TRS buffer from renderer.json
	auto  frequency     = a_renderer.calculate_shader_buffer_index(trs_buffers[0].frequency());        // The 0 means the first shader buffer in the list, only used for frequency calculations, the actual TRS buffer is calculated next using the right frequency

	return trs_buffers[frequency];
}

void Scene::compute_pass_walk_scene(rhi::ComputeCommandEncoder &a_command_encoder,
                                    rhi::Device                &a_device,
                                    rhi::BuffersPack           &a_buffers_pack,
                                    ror::Renderer              &a_renderer,
                                    const rhi::Rendersubpass   &a_subpass,
                                    Timer                      &a_timer,
                                    ror::EventSystem           &a_event_system)
{
	(void) a_device;
	(void) a_buffers_pack;
	(void) a_subpass;
	(void) a_timer;
	(void) a_event_system;

	if (this->m_nodes.size() == 0)
		return;

	auto program_id = a_subpass.program_id();

	assert(program_id != -1 && "No program provided for compute pass walk scene");

	auto &compute_pso                       = a_renderer.programs()[static_cast<size_t>(program_id)];
	auto &trs_buffer                        = find_trs_buffer(a_renderer, a_subpass);
	auto  per_frame_uniform                 = a_renderer.shader_buffer("per_frame_uniform");
	auto  nodes_models_uniform              = a_renderer.shader_buffer("nodes_models");
	auto  morphs_weights_uniform            = a_renderer.shader_buffer("morphs_weights");
	auto  node_transform_input_uniform      = a_renderer.shader_buffer("node_transform_input");
	auto  node_transform_output_uniform     = a_renderer.shader_buffer("node_transform_output");
	auto  animations_uniform                = a_renderer.shader_buffer("animations");
	auto  animations_sampler_input_uniform  = a_renderer.shader_buffer("animations_sampler_input");
	auto  animations_sampler_output_uniform = a_renderer.shader_buffer("animations_sampler_output");
	auto  current_animations_uniform        = a_renderer.shader_buffer("current_animations");
	auto  per_view_uniform                  = a_renderer.shader_buffer("per_view_uniform");

	uint32_t ncount = static_cast_safe<uint32_t>(copy_node_transforms(*this, trs_buffer));

	uint32_t animation_size{0u};
	uint32_t animation_count{0u};
	uint32_t sampler_input_size{0u};
	uint32_t sampler_output_size{0u};
	uint32_t weights_output_size{0u};

	get_animation_sizes(*this, animation_size, animation_count, sampler_input_size, sampler_output_size, weights_output_size);

	static float32_t seconds{0.0f};
	seconds += static_cast<float32_t>(a_timer.tick_seconds());

	// TODO: After some time reset the seconds
	if (seconds > 1000.0f)        // TODO: Move the 1000 to settings
		seconds = seconds - static_cast<float32_t>(static_cast<int32_t>(seconds));

	if (this->m_pause_animation)
		animation_count = 0;

	per_frame_uniform->buffer_map();
	per_frame_uniform->update("delta_time", &seconds);
	per_frame_uniform->update("nodes_count", &ncount);
	per_frame_uniform->update("animations_count", &animation_count);
	per_frame_uniform->buffer_unmap();

	uint32_t node_matrices_size = static_cast_safe<uint32_t>(this->m_nodes.size());

	// Encode the pipeline state object and its parameters.
	a_command_encoder.compute_pipeline_state(a_device, compute_pso);
	trs_buffer.buffer_bind(a_command_encoder, rhi::ShaderStage::compute);
	per_frame_uniform->buffer_bind(a_command_encoder, rhi::ShaderStage::compute);

	// Calculate a threadgroup size.
	uint32_t max_thread_group_size = 1024;        // compute_pso->maxTotalThreadsPerThreadgroup();

	if (max_thread_group_size > node_matrices_size)
		max_thread_group_size = node_matrices_size;

	static bool first_run = true;
	if (first_run)
	{
		auto anim_handler = [this](Event) {
			this->m_pause_animation = !this->m_pause_animation;
		};

		auto fill_mode_handler = [this](Event) {
			if (this->m_triangle_fill_mode == rhi::TriangleFillMode::fill)
				this->m_triangle_fill_mode = rhi::TriangleFillMode::lines;
			else
				this->m_triangle_fill_mode = rhi::TriangleFillMode::fill;
		};

		a_event_system.subscribe(ror::keyboard_p_down, anim_handler);
		a_event_system.subscribe(ror::keyboard_t_down, fill_mode_handler);

		rhi::descriptor_update_type buffers_images;

		const rhi::descriptor_variant per_frame_uniform_buffer                 = per_frame_uniform;
		const rhi::descriptor_variant nodes_models_uniform_buffer              = nodes_models_uniform;
		const rhi::descriptor_variant morphs_weights_uniform_buffer            = morphs_weights_uniform;
		const rhi::descriptor_variant node_transform_input_uniform_buffer      = node_transform_input_uniform;
		const rhi::descriptor_variant node_transform_output_uniform_buffer     = node_transform_output_uniform;
		const rhi::descriptor_variant animations_uniform_buffer                = animations_uniform;
		const rhi::descriptor_variant animations_sampler_input_uniform_buffer  = animations_sampler_input_uniform;
		const rhi::descriptor_variant animations_sampler_output_uniform_buffer = animations_sampler_output_uniform;
		const rhi::descriptor_variant current_animations_uniform_buffer        = current_animations_uniform;
		const rhi::descriptor_variant per_view_uniform_buffer                  = per_view_uniform;

		// Most of animation data is in set 0 while per_view_uniform is in set 1
		buffers_images[0].emplace_back(std::make_pair(per_frame_uniform_buffer, 18u));
		buffers_images[0].emplace_back(std::make_pair(nodes_models_uniform_buffer, 19u));
		buffers_images[0].emplace_back(std::make_pair(morphs_weights_uniform_buffer, 29u));
		buffers_images[0].emplace_back(std::make_pair(node_transform_input_uniform_buffer, 0u));
		buffers_images[0].emplace_back(std::make_pair(node_transform_output_uniform_buffer, 1u));
		buffers_images[0].emplace_back(std::make_pair(animations_uniform_buffer, 2u));
		buffers_images[0].emplace_back(std::make_pair(animations_sampler_input_uniform_buffer, 3u));
		buffers_images[0].emplace_back(std::make_pair(animations_sampler_output_uniform_buffer, 4u));
		buffers_images[0].emplace_back(std::make_pair(current_animations_uniform_buffer, 5u));
		buffers_images[1].emplace_back(std::make_pair(per_view_uniform_buffer, 20u));

		// This shader only have
		// layout(std140, set = 0, binding = 18) uniform per_frame_uniform
		// layout(std430, set = 0, binding = 19) buffer nodes_models
		// layout(std430, set = 0, binding = 29) buffer morphs_weights
		// layout(std430, set = 0, binding = 0) buffer node_transform_input
		// layout(std430, set = 0, binding = 1) buffer node_transform_output
		// layout(std430, set = 0, binding = 2) buffer animations
		// layout(std430, set = 0, binding = 3) buffer animations_sampler_input
		// layout(std430, set = 0, binding = 4) buffer animations_sampler_output
		// layout(std430, set = 0, binding = 5) buffer current_animations
		// layout(std140, set = 1, binding = 20) uniform per_view_uniform

		// TODO: Find out if this needs to happen every tick or once
		compute_pso.update_descriptor(a_device, a_renderer, buffers_images, false);
		first_run = false;
	}

	// Encode the compute command.
	a_command_encoder.dispatch_threads({node_matrices_size, 1, 1}, {static_cast<uint32_t>(max_thread_group_size), 1, 1});
}

struct DrawData
{
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *positions{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *texture_coords0{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *texture_coords1{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *texture_coords2{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *normals{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *bent_normals{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *weights0{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *weights1{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *joint_ids0{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *joint_ids1{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *tangents{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *colors0{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *colors1{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *morph_target{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *morph_weights{nullptr};
	rhi::BufferHybrid<rhi::Buffer, rhi::Static> *indices{nullptr};
	rhi::RenderCommandEncoder                   *encoder{nullptr};
};

template <typename _type>
FORCE_INLINE void enable_material_component(const Material::Component<_type>                                            &a_component,
                                            std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>               &a_textures,
                                            std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>>     &a_images,
                                            std::vector<rhi::TextureSampler, rhi::BufferAllocator<rhi::TextureSampler>> &a_samplers,
                                            uint32_t                                                                    &a_index,
                                            DrawData                                                                    &a_dd)
{
	if (a_component.m_texture != -1)
	{
		auto &texture = a_textures[static_cast_safe<size_t>(a_component.m_texture)];
		assert(texture.texture_image() != -1);
		auto &image   = a_images[static_cast_safe<size_t>(texture.texture_image())];
		auto &sampler = a_samplers[texture.texture_sampler()];

		a_dd.encoder->fragment_texture(image, a_index);
		a_dd.encoder->fragment_sampler(sampler, a_index++);        // This behaviour matches what's in the shader_system.cpp
	}
};

void render_mesh(const rhi::Device &a_device, ror::Model &a_model, ror::Mesh &a_mesh, DrawData &a_dd, const ror::Renderer &a_renderer, ror::Scene &a_scene, const rhi::Rendersubpass &subpass)
{
	auto &programs      = a_scene.programs();
	auto &pass_programs = programs.at(subpass.type());

	for (size_t prim_id = 0; prim_id < a_mesh.primitives_count(); ++prim_id)
	{
		auto material_index = a_mesh.material(prim_id);
		assert(material_index != -1 && "Material index can't be -1");
		auto &material         = a_model.materials()[static_cast<uint32_t>(material_index)];
		auto &material_factors = material.shader_buffer();
		// material_factors.bind(a_encoder, rhi::ShaderType::fragment, buffer_index_offset);
		material_factors.buffer_bind(*a_dd.encoder, rhi::ShaderStage::fragment);

		const rhi::Program *pso{nullptr};
		pso = &pass_programs[static_cast<size_t>(a_mesh.program(prim_id))];

		if (subpass.program_id() != -1)        // this means we have a renderer subpass program that overrides what's generated for the primitive
			pso = &a_renderer.program(static_cast<size_t>(subpass.program_id()));

		assert(pso && "There should be a PSO available for the mesh primitive");

		if (!pso->render_pipeline_state())
		{
			ror::log_critical("Mesh primitive {} pso is null", prim_id);
			continue;
		}

		a_dd.encoder->render_pipeline_state(a_device, *pso);

		if (material.m_double_sided)
			a_dd.encoder->cull_mode(rhi::PrimitiveCullMode::none);
		else
			a_dd.encoder->cull_mode(subpass.cull_mode());

		// Bind standard vertex attributes
		auto &vertex_attributes = a_mesh.vertex_descriptor(prim_id);
		for (auto &va : vertex_attributes.attributes())
		{
			rhi::Buffer *va_buffer{nullptr};
			if (va.semantics() == rhi::BufferSemantic::vertex_position)
				va_buffer = a_dd.positions;
			else if (va.semantics() == rhi::BufferSemantic::vertex_texture_coord_0)
				va_buffer = a_dd.texture_coords0;
			else if (va.semantics() == rhi::BufferSemantic::vertex_texture_coord_1)
				va_buffer = a_dd.texture_coords1;
			else if (va.semantics() == rhi::BufferSemantic::vertex_texture_coord_2)
				va_buffer = a_dd.texture_coords2;
			else if (va.semantics() == rhi::BufferSemantic::vertex_normal)
				va_buffer = a_dd.normals;
			else if (va.semantics() == rhi::BufferSemantic::vertex_bent_normal)
				va_buffer = a_dd.bent_normals;
			else if (va.semantics() == rhi::BufferSemantic::vertex_tangent)
				va_buffer = a_dd.tangents;
			else if (va.semantics() == rhi::BufferSemantic::vertex_color_0)
				va_buffer = a_dd.colors0;
			else if (va.semantics() == rhi::BufferSemantic::vertex_color_1)
				va_buffer = a_dd.colors1;
			else if (va.semantics() == rhi::BufferSemantic::vertex_bone_id_0)
				va_buffer = a_dd.joint_ids0;
			else if (va.semantics() == rhi::BufferSemantic::vertex_bone_id_1)
				va_buffer = a_dd.joint_ids1;
			else if (va.semantics() == rhi::BufferSemantic::vertex_weight_0)
				va_buffer = a_dd.weights0;
			else if (va.semantics() == rhi::BufferSemantic::vertex_weight_1)
				va_buffer = a_dd.weights1;
			else if (va.semantics() == rhi::BufferSemantic::vertex_index)
			{}
			else
				assert(0 && "Shouldn't reach here");

			if (va_buffer)
			{
				// Offset is 0 here because we have provided attribute offset in attribute descriptor
				a_dd.encoder->vertex_buffer(*va_buffer, 0, va.location());
			}
			else
			{
				assert(va.semantics() == rhi::BufferSemantic::vertex_index && "Attribute buffer is null for non-index buffer");
			}
		}

		// Bind morph targets vertex attributes
		auto &morph_target_vertex_descriptors = a_mesh.target_descriptor(prim_id);
		for (auto &morph_descriptor : morph_target_vertex_descriptors)
		{
			for (auto &va : morph_descriptor.attributes())
			{
				rhi::Buffer *va_buffer{nullptr};
				if (va.semantics() == rhi::BufferSemantic::vertex_position)
					va_buffer = a_dd.positions;
				else if (va.semantics() == rhi::BufferSemantic::vertex_normal)
					va_buffer = a_dd.normals;
				else if (va.semantics() == rhi::BufferSemantic::vertex_tangent)
					va_buffer = a_dd.tangents;
				else
				{
					assert(0 && "Shouldn't reach here. don't support any other morph target");
				}

				if (va_buffer)
				{
					a_dd.encoder->vertex_buffer(*va_buffer, 0, va.location());
				}
				else
				{
					assert(va.semantics() == rhi::BufferSemantic::vertex_index && "Attribute buffer is null for non-index buffer");
				}
			}
		}

		auto &textures = a_model.textures();
		auto &images   = a_model.images();
		auto &samplers = a_model.samplers();
		auto  binding_index{0u};

		enable_material_component(material.m_base_color, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_diffuse_color, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_specular_glossyness, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_emissive, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_anisotropy, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_transmission, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_sheen_color, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_sheen_roughness, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_clearcoat_normal, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_clearcoat, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_clearcoat_roughness, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_metallic, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_roughness, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_occlusion, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_normal, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_bent_normal, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_height, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_subsurface_color, textures, images, samplers, binding_index, a_dd);

		// Incase binding_index used for anything else after this
		auto &renderer_images   = a_renderer.images();
		auto &renderer_samplers = a_renderer.samplers();
		if (a_scene.has_shadows())
		{
			auto *shadow_texture = a_renderer.get_shadow_texture();
			if (shadow_texture)
			{
				auto &image   = renderer_images[static_cast_safe<size_t>(shadow_texture->texture_image())];
				auto &sampler = renderer_samplers[static_cast_safe<size_t>(shadow_texture->texture_sampler())];

				a_dd.encoder->fragment_texture(image, binding_index);
				a_dd.encoder->fragment_sampler(sampler, binding_index);
			}
			binding_index++;        // To be consistent with shader_system.cpp
		}

		// This shouldn't be done with each mesh. Rather once at render subpass time, find a solution. The reason its done here is because of binding_index
		if (settings().m_environment.m_visible)
		{
			auto &env                    = a_renderer.current_environment();
			auto &skybox_image           = renderer_images[static_cast_safe<size_t>(env.skybox())];        // TODO: I don't think anyone is using this at render time, apart from the skybox itself which is renderered separately
			auto &brdf_integration_image = renderer_images[static_cast_safe<size_t>(env.brdf_integration())];
			auto &irradiance_image       = renderer_images[static_cast_safe<size_t>(env.irradiance())];
			auto &radiance_image         = renderer_images[static_cast_safe<size_t>(env.radiance())];

			auto &sampler = a_renderer.samplers()[static_cast_safe<size_t>(env.skybox_sampler())];

			a_dd.encoder->fragment_sampler(sampler, binding_index);
			a_dd.encoder->fragment_texture(brdf_integration_image, binding_index++);
			a_dd.encoder->fragment_sampler(sampler, binding_index);
			a_dd.encoder->fragment_texture(skybox_image, binding_index++);
			a_dd.encoder->fragment_sampler(sampler, binding_index);
			a_dd.encoder->fragment_texture(irradiance_image, binding_index++);
			a_dd.encoder->fragment_sampler(sampler, binding_index);
			a_dd.encoder->fragment_texture(radiance_image, binding_index++);
		}

		if (a_mesh.has_indices(prim_id))
		{
			auto &index_buffer_attribute = vertex_attributes.attribute(rhi::BufferSemantic::vertex_index);

			if (index_buffer_attribute.count() > 0)
				a_dd.encoder->draw_indexed_primitives(a_mesh.primitive_type(prim_id),
				                                      index_buffer_attribute.count(),
				                                      index_buffer_attribute.format(),
				                                      *a_dd.indices,
				                                      index_buffer_attribute.buffer_offset() + index_buffer_attribute.offset());
		}
		else
		{
			auto &vertex_buffer_attribute = vertex_attributes.attribute(rhi::BufferSemantic::vertex_position);
			a_dd.encoder->draw_primitives(a_mesh.primitive_type(prim_id), 0, vertex_buffer_attribute.count());
		}
	}
}

uint32_t animation_sampler_type(rhi::VertexFormat a_format)
{
	assert((a_format == rhi::VertexFormat::float32_4 ||
	        a_format == rhi::VertexFormat::float32_3 ||
	        a_format == rhi::VertexFormat::float32_1 ||
	        a_format == rhi::VertexFormat::int32_1 ||
	        a_format == rhi::VertexFormat::uint32_1 ||
	        a_format == rhi::VertexFormat::int16_1 ||
	        a_format == rhi::VertexFormat::uint16_1 ||
	        a_format == rhi::VertexFormat::int8_1 ||
	        a_format == rhi::VertexFormat::uint8_1) &&
	       "sampler format is not in the right format");

	if (a_format == rhi::VertexFormat::float32_4)
		return 0;
	else if (a_format == rhi::VertexFormat::float32_3)
		return 1;
	else if (a_format == rhi::VertexFormat::float32_1)
		return 2;
	else if (a_format == rhi::VertexFormat::int32_1)
		return 3;
	else if (a_format == rhi::VertexFormat::uint32_1)
		return 4;
	else if (a_format == rhi::VertexFormat::int16_1)
		return 5;
	else if (a_format == rhi::VertexFormat::uint16_1)
		return 6;
	else if (a_format == rhi::VertexFormat::int8_1)
		return 7;
	else if (a_format == rhi::VertexFormat::uint8_1)
		return 8;

	assert(0 && "Shouldn't reach here");

	return 0;
}

void get_animation_sizes(ror::Scene &a_scene,
                         uint32_t   &a_animation_size,
                         uint32_t   &a_animation_count,
                         uint32_t   &a_sampler_input_size,
                         uint32_t   &a_sampler_output_size,
                         uint32_t   &a_weights_output_size)
{
	// Lets upload animations data
	for (auto &node : a_scene.nodes_side_data())
	{
		if (node.m_model != -1)
		{
			auto &model = a_scene.models()[static_cast_safe<size_t>(node.m_model)];
			for (auto &model_node : model.nodes())
			{
				if (model_node.m_mesh_index != -1)
				{
					auto &mesh = model.meshes()[static_cast<size_t>(model_node.m_mesh_index)];
					if (mesh.has_morphs())
						a_weights_output_size += ror::static_cast_safe<uint32_t>(mesh.weights_count());
				}
			}

			auto current_anim{0u};
			for (auto &anim : model.animations())
			{
				a_animation_size += ror::static_cast_safe<uint32_t>(anim.m_channels.size());

				if (current_anim == node.m_animation)
					a_animation_count++;

				current_anim++;

				for (auto &sampler : anim.m_samplers)
				{
					a_sampler_input_size += ror::static_cast_safe<uint32_t>(sampler.m_input.size());
					a_sampler_output_size += ror::static_cast_safe<uint32_t>(sampler.m_output.size() / sizeof(float32_t));
					// a_sampler_output_size = ror::align16(a_sampler_output_size); // TODO: Fix me when other types are supported their start should be aligned
				}
			}
		}
	}
}

void fill_morph_weights(ror::Scene &a_scene, rhi::ShaderBuffer &a_shader_buffer, uint32_t a_weights_size)
{
	a_shader_buffer.buffer_map();

	auto stride = a_shader_buffer.stride("morph_weights");
	assert(stride == sizeof(float32_t) && "morph_weights are not packed");
	(void) stride;

	auto morph_offset{0u};
	for (auto &node : a_scene.nodes_side_data())
	{
		if (node.m_model != -1)
		{
			auto &model = a_scene.models()[static_cast_safe<size_t>(node.m_model)];
			for (auto &model_node : model.nodes())
			{
				if (model_node.m_mesh_index != -1)
				{
					auto &mesh = model.meshes()[static_cast<size_t>(model_node.m_mesh_index)];
					if (mesh.has_morphs())
					{
						a_shader_buffer.update("morph_weights", morph_offset * sizeof(float32_t), mesh.weights().data(), static_cast_safe<uint32_t>(mesh.weights().size() * sizeof(float32_t)));
						morph_offset += ror::static_cast_safe<uint32_t>(mesh.weights_count());
					}
				}
			}
		}
	}

	a_shader_buffer.buffer_unmap();

	(void) a_weights_size;
	assert(a_weights_size == morph_offset && "Not all of the weights are copied");
}

std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> morph_weights_offsets_count(ror::Scene &a_scene)
{
	std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> output;

	uint32_t node_offset{static_cast_safe<uint32_t>(a_scene.nodes().size())};

	auto morph_offset{0u};
	for (auto &node : a_scene.nodes_side_data())
	{
		if (node.m_model != -1)
		{
			auto &model = a_scene.models()[static_cast_safe<size_t>(node.m_model)];
			for (auto &model_node : model.nodes())
			{
				if (model_node.m_mesh_index != -1)
				{
					auto &mesh = model.meshes()[static_cast<size_t>(model_node.m_mesh_index)];
					if (mesh.has_morphs())
					{
						output[node_offset] = std::make_pair(morph_offset, mesh.weights_count());
						morph_offset += ror::static_cast_safe<uint32_t>(mesh.weights_count());
					}
				}
				node_offset++;
			}
		}
	}

	return output;
}

void fill_animation_buffers(ror::Scene &a_scene, ror::Renderer &a_renderer)
{
	auto animation_buffer         = a_renderer.shader_buffer("animations");
	auto sampler_input_buffer     = a_renderer.shader_buffer("animations_sampler_input");
	auto sampler_output_buffer    = a_renderer.shader_buffer("animations_sampler_output");
	auto current_animation_buffer = a_renderer.shader_buffer("current_animations");

	auto anim_size = sizeof(ror::Vector4ui) * 3;
	auto animation_size{0u};
	auto animation_count{0u};
	auto sampler_input_size{0u};
	auto sampler_output_size{0u};
	auto weights_output_size{0u};

	// This returns amounts of floats in input and output, output might still be float[1-3]
	get_animation_sizes(a_scene, animation_size, animation_count, sampler_input_size, sampler_output_size, weights_output_size);

	std::vector<uint8_t>        anim_data;
	std::vector<ror::Vector2ui> current_anim_data;
	std::vector<float32_t>      sampler_input_data;
	std::vector<float32_t>      sampler_output_data;
	std::vector<float32_t>      weights_output_data;

	anim_data.resize(animation_size * anim_size);        // For all animations, the animation struct has 2 uvec4s
	current_anim_data.reserve(animation_size);           // Very conservative, reserve for all animatoins
	sampler_input_data.resize(sampler_input_size);
	sampler_output_data.resize(sampler_output_size);
	weights_output_data.reserve(weights_output_size);

	auto anim_data_ptr           = anim_data.data();
	auto sampler_input_data_ptr  = reinterpret_cast<uint8_t *>(sampler_input_data.data());
	auto sampler_output_data_ptr = reinterpret_cast<uint8_t *>(sampler_output_data.data());

	auto     sampler_input_offset{0u};
	auto     sampler_output_offset{0u};
	uint32_t model_anim_index{0u};

	uint32_t node_offset{static_cast_safe<uint32_t>(a_scene.nodes().size())};

	auto node_to_offset{morph_weights_offsets_count(a_scene)};

	auto weights_offset{0u};
	auto weights_count{0u};
	for (auto &node : a_scene.nodes_side_data())
	{
		if (node.m_model != -1)
		{
			auto &model = a_scene.models()[static_cast_safe<size_t>(node.m_model)];
			auto  current_anim{0u};
			for (auto &anim : model.animations())
			{
				if (current_anim == node.m_animation)
					current_anim_data.emplace_back(model_anim_index, static_cast<uint32_t>(anim.m_channels.size()));

				model_anim_index += ror::static_cast_safe<uint32_t>(anim.m_channels.size());
				for (auto &chanl : anim.m_channels)
				{
					auto &anim_sampler = anim.m_samplers[chanl.m_sampler_index];

					// Encode channel for the compute shader
					ror::Vector4ui channel{chanl.m_sampler_index,
					                       static_cast_safe<uint32_t>(anim_sampler.m_input.size()),
					                       chanl.m_target_node_index + node_offset,
					                       ror::enum_to_type_cast(chanl.m_target_node_path)};

					// Encode sampler for the compute shader
					ror::Vector4ui sampler{*reinterpret_cast<const uint32_t *>(&anim_sampler.m_minimum),
					                       *reinterpret_cast<const uint32_t *>(&anim_sampler.m_maximum),
					                       animation_sampler_type(anim_sampler.m_output_format),
					                       ror::enum_to_type_cast(anim_sampler.m_interpolation)};

					auto i = node_to_offset.find(chanl.m_target_node_index + node_offset);
					if (i != node_to_offset.end())
					{
						weights_offset = i->second.first;
						weights_count  = i->second.second;
					}
					else
					{
						weights_offset = 0u;
						weights_count  = 0u;
					}

					// Encode offets
					ror::Vector4ui offsets{sampler_input_offset,
					                       sampler_output_offset,
					                       weights_offset,
					                       weights_count};

					// Lets advance the offset
					sampler_input_offset += ror::static_cast_safe<uint32_t>(anim_sampler.m_input.size());
					sampler_output_offset += ror::static_cast_safe<uint32_t>(anim_sampler.m_output.size() / sizeof(float32_t));

					std::memcpy(anim_data_ptr, &channel.x, sizeof(ror::Vector4ui));
					anim_data_ptr += sizeof(ror::Vector4ui);

					std::memcpy(anim_data_ptr, &sampler.x, sizeof(ror::Vector4ui));
					anim_data_ptr += sizeof(ror::Vector4ui);

					std::memcpy(anim_data_ptr, &offsets.x, sizeof(ror::Vector4ui));
					anim_data_ptr += sizeof(ror::Vector4ui);

					// Lets copy its sampler as well
					auto input_size  = anim_sampler.m_input.size() * sizeof(float32_t);
					auto output_size = anim_sampler.m_output.size();

					std::memcpy(sampler_input_data_ptr, anim_sampler.m_input.data(), input_size);
					std::memcpy(sampler_output_data_ptr, anim_sampler.m_output.data(), output_size);

					sampler_input_data_ptr += input_size;
					sampler_output_data_ptr += output_size;
				}
				current_anim++;
			}
			node_offset += static_cast_safe<uint32_t>(model.nodes().size());
		}
	}

	{
		auto stride = animation_buffer->stride("animation");

		anim_data_ptr = anim_data.data();

		animation_buffer->buffer_map();
		for (uint32_t i = 0; i < animation_size; ++i)
		{
			auto anim_data_offset = i * anim_size;
			auto anim_ptr         = anim_data_ptr + anim_data_offset;

			animation_buffer->update("animation_channel", anim_ptr, i, stride);

			anim_ptr += sizeof(ror::Vector4ui);
			animation_buffer->update("animation_sampler", anim_ptr, i, stride);

			anim_ptr += sizeof(ror::Vector4ui);
			animation_buffer->update("node_offsets", anim_ptr, i, stride);
		}
		animation_buffer->buffer_unmap();
	}

	{
		auto stride = current_animation_buffer->stride("current_animation");
		assert(animation_count == current_anim_data.size() && "Animations not fully collected");
		assert(stride == sizeof(ror::Vector2ui) && "Stride is wrong");
		(void) stride;
		current_animation_buffer->buffer_map();

		auto i{0u};
		for (auto &canim : current_anim_data)
			current_animation_buffer->update("current_animation", &canim.x, i++, stride);

		current_animation_buffer->buffer_unmap();
	}

	if (animation_size)
	{
		sampler_input_data_ptr = reinterpret_cast<uint8_t *>(sampler_input_data.data());

		sampler_input_buffer->buffer_map();
		sampler_input_buffer->update("inputs", sampler_input_data_ptr, static_cast_safe<uint32_t>(sampler_input_data.size() * sizeof(float32_t)));
		sampler_input_buffer->buffer_unmap();
	}

	if (animation_size)
	{
		sampler_output_data_ptr = reinterpret_cast<uint8_t *>(sampler_output_data.data());

		sampler_output_buffer->buffer_map();
		sampler_output_buffer->update("outputs", sampler_output_data_ptr, static_cast_safe<uint32_t>(sampler_output_data.size() * sizeof(float32_t)));
		sampler_output_buffer->buffer_unmap();
	}
}

void Scene::pre_render(rhi::RenderCommandEncoder &a_encoder, rhi::BuffersPack &a_buffers_pack, ror::Renderer &a_renderer, const rhi::Rendersubpass &a_subpass)
{
	(void) a_buffers_pack;
	(void) a_subpass;
	(void) a_renderer;
	(void) a_encoder;

	for (auto &light : this->m_lights)
		light.update(a_renderer.current_frame_index());

	if (this->m_indices_dirty)
	{
		ror::Vector4ui node_index{0};        // Index contains index of the node in the resolved matrix array, and index offset for other things like joints
		for (auto &node : this->m_nodes_data)
		{
			node.update_offsets(node_index);
			node_index.x++;
		}

		auto morph_offset{0u};
		for (auto &node : this->m_nodes_data)
		{
			if (node.m_model != -1)
			{
				node_index.y = node_index.x;

				auto &model       = this->m_models[static_cast_safe<size_t>(node.m_model)];
				auto &model_nodes = model.nodes_side_data();

				for (uint32_t model_node_index = 0; model_node_index < model_nodes.size(); ++model_node_index)
				{
					auto &model_node = model.nodes()[model_node_index];
					if (model_node.m_mesh_index != -1)
					{
						auto &mesh = model.meshes()[static_cast<size_t>(model_node.m_mesh_index)];
						if (mesh.has_morphs())
						{
							node_index.z = morph_offset;
							morph_offset += ror::static_cast_safe<uint32_t>(mesh.weights_count());
						}
						else
						{
							node_index.z = 0;
						}
					}
					model_nodes[model_node_index].update_offsets(node_index);
					node_index.x++;
				}
			}
		}
		this->m_indices_dirty = false;
	}

	static bool first_time = true;        // TODO: Remove and add somewhere else
	if (first_time)
	{
		fill_animation_buffers(*this, a_renderer);
		first_time = false;
	}
}

void Scene::reset_to_default_state(ror::Renderer &a_renderer, rhi::RenderCommandEncoder &a_encoder, const rhi::Renderpass &a_pass, const rhi::Rendersubpass &a_subpass)
{
	(void) a_subpass;

	auto dimensions = a_pass.dimensions();

	a_encoder.cull_mode(rhi::PrimitiveCullMode::back);                               // No face culling
	a_encoder.front_facing_winding(rhi::PrimitiveWinding::counter_clockwise);        // What apis like Metal requires
	a_encoder.depth_stencil_state(a_renderer.render_state().depth_state_less_equal());
	a_encoder.viewport({0.0f, 0.0f, static_cast<float32_t>(dimensions.x), static_cast<float32_t>(dimensions.y)}, {0.0f, 1.0f});
	a_encoder.scissor({0, 0, static_cast<uint32_t>(dimensions.x), static_cast<uint32_t>(dimensions.y)});
}

void Scene::fill_scene_data()
{
	auto &camera = this->current_camera();

	this->m_scene_state.m_camera_center  = camera.center();
	this->m_scene_state.m_camera_eye     = camera.eye();
	this->m_scene_state.m_camera_right   = camera.right();
	this->m_scene_state.m_camera_up      = camera.up();
	this->m_scene_state.m_camera_forward = camera.forward();
	this->m_scene_state.m_camera_minimum = camera.minimum();
	this->m_scene_state.m_camera_maximum = camera.maximum();
	this->m_scene_state.m_camera_y_fov   = camera.y_fov();
	this->m_scene_state.m_camera_z_near  = camera.z_near();
	this->m_scene_state.m_camera_z_far   = camera.z_far();
	this->m_scene_state.m_camera_width   = camera.width();
	this->m_scene_state.m_camera_height  = camera.height();
	this->m_scene_state.m_camera_x_mag   = camera.x_mag();
	this->m_scene_state.m_camera_y_mag   = camera.y_mag();
	this->m_scene_state.m_camera_mode    = camera.mode();
	this->m_scene_state.m_camera_type    = camera.type();
}

const Light *Scene::light(ror::Light::LightType a_type) const
{
	for (auto &light : this->m_lights)
	{
		if (light.m_type == a_type)
			return &light;
	}

	return nullptr;
}

const Light *Scene::directional_light() const
{
	return this->light(ror::Light::LightType::directional);
}

const Light *Scene::spot_light() const
{
	return this->light(ror::Light::LightType::spot);
}

const Light *Scene::point_light() const
{
	return this->light(ror::Light::LightType::point);
}

const Light *Scene::area_light() const
{
	return this->light(ror::Light::LightType::area);
}

void Scene::render(const rhi::Device &a_device, rhi::RenderCommandEncoder &a_encoder, rhi::BuffersPack &a_buffers_pack, ror::Renderer &a_renderer, const rhi::Renderpass &a_pass, const rhi::Rendersubpass &a_subpass, ror::EventSystem &a_event_system)
{
	(void) a_pass;
	(void) a_event_system;
	(void) a_device;
	(void) a_encoder;
	(void) a_buffers_pack;
	(void) a_renderer;
	(void) a_subpass;

	a_encoder.triangle_fill_mode(this->m_triangle_fill_mode);
	this->reset_to_default_state(a_renderer, a_encoder, a_pass, a_subpass);

	DrawData dd;

	dd.positions       = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_position);
	dd.texture_coords0 = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_texture_coord_0);
	dd.texture_coords1 = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_texture_coord_1);
	dd.texture_coords2 = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_texture_coord_2);
	dd.normals         = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_normal);
	dd.bent_normals    = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_bent_normal);
	dd.weights0        = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_weight_0);
	dd.weights1        = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_weight_1);
	dd.joint_ids0      = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_bone_id_0);
	dd.joint_ids1      = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_bone_id_1);
	dd.tangents        = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_tangent);
	dd.colors0         = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_color_0);
	dd.colors1         = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_color_1);
	dd.morph_target    = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_morph_target);
	dd.morph_weights   = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_morph_weight);
	dd.indices         = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_index);

	assert(dd.positions);
	assert(dd.texture_coords0);
	assert(dd.texture_coords1);
	assert(dd.texture_coords2);
	assert(dd.normals);
	assert(dd.bent_normals);
	assert(dd.weights0);
	assert(dd.weights1);
	assert(dd.joint_ids0);
	assert(dd.joint_ids1);
	assert(dd.tangents);
	assert(dd.colors0);
	assert(dd.colors1);
	assert(dd.morph_target);
	assert(dd.morph_weights);
	assert(dd.indices);

	dd.encoder = &a_encoder;

	const auto &per_view_uniforms     = a_renderer.shader_buffer("per_view_uniform");        // Shared per_view_uniform amongst all cameras
	const auto &per_frame_uniform     = a_renderer.shader_buffer("per_frame_uniform");
	const auto &weights_shader_buffer = a_renderer.shader_buffer("morphs_weights");

	// Vertex shader bindings
	per_view_uniforms->buffer_bind(a_encoder, rhi::ShaderStage::vertex);

	// Fragment shader bindings
	per_view_uniforms->buffer_bind(a_encoder, rhi::ShaderStage::fragment);

	// Fragment shader bindings per frame
	per_frame_uniform->buffer_bind(a_encoder, rhi::ShaderStage::fragment);

	for (auto &light_type : this->m_lights)
		light_type.shader_buffer()[a_renderer.current_frame_index()].buffer_bind(a_encoder, rhi::ShaderStage::fragment);

	const auto &index_buffer_out = a_renderer.shader_buffer("nodes_models");
	index_buffer_out->buffer_bind(a_encoder, rhi::ShaderStage::vertex);

	this->pre_render(a_encoder, a_buffers_pack, a_renderer, a_subpass);

	// Render the scene graph
	for (auto &node : this->m_nodes_data)
	{
		if (node.m_model != -1)
		{
			// TODO: Test cull mode for DoubleSided material how does that work
			auto &model            = this->m_models[static_cast_safe<size_t>(node.m_model)];
			auto &meshes           = model.meshes();
			auto &model_nodes_data = model.nodes_side_data();

			size_t node_data_index = 0;
			for (auto &model_node : model.nodes())
			{
				if (model_node.m_mesh_index != -1 && model_node.m_visible)
				{
					auto &mesh = meshes[static_cast<size_t>(model_node.m_mesh_index)];
					model_nodes_data[node_data_index].bind(a_encoder, rhi::ShaderStage::vertex);
					if (mesh.skin_index() != -1 && model_node.m_skin_index != -1)
					{
						assert(mesh.skin_index() == model_node.m_skin_index && "Mesh and Node skin indices should be the same");
						auto &skin = model.skins()[static_cast<size_t>(model_node.m_skin_index)];
						skin.bind_joint_offset_buffer(a_encoder, rhi::ShaderStage::vertex);
						skin.bind_inverse_bind_buffer(a_encoder, rhi::ShaderStage::vertex);
					}

					if (mesh.has_morphs())
					{
						weights_shader_buffer->buffer_bind(a_encoder, rhi::ShaderStage::vertex);
					}

					a_encoder.front_facing_winding(model_node.m_winding);

					render_mesh(a_device, model, mesh, dd, a_renderer, *this, a_subpass);
				}
				node_data_index++;
			}
		}
	}

	// Don't render dynamic meshes in shadow pass
	assert(a_pass.subpasses().size() && "There are no subpasses in the renderpass");
	if (a_pass.subpasses()[0].type() != rhi::RenderpassType::shadow)
	{
		// Lets render all the dynamic meshes before we render GUI
		this->reset_to_default_state(a_renderer, a_encoder, a_pass, a_subpass);
		for (auto &dm : this->m_dynamic_meshes)
			dm.render(a_device, a_renderer, a_encoder);

		// Lets render all the renderer dynamic mesh, usually it only has a skybox cubmap
		// TODO: Check why the order of this and the one above matters, if scene dynamic meshes are rendererd first nothing shows
		this->reset_to_default_state(a_renderer, a_encoder, a_pass, a_subpass);
		for (auto &dm : a_renderer.dynamic_meshes())
			dm->render(a_device, a_renderer, a_encoder);

		// Lets update the UI elements
		// TODO: Should move out of scene and into global end of frame renderpass
		auto &setting = ror::settings();

		if (setting.m_generate_gui_mesh && setting.m_gui.m_visible)
			ror::gui().render(a_device, a_renderer, a_encoder, this->m_cameras[0], a_event_system);        // TODO: Fix the camera
	}
}

void Scene::update(ror::Renderer &a_renderer, ror::Timer &a_timer)
{
	(void) a_timer;
	(void) a_renderer;

	// auto &camera = this->m_cameras[this->m_current_camera_index];
	// camera.update(a_renderer);
}

void Scene::update_bounding_box()
{
	size_t node_id = 0;
	for (auto &node : this->nodes_side_data())
	{
		if (node.m_model != -1)
		{
			auto &model = this->m_models[static_cast_safe<size_t>(node.m_model)];
			auto  bbox  = model.bounding_box_scaled();

			// TODO: These needs to be traversed to the root to be correct
			auto &translation = this->m_nodes[node_id].m_trs_transform.m_translation;
			auto &rotation    = this->m_nodes[node_id].m_trs_transform.m_rotation;
			auto &scale       = this->m_nodes[node_id].m_trs_transform.m_scale;
			auto  min         = bbox.minimum();
			auto  max         = bbox.maximum();
			auto  min_res     = translation + (rotation * (scale * min));
			auto  max_res     = translation + (rotation * (scale * max));
			auto  box         = ror::BoundingBoxf(min_res, max_res);

			this->m_bounding_box.add_bounding(box);
		}
		node_id++;
	}

	auto extent = this->m_bounding_box.maximum() - this->m_bounding_box.minimum();
	if (extent.x < 0.5f || extent.y < 0.5f)
	{
		this->m_bounding_box.set(ror::Vector3f(-1.0f), ror::Vector3f(1.0f));
	}
}

uint32_t Scene::models_count()
{
	auto model_nodes{0u};
	for (auto &node : this->m_nodes_data)
		if (node.m_model_path != "")
			model_nodes++;

	return model_nodes;
}

void Scene::add_model_node(int32_t a_model_index)
{
	ror::SceneNode     node{};
	ror::SceneNodeData node_data{};
	node_data.m_model = a_model_index;

	this->m_nodes.emplace_back(std::move(node));
	this->m_nodes_data.emplace_back(std::move(node_data));
}

void Scene::add_node()
{
	this->add_model_node(-1);
}

// NOTE: This method is not thread safe
void Scene::create_global_program(const char *a_vertex_shader,
                                  const char *a_fragment_shader,
                                  size_t      a_node_id,
                                  size_t      a_model_id)
{
	// Lets load and upload its shaders
	int32_t vid = static_cast_safe<int32_t>(this->m_shaders.size());
	int32_t fid = vid + 1;

	// This is ok here because generate_shaders hasn't been called yet
	this->m_shaders.emplace_back(rhi::load_shader<rhi::Shader>(a_vertex_shader));
	this->m_shaders.back().compile();
	this->m_shaders.emplace_back(rhi::load_shader<rhi::Shader>(a_fragment_shader));
	this->m_shaders.back().compile();

	GlobalProgram gb;

	gb.program  = rhi::Program{vid, fid};
	gb.node_id  = static_cast_safe<int32_t>(a_node_id);        // Remember I haven't added a node for this yet, so the size will be its index when I call add_model_node()
	gb.model_id = static_cast_safe<int32_t>(a_model_id);

	uint32_t mesh_index = 0;
	for (auto &mesh : this->m_models[a_model_id].meshes())
	{
		gb.mesh_id = static_cast_safe<int32_t>(mesh_index);

		for (size_t prim_index = 0; prim_index < mesh.primitives_count(); ++prim_index)
		{
			// Don't assign the program id here, we will do it in generate_shaders
			gb.primitive_id = static_cast_safe<int32_t>(prim_index);

			int32_t mesh_program_index = -1;
			for (uint32_t i = 0; i < renderpasstype_max(); ++i)
			{
				auto &pass_global_programs = this->m_global_programs[to_renderpasstype(i)];

				// Some sanity checks
				if (mesh_program_index == -1)
				{
					mesh_program_index = static_cast_safe<int32_t>(pass_global_programs.size());
				}
				else
				{
					assert(static_cast_safe<int32_t>(pass_global_programs.size()) == mesh_program_index && "Some renderpass programs are not properly initialized");
				}

				pass_global_programs.emplace_back(gb);
			}
		}
		mesh_index++;
	}
}

// Obsolete method. Here to show how one can create a model with a global program
void Scene::generate_grid_model(ror::JobSystem &a_job_system, const std::function<bool(size_t)> &a_upload_job, std::vector<ror::JobHandle<bool>> &a_job_handles, size_t a_model_index, rhi::BuffersPack &a_buffer_pack)
{
	auto grid_generation_job = [this, &a_buffer_pack](size_t model_index, size_t node_index) -> auto {
		Model &model = this->m_models[model_index];
		model.create_grid(false, a_buffer_pack);

		this->create_global_program("grid.glsl.vert", "grid.glsl.frag", node_index, model_index);

		return true;
	};

	// Add scene nodes for this model
	auto node_index = this->m_nodes.size();
	this->add_model_node(static_cast_safe<int32_t>(a_model_index));        // Doing this outside nested jobs because its not thread safe

	// Save the model ID somewhere for quick access
	// this->m_grid_model_id = static_cast_safe<int32_t>(a_model_index);

	// kick off grid generation job
	auto grid_job_handle   = a_job_system.push_job(grid_generation_job, a_model_index, node_index);
	auto upload_job_handle = a_job_system.push_job(a_upload_job, grid_job_handle.job(), a_model_index);

	a_job_handles.emplace_back(std::move(grid_job_handle));
	a_job_handles.emplace_back(std::move(upload_job_handle));
}

auto trs_to_matrix4(ror::Transformf &a_trs)
{
	ror::Matrix4f translation = ror::matrix4_translation(a_trs.translation());
	ror::Matrix4f rotation    = ror::matrix4_rotation(a_trs.rotation());
	ror::Matrix4f scale       = ror::matrix4_scaling(a_trs.scale());

	return translation * rotation * scale;
}

template <typename _node_container, typename _node_type>
auto get_node_global_transform(_node_container &a_model, _node_type &a_node)
{
	auto         &model_nodes = a_model.nodes();
	ror::Matrix4f node_matrix{trs_to_matrix4(a_node.m_trs_transform)};
	_node_type   *node = &a_node;

	while (node->m_parent != -1)
	{
		auto &p     = model_nodes[static_cast<size_t>(node->m_parent)];
		node_matrix = trs_to_matrix4(p.m_trs_transform) * node_matrix;

		node = &p;
	}

	return node_matrix;
}

// Good reference for how to add different geometry to debug data
// static void create_lights(std::vector<std::vector<float32_t>> &a_debug_data, const std::vector<ror::Light> &a_lights, std::vector<rhi::PrimitiveTopology> &a_topology_data)
// {
// 	ror::Vector3f light_position{-3.0f, 3.0f, 0.0f};

// 	ror::Vector4f color{1.0f, 0.647f, 0.0f, 1.0f};        // orange color
// 	srgb_to_linear(color);

// 	for (auto &light : a_lights)
// 	{
// 		color = ror::Vector4f{light.m_color, 1.0f};

// 		if (light.m_type == ror::Light::LightType::directional)
// 		{
// 			ror::Vector3f f{0.0f, 1.0f, 0.0f};        // direction of the arrow which is Y-up
// 			auto          rot = ror::matrix3_rotation(f, light.m_direction);

// 			std::vector<ror::Vector3f> arrow_triangles_data{};
// 			create_arrow(arrow_triangles_data, ror::Vector3f{1.0f});

// 			std::vector<float32_t> arrow_triangles{};

// 			float32_t offsetx[3] = {-0.1f, 0.0f, 0.1f};
// 			for (int32_t i = 0; i < 3; ++i)
// 			{
// 				ror::Vector3f offset{offsetx[i], 0.0f, 0.0f};
// 				for (auto point : arrow_triangles_data)
// 				{
// 					point = rot * point;
// 					point += light_position + offset;

// 					arrow_triangles.push_back(point.x);
// 					arrow_triangles.push_back(point.y);
// 					arrow_triangles.push_back(point.z);
// 					arrow_triangles.push_back(0.0f);

// 					arrow_triangles.push_back(color.x);
// 					arrow_triangles.push_back(color.y);
// 					arrow_triangles.push_back(color.z);
// 					arrow_triangles.push_back(color.w);
// 				}
// 			}

// 			a_debug_data.emplace_back(arrow_triangles);
// 			a_topology_data.emplace_back(rhi::PrimitiveTopology::triangles);
// 		}
// 		else if (light.m_type == ror::Light::LightType::point)
// 		{
// 			light_position = light.m_position;

// 			std::vector<ror::Vector3f> sphere_triangles_data{};
// 			make_sphere_triangles(sphere_triangles_data, 2);

// 			std::vector<float32_t> sphere_triangles{};

// 			for (auto &point : sphere_triangles_data)
// 			{
// 				point += light_position;

// 				sphere_triangles.push_back(point.x);
// 				sphere_triangles.push_back(point.y);
// 				sphere_triangles.push_back(point.z);
// 				sphere_triangles.push_back(0.0f);

// 				sphere_triangles.push_back(color.x);
// 				sphere_triangles.push_back(color.y);
// 				sphere_triangles.push_back(color.z);
// 				sphere_triangles.push_back(color.w);
// 			}

// 			a_debug_data.emplace_back(sphere_triangles);
// 			a_topology_data.emplace_back(rhi::PrimitiveTopology::triangles);
// 		}
// 		else if (light.m_type == ror::Light::LightType::spot)
// 		{}
// 	}
// }

// Does not create a job and is run on main thread
void Scene::generate_debug_model(size_t a_model_index, rhi::BuffersPack &a_buffer_pack)
{
	// Add scene nodes for this model
	auto node_index = this->m_nodes.size();
	this->add_model_node(static_cast_safe<int32_t>(a_model_index));

	auto &setting = ror::settings();

	std::vector<std::vector<float32_t>> debug_data{};
	std::vector<rhi::PrimitiveTopology> primitive_data{};

	std::vector<ror::Vector3f> sphere_triangles_data{};
	make_sphere_triangles(sphere_triangles_data, 2);
	std::vector<ror::Vector3f> box_lines_data{};
	make_box_lines(box_lines_data, ror::Vector3f{0.0}, ror::Vector3f{1.0});
	std::vector<ror::Vector3f> box_triangles_data{};
	make_box_triangles(box_triangles_data, ror::Vector3f{0.0}, ror::Vector3f{1.0});

	auto box_type      = true;
	auto topology_type = rhi::PrimitiveTopology::lines;

	std::vector<ror::Vector3f> &geometry_data = box_lines_data;        // By default lets draw boxes
	if (setting.m_debug_mesh_type == Settings::DebugMeshType::triangle_sphere)
	{
		geometry_data = sphere_triangles_data;
		box_type      = false;
		topology_type = rhi::PrimitiveTopology::triangles;
	}
	else if (setting.m_debug_mesh_type == Settings::DebugMeshType::triangle_box)
	{
		geometry_data = box_triangles_data;
		topology_type = rhi::PrimitiveTopology::triangles;
	}
	else if (setting.m_debug_mesh_type == Settings::DebugMeshType::line_sphere)
	{
		assert(0 && "Currently unsupported debug mode");
	}

	ror::Vector4f color{setting.m_debug_mesh_color};

#define add_pos4_col4(output, input, scale, offset, xform)   \
	for (size_t i = 0; i < geometry_data.size(); ++i)        \
	{                                                        \
		auto xtdata = xform * ((input[i] * scale) + offset); \
		output.push_back(xtdata.x);                          \
		output.push_back(xtdata.y);                          \
		output.push_back(xtdata.z);                          \
		output.push_back(0.0f);                              \
                                                             \
		output.push_back(color.x);                           \
		output.push_back(color.y);                           \
		output.push_back(color.z);                           \
		output.push_back(color.w);                           \
	}                                                        \
	(void) 0

	if (setting.m_debug_mesh_type != Settings::DebugMeshType::none)
	{
		size_t node_id = 0;
		for (auto &node : this->m_nodes_data)
		{
			if (node.m_model != -1)
			{
				auto scene_node_xform = get_node_global_transform(*this, this->m_nodes[node_id]);

				auto &model  = this->m_models[static_cast_safe<size_t>(node.m_model)];
				auto &meshes = model.meshes();

				for (auto &model_node : model.nodes())
				{
					if (model_node.m_mesh_index != -1)
					{
						auto  node_xform = get_node_global_transform(model, model_node);
						auto  xform      = scene_node_xform * node_xform;
						auto &mesh       = meshes[static_cast<size_t>(model_node.m_mesh_index)];

						for (size_t prim_index = 0; prim_index < mesh.primitives_count(); ++prim_index)
						{
							ror::BoundingBoxf &bbox        = mesh.bounding_box(prim_index);
							auto               min         = bbox.minimum();
							auto               sphere_size = ror::Vector3f{bbox.diagonal() / 2};
							auto               box_size    = bbox.extent();
							auto               box_center  = bbox.center();

							auto &scale  = box_type ? box_size : sphere_size;
							auto &offset = box_type ? min : box_center;

							std::vector<float32_t> points_colors{};
							add_pos4_col4(points_colors, geometry_data, scale, offset, xform);

							debug_data.emplace_back(points_colors);
							primitive_data.emplace_back(topology_type);
						}
					}
				}
			}
			node_id++;
		}
	}

	// create the axis
	if (setting.m_show_axis)
		create_axis(debug_data, primitive_data);

	Model &model = this->m_models[a_model_index];
	model.create_debug(false, debug_data, primitive_data, a_buffer_pack);

	this->create_global_program("debug.glsl.vert", "debug.glsl.frag", node_index, a_model_index);

#undef add_pos4_col4
}

void Scene::load_models(ror::JobSystem &a_job_system, rhi::Device &a_device, const ror::Renderer &a_renderer, ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffers_packs)
{
	auto &setting = ror::settings();
	auto  model_nodes{this->models_count()};

	// Add node placeholders all the procedurally created models here
	if (setting.m_generate_debug_mesh)
		model_nodes++;        // for debug model

	if (model_nodes > 0)
	{
		this->m_models.resize(model_nodes);        // NOTE: I am resizing the models vector because I don't want many threads to emplace to it at the same time
		std::vector<ror::JobHandle<bool>> job_handles;
		job_handles.reserve(model_nodes);

		auto model_load_job = [this, &a_buffers_packs](const std::string &node_model_path, size_t a_model_index, bool a_generate_shaders) -> auto {
			log_info("Loading model {}", node_model_path.c_str());
			Model &model = this->m_models[a_model_index];
			model.load_from_gltf_file(node_model_path, this->m_cameras, this->m_lights, a_generate_shaders, a_buffers_packs);

			return true;
		};

		auto model_index{0u};
		for (auto &node : this->m_nodes_data)
		{
			if (node.m_model_path != "")
			{
				auto load_job_handle = a_job_system.push_job(model_load_job, node.m_model_path, model_index, node.m_program_id == -1);
				job_handles.emplace_back(std::move(load_job_handle));
				model_index++;
			}
		}

		// Wait for all jobs to finish
		for (auto &jh : job_handles)
			if (!jh.data())
				ror::log_critical("Can't load models specified in the scene.");

		// Generate debug geometries into the debug model slot we have allocated
		// This needs to happen after all models are loaded and can't be a job that isn't finished before generate_shaders is called below
		// This is why its called on main thread
		if (setting.m_generate_debug_mesh)
		{
			generate_debug_model(model_index, a_buffers_packs);
			model_index++;
		}

		assert(model_index == model_nodes && "Models count vs how many are queued and loaded doesn't match");
	}

	// Lets create and upload the stuff required for the UI as well
	auto gui_gen_job_handle = a_job_system.push_job([&a_device, &a_event_system, &setting]() -> auto {if (setting.m_generate_gui_mesh) ror::gui().init(a_device, a_event_system); return true; });

	this->read_programs();        // Now do this because I can only do this after all the models are loaded and glslang is initialized, it can't be done in the config load

	// Lets kick off shader generation
	auto shader_gen_job_handle = a_job_system.push_job([this, &a_renderer, &a_job_system]() -> auto { this->generate_shaders(a_renderer, a_job_system); return true; });

	if (!shader_gen_job_handle.data())
		ror::log_critical("Can't generate model shaders.");

	if (!gui_gen_job_handle.data())
		ror::log_critical("Can't generate ui.");

	this->update_bounding_box();
	this->make_overlays();
	this->verify_nodes_data_shader_buffer(a_renderer);
}

void Scene::upload_models(ror::JobSystem &a_job_system, rhi::Device &a_device, const ror::Renderer &a_renderer, rhi::BuffersPack &a_buffers_packs)
{
	auto &setting = ror::settings();

	auto model_nodes{this->models_count()};

	// Add node placeholders all the procedurally created models here
	if (setting.m_generate_debug_mesh)
		model_nodes++;        // for debug model

	assert(this->m_models.size() == model_nodes && "Models are not loaded");

	if (model_nodes > 0)
	{
		std::vector<ror::JobHandle<bool>> job_handles;
		job_handles.reserve(model_nodes);        // Multiplied by 2 because I am creating two jobs, load and upload per model

		auto model_upload_job = [this, &a_device](size_t a_index) -> auto {
			Model &model = this->m_models[a_index];
			model.upload(a_device);        // I can't confirm if doing this in multiple threads is defined behaviour or not. But https://developer.apple.com/forums/thread/93346 seems to suggest its ok
			return true;
		};

		auto model_index{0u};
		for (auto &node : this->m_nodes_data)
		{
			if (node.m_model_path != "")
			{
				auto upload_job_handle = a_job_system.push_job(model_upload_job, model_index);
				job_handles.emplace_back(std::move(upload_job_handle));
				model_index++;
			}
		}

		// Generate debug geometries into the debug model slot we have allocated
		// This needs to happen after all models are loaded and can't be a job that isn't finished before generate_shaders is called below
		// This is why its called on main thread
		if (setting.m_generate_debug_mesh)
		{
			auto upload_job_handle = a_job_system.push_job(model_upload_job, model_index);
			job_handles.emplace_back(std::move(upload_job_handle));
			model_index++;
		}

		// Wait for all jobs to finish
		for (auto &jh : job_handles)
			if (!jh.data())
				ror::log_critical("Can't load models specified in the scene.");

		assert(model_index == model_nodes && "Models count vs how many are queued and loaded doesn't match");
	}

	// Lets create and upload the stuff required for the UI as well
	auto gui_gen_job_handle = a_job_system.push_job([&a_device, &a_renderer, &setting]() -> auto {if (setting.m_generate_gui_mesh) ror::gui().upload(a_device, a_renderer); return true; });

	// By this time the buffer pack should be primed and filled with all kinds of geometry and animatiom data, lets upload it, all in one go
	// TODO: find out this might need to be done differently for Vulkan, also should be moved to upload()
	a_buffers_packs.upload(a_device);

	if (!gui_gen_job_handle.data())
		ror::log_critical("Can't generate ui.");
}

#define scene_state_name "_state.json"

void Scene::init(const std::filesystem::path &a_level, ror::EventSystem &a_event_system)
{
	install_input_handlers(a_event_system);

	auto state_file = std::filesystem::path{a_level}.stem().string() + scene_state_name;

	try
	{
		this->m_scene_state.init(state_file);
	}
	catch (const json::exception &e)
	{
		log_critical("Scene state file not found or its invalid {}", e.what());

		auto  abs_path       = find_resource(a_level, ror::ResourceSemantic::scenes);
		auto  abs_state_path = std::filesystem::path{abs_path}.stem().string() + scene_state_name;
		auto &re             = ror::resource(abs_state_path, ror::ResourceSemantic::scenes, ror::ResourceAction::create, "assets");

		bytes_vector data{};
		std::string  empty_state_contents{"{\"current_camera\": {}}"};

		data.insert(data.begin(), empty_state_contents.begin(), empty_state_contents.end());
		re.update({data.begin(), data.end()}, true, false, true);        // Force updating the Resource because I am sure no one else is using it
		re.flush();

		try
		{
			this->m_scene_state.init(state_file);
			this->m_scene_state.m_is_valid = false;
		}
		catch (const json::exception &e)
		{
			log_critical("After creating empty scene state file loading failed with error {}", e.what());
		}
	}
}

void Scene::shutdown(std::filesystem::path a_level, ror::EventSystem &a_event_system)
{
	if (settings().m_save_scene_state)
	{
		this->fill_scene_data();
		this->m_scene_state.write(std::filesystem::path{a_level}.stem().string() + scene_state_name, ror::ResourceSemantic::scenes);
	}
	uninstall_input_handlers(a_event_system);
}

#undef scene_state_name

void Scene::install_input_handlers(ror::EventSystem &)
{}

void Scene::uninstall_input_handlers(ror::EventSystem &)
{}

void Scene::verify_nodes_data_shader_buffer(const ror::Renderer &a_renderer) const
{
	// Only want to do this in debug builds, not critical for run time
	if constexpr (ror::get_build() == ror::BuildType::build_debug)
	{
		const auto &nodes_offsets_shader_buffer = a_renderer.shader_buffer("nodes_offsets");

		for (const auto &nodes_data : this->m_nodes_data)
		{
			if (!(nodes_data.shader_buffer() == *nodes_offsets_shader_buffer))
			{
				assert(0 && "Renderer's defined nodes_offset buffer is not the same as whats inside 'NodeData'");
			}
		}

		for (const auto &models : this->m_models)
		{
			for (const auto &mnsd : models.nodes_side_data())
			{
				if (!(mnsd.shader_buffer() == *nodes_offsets_shader_buffer))
				{
					assert(0 && "Renderer's defined nodes_offset buffer is not the same as whats inside 'NodeData'");
				}
			}
		}
	}
}

void Scene::make_overlays()
{
	auto &ui       = ror::gui();
	auto &overlays = ui.overlays();

	overlays.add(this->m_lights);
	overlays.add(this->m_cameras);
}

hash_64_t pass_aware_vertex_hash(rhi::RenderpassType a_passtype, const ror::Mesh &a_mesh, size_t a_prim_index, const std::vector<ror::Skin, rhi::BufferAllocator<ror::Skin>> &a_skins)
{
	// NOTE: If ever need to change how this hashing works, make sure to change in rormodel.cpp hash gen as well.
	// Ideally it should be abstracted out
	hash_64_t vertex_hash{};

	auto &vertex_attribute_descriptor              = a_mesh.vertex_descriptor(a_prim_index);
	auto &morph_target_vertex_attribute_descriptor = a_mesh.target_descriptor(a_prim_index);

	// Setup vertex hash
	vertex_hash = vertex_attribute_descriptor.hash_64_pass_aware(a_passtype);

	for (auto &attrib : morph_target_vertex_attribute_descriptor)
		hash_combine_64(vertex_hash, attrib.hash_64_pass_aware(a_passtype));

	// Only check if we have weights
	auto weights_count = a_mesh.weights_count();

	if (weights_count > 0)
		hash_combine_64(vertex_hash, hash_64(&weights_count, sizeof(weights_count)));

	auto skin_index = a_mesh.skin_index();
	if (skin_index != -1)
	{
		const auto &skin = a_skins[ror::static_cast_safe<size_t>(skin_index)];
		hash_combine_64(vertex_hash, skin.joints_count());
	}

	return vertex_hash;
}

hash_64_t pass_aware_fragment_hash(hash_64_t a_fragment_hash, hash_64_t a_vertex_hash)
{
	// Since a_vertex_hash is already pass aware, nothing more to do here but to combine existing fragment has with vertex hash
	auto fragment_hash = a_vertex_hash;
	hash_combine_64(fragment_hash, a_fragment_hash);

	return fragment_hash;
}

auto Scene::find_global_program(rhi::RenderpassType a_passtype, uint32_t a_model_id, uint32_t a_mesh_id, size_t a_prim_id, Scene::GlobalProgram **a_global_program)
{
	for (size_t i = 0; i < this->m_global_programs[a_passtype].size(); ++i)
	{
		auto &gb = this->m_global_programs[a_passtype][i];

		if (gb.model_id == static_cast_safe<int32_t>(a_model_id) &&
		    gb.mesh_id == static_cast_safe<int32_t>(a_mesh_id) &&
		    gb.primitive_id == static_cast_safe<int32_t>(a_prim_id))
		{
			*a_global_program = &this->m_global_programs[a_passtype][i];

			return true;
		}
	}

	return false;
}

void Scene::generate_shaders(const ror::Renderer &a_renderer, ror::JobSystem &a_job_system)
{
	const std::vector<rhi::RenderpassType> render_pass_types = a_renderer.render_pass_types();

	size_t shaders_count = 0;

	for (auto &model : this->m_models)
		for (auto &mesh : model.meshes())
			shaders_count += mesh.primitives_count();

	// For each render pass in the framegraph create programs the model meshes can use

	for (auto &passtype : render_pass_types)
		if (passtype == rhi::RenderpassType::shadow)
			this->m_has_shadows = true;

	// This should be shaders_count * 2 * render_passes.size();
	// Then fill me up in a loop before entering the next loop with jobs in it
	this->m_shaders.reserve(shaders_count * 2 * render_pass_types.size());

	log_warn("About to create {} shaders ", shaders_count * 2 * render_pass_types.size());

	// Two pass approach, first create all the shaders into m_shaders, then allocate each to a job to fill it in with data
	std::unordered_map<hash_64_t, std::atomic_flag> shader_hash_to_flag{};         // Used to check if this is generated or not
	std::unordered_map<hash_64_t, size_t>           shader_hash_to_index{};        // Keeps the shader index by hash
	std::unordered_set<hash_64_t>                   unique_shaders{};              // All unique shader hashes

#define create_hash_to_index(shader_hash, shader_type, extension)                                           \
	{                                                                                                       \
		auto res = unique_shaders.emplace(shader_hash);                                                     \
		if (res.second)                                                                                     \
		{                                                                                                   \
			shader_hash_to_index[shader_hash] = this->m_shaders.size();                                     \
			shader_hash_to_flag.emplace(shader_hash, false);                                                \
			const auto shader_name = std::string(std::to_string(shader_hash) + pass_string + extension);    \
			this->m_shaders.emplace_back(shader_name, shader_hash, shader_type, ror::ResourceAction::make); \
		}                                                                                                   \
	}                                                                                                       \
	(void) 0

	for (auto &passtype : render_pass_types)
	{
		this->m_programs[passtype] = {};
		auto &pass_programs        = this->m_programs[passtype];
		auto  pass_string          = std::string("_") + rhi::renderpass_type_to_string(passtype);        // This will be the generated shader name but mostly the first pass will be dominent, because the rest won't be unique
		pass_programs.reserve(shaders_count);                                                            // Pre-reserve so no reallocation happen under the hood while jobs are using it, times 2 for both vertex and fragment

		// Create vertex and fragment shaders but don't generate sources yet
		uint32_t model_index = 0;
		for (auto &model : this->m_models)
		{
			uint32_t mesh_index = 0;
			for (auto &mesh : model.meshes())
			{
				for (size_t prim_index = 0; prim_index < mesh.primitives_count(); ++prim_index)
				{
					// auto vs_hash = mesh.vertex_hash(prim_index);
					auto vs_hash     = pass_aware_vertex_hash(passtype, mesh, prim_index, model.skins());
					auto old_fs_hash = mesh.fragment_hash(prim_index);
					auto fs_hash     = pass_aware_fragment_hash(old_fs_hash, vs_hash);

					// At this point you either have a per render pass shader available for the model or you generate it
					size_t program_index = pass_programs.size();
					if (model.generate_shaders())
					{
						create_hash_to_index(vs_hash, rhi::ShaderType::vertex, ".vert");
						create_hash_to_index(fs_hash, rhi::ShaderType::fragment, ".frag");

						pass_programs.emplace_back(shader_hash_to_index[vs_hash], shader_hash_to_index[fs_hash]);

						// This is set once for the first render pass but it must stay the same for all of the rest because every pass must contain the same amount of shaders
						if (mesh.program(prim_index) == -1)
							mesh.program(static_cast_safe<uint32_t>(prim_index), static_cast_safe<int32_t>(program_index));
					}
					else
					{
						shader_hash_to_flag.emplace(vs_hash, true);
						shader_hash_to_flag.emplace(fs_hash, true);

						GlobalProgram *gb{nullptr};
						auto           res = find_global_program(passtype, model_index, mesh_index, prim_index, &gb);
						(void) res;
						assert(res && "Couldn't find global program for render pass");

						pass_programs.emplace_back(gb->program);

						// This is set once for the first render pass but it must stay the same for all of the rest because every pass must contain the same amount of shaders
						if (mesh.program(prim_index) == -1)
							mesh.program(static_cast_safe<uint32_t>(prim_index), static_cast_safe<int32_t>(program_index));
						else
						{
							assert(mesh.program(prim_index) == static_cast_safe<int32_t>(program_index) && "Something went wrong, these indices should be the same");
						}
					}
				}
				++mesh_index;
			}
			++model_index;
		}
	}

#undef create_hash_to_index

	std::vector<ror::JobHandle<bool>> job_handles;
	job_handles.reserve(shaders_count * 2);        // Multiplied by 2 because I am creating two jobs for each vertex and fragment shaders

	for (auto &passtype : render_pass_types)
	{
		// Generate vertex and fragment shaders for pre allocated shaders
		for (auto &model : this->m_models)
		{
			uint32_t mesh_index = 0;
			for (auto &mesh : model.meshes())
			{
				for (size_t prim_index = 0; prim_index < mesh.primitives_count(); ++prim_index)
				{
					// auto  vs_hash         = mesh.vertex_hash(prim_index);
					auto old_fs_hash = mesh.fragment_hash(prim_index);
					auto vs_hash     = pass_aware_vertex_hash(passtype, mesh, prim_index, model.skins());
					auto fs_hash     = pass_aware_fragment_hash(old_fs_hash, vs_hash);

					if (!shader_hash_to_flag[vs_hash].test_and_set())        // Not generated yet
					{
						auto &vs_shader_index = shader_hash_to_index[vs_hash];
						auto &vs_shader       = this->m_shaders[vs_shader_index];
						auto  vs_job_handle   = a_job_system.push_job([mesh_index, prim_index, passtype, &a_renderer, &vs_shader, &model]() -> auto {
                            auto vs = ror::generate_primitive_vertex_shader(model, mesh_index, static_cast_safe<uint32_t>(prim_index), passtype, a_renderer);
                            vs_shader.source(vs);
                            vs_shader.compile();
                            return true;
                        });

						job_handles.emplace_back(std::move(vs_job_handle));
					}

					if (!shader_hash_to_flag[fs_hash].test_and_set())        // Not generated yet
					{
						auto &fs_shader_index = shader_hash_to_index[fs_hash];
						auto &fs_shader       = this->m_shaders[fs_shader_index];
						auto  fs_job_handle   = a_job_system.push_job([prim_index, passtype, has_shadows = this->m_has_shadows, &fs_shader, &mesh, &model, &a_renderer]() -> auto {
                            auto fs = ror::generate_primitive_fragment_shader(mesh, model.materials(), static_cast_safe<uint32_t>(prim_index), passtype, a_renderer, has_shadows);
                            fs_shader.source(fs);
                            fs_shader.compile();
                            return true;
                        });

						job_handles.emplace_back(std::move(fs_job_handle));
					}
				}
				++mesh_index;
			}
		}
	}

	for (auto &jh : job_handles)
		if (!jh.data())
			ror::log_critical("Can't load shaders specified required for the scene.");

	if constexpr (get_build() == BuildType::build_debug)
	{
		auto &shaders = shader_hash_to_flag;
		for (auto &s : shaders)
		{
			(void) s;
			assert(s.second.test_and_set() == true && "Not all unique shaders are generated");
		}

		for (auto &passtype1 : render_pass_types)
		{
			auto &pass_programs1 = this->m_programs[passtype1];
			auto  size           = pass_programs1.size();
			(void) size;
			for (auto &passtype2 : render_pass_types)
			{
				auto &pass_programs2 = this->m_programs[passtype2];
				assert(pass_programs2.size() == size && "Something went wrong, sizes don't match");
				(void) pass_programs2;
			}
		}
	}

	log_warn("Actual number of shaders created {} ", this->m_shaders.size());

	// Just a cleanup for global programs
	this->m_global_programs.clear();

	// Sign up all shaders for updates
	this->push_shader_updates(a_renderer);
}

rhi::Rendersubpass *get_render_pass_by_types(const ror::Renderer &a_renderer, rhi::RenderpassType a_type)
{
	for (auto &rp : a_renderer.current_frame_graph())
	{
		auto &subpasses = rp.subpasses();
		for (auto &srp : subpasses)
		{
			if (srp.type() == a_type)
				return &srp;
		}
	}

	return nullptr;
}

void Scene::push_shader_updates(const ror::Renderer &a_renderer)
{
	auto &render_passes = a_renderer.current_frame_graph();
	auto &dependencies  = ror::mesh_shaders_dependencies();
	auto &updator       = shader_updater();
	auto  has_shadows   = this->m_has_shadows;
	auto &shaders       = this->m_shaders;
	auto  with_environment{settings().m_environment.m_visible};

	static std::mutex cache_mutex;        // Mutex to lock the lambda cache access

	size_t records{0};
	for (auto &rpass : render_passes)
	{
		for (auto &rsubpass : rpass.subpasses())
		{
			auto &programs = this->m_programs[rsubpass.type()];
			for (auto &model : this->m_models)
			{
				uint32_t mesh_index = 0;
				for (auto &mesh : model.meshes())
				{
					for (size_t prim_index = 0; prim_index < mesh.primitives_count(); ++prim_index)
					{
						auto shader_update = [&model, this, mesh_index, prim_index, &a_renderer,
						                      &pass    = std::as_const(rpass),
						                      &subpass = std::as_const(rsubpass),
						                      &mesh, has_shadows, with_environment](rhi::Device &device, std::unordered_set<hash_64_t> *cache) {
							assert(cache && "Scene shaders can't be re-created without a cache, otherwise it takes too long");
							auto &prgs = this->m_programs[subpass.type()];
							auto &prg  = prgs[static_cast<size_t>(mesh.program(prim_index))];

							auto &textures       = model.textures();
							auto &images         = model.images();
							auto &samplers       = model.samplers();
							auto &materials      = model.materials();
							auto  material_index = mesh.material(prim_index);
							assert(material_index != -1 && "Material index can't be -1");
							auto &material = materials[static_cast<size_t>(material_index)];

							const ror::Skin *skin{nullptr};
							if (mesh.skin_index() != -1)
								skin = &model.skins()[static_cast<size_t>(mesh.skin_index())];

							if (prg.vertex_id() != -1)
							{
								auto &shdr = this->m_shaders[static_cast<size_t>(prg.vertex_id())];
								bool  cache_hit;
								{
									std::lock_guard<std::mutex> lock{cache_mutex};
									auto                        res = cache->emplace(shdr.hash());
									cache_hit                       = res.second;
								}
								if (cache_hit)
								{
									auto vs = ror::generate_primitive_vertex_shader(model, mesh_index, static_cast_safe<uint32_t>(prim_index), subpass.type(), a_renderer);
									shdr.source(vs);
									shdr.compile();
									shdr.upload(device);
								}
							}

							if (prg.fragment_id() != -1)
							{
								auto &shdr = this->m_shaders[static_cast<size_t>(prg.fragment_id())];
								bool  cache_hit;
								{
									std::lock_guard<std::mutex> lock{cache_mutex};
									auto                        res = cache->emplace(shdr.hash());
									cache_hit                       = res.second;
								}
								if (cache_hit)
								{
									auto fs = ror::generate_primitive_fragment_shader(mesh, model.materials(), static_cast_safe<uint32_t>(prim_index), subpass.type(), a_renderer, has_shadows);
									shdr.source(fs);
									shdr.compile();
									shdr.upload(device);
								}
							}

							prg.build_descriptor(device, a_renderer, this->m_shaders, this, &material, &textures, &images, &samplers, skin, has_shadows, with_environment);

							prg.upload(device, pass, subpass, this->m_shaders, model, mesh_index, static_cast<uint32_t>(prim_index), false);
						};

						updator.push_program_record(mesh.program(prim_index), programs, shaders, shader_update, &dependencies);

						records++;
					}
					++mesh_index;
				}
			}
		}
	}

	ror::log_info("{} Shader update records created for the scene programs", records);
}

void Scene::deferred_upload(rhi::Device &a_device, ror::JobSystem &a_job_system, const ror::Renderer &a_renderer)
{
	auto program_descriptor_upload_job = [&a_device](rhi::Program &a_program) -> auto {
		a_program.update_descriptor(a_device);

		return true;
	};

	auto &render_passes = a_renderer.current_frame_graph();

	// Lets do this in parallel
	// Upload all the shader programs, this creates pipelines in metal and vulkan cases
	std::vector<ror::JobHandle<bool>> job_handles;
	uint32_t                          programs_count = 1000;        // This should be big enough, otherwise its no big deal
	job_handles.reserve(programs_count);

	for (auto &pass : render_passes)
	{
		for (auto &subpass : pass.subpasses())
		{
			if (subpass.technique() != rhi::RenderpassTechnique::fragment)
				continue;

			auto    &pass_programs = this->m_programs[subpass.type()];
			uint32_t program       = 0;
			for (auto &model : this->m_models)
			{
				for (auto &mesh : model.meshes())
				{
					for (size_t prim_index = 0; prim_index < mesh.primitives_count(); ++prim_index)
					{
						auto upload_job_handle = a_job_system.push_job(program_descriptor_upload_job,
						                                               std::ref(pass_programs[program]));

						job_handles.emplace_back(std::move(upload_job_handle));

						program++;
					}
				}
			}
		}
	}

	for (auto &jh : job_handles)
		if (!jh.data())
			ror::log_critical("Can't upload all programs descriptors for all models in all the render passes.");
}

void Scene::upload(rhi::Device &a_device, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system, ror::Renderer &a_renderer, rhi::BuffersPack &a_buffers_packs)
{
	this->upload_models(a_job_system, a_device, a_renderer, a_buffers_packs);
	this->init_upload_debug_geometry(a_device, a_renderer);

	auto &render_passes = a_renderer.current_frame_graph();
	auto  with_environment{ror::settings().m_environment.m_visible};

	// Now lets upload them
	for (auto &shader : this->m_shaders)
	{
		shader.upload(a_device);
	}

	auto program_upload_job = [&a_device, &a_renderer, this, with_environment](rhi::Program                   &a_program,
	                                                                           const std::vector<rhi::Shader> &a_shaders,
	                                                                           const ror::Model &a_model, uint32_t a_mesh_index, uint32_t a_prim_index,
	                                                                           const rhi::Renderpass &a_pass, const rhi::Rendersubpass &a_subpass, bool a_need_shadow_map) -> auto {
		auto &textures       = a_model.textures();
		auto &images         = a_model.images();
		auto &samplers       = a_model.samplers();
		auto &meshes         = a_model.meshes();
		auto &mesh           = meshes[a_mesh_index];
		auto &materials      = a_model.materials();
		auto  material_index = mesh.material(a_prim_index);
		assert(material_index != -1 && "Material index can't be -1");
		auto &material = materials[static_cast<size_t>(material_index)];

		const ror::Skin *skin{nullptr};
		if (mesh.skin_index() != -1)
			skin = &a_model.skins()[static_cast<size_t>(mesh.skin_index())];

		a_program.build_descriptor(a_device, a_renderer, this->m_shaders, this, &material, &textures, &images, &samplers, skin, a_need_shadow_map, with_environment);
		// Need to call a_program.update_descriptor later at when we know what resource is used by each shader

		a_program.upload(a_device, a_pass, a_subpass, a_shaders, a_model, a_mesh_index, a_prim_index, false);

		return true;
	};

	// Lets do this in parallel
	// Upload all the shader programs, this creates pipelines in metal and vulkan cases
	std::vector<ror::JobHandle<bool>> job_handles;
	uint32_t                          programs_count = 1000;        // This should be big enough, otherwise its no big deal
	job_handles.reserve(programs_count);

	for (auto &pass : render_passes)
	{
		for (auto &subpass : pass.subpasses())
		{
			if (subpass.technique() != rhi::RenderpassTechnique::fragment)
				continue;

			auto    &pass_programs = this->m_programs[subpass.type()];
			uint32_t program       = 0;
			for (auto &model : this->m_models)
			{
				uint32_t mesh_index = 0;
				for (auto &mesh : model.meshes())
				{
					for (size_t prim_index = 0; prim_index < mesh.primitives_count(); ++prim_index)
					{
						auto upload_job_handle = a_job_system.push_job(program_upload_job,
						                                               std::ref(pass_programs[program]),
						                                               std::cref(this->m_shaders),
						                                               std::cref(model),
						                                               mesh_index,
						                                               static_cast_safe<uint32_t>(prim_index),
						                                               std::ref(pass),
						                                               std::ref(subpass),
						                                               this->m_has_shadows);

						job_handles.emplace_back(std::move(upload_job_handle));

						program++;
					}
					mesh_index++;
				}
			}
		}
	}

	// Upload lights
	for (auto &light : this->m_lights)
	{
		light.upload(a_device);
	}

	// Upload cameras
	for (auto &camera : this->m_cameras)
		camera.upload(a_device);

	// Upload all nodes shader buffers
	for (auto &node : this->m_nodes_data)
		node.upload(a_device);

	for (auto &model : this->m_models)
		for (auto &node : model.nodes_side_data())
			node.upload(a_device);

	for (auto &jh : job_handles)
		if (!jh.data())
			ror::log_critical("Can't upload all programs for all models in all the render passes.");

	// Special treatment of weights UBO that needs filling up from mesh static weights unlike other ones, although these might get animated later
	// Thats why its uploaded in renderer

	// Sanity check that all pipelines for all render passes are generated correctly
	const std::vector<rhi::RenderpassType> render_pass_types = a_renderer.render_pass_types();

	for (auto &passtype1 : render_pass_types)
	{
		auto &pass_programs1 = this->m_programs[passtype1];

		for (auto &pprgs : pass_programs1)
		{
			auto rsp = pprgs.render_pipeline_state();
			auto csp = pprgs.compute_pipeline_state();

			if (rsp == nullptr && csp == nullptr)
				ror::log_critical("Pass type {}, program {} is null {}, {}", rhi::renderpass_type_to_string(passtype1), reinterpret_cast<void *>(&pprgs), reinterpret_cast<void *>(rsp), reinterpret_cast<void *>(csp));
		}
	}

	this->deferred_upload(a_device, a_job_system, a_renderer);
	this->setup_cameras(a_renderer, a_event_system);
}

void Scene::read_nodes()
{
	assert(this->m_json_file.contains("nodes") && "Provided scene file is not a roar scene.");

	// Read all the nodes
	auto    nodes = this->m_json_file["nodes"];
	int32_t model_index{0u};
	for (auto &node : nodes)
	{
		SceneNode     nod;
		SceneNodeData nod_data;

		nod_data.name(node["name"]);

		if (node.contains("children"))
			nod_data.children(node["children"].get<std::vector<uint32_t>>());

		if (node.contains("translation"))
		{
			std::array<float32_t, 3> t = node["translation"];
			nod.m_trs_transform.translation({t[0], t[1], t[2]});
		}

		if (node.contains("rotation"))
		{
			std::array<float32_t, 4> r = node["rotation"];
			nod.m_trs_transform.rotation({r[0], r[1], r[2], r[3]});
		}

		if (node.contains("scale"))
		{
			std::array<float32_t, 3> s = node["scale"];
			nod.m_trs_transform.scale({s[0], s[1], s[2]});

			if (s[0] != s[1] || s[0] != s[2] || s[1] != s[2])
				ror::log_critical("There is non-uniform scale in node {}, make sure the vertex shader normalisation code has no-uniform scale support", nod_data.name().c_str());
		}

		if (node.contains("path"))
		{
			nod_data.m_model_path = node["path"];
			nod_data.m_model      = model_index;
			model_index++;
		}

		if (node.contains("program"))
			nod_data.m_program_id = node["program"];

		if (node.contains("animation"))
			nod_data.m_animation = node["animation"];

		if (node.contains("bvh"))
			nod_data.m_has_bvh = node["bvh"];

		if (node.contains("light"))
			nod_data.m_light_id = node["light"];

		if (node.contains("camera"))
			nod_data.m_light_id = node["camera"];

		if (node.contains("particle_emitter"))
			nod_data.m_particle_id = node["particle_emitter"];

		this->m_nodes.emplace_back(std::move(nod));
		this->m_nodes_data.emplace_back(std::move(nod_data));
	}

	// Since all nodes are loaded lets set the parent ids
	for (size_t i = 0; i < this->m_nodes_data.size(); ++i)
	{
		auto &node_data = this->m_nodes_data[i];
		for (auto &child : node_data.children())
		{
			auto &node    = this->m_nodes[child];
			node.m_parent = static_cast<int32_t>(i);
		}
	}
}

void Scene::read_cameras()
{
	if (this->m_json_file.contains("cameras"))
	{
		// Read all the cameras
		auto cameras = this->m_json_file["cameras"];
		for (auto &camera : cameras)
		{
			OrbitCamera cam{};
			auto        cam_type = camera["type"];

			if (cam_type == "perspective")
			{
				cam.type(CameraType::perspective);
				if (camera.contains("perspective"))
				{
					auto cam_args = camera["perspective"];

					for (auto &[key, value] : cam_args.items())
					{
						if (key == "aspectRatio")
						{
							cam.ratio(value);
						}
						if (key == "yfov")
						{
							cam.fov(value);
						}
						if (key == "znear")
						{
							cam.near(value);
						}
						if (key == "zfar")
						{
							cam.far(value);
						}
					}
				}
			}
			else
			{
				cam.type(CameraType::orthographic);
				if (camera.contains("orthographic"))
				{
					auto cam_args = camera["orthographic"];

					for (auto &[key, value] : cam_args.items())
					{
						if (key == "xmag")
						{
							cam.x_mag(value);
						}
						if (key == "ymag")
						{
							cam.y_mag(value);
						}
						if (key == "znear")
						{
							cam.near(value);
						}
						if (key == "zfar")
						{
							cam.far(value);
						}
					}
				}
			}

			this->m_cameras.emplace_back(std::move(cam));
		}
	}
	else
	{
		log_info("No cameras founds in the scene, will assign default one");
		OrbitCamera cam{};
		this->m_cameras.emplace_back(std::move(cam));
	}
}

void Scene::read_lights()
{
	if (this->m_json_file.contains("lights"))
	{
		// Read all the lights
		auto lights = this->m_json_file["lights"];
		for (auto &light : lights)
		{
			Light lit;
			auto  type = light["type"];

			if (type == "directional")
				lit.m_type = Light::LightType::directional;
			else if (type == "point")
				lit.m_type = Light::LightType::point;
			else if (type == "spot")
				lit.m_type = Light::LightType::spot;
			else if (type == "area")
				lit.m_type = Light::LightType::area;

			if (light.contains("color"))
			{
				std::array<float32_t, 3> c = light["color"];

				lit.m_color.x = c[0];
				lit.m_color.y = c[1];
				lit.m_color.z = c[2];
			}

			if (light.contains("position"))
			{
				std::array<float32_t, 3> p = light["position"];

				lit.m_position.x = p[0];
				lit.m_position.y = p[1];
				lit.m_position.z = p[2];
			}

			if (light.contains("direction"))
			{
				std::array<float32_t, 3> d = light["direction"];

				lit.m_direction.x = d[0];
				lit.m_direction.y = d[1];
				lit.m_direction.z = d[2];
			}

			if (light.contains("intensity"))
				lit.m_intensity = light["intensity"];

			if (light.contains("innerAngle"))
				lit.m_inner_angle = light["innerAngle"];

			if (light.contains("outerAngle"))
				lit.m_outer_angle = light["outerAngle"];

			if (light.contains("range"))
				lit.m_range = light["range"];

			lit.fill_shader_buffer();
			this->m_lights.emplace_back(std::move(lit));
		}
	}
}

void Scene::read_programs()
{
	// assert(this->m_shaders.size() == 0 && "Pre-initialized shaders are not supported, for support requires updating vid, fid and cid below");
	assert(this->m_nodes_data.size() <= std::numeric_limits<int32_t>::max() && "Too many nodes in the scene graph, it doesn't fit in int32_t");

	// Temporary mapping of programs to their nodes
	std::unordered_map<int32_t, uint32_t> program_to_node{};

	uint32_t node_index{0};
	for (auto &node_data : this->m_nodes_data)
	{
		if (node_data.m_program_id != -1)
			program_to_node[node_data.m_program_id] = node_index;

		node_index++;
	}

	// Temporary list of shaders needed when we load programs
	std::vector<std::string> shader_file_names{};

	if (this->m_json_file.contains("shaders"))
	{
		// Read all the shaders
		auto shaders = this->m_json_file["shaders"];
		shader_file_names.reserve(shaders.size());
		for (auto &shader : shaders)
			// this->m_shaders.emplace_back(rhi::load_shader(shader));        // No need the move which prevents copy elision
			shader_file_names.emplace_back(shader);
	}

	if (this->m_json_file.contains("programs"))
	{
		auto    programs = this->m_json_file["programs"];
		int32_t program_id{0};
		for (auto &program : programs)
		{
			if (program.contains("vertex"))
			{
				assert(program.contains("fragment") && "Program must contain both vertex and fragment ids");

				auto vid = program["vertex"];
				auto fid = program["fragment"];

				auto node_id  = program_to_node[program_id];
				auto model_id = static_cast_safe<size_t>(this->m_nodes_data[node_id].m_model);

				this->create_global_program(shader_file_names[vid].c_str(), shader_file_names[fid].c_str(), node_id, model_id);
			}
			else if (program.contains("compute"))
			{
				assert(0 && "Implement me");
				assert((!program.contains("vertex") || !program.contains("fragment")) && "Program can't have compute and vertex/fragment ids");

				int32_t cid = program["compute"];

				GlobalProgram gb;
				gb.program = rhi::Program(cid);

				for (uint32_t i = 0; i < renderpasstype_max(); ++i)
					this->m_global_programs[to_renderpasstype(i)].emplace_back(gb);
			}
			else
			{
				assert(0 && "Implement this type of program support");
			}
		}
	}
}

void Scene::init_global_programs()
{
	// Prim the global programs
	for (uint32_t i = 0; i < renderpasstype_max(); ++i)
		this->m_global_programs[to_renderpasstype(i)] = {};
}

void Scene::read_probes()
{
	if (this->m_json_file.contains("environment_probes"))
	{
		auto probes = this->m_json_file["environment_probes"];
		for (auto &probe : probes)
		{
			EnvironmentProbe prob;
			ror::Transformf  xform;
			if (probe.contains("translation"))
			{
				std::array<float32_t, 3> t = probe["translation"];
				xform.translation({t[0], t[1], t[2]});
			}
			if (probe.contains("rotation"))
			{
				std::array<float32_t, 4> r = probe["rotation"];
				xform.rotation({r[0], r[1], r[2], r[3]});
			}
			if (probe.contains("scale"))
			{
				std::array<float32_t, 3> s = probe["scale"];
				xform.scale({s[0], s[1], s[2]});
			}

			prob.transform(xform);

			if (probe.contains("path"))
				prob.path(probe["path"]);

			this->m_probes.emplace_back(std::move(prob));
		}
	}
}

void Scene::load_specific()
{
	this->read_nodes();
	this->read_cameras();
	this->read_lights();
	// this->read_programs();        // I can only do this after the all the models are loaded and glslang is initialized
	this->init_global_programs();
	this->read_probes();
}

void Scene::unload()
{}

void Scene::init_upload_debug_geometry(const rhi::Device &a_device, const ror::Renderer &a_renderer)
{
	// Different tests of dynamic_mesh used for cube, quad and fullscreen quad
	auto &setting = ror::settings();

	// Different tests of dynamic_mesh used for cube, quad and fullscreen quad
	if (setting.m_generate_cube_mesh)
	{
		ror::DynamicMesh cube_mesh{};

		cube_mesh.init_upload(a_device, a_renderer, rhi::BlendMode::blend, rhi::PrimitiveTopology::triangles);
		cube_mesh.upload_data(reinterpret_cast<const uint8_t *>(&cube_vertex_buffer_interleaved), 9 * 36 * sizeof(float), 36,
		                      reinterpret_cast<const uint8_t *>(cube_index_buffer_uint16), 36 * sizeof(uint16_t), 36);

		this->m_dynamic_meshes.emplace_back(std::move(cube_mesh));
	}

	if (setting.m_generate_cube_map)
	{
		ror::DynamicMesh cube_map_mesh{};

		rhi::VertexDescriptor vertex_descriptor = create_p_float3_descriptor();

		cube_map_mesh.init(a_device, rhi::PrimitiveTopology::triangles);
		cube_map_mesh.setup_vertex_descriptor(&vertex_descriptor);        // Moves vertex_descriptor can't use it afterwards
		cube_map_mesh.load_texture(a_device);                             // What if I want to just set the texture, to something I want to display on it
		auto &envs = a_renderer.environments();
		auto &env  = envs[0];
		if (/* DISABLES CODE */ (1))
		{
			auto irradiance_image   = &a_renderer.images()[env.irradiance()];        // Only testing, if the LUT texture was displayed on the quad, how would it look like
			auto irradiance_sampler = &a_renderer.samplers()[env.irradiance_sampler()];

			cube_map_mesh.set_texture(const_cast<rhi::TextureImage *>(irradiance_image),
			                          const_cast<rhi::TextureSampler *>(irradiance_sampler));        // NOTE: const_cast only allowed in test code, this is just a test code, there is no reason to make a_renderer non-const for this to work.
		}
		else
		{
			auto radiance_image   = &a_renderer.images()[env.radiance()];        // Only testing, if the LUT texture was displayed on the quad, how would it look like
			auto radiance_sampler = &a_renderer.samplers()[env.radiance_sampler()];

			cube_map_mesh.set_texture(const_cast<rhi::TextureImage *>(radiance_image),
			                          const_cast<rhi::TextureSampler *>(radiance_sampler));        // NOTE: const_cast only allowed in test code, this is just a test code, there is no reason to make a_renderer non-const for this to work.
		}
		rhi::descriptor_update_type a_buffers_images;

		cube_map_mesh.setup_shaders(a_renderer, rhi::BlendMode::blend, "equirectangular_cubemap.glsl.vert", "equirectangular_cubemap.glsl.frag");
		cube_map_mesh.setup_descriptors(a_renderer, a_buffers_images, false);        // TODO: Fix me. this will break but I don't have the shaders above to see what images/buffers are used
		cube_map_mesh.topology(rhi::PrimitiveTopology::triangles);
		cube_map_mesh.upload_data(reinterpret_cast<const uint8_t *>(&cube_vertex_buffer_position), 3 * 36 * sizeof(float), 36,
		                          reinterpret_cast<const uint8_t *>(cube_index_buffer_uint16), 36 * sizeof(uint16_t), 36);

		this->m_dynamic_meshes.emplace_back(std::move(cube_map_mesh));
	}

	if (setting.m_generate_canonical_cube_map)
	{
		ror::DynamicMesh canonical_cube{};

		rhi::VertexDescriptor vertex_descriptor = create_p_float3_t_float2_descriptor();

		canonical_cube.init(a_device, rhi::PrimitiveTopology::triangles);
		canonical_cube.setup_vertex_descriptor(&vertex_descriptor);        // Moves vertex_descriptor can't use it afterwards
		canonical_cube.load_texture(a_device);                             // What if I want to just set the texture, to something I want to display on it
		auto &canonical_cube_image = a_renderer.images()[a_renderer.canonical_cube()];

		rhi::descriptor_update_type buffers_images;

		const rhi::TextureImage      *image            = &canonical_cube.texture_image();
		const rhi::TextureSampler    *sampler          = &canonical_cube.texture_sampler();
		const rhi::descriptor_variant per_view_uniform = a_renderer.shader_buffer("per_view_uniform");
		const rhi::descriptor_variant cube_map         = std::make_pair(image, sampler);

		buffers_images[0].emplace_back(std::make_pair(cube_map, 0u));
		buffers_images[0].emplace_back(std::make_pair(per_view_uniform, 20u));
		// These shaders only have
		// layout(std140, set = 0, binding = 20) uniform per_view_uniform
		// layout(set = 0, binding = 0) uniform highp samplerCube cube_map;

		canonical_cube.set_texture(const_cast<rhi::TextureImage *>(&canonical_cube_image));        // NOTE: const_cast only allowed in test code, this is just a test code, there is no reason to make a_renderer non-const for this to work.
		canonical_cube.setup_shaders(a_renderer, rhi::BlendMode::blend, "canonical_cubemap.glsl.vert", "canonical_cubemap.glsl.frag");
		canonical_cube.setup_descriptors(a_renderer, buffers_images, false);
		canonical_cube.topology(rhi::PrimitiveTopology::triangles);
		canonical_cube.upload_data(reinterpret_cast<const uint8_t *>(setting.m_invert_canonical_cube_map ? &cube_vertex_position_uv_interleaved_inverted : &cube_vertex_position_uv_interleaved), 5 * 36 * sizeof(float), 36,
		                           reinterpret_cast<const uint8_t *>(cube_index_buffer_uint16), 36 * sizeof(uint16_t), 36);

		this->m_dynamic_meshes.emplace_back(std::move(canonical_cube));
	}

	if (setting.m_generate_quad_mesh)
	{
		ror::DynamicMesh quad_mesh{};

		rhi::VertexDescriptor vertex_descriptor = create_p_float3_t_float2_descriptor();

		auto image_lut = &a_renderer.images()[2];

		quad_mesh.init(a_device, rhi::PrimitiveTopology::triangles);
		quad_mesh.setup_vertex_descriptor(&vertex_descriptor);        // Moves vertex_descriptor can't use it afterwards
		quad_mesh.load_texture(a_device);                             // What if I want to just set the texture, to something I want to display on it
		quad_mesh.set_texture(const_cast<rhi::TextureImage *>(image_lut));

		rhi::descriptor_update_type buffers_images;

		const rhi::TextureImage      *image              = image_lut;
		const rhi::TextureSampler    *sampler            = &quad_mesh.texture_sampler();
		const rhi::descriptor_variant per_view_uniform   = a_renderer.shader_buffer("per_view_uniform");
		const rhi::descriptor_variant base_color_sampler = std::make_pair(image, sampler);

		buffers_images[0].emplace_back(std::make_pair(base_color_sampler, 0u));
		buffers_images[0].emplace_back(std::make_pair(per_view_uniform, 20u));
		// These shaders only have
		// layout(std140, set = 0, binding = 20) uniform per_view_uniform
		// layout(set = 0, binding = 0) uniform highp sampler2D base_color_sampler;

		quad_mesh.setup_shaders(a_renderer, rhi::BlendMode::blend, "textured_quad.glsl.vert", "textured_quad.glsl.frag");
		quad_mesh.setup_descriptors(a_renderer, buffers_images, false);
		quad_mesh.topology(rhi::PrimitiveTopology::triangles);
		quad_mesh.upload_data(reinterpret_cast<const uint8_t *>(&quad_vertex_buffer_interleaved), 5 * 6 * sizeof(float), 6);

		this->m_dynamic_meshes.emplace_back(std::move(quad_mesh));
	}

	// NOTE: This fullscreen quad doesn't use any vertex attributes, so this is a unique case of rendering a quad without vertices
	if (setting.m_generate_fullscreen_quad_mesh)
	{
		ror::DynamicMesh quad_mesh{};

		rhi::VertexDescriptor vertex_descriptor = create_default_descriptor();

		quad_mesh.init(a_device, rhi::PrimitiveTopology::triangles);
		quad_mesh.setup_vertex_descriptor(&vertex_descriptor);        // Moves vertex_descriptor can't use it afterwards
		quad_mesh.load_texture(a_device);                             // What if I want to just set the texture, to something I want to display on it
		if (a_renderer.images().size() > 12)
		{
			auto image_lut = a_renderer.m_skybox_hdr_patch_ti;
			// auto image_lut = &a_renderer.images()[2];                                // Only testing, if the LUT texture was displayed on the quad, how would it look like
			quad_mesh.set_texture(const_cast<rhi::TextureImage *>(image_lut));        // NOTE: const_cast only allowed in test code, this is just a test code, there is no reason to make a_renderer non-const for this to work.
		}

		rhi::descriptor_update_type buffers_images;

		const rhi::TextureImage      *image              = &quad_mesh.texture_image();
		const rhi::TextureSampler    *sampler            = &quad_mesh.texture_sampler();
		const rhi::descriptor_variant base_color_sampler = std::make_pair(image, sampler);

		buffers_images[0].emplace_back(std::make_pair(base_color_sampler, 0u));
		// These shaders only have
		// layout(set = 0, binding = 0) uniform highp sampler2D base_color_sampler;

		quad_mesh.setup_shaders(a_renderer, rhi::BlendMode::blend, "quad_no_attributes.glsl.vert", "quad_no_attributes.glsl.frag");
		quad_mesh.setup_descriptors(a_renderer, buffers_images, false);
		quad_mesh.topology(rhi::PrimitiveTopology::triangles);
		quad_mesh.upload_data(nullptr, 0, 6);        // This 6 here means I want to draw 6 vertices without any attributes as given by VertexDescriptor

		this->m_dynamic_meshes.emplace_back(std::move(quad_mesh));
	}
}

/*
// Code for testing CPU walk, only works for one model at a time
auto node_has_animation(ror::Model &a_model, uint32_t a_node_index, uint32_t a_animation_index)
{
    auto &model = a_model;
    auto  current_anim{0u};
    for (auto &anim : model.animations())
    {
        if (current_anim == a_animation_index)
            for (auto &chanl : anim.m_channels)
                if (chanl.m_target_node_index == a_node_index)
                    return true;
        current_anim++;
    }

    return false;
}

auto get_animated_trs(ror::Model &a_model, ror::Node &a_node, float32_t a_seconds, uint32_t a_node_index, uint32_t a_animation_index, size_t a_frame)
{
    ror::Transformf trs;
    (void) a_seconds;

    auto &model = a_model;
    auto  current_anim{0u};
    for (auto &anim : model.animations())
    {
        if (current_anim == a_animation_index)
        {
            const Animation::AnimationChannel *tc{nullptr};
            const Animation::AnimationChannel *rc{nullptr};
            const Animation::AnimationChannel *sc{nullptr};

            for (auto &chanl : anim.m_channels)
            {
                if (chanl.m_target_node_index == a_node_index)
                {
                    if (chanl.m_target_node_path == AnimationTarget::translation)
                    {
                        assert(tc == nullptr);
                        tc = &chanl;
                    }
                    else if (chanl.m_target_node_path == AnimationTarget::rotation)
                    {
                        assert(rc == nullptr);
                        rc = &chanl;
                    }
                    else if (chanl.m_target_node_path == AnimationTarget::scale)
                    {
                        assert(sc == nullptr);
                        sc = &chanl;
                    }
                }
            }

            if (tc != nullptr)
            {
                auto &anim_sampler = anim.m_samplers[tc->m_sampler_index];

                size_t input_time_start = 0;
                // size_t input_time_end   = anim_sampler.m_input.size();

                // float time = std::fmodf(a_seconds, (anim_sampler.m_maximum.m_value - anim_sampler.m_minimum.m_value));

                // while (input_time_start < input_time_end && anim_sampler.m_input[input_time_start].m_value < time)
                // 	input_time_start++;

                input_time_start++;

                input_time_start = a_frame;
                input_time_start = input_time_start % anim_sampler.m_input.size();

                // input_time_start = 0;
                size_t    cin = input_time_start * 3;
                float32_t sx  = reinterpret_cast<const float32_t *>(anim_sampler.m_output.data())[cin + 0];
                float32_t sy  = reinterpret_cast<const float32_t *>(anim_sampler.m_output.data())[cin + 1];
                float32_t sz  = reinterpret_cast<const float32_t *>(anim_sampler.m_output.data())[cin + 2];

                trs.translation({sx, sy, sz});
            }
            if (rc != nullptr)
            {
                auto &anim_sampler = anim.m_samplers[rc->m_sampler_index];

                size_t input_time_start = 0;
                input_time_start = a_frame;
                input_time_start = input_time_start % anim_sampler.m_input.size();

                // input_time_start = 0;
                size_t    cin = input_time_start * 4;
                float32_t rx  = reinterpret_cast<const float32_t *>(anim_sampler.m_output.data())[cin + 0];
                float32_t ry  = reinterpret_cast<const float32_t *>(anim_sampler.m_output.data())[cin + 1];
                float32_t rz  = reinterpret_cast<const float32_t *>(anim_sampler.m_output.data())[cin + 2];
                float32_t rw  = reinterpret_cast<const float32_t *>(anim_sampler.m_output.data())[cin + 3];

                trs.rotation({rx, ry, rz, rw});
            }
            if (sc != nullptr)
            {
                auto &anim_sampler = anim.m_samplers[sc->m_sampler_index];

                size_t input_time_start = 0;
                input_time_start++;

                input_time_start = a_frame;
                input_time_start = input_time_start % anim_sampler.m_input.size();
                // input_time_start = 0;
                size_t    cin = input_time_start * 3;
                float32_t sx  = reinterpret_cast<const float32_t *>(anim_sampler.m_output.data())[cin + 0];
                float32_t sy  = reinterpret_cast<const float32_t *>(anim_sampler.m_output.data())[cin + 1];
                float32_t sz  = reinterpret_cast<const float32_t *>(anim_sampler.m_output.data())[cin + 2];

                trs.scale({sx, sy, sz});
            }
            break;
        }
        current_anim++;
    }

    (void) a_node;

    return trs;
}

auto trs_to_matrix4(ror::Transformf& a_trs)
{
    ror::Matrix4f translation = ror::matrix4_translation(a_trs.translation());
    ror::Matrix4f rotation    = ror::matrix4_rotation(a_trs.rotation());
    ror::Matrix4f scale       = ror::matrix4_scaling(a_trs.scale());

    return translation * rotation * scale;
}

auto get_node_global_transform(ror::Model &a_model, ror::Node &a_node, const ror::SceneNodeData &a_scene_node_data, float32_t a_seconds, uint32_t a_node_index, size_t a_frame)
{
    (void) a_scene_node_data;
    (void) a_seconds;
    (void) a_node_index;
    (void) a_frame;

    ror::Matrix4f node_matrix{};
    auto         &model_nodes = a_model.nodes();

    // uint32_t        animation_index = a_scene_node_data.m_animation;
    ror::Transformf trs{};
    // if (node_has_animation(a_model, a_node_index, animation_index))
    // 	trs = get_animated_trs(a_model, a_node, a_seconds, a_node_index, animation_index, a_frame);
    // else
    trs = a_node.m_trs_transform;

    node_matrix = trs_to_matrix4(trs);

    ror::Node *node = &a_node;

    while (node->m_parent != -1)
    {
        auto           &p = model_nodes[static_cast<size_t>(node->m_parent)];
        ror::Transformf ptrs{};
        // if (node_has_animation(a_model, static_cast<uint32_t>(node->m_parent), animation_index))
        // 	ptrs = get_animated_trs(a_model, a_node, a_seconds, static_cast<uint32_t>(node->m_parent), animation_index, a_frame);
        // else
        ptrs = p.m_trs_transform;

        node_matrix = trs_to_matrix4(ptrs) * node_matrix;

        node = &p;
    }

    return node_matrix;
}

auto get_node_transforms(ror::Scene &a_scene, float32_t a_seconds)
{
    auto &nodes_data = a_scene.nodes_side_data();

    std::vector<ror::Matrix4f> node_matrices{};

    for (auto &node : a_scene.nodes())
        node_matrices.emplace_back(trs_to_matrix4(node.m_trs_transform));

    static size_t    frame          = 0;
    static float32_t frame_fraction = 0.0f;

    bool found_one = false;
    for (auto &node : nodes_data)
    {
        if (node.m_model != -1)
        {
            assert(found_one == false);
            found_one = true;

            auto &model       = a_scene.models()[ror::static_cast_safe<size_t>(node.m_model)];
            auto &model_nodes = model.nodes();

            node_matrices.reserve(node_matrices.size() + model_nodes.size());

            for (uint32_t model_node_index = 0; model_node_index < model_nodes.size(); ++model_node_index)
            {
                auto &n = model_nodes[model_node_index];
                node_matrices.emplace_back(get_node_global_transform(model, n, node, a_seconds, model_node_index, frame));
            }
        }
    }

    frame_fraction += 0.5f;

    frame = static_cast_safe<size_t>(frame_fraction);

    return node_matrices;
}

void Scene::cpu_walk_scene(rhi::ComputeCommandEncoder &a_command_encoder,
                           rhi::Device                &a_device,
                           rhi::BuffersPack           &a_buffers_pack,
                           ror::Renderer              &a_renderer,
                           const rhi::Rendersubpass   &a_subpass,
                           Timer                      &a_timer,
                           ror::EventSystem           &a_event_system)
{
    (void) a_device;
    (void) a_buffers_pack;
    (void) a_command_encoder;
    (void) a_subpass;
    (void) a_timer;
    (void) a_event_system;
    (void) a_renderer;

    static float32_t seconds{0.0f};
    seconds += static_cast<float32_t>(a_timer.tick_seconds());

    // TODO: After some time reset the seconds
    if (seconds > 1000.0f)        // TODO: Move the 1000 to settings
        seconds = seconds - static_cast<float32_t>(static_cast<int32_t>(seconds));

    auto matrices  = get_node_transforms(*this, seconds);
    auto model_ubo = a_renderer.shader_buffer("nodes_model");

    model_ubo->buffer_map();
    model_ubo->update("node_model_mat4", &matrices[0].m_values, static_cast_safe<uint32_t>(matrices.size() * sizeof(ror::Matrix4f)));
    model_ubo->buffer_unmap();
}
*/

}        // namespace ror
