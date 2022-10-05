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

#include "foundation/rorhash.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rornode.hpp"
#include "math/rormatrix.hpp"
#include "math/rorvector.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorbuffer_view.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rorhandles.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"
#include <cassert>
#include <cstddef>
#include <cstring>
#include <filesystem>
#include <stack>
#include <unordered_map>
#include <vector>

#define CGLTF_IMPLEMENTATION
#include "cgltf/cgltf.h"

#include "stb/stb_image.h"

#define USE_JS

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
	else if (std::strcmp(a_mimetype, "image/ktx2") == 0)
		return ResourceExtension::texture_ktx2;
	else if (std::strcmp(a_mimetype, "image/jpg") == 0)
		return ResourceExtension::texture_jpeg;
	else if (std::strcmp(a_mimetype, "image/jpeg") == 0)
		return ResourceExtension::texture_jpeg;

	assert(0 && "Add other mimetypes");
	return ResourceExtension::unknown;
}

size_t base64_decoded_size(const char *a_base64)
{
	size_t data_length = strlen(a_base64);
	assert(data_length > 0 && "base64 size calculation failed, nothing in base64 to read");
	size_t padded_size = 0;
	if (a_base64[data_length - 2] == '=')
		padded_size = 2;
	else if (a_base64[data_length - 1] == '=')
		padded_size = 1;

	// From https://blog.aaronlenoir.com/2017/11/10/get-original-length-from-base-64-string/
	return ((data_length / 4) * 3) - padded_size;        // Size calculated from the fact that base64 each 3 bytes turns into 4 bytes
}

rhi::TextureImage read_texture_from_cgltf_base64(const cgltf_options *a_options, const char *a_uri, const char *a_mimetype)
{
	const char *data_start = strchr(a_uri, ',');
	assert(data_start && "Can't find start of base64 image data");
	data_start++;

	ResourceExtension extension{ResourceExtension::unknown};

	if (a_mimetype)
	{
		extension = extension_from_mimetype(a_mimetype);
	}
	else
	{
		const char *colon      = strchr(a_uri, ':');
		const char *semi_colon = strchr(a_uri, ';');
		assert(colon && semi_colon && "Can't parse base64 uri for mimetype");
		const auto  len = static_cast<uint32_t>(semi_colon - colon - 1);
		std::string mimetype(a_uri, 5, len);        // "data:image/png;base64,AAA" Thats how it should be otherwise we don't have a valid base64 image mimetype
		extension = extension_from_mimetype(mimetype.c_str());
	}

	(void) extension;
	assert(extension != ResourceExtension::unknown && "Couldn't find extension from mimetype");

	rhi::TextureImage ti;

	size_t   data_size = base64_decoded_size(data_start);
	uint8_t *data      = new uint8_t[data_size];        // Data that needs to be allocated for decoding

	cgltf_result res = cgltf_load_buffer_base64(a_options, data_size, data_start, reinterpret_cast<void **>(&data));
	assert(res == cgltf_result_success && "Base64 decoding failed for image");
	(void) res;

	rhi::read_texture_from_memory(data, data_size, ti, "cgltf_base64_image");

	// Delete data pointer
	delete[] data;

	return ti;
}

rhi::TextureImage read_texture_from_cgltf_buffer_view(const cgltf_buffer_view *a_buffer_view, const char *a_mimetype)
{
	// unsigned char *data = static_cast<unsigned char *>(a_buffer_view->data);
	const uint8_t *data = cgltf_buffer_view_data(a_buffer_view);

	assert(data && "Can't find valid data inside image buffer_view");

	ResourceExtension extension{extension_from_mimetype(a_mimetype)};
	assert(extension != ResourceExtension::unknown && "Couldn't find extension from mimetype");
	assert((extension == ResourceExtension::texture_png || extension == ResourceExtension::texture_jpeg) && "Unsupported extension in buffer view");
	(void) extension;

	rhi::TextureImage ti;

	// TODO: Abstract this out into rortexture.cpp
	cgltf_size data_size{a_buffer_view->size};
	rhi::read_texture_from_memory(data, data_size, ti, (a_buffer_view->name != nullptr) ? a_buffer_view->name : "cgltf_buffer_view_image");

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
	auto &data     = resource.data();

	assert(data.size() == size && "Resource loaded of wrong size");
	(void) size;

	memory_free(options->memory.user_data, path);

	// Data pointer from the Resource is aliased into out_data within the gltf file
	// This is ok, because the deleter for this type in cgltf is void so won't be deleted under Resource's feet
	*out_data = const_cast<void *>(reinterpret_cast<const void *>(data.data()));        // This is gross but I don't want to create resource::data() non-const, small hack because of TP lib interface

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
		case cgltf_component_type::cgltf_component_type_r_32f:		   return (a_type == cgltf_type::cgltf_type_mat2 ? rhi::VertexFormat::float32_2x2 : rhi::VertexFormat::float32_4);
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
			return rhi::VertexFormat::float32_4x4;
		else
			assert(0 && "This type of mat4 is not supported");
	}
	else if (a_type == cgltf_type::cgltf_type_mat3)
	{
		if (a_component_type == cgltf_component_type::cgltf_component_type_r_32f)
			return rhi::VertexFormat::float32_3x3;
		else
			assert(0 && "This type of mat4 is not supported");
	}

	return rhi::VertexFormat::float32_4;
}

rhi::VertexFormat int_format_to_int32_format_bit(rhi::VertexFormat a_input)
{
	switch (a_input)
	{
		case rhi::VertexFormat::bool32_1:
		case rhi::VertexFormat::int8_1:
		case rhi::VertexFormat::int16_1:
		case rhi::VertexFormat::int32_1:
		case rhi::VertexFormat::half16_1:
			return rhi::VertexFormat::int32_1;
		case rhi::VertexFormat::uint8_1:
		case rhi::VertexFormat::uint16_1:
		case rhi::VertexFormat::uint32_1:
			return rhi::VertexFormat::uint32_1;

		case rhi::VertexFormat::bool32_2:
		case rhi::VertexFormat::int8_2:
		case rhi::VertexFormat::int16_2:
		case rhi::VertexFormat::int32_2:
		case rhi::VertexFormat::half16_2:
			return rhi::VertexFormat::int32_2;
		case rhi::VertexFormat::uint8_2:
		case rhi::VertexFormat::uint16_2:
		case rhi::VertexFormat::uint32_2:
			return rhi::VertexFormat::uint32_2;

		case rhi::VertexFormat::bool32_3:
		case rhi::VertexFormat::int8_3:
		case rhi::VertexFormat::int16_3:
		case rhi::VertexFormat::int32_3:
		case rhi::VertexFormat::half16_3:
			return rhi::VertexFormat::int32_3;
		case rhi::VertexFormat::uint8_3:
		case rhi::VertexFormat::uint16_3:
		case rhi::VertexFormat::uint32_3:
			return rhi::VertexFormat::uint32_3;

		case rhi::VertexFormat::bool32_4:
		case rhi::VertexFormat::int8_4:
		case rhi::VertexFormat::int16_4:
		case rhi::VertexFormat::int32_4:
		case rhi::VertexFormat::half16_4:
			return rhi::VertexFormat::int32_4;
		case rhi::VertexFormat::uint8_4:
		case rhi::VertexFormat::uint16_4:
		case rhi::VertexFormat::uint32_4:
			return rhi::VertexFormat::uint32_4;

		case rhi::VertexFormat::invalid:
		case rhi::VertexFormat::struct_1:
		case rhi::VertexFormat::float32_1:
		case rhi::VertexFormat::float32_2:
		case rhi::VertexFormat::float32_3:
		case rhi::VertexFormat::float32_4:
		case rhi::VertexFormat::float32_2x2:
		case rhi::VertexFormat::float32_2x3:
		case rhi::VertexFormat::float32_2x4:
		case rhi::VertexFormat::float32_3x2:
		case rhi::VertexFormat::float32_3x3:
		case rhi::VertexFormat::float32_3x4:
		case rhi::VertexFormat::float32_4x2:
		case rhi::VertexFormat::float32_4x3:
		case rhi::VertexFormat::float32_4x4:
		case rhi::VertexFormat::float64_2x2:
		case rhi::VertexFormat::float64_2x3:
		case rhi::VertexFormat::float64_2x4:
		case rhi::VertexFormat::float64_3x2:
		case rhi::VertexFormat::float64_3x3:
		case rhi::VertexFormat::float64_3x4:
		case rhi::VertexFormat::float64_4x2:
		case rhi::VertexFormat::float64_4x3:
		case rhi::VertexFormat::float64_4x4:
		case rhi::VertexFormat::float64_1:
		case rhi::VertexFormat::float64_2:
		case rhi::VertexFormat::float64_3:
		case rhi::VertexFormat::float64_4:
		case rhi::VertexFormat::uint64_1:
		case rhi::VertexFormat::uint64_2:
		case rhi::VertexFormat::uint64_3:
		case rhi::VertexFormat::uint64_4:
		case rhi::VertexFormat::int8_1_norm:
		case rhi::VertexFormat::int8_2_norm:
		case rhi::VertexFormat::int8_3_norm:
		case rhi::VertexFormat::int8_4_norm:
		case rhi::VertexFormat::int16_1_norm:
		case rhi::VertexFormat::int16_2_norm:
		case rhi::VertexFormat::int16_3_norm:
		case rhi::VertexFormat::int16_4_norm:
		case rhi::VertexFormat::uint8_1_norm:
		case rhi::VertexFormat::uint8_2_norm:
		case rhi::VertexFormat::uint8_3_norm:
		case rhi::VertexFormat::uint8_4_norm:
		case rhi::VertexFormat::uint16_1_norm:
		case rhi::VertexFormat::uint16_2_norm:
		case rhi::VertexFormat::uint16_3_norm:
		case rhi::VertexFormat::uint16_4_norm:
		case rhi::VertexFormat::int1010102_norm:
		case rhi::VertexFormat::uint1010102_norm:
		case rhi::VertexFormat::int101111_norm:
		case rhi::VertexFormat::uint101111_norm:
		case rhi::VertexFormat::uint8_4_norm_bgra:
		case rhi::VertexFormat::uint8_custom:
		case rhi::VertexFormat::uint16_custom:
		case rhi::VertexFormat::uint32_custom:
		case rhi::VertexFormat::float32_custom:
			return a_input;
	}

	return rhi::VertexFormat::int32_1;
}

rhi::TextureSampler cgltf_sampler_to_sampler(const cgltf_sampler *a_sampler)
{
	// gltf Spec values for reference
	// "enum" :           [ 9728,      9729,     9984,                     9985,                    9986,                    9987],
	// "gltf_enumNames" : ["NEAREST", "LINEAR", "NEAREST_MIPMAP_NEAREST", "LINEAR_MIPMAP_NEAREST", "NEAREST_MIPMAP_LINEAR", "LINEAR_MIPMAP_LINEAR"],

	// "enum" :           [  33071,           33648,            10497],
	// "gltf_enumNames" : ["CLAMP_TO_EDGE", "MIRRORED_REPEAT", "REPEAT"],
#define NEAREST 9728
#define LINEAR 9729
#define NEAREST_MIPMAP_NEAREST 9984
#define LINEAR_MIPMAP_NEAREST 9985
#define NEAREST_MIPMAP_LINEAR 9986
#define LINEAR_MIPMAP_LINEAR 9987

#define CLAMP_TO_EDGE 33071
#define MIRRORED_REPEAT 33648
#define REPEAT 10497

	rhi::TextureSampler sampler;

	if (a_sampler->mag_filter == NEAREST)
		sampler.mag_filter(rhi::TextureFilter::nearest);
	else if (a_sampler->mag_filter == LINEAR)
		sampler.mag_filter(rhi::TextureFilter::linear);

	if (a_sampler->min_filter == NEAREST ||
	    a_sampler->min_filter == NEAREST_MIPMAP_LINEAR ||
	    a_sampler->min_filter == NEAREST_MIPMAP_LINEAR)
		sampler.min_filter(rhi::TextureFilter::nearest);
	else if (a_sampler->min_filter == LINEAR ||
	         a_sampler->min_filter == LINEAR_MIPMAP_LINEAR ||
	         a_sampler->min_filter == LINEAR_MIPMAP_LINEAR)
		sampler.min_filter(rhi::TextureFilter::linear);

	if (a_sampler->min_filter == NEAREST_MIPMAP_NEAREST ||
	    a_sampler->min_filter == LINEAR_MIPMAP_NEAREST)
		sampler.mip_mode(rhi::TextureMipFilter::nearest);
	else if (a_sampler->min_filter == NEAREST_MIPMAP_LINEAR ||
	         a_sampler->min_filter == LINEAR_MIPMAP_LINEAR)
		sampler.mip_mode(rhi::TextureMipFilter::linear);

	if (a_sampler->wrap_s == CLAMP_TO_EDGE)
		sampler.wrap_s(rhi::TextureAddressMode::clamp_to_edge);
	else if (a_sampler->wrap_s == MIRRORED_REPEAT)
		sampler.wrap_s(rhi::TextureAddressMode::mirror_repeat);
	else if (a_sampler->wrap_s == REPEAT)
		sampler.wrap_s(rhi::TextureAddressMode::repeat);

	if (a_sampler->wrap_t == CLAMP_TO_EDGE)
		sampler.wrap_t(rhi::TextureAddressMode::clamp_to_edge);
	else if (a_sampler->wrap_t == MIRRORED_REPEAT)
		sampler.wrap_t(rhi::TextureAddressMode::mirror_repeat);
	else if (a_sampler->wrap_t == REPEAT)
		sampler.wrap_t(rhi::TextureAddressMode::repeat);

	return sampler;

#undef NEAREST
#undef LINEAR
#undef NEAREST_MIPMAP_NEAREST
#undef LINEAR_MIPMAP_NEAREST
#undef NEAREST_MIPMAP_LINEAR
#undef LINEAR_MIPMAP_LINEAR

#undef CLAMP_TO_EDGE
#undef MIRRORED_REPEAT
#undef REPEAT
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

void read_material_factor(ror::Color4f &a_factor, const float *a_input)
{
	a_factor.x = a_input[0];
	a_factor.y = a_input[1];
	a_factor.z = a_input[2];
	a_factor.w = a_input[3];
}

void read_material_factor(ror::Color3f &a_factor, const float *a_input)
{
	a_factor.x = a_input[0];
	a_factor.y = a_input[1];
	a_factor.z = a_input[2];
}

void read_material_factor(ror::Color2f &a_factor, const float *a_input)
{
	a_factor.x = a_input[0];
	a_factor.y = a_input[1];
}

void read_material_factor(float32_t &a_factor, const float *a_input)
{
	a_factor = *a_input;
}

template <typename _factor_type>
void read_material_component(ror::Material::Component<_factor_type>                   &a_component,
                             const std::unordered_map<const cgltf_texture *, int32_t> &a_texture_to_index,
                             const cgltf_texture_view                                 &a_texture_view,
                             const float                                              *a_factor)
{
	const auto convert_texture_transform_to_mat3 = [](const cgltf_texture_transform &a_transform) {
		ror::Matrix3f translation = ror::Matrix3f(1.0f, 0.0f, 0.0f,
		                                          0.0f, 1.0f, 0.0f,
		                                          a_transform.offset[0], a_transform.offset[1], 1.0f);
		ror::Matrix3f rotation    = ror::Matrix3f(cos(a_transform.rotation), sin(a_transform.rotation), 0.0f,
		                                          -sin(a_transform.rotation), cos(a_transform.rotation), 0.0f,
		                                          0.0f, 0.0f, 1.0f);
		ror::Matrix3f scale       = ror::Matrix3f(a_transform.scale[0], 0.0f, 0.0f,
		                                          0.0f, a_transform.scale[1], 0.0f,
		                                          0.0f, 0.0f, 1.0f);
		return translation * rotation * scale;
	};

	read_material_factor(a_component.m_factor, a_factor);
	a_component.m_texture       = rhi::TextureHandle(find_safe_index(a_texture_to_index, a_texture_view.texture));
	a_component.m_uv_map        = static_cast<uint32_t>(a_texture_view.texcoord);
	a_component.m_has_transform = a_texture_view.has_transform;

	if (a_component.m_has_transform)
	{
		a_component.m_transform = convert_texture_transform_to_mat3(a_texture_view.transform);
		if (a_texture_view.transform.has_texcoord)
			a_component.m_uv_map = static_cast<uint32_t>(a_texture_view.transform.texcoord);
	}

	if (a_component.m_texture != -1)
		a_component.m_type = Material::ComponentType::factor_texture;
	else
		a_component.m_type = Material::ComponentType::factor;
}

std::string cgltf_result_to_string(cgltf_result a_result)
{
	// clang-format off
    switch (a_result)
    {
        case cgltf_result_success:          return "cgltf_result_success";
        case cgltf_result_data_too_short:   return "cgltf_result_data_too_short";
        case cgltf_result_unknown_format:   return "cgltf_result_unknown_format";
        case cgltf_result_invalid_json:     return "cgltf_result_invalid_json";
        case cgltf_result_invalid_gltf:     return "cgltf_result_invalid_gltf";
        case cgltf_result_invalid_options:  return "cgltf_result_invalid_options";
        case cgltf_result_file_not_found:   return "cgltf_result_file_not_found";
        case cgltf_result_io_error:         return "cgltf_result_io_error";
        case cgltf_result_out_of_memory:    return "cgltf_result_out_of_memory";
        case cgltf_result_legacy_gltf:      return "cgltf_result_legacy_gltf";
    }
	// clang-format on

	return "cgltf_result_success";
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
	cgltf_result result = cgltf_parse(&options, resource.data().data(), resource.data().size(), &data);

	if (result != cgltf_result_success)
	{
		ror::log_critical("glTF file {} load failed with error {}", a_filename.c_str(), cgltf_result_to_string(result).c_str());
		return;
	}
	else
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
		auto from_file_lambda = [](std::filesystem::path a_texture_path) -> rhi::TextureImage {
			return rhi::read_texture_2d_from_file(a_texture_path);
		};

		auto from_base64_lambda = [&options](const char *a_uri, const char *a_mimetype) -> rhi::TextureImage {
			return ror::read_texture_from_cgltf_base64(&options, a_uri, a_mimetype);
		};

		auto from_buffer_view_lambda = [](const cgltf_buffer_view *a_buffer_view, const char *a_mimetype) -> rhi::TextureImage {
			return ror::read_texture_from_cgltf_buffer_view(a_buffer_view, a_mimetype);
		};

#if defined(USE_JS)
		std::vector<JobHandle<rhi::TextureImage>> future_texures{};
		future_texures.reserve(data->images_count);
#endif
		// Create jobs for all the images
		this->m_images.reserve(data->images_count);
		for (size_t i = 0; i < data->images_count; ++i)
		{
			const char *uri = data->images[i].uri;

			if (uri)
			{
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
					std::string texture_path = filename.parent_path() / uri;
					size_t      path_size    = texture_path.size();
					char       *decoded_path = new char[path_size + 1];        // +1 for cgltf_decode_uri

					std::copy(texture_path.begin(), texture_path.end(), decoded_path);
					decoded_path[path_size] = '\0';
					cgltf_decode_uri(decoded_path);

#if defined(USE_JS)
					future_texures.emplace_back(js.push_job(from_file_lambda, decoded_path));        // vector of job_handles
#else
					this->m_images.emplace_back(from_file_lambda(texture_path));
#endif
				}
			}
			else
			{
				assert(data->images[i].buffer_view && data->images[i].mime_type && "Image with buffer view must have a valid buffer view and mimeType");
#if defined(USE_JS)
				future_texures.emplace_back(js.push_job(from_buffer_view_lambda, buffers_load_handle.job(), data->images[i].buffer_view, data->images[i].mime_type));        // vector of job_handles
#else
				this->m_images.emplace_back(from_buffer_view_lambda(data->images[i].buffer_view, data->images[i].mime_type));
#endif
			}
			image_to_index.emplace(&data->images[i], i);
		}

		// This job reads all samplers, textures and materials in one job
		auto rest_of_data_load_lambda = [&]() -> bool {
			// Lets validate the data loaded is correct as well
			if constexpr (get_build() == BuildType::build_debug)
			{
				auto res = cgltf_validate(data);
				assert(res == cgltf_result_success && "Invalid glTF data");
				(void) res;
			}

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
				cgltf_image *image = data->textures[i].image;

				if (!image)
					image = data->textures[i].basisu_image;

				if (!image)
					ror::log_critical("Texture must have an image, neither standard image nor basis ktx2 texture found");

				rhi::Texture texture;
				texture.texture_image(rhi::TextureImageHandle(find_safe_index(image_to_index, image)));
				assert(texture.texture_image() != -1 && "Couldn't find the image loaded for the texture");

				auto texture_sampler = -1;

				if (data->textures[i].sampler != nullptr)
					texture_sampler = find_safe_index(sampler_to_index, data->textures[i].sampler);

				// If no samplers provided use default at index 0
				if (texture_sampler == -1)
					texture_sampler = 0;

				texture.texture_sampler(rhi::TextureSamplerHandle(static_cast_safe<uint32_t>(texture_sampler)));

				this->m_textures.emplace_back(std::move(texture));
				texture_to_index.emplace(&data->textures[i], i);
			}

			// Read all the materials
			this->m_materials.reserve(data->materials_count);
			for (size_t i = 0; i < data->materials_count; ++i)
			{
				// Using calculations from https://google.github.io/filament/Filament.html#toc4.8.3.2 to calculate f0 from ior
				const auto convert_ior_to_f0 = [](const float ior) { return ((ior - 1.0f) * (ior - 1.0f)) / ((ior + 1.0f) * (ior + 1.0f)); };

				const cgltf_material &mat = data->materials[i];

				Material material;
				material.m_double_sided     = mat.double_sided;
				material.m_opacity.m_factor = mat.alpha_cutoff;        // TODO: Check if there is a texture instead
				material.m_opacity.m_type   = Material::ComponentType::factor;

				if (mat.has_pbr_metallic_roughness)
				{
					// TODO: Separate textures and combine with occlusion
					read_material_component(material.m_base_color, texture_to_index, mat.pbr_metallic_roughness.base_color_texture, mat.pbr_metallic_roughness.base_color_factor);
					read_material_component(material.m_metallic, texture_to_index, mat.pbr_metallic_roughness.metallic_roughness_texture, &mat.pbr_metallic_roughness.metallic_factor);
					read_material_component(material.m_roughness, texture_to_index, mat.pbr_metallic_roughness.metallic_roughness_texture, &mat.pbr_metallic_roughness.roughness_factor);

					material.m_reflectance = mat.has_ior ? convert_ior_to_f0(mat.ior.ior) : 1.0f;

					// TODO: Add support for texture transforms and maybe scale for other textures apart from occlusion strength(scale) and normal scale which are alreayd handled
					// mat.pbr_metallic_roughness.base_color_texture.scale
					// mat.pbr_metallic_roughness.base_color_texture.transform
				}
				else
				{
					assert(mat.has_pbr_specular_glossiness && "The must be either Metalic roughness or specular glossiness material provided");
					ror::log_critical("FIXME: Specular glossiness material not supported at the moment");
				}

				if (mat.occlusion_texture.texture)
				{
					// TODO: Combine the texture, with metalic roughness if not together already
					read_material_component(material.m_occlusion, texture_to_index, mat.occlusion_texture, &mat.occlusion_texture.scale);        // NOTE: This is actually occlusion strength that cgltf calls scale, Default 1.0
				}

				if (mat.normal_texture.texture)
				{
					read_material_component(material.m_normal, texture_to_index, mat.normal_texture, &mat.normal_texture.scale);        // Default 1.0
				}

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

				if (mat.emissive_texture.texture)
				{
					// TODO: There is an Emissive factor default of 0, 0, 0 which should be used even if no textue available
					float factor[4]{mat.emissive_factor[0], mat.emissive_factor[1], mat.emissive_factor[2], (mat.has_emissive_strength ? mat.emissive_strength.emissive_strength : 1.0f)};
					read_material_component(material.m_emissive, texture_to_index, mat.emissive_texture, factor);
				}

				if (mat.has_clearcoat)
				{
					float normal[3]{0.0f, 0.0f, 1.0f};

					read_material_component(material.m_clearcoat, texture_to_index, mat.clearcoat.clearcoat_texture, &mat.clearcoat.clearcoat_factor);
					read_material_component(material.m_clearcoat_normal, texture_to_index, mat.clearcoat.clearcoat_normal_texture, normal);
					read_material_component(material.m_clearcoat_roughness, texture_to_index, mat.clearcoat.clearcoat_roughness_texture, &mat.clearcoat.clearcoat_roughness_factor);
				}

				if (mat.has_transmission)
				{
					float factor[2]{mat.transmission.transmission_factor, 0.0f /*Transmission roughness 0.0f, Should this be texture.scale?*/};
					read_material_component(material.m_transmission, texture_to_index, mat.transmission.transmission_texture, factor);
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

					ror::log_critical("Don't support specular extension at the moment");
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
					ror::log_critical("Don't support volume extension at the moment");
				}

				if (mat.has_sheen)
				{
					read_material_component(material.m_sheen_color, texture_to_index, mat.sheen.sheen_color_texture, mat.sheen.sheen_color_factor);
					read_material_component(material.m_sheen_roughness, texture_to_index, mat.sheen.sheen_roughness_texture, &mat.sheen.sheen_roughness_factor);
				}

				if (mat.has_ior)
				{
					// typedef struct cgltf_ior
					// {
					//	cgltf_float ior;
					// } cgltf_ior;
					ror::log_critical("Don't support ior extension at the moment");
				}

				// Lets generate a GLSL ShaderBuffer for this material // TODO: This shouldn't be GLSL specific
				material.generate_hash();

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
				mesh.m_primitive_vertex_hashes.resize(cmesh.primitives_count);
				mesh.m_primitive_fragment_hashes.resize(cmesh.primitives_count);
				mesh.m_primitive_program_hashes.resize(cmesh.primitives_count);
				// The last 2 are important and definitely needs reserving because these are BufferAllocated
				mesh.m_bounding_boxes.resize(cmesh.primitives_count);
				mesh.m_material_indices.resize(cmesh.primitives_count);
				mesh.m_program_indices.resize(cmesh.primitives_count);

				for (size_t j = 0; j < cmesh.primitives_count; ++j)
				{
					const cgltf_primitive &cprim                                    = cmesh.primitives[j];
					auto                  &vertex_attribute_descriptor              = mesh.m_attribute_vertex_descriptors[j];
					auto                  &morph_target_vertex_attribute_descriptor = mesh.m_morph_targets_vertex_descriptors[j];

					std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>> attribs_data;

					assert(cprim.type == cgltf_primitive_type_triangles && "Mesh primitive type is not triangles which is the only supported type at the moment");
					mesh.m_primitive_types[j] = cglf_primitive_to_primitive_topology(cprim.type);

					if (cprim.has_draco_mesh_compression)
						ror::log_critical("Mesh has draco mesh compression but its not supported");

					if (cprim.material)
						mesh.m_material_indices[j] = find_safe_index(material_to_index, cprim.material);
					else
						mesh.m_material_indices[j] = -1;

					mesh.m_program_indices[j] = -1;

					// Read all other vertex attributes
					for (size_t k = 0; k < cprim.attributes_count; ++k)
					{
						const cgltf_attribute &attrib = cprim.attributes[k];

						assert(attrib.data->buffer_view && "rhi::BufferView doesn't have a valid buffer view");

						if (attrib.data->is_sparse)
							ror::log_critical("Don't support sparse attribute accessors");

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
								assert((attrib.data->component_type == cgltf_component_type_r_32f || attrib.data->component_type == cgltf_component_type_r_8) &&
								       (attrib.data->type == cgltf_type_vec3 || attrib.data->type == cgltf_type_vec2) && "Normal not in the right format, float3 required");
								assert(attrib.index == 0 && "Don't suport more than 1 normal");
								current_index = rhi::BufferSemantic::vertex_normal;
								break;
							case cgltf_attribute_type_tangent:
								assert((attrib.data->component_type == cgltf_component_type_r_32f || attrib.data->component_type == cgltf_component_type_r_8) &&
								       (attrib.data->type == cgltf_type_vec4 || attrib.data->type == cgltf_type_vec3) && "Tangent not in the right format, float4 required");
								assert(attrib.index == 0 && "Don't suport more than 1 tangent");
								current_index = rhi::BufferSemantic::vertex_tangent;
								break;
							case cgltf_attribute_type_texcoord:
								assert((attrib.data->component_type == cgltf_component_type_r_32f || attrib.data->component_type == cgltf_component_type_r_16u) &&
								       (attrib.data->type == cgltf_type_vec2 || attrib.data->type == cgltf_type_vec3) && "Texture coordinate not in the right format, float2 required");
								assert(attrib.index < 3 && "Don't support more than 3 texture coordinate sets");
								current_index = static_cast<rhi::BufferSemantic>(ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0) << static_cast<uint64_t>(attrib.index));
								break;
							case cgltf_attribute_type_color:
								assert((attrib.data->component_type == cgltf_component_type_r_32f || attrib.data->component_type == cgltf_component_type_r_8u) &&
								       (attrib.data->type == cgltf_type_vec3 || attrib.data->type == cgltf_type_vec4) && "Color not in the right format, float3 required");
								assert(attrib.index < 2 && "Don't support more than 2 color sets");
								current_index = static_cast<rhi::BufferSemantic>(ror::enum_to_type_cast(rhi::BufferSemantic::vertex_color_0) << static_cast<uint64_t>(attrib.index));
								break;
							case cgltf_attribute_type_joints:
								assert((attrib.data->component_type == cgltf_component_type_r_32u || attrib.data->component_type == cgltf_component_type_r_16u || attrib.data->component_type == cgltf_component_type_r_8u) &&
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

						if (attrib_accessor->normalized)
							ror::log_critical("Attribute accessor data is normalised but there is no support at the moment for normalised data");

						assert(buffer_index >= 0 && "Not a valid buffer index returned, possibly no buffer associated with this attribute");
						uint8_t *data_pointer = reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data());

						std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer + offset, attrib_accessor->count * attrib_byte_size, stride};
						attribs_data.emplace(current_index, std::move(data_tuple));

						vertex_attribute_descriptor.add(current_index, attrib_format, &bp);
					}

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

						if (attrib_accessor->normalized)
							ror::log_critical("Attribute Index data is normalised but there is no support at the moment for normalised data");

						assert(buffer_index >= 0 && "Not a valid buffer index returned, possibly no buffer associated with this index buffer");
						uint8_t *data_pointer = reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data());

						std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer, attrib_accessor->count * indices_byte_size, stride};
						attribs_data.emplace(rhi::BufferSemantic::vertex_index, std::move(data_tuple));

						vertex_attribute_descriptor.add(rhi::BufferSemantic::vertex_index, index_format, &bp);
					}
					else
						mesh.m_has_indices_states[j] = false;

					// Now upload data from all the attributes into vertex_attribute_descriptor
					vertex_attribute_descriptor.upload(attribs_data, &bp);

					// Read morph targets
					for (size_t k = 0; k < cprim.targets_count; ++k)
					{
						const cgltf_morph_target &target = cprim.targets[k];

						std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>> morph_targets_attribs_data;

						rhi::VertexDescriptor target_vertex_descriptor{};
						for (size_t l = 0; l < target.attributes_count; ++l)
						{
							const cgltf_attribute &attrib = target.attributes[l];

							if (attrib.data->is_sparse)
								ror::log_critical("Don't support sparse attribute accessors");

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
									assert((attrib.data->component_type == cgltf_component_type_r_32f || attrib.data->component_type == cgltf_component_type_r_8) &&
									       (attrib.data->type == cgltf_type_vec3 || attrib.data->type == cgltf_type_vec2) && "Normal not in the right format");
									current_index = rhi::BufferSemantic::vertex_normal;
									break;
								case cgltf_attribute_type_tangent:
									assert((attrib.data->component_type == cgltf_component_type_r_32f || attrib.data->component_type == cgltf_component_type_r_8) &&
									       (attrib.data->type == cgltf_type_vec4 || attrib.data->type == cgltf_type_vec3) && "Tangent not in the right format");
									current_index = rhi::BufferSemantic::vertex_tangent;
									break;
								case cgltf_attribute_type_texcoord:
								case cgltf_attribute_type_color:
								case cgltf_attribute_type_joints:
								case cgltf_attribute_type_weights:
								case cgltf_attribute_type_invalid:
									assert(0 && "Morph target not supported for this attribute");
									break;
							}

							// TODO: GL expects stride to be zero if data is tightly packed

							const auto *attrib_accessor  = attrib.data;
							auto        attrib_format    = get_format_from_gltf_type_format(attrib_accessor->type, attrib_accessor->component_type);
							auto        buffer_index     = find_safe_index(buffer_to_index, attrib_accessor->buffer_view->buffer);
							auto        attrib_byte_size = cgltf_calc_size(attrib_accessor->type, attrib_accessor->component_type);
							auto        stride           = attrib_accessor->buffer_view->stride;
							auto        offset           = attrib_accessor->buffer_view->offset + attrib_accessor->offset;

							if (attrib_accessor->normalized)
								ror::log_critical("Attribute morph taget data is normalised but there is no support at the moment for normalised data");

							assert(buffer_index >= 0 && "Not a valid buffer index returned, possibly no buffer associated with this morph target attribute");
							uint8_t *data_pointer = reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data());

							std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer + offset, attrib_accessor->count * attrib_byte_size, stride};

							morph_targets_attribs_data.emplace(current_index, std::move(data_tuple));

							target_vertex_descriptor.add(current_index, attrib_format, &bp);
						}

						// Now copy all the buffer_views for morph_targets
						target_vertex_descriptor.upload(morph_targets_attribs_data, &bp);
						morph_target_vertex_attribute_descriptor.emplace_back(std::move(target_vertex_descriptor));
					}

					// By now each attribute and morph target attributes are loaded, lets update the locations in morph target attributes
					uint32_t location_offset = 0;
					if (!vertex_attribute_descriptor.attributes().empty())
					{
						const auto &att = vertex_attribute_descriptor.attributes().back();

						// Its +1 than the last attribute in the list, but if last attribute was vertex_index we use its location instead
						auto location = att.location();
						if (att.semantics() == rhi::BufferSemantic::vertex_index)
							location_offset = location;
						else
							location_offset = location + 1;
					}

					for (auto &morph_vertex_descriptor : morph_target_vertex_attribute_descriptor)
					{
						auto &morph_attributes = morph_vertex_descriptor.attributes();
						for (auto &morph_attribute : morph_attributes)
						{
							morph_attribute.location(morph_attribute.location() + location_offset);
						}
						if (!morph_attributes.empty())
						{
							auto location = morph_attributes.back().location();
							location_offset = location + 1;
						}
					}

					// Save Morph target weights
					assert(cmesh.weights_count == cprim.targets_count && "Targets count and weights don't match data error");
					mesh.m_morph_weights.resize(cmesh.weights_count);

					// TODO: Do a bulk copy please once tested and it works
					for (size_t m = 0; m < cmesh.weights_count; ++m)
						mesh.m_morph_weights[m] = cmesh.weights[m];
				}

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
					assert(cskin.inverse_bind_matrices->type == cgltf_type_mat4 && "Inverse bind matrix must be 4x4 matrices");

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
						ror::log_warn("Stride was suppose to be adjusted for GL, it won't work in Vulkan/Metal/DX");
					}
				}

				skin.m_joints.reserve(cskin.joints_count);
				for (size_t j = 0; j < cskin.joints_count; ++j)
					skin.m_joints.emplace_back(find_safe_index(node_to_index, cskin.joints[j]));

				skin.m_joint_matrices.resize(cskin.joints_count);

				skin_to_index.emplace(&cskin, i);
				this->m_skins.emplace_back(std::move(skin));
			}

			// Read the scene and flatten all scenes into a node tree
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

					// Lets update Mesh skin_index as well
					if (node.m_mesh_index > -1)
						this->m_meshes[static_cast<size_t>(node.m_mesh_index)].m_skin_index = node.m_skin_index;

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
					{
						if (cnode->has_scale == 0 && cnode->has_rotation == 0 && cnode->has_translation == 0)
						{
							Matrix4f transformation{};        //! Transformation in the form of a matrix for the node
							cgltf_node_transform_local(cnode, transformation.m_values);
							node.m_trs_transform.set(transformation);
						}
						else
							ror::log_critical("Node has transformation as well as a matrix leaving TRS unchanged");
					}

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

						if (anim_sampler_accessor->normalized)
							ror::log_critical("Animation sampler input data is normalised but there is no support at the moment for normalised data");

						if constexpr (get_build() == BuildType::build_debug)
						{
							auto format           = get_format_from_gltf_type_format(anim_sampler_accessor->type, anim_sampler_accessor->component_type);
							auto attrib_byte_size = cgltf_calc_size(anim_sampler_accessor->type, anim_sampler_accessor->component_type);

							assert(format == rhi::VertexFormat::float32_1 && attrib_byte_size == 4 && "Animation sampler input value must be in float scalar 4 bytes");
							assert(anim_sampler_accessor->buffer_view->stride <= attrib_byte_size && "Looks like sampler input data is interleaved, not supported");
							(void) format;
							(void) attrib_byte_size;
						}

						animation_sampler.m_input.resize(anim_sampler_accessor->count);        // Don't need to multiply attrib_byte_size because m_input is float32_t
						cgltf_accessor_unpack_floats(anim_sampler_accessor,
						                             reinterpret_cast<cgltf_float *>(animation_sampler.m_input.data()),
						                             animation_sampler.m_input.size());
					}

					{
						const auto *anim_sampler_accessor = canimation.samplers[j].output;

						if (anim_sampler_accessor->normalized)
							ror::log_critical("Animation sampler output data is normalised but there is no support at the moment for normalised data");

						if constexpr (get_build() == BuildType::build_debug)
						{
							auto attrib_byte_size = cgltf_calc_size(anim_sampler_accessor->type, anim_sampler_accessor->component_type);
							assert(anim_sampler_accessor->buffer_view->stride <= attrib_byte_size && "Looks like sampler output data is interleaved, not supported");
							(void) attrib_byte_size;
						}
						auto component_size  = cgltf_component_size(anim_sampler_accessor->component_type);        // Bytes
						auto component_count = cgltf_num_components(anim_sampler_accessor->type);                  // vec2, vec3
						auto format          = get_format_from_gltf_type_format(anim_sampler_accessor->type, anim_sampler_accessor->component_type);

						animation_sampler.m_output.resize(anim_sampler_accessor->count * sizeof(float32_t) * component_count);        // Using sizeof float32_t which is the same as sizeof uint32_t these are the two types allowed

						if (anim_sampler_accessor->component_type < cgltf_component_type_r_32f)
						{
							animation_sampler.m_output_format = int_format_to_int32_format_bit(format);        // Since we are casting everyting to uint lets adjust format accordingly
							uint32_t *ptr_to_data             = reinterpret_cast<uint32_t *>(animation_sampler.m_output.data());
							for (size_t index = 0; index < anim_sampler_accessor->count; ++index)
							{
								cgltf_accessor_read_uint(anim_sampler_accessor, index, ptr_to_data, component_size);
								ptr_to_data += component_count;
							}
						}
						else
						{
							animation_sampler.m_output_format = format;
							cgltf_accessor_unpack_floats(anim_sampler_accessor,
							                             reinterpret_cast<cgltf_float *>(animation_sampler.m_output.data()),
							                             animation_sampler.m_output.size());
						}
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

			// Update mesh hashes after everything is loaded
			for (size_t i = 0; i < data->meshes_count; ++i)
			{
				Mesh             &mesh  = this->m_meshes[i];
				const cgltf_mesh &cmesh = data->meshes[i];

				if (cmesh.name)
					mesh.m_name = cmesh.name;

				for (size_t j = 0; j < cmesh.primitives_count; ++j)
				{
					auto &vertex_attribute_descriptor              = mesh.m_attribute_vertex_descriptors[j];
					auto &morph_target_vertex_attribute_descriptor = mesh.m_morph_targets_vertex_descriptors[j];
					auto &vertex_hash                              = mesh.m_primitive_vertex_hashes[j];
					auto &fragment_hash                            = mesh.m_primitive_fragment_hashes[j];
					auto &program_hash                             = mesh.m_primitive_program_hashes[j];

					// Setup vertex hash
					vertex_hash = vertex_attribute_descriptor.hash_64();

					for (auto &attrib : morph_target_vertex_attribute_descriptor)
						hash_combine_64(vertex_hash, attrib.hash_64());

					// Only check if we have weights
					auto weights_count = mesh.m_morph_weights.size();

					if (weights_count > 0)
						hash_combine_64(vertex_hash, hash_64(&weights_count, sizeof(weights_count)));

					auto skin_index = mesh.m_skin_index;
					if (skin_index != -1)
					{
						auto &skin = this->m_skins[ror::static_cast_safe<size_t>(skin_index)];
						hash_combine_64(vertex_hash, skin.m_joints.size());
					}

					// Setup material hash
					// Adding material index here because if material is the same for specific vertex attributes then we use the same shader
					auto material_index = mesh.m_material_indices[j];
					fragment_hash       = vertex_hash;
					if (material_index != -1)
					{
						auto &material = this->m_materials[ror::static_cast_safe<size_t>(material_index)];
						hash_combine_64(fragment_hash, material.m_hash);
					}

					// Setup program hash
					program_hash = vertex_hash;
					hash_combine_64(program_hash, fragment_hash);
				}

				// Lets update the mesh hash
				mesh.generate_hash();
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
		assert(data->images_count == future_texures.size() && "Not all textures were queued for loading");
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
}

void Model::upload(rhi::Device &a_device)
{
	// TODO: Jobify me
	for (auto &sampler : this->m_samplers)
	{
		sampler.upload(a_device);
	}

	for (auto &image : this->m_images)
	{
		image.upload(a_device);
	}
}

void Model::update_mesh_program(uint32_t a_mesh_index, uint32_t a_primitive_index, int32_t a_program_index)
{
	this->m_meshes[a_mesh_index].m_program_indices[a_primitive_index] = a_program_index;
}

}        // namespace ror
