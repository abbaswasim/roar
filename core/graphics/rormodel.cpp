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
#include "foundation/rorutilities.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rornode.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorbuffer_view.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"
#include <cassert>
#include <cstddef>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <stack>

#define CGLTF_IMPLEMENTATION
#include "cgltf/cgltf.h"

#define USE_JS 1

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
	uint8_t     *data = new uint8_t[data_size];                    // Data that needs to be allocated for decoding
	cgltf_result res  = cgltf_load_buffer_base64(a_options, data_size, data_start + 1, reinterpret_cast<void **>(&data));
	assert(res == cgltf_result_success && "Base64 decoding failed for image");

	int32_t w = 0, h = 0, bpp = 0;
	auto   *new_data = stbi_load_from_memory(data, static_cast_safe<int32_t>(data_size), &w, &h, &bpp, 0);        // Final argument = 0 means get real bpp

	ti.push_empty_mip();
	ti.format(rhi::PixelFormat::r8g8b8a8_uint32_norm_srgb);        // TODO: How do I read this via STB or gltf?
	ti.reset(new_data, static_cast<uint64_t>(w * h * bpp));        // ti now owns the new_data pointer returned by stbi
	ti.width(static_cast<uint32_t>(w));
	ti.height(static_cast<uint32_t>(h));
	ti.depth(static_cast<uint32_t>(bpp));

	// Delete data pointer
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
// The binary file for this glTF is loaded via a resource instead of straight from disk
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

rhi::TextureSampler cgltf_sampler_to_sampler(const cgltf_sampler *a_sampler)
{
	// gltf Spec values for reference
	// "enum" :           [ 9728,      9729,     9984,                     9985,                    9986,                    9987],
	// "gltf_enumNames" : ["NEAREST", "LINEAR", "NEAREST_MIPMAP_NEAREST", "LINEAR_MIPMAP_NEAREST", "NEAREST_MIPMAP_LINEAR", "LINEAR_MIPMAP_LINEAR"],

	// "enum" : [33071, 33648, 10497],
	// "gltf_enumNames" : ["CLAMP_TO_EDGE", "MIRRORED_REPEAT", "REPEAT"],

	rhi::TextureSampler sampler;

	sampler.m_mag_filter = static_cast<rhi::TextureFilter>(a_sampler->mag_filter - 9728);                                  // 9728 is NEAREST GLTF GL value for Mag filter (can only be NEAREST and LINEAR)
	sampler.m_min_filter = a_sampler->min_filter % 2 ? rhi::TextureFilter::linear : rhi::TextureFilter::nearest;           // All NEAREST min filters are even, it can be all of the above
	sampler.m_mip_mode   = a_sampler->min_filter > 9985 ? rhi::TextureFilter::linear : rhi::TextureFilter::nearest;        // Only values above 9985 are mipmapped linear
	if (a_sampler->wrap_s != 10497)
		sampler.m_wrap_s = a_sampler->wrap_s == 33648 ? rhi::TextureAddressMode::mirrored_repeat : rhi::TextureAddressMode::clamp_to_edge;        // Else leave the default repeat
	if (a_sampler->wrap_t != 10497)
		sampler.m_wrap_t = a_sampler->wrap_t == 33648 ? rhi::TextureAddressMode::mirrored_repeat : rhi::TextureAddressMode::clamp_to_edge;        // Else leave the default repeat

	return sampler;
}

rhi::PrimitiveTopology cglf_primitive_to_primitive_topology(cgltf_primitive_type a_type)
{
	// clang-format off
	switch (a_type)
	{
		case cgltf_primitive_type_points:            return rhi::PrimitiveTopology::points;
		case cgltf_primitive_type_lines:             return rhi::PrimitiveTopology::lines;
		case cgltf_primitive_type_line_loop:         return rhi::PrimitiveTopology::lines_loop;
		case cgltf_primitive_type_line_strip:        return rhi::PrimitiveTopology::lines_strip;
		case cgltf_primitive_type_triangles:         return rhi::PrimitiveTopology::triangles;
		case cgltf_primitive_type_triangle_strip:    return rhi::PrimitiveTopology::triangles_strip;
		case cgltf_primitive_type_triangle_fan:      return rhi::PrimitiveTopology::triangles_fan;
	}
	// clang-format on

	return rhi::PrimitiveTopology::triangles;
}

void Model::load_from_gltf_file(std::filesystem::path a_filename)
{
	// Lets start by reading a_filename via resource cache
	auto &resource = load_resource(a_filename, ResourceSemantic::models);
	auto &filename = resource.absolute_path();

	// Get an instance of job system
#if defined(USE_JS)
	auto &js = ror::get_job_system();
#endif
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

		// TODO: This loading is very serial, turning this into Jobs doesn't help much. Find a way to be more parallel

		auto buffers_load_lambda = [&options, &data, &filename, this, &buffer_to_index]() -> bool {
			// Load all the buffers as resource(s)
			cgltf_result buffers_result = cgltf_load_buffers_as_resource(&options, data, filename.c_str());

			if (buffers_result != cgltf_result_success)
			{
				ror::log_critical("Can't load gltf binary data");
				return false;
			}

			assert(data->buffers_count > 0 && "No buffers available in the gltf file, its probably ok though. verify!");
			this->m_buffers.reserve(data->buffers_count);
			for (size_t i = 0; i < data->buffers_count; ++i)
			{
				// In the following I am creating std::vector<uint8_t> buffer from data->buffers[i] and emplacing it directly into m_buffers
				// Note we don't need a reserve here because the vector size is provided in constructor to emplace_back
				this->m_buffers.emplace_back(static_cast<BinBuffer *>(data->buffers[i].data), static_cast<BinBuffer *>(data->buffers[i].data) + data->buffers[i].size);
				buffer_to_index.emplace(&data->buffers[i], i);
			}

			return true;
		};

#if defined(USE_JS)
		auto buffers_load_handle = js.push_job(buffers_load_lambda);
#else
		buffers_load_lambda();
#endif
		auto from_file_lambda = [](std::filesystem::path &a_texture_path) -> rhi::TextureImage {
			return rhi::read_texture_2d_from_file(a_texture_path);
		};

		auto from_base64_lambda = [&options](const char *a_uri, const char *a_mimetype) -> rhi::TextureImage {
			return ror::read_texture_from_cgltf_base64(&options, a_uri, a_mimetype);
		};

		std::vector<JobHandle<rhi::TextureImage>> future_texures{};
		future_texures.reserve(data->images_count);
		// Create jobs for all the images
		for (size_t i = 0; i < data->images_count; ++i)
		{
			const char *uri = data->images[i].uri;
			assert(uri && "Image URI is null, only support URI based images");        // TODO: Read images from bufferviews
			if (uri_base64_encoded(uri))
			{
#if defined(USE_JS)
				future_texures.emplace_back(js.push_job(from_base64_lambda, uri, data->images[i].mime_type));        // vector of job_handles
#else
				this->m_images.emplace_back(from_base64_lambda(uri, data->images[i].mime_type));
#endif
			}
			else        // Assumes its a path otherwise
			{
				auto texture_path = filename.parent_path() / uri;
#if defined(USE_JS)
				future_texures.emplace_back(js.push_job(from_file_lambda, texture_path));        // vector of job_handles
#else
				this->m_images.emplace_back(from_file_lambda(texture_path));
#endif
			}
			image_to_index.emplace(&data->images[i], i);
		}

		// This job reads all samplers, textures and materials in one job
		auto rest_of_data_load_lambda = [&]() -> bool {
			// Read all the samplers
			this->m_samplers.reserve(data->samplers_count + 1);

			// Lets have a default sampler at index 0
			this->m_samplers.emplace_back();

			for (size_t i = 0; i < data->samplers_count; ++i)
			{
				this->m_samplers.emplace_back(cgltf_sampler_to_sampler(&data->samplers[i]));
				sampler_to_index.emplace(&data->samplers[i], i + 1);        // Index 0 is default hence the +1
			}

			// Read all the textures
			this->m_textures.reserve(data->textures_count);

			for (size_t i = 0; i < data->textures_count; ++i)
			{
				assert(data->textures[i].image && "Texture must have an image");

				rhi::Texture texture;
				texture.m_texture_image = find_safe_index(image_to_index, data->textures[i].image);
				assert(texture.m_texture_image.m_handle != -1 && "Couldn't find the image loaded for the texture");

				auto texture_sampler = -1;

				if (data->textures[i].sampler != nullptr)
					texture_sampler = find_safe_index(sampler_to_index, data->textures[i].sampler);

				// If no samplers provided use default at index 0
				if (texture_sampler == -1)
					texture_sampler = 0;

				texture.m_texture_sampler = static_cast_safe<uint32_t>(texture_sampler);

				this->m_textures.emplace_back(std::move(texture));
				texture_to_index.emplace(&data->textures[i], i);
			}

			// Read all the materials
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

			// Read all the meshes
			this->m_meshes.resize(data->meshes_count);
			for (size_t i = 0; i < data->meshes_count; ++i)
			{
				Mesh             &mesh  = this->m_meshes[i];
				const cgltf_mesh &cmesh = data->meshes[i];

				mesh.m_attribute_vertex_descriptors.resize(cmesh.primitives_count);
				mesh.m_morph_targets_vertex_descriptors.resize(cmesh.primitives_count);
				mesh.m_primitive_types.resize(cmesh.primitives_count);
				mesh.m_has_indices_states.resize(cmesh.primitives_count);
				// The last 2 are important and definitely needs reserving because these are BufferAllocated
				mesh.m_bounding_boxes.resize(cmesh.primitives_count);
				mesh.m_material_indices.resize(cmesh.primitives_count);

				for (size_t j = 0; j < cmesh.primitives_count; ++j)
				{
					const cgltf_primitive &cprim = cmesh.primitives[j];
					auto                  &avd   = mesh.m_attribute_vertex_descriptors[j];
					auto                  &amts  = mesh.m_morph_targets_vertex_descriptors[j];

					std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>> attribs_data;

					assert(cprim.type == cgltf_primitive_type_triangles && "Mesh primitive type is not triangles which is the only supported type at the moment");
					mesh.m_primitive_types[i] = cglf_primitive_to_primitive_topology(cprim.type);

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
						assert(!cprim.indices->is_sparse && "Sparce index buffers not supported");

						auto attrib_accessor   = cprim.indices;
						auto buffer_index      = find_safe_index(buffer_to_index, cprim.indices->buffer_view->buffer);
						auto indices_byte_size = cgltf_calc_size(attrib_accessor->type, attrib_accessor->component_type);
						auto stride            = cprim.indices->buffer_view->stride;

						assert(buffer_index >= 0 && "Not a valid buffer index returned, possibly no buffer associated with this index buffer");
						uint8_t *data_pointer = reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data());

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

						rhi::BufferSemantic current_index = rhi::BufferSemantic::vertex_position;

						switch (attrib.type)
						{
							case cgltf_attribute_type_position:
								assert(attrib.data->has_min && attrib.data->has_max && "Position attributes must provide min and max");
								assert(attrib.index == 0 && "Don't suport more than 1 position");

								current_index = rhi::BufferSemantic::vertex_position;
								mesh.m_bounding_boxes[j].create_from_min_max({attrib.data->min[0], attrib.data->min[1], attrib.data->min[2]},
																			 {attrib.data->max[0], attrib.data->max[1], attrib.data->max[2]});
								break;
							case cgltf_attribute_type_normal:
								assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec3 && "Normal not in the right format, float3 required");
								assert(attrib.index == 0 && "Don't suport more than 1 normal");
								current_index = rhi::BufferSemantic::vertex_normal;
								break;
							case cgltf_attribute_type_tangent:
								assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec4 && "Tangent not in the right format, float3 required");
								assert(attrib.index == 0 && "Don't suport more than 1 tangent");
								current_index = rhi::BufferSemantic::vertex_tangent;
								break;
							case cgltf_attribute_type_texcoord:
								assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec2 && "Texture coordinate not in the right format, float2 required");
								assert(attrib.index < 2 && "Don't support more than 2 texture coordinate sets");
								current_index = static_cast<rhi::BufferSemantic>(ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0) << static_cast<uint64_t>(attrib.index));
								break;
							case cgltf_attribute_type_color:
								// assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec3 && "Color not in the right format, float3 required");
								assert(attrib.index < 2 && "Don't support more than 2 color sets");
								current_index = static_cast<rhi::BufferSemantic>(ror::enum_to_type_cast(rhi::BufferSemantic::vertex_color_0) << static_cast<uint64_t>(attrib.index));
								break;
							case cgltf_attribute_type_joints:
								assert((attrib.data->component_type == cgltf_component_type_r_16u || attrib.data->component_type == cgltf_component_type_r_8u) &&
									   attrib.data->type == cgltf_type_vec4 && "Joints not in the right format, unsigned8/16_4 required");
								assert(attrib.index < 2 && "Don't support more than 2 joint sets");
								current_index = static_cast<rhi::BufferSemantic>(ror::enum_to_type_cast(rhi::BufferSemantic::vertex_bone_id_0) << static_cast<uint64_t>(attrib.index));
								break;
							case cgltf_attribute_type_weights:
								assert((attrib.data->component_type == cgltf_component_type_r_32f || attrib.data->component_type == cgltf_component_type_r_16u || attrib.data->component_type == cgltf_component_type_r_8u) &&
									   attrib.data->type == cgltf_type_vec4 && "Weights not in the right format, unsigned_8/16/float_4 required");
								assert(attrib.index < 2 && "Don't support more than 2 weight sets");
								current_index = static_cast<rhi::BufferSemantic>(ror::enum_to_type_cast(rhi::BufferSemantic::vertex_weight_0) << static_cast<uint64_t>(attrib.index));
								break;
							case cgltf_attribute_type_invalid:
								assert(0 && "rhi::BufferView not valid yet");
								break;
						}

						// TODO: GL expects stride to be zero if data is tightly packed

						const auto *attrib_accessor  = attrib.data;
						auto        attrib_format    = get_format_from_gltf_type_format(attrib_accessor->type, attrib_accessor->component_type);
						auto        buffer_index     = find_safe_index(buffer_to_index, attrib_accessor->buffer_view->buffer);
						auto        attrib_byte_size = cgltf_calc_size(attrib_accessor->type, attrib_accessor->component_type);
						auto        stride           = attrib_accessor->buffer_view->stride;
						auto        offset           = attrib_accessor->buffer_view->offset + attrib_accessor->offset;

						assert(buffer_index >= 0 && "Not a valid buffer index returned, possibly no buffer associated with this attribute");
						uint8_t *data_pointer = reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data());

						std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer + offset, attrib_accessor->count * attrib_byte_size, stride};
						attribs_data.emplace(current_index, std::move(data_tuple));

						avd.add(current_index, attrib_format, &bp);
					}

					// Now upload data from all the attributes into avd
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
							rhi::BufferSemantic current_index = rhi::BufferSemantic::vertex_position;

							switch (attrib.type)
							{
								case cgltf_attribute_type_position:
									assert(attrib.data->has_min && attrib.data->has_max && "Position attributes must provide min and max");

									current_index = rhi::BufferSemantic::vertex_position;
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
									current_index = rhi::BufferSemantic::vertex_normal;
									break;
								case cgltf_attribute_type_tangent:
									// assert(attrib.data->component_type == cgltf_component_type_r_32f && attrib.data->type == cgltf_type_vec4 && "Tangent not in the right format, float3 required");
									current_index = rhi::BufferSemantic::vertex_tangent;
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

							const auto *attrib_accessor  = attrib.data;
							auto        attrib_format    = get_format_from_gltf_type_format(attrib_accessor->type, attrib_accessor->component_type);
							auto        buffer_index     = find_safe_index(buffer_to_index, attrib_accessor->buffer_view->buffer);
							auto        attrib_byte_size = cgltf_calc_size(attrib_accessor->type, attrib_accessor->component_type);
							auto        stride           = attrib_accessor->buffer_view->stride;
							auto        offset           = attrib_accessor->buffer_view->offset + attrib_accessor->offset;

							assert(buffer_index >= 0 && "Not a valid buffer index returned, possibly no buffer associated with this morph target attribute");
							uint8_t *data_pointer = reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data());

							std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer + offset, attrib_accessor->count * attrib_byte_size, stride};
							attribs_data.emplace(current_index, std::move(data_tuple));

							amts[k].add(current_index, attrib_format, &bp);
						}

						// Now copy all the buffer_views for morph_targets
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
			this->m_nodes_side_data.resize(data->nodes_count);        // Will fill the empty Node side data just created later
			for (size_t i = 0; i < data->nodes_count; ++i)
			{
				const cgltf_node &cnode = data->nodes[i];
				node_to_index.emplace(&cnode, i);
			}

			// Read the skins
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
					// TODO: Do this unpacking manually, there is a lot of overhead of doing it this way
					cgltf_accessor_unpack_floats(inverse_bind_matrices_accessor,
												 reinterpret_cast<cgltf_float *>(skin.m_inverse_bind_matrices.data()),
												 skin.m_inverse_bind_matrices.size() * 16);

					auto attrib_byte_size = cgltf_calc_size(inverse_bind_matrices_accessor->type, inverse_bind_matrices_accessor->component_type);

					assert(inverse_bind_matrices_accessor->stride == attrib_byte_size && "Looks like inverse_bind_matrices data is interleaved, not supported");

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

			// Read the scene and flattens all scenes into a node tree
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
			this->m_animations.resize(data->animations_count);
			for (size_t i = 0; i < data->animations_count; ++i)
			{
				const cgltf_animation &canimation = data->animations[i];

				assert(canimation.channels_count > 0 && "No animation channels availabled in the animation");
				assert(canimation.samplers_count > 0 && "No animation samplers availabled in the animation");

				Animation &animation = this->m_animations[i];

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
							assert((bytes == 16 || bytes == 12) && "Animation sampler output values are not floats 16 or float 12");
						}

						auto attrib_byte_size = cgltf_calc_size(anim_sampler_accessor->type, anim_sampler_accessor->component_type);
						animation_sampler.m_output.resize(anim_sampler_accessor->count * attrib_byte_size / sizeof(float32_t));

						// TODO: Do this unpacking manually, there is a lot of overhead of doing it this way
						cgltf_accessor_unpack_floats(anim_sampler_accessor,
													 reinterpret_cast<cgltf_float *>(animation_sampler.m_output.data()),
													 animation_sampler.m_output.size());

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
			}
			return true;
		};

#if defined(USE_JS)
		auto rest_of_data_load_handle = js.push_job(rest_of_data_load_lambda, buffers_load_handle.job());
#else
		rest_of_data_load_lambda();
#endif

#if defined(USE_JS)
		// Wait for all the images to load
		this->m_images.reserve(data->images_count);
		for (size_t i = 0; i < data->images_count; ++i)
			this->m_images.emplace_back(future_texures[i].data());

		// Wait for all the data to have loaded to load
		auto data_load_success = rest_of_data_load_handle.data();
		if (!data_load_success)
		{
			ror::log_critical("Loading glTF data failed, this is highly unlikely so IO error must have happened {}", filename.c_str());
		}

#endif

		cgltf_free(data);
	}
	else
	{
		ror::log_critical("Can't load gltf file {}", filename.c_str());
	}
}

}        // namespace ror
