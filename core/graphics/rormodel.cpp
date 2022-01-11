// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021-2022
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

#include "foundation/rorjobsystem.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rornode.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"
#include <cassert>
#include <cstddef>
#include <cstring>
#include <unordered_map>
#include <vector>

#define CGLTF_IMPLEMENTATION
#include "cgltf/cgltf.h"

#include "ctpl_stl.h"

namespace ror
{

template <typename _type>
static inline auto find_safe_index(const _type &a_map, typename _type::key_type a_key) -> typename _type::mapped_type
{
	auto iter = a_map.find(a_key);
	return iter != a_map.end() ? iter->second : -1;
}

ResourceExtension extension_from_mimetype(const char *a_mimetype)
{
	if (std::strcmp(a_mimetype, "image/png") == 0)
		return ResourceExtension::texture_png;
	else if (std::strcmp(a_mimetype, "image/ktx") == 0)
		return ResourceExtension::texture_ktx;
	else if (std::strcmp(a_mimetype, "image/jpg") == 0)
		return ResourceExtension::texture_jpeg;
	else if (std::strcmp(a_mimetype, "image/jpeg") == 0)
		return ResourceExtension::texture_jpeg;

	assert(0 && "Add other mimetypes");
	return ResourceExtension::unknown;
}

rhi::TextureImage read_texture_from_file(std::filesystem::path a_texture_file)
{
	(void) a_texture_file;

	rhi::TextureImage ti;
	// TODO: Now read the image from memory using stb_image
	// First read as Resource then read from memory
	// stbi_load_from_memory(buffer,size,x,y,nc,dc);
	return ti;
}

rhi::TextureImage read_texture_from_cgltf_base64(const cgltf_options *a_options, const char *a_uri, const char *a_mimetype)
{
	const char *data_start = strchr(a_uri, ',');
	assert(data_start && "Can't find start of base64 image data");

	ResourceExtension extension{ResourceExtension::unknown};

	if (a_mimetype)
	{
		extension = extension_from_mimetype(a_mimetype);
	}
	else
	{
		std::string mimetype(a_uri, 5, 9);        // "data:image/png;base64,AAA" Thats how it should be otherwise we don't have a valid base64 image mimetype
		extension = extension_from_mimetype(mimetype.c_str());
	}

	assert(extension != ResourceExtension::unknown && "Couldn't find extension from mimetype");

	rhi::TextureImage ti;

	cgltf_size   data_size{((strlen(a_uri) + 2) / 3) << 2};        // Size calculated from the fact that base64 each 3 bytes turns into 4 bytes
	int8_t      *data = new int8_t[data_size];                     // Data that needs to be allocated for decoding
	cgltf_result res  = cgltf_load_buffer_base64(a_options, data_size, data_start, reinterpret_cast<void **>(&data));
	assert(res == cgltf_result_success && "Base64 decoding failed for image");

	assert(0);
	// TODO: Now read the image from memory using stb_image
	// stbi_load_from_memory(buffer,size,x,y,nc,dc);
	// Later need to delete data pointer
	delete[] data;

	return ti;
}

bool uri_base64_encoded(const char *a_uri)
{
	if (strncmp(a_uri, "data:", 5) == 0)
	{
		const char *comma = strchr(a_uri, ',');

		if (comma && comma - a_uri >= 7 && strncmp(comma - 7, ";base64", 7) == 0)
		{
			return true;
		}
		else
		{
			assert(0 && "Uri is encoded in an unknown format");
		}
	}
	return false;
}

// This is almost an exact copy of cgltf_load_buffer_file() but slightly modified for Resource usage
cgltf_result cgltf_load_buffer_file_as_resource(const cgltf_options *options, cgltf_size size, const char *uri, const char *gltf_path, void **out_data)
{
	void *(*memory_alloc)(void *, cgltf_size) = options->memory.alloc ? options->memory.alloc : &cgltf_default_alloc;
	void (*memory_free)(void *, void *)       = options->memory.free ? options->memory.free : &cgltf_default_free;

	char *path = static_cast<char *>(memory_alloc(options->memory.user_data, strlen(uri) + strlen(gltf_path) + 1));
	if (!path)
	{
		return cgltf_result_out_of_memory;
	}

	cgltf_combine_paths(path, gltf_path, uri);

	// after combining, the tail of the resulting path is a uri; decode_uri converts it into path
	cgltf_decode_uri(path + strlen(path) - strlen(uri));

	auto &resource = load_resource(path, ResourceSemantic::models);
	auto &data     = resource.get_data();

	assert(data.size() == size && "Resource loaded of wrong size");
	(void) size;

	memory_free(options->memory.user_data, path);

	// Data pointer from the Resource is aliased into out_data within the gltf file
	// This is ok, because the deleter for this type in cgltf is void so won't be deleted under Resource's feet
	*out_data = const_cast<void *>(reinterpret_cast<const void *>(data.data()));        // This is gross but no other way but to make resource get_data() non-const, which I don't want to do

	return cgltf_result_success;
}

// This will load all buffers mentioned in the gltf as resources if file or base64 decoded otherwise
// This is almost an exact copy of cgltf_load_buffers() but slightly modified for Resource usage
cgltf_result cgltf_load_buffers_as_resource(const cgltf_options *options, cgltf_data *data, const char *gltf_path)
{
	if (options == nullptr)
	{
		return cgltf_result_invalid_options;
	}

	// This happenes if we are reading binary gltf glb
	if (data->buffers_count && data->buffers[0].data == nullptr && data->buffers[0].uri == nullptr && data->bin)
	{
		if (data->bin_size < data->buffers[0].size)
		{
			return cgltf_result_data_too_short;
		}

		data->buffers[0].data             = const_cast<void *>(data->bin);        // This is not great but in C++ its not allowed otherwise
		data->buffers[0].data_free_method = cgltf_data_free_method_none;
	}

	for (cgltf_size i = 0; i < data->buffers_count; ++i)
	{
		if (data->buffers[i].data)
		{
			continue;
		}

		const char *uri = data->buffers[i].uri;

		if (uri == nullptr)
		{
			continue;
		}

		if (uri_base64_encoded(uri))
		{
			const char *comma = strchr(uri, ',');
			assert(comma && "Base64 encoded URI doesn't have a comma");

			cgltf_result res                  = cgltf_load_buffer_base64(options, data->buffers[i].size, comma + 1, &data->buffers[i].data);
			data->buffers[i].data_free_method = cgltf_data_free_method_memory_free;

			if (res != cgltf_result_success)
			{
				return res;
			}
		}
		else if (strstr(uri, "://") == nullptr && gltf_path)
		{
			cgltf_result res                  = cgltf_load_buffer_file_as_resource(options, data->buffers[i].size, uri, gltf_path, &data->buffers[i].data);
			data->buffers[i].data_free_method = cgltf_data_free_method_none;        // NOTE: No free method provided because Resource owns data

			if (res != cgltf_result_success)
			{
				return res;
			}
		}
		else
		{
			return cgltf_result_unknown_format;
		}
	}

	return cgltf_result_success;
}

// typedef enum cgltf_component_type
// {
//	cgltf_component_type_invalid,
//	cgltf_component_type_r_8,   /* BYTE */
//	cgltf_component_type_r_8u,  /* UNSIGNED_BYTE */
//	cgltf_component_type_r_16,  /* SHORT */
//	cgltf_component_type_r_16u, /* UNSIGNED_SHORT */
//	cgltf_component_type_r_32u, /* UNSIGNED_INT */
//	cgltf_component_type_r_32f, /* FLOAT */
// } cgltf_component_type;

// typedef enum cgltf_type
// {
//	cgltf_type_invalid,
//	cgltf_type_scalar,
//	cgltf_type_vec2,
//	cgltf_type_vec3,
//	cgltf_type_vec4,
//	cgltf_type_mat2,
//	cgltf_type_mat3,
//	cgltf_type_mat4,
// } cgltf_type;

rhi::Format get_format_from_gltf_type_format(cgltf_type a_type, cgltf_component_type a_component_type)
{
	if (a_type == cgltf_type::cgltf_type_vec4 || a_type == cgltf_type::cgltf_type_mat2)
	{
		// clang-format off
		switch (a_component_type)
		{
		case cgltf_component_type::cgltf_component_type_invalid:       assert(0 && "cgltf_component_type isn't valid"); break;
		case cgltf_component_type::cgltf_component_type_r_8:           return rhi::VertexFormat::int8_4;
		case cgltf_component_type::cgltf_component_type_r_8u:          return rhi::VertexFormat::uint8_4;
		case cgltf_component_type::cgltf_component_type_r_16:          return rhi::VertexFormat::int16_4;
		case cgltf_component_type::cgltf_component_type_r_16u:         return rhi::VertexFormat::uint16_4;
		case cgltf_component_type::cgltf_component_type_r_32u:         return rhi::VertexFormat::uint32_4;
		case cgltf_component_type::cgltf_component_type_r_32f:		   return rhi::VertexFormat::float32_4;
		}
		// clang-format on
	}
	else if (a_type == cgltf_type::cgltf_type_vec3)
	{
		// clang-format off
		switch (a_component_type)
		{
		case cgltf_component_type::cgltf_component_type_invalid:       assert(0 && "cgltf_component_type isn't valid"); break;
		case cgltf_component_type::cgltf_component_type_r_8:           return rhi::VertexFormat::int8_3;
		case cgltf_component_type::cgltf_component_type_r_8u:          return rhi::VertexFormat::uint8_3;
		case cgltf_component_type::cgltf_component_type_r_16:          return rhi::VertexFormat::int16_3;
		case cgltf_component_type::cgltf_component_type_r_16u:         return rhi::VertexFormat::uint16_3;
		case cgltf_component_type::cgltf_component_type_r_32u:         return rhi::VertexFormat::uint32_3;
		case cgltf_component_type::cgltf_component_type_r_32f:		   return rhi::VertexFormat::float32_3;
		}
		// clang-format on
	}
	else if (a_type == cgltf_type::cgltf_type_vec2)
	{
		// clang-format off
		switch (a_component_type)
		{
		case cgltf_component_type::cgltf_component_type_invalid:       assert(0 && "cgltf_component_type isn't valid"); break;
		case cgltf_component_type::cgltf_component_type_r_8:           return rhi::VertexFormat::int8_2;
		case cgltf_component_type::cgltf_component_type_r_8u:          return rhi::VertexFormat::uint8_2;
		case cgltf_component_type::cgltf_component_type_r_16:          return rhi::VertexFormat::int16_2;
		case cgltf_component_type::cgltf_component_type_r_16u:         return rhi::VertexFormat::uint16_2;
		case cgltf_component_type::cgltf_component_type_r_32u:         return rhi::VertexFormat::uint32_2;
		case cgltf_component_type::cgltf_component_type_r_32f:		   return rhi::VertexFormat::float32_2;
		}
		// clang-format on
	}
	else if (a_type == cgltf_type::cgltf_type_scalar)
	{
		// clang-format off
		switch (a_component_type)
		{
		case cgltf_component_type::cgltf_component_type_invalid:       assert(0 && "cgltf_component_type isn't valid"); break;
		case cgltf_component_type::cgltf_component_type_r_8:           return rhi::VertexFormat::int8_1;
		case cgltf_component_type::cgltf_component_type_r_8u:          return rhi::VertexFormat::uint8_1;
		case cgltf_component_type::cgltf_component_type_r_16:          return rhi::VertexFormat::int16_1;
		case cgltf_component_type::cgltf_component_type_r_16u:         return rhi::VertexFormat::uint16_1;
		case cgltf_component_type::cgltf_component_type_r_32u:         return rhi::VertexFormat::uint32_1;
		case cgltf_component_type::cgltf_component_type_r_32f:		   return rhi::VertexFormat::float32_1;
		}
		// clang-format on
	}
	else if (a_type == cgltf_type::cgltf_type_mat4)
	{
		if (a_component_type == cgltf_component_type::cgltf_component_type_r_32f)
			return rhi::VertexFormat::float32_16;
		else
			assert(0 && "This type of mat4 is not supported");
	}
	else if (a_type == cgltf_type::cgltf_type_mat3)
	{
		if (a_component_type == cgltf_component_type::cgltf_component_type_r_32f)
			return rhi::VertexFormat::float32_9;
		else
			assert(0 && "This type of mat4 is not supported");
	}

	return rhi::VertexFormat::float32_4;
}

void Model::load_from_gltf_file(std::filesystem::path a_filename)
{
	// Lets start by reading a_filename via resource cache
	auto &resource = load_resource(a_filename, ResourceSemantic::models);
	auto &filename = resource.absolute_path();

	// Get an instance of job system
	auto &js = ror::get_job_system();
	auto &bp = rhi::get_buffers_pack();

	cgltf_options options{};        // Default setting
	cgltf_data   *data{nullptr};
	// Since we have loaded as resource use cgltf_parse instead of cgltf_parse_file(&options, filename.c_str(), &data);
	cgltf_result result = cgltf_parse(&options, resource.get_data().data(), resource.get_data().size(), &data);

	if (result != cgltf_result_success)
	{
		ror::log_critical("Can't load gltf file");
		return;
	}

	// Load all the buffers as resource(s)
	cgltf_result buffers_result = cgltf_load_buffers_as_resource(&options, data, filename.c_str());

	if (buffers_result != cgltf_result_success)
	{
		ror::log_critical("Can't load gltf binary data");
		return;
	}

	std::filesystem::path root_dir = filename.parent_path();

	if (result == cgltf_result_success)
	{
		std::unordered_map<const cgltf_image *, int32_t>    image_to_index{};
		std::unordered_map<const cgltf_sampler *, int32_t>  sampler_to_index{};
		std::unordered_map<const cgltf_texture *, int32_t>  texture_to_index{};
		std::unordered_map<const cgltf_material *, int32_t> material_to_index{};
		std::unordered_map<const cgltf_buffer *, int32_t>   buffer_to_index{};
		std::unordered_map<const cgltf_mesh *, int32_t>     mesh_to_index{};
		std::unordered_map<const cgltf_node *, int32_t>     node_to_index{};
		std::unordered_map<const cgltf_skin *, int32_t>     skin_to_index{};

		auto from_file_lambda = [](std::filesystem::path &a_texture_path) -> rhi::TextureImage {
			return ror::read_texture_from_file(a_texture_path.c_str());
		};

		auto from_base64_lambda = [&options](const char *a_uri, const char *a_mimetype) -> rhi::TextureImage {
			return ror::read_texture_from_cgltf_base64(&options, a_uri, a_mimetype);
		};

		// Read all the images
		std::vector<JobHandle<rhi::TextureImage>> future_texures{};
		future_texures.reserve(data->images_count);

		for (size_t i = 0; i < data->images_count; ++i)
		{
			const char *uri = data->images[i].uri;
			assert(uri && "Image URI is null, only support URI based images");        // TODO: Read images from bufferviews
			if (uri_base64_encoded(uri))
			{
				future_texures.emplace_back(js.push_job(from_base64_lambda, uri, data->images[i].mime_type));        // vector of job_handles
			}
			else        // Assumes its a path otherwise
			{
				auto texture_path = root_dir / uri;
				future_texures.emplace_back(js.push_job(from_file_lambda, texture_path));        // vector of job_handles
			}

			image_to_index.emplace(&data->images[i], i);
		}

		// Read all the samplers
		// TODO: Turn me into a job
		this->m_samplers.reserve(data->samplers_count + 1);

		// Lets have a default sampler at index 0
		this->m_samplers.emplace_back();

		for (size_t i = 0; i < data->samplers_count; ++i)
		{
			rhi::TextureSampler sampler;

			sampler.m_mag_filter = static_cast<rhi::TextureFilter>(data->samplers[i].mag_filter);
			sampler.m_min_filter = static_cast<rhi::TextureFilter>(data->samplers[i].min_filter);
			sampler.m_wrap_s     = static_cast<rhi::TextureWrap>(data->samplers[i].wrap_s);
			sampler.m_wrap_t     = static_cast<rhi::TextureWrap>(data->samplers[i].wrap_t);

			this->m_samplers.emplace_back(std::move(sampler));
			sampler_to_index.emplace(&data->samplers[i], i + 1);        // Index 0 is default hence the +1
		}

		// Read all the textures
		// TODO: Turn me into a job
		this->m_textures.reserve(data->textures_count);

		for (size_t i = 0; i < data->textures_count; ++i)
		{
			assert(data->textures[i].image && "Texture must have an image");

			rhi::Texture texture;
			texture.m_texture_image = find_safe_index(image_to_index, data->textures[i].image);
			assert(texture.m_texture_image.m_handle != -1 && "Couldn't find the image loaded for the texture");

			// if (data->textures[i].sampler != nullptr)
			//	texture.m_texture_sampler = find_safe_index(sampler_to_index, data->textures[i].sampler);

			// // If no samplers provided use default at index 0
			// if (texture.m_texture_sampler.m_handle == -1)
			//	texture.m_texture_sampler = 0;

			auto texture_sampler = -1;

			if (data->textures[i].sampler != nullptr)
				texture_sampler = find_safe_index(sampler_to_index, data->textures[i].sampler);

			// If no samplers provided use default at index 0
			if (texture_sampler == -1)
				texture_sampler = 0;

			this->m_textures.emplace_back(std::move(texture));
			texture_to_index.emplace(&data->textures[i], i);
		}

		// Read all the materials
		// TODO: Turn me into a job
		this->m_materials.reserve(data->materials_count);

		for (size_t i = 0; i < data->materials_count; ++i)
		{
			const auto convert_to_vec4      = [](const float input[4]) { return ror::Vector4f{input[0], input[1], input[2], input[3]}; };
			const auto convert_to_vec3      = [](const float input[3]) { return ror::Vector3f{input[0], input[1], input[2]}; };
			const auto convert_vec3_to_vec4 = [](const float input[3]) { return ror::Vector4f{input[0], input[1], input[2], 1.0f}; };
			// Using calculations from https://google.github.io/filament/Filament.html#toc4.8.3.2 to calculate f0 from ior
			const auto convert_ior_to_f0 = [](const float ior) { return ((ior - 1.0f) * (ior - 1.0f)) / ((ior + 1.0f) * (ior + 1.0f)); };

			const cgltf_material &mat = data->materials[i];

			Material material;
			material.m_double_sided     = mat.double_sided;
			material.m_opacity.m_factor = mat.alpha_cutoff;        // TODO: Check if there is a texture instead

			if (mat.has_pbr_metallic_roughness)
			{
				auto mro_texture_id = find_safe_index(texture_to_index, mat.pbr_metallic_roughness.metallic_roughness_texture.texture);        // TODO: Separate textures

				// material.m_type = Material::MaterialType::metalic_roughness; // Default already
				material.m_base_color.m_factor = convert_to_vec4(mat.pbr_metallic_roughness.base_color_factor);
				material.m_metallic.m_factor   = mat.pbr_metallic_roughness.metallic_factor;
				material.m_roughness.m_factor  = mat.pbr_metallic_roughness.roughness_factor;
				material.m_occlusion.m_factor  = mat.occlusion_texture.scale;

				material.m_base_color.m_texture = find_safe_index(texture_to_index, mat.pbr_metallic_roughness.base_color_texture.texture);
				material.m_metallic.m_texture   = mro_texture_id;
				material.m_roughness.m_texture  = mro_texture_id;
				material.m_occlusion.m_texture  = mro_texture_id;

				material.m_base_color.m_uv_map = static_cast<uint32_t>(mat.pbr_metallic_roughness.base_color_texture.texcoord);
				material.m_metallic.m_uv_map   = static_cast<uint32_t>(mat.pbr_metallic_roughness.metallic_roughness_texture.texcoord);
				material.m_occlusion.m_uv_map  = material.m_metallic.m_uv_map;
				material.m_roughness.m_uv_map  = material.m_metallic.m_uv_map;
				material.m_reflectance_factor  = mat.has_ior ? convert_ior_to_f0(mat.ior.ior) : 1.0f;

				// TODO: Find out why there is scale and transform in textures but not mentioned in gltf 2.0 spec
				// mat.pbr_metallic_roughness.base_color_texture.scale
				// mat.pbr_metallic_roughness.base_color_texture.transform
			}
			else
			{
				assert(mat.has_pbr_specular_glossiness && "Neither Metalic roughness nor specular glossiness material not supported");
				// TODO: File out specular glossiness data
				assert(0);
			}

			if (mat.occlusion_texture.texture)
			{
				// TODO: Combine the texture, with metalic roughness if not together already
				material.m_metallic.m_factor  = mat.occlusion_texture.scale;        // NOTE: This isn't fully correct, gltf doesn't provide an occlusion scale but its close enough
				material.m_metallic.m_texture = find_safe_index(texture_to_index, mat.occlusion_texture.texture);
				material.m_metallic.m_uv_map  = static_cast<uint32_t>(mat.occlusion_texture.texcoord);
			}

			material.m_normal.m_texture = find_safe_index(texture_to_index, mat.normal_texture.texture);
			material.m_normal.m_uv_map  = static_cast<uint32_t>(mat.normal_texture.texcoord);

			switch (mat.alpha_mode)
			{
				case cgltf_alpha_mode_opaque:
					material.m_blend_mode = rhi::BlendMode::opaque;
					break;
				case cgltf_alpha_mode_mask:
					material.m_blend_mode = rhi::BlendMode::mask;
					break;
				case cgltf_alpha_mode_blend:
					material.m_blend_mode = rhi::BlendMode::blend;
					break;
			}

			material.m_material_model = (mat.unlit ? rhi::MaterialModel::unlit : rhi::MaterialModel::lit);

			material.m_emissive.m_factor  = convert_vec3_to_vec4(mat.emissive_factor);
			material.m_emissive.m_texture = find_safe_index(texture_to_index, mat.emissive_texture.texture);
			material.m_emissive.m_uv_map  = static_cast<uint32_t>(mat.emissive_texture.texcoord);

			if (mat.has_emissive_strength)
				material.m_emissive.m_factor.w = mat.emissive_strength.emissive_strength;

			if (mat.has_clearcoat)
			{
				material.m_clearcoat_normal.m_texture  = find_safe_index(texture_to_index, mat.clearcoat.clearcoat_normal_texture.texture);
				material.m_clearcoat_normal.m_factor.x = mat.clearcoat.clearcoat_factor;
				material.m_clearcoat_normal.m_factor.y = mat.clearcoat.clearcoat_roughness_factor;

				material.m_clearcoat.m_texture           = find_safe_index(texture_to_index, mat.clearcoat.clearcoat_texture.texture);
				material.m_clearcoat_roughness.m_texture = find_safe_index(texture_to_index, mat.clearcoat.clearcoat_roughness_texture.texture);
			}

			if (mat.has_transmission)
			{
				material.m_transmission.m_texture  = find_safe_index(texture_to_index, mat.transmission.transmission_texture.texture);
				material.m_transmission.m_factor.x = mat.transmission.transmission_factor;
				material.m_transmission.m_factor.y = 0.0f;        // Transmission roughness 0.0f, Should this be texture.scale?
			}

			if (mat.has_specular)
			{
				// typedef struct cgltf_specular
				// {
				//	cgltf_texture_view specular_texture;
				//	cgltf_texture_view specular_color_texture;
				//	cgltf_float        specular_color_factor[3];
				//	cgltf_float        specular_factor;
				// } cgltf_specular;

				assert(0 && "Don't support specular extension, probably shouldn't assert thought");
			}

			if (mat.has_volume)
			{
				// typedef struct cgltf_volume
				// {
				//	cgltf_texture_view thickness_texture;
				//	cgltf_float        thickness_factor;
				//	cgltf_float        attenuation_color[3];
				//	cgltf_float        attenuation_distance;
				// } cgltf_volume;
				assert(0 && "Don't support volume extension, probably shouldn't assert thought");
			}

			if (mat.has_sheen)
			{
				material.m_sheen_color.m_texture     = find_safe_index(texture_to_index, mat.sheen.sheen_color_texture.texture);
				material.m_sheen_color.m_factor      = convert_to_vec3(mat.sheen.sheen_color_factor);
				material.m_sheen_roughness.m_texture = find_safe_index(texture_to_index, mat.sheen.sheen_roughness_texture.texture);
				material.m_sheen_roughness.m_factor  = mat.sheen.sheen_roughness_factor;
			}

			this->m_materials.emplace_back(std::move(material));

			material_to_index.emplace(&mat, i);
		}

		assert(data->buffers_count > 0 && "No buffers available in the gltf file, its probably ok though. verify!");
		this->m_buffers.reserve(data->buffers_count);
		for (size_t i = 0; i < data->buffers_count; ++i)
		{
			// In the following I am creating std::vector<uint8_t> buffer from data->buffers[i] and emplacing it directly into m_buffers
			// Note we don't need a reserve here becaues the vector size is provided in constructor to emplace_back
			this->m_buffers.emplace_back(static_cast<BinBuffer *>(data->buffers[i].data), static_cast<BinBuffer *>(data->buffers[i].data) + data->buffers[i].size);
			buffer_to_index.emplace(&data->buffers[i], i);
		}

		// Read all the meshes
		// TODO: Make me a job
		this->m_meshes.resize(data->meshes_count);
		for (size_t i = 0; i < data->meshes_count; ++i)
		{
			Mesh             &mesh  = this->m_meshes[i];
			const cgltf_mesh &cmesh = data->meshes[i];

			mesh.m_parts.resize(cmesh.primitives_count);
			mesh.m_primitive_types.resize(cmesh.primitives_count);
			mesh.m_has_indices_states.resize(cmesh.primitives_count);
			// The last 2 are important and definitely needs reserving because these are BufferAllocated
			mesh.m_bounding_boxes.resize(cmesh.primitives_count);
			mesh.m_material_indices.resize(cmesh.primitives_count);

			for (size_t j = 0; j < cmesh.primitives_count; ++j)
			{
				const cgltf_primitive &cprim = cmesh.primitives[j];
				Mesh::Drawable        &prim  = mesh.m_parts[j];

				assert(cprim.type == cgltf_primitive_type_triangles && "Mesh primitive type is not triangles which is the only supported type at the moment");
				mesh.m_primitive_types[i] = static_cast<rhi::PrimitiveTopology>(cprim.type);        // TODO: Create a switch for this, incase the types don't match in the future, also support more types

				if (cprim.has_draco_mesh_compression)
					ror::log_critical("Mesh has draco mesh compression but its not supported");

				if (cprim.material)
					mesh.m_material_indices[j] = find_safe_index(material_to_index, cprim.material);

				// Read vertex indices buffer
				if (cprim.indices)
				{
					assert(cprim.indices->type == cgltf_type_scalar && "Indices are not the right type, only SCALAR indices supported");
					assert((cprim.indices->component_type == cgltf_component_type_r_32u || cprim.indices->component_type == cgltf_component_type_r_16u || cprim.indices->component_type == cgltf_component_type_r_8u) &&
						   "Indices are not in the right component type , only uint8_t, uint16_t and uint32_t supported");

					assert(cprim.indices->buffer_view && "Indices doesn't have a valid buffer view");
					// assert(cprim.indices->buffer_view->type == cgltf_buffer_view_type_indices && "Indices buffer view type is wrong"); type is always invalid, because no such thing in bufferView in glTF

					if (cprim.indices->buffer_view->has_meshopt_compression)
						ror::log_critical("Mesh has meshopt_compression but its not supported");

					mesh.m_has_indices_states[j] = true;
					auto index_format            = get_format_from_gltf_type_format(cprim.indices->type, cprim.indices->component_type);

					// TODO: GL expects stride to be zero if data is tightly packed

					// TODO: Check if accessor is sparse to unpack it using cgltf_accessor_unpack_floats first before calling the bellow
					auto attrib_accessor   = cprim.indices;
					auto buffer_index      = find_safe_index(buffer_to_index, cprim.indices->buffer_view->buffer);
					auto indices_byte_size = cgltf_calc_size(attrib_accessor->type, attrib_accessor->component_type);
					auto stride            = cprim.indices->buffer_view->stride;

					uint8_t *data_pointer = reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data());        // or cprim.indices->buffer_view->buffer??? TODO: Verify

					std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer, attrib_accessor->count * indices_byte_size, stride};
					attribs_data.emplace(rhi::BufferSemantic::vertex_index, std::move(data_tuple));

					avd.add(rhi::BufferSemantic::vertex_index, index_format, &bp);
				}

				// Read all other vertex attributes
				for (size_t k = 0; k < cprim.attributes_count; ++k)
				{
					const cgltf_attribute &attrib = cprim.attributes[k];

					assert(attrib.data->buffer_view && "rhi::BufferView doesn't have a valid buffer view");
					assert(!attrib.data->is_sparse && "Don't support sparse attribute accessors");

					AttributeIndices current_index = AttributeIndices::vertex_position_index;

					switch (attrib.type)
					{
						case cgltf_attribute_type_position:
							assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec3 && "Position not in the right format, float3 required");        // FIXME: Allow other types
							assert(attrib.data->has_min && attrib.data->has_max && "Position attributes must provide min and max");
							assert(attrib.index == 0 && "Don't suport more than 1 position");

							current_index = AttributeIndices::vertex_position_index;
							mesh.m_bounding_boxes[j].create_from_min_max({attrib.data->min[0], attrib.data->min[1], attrib.data->min[2]},
																		 {attrib.data->max[0], attrib.data->max[1], attrib.data->max[2]});
							break;
						case cgltf_attribute_type_normal:
							assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec3 && "Normal not in the right format, float3 required");
							assert(attrib.index == 0 && "Don't suport more than 1 normal");
							current_index = AttributeIndices::vertex_normal_index;
							break;
						case cgltf_attribute_type_tangent:
							assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec4 && "Tangent not in the right format, float3 required");
							assert(attrib.index == 0 && "Don't suport more than 1 tangent");
							current_index = AttributeIndices::vertex_tangent_index;
							break;
						case cgltf_attribute_type_texcoord:
							assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec2 && "Texture coordinate not in the right format, float2 required");
							assert(attrib.index < 2 && "Don't support more than 2 texture coordinate sets");
							current_index = static_cast<AttributeIndices>(AttributeIndices::vertex_texture_coord_0_index + attrib.index);
							break;
						case cgltf_attribute_type_color:
							// assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec3 && "Color not in the right format, float3 required");
							assert(attrib.index < 2 && "Don't support more than 2 color sets");
							current_index = static_cast<AttributeIndices>(AttributeIndices::vertex_color_0_index + attrib.index);
							break;
						case cgltf_attribute_type_joints:
							assert((attrib.data->component_type == cgltf_component_type_r_16u || attrib.data->component_type == cgltf_component_type_r_8u) &&
								   attrib.data->type == cgltf_type_vec4 && "Joints not in the right format, unsigned8/16_4 required");
							assert(attrib.index < 2 && "Don't support more than 2 joint sets");
							current_index = static_cast<AttributeIndices>(AttributeIndices::vertex_bone_id_0_index + attrib.index);
							break;
						case cgltf_attribute_type_weights:
							assert((attrib.data->component_type == cgltf_component_type_r_32f || attrib.data->component_type == cgltf_component_type_r_16u || attrib.data->component_type == cgltf_component_type_r_8u) &&
								   attrib.data->type == cgltf_type_vec4 && "Weights not in the right format, unsigned_8/16/float_4 required");
							assert(attrib.index < 2 && "Don't support more than 2 weight sets");
							current_index = static_cast<AttributeIndices>(AttributeIndices::vertex_weight_0_index + attrib.index);
							break;
						case cgltf_attribute_type_invalid:
							assert(0 && "rhi::BufferView not valid yet");
							break;
					}

					// TODO: GL expects stride to be zero if data is tightly packed
					// TODO: Check for sparse accessors and unpack it

					const auto *attrib_accessor  = attrib.data;
					auto        attrib_format    = get_format_from_gltf_type_format(attrib_accessor->type, attrib_accessor->component_type);
					auto        buffer_index     = find_safe_index(buffer_to_index, attrib_accessor->buffer_view->buffer);
					auto        attrib_byte_size = cgltf_calc_size(attrib_accessor->type, attrib_accessor->component_type);
					auto        stride           = attrib_accessor->buffer_view->stride;
					uint8_t    *data_pointer     = reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data());        // or cprim.indices->buffer_view->buffer??? TODO: Verify

					std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer, attrib_accessor->count * attrib_byte_size, stride};
					attribs_data.emplace(rhi::BufferSemantic::vertex_index, std::move(data_tuple));

					avd.add(static_cast<rhi::BufferSemantic>(current_index), attrib_format);
				}

				// Now upload data from all the attributes int avd
				avd.upload(attribs_data, &bp);

				// Read morph targets
				assert(cprim.targets_count <= max_morph_targets && "Too many morph targets provided");
				for (size_t k = 0; k < cprim.targets_count; ++k)
				{
					const cgltf_morph_target &target = cprim.targets[k];

					std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>> morph_targets_attribs_data;

					for (size_t l = 0; l < target.attributes_count; ++l)
					{
						const cgltf_attribute &attrib = target.attributes[l];

						assert(!attrib.data->is_sparse && "Don't support sparse attribute accessors");
						assert(attrib.data->buffer_view && "rhi::BufferView doesn't have a valid buffer view");
						AttributeIndices current_index = AttributeIndices::vertex_position_index;

						switch (attrib.type)
						{
							case cgltf_attribute_type_position:
								assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec3 && "Position not in the right format, float3 required");        // FIXME: Allow other types
								assert(attrib.data->has_min && attrib.data->has_max && "Position attributes must provide min and max");

								current_index = AttributeIndices::vertex_position_index;
								{
									ror::BoundingBoxf total_bbox;

									total_bbox.create_from_min_max({attrib.data->min[0], attrib.data->min[1], attrib.data->min[2]},
																   {attrib.data->max[0], attrib.data->max[1], attrib.data->max[2]});

									// Not actually precise bounding box will do for now
									mesh.m_bounding_boxes[j].add_bounding(total_bbox);
								}
								break;
							case cgltf_attribute_type_normal:
								assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec3 && "Normal not in the right format, float3 required");
								current_index = AttributeIndices::vertex_normal_index;
								break;
							case cgltf_attribute_type_tangent:
								// assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec4 && "Tangent not in the right format, float3 required");
								current_index = AttributeIndices::vertex_tangent_index;
								break;
							case cgltf_attribute_type_texcoord:
							case cgltf_attribute_type_color:
							case cgltf_attribute_type_joints:
							case cgltf_attribute_type_weights:
							case cgltf_attribute_type_invalid:
								assert(0 && "Morph targer not valid for this attribute");
								break;
						}

						// TODO: GL expects stride to be zero if data is tightly packed
						// TODO: Check for sparse accessors and unpack it

						const auto *attrib_accessor  = attrib.data;
						auto        attrib_format    = get_format_from_gltf_type_format(attrib_accessor->type, attrib_accessor->component_type);
						auto        buffer_index     = find_safe_index(buffer_to_index, attrib_accessor->buffer_view->buffer);
						auto        attrib_byte_size = cgltf_calc_size(attrib_accessor->type, attrib_accessor->component_type);
						auto        stride           = attrib_accessor->buffer_view->stride;
						uint8_t    *data_pointer     = reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data());        // or cprim.indices->buffer_view->buffer??? TODO: Verify

						std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer, attrib_accessor->count * attrib_byte_size, stride};
						attribs_data.emplace(rhi::BufferSemantic::vertex_index, std::move(data_tuple));

						amts[k].add(static_cast<rhi::BufferSemantic>(current_index), attrib_format);
					}

					// TODO: Now copy all the buffer_views for morph_targets
					amts[k].upload(morph_targets_attribs_data, &bp);
				}

				// Save Morph target weights
				mesh.m_morph_weights.reserve(cmesh.weights_count);
				assert(cmesh.weights_count <= max_morph_targets && "More than supported morpth target weights");

				// TODO: Do a bulk copy please once tested and it works
				for (size_t m = 0; m < cmesh.weights_count; ++m)
					mesh.m_morph_weights.emplace_back(cmesh.weights[m]);

				// No need to do this we were working on prim = m_meshes[i].m_parts[j] reference
				// mesh.m_parts.emplace_back(prim);
			}

			// No need to do this we were working on mesh = m_meshes[i] reference
			// this->m_meshes.emplace_back(mesh);
			mesh_to_index.emplace(&cmesh, i);
		}

		// Read all the nodes
		this->m_nodes.resize(data->nodes_count);                  // Will fill the empty Node just created later
		this->m_nodes_side_data.resize(data->nodes_count);        // Will fill the empty Node just created later
		for (size_t i = 0; i < data->nodes_count; ++i)
		{
			const cgltf_node &cnode = data->nodes[i];
			node_to_index.emplace(&cnode, i);
		}

		// Read the skins
		// TODO: Jobify please
		this->m_skins.reserve(data->skins_count);
		for (size_t i = 0; i < data->skins_count; ++i)
		{
			const cgltf_skin &cskin = data->skins[i];
			assert(cskin.joints_count == cskin.inverse_bind_matrices->count && "Skin joints should be equal to bind matrices");

			Skin skin;
			if (cskin.skeleton)
				skin.m_root = find_safe_index(node_to_index, cskin.skeleton);

			// Important: skin.m_node_index is set when we read the scene in the next section

			if (cskin.inverse_bind_matrices)
			{
				assert(cskin.inverse_bind_matrices->count == cskin.joints_count && "Inverse bind matrix count must match joints count");

				auto *inverse_bind_matrices_accessor = cskin.inverse_bind_matrices;
				skin.m_inverse_bind_matrices.resize(cskin.inverse_bind_matrices->count);
				cgltf_accessor_unpack_floats(inverse_bind_matrices_accessor,
											 reinterpret_cast<cgltf_float *>(skin.m_inverse_bind_matrices.data()),
											 skin.m_inverse_bind_matrices.size() * 16);

				auto attrib_byte_size = cgltf_calc_size(inverse_bind_matrices_accessor->type, inverse_bind_matrices_accessor->component_type);

				assert(inverse_bind_matrices_accessor->buffer_view->stride == attrib_byte_size && "Looks like inverse_bind_matrices data is interleaved, not supported");

				// TODO: This is only required because GL expects stride to be zero if data is tightly packed
				if (inverse_bind_matrices_accessor->buffer_view->stride >= attrib_byte_size)
				{
					ror::log_error("Stride was suppose to be adjusted for GL, it won't work in Vulkan/Metal/DX");
				}
			}

			skin.m_joints.reserve(cskin.joints_count);
			for (size_t j = 0; j < cskin.joints_count; ++j)
				skin.m_joints.emplace_back(find_safe_index(node_to_index, cskin.joints[j]));

			skin.m_joint_matrices.resize(cskin.joints_count);

			skin_to_index.emplace(&cskin, i);
			this->m_skins.emplace_back(std::move(skin));
		}

		// Read the scene
		// Flattens all scenes into a node tree
		for (size_t i = 0; i < data->scenes_count; ++i)
		{
			const cgltf_scene &scene = data->scenes[i];

			assert(scene.nodes_count >= 1 && "Scene must provide at least one root node");
			std::stack<std::pair<cgltf_node *, int32_t>> node_stack;        // Node stack of node and parent

			for (size_t j = 0; j < scene.nodes_count; ++j)
				node_stack.push(std::make_pair(scene.nodes[j], -1));        // Insert all root nodes

			while (!node_stack.empty())
			{
				auto node_index = node_stack.top();
				node_stack.pop();

				cgltf_node *cnode = node_index.first;

				auto      ni             = node_to_index[cnode];
				Node     &node           = this->m_nodes[static_cast<size_t>(ni)];                  // Casting from int to size_t, confident index will not be negative here
				NodeData &node_side_data = this->m_nodes_side_data[static_cast<size_t>(ni)];        // Casting from int to size_t, confident index will not be negative here

				node.m_mesh_index = find_safe_index(mesh_to_index, cnode->mesh);
				node.m_skin_index = find_safe_index(skin_to_index, cnode->skin);
				node.m_parent     = find_safe_index(node_to_index, cnode->parent);

				if (cnode->name)
					node_side_data.m_name = cnode->name;

				if (node.m_skin_index != -1)
					this->m_skins[static_cast<size_t>(node.m_skin_index)].m_node_index = node_to_index[cnode];        // Cast ok again, index not negative

				if (cnode->has_translation)
					node.m_trs_transform.m_translation = ror::Vector3f(cnode->translation[0], cnode->translation[1], cnode->translation[2]);

				if (cnode->has_rotation)
					node.m_trs_transform.m_rotation = ror::Quaternionf(cnode->rotation[0], cnode->rotation[1], cnode->rotation[2], cnode->rotation[3]);

				if (cnode->has_scale)
					node.m_trs_transform.m_scale = ror::Vector3f(cnode->scale[0], cnode->scale[1], cnode->scale[2]);

				if (cnode->has_matrix)
					cgltf_node_transform_local(cnode, node.m_transformation.m_values);

				for (size_t k = 0; k < cnode->children_count; ++k)
				{
					assert(node_to_index.find(cnode->children[k]) != node_to_index.end() && "Children not found in the inserted nodes");

					node_stack.push(std::make_pair(cnode->children[k], node_to_index[cnode]));

					uint32_t child_index = static_cast<uint32_t>(node_to_index[cnode->children[k]]);

					node_side_data.m_children.push_back(child_index);
				}
			}
		}

		// Read the animations
		this->m_animations.reserve(data->animations_count);
		for (size_t i = 0; i < data->animations_count; ++i)
		{
			const cgltf_animation &canimation = data->animations[i];

			assert(canimation.channels_count > 0 && "No animation channels availabled in the animation");
			assert(canimation.samplers_count > 0 && "No animation samplers availabled in the animation");

			Animation animation;

			std::unordered_map<const cgltf_animation_sampler *, int32_t> animation_sampler_to_index{};
			animation_sampler_to_index.reserve(canimation.samplers_count);

			// Read the samplers first
			animation.m_samplers.reserve(canimation.samplers_count);
			for (size_t j = 0; j < canimation.samplers_count; ++j)
			{
				Animation::AnimationSampler animation_sampler;

				{
					const auto *anim_sampler_accessor = canimation.samplers[j].input;
					if constexpr (get_build() == BuildType::build_debug)
					{
						auto fmt   = get_format_from_gltf_type_format(anim_sampler_accessor->type, anim_sampler_accessor->component_type);
						auto bytes = rhi::format_to_bytes(fmt);
						assert(bytes == 4 && "Animation sampler input value is not floats and 4 bytes");
					}
					auto attrib_byte_size = cgltf_calc_size(anim_sampler_accessor->type, anim_sampler_accessor->component_type);
					animation_sampler.m_input.resize(anim_sampler_accessor->count * attrib_byte_size / sizeof(float32_t));        // This will be 4/4 which is fine

					cgltf_accessor_unpack_floats(anim_sampler_accessor,
												 reinterpret_cast<cgltf_float *>(animation_sampler.m_input.data()),
												 animation_sampler.m_input.size());

					assert(anim_sampler_accessor->buffer_view->stride == attrib_byte_size && "Looks like sampler input data is interleaved, not supported");
				}

				{
					const auto *anim_sampler_accessor = canimation.samplers[j].output;
					if constexpr (get_build() == BuildType::build_debug)
					{
						auto fmt   = get_format_from_gltf_type_format(anim_sampler_accessor->type, anim_sampler_accessor->component_type);
						auto bytes = rhi::format_to_bytes(fmt);
						assert(bytes == 16 || bytes == 12 && "Animation sampler output values are not floats 16 or float 12");
					}

					auto attrib_byte_size = cgltf_calc_size(anim_sampler_accessor->type, anim_sampler_accessor->component_type);
					animation_sampler.m_output.resize(anim_sampler_accessor->count * attrib_byte_size / sizeof(float32_t));

					cgltf_accessor_unpack_floats(anim_sampler_accessor,
												 reinterpret_cast<cgltf_float *>(animation_sampler.m_output.data()),
												 animation_sampler.m_output.size());        // TODO: Why was I doing this before * anim_sampler_accessor->count);

					assert(anim_sampler_accessor->buffer_view->stride == attrib_byte_size && "Looks like sampler output data is interleaved, not supported");
				}

				switch (canimation.samplers[j].interpolation)
				{
					case cgltf_interpolation_type_linear:
						animation_sampler.m_interpolation = AnimationInterpolation::linear;
						break;
					case cgltf_interpolation_type_step:
						animation_sampler.m_interpolation = AnimationInterpolation::step;
						break;
					case cgltf_interpolation_type_cubic_spline:
						animation_sampler.m_interpolation = AnimationInterpolation::cubicspline;
						break;
				}

				animation_sampler_to_index.emplace(&canimation.samplers[j], j);
				animation.m_samplers.emplace_back(std::move(animation_sampler));
			}

			// Now read the channels second
			animation.m_channels.reserve(canimation.channels_count);
			for (size_t j = 0; j < canimation.channels_count; ++j)
			{
				const auto &cchannel = canimation.channels[j];

				Animation::AnimationChannel animation_channel;

				auto target_sampler_index = find_safe_index(animation_sampler_to_index, cchannel.sampler);
				auto target_node_index    = find_safe_index(node_to_index, cchannel.target_node);

				assert(target_node_index >= 0 && "An animation channel must target a sampler, but I can't find the node");
				assert(target_sampler_index >= 0 && "An animation channel must target a node, but I can't find the node");

				animation_channel.m_sampler_index     = static_cast<uint32_t>(target_sampler_index);
				animation_channel.m_target_node_index = static_cast<uint32_t>(target_node_index);

				switch (cchannel.target_path)
				{
					case cgltf_animation_path_type_translation:
						animation_channel.m_target_node_path = AnimationTarget::translation;
						break;
					case cgltf_animation_path_type_rotation:
						animation_channel.m_target_node_path = AnimationTarget::rotation;
						break;
					case cgltf_animation_path_type_scale:
						animation_channel.m_target_node_path = AnimationTarget::scale;
						break;
					case cgltf_animation_path_type_weights:
						animation_channel.m_target_node_path = AnimationTarget::weight;
						break;
					case cgltf_animation_path_type_invalid:
						assert(0 && "Can't deal with invalid interpolation");
				}

				animation.m_channels.emplace_back(std::move(animation_channel));
			}

			this->m_animations.emplace_back(animation);
		}

		// Place where I wait for all JobSystem jobs to finish
		// All the images
		this->m_images.reserve(data->images_count);
		for (size_t i = 0; i < data->images_count; ++i)
		{
			this->m_images.emplace_back(future_texures[i].data());
		}

		cgltf_free(data);
	}
	else
	{
		ror::log_critical("Can't load gltf file {}", filename.c_str());
	}
}

}        // namespace ror
