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

#include "configuration/rorconfiguration.hpp"
#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rorcompiler_workarounds.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "geometry/rorgeometry_utilities.hpp"
#include "graphics/primitive_geometries.hpp"
#include "graphics/rordynamic_mesh.hpp"
#include "graphics/rorenvironment.hpp"
#include "graphics/rorscene.hpp"
#include "math/rorvector_functions.hpp"
#include "profiling/rorlog.hpp"
#include "profiling/rortimer.hpp"
#include "renderer/rorrenderer.hpp"
#include "resources/rorresource.hpp"
#include "rhi/crtp_interfaces/rorcompute_dispatch.hpp"
#include "rhi/crtp_interfaces/rorrenderpass.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rorcommand_buffer.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rorshader_input.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include "shader_system/rorshader_system.hpp"
#include "watchcat/rorwatchcat.hpp"
#include <cassert>
#include <filesystem>
#include <iterator>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace ror
{

define_translation_unit_vtable(Renderer)
{}

rhi::TextureTarget string_to_texture_target(const std::string &a_target)
{
	if (a_target == "2D")
		return rhi::TextureTarget::texture_2D;
	else if (a_target == "3D")
		return rhi::TextureTarget::texture_3D;
	else if (a_target == "CUBE")
		return rhi::TextureTarget::texture_cube;
	else
		assert(0 && "Implement other texture targets support");

	return rhi::TextureTarget::texture_2D;
}

rhi::TextureFilter string_to_texture_filter(const std::string &a_filter)
{
	if (a_filter == "nearest")
		return rhi::TextureFilter::nearest;
	else if (a_filter == "linear")
		return rhi::TextureFilter::linear;
	else
		assert(0 && "Implement other texture filter support");

	return rhi::TextureFilter::nearest;
}

rhi::TextureMipFilter string_to_texture_mip_filter(const std::string &a_filter)
{
	if (a_filter == "not_mipmapped")
		return rhi::TextureMipFilter::not_mipmapped;
	else if (a_filter == "nearest")
		return rhi::TextureMipFilter::nearest;
	else if (a_filter == "linear")
		return rhi::TextureMipFilter::linear;
	else
		assert(0 && "Implement other texture mip filter support");

	return rhi::TextureMipFilter::not_mipmapped;
}

rhi::TextureAddressMode string_to_texture_address_mode(const std::string &a_filter)
{
	if (a_filter == "clamp_to_edge")
		return rhi::TextureAddressMode::clamp_to_edge;
	else if (a_filter == "mirror_clamp_to_edge")
		return rhi::TextureAddressMode::mirror_clamp_to_edge;
	else if (a_filter == "repeat")
		return rhi::TextureAddressMode::repeat;
	else if (a_filter == "mirror_repeat")
		return rhi::TextureAddressMode::mirror_repeat;
	else if (a_filter == "clamp_to_zero")
		return rhi::TextureAddressMode::clamp_to_zero;
	else if (a_filter == "clamp_to_border_color")
		return rhi::TextureAddressMode::clamp_to_border_color;
	else
		assert(0 && "Implement other texture address modes support");

	return rhi::TextureAddressMode::clamp_to_edge;
}

rhi::TextureBorder string_to_texture_border(const std::string &a_filter)
{
	if (a_filter == "transparent")
		return rhi::TextureBorder::transparent;
	else if (a_filter == "opaque")
		return rhi::TextureBorder::opaque;
	else if (a_filter == "white")
		return rhi::TextureBorder::white;
	else
		assert(0 && "Implement other texture border types support");

	return rhi::TextureBorder::transparent;
}

void Renderer::update_shader(rhi::Device &a_device, std::string &a_shader)        // a_shader is the name/path of the shader in renderer or anywhere else like the scene
{
	auto shader_callbacks = this->m_update_callbacks_mapping.find(a_shader);
	if (shader_callbacks != this->m_update_callbacks_mapping.end())
	{
		ror::log_info("Calling all update shader_callbacks on {}", a_shader.c_str());
		for (auto &shader_callback : shader_callbacks->second)
		{
			shader_callback(a_device, *this);
		}
	}
}

void Renderer::patch_shader(rhi::Shader &a_shader, std::string &a_shader_name)
{
	auto shader_callback = this->m_callbacks_mapping.find(a_shader_name);
	if (shader_callback != this->m_callbacks_mapping.end())
	{
		ror::log_info("Calling patch shader_callback on {}", a_shader_name.c_str());
		auto shader_source = a_shader.source();
		shader_callback->second(shader_source, *this);
		a_shader.source(shader_source);
	}
}

void Renderer::load_programs()
{
	if (this->m_json_file.contains("shaders"))
	{
		// Read all the shaders
		std::vector<std::string> shaders = this->m_json_file["shaders"];
		for (auto &shader : shaders)
		{
			auto      s_path = std::filesystem::path(shader);
			auto      type   = rhi::string_to_shader_type(s_path.extension());
			hash_64_t hash   = hash_64(s_path.c_str(), s_path.string().length());
			this->m_shaders.emplace_back(shader, hash, type, ror::ResourceAction::load);
			this->patch_shader(this->m_shaders.back(), shader);
		}
	}

	if (this->m_json_file.contains("programs"))
	{
		auto programs = this->m_json_file["programs"];
		for (auto &program : programs)
		{
			if (program.contains("vertex"))
			{
				assert(program.contains("fragment") && "Program must contain both vertex and fragment ids");

				int32_t vid = program["vertex"];
				int32_t fid = program["fragment"];

				this->m_programs.emplace_back(vid, fid);
			}
			else if (program.contains("compute"))
			{
				assert(!program.contains("fragment") && !program.contains("vertex") && "Program can't have both compute and vertex,fragment ids");

				int32_t cid = program["compute"];

				this->m_programs.emplace_back(cid);
			}
		}
	}

	// Now lets create shader records and do the mapping
	int32_t shader_id{0};
	for (auto &shader : this->m_shaders)
	{
		ShaderRecord shader_record{};
		shader_record.m_shader = &shader;
		shader_record.m_id     = shader_id++;

		for (auto &program : this->m_programs)
		{
			if (program.vertex_id() == shader_record.m_id ||
			    program.fragment_id() == shader_record.m_id ||
			    program.compute_id() == shader_record.m_id ||
			    program.mesh_id() == shader_record.m_id ||
			    program.tile_id() == shader_record.m_id)
			{
				shader_record.m_programs.emplace_back(&program);
			}
		}

		this->m_shaders_mapping[shader.shader_path().filename()] = std::move(shader_record);
	}
}

// Only works for RGBA (LDR/HDR) Images
void make_texture_pink(rhi::TextureImage &a_texture_image)
{
	assert(a_texture_image.data());

	auto *ptr = a_texture_image.data();

	for (size_t i = 0; i < a_texture_image.size();)
	{
		if (a_texture_image.hdr())
		{
			float32_t *fptr = reinterpret_cast<float32_t *>(ptr);

			fptr[i + 0] = 1.0f;
			fptr[i + 1] = 20.0f / 255.0f;
			fptr[i + 2] = 147.0f / 255.0f;
			fptr[i + 3] = 1.0f;

			i += 16;
		}
		else
		{
			ptr[i + 0] = 255;
			ptr[i + 1] = 20;
			ptr[i + 2] = 147;
			ptr[i + 3] = 255;

			i += 4;
		}
	}
}

void read_into_texture_image(rhi::TextureImage &a_texture_image, nlohmann::json &a_texture, bool a_allocate, rhi::TextureUsage a_usage)
{
	a_texture_image.push_empty_mip();

	assert(a_texture.contains("name") && a_texture.contains("format") && "Texture must specify name and format");
	a_texture_image.format(rhi::string_to_pixel_format(a_texture["format"]));        // This also sets bytes_per_pixel
	a_texture_image.usage(a_usage);                                                  // Is also changed/updated later in reading framegraphs

	if (a_texture.contains("width"))
		a_texture_image.width(a_texture["width"]);

	if (a_texture.contains("height"))
		a_texture_image.height(a_texture["height"]);

	if (a_texture.contains("size"))
	{
		a_texture_image.width(a_texture["size"]);
		a_texture_image.height(a_texture["size"]);
	}

	if (a_texture.contains("depth"))
		a_texture_image.depth(a_texture["depth"]);

	if (a_texture.contains("target"))
		a_texture_image.target(string_to_texture_target(a_texture["target"]));

	if (a_texture.contains("name"))        // Name is also used as path
		a_texture_image.name(a_texture["name"]);

	if (a_texture.contains("writeable"))
		if (a_texture["writeable"] == true)
			a_texture_image.usage(rhi::TextureUsage::shader_write);

	auto size_in_bytes{0ul};
	if (a_texture.contains("mipmapped"))
		a_texture_image.mipmapped(a_texture["mipmapped"]);

	size_in_bytes = a_texture_image.setup();

	if (a_allocate)
	{
		size_t size{rhi::calculate_texture_size(a_texture_image.width(), a_texture_image.height(), a_texture_image.depth(), a_texture_image.format(),
		                                        a_texture_image.mipmapped(), rhi::is_texture_cubemap(a_texture_image.target()), rhi::is_texture_array(a_texture_image.target()))};
		if (a_texture_image.mipmapped())
		{
			assert(size_in_bytes == size && "Mipmapped texture sizes don't match");
		}
		else
		{
			size_t manual_size = a_texture_image.width() * a_texture_image.height() * a_texture_image.depth() * a_texture_image.bytes_per_pixel() * (rhi::is_texture_cubemap(a_texture_image.target()) ? 6 : 1);
			assert(manual_size == size && "Mipmapped texture sizes don't match");
			(void) manual_size;
		}

		a_texture_image.allocate(size);        // This could also be allocate() but I am doing more validation here just in case

		if (settings().m_generate_pink_textures)
			make_texture_pink(a_texture_image);

		// a_texture_image.reset(a_size);
	}
}

void read_into_texture_sampler(rhi::TextureSampler &a_texture_sampler, nlohmann::json &a_sampler)
{
	if (a_sampler.contains("mag_filter"))
		a_texture_sampler.mag_filter(string_to_texture_filter(a_sampler["mag_filter"]));

	if (a_sampler.contains("min_filter"))
		a_texture_sampler.min_filter(string_to_texture_filter(a_sampler["min_filter"]));

	if (a_sampler.contains("mip_filter"))
		a_texture_sampler.mip_mode(string_to_texture_mip_filter(a_sampler["mip_filter"]));

	if (a_sampler.contains("wrap_s"))
		a_texture_sampler.wrap_s(string_to_texture_address_mode(a_sampler["wrap_s"]));

	if (a_sampler.contains("wrap_t"))
		a_texture_sampler.wrap_t(string_to_texture_address_mode(a_sampler["wrap_t"]));

	if (a_sampler.contains("wrap_u"))
		a_texture_sampler.wrap_u(string_to_texture_address_mode(a_sampler["wrap_u"]));

	if (a_sampler.contains("border"))
		a_texture_sampler.border_color(string_to_texture_border(a_sampler["border"]));
}

void Renderer::load_textures()
{
	if (this->m_json_file.contains("textures"))
	{
		auto textures = this->m_json_file["textures"];
		for (auto &texture : textures)
		{
			rhi::TextureImage texture_image;
			texture_image.push_empty_mip();

			// Not allocated render target, will be allocated when needed in render passes and properties set
			read_into_texture_image(texture_image, texture, false, rhi::TextureUsage::render_target);        // render_target use is also changed/updated later in reading framegraphs

			this->m_images.emplace_back(std::move(texture_image));
		}
	}
	else
	{
		ror::log_critical("Renderer config should contain textures description but found nothing");
	}
}

void Renderer::load_samplers()
{
	if (this->m_json_file.contains("samplers"))
	{
		auto samplers = this->m_json_file["samplers"];
		for (auto &sampler : samplers)
		{
			rhi::TextureSampler texture_sampler;

			read_into_texture_sampler(texture_sampler, sampler);

			this->m_samplers.emplace_back(std::move(texture_sampler));
		}
	}
	else
	{
		ror::log_critical("Renderer config should contain sampler description but found nothing");
	}
}

int32_t Renderer::brdf_integration_lut_index(std::string a_name)
{
	auto index{-1};
	for (auto &texture : this->m_images)
	{
		++index;

		if (texture.name() == a_name)
			return index;
	}

	return index;
}

void Renderer::load_environments()
{
	if (this->m_json_file.contains("environments"))
	{
		auto environments = this->m_json_file["environments"];
		for (auto &environment : environments)
		{
			assert(environment.contains("name") &&
			       environment.contains("input") &&
			       environment.contains("skybox_pso") &&
			       environment.contains("radiance_pso") &&
			       environment.contains("irradiance_pso") &&
			       environment.contains("skybox_sampler") &&
			       environment.contains("radiance_sampler") &&
			       environment.contains("irradiance_sampler") &&
			       "Environments must specifiy these parameters");

			ror::IBLEnvironment ibl_environment;
			ibl_environment.name(environment["name"]);
			ibl_environment.skybox_pso(environment["skybox_pso"]);
			ibl_environment.radiance_pso(environment["radiance_pso"]);
			ibl_environment.irradiance_pso(environment["irradiance_pso"]);
			ibl_environment.skybox_sampler(environment["skybox_sampler"]);
			ibl_environment.radiance_sampler(environment["radiance_sampler"]);
			ibl_environment.irradiance_sampler(environment["irradiance_sampler"]);

			// Find brdf_integration_lut in textures
			auto lut_index = this->brdf_integration_lut_index();
			assert(lut_index != -1 && "No brdf_integration_lut index found in the textures");

			if (environment.contains("brdf_integration"))
			{
				auto env_lut_index = environment["brdf_integration"];
				assert(env_lut_index < this->m_images.size() && "Out of bound brdf_intgration_lut index");
				assert(lut_index == env_lut_index && "brdf_intgration_lut index doesn't match one available in textures");

				ibl_environment.brdf_integration(env_lut_index);
			}
			else
				ibl_environment.brdf_integration(static_cast<uint32_t>(lut_index));

			if (environment.contains("brdf_integration_pso"))
				ibl_environment.brdf_integration_pso(environment["brdf_integration_pso"]);

#define read_env_texture(name, s)                                                                              \
	rhi::TextureImage name##_texture_image;                                                                    \
	name##_texture_image.push_empty_mip();                                                                     \
	if (environment.contains(#name))                                                                           \
	{                                                                                                          \
		auto texture_description = environment[#name];                                                         \
		read_into_texture_image(name##_texture_image, texture_description, s, rhi::TextureUsage::shader_read); \
	}                                                                                                          \
	else                                                                                                       \
	{                                                                                                          \
		assert(0 && "Add empty cubemap");                                                                      \
	}                                                                                                          \
	ibl_environment.name(static_cast_safe<uint32_t>(this->m_images.size()));                                   \
	this->m_images.emplace_back(std::move(name##_texture_image));                                              \
	(void) 0

			read_env_texture(input, false);
			read_env_texture(irradiance, true);
			read_env_texture(radiance, true);
			read_env_texture(skybox, true);

			this->m_environments.emplace_back(std::move(ibl_environment));

#undef read_env_texture
		}
	}
	else
	{
		ror::log_info("No environments available in renderer config");
	}

	if (this->m_json_file.contains("environment"))
		this->m_current_environment = this->m_json_file["environment"];

	if (this->m_current_environment == -1 && this->m_environments.size())
	{
		ror::log_info("No environments chosen from the provided environment, choosing the first one now");
		this->m_current_environment = 0;
	}
}

void read_entry(nlohmann::json a_json_entry, rhi::ShaderBufferTemplate::Struct &a_struct, rhi::Layout a_layout)
{
	if (a_json_entry.contains("struct"))
	{
		rhi::ShaderBufferTemplate::Struct shb_struct;
		auto                              json_struct = a_json_entry["struct"];

		assert(json_struct.contains("name") && "Struct must provide a name");
		shb_struct.m_name = json_struct["name"];
		if (json_struct.contains("count"))
			shb_struct.m_count = json_struct["count"];

		auto entries = json_struct["entries"];
		for (auto &entry : entries)
			read_entry(entry, shb_struct, a_layout);

		a_struct.add_struct(std::move(shb_struct));
	}
	else
	{
		std::string name{};
		rhi::Format format{rhi::Format::float32_4};
		uint32_t    count{1};

		assert(a_json_entry.contains("name") && "Entry must provide a name");
		name = a_json_entry["name"];
		if (a_json_entry.contains("format"))
			format = rhi::string_to_vertex_format(a_json_entry["format"]);
		if (a_json_entry.contains("count"))
			count = a_json_entry["count"];

		a_struct.add_entry(name, format, a_layout, count);
	}
}

void Renderer::load_buffers()
{
	if (this->m_json_file.contains("buffers"))
	{
		auto buffers = this->m_json_file["buffers"];
		for (auto &buffer : buffers)
		{
			std::string           name{};
			rhi::Layout           layout{rhi::Layout::std140};
			rhi::ShaderBufferType type{rhi::ShaderBufferType::ubo};
			uint32_t              set{0};
			uint32_t              binding{0};

			assert(buffer.contains("name") && "Buffer must provide a name");
			name = buffer["name"];

			if (buffer.contains("layout"))
				layout = rhi::string_to_layout(buffer["layout"]);
			if (buffer.contains("type"))
				type = rhi::string_to_shader_buffer_type(buffer["type"]);
			if (buffer.contains("set"))
				set = buffer["set"];
			if (buffer.contains("binding"))
				binding = buffer["binding"];

			rhi::ShaderBuffer shader_buffer{name, type, layout, set, binding};

			assert(buffer.contains("entries") && "Buffer description must contain entries");
			{
				auto entries = buffer["entries"];
				for (auto &entry : entries)
				{
					read_entry(entry, shader_buffer.top_level(), layout);
				}
			}

			this->m_buffers.emplace_back(std::move(shader_buffer));
		}
	}

	this->generate_shader_buffers_mapping();

	// Ignore the hard-coded sets and bindings in renderer if any and use the settings ones
	this->reset_sets_bindings();
}

rhi::RenderpassType string_to_renderpass_type(const std::string &a_type)
{
	// clang-format off
	if      (a_type == "lut")                    return rhi::RenderpassType::lut;
	else if (a_type == "main")                   return rhi::RenderpassType::main;
	else if (a_type == "depth")                  return rhi::RenderpassType::depth;
	else if (a_type == "shadow")                 return rhi::RenderpassType::shadow;
	else if (a_type == "light_bin")              return rhi::RenderpassType::light_bin;
	else if (a_type == "reflection")             return rhi::RenderpassType::reflection;
	else if (a_type == "refraction")             return rhi::RenderpassType::refraction;
	else if (a_type == "pre_process")            return rhi::RenderpassType::pre_process;
	else if (a_type == "post_process")           return rhi::RenderpassType::post_process;
	else if (a_type == "tone_mapping")           return rhi::RenderpassType::tone_mapping;
	else if (a_type == "forward_light")          return rhi::RenderpassType::forward_light;
	else if (a_type == "node_transform")         return rhi::RenderpassType::node_transform;
	else if (a_type == "deferred_gbuffer")       return rhi::RenderpassType::deferred_gbuffer;
	else if (a_type == "reflection_probes")      return rhi::RenderpassType::reflection_probes;
	else if (a_type == "image_based_light")      return rhi::RenderpassType::image_based_light;
	else if (a_type == "ambient_occlusion")      return rhi::RenderpassType::ambient_occlusion;
	else if (a_type == "skeletal_transform")     return rhi::RenderpassType::skeletal_transform;
	else if (a_type == "deferred_clustered")     return rhi::RenderpassType::deferred_clustered;
	else if (a_type == "image_based_light_lut")  return rhi::RenderpassType::image_based_light_lut;
	// clang-format on

	assert(0);
	return rhi::RenderpassType::main;
}

rhi::LoadAction to_load_action(nlohmann::json a_loadaction)
{
	rhi::LoadAction load_action{rhi::LoadAction::clear};
	if (a_loadaction == "load")
		load_action = rhi::LoadAction::load;
	else if (a_loadaction == "clear")
		load_action = rhi::LoadAction::clear;
	else if (a_loadaction == "dont_care")
		load_action = rhi::LoadAction::dont_care;
	else
		assert(0 && "Invalid load/store action string provided");

	return load_action;
}

rhi::StoreAction to_store_action(nlohmann::json a_storeaction)
{
	rhi::StoreAction store_action{rhi::StoreAction::dont_care};
	if (a_storeaction == "store")
		store_action = rhi::StoreAction::store;
	else if (a_storeaction == "discard")
		store_action = rhi::StoreAction::discard;
	else if (a_storeaction == "dont_care")
		store_action = rhi::StoreAction::dont_care;
	else
		assert(0 && "Invalid store action string provided");

	return store_action;
}

rhi::RenderOutputType to_render_output_type(nlohmann::json a_type)
{
	rhi::RenderOutputType type{rhi::RenderOutputType::color};
	if (a_type == "color")
		type = rhi::RenderOutputType::color;
	else if (a_type == "depth")
		type = rhi::RenderOutputType::depth;
	else if (a_type == "resolve")
		type = rhi::RenderOutputType::resolve;
	else if (a_type == "buffer")
		type = rhi::RenderOutputType::buffer;
	else
		assert(0 && "Invalid type string provided");

	return type;
}

void read_render_pass(json &a_render_pass, std::vector<rhi::Renderpass> &a_frame_graph, ror::Vector2ui a_dimensions,
                      std::vector<rhi::TextureImage> &a_textures,
                      std::vector<rhi::ShaderBuffer> &a_buffers)
{
	rhi::Renderpass render_pass;

	if (a_render_pass.contains("disabled"))
	{
		auto disabled = a_render_pass["disabled"];
		render_pass.enabled(!disabled);
	}

	render_pass.dimensions(a_dimensions);

	ror::Vector2ui dims = render_pass.dimensions();

	if (a_render_pass.contains("width"))
		dims.x = (a_render_pass["width"]);

	if (a_render_pass.contains("height"))
		dims.y = (a_render_pass["height"]);

	render_pass.dimensions(dims);

	if (a_render_pass.contains("depends_on"))
	{
		auto parents = a_render_pass["depends_on"];
		render_pass.parent_ids(parents);
	}

	if (a_render_pass.contains("cull_mode"))
	{
		auto cull_mode = a_render_pass["cull_mode"];
		render_pass.cull_mode(rhi::string_to_cull_mode(cull_mode));
	}

	if (a_render_pass.contains("background"))
	{
		auto color = a_render_pass["background"];
		assert(color.size() == 4 && "Renderpass background color is not correctly defined");

		ror::Vector4f bc{color[0], color[1], color[2], color[3]};
		if (ror::settings().m_background_srgb_to_linear)
			ror::srgb_to_linear(bc);

		render_pass.background(bc);
	}

	assert((a_render_pass.contains("render_targets") || a_render_pass.contains("render_buffers")) && "Render pass must have render targets or render buffers");

	if (a_render_pass.contains("render_targets"))
	{
		auto render_targets = a_render_pass["render_targets"];

		std::vector<rhi::RenderTarget> rts;

		for (auto &rt : render_targets)
		{
			assert(rt.contains("index") && rt.contains("load_action") && rt.contains("store_action") && rt.contains("type") && "Render Target must contain all index, type, load and store actions");

			uint32_t index       = rt["index"];
			auto     loadaction  = rt["load_action"];
			auto     storeaction = rt["store_action"];
			auto     typ         = rt["type"];

			rhi::LoadAction       load_action  = to_load_action(loadaction);
			rhi::StoreAction      store_action = to_store_action(storeaction);
			rhi::RenderOutputType type         = to_render_output_type(typ);

			// Emplaces a RenderTarget
			assert(index < a_textures.size() && "Index is out of bound for render targets provided");

			rts.emplace_back(index, a_textures[index], load_action, store_action, type);
		}

		// Check if there are any depth buffers attached or not
		bool has_depth{false};
		for (auto &rt : rts)
		{
			if (is_pixel_format_depth_format(rt.m_target_reference.get().format()))
			{
				assert(!has_depth && "Too many depth buffers attached");
				has_depth = true;
				break;
			}
		}

		if (!has_depth)
			ror::log_info("No depth buffer provided for this render pass");

		render_pass.render_targets(std::move(rts));
	}

	if (a_render_pass.contains("render_buffers"))
	{
		auto render_buffers = a_render_pass["render_buffers"];

		std::vector<rhi::RenderBuffer> rbs;

		for (auto &rb : render_buffers)
		{
			assert(rb.contains("index") && rb.contains("load_action") && rb.contains("store_action") && rb.contains("type") && "Render Buffer must contain all index, load and store actions");

			uint32_t index       = rb["index"];
			auto     loadaction  = rb["load_action"];
			auto     storeaction = rb["store_action"];
			auto     typ         = rb["type"];

			rhi::LoadAction       load_action  = to_load_action(loadaction);
			rhi::StoreAction      store_action = to_store_action(storeaction);
			rhi::RenderOutputType type         = to_render_output_type(typ);

			// Emplaces a RenderTarget
			assert(index < a_buffers.size() && "Index is out of bound for render buffers provided");

			rbs.emplace_back(index, a_buffers[index], load_action, store_action, type);
		}

		render_pass.render_buffers(std::move(rbs));
	}

	assert(a_render_pass.contains("subpasses") && "There must be at least one subpass in a render pass");

	auto subpasses = a_render_pass["subpasses"];

	std::vector<rhi::Rendersubpass> rsps;
	rsps.reserve(subpasses.size());

	for (auto &subpass : subpasses)
	{
		rhi::Rendersubpass rsp{};

		if (subpass.contains("name"))
			rsp.name(subpass["name"]);

		if (subpass.contains("technique"))
		{
			rsp.technique(subpass["technique"] == "fragment" ? rhi::RenderpassTechnique::fragment : rhi::RenderpassTechnique::compute);
		}

		if (subpass.contains("type"))
			rsp.type(string_to_renderpass_type(subpass["type"]));

		if (subpass.contains("cull_mode"))
		{
			auto cull_mode = subpass["cull_mode"];
			rsp.cull_mode(rhi::string_to_cull_mode(cull_mode));
		}
		else
		{
			rsp.cull_mode(render_pass.cull_mode());
		}

		if (subpass.contains("state"))
			rsp.state(subpass["state"] == "transient" ? rhi::RenderpassState::transient : rhi::RenderpassState::persistent);

		if (subpass.contains("debug_output"))
			rsp.debug_output(subpass["debug_output"]);

		if (subpass.contains("program"))
			rsp.program_id(subpass["program"]);

		if (subpass.contains("rendered_inputs"))
		{
			rhi::Rendersubpass::RenderTargets render_inputs_temp;

			auto rendered_inputs = subpass["rendered_inputs"];
			render_inputs_temp.reserve(rendered_inputs.size());

			for (auto &rt_input : rendered_inputs)
			{
				assert(rt_input.contains("index") && "render_input must have an index");
				auto rt_index = rt_input["index"];
				assert(rt_index < a_textures.size() && "This rendered input doesn't exist in the render targets");
				rhi::ShaderStage stage{rhi::ShaderStage::fragment};
				if (rt_input.contains("stage"))
					stage = rhi::string_to_shader_stage(rt_input["stage"]);

				render_inputs_temp.emplace_back(nullptr, rt_index, stage);

				a_textures[rt_index].usage(rhi::TextureUsage::render_target_read);
			}
			rsp.rendered_inputs(std::move(render_inputs_temp));
		}

		if (subpass.contains("buffer_inputs"))
		{
			rhi::Rendersubpass::BufferTargets buffer_inputs_temp;

			auto buffer_inputs = subpass["buffer_inputs"];
			buffer_inputs_temp.reserve(buffer_inputs.size());

			for (auto &buffer_input : buffer_inputs)
			{
				assert(buffer_input.contains("index") && "buffer_input must have an index");
				auto bi_index = buffer_input["index"];
				assert(bi_index < a_buffers.size() && "This buffer input doesn't exist in the buffer targets");
				rhi::ShaderStage stage{rhi::ShaderStage::vertex};
				if (buffer_input.contains("stage"))
					stage = rhi::string_to_shader_stage(buffer_input["stage"]);

				buffer_inputs_temp.emplace_back(nullptr, bi_index, stage);
			}

			rsp.buffer_inputs(std::move(buffer_inputs_temp));
		}

		if (subpass.contains("subpass_inputs"))
		{
			rhi::Rendersubpass::RenderTargets render_inputs_temp;

			auto rendered_inputs = subpass["subpass_inputs"];
			render_inputs_temp.reserve(rendered_inputs.size());

			for (auto &rt_input : rendered_inputs)
			{
				assert(rt_input.contains("index") && "subpass_input must have an index");
				auto rt_index = rt_input["index"];
				assert(rt_index < a_textures.size() && "This subpass input doesn't exist in the render targets");
				rhi::ShaderStage stage{rhi::ShaderStage::fragment};
				if (rt_input.contains("stage"))
					stage = rhi::string_to_shader_stage(rt_input["stage"]);

				render_inputs_temp.emplace_back(nullptr, rt_index, stage);

				a_textures[rt_index].usage(rhi::TextureUsage::render_target_read);
			}
			rsp.input_attachments(std::move(render_inputs_temp));
		}

		// Its reference to which render targets from the renderpass this subpass is using
		auto &subpass_rt_indicies = rsp.render_targets();
		assert(subpass_rt_indicies.size() == 0 && "Subpass render targets can't be preinitialized");
		if (subpass.contains("render_targets"))
		{
			auto rts_indices = subpass["render_targets"];
			subpass_rt_indicies.reserve(rts_indices.size());
			for (size_t i = 0; i < rts_indices.size(); ++i)
				subpass_rt_indicies.push_back(rts_indices[i]);
		}

		// This means does not have render_targets, then we use all the RTs from render pass
		if (subpass_rt_indicies.size() == 0)
		{
			auto renderpass_rt_indicies = render_pass.render_targets();
			subpass_rt_indicies.reserve(renderpass_rt_indicies.size());
			for (size_t i = 0; i < renderpass_rt_indicies.size(); ++i)
				subpass_rt_indicies.push_back(static_cast<uint32_t>(i));
		}

		auto render_pass_rts = render_pass.render_targets();
		if (rsp.has_depth_attachment(render_pass_rts, subpass_rt_indicies))
			rsp.has_depth(true);
		else
			ror::log_info("No depth texture created for this subpass {}", rsp.name().c_str());

		rsps.emplace_back(std::move(rsp));
	}

	render_pass.subpasses(std::move(rsps));

	a_frame_graph.emplace_back(std::move(render_pass));
}

void Renderer::load_frame_graphs()
{
	assert(this->m_json_file.contains("frame_graph") && "There are no framegraphs provided, can't continue");
	auto frame_graph = this->m_json_file["frame_graph"];

	if (frame_graph.contains("width"))
	{
		this->m_dimensions.x = frame_graph["width"];
	}

	if (frame_graph.contains("height"))
	{
		this->m_dimensions.y = frame_graph["height"];
	}

	if (frame_graph.contains("viewport"))
	{
		auto viewport      = frame_graph["viewport"];
		this->m_viewport.x = viewport["x"];
		this->m_viewport.y = viewport["y"];
		this->m_viewport.z = viewport["w"];
		this->m_viewport.w = viewport["h"];
	}

	assert((frame_graph.contains("forward") || frame_graph.contains("deferred")) && "There must be at least one frame graph provided");

	if (frame_graph.contains("forward"))
	{
		auto forward_passes = frame_graph["forward"];
		for (auto &forward_pass : forward_passes)
		{
			ror::Vector2ui dimensions{static_cast<uint32_t>(this->m_dimensions.x), static_cast<uint32_t>(this->m_dimensions.y)};
			read_render_pass(forward_pass, this->m_frame_graphs["forward"], dimensions, this->m_images, this->m_buffers);
		}
	}

	if (frame_graph.contains("deferred"))
	{
		auto deferred_passes = frame_graph["deferred"];
		for (auto &deferred_pass : deferred_passes)
		{
			ror::Vector2ui dimensions{static_cast<uint32_t>(this->m_dimensions.x), static_cast<uint32_t>(this->m_dimensions.y)};
			read_render_pass(deferred_pass, this->m_frame_graphs["deferred"], dimensions, this->m_images, this->m_buffers);
		}
	}

	if (frame_graph.contains("current"))
		this->m_current_frame_graph = &this->m_frame_graphs[frame_graph["current"]];
	else
	{
		if (frame_graph.contains("forward"))
			this->m_current_frame_graph = &this->m_frame_graphs["forward"];
		else
			this->m_current_frame_graph = &this->m_frame_graphs["deferred"];
	}
}

const rhi::RenderTarget *Renderer::find_rendertarget_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index)
{
	for (auto &pass : a_renderpasses)
	{
		for (auto &rts : pass.render_targets())
		{
			if (a_index == rts.m_target_index)
				return &rts;
		}
	}

	// If we don't have this render target lets create one
	assert(a_index < this->m_images.size() && "Index is out of bound in the textures array");
	rhi::LoadAction       load_action{rhi::LoadAction::clear};
	rhi::StoreAction      store_action{rhi::StoreAction::store};
	rhi::RenderOutputType type{rhi::RenderOutputType::color};

	if (this->m_input_render_targets.size() == 0)
		this->m_input_render_targets.reserve(20);        // Should be enough otherwise an error will happen which I will know about

	this->m_input_render_targets.emplace_back(a_index, this->m_images[a_index], load_action, store_action, type);

	return &this->m_input_render_targets.back();        // back is ok here because this vector can't be reallocated
}

const rhi::RenderBuffer *Renderer::find_renderbuffer_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index)
{
	for (auto &pass : a_renderpasses)
	{
		for (auto &rts : pass.render_buffers())
		{
			if (a_index == rts.m_target_index)
				return &rts;
		}
	}

	// If we don't have this render buffers lets create one
	assert(a_index < this->m_buffers.size() && "Index is out of bound in the render buffers array");
	rhi::LoadAction       load_action{rhi::LoadAction::clear};
	rhi::StoreAction      store_action{rhi::StoreAction::store};
	rhi::RenderOutputType type{rhi::RenderOutputType::buffer};

	if (this->m_input_render_buffers.size() == 0)
		this->m_input_render_buffers.reserve(20);        // Should be enough otherwise an error will happen which I will know about

	this->m_input_render_buffers.emplace_back(a_index, this->m_buffers[a_index], load_action, store_action, type);

	return &this->m_input_render_buffers.back();        // back is ok here because this vector can't be reallocated
}

void Renderer::setup_references()
{
	// Lets setup all the refrences, we have to do this last because hopefully everything is loaded by now
	// All reference are set here except RenderTargets in subpasses which are done earlier
	// NOTE: I am doing this for all framegraphs loaded from renderer.json otherwise tests crash, which tests all graphs
	for (auto &graph : this->m_frame_graphs)
	{
		auto &render_passes = graph.second;
		for (auto &pass : render_passes)
		{
			// All the parent_ids into parents (refrences)
			{
				auto &pids = pass.parent_ids();

				rhi::Renderpass::Renderpasses ps{};

				assert(pids.size() <= render_passes.size() && "Parent Ids and number of render passes in this graph doesn't match");

				for (auto pid : pids)
				{
					assert(pid < render_passes.size() && "Parent Id is out of bound");
					ps.emplace_back(std::ref(render_passes[pid]));
				}

				pass.parents(std::move(ps));
			}

			for (auto &subpass : pass.subpasses())
			{
				// All the render input ids into render input references
				{
					auto &inputs = subpass.rendered_inputs();

					assert(inputs.size() <= this->m_images.size() && "Rendered Ids and number of render targets in the renderer doesn't match");

					for (auto &input : inputs)
					{
						assert(input.m_index < this->m_images.size() && "Render input Id is out of bound");
						input.m_render_output = find_rendertarget_reference(render_passes, input.m_index);
					}
				}
				// All the subpass input ids into subpass input references
				{
					auto &iads = subpass.input_attachments();

					assert(iads.size() <= this->m_images.size() && "Subpass input attachment Ids and number of subpasses in this render pass doesn't match");

					for (auto &rid : iads)
					{
						assert(rid.m_index < this->m_images.size() && "Input attachment Id is out of bound");
						rid.m_render_output = find_rendertarget_reference(render_passes, rid.m_index);
					}
				}
				// All the buffer input ids into buffer input references
				{
					auto &biid = subpass.buffer_inputs();

					for (auto &bid : biid)
					{
						assert(bid.m_index < this->m_buffers.size() && "Input attachment Id is out of bound");
						bid.m_render_output = find_renderbuffer_reference(render_passes, bid.m_index);
					}
				}
			}
		}
	}
}

void Renderer::load_specific()
{
	this->generate_shader_callbacks_mapping();

	// Order is important don't re-order
	this->load_buffers();
	this->load_programs();
	this->load_textures();
	this->load_samplers();
	this->load_environments();
	this->load_frame_graphs();
	this->setup_references();
}

void Renderer::render(ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer)
{
	(void) a_job_system;
	(void) a_event_system;
	(void) a_buffer_pack;
	(void) a_device;
	(void) a_timer;

	rhi::Swapchain surface = a_device.platform_swapchain();

	if (surface)
	{
		this->update_shader_update_candidates(a_device, a_buffer_pack);

		// Only one command_buffer is enough per frame, but the update_shader_update_candidates might have created, commited and closed its own
		rhi::CommandBuffer command_buffer{a_device};
		auto              &render_passes = this->current_frame_graph();

		for (auto &render_pass : render_passes)
			render_pass.execute(command_buffer, a_scene, surface, a_job_system, a_event_system, a_buffer_pack, a_device, a_timer, *this);

		command_buffer.present_drawable(surface);

		command_buffer.addCompletedHandler([]() {});
		command_buffer.commit();

		// TODO: Pipeline the render passes and remove this, its a performance killer
		command_buffer.wait_until_completed();
		command_buffer.release();
		surface->release();
	}
}

void Renderer::set_modifier_events(ror::EventSystem &a_event_system)
{
	EventHandle handle = create_event_handle(EventType::renderer);

	auto render_mode = [this](Event &e) {
		auto index = e.get_payload<uint32_t>();
		this->set_render_mode(index);
	};

	a_event_system.subscribe(handle, render_mode);
}

void Renderer::reset_sets_bindings()
{
	auto perframe_ubo = this->shader_buffer("per_frame_uniform");
	auto perview_ubo  = this->shader_buffer("per_view_uniform");
	auto model_ubo    = this->shader_buffer("nodes_models");
	auto offset_ubo   = this->shader_buffer("nodes_offsets");
	auto weights_ubo  = this->shader_buffer("morphs_weights");

	// These ones are hard-coded in renderer.json because these are not shared at the moment, so don't need reseting

	// auto input_ubo                     = this->shader_buffer("node_transform_input");
	// auto ouptut_ubo                    = this->shader_buffer("node_transform_output");
	// auto animations_ubo                = this->shader_buffer("animations");
	// auto animations_sampler_input_ubo  = this->shader_buffer("animations_sampler_input");
	// auto animations_sampler_output_ubo = this->shader_buffer("animations_sampler_output");
	// auto current_animations_ubo        = this->shader_buffer("current_animations");

	auto &setting = ror::settings();

	perframe_ubo->set(setting.per_frame_uniform_set());
	perframe_ubo->binding(setting.per_frame_uniform_binding());

	perview_ubo->set(setting.per_view_uniform_set());
	perview_ubo->binding(setting.per_view_uniform_binding());

	model_ubo->set(setting.nodes_model_set());
	model_ubo->binding(setting.nodes_model_binding());

	offset_ubo->set(setting.nodes_offset_set());
	offset_ubo->binding(setting.nodes_offset_binding());

	weights_ubo->set(setting.morph_weights_set());
	weights_ubo->binding(setting.morph_weights_binding());

	// Sanity check if other buffers are added in the future, make sure to update their sets and bindings
	assert(this->m_buffers.size() == 11);
}

void Renderer::set_render_mode(uint32_t a_render_mode)
{
	auto perframe_ubo = this->shader_buffer("per_frame_uniform");

	perframe_ubo->buffer_map();
	perframe_ubo->update("render_mode", &a_render_mode);
	perframe_ubo->buffer_unmap();
}

void Renderer::deferred_buffer_upload(rhi::Device &a_device, ror::Scene &a_scene)
{
	auto output_ubo = this->shader_buffer("node_transform_output");
	auto input_ubo  = this->shader_buffer("node_transform_input");
	auto model_ubo  = this->shader_buffer("nodes_models");

	auto animations_ubo                = this->shader_buffer("animations");
	auto animations_sampler_input_ubo  = this->shader_buffer("animations_sampler_input");
	auto animations_sampler_output_ubo = this->shader_buffer("animations_sampler_output");
	auto current_animations_ubo        = this->shader_buffer("current_animations");
	auto weights_ubo                   = this->shader_buffer("morphs_weights");

	uint32_t nodes_count = static_cast_safe<uint32_t>(a_scene.nodes().size());
	for (auto &model : a_scene.models())
		nodes_count += static_cast_safe<uint32_t>(model.nodes().size());

	model_ubo->update_count("node_model", nodes_count);
	input_ubo->update_count("trs_transform_input", nodes_count);
	output_ubo->update_count("trs_transform_output", nodes_count);

	uint32_t animation_size{0u};
	uint32_t animation_count{0u};
	uint32_t sampler_input_size{0u};
	uint32_t sampler_output_size{0u};
	uint32_t weights_output_size{0u};

	get_animation_sizes(a_scene, animation_size, animation_count, sampler_input_size, sampler_output_size, weights_output_size);

	// std::cout << "Animation output and input and after\n"
	//           << animations_ubo.to_glsl_string() << std::endl
	//           << animations_sampler_input_ubo.to_glsl_string() << std::endl
	//           << animations_sampler_output_ubo.to_glsl_string() << std::endl;

	animations_ubo->update_count("animation", animation_size);
	animations_sampler_input_ubo->update_count("inputs", sampler_input_size);
	animations_sampler_output_ubo->update_count("outputs", sampler_output_size);
	weights_ubo->update_count("morph_weights", weights_output_size);
	current_animations_ubo->update_count("current_animation", animation_count);

	for (auto &render_buffer : this->m_input_render_buffers)
	{
		render_buffer.m_target_reference.get().upload(a_device);        // This doesn't make it ready so it will be re-created later again unless ready is explicitly called on it
		render_buffer.m_target_reference.get().ready(true);
	}

	for (auto &render_buffer : this->m_buffers)
	{
		if (!render_buffer.ready())
		{
			render_buffer.upload(a_device);        // this will still not make it ready, have to be done later before first use, unless ready is called next
			render_buffer.ready(true);
		}
	}

	// Special treatment of weights UBO that needs filling up from mesh static weights unlike other ones, although these might get animated later
	// This is here and not in scene because it requires morph_weights shader buffer to be uploaded first
	fill_morph_weights(a_scene, *weights_ubo, weights_output_size);
}

void Renderer::dimensions(const ror::Vector4f &a_dimensions, rhi::Device &a_device)
{
	this->m_dimensions = a_dimensions;
	ror::Vector2ui dimensions{static_cast<uint32_t>(a_dimensions.x), static_cast<uint32_t>(a_dimensions.y)};

	auto render_passes = this->m_current_frame_graph;
	assert(render_passes && "There must be a current framegraph");

	for (auto &pass : *render_passes)
	{
		auto &render_supasses = pass.subpasses();
		for (auto &subpass : render_supasses)
		{
			if (subpass.technique() == rhi::RenderpassTechnique::fragment)
			{
				// Update dimensions for those render passes that I think rely on framebuffer/window size. These would usually be swapchains
				if (subpass.type() == rhi::RenderpassType::forward_light ||
				    subpass.type() == rhi::RenderpassType::main)
				{
					pass.dimensions(dimensions);
				}
			}
		}
	}

	this->upload_frame_graphs(a_device);
}

void Renderer::upload_frame_graphs(rhi::Device &a_device)
{
	auto render_passes = this->m_current_frame_graph;
	assert(render_passes && "There must be a current framegraph");
	for (auto &pass : *render_passes)
	{
		// Prepare render targets
		auto pass_render_targets = pass.render_targets();
		for (auto &render_target : pass_render_targets)
		{
			auto &texture = render_target.m_target_reference.get();
			if (!texture.ready() || ((texture.width() != pass.dimensions().x) || (texture.height() != pass.dimensions().y)))
			{
				texture.width(pass.dimensions().x);
				texture.height(pass.dimensions().y);
				texture.upload(a_device);        // Doesn't necessarily upload a texture to the GPU but creates the texture in the GPU for later use
			}
		}

		pass.upload(a_device);
	}

	// Make sure all the rendered_inputs are ready
	for (auto &pass : *render_passes)
	{
		auto pass_render_targets = pass.render_targets();

		for (auto &render_target : pass_render_targets)
		{
			auto &texture = render_target.m_target_reference.get();
			(void) texture;
			assert(texture.ready() && "Required textures are not ready");
		}
	}
}

void Renderer::upload_remaining_textures(rhi::Device &a_device)
{
	// Not all textures are uploaded as part of render passes as render targets, here we upload the rest
	for (auto &texture : this->m_images)
	{
		std::filesystem::path texture_path{texture.name()};
		if (!texture.ready())
		{
			if (texture_path.extension() != "")
			{
				auto &texture_file = ror::load_resource(texture_path, ror::ResourceSemantic::textures);
				bool  is_hdr       = texture_path.extension() == ".hdr" || texture_path.extension() == ".exr";

				rhi::read_texture_from_resource(texture_file, texture, is_hdr);
			}

			if (texture.size() > 0)
				texture.upload(a_device);
		}
	}

	// Hack in cubemap texture with numbers
	{
		rhi::TextureImage txp;
		rhi::TextureImage txn;
		rhi::TextureImage typ;
		rhi::TextureImage tyn;
		rhi::TextureImage tzp;
		rhi::TextureImage tzn;

		auto &x_p = ror::load_resource("cube_map/x_p.png", ror::ResourceSemantic::textures);
		auto &x_n = ror::load_resource("cube_map/x_n.png", ror::ResourceSemantic::textures);
		auto &y_p = ror::load_resource("cube_map/y_p.png", ror::ResourceSemantic::textures);
		auto &y_n = ror::load_resource("cube_map/y_n.png", ror::ResourceSemantic::textures);
		auto &z_p = ror::load_resource("cube_map/z_p.png", ror::ResourceSemantic::textures);
		auto &z_n = ror::load_resource("cube_map/z_n.png", ror::ResourceSemantic::textures);

		rhi::read_texture_from_resource(x_p, txp, false);
		rhi::read_texture_from_resource(x_n, txn, false);
		rhi::read_texture_from_resource(y_p, typ, false);
		rhi::read_texture_from_resource(y_n, tyn, false);
		rhi::read_texture_from_resource(z_p, tzp, false);
		rhi::read_texture_from_resource(z_n, tzn, false);

		auto &texture = this->m_images[14];
		(void) texture;

		auto ptr = texture.data();

		memcpy(ptr + texture.mips()[0].m_offset, txp.data(), texture.mips()[0].m_size);
		memcpy(ptr + texture.mips()[1].m_offset, txn.data(), texture.mips()[1].m_size);
		memcpy(ptr + texture.mips()[2].m_offset, typ.data(), texture.mips()[2].m_size);
		memcpy(ptr + texture.mips()[3].m_offset, tyn.data(), texture.mips()[3].m_size);
		memcpy(ptr + texture.mips()[4].m_offset, tzp.data(), texture.mips()[4].m_size);
		memcpy(ptr + texture.mips()[5].m_offset, tzn.data(), texture.mips()[5].m_size);

		texture.upload(a_device);

		// this->m_images.emplace_back(std::move(txp));
		// this->m_images.emplace_back(std::move(txn));
		// this->m_images.emplace_back(std::move(typ));
		// this->m_images.emplace_back(std::move(tyn));
		// this->m_images.emplace_back(std::move(tzp));
		// this->m_images.emplace_back(std::move(tzn));
	}
}

void Renderer::upload_samplers(rhi::Device &a_device)
{
	for (auto &sampler : this->m_samplers)
	{
		sampler.upload(a_device);
	}
}

uint32_t Renderer::environment_visualize_mode(uint32_t a_environment_index)
{
	assert(a_environment_index < this->environments().size() && "Environment index out of bound");

	auto &setting = ror::settings();
	auto &envs    = this->environments();
	auto &env     = envs[a_environment_index];

	if (setting.m_environment.m_mode == Settings::Environment::VisualizeMode::radiance)
		return env.radiance();
	else if (setting.m_environment.m_mode == Settings::Environment::VisualizeMode::irradiance)
		return env.irradiance();
	else if (setting.m_environment.m_mode == Settings::Environment::VisualizeMode::brdf_lut)
		return env.brdf_integration();
	else if (setting.m_environment.m_mode == Settings::Environment::VisualizeMode::skybox)
		return env.skybox();

	return env.skybox();
}

void Renderer::create_environment_mesh(rhi::Device &a_device)
{
	if (this->environments().size() == 0 || this->m_current_environment == -1)
	{
		ror::log_critical("Can't create environment meshe, no environments available or selected");
		return;
	}

	auto &setting = ror::settings();

	// First we create a single cubemap mesh for all of the environment skyboxes we will use
	rhi::VertexDescriptor vertex_descriptor = create_p_float3_i_uint16_descriptor();

	this->m_cube_map_mesh.init(a_device, rhi::PrimitiveTopology::triangles);

	auto &envs = this->environments();
	auto &env  = envs[static_cast<size_t>(this->m_current_environment)];

	std::vector<ror::Vector3f>          positions;        // Cube positions, used if brdf_integration is chosen to be visualised
	std::vector<ror::Vector3<uint16_t>> indices;          // Cube indices, used if brdf_integration is chosen to be visualised

	auto    *skybox_pso           = &this->programs()[env.skybox_pso()];
	uint32_t map_index            = this->environment_visualize_mode(static_cast<uint32_t>(this->m_current_environment));
	int32_t  brdf_integration_pso = env.brdf_integration_pso();
	auto     skybox_image         = &this->images()[map_index];
	auto     skybox_sampler       = &this->samplers()[env.skybox_sampler()];

	this->m_cube_map_mesh.set_texture(skybox_image, skybox_sampler);

	make_box_triangles_indexed(positions, indices);

	if (setting.m_environment.m_mode == Settings::Environment::VisualizeMode::brdf_lut && brdf_integration_pso != -1)
	{
		vertex_descriptor = create_p_float3_t_float2_i_uint16_descriptor();
		skybox_pso        = &this->programs()[static_cast<uint32_t>(brdf_integration_pso)];
	}

	// Skybox pso requires a reupload because previous it was created without default vertex descriptor, which is no good here
	skybox_pso->upload(a_device, vertex_descriptor, this->m_shaders, rhi::BlendMode::blend, rhi::PrimitiveTopology::triangles, "skybox_pso", true, false, false);
	this->m_cube_map_mesh.setup_program(skybox_pso);
	this->m_cube_map_mesh.setup_vertex_descriptor(&vertex_descriptor);        // Moves vertex_descriptor can't use it afterwards

	if (setting.m_environment.m_mode == Settings::Environment::VisualizeMode::brdf_lut && brdf_integration_pso != -1)
		this->m_cube_map_mesh.upload_data(reinterpret_cast<const uint8_t *>(&cube_vertex_position_uv_interleaved), 5 * 36 * sizeof(float), 36,
		                                  reinterpret_cast<const uint8_t *>(cube_index_buffer_uint16), 36 * sizeof(uint16_t), 36);
	else
		this->m_cube_map_mesh.upload_data(reinterpret_cast<const uint8_t *>(positions.data()), positions.size() * sizeof(float32_t) * 3, static_cast_safe<uint32_t>(positions.size()),
		                                  reinterpret_cast<const uint8_t *>(indices.data()), indices.size() * 3 * sizeof(uint16_t), static_cast_safe<uint32_t>(indices.size() * 3));

	// Now we save a reference to it in the dynamic meshes within the renderer
	this->m_dynamic_meshes.emplace_back(&this->m_cube_map_mesh);
}

void Renderer::upload_environments(rhi::Device &a_device)
{
	this->create_environment_mesh(a_device);

	Timer t;

	// auto &radiance = this->m_images[environment.radiance()];
	// auto &irradiance_sampler = this->m_samplers[environment.irradiance_sampler()];
	// auto &radiance_pso   = this->programs()[static_cast<size_t>(environment.radiance_pso())];
	// auto &radiance = this->m_images[environment.radiance()];
	// auto &irradiance_sampler = this->m_samplers[environment.irradiance_sampler()];

	static rhi::ShaderBuffer mipmaps_shader_buffer{"Light",
	                                               rhi::ShaderBufferType::ubo,
	                                               rhi::Layout::std140,
	                                               0, 0};

	for (auto &environment : this->m_environments)
	{
		auto env_update_lambda = [&](rhi::Device &, ror::Renderer &) {
			auto &input      = this->m_images[environment.input()];
			auto &irradiance = this->m_images[environment.irradiance()];
			auto &radiance = this->m_images[environment.radiance()];
			auto &skybox     = this->m_images[environment.skybox()];

			auto &irradiance_pso = this->programs()[static_cast<size_t>(environment.irradiance_pso())];

			std::vector<rhi::TextureImage *> images{&input, &irradiance, &radiance, &skybox};
			rhi::compute_dispatch_and_wait(a_device, {irradiance.width(), irradiance.height(), 6}, {32, 32, 1}, irradiance_pso, images, mipmaps_shader_buffer, []() {});
		};

		env_update_lambda(a_device, *this);        // Lets do the actual update and also register as shader update

		auto &irradiance_pso = this->programs()[static_cast<size_t>(environment.irradiance_pso())];
		auto &shader         = this->shaders()[static_cast<size_t>(irradiance_pso.compute_id())];
		this->m_update_callbacks_mapping[shader.shader_path().filename()].push_back(env_update_lambda);

		// irradiance.upload(a_device);
		// radiance.upload(a_device);
	}

	ror::log_info("Uploading all environments took {} nanoseconds.", t.tick());        // TODO: If time is too much, make compute_dispatch_and_wait into command_buffer
}

void Renderer::push_shader_update_candidate(std::string a_shader)
{
	this->m_shaders_update_candidates.emplace_back(ShaderUpdateRecord{2, a_shader});        // Pushes an ShaderUpdateRecord
}

void Renderer::update_shader_update_candidates(rhi::Device &a_device, rhi::BuffersPack &a_buffer_pack)
{
	for (auto &shader_candidate : this->m_shaders_update_candidates)
	{
		--shader_candidate.m_counter;

		if (shader_candidate.m_counter <= 0)
		{
			ShaderRecord &shader_record = this->m_shaders_mapping[shader_candidate.m_shader];

			shader_record.m_shader->reload();
			shader_record.m_shader->compile();
			shader_record.m_shader->upload(a_device);

			for (auto &program : shader_record.m_programs)
				program->upload(a_device, this->m_shaders, a_buffer_pack, false);

			this->update_shader(a_device, shader_candidate.m_shader);
		}
	}

	this->m_shaders_update_candidates.remove_if([](ShaderUpdateRecord &shader_record) { return shader_record.m_counter <= 0; });
}

void setup_shader_watcher(Renderer *a_renderer)
{
	std::vector<std::filesystem::path> paths{};

	// TODO: Think about how we give it path from within editor
	// paths.emplace_back("/System/Volumes/Data/personal/roar_engine/editor");
	paths.emplace_back("/System/Volumes/Data/personal/roar_engine/core/assets/shaders");

	static ror::WatchCat watcher(
	    paths, [a_renderer](std::vector<ror::WatchCatEvent> events) {
		    for (auto e : events)
			    if (e.m_type == ror::WatchCatEventType::change)
				    a_renderer->push_shader_update_candidate(e.m_path.filename());
	    },
	    0.00001f);
}

void Renderer::upload(rhi::Device &a_device, rhi::BuffersPack &a_buffer_pack)
{
	setup_shader_watcher(this);

	for (auto &shader : this->m_shaders)
	{
		shader.compile();
		shader.upload(a_device);
	}

	for (auto &program : this->m_programs)
		program.upload(a_device, this->m_shaders, a_buffer_pack, false);        // TODO: Retrieve pre-multiplied state from renderer for each shader

	// Upload all render targets now, render buffers are deffered after scenes are loaded
	for (auto &render_target : this->m_input_render_targets)
		render_target.m_target_reference.get().upload(a_device);

	// NOTE: Order is important, don't re-order, upload_remaining_textures needs to be called before environments are called
	this->upload_frame_graphs(a_device);
	this->upload_remaining_textures(a_device);
	this->upload_samplers(a_device);
	this->upload_environments(a_device);

	this->m_render_state.upload(a_device);
}

std::vector<rhi::RenderpassType> Renderer::render_pass_types(const std::vector<rhi::Renderpass> &a_pass) const
{
	std::vector<rhi::RenderpassType> passes;
	for (auto &rp : a_pass)
	{
		const auto &subpasses = rp.subpasses();
		for (auto &srp : subpasses)
		{
			passes.emplace_back(srp.type());
		}
	}

	return passes;
}

std::vector<rhi::RenderpassType> Renderer::render_pass_types() const
{
	assert(this->m_current_frame_graph);
	return this->render_pass_types(*this->m_current_frame_graph);
}

// void Renderer::add_shader_buffer(std::string a_name, rhi::ShaderInput &&a_shader_buffer)
// {
// 	// TODO: Check if already exists
// 	this->m_global_shader_buffers.emplace(std::move(a_name), std::move(a_shader_buffer));
// }

void Renderer::generate_shader_buffers_mapping()
{
	for (auto &buffer : this->m_buffers)
		this->m_buffers_mapping.emplace(buffer.top_level().m_name, &buffer);
}

void Renderer::generate_shader_callbacks_mapping()
{
	this->m_callbacks_mapping.emplace("node_transform.glsl.comp", node_transform_glsl_comp);
}

}        // namespace ror
