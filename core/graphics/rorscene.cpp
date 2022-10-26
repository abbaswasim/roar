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

#include "camera/rorcamera.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/rormesh.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rornode.hpp"
#include "graphics/rorscene.hpp"
#include "math/rorquaternion.hpp"
#include "math/rortransform.hpp"
#include "math/rorvector3.hpp"
#include "profiling/rorlog.hpp"
#include "profiling/rortimer.hpp"
#include "renderer/rorrenderer.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rorhandles.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rortypes.hpp"
#include "shader_system/rorshader_system.hpp"
#include <array>
#include <cassert>
#include <cstddef>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ror
{

define_translation_unit_vtable(Scene)
{}

void Light::fill_shader_buffer()
{
	const uint32_t fixed_light_count = 2;        // TODO: needs to be moved out of here, at least 2 so we make an array
	assert(this->m_type != ror::Light::LightType::area && "Area lights not supported yet");

	rhi::ShaderBufferTemplate::Struct light_type("lights", static_cast_safe<uint32_t>(fixed_light_count));

	light_type.add_entry("mvp", rhi::Format::float32_4x4, rhi::Layout::std140, 1);
	light_type.add_entry("color", rhi::Format::float32_3, rhi::Layout::std140, 1);

	if (this->m_type != ror::Light::LightType::directional)
		light_type.add_entry("position", rhi::Format::float32_3, rhi::Layout::std140, 1);

	if (this->m_type != ror::Light::LightType::point)
		light_type.add_entry("direction", rhi::Format::float32_3, rhi::Layout::std140, 1);

	light_type.add_entry("intensity", rhi::Format::float32_1, rhi::Layout::std140, 1);
	light_type.add_entry("range", rhi::Format::float32_1, rhi::Layout::std140, 1);

	if (this->m_type == ror::Light::LightType::spot)
	{
		light_type.add_entry("inner_angle", rhi::Format::float32_1, rhi::Layout::std140, 1);
		light_type.add_entry("outer_angle", rhi::Format::float32_1, rhi::Layout::std140, 1);
	}

	auto &shader_buffer = this->m_shader_buffer.shader_buffer();
	shader_buffer.add_struct(light_type);
}

void Light::update()
{
	auto mapping = this->m_shader_buffer.map();

	std::memcpy(mapping + this->m_mvp_offset, &this->m_mvp.m_values, sizeof(decltype(this->m_mvp)));
	std::memcpy(mapping + this->m_color_offset, &this->m_color, sizeof(decltype(this->m_color)));

	if (this->m_type != ror::Light::LightType::directional)
		std::memcpy(mapping + this->m_position_offset, &this->m_position, sizeof(decltype(this->m_position)));

	if (this->m_type != ror::Light::LightType::point)
		std::memcpy(mapping + this->m_direction_offset, &this->m_direction, sizeof(decltype(this->m_direction)));

	std::memcpy(mapping + this->m_intensity_offset, &this->m_intensity, sizeof(decltype(this->m_intensity)));
	std::memcpy(mapping + this->m_range_offset, &this->m_range, sizeof(decltype(this->m_range)));

	if (this->m_type == ror::Light::LightType::spot)
	{
		std::memcpy(mapping + this->m_inner_angle_offset, &this->m_inner_angle, sizeof(decltype(this->m_inner_angle)));
		std::memcpy(mapping + this->m_outer_angle_offset, &this->m_outer_angle, sizeof(decltype(this->m_outer_angle)));
	}

	this->m_shader_buffer.unmap();
}

void Light::upload(rhi::Device &a_device)
{
	// Looking to create a UBO for directional light like below
	/*
	  const uint directional_lights_count = @;
	  struct light_type
	  {
	      vec3  color;
	      vec3  direction;
	      float intensity;
	      mat4  mvp;
	  };

	  layout(std140, set = @, binding = @) uniform directional_light_uniform
	  {
	      light_type lights[directional_lights_count];
	  } in_directional_light_uniforms;
	*/
	this->fill_shader_buffer();
	auto &shader_buffer = this->m_shader_buffer.shader_buffer();
	auto  entries       = shader_buffer.entries_structs();

	auto size = 0u;
	for (auto entry : entries)
	{
		if (entry->m_name == "lights")
			size = entry->m_stride;
		else if (entry->m_name == "mvp")
			this->m_mvp_offset = entry->m_offset;
		else if (entry->m_name == "color")
			this->m_color_offset = entry->m_offset;
		else if (entry->m_name == "position")
			this->m_position_offset = entry->m_offset;
		else if (entry->m_name == "direction")
			this->m_direction_offset = entry->m_offset;
		else if (entry->m_name == "intensity")
			this->m_intensity_offset = entry->m_offset;
		else if (entry->m_name == "range")
			this->m_range_offset = entry->m_offset;
		else if (entry->m_name == "inner_angle")
			this->m_inner_angle_offset = entry->m_offset;
		else if (entry->m_name == "outer_angle")
			this->m_outer_angle_offset = entry->m_offset;
	}

	assert(size != 0 && "Couldn't determine lights struct size in the UBO");
	this->m_shader_buffer.init(a_device, size);

	this->update();
}

Scene::Scene(std::filesystem::path a_level)
{
	this->load(a_level, ResourceSemantic::scenes);
}

void Scene::render(const RenderDevice *a_rendering_device)
{
	(void) a_rendering_device;
}

void Scene::update(double64_t a_milli_seconds)
{
	(void) a_milli_seconds;
}

void Scene::load_models(ror::JobSystem &a_job_system, rhi::Device &a_device, const ror::Renderer &a_renderer)
{
	auto model_nodes{0u};
	for (auto &node : this->m_nodes_data)
		if (node.m_model_path != "")
			model_nodes++;

	if (model_nodes > 0)
	{
		this->m_models.resize(model_nodes);        // NOTE: I am resizing the models vector because I don't want many threads to emplace to it at the same time
		std::vector<ror::JobHandle<bool>> job_handles;
		job_handles.reserve(model_nodes * 2);        // Multiplied by 2 because I am creating two jobs, load and upload per model

		auto model_load_job = [this](SceneNodeData & node, size_t a_index) -> auto
		{
			log_info("Loading model {}", node.m_model_path.c_str());
			Model &model = this->m_models[a_index];
			model.load_from_gltf_file(node.m_model_path, this->m_cameras);
			return true;
		};

		auto model_upload_job = [this, &a_device](size_t a_index) -> auto
		{
			Model &model = this->m_models[a_index];
			model.upload(a_device);
			return true;
		};

		auto model_index{0u};
		for (auto &node : this->m_nodes_data)
		{
			if (node.m_model_path != "")
			{
				auto load_job_handle   = a_job_system.push_job(model_load_job, node, model_index);
				auto upload_job_handle = a_job_system.push_job(model_upload_job, load_job_handle.job(), model_index);
				job_handles.emplace_back(std::move(load_job_handle));
				job_handles.emplace_back(std::move(upload_job_handle));
				model_index++;
			}
		}

		assert(model_index == model_nodes && "Models count vs how many are loaded doesn't match");
		(void) model_index;

		// Wait for all jobs to finish
		for (auto &jh : job_handles)
			if (!jh.data())
				ror::log_critical("Can't load models specified in the scene.");
	}

	// Lets kick off shader generation while we upload the buffers
	auto shader_gen_job_handle = a_job_system.push_job([this, &a_renderer, &a_job_system]() -> auto{ this->generate_shaders(a_renderer, a_job_system); return true; });

	// By this time the buffer pack should be primed and filled with all kinds of geometry and animatiom data, lets upload it, all in one go
	// TODO: find out this might need to be done differently for Vulkan, also should be moved to upload()
	auto &bpack = rhi::get_buffers_pack();
	bpack.upload(a_device);

	if (!shader_gen_job_handle.data())
		ror::log_critical("Can't generate model shaders.");
}

hash_64_t pass_aware_vertex_hash(rhi::RenderpassType a_passtype, const ror::Mesh &a_mesh, size_t a_prim_index, const std::vector<ror::Skin, rhi::BufferAllocator<ror::Skin>> &a_skins)
{
	// NOTE: If every need to change how this hashing works, make sure to change in rormodel.cpp hash gen as well.
	// Ideally it should be abstracted out
	hash_64_t vertex_hash{};

	auto &vertex_attribute_descriptor              = a_mesh.m_attribute_vertex_descriptors[a_prim_index];
	auto &morph_target_vertex_attribute_descriptor = a_mesh.m_morph_targets_vertex_descriptors[a_prim_index];

	// Setup vertex hash
	vertex_hash = vertex_attribute_descriptor.hash_64_pass_aware(a_passtype);

	for (auto &attrib : morph_target_vertex_attribute_descriptor)
		hash_combine_64(vertex_hash, attrib.hash_64_pass_aware(a_passtype));

	// Only check if we have weights
	auto weights_count = a_mesh.m_morph_weights.size();

	if (weights_count > 0)
		hash_combine_64(vertex_hash, hash_64(&weights_count, sizeof(weights_count)));

	auto skin_index = a_mesh.m_skin_index;
	if (skin_index != -1)
	{
		const auto &skin = a_skins[ror::static_cast_safe<size_t>(skin_index)];
		hash_combine_64(vertex_hash, skin.m_joints.size());
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

void Scene::generate_shaders(const ror::Renderer &a_renderer, ror::JobSystem &a_job_system)
{
	const std::vector<rhi::RenderpassType> render_pass_types = a_renderer.render_pass_types();

	size_t shaders_count = 0;

	for (auto &model : this->m_models)
		for (auto &mesh : model.meshes())
			shaders_count += mesh.primitives_count();

	// For each render pass in the framegraph create programs the model meshes can use
	bool has_shadows = false;

	for (auto &passtype : render_pass_types)
		if (passtype == rhi::RenderpassType::shadow)
			has_shadows = true;

	// This should be shaders_count * 2 * render_passes.size();
	// Then fill me up in a loop before entering the next loop with jobs in it
	this->m_shaders.reserve(shaders_count * 2 * render_pass_types.size());

	log_warn("About to create {} shaders ", shaders_count * 2 * render_pass_types.size());

	// Two pass approach, first create all the shaders into m_shaders, then allocate each to a job to fill it in with data
	std::unordered_map<hash_64_t, std::atomic_flag> shader_hash_to_flag{};
	std::unordered_map<hash_64_t, size_t>           shader_hash_to_index{};
	std::unordered_set<hash_64_t>                   unique_shaders{};

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
		auto  pass_string          = std::string("_") + rhi::renderpass_type_to_string(passtype);
		pass_programs.reserve(shaders_count);        // Pre-reserve so no reallocation happen under the hood while jobs are using it, times 2 for both vertex and fragment

		// Create vertex and fragment shaders but don't generate sources yet
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

					create_hash_to_index(vs_hash, rhi::ShaderType::vertex, ".vert");
					create_hash_to_index(fs_hash, rhi::ShaderType::fragment, ".frag");

					pass_programs.emplace_back(shader_hash_to_index[vs_hash], shader_hash_to_index[fs_hash]);

					// This is set once for the first render pass but it must stay the same for all of the rest because every pass must contain the same amount of shaders
					if (mesh.m_program_indices[prim_index] == -1)
						model.update_mesh_program(mesh_index, static_cast_safe<uint32_t>(prim_index), static_cast_safe<int32_t>(pass_programs.size()) - 1);
				}
				++mesh_index;
			}
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
						auto  vs_job_handle   = a_job_system.push_job([mesh_index, prim_index, passtype, &vs_shader, &model]() -> auto{
                            auto vs = ror::generate_primitive_vertex_shader(model, mesh_index, static_cast_safe<uint32_t>(prim_index), passtype);
                            vs_shader.source(vs);
                            return true;
						   });

						job_handles.emplace_back(std::move(vs_job_handle));
					}

					if (!shader_hash_to_flag[fs_hash].test_and_set())        // Not generated yet
					{
						auto &fs_shader_index = shader_hash_to_index[fs_hash];
						auto &fs_shader       = this->m_shaders[fs_shader_index];
						auto  fs_job_handle   = a_job_system.push_job([prim_index, passtype, has_shadows, &fs_shader, &mesh, &model]() -> auto{
                            auto fs = ror::generate_primitive_fragment_shader(mesh, model.materials(), static_cast_safe<uint32_t>(prim_index), passtype, has_shadows);
                            fs_shader.source(fs);
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
			assert(s.second.test() == true && "Not all unique shaders are generated");
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
}

void Scene::upload(const ror::Renderer &a_renderer, rhi::Device &a_device, ror::EventSystem &a_event_system)
{
	auto render_passes = a_renderer.current_frame_graph();

	// Now lets upload them
	for (auto &shader : this->m_shaders)
	{
		shader.upload(a_device);
	}

	for (auto &shader : this->m_global_shaders)
	{
		shader.compile();
		shader.upload(a_device);
	}

	for (auto &program : this->m_global_programs)
	{
		program.upload(a_device, this->m_shaders);
	}

	// Upload all the shader programs creates pipelines in metal and vulkan cases
	for (auto &pass : render_passes)
	{
		for (auto &subpass : pass.subpasses())
		{
			auto    &pass_programs = this->m_programs[subpass.type()];
			uint32_t program       = 0;
			for (auto &model : this->m_models)
			{
				uint32_t mesh_index = 0;
				for (auto &mesh : model.meshes())
				{
					for (size_t prim_index = 0; prim_index < mesh.primitives_count(); ++prim_index)
					{
						pass_programs[program].upload(a_device, this->m_shaders, model, mesh_index, static_cast_safe<uint32_t>(prim_index), subpass);
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
	{
		camera.init(a_event_system);
		camera.upload(a_device);
	}
}

void Scene::read_nodes()
{
	assert(this->m_json_file.contains("nodes") && "Provided scene file is not a roar scene.");

	// Read all the nodes
	auto nodes = this->m_json_file["nodes"];
	for (auto &node : nodes)
	{
		SceneNode     nod;
		SceneNodeData nod_data;

		nod_data.m_name = node["name"];

		if (node.contains("children"))
			nod_data.m_children = node["children"].get<std::vector<uint32_t>>();

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
		}

		if (node.contains("path"))
			nod_data.m_model_path = node["path"];

		if (node.contains("shader"))
			nod_data.m_program_id = node["shader"];

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

	// Now lets since all nodes are loaded lets set the parent ids
	for (size_t i = 0; i < this->m_nodes_data.size(); ++i)
	{
		auto &node_data = this->m_nodes_data[i];
		for (auto &child : node_data.m_children)
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
			OrbitCamera cam;
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
							cam.aspect_ratio(value);
						}
						if (key == "yfov")
						{
							cam.y_fov(value);
						}
						if (key == "znear")
						{
							cam.z_near(value);
						}
						if (key == "zfar")
						{
							cam.z_far(value);
						}
					}
				}
				if (camera.contains("orthographic"))
				{
					auto cam_args = camera["orthographic"];

					for (auto &[key, value] : cam_args.items())
					{
						if (key == "xmag")
						{
							cam.width(value);
						}
						if (key == "ymag")
						{
							cam.height(value);
						}
						if (key == "znear")
						{
							cam.z_near(value);
						}
						if (key == "zfar")
						{
							cam.z_far(value);
						}
					}
				}
			}
			else
			{
				cam.type(CameraType::orthographic);
			}

			this->m_cameras.emplace_back(std::move(cam));
		}
	}
	else
	{
		log_info("No cameras founds in the scene, will assign default one");
		OrbitCamera cam;
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

			if (light.contains("intensity"))
				lit.m_intensity = light["intensity"];

			if (light.contains("innerAngle"))
				lit.m_inner_angle = light["innerAngle"];

			if (light.contains("outerAngle"))
				lit.m_outer_angle = light["outerAngle"];

			if (light.contains("range"))
				lit.m_range = light["range"];

			this->m_lights.emplace_back(std::move(lit));
		}
	}
}

void Scene::read_programs()
{
	if (this->m_json_file.contains("shaders"))
	{
		// Read all the shaders
		auto shaders = this->m_json_file["shaders"];
		for (auto &shader : shaders)
		{
			auto s_path = std::filesystem::path(shader);
			auto type   = rhi::string_to_shader_type(s_path.extension());
			auto hash   = hash_64(s_path.c_str(), s_path.string().length());
			this->m_global_shaders.emplace_back(shader, hash, type, ror::ResourceAction::load);
		}
	}

	if (this->m_json_file.contains("programs"))
	{
		auto programs = this->m_json_file["programs"];
		for (auto &program : programs)
		{
			(void) program;

			if (program.contains("vertex"))
			{
				assert(program.contains("fragment") && "Program must contain both vertex and fragment ids");

				int32_t vid = program["vertex"];
				int32_t fid = program["fragment"];

				this->m_global_programs.emplace_back(vid, fid);
			}
			else if (program.contains("compute"))
			{
				assert(!program.contains("fragment") && "Program can't have compute and fragment ids");

				int32_t cid = program["compute"];

				this->m_global_programs.emplace_back(cid);
			}

			// TODO: Need to find a way to upload these programs (Challenge is for which model/mesh/primitive?)
		}
	}
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
	this->read_programs();
	this->read_probes();
}

void Scene::unload()
{}

}        // namespace ror
