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

#include "camera/rorcamera.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rornode.hpp"
#include "math/rormatrix.hpp"
#include "math/rorvector.hpp"
#include "math/rorvector4.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorbuffer_view.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rorhandles.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <set>
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <utility>
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

	rhi::read_texture_from_memory(data, data_size, ti, false, "cgltf_base64_image");

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
	rhi::read_texture_from_memory(data, data_size, ti, false, (a_buffer_view->name != nullptr) ? a_buffer_view->name : "cgltf_buffer_view_image");

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
		case rhi::VertexFormat::struct_0:
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

static void force_sampler_mipmaps(rhi::TextureSampler &a_sampler)
{
	a_sampler.mip_mode(rhi::TextureMipFilter::linear);
	a_sampler.min_filter(rhi::TextureFilter::linear);
	a_sampler.mag_filter(rhi::TextureFilter::linear);
}

static void force_image_mipmaps(rhi::TextureImage &a_image)
{
	a_image.mipmapped(true);
	a_image.mip_gen_mode(rhi::TextureMipGenMode::automatic);
	a_image.setup();
}

static void force_mipmaps(rhi::TextureImage &a_image, rhi::TextureSampler &a_sampler)
{
	force_image_mipmaps(a_image);
	force_sampler_mipmaps(a_sampler);
}

rhi::PrimitiveTopology cglf_primitive_to_primitive_topology(cgltf_primitive_type a_type)
{
	// clang-format off
	switch (a_type)
	{
		case cgltf_primitive_type_points:            return rhi::PrimitiveTopology::points;
		case cgltf_primitive_type_lines:             return rhi::PrimitiveTopology::lines;
		case cgltf_primitive_type_line_strip:        return rhi::PrimitiveTopology::lines_strip;
		case cgltf_primitive_type_triangles:         return rhi::PrimitiveTopology::triangles;
		case cgltf_primitive_type_triangle_strip:    return rhi::PrimitiveTopology::triangles_strip;

		case cgltf_primitive_type_line_loop:
		case cgltf_primitive_type_triangle_fan:
			assert(0 && "Unsupported primitive type foudn in gltf");
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

ror::OrbitCamera read_node_camera(cgltf_camera *a_camera)
{
	ror::OrbitCamera camera;

	if (a_camera->type == cgltf_camera_type_perspective)
	{
		camera.type(ror::CameraType::perspective);

		cgltf_camera_perspective &perspective_camera = a_camera->data.perspective;

		if (perspective_camera.has_aspect_ratio)
			camera.ratio(perspective_camera.aspect_ratio);

		camera.fov(perspective_camera.yfov);

		if (perspective_camera.has_zfar)
			camera.far(perspective_camera.zfar);

		camera.near(perspective_camera.znear);
	}
	else if (a_camera->type == cgltf_camera_type_orthographic)
	{
		cgltf_camera_orthographic &orthographic_camera = a_camera->data.orthographic;
		camera.type(ror::CameraType::orthographic);
		camera.far(orthographic_camera.zfar);
		camera.near(orthographic_camera.znear);
		(void) orthographic_camera.xmag;
		(void) orthographic_camera.ymag;
		ror::log_warn("Unused orthographic camera parameter xmag and ymag");
	}
	else
	{
		ror::log_warn("Data error! Invalid camera type provided, using default camera.");
	}

	return camera;
}

void get_accessor_info(const cgltf_accessor *a_accessor, size_t &a_component_count, size_t &a_byte_size, size_t &a_offset, size_t &a_stride, uint8_t **a_ptr)
{
	a_component_count = cgltf_num_components(a_accessor->type);        // could be vec2, vec3 etc and returning 2, 3 respectively
	a_byte_size       = cgltf_calc_size(a_accessor->type, a_accessor->component_type);
	a_offset          = a_accessor->buffer_view->offset + a_accessor->offset;
	a_stride          = a_accessor->stride;

	if (a_stride == 0)
		a_stride = a_byte_size;

	*a_ptr = reinterpret_cast<uint8_t *>(a_accessor->buffer_view->buffer->data) + a_offset;
}

std::vector<uint16_t> unpack_uint8_to_uint16(const cgltf_accessor *a_accessor)
{
	std::vector<uint16_t> temp;

	size_t   component_count;
	size_t   byte_size;
	size_t   offset;
	size_t   stride;
	uint8_t *ptr{nullptr};

	get_accessor_info(a_accessor, component_count, byte_size, offset, stride, &ptr);

	assert(byte_size == sizeof(uint8_t) && "Can't unpack from non-uint8_t");
	assert(component_count == 1 && "Can only unpack single component accessors");

	temp.reserve(a_accessor->count);

	for (size_t i = 0; i < a_accessor->count; ++i)
	{
		temp.emplace_back((reinterpret_cast<uint8_t *>(ptr))[0]);

		ptr += stride;
	}

	return temp;
}

// clang-format off
template <typename> constexpr float32_t type_max()            noexcept { ror::log_critical("Can't normalize unsupported weights"); return 1.0f; }
template <>         constexpr float32_t type_max<float32_t>() noexcept { return 1.0f;                                                           }
template <>         constexpr float32_t type_max<uint8_t>()   noexcept { return static_cast<uint32_t>(255);                                     }
template <>         constexpr float32_t type_max<uint16_t>()  noexcept { return static_cast<uint32_t>(65535);                                   }
// clang-format on

template <typename _type>
std::vector<_type> unpack_normalized(const cgltf_accessor *a_accessor)
{
	std::vector<_type> temp;

	size_t   component_count;
	size_t   byte_size;
	size_t   offset;
	size_t   stride;
	uint8_t *ptr{nullptr};

	get_accessor_info(a_accessor, component_count, byte_size, offset, stride, &ptr);

	assert(component_count == 4 && "Can only unpack weights having 4 components");

	temp.reserve(a_accessor->count);

	for (size_t i = 0; i < a_accessor->count; ++i)
	{
		auto x = (reinterpret_cast<_type *>(ptr))[0];
		auto y = (reinterpret_cast<_type *>(ptr))[1];
		auto z = (reinterpret_cast<_type *>(ptr))[2];
		auto w = (reinterpret_cast<_type *>(ptr))[3];

		float32_t sum = static_cast<float32_t>(x + y + z + w);

		if (sum > 0.0f && (sum < type_max<_type>() || sum > type_max<_type>()))
		{
			sum = type_max<_type>() / sum;

			x = static_cast<_type>(static_cast<float32_t>(x) * sum);
			y = static_cast<_type>(static_cast<float32_t>(y) * sum);
			z = static_cast<_type>(static_cast<float32_t>(z) * sum);
			w = static_cast<_type>(static_cast<float32_t>(w) * sum);
		}

		temp.emplace_back(x);
		temp.emplace_back(y);
		temp.emplace_back(z);
		temp.emplace_back(w);

		ptr += stride;
	}

	return temp;
}

// Only support packing from and into a 4-channel texture at the moment
void pack_into_channel(ror::Material::Component<float32_t>                                     &a_destination,
                       ror::Material::Component<float32_t>                                     &a_source,
                       std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>           &a_textures,
                       std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>> &a_images,
                       uint32_t a_destination_channel, uint32_t a_source_channel)
{
	auto &d_texture = a_textures[static_cast_safe<size_t>(a_destination.m_texture)];
	auto &s_texture = a_textures[static_cast_safe<size_t>(a_source.m_texture)];
	assert(d_texture.texture_image() != -1);
	assert(s_texture.texture_image() != -1);
	auto &d_image = a_images[static_cast_safe<size_t>(d_texture.texture_image())];
	auto &s_image = a_images[static_cast_safe<size_t>(s_texture.texture_image())];

	auto d_width  = d_image.width();
	auto d_height = d_image.height();
	auto d_depth  = d_image.depth();
	auto d_bpp    = d_image.bytes_per_pixel();

	auto s_width  = s_image.width();
	auto s_height = s_image.height();
	auto s_depth  = s_image.depth();
	auto s_bpp    = s_image.bytes_per_pixel();

	assert(d_bpp == 4 && s_bpp == 4 && "Packing only supports 4 bpp destination and source textures");
	assert(a_destination_channel < 4 && a_source_channel < 4 && "Channel indices out of bound");
	assert(d_width == s_width && d_height == s_height && d_depth == s_depth && "Width, height and depth of both textures is not the same");
	// Don't remove these, required for release build
	(void) d_depth;
	(void) s_depth;
	(void) s_width;
	(void) s_height;
	(void) s_bpp;

	auto d_data = d_image.data();
	auto s_data = s_image.data();

	for (size_t i = 0; i < d_width * d_height * d_bpp; i += d_bpp)
		d_data[i + a_destination_channel] = s_data[i + a_source_channel];

	// Now reset the destination texture image index
	// At this point the o_texture_image will be unreferenced (thats ok) or referenced by someone else for something else
	// This doesn't help much on its own unless we do the shader work of saving sampler and texture bindings
	// TODO: Fix shaders to reuse the samplers and textures for AO
	s_texture.texture_image(d_texture.texture_image());
	s_texture.texture_sampler(d_texture.texture_sampler());
}

void pack_materials_textures(std::vector<ror::Material, rhi::BufferAllocator<ror::Material>>         &a_materials,
                             std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>           &a_textures,
                             std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>> &a_images)
{
	for (auto &material : a_materials)
	{
		if (material.m_roughness.m_texture != -1)
		{
			assert(material.m_roughness.m_texture == material.m_metallic.m_texture && "Material roughness and metallic should be the same texture");
			// Lets pack occlusion into roughness Red
			if (material.m_occlusion.m_texture != -1 && material.m_roughness.m_texture != material.m_occlusion.m_texture)
				pack_into_channel(material.m_roughness, material.m_occlusion, a_textures, a_images, 0, 0);        // Pack into Red channel

			// and height into Alpha if exists
			if (material.m_height.m_texture != -1 && material.m_roughness.m_texture != material.m_height.m_texture)
				pack_into_channel(material.m_roughness, material.m_height, a_textures, a_images, 3, 0);        // Pack into Alpha channel
		}
	}
}

template <typename _component_type>
FORCE_INLINE void set_image_to_linear(_component_type &a_material, const char *a_name,
                                      std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>           &a_textures,
                                      std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>> &a_images)
{
	if (a_material.m_texture != -1)
	{
		auto &texture = a_textures[static_cast_safe<size_t>(a_material.m_texture)];
		assert(texture.texture_image() != -1);
		auto &image = a_images[static_cast<size_t>(texture.texture_image())];

		if (image.format() == rhi::PixelFormat::r8g8b8a8_uint32_norm)
			ror::log_info("{} texture has already been modified", a_name);

		image.format(rhi::PixelFormat::r8g8b8a8_uint32_norm);
	}
}
void update_materials_textures_to_linear(std::vector<ror::Material, rhi::BufferAllocator<ror::Material>>         &a_materials,
                                         std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>           &a_textures,
                                         std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>> &a_images)
{
	for (auto &material : a_materials)
	{
		// These ones are suppose to be in sRGB which is default texture format
		// set_image_to_srgb(material.m_base_color, "Base color", a_textures, a_images);
		// set_image_to_srgb(material.m_emissive, "Emissive color", a_textures, a_images);
		// set_image_to_linear(material.m_sheen_color, "Sheen", a_textures, a_images);
		// set_image_to_linear(material.m_specular_glossyness, "Specular glossyness", a_textures, a_images);

		// These ones needs to be reset to linear
		set_image_to_linear(material.m_diffuse_color, "Diffuse", a_textures, a_images);
		set_image_to_linear(material.m_anisotropy, "Anisotropy", a_textures, a_images);
		set_image_to_linear(material.m_transmission, "Transmission", a_textures, a_images);
		set_image_to_linear(material.m_sheen_roughness, "Sheen roughness", a_textures, a_images);
		set_image_to_linear(material.m_clearcoat_normal, "Clearcoat normal", a_textures, a_images);
		set_image_to_linear(material.m_clearcoat, "Clearcoat", a_textures, a_images);
		set_image_to_linear(material.m_clearcoat_roughness, "Clearcoat roughness", a_textures, a_images);
		set_image_to_linear(material.m_metallic, "Metallic", a_textures, a_images);
		set_image_to_linear(material.m_roughness, "Roughness", a_textures, a_images);
		set_image_to_linear(material.m_occlusion, "Occlusion", a_textures, a_images);
		set_image_to_linear(material.m_normal, "Normal", a_textures, a_images);
		set_image_to_linear(material.m_bent_normal, "Bent normal", a_textures, a_images);
		set_image_to_linear(material.m_height, "Height", a_textures, a_images);
		set_image_to_linear(material.m_subsurface_color, "Subsurface", a_textures, a_images);
	}
}

void Model::update_hashes()
{
	for (size_t i = 0; i < this->m_meshes.size(); ++i)
	{
		Mesh &mesh = this->m_meshes[i];

		for (size_t j = 0; j < mesh.primitives_count(); ++j)
		{
			auto   skin_index   = mesh.skin_index();
			size_t joints_count = 0;
			if (skin_index != -1)
			{
				auto &skin   = this->m_skins[ror::static_cast_safe<size_t>(skin_index)];
				joints_count = skin.joints_count();
			}

			auto material_index = mesh.material(j);
			assert(material_index != -1 && "Material index can't be -1");
			hash_64_t material_hash = 0;
			auto     &material      = this->m_materials[ror::static_cast_safe<size_t>(material_index)];
			material_hash           = material.m_hash;

			mesh.update_primitive_hash(j, joints_count, material_hash);
		}

		// Lets update the mesh hash
		mesh.generate_hash();
	}
}

/**
 * @brief      Use this to create a mesh in the model ready to describe its VertexDescriptor and upload data into it
 * @details    This method will create a default mesh with default sampler and default material if no materials provided.
 *             One has to then define the VertexDescriptor for one or all of its primitives and upload its data.
 *             Then you should be ready to use it.
 * @param      a_name Name of the mesh.
 * @param      a_generate_shaders Whether to let the system generate shaders for it or shaders will be loaded manually.
 * @param      a_mesh_data        This is a typle of primitives type, has indices bool and material index for each primitive
 * @param      a_mesh_materials   The materials you want the mesh to use. If empty a default one will be created.
 * @return     return void
 */

void Model::create_default_mesh(const char                  *a_name,
                                bool                         a_generate_shaders,
                                DefaultMeshData              a_mesh_data,
                                std::vector<ror::Material> &&a_mesh_materials)
{
	if (a_mesh_data.size() == 0)
	{
		log_critical("Can't create a default mesh with zero meshes count {}", a_mesh_data.size());
		return;
	}
	else if (a_mesh_data[0].size() == 0)
	{
		log_critical("Can't create a default mesh with zero primitives count {}", a_mesh_data[0].size());
		return;
	}

	this->m_generate_shaders = a_generate_shaders;

	// All things to do for a Simple Model/Mesh, however not all apply in all cases
	// Load image
	// Create texture using
	// default sampler at 0
	// default material at 0
	// Create nodes and then
	// Create mesh
	// Update the bounding box
	// Load shaders/programs that this mesh can use, done at higher scene level than model

	assert(this->m_meshes.size() == 0 && "Mesh is already initialized");
	this->m_meshes.resize(a_mesh_data.size());        // Want to have a_mesh_count meshes

	// Lets have a default material and sampler at index 0
	this->m_samplers.emplace_back();

	// Save the provided materials if any
	if (a_mesh_materials.size())
		for (auto &material : a_mesh_materials)
		{
			material.fill_shader_buffer();
			this->m_materials.emplace_back(std::move(material));
		}
	else
	{
		this->m_materials.emplace_back();
		this->m_materials.back().fill_shader_buffer();
	}

	for (size_t mesh_id = 0; mesh_id < a_mesh_data.size(); ++mesh_id)
	{
		auto         &mesh           = this->m_meshes[mesh_id];
		auto         &primitive_data = a_mesh_data[mesh_id];
		ror::Vector3f min{};
		ror::Vector3f max{};

		ror::Node     node{};
		ror::NodeData node_data{};

		node.m_mesh_index = static_cast_safe<int32_t>(mesh_id);
		mesh.resize(primitive_data.size());        // Want to have a_primitive_data.size() mesh primitives
		mesh.name(a_name);

		for (size_t prim_id = 0; prim_id < primitive_data.size(); ++prim_id)
		{
			mesh.program(prim_id, -1);

			auto &primitive_tuple = primitive_data[prim_id];

			mesh.primitive_type(prim_id, std::get<0>(primitive_tuple));
			mesh.has_indices(prim_id, std::get<1>(primitive_tuple));
			mesh.material(prim_id, std::get<2>(primitive_tuple));

			auto &mesh_bbox = mesh.bounding_box(prim_id);
			mesh_bbox.create_from_min_max(min, max);        // Nothing added at the moment
		}

		this->m_nodes.emplace_back(std::move(node));
		this->m_nodes_side_data.emplace_back(std::move(node_data));
	}

	this->update_hashes();
}

static void upload_position4_color4(ror::Mesh &a_mesh, size_t a_primitive_id, std::vector<float32_t> &a_data, rhi::BuffersPack &a_buffers_pack)
{
	auto data_pointer = reinterpret_cast<uint8_t *>(a_data.data());
	auto data_size    = a_data.size() * sizeof(float32_t) / 2;
	auto stride       = sizeof(float32_t) * 8;

	a_mesh.mesh_data(a_primitive_id,
	                 {{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_4, data_pointer, data_size, stride},
	                  {rhi::BufferSemantic::vertex_color_0, rhi::VertexFormat::float32_4, data_pointer + sizeof(float32_t) * 4, data_size, stride}},
	                 a_buffers_pack);
}

static void upload_position2_uv2_color4_index(ror::Mesh &a_mesh, size_t a_primitive_id, std::vector<uint8_t> &a_data, size_t a_vertex_buffer_size, rhi::BuffersPack &a_buffers_pack)
{
	assert(a_data.size() > a_vertex_buffer_size);

	auto data_pointer         = reinterpret_cast<uint8_t *>(a_data.data());
	auto stride               = 20u;        // Fix size of ImGuiVert
	auto data_size_multiplier = a_vertex_buffer_size / stride;
	auto index_data_size      = a_data.size() - a_vertex_buffer_size;
	auto two_floats           = sizeof(float32_t) * 2;

	a_mesh.mesh_data(a_primitive_id,
	                 {{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_2, data_pointer, data_size_multiplier * two_floats, stride},
	                  {rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2, data_pointer + two_floats, data_size_multiplier * two_floats, stride},
	                  {rhi::BufferSemantic::vertex_color_0, rhi::VertexFormat::uint8_4_norm, data_pointer + sizeof(float32_t) * 4, data_size_multiplier * 4, stride},
	                  {rhi::BufferSemantic::vertex_index, rhi::VertexFormat::uint16_1, data_pointer + a_vertex_buffer_size, index_data_size, sizeof(uint16_t)}},
	                 a_buffers_pack);
}

static void update_position2_uv2_color4_index(ror::Mesh &a_mesh, size_t a_primitive_id, std::vector<uint8_t> &a_vertex_buffer, std::vector<uint8_t> &a_index_buffer, rhi::BuffersPack &a_buffers_pack)
{
	auto data_pointer       = reinterpret_cast<uint8_t *>(a_vertex_buffer.data());
	auto index_data_pointer = reinterpret_cast<uint8_t *>(a_index_buffer.data());
	auto stride             = 20u;        // Fix size of ImGuiVert
	assert(a_vertex_buffer.size() > stride);
	auto data_size_multiplier = a_vertex_buffer.size() / stride;

	std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>>
	    attribs_data{{rhi::BufferSemantic::vertex_position, {data_pointer, data_size_multiplier * sizeof(float32_t) * 2, stride}},
	                 {rhi::BufferSemantic::vertex_texture_coord_0, {data_pointer + sizeof(float32_t) * 2, data_size_multiplier * sizeof(float32_t) * 2, stride}},
	                 {rhi::BufferSemantic::vertex_color_0, {data_pointer + sizeof(float32_t) * 4, data_size_multiplier * 4, stride}},
	                 {rhi::BufferSemantic::vertex_index, {index_data_pointer, a_index_buffer.size(), sizeof(uint16_t)}}};

	a_mesh.update_mesh_data(a_primitive_id, attribs_data, a_buffers_pack);
}

void Model::create_debug(bool a_generate_shaders, std::vector<std::vector<float32_t>> &attributes_data, std::vector<rhi::PrimitiveTopology> a_topology, rhi::BuffersPack &a_buffers_pack)
{
	(void) upload_position2_uv2_color4_index;
	(void) update_position2_uv2_color4_index;

	// Grid is a geometry asset that single mesh and single primitive and has positions and colors only
	auto &setting = ror::settings();

	// Although its called debug_mesh_count its actually debug_mesh_primitives count in the 1 debug mesh
	auto primitives_count = std::min(attributes_data.size(), static_cast<size_t>(setting.m_debug_mesh_count));

	ror::Material material;
	material.m_material_model = rhi::MaterialModel::unlit;
	material.m_blend_mode     = rhi::BlendMode::blend;
	std::vector<ror::Material> materials;
	materials.emplace_back(std::move(material));

	DefaultMeshData                                                mesh_data;
	std::vector<std::tuple<rhi::PrimitiveTopology, bool, int32_t>> primitives_data;
	primitives_data.reserve(a_topology.size());
	for (auto topology : a_topology)
		primitives_data.emplace_back(std::make_tuple(topology, false, 0));

	mesh_data.emplace_back(primitives_data);

	this->create_default_mesh("debug", a_generate_shaders, mesh_data, std::move(materials));

	ror::Mesh &mesh = this->m_meshes[0];

	for (size_t prim_id = 0; prim_id < primitives_count; ++prim_id)
	{
		auto &primitive_data = attributes_data[prim_id];
		upload_position4_color4(mesh, prim_id, primitive_data, a_buffers_pack);
	}
}

void Model::reset()
{
	// Setting the attributes count to 0 essetially resets it
	for (auto &mesh : this->m_meshes)
	{
		for (size_t prim_id = 0; prim_id < mesh.primitives_count(); ++prim_id)
		{
			auto &vd = mesh.vertex_descriptor(prim_id);
			for (auto &attrib : vd.attributes())
				attrib.count(0);
		}
	}
}

// Obsolete method but shows a way how to make a model at runtime
void Model::create_grid(bool a_generate_shaders, rhi::BuffersPack &a_buffers_pack)
{
	ror::Material material;
	material.m_material_model = rhi::MaterialModel::unlit;
	material.m_blend_mode     = rhi::BlendMode::blend;
	std::vector<ror::Material> materials;
	materials.emplace_back(std::move(material));

	DefaultMeshData                                                mesh_data;
	std::vector<std::tuple<rhi::PrimitiveTopology, bool, int32_t>> primitives_data;
	primitives_data.emplace_back(std::make_tuple(rhi::PrimitiveTopology::lines, false, 0));

	mesh_data.emplace_back(primitives_data);

	this->create_default_mesh("grid", a_generate_shaders, mesh_data, std::move(materials));

	// Grid is a geometry asset with a single mesh and single primitive and has positions and colors only
	ror::Mesh &mesh    = this->m_meshes[0];
	uint32_t   prim_id = 0;

	std::vector<float> grid_data{};
	upload_position4_color4(mesh, prim_id, grid_data, a_buffers_pack);
}

void Model::load_from_gltf_file(std::filesystem::path a_filename, std::vector<ror::OrbitCamera> &a_cameras, std::vector<ror::Light> &a_lights, bool a_generate_shaders, rhi::BuffersPack &a_buffers_pack)
{
	(void) a_lights;

	this->m_generate_shaders = a_generate_shaders;

	// Lets start by reading a_filename via resource cache
	auto &resource = load_resource(a_filename, ResourceSemantic::models);
	auto &filename = resource.absolute_path();

	// Get an instance of job system
#if defined(USE_JS)
	auto &js = ror::get_job_system();
#endif

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
					std::string       texture_path = filename.parent_path() / uri;
					size_t            path_size    = texture_path.size();
					std::vector<char> decoded_path{};

					decoded_path.resize(path_size + 1);        // +1 for cgltf_decode_uri
					std::copy(texture_path.begin(), texture_path.end(), decoded_path.data());
					decoded_path.back() = '\0';

					auto decoded_size = cgltf_decode_uri(decoded_path.data());
					assert(decoded_size <= path_size && "Decoding from URI into path failed");
					(void) decoded_size;

#if defined(USE_JS)
					future_texures.emplace_back(js.push_job(from_file_lambda, std::filesystem::path{decoded_path.data()}));        // vector of job_handles
#else
					this->m_images.emplace_back(from_file_function(std::filesystem::path{decoded_path.data()}));
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

				rhi::Texture texture{};
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
			this->m_materials.reserve(data->materials_count + 1);
			// Lets have a default material at index 0
			this->m_materials.emplace_back();
			this->m_materials.back().fill_shader_buffer();
			for (size_t i = 0; i < data->materials_count; ++i)
			{
				// Using calculations from https://google.github.io/filament/Filament.html#toc4.8.3.2 to calculate f0 from ior
				const auto convert_ior_to_f0 = [](const float ior) { return ((ior - 1.0f) * (ior - 1.0f)) / ((ior + 1.0f) * (ior + 1.0f)); };

				const cgltf_material &mat = data->materials[i];

				Material material;
				material.m_double_sided = mat.double_sided;
				material.m_opacity      = mat.alpha_cutoff;        // only valid if alpha_mode == MASK

				if (mat.has_pbr_metallic_roughness)
				{
					read_material_component(material.m_base_color, texture_to_index, mat.pbr_metallic_roughness.base_color_texture, mat.pbr_metallic_roughness.base_color_factor);
					read_material_component(material.m_metallic, texture_to_index, mat.pbr_metallic_roughness.metallic_roughness_texture, &mat.pbr_metallic_roughness.metallic_factor);
					read_material_component(material.m_roughness, texture_to_index, mat.pbr_metallic_roughness.metallic_roughness_texture, &mat.pbr_metallic_roughness.roughness_factor);

					if (mat.has_ior)
						material.m_f0 = convert_ior_to_f0(mat.ior.ior);

					// TODO: Add support for texture transforms and maybe scale for other textures apart from occlusion strength(scale) and normal scale which are already handled
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
				material.fill_shader_buffer();

				this->m_materials.emplace_back(std::move(material));

				material_to_index.emplace(&mat, i + 1);        // Index 0 is default hence the +1
			}

			// Read all the meshes
			this->m_meshes.resize(data->meshes_count);
			for (size_t i = 0; i < data->meshes_count; ++i)
			{
				Mesh             &mesh  = this->m_meshes[i];
				const cgltf_mesh &cmesh = data->meshes[i];

				if (cmesh.name)
					mesh.name(cmesh.name);

				mesh.resize(cmesh.primitives_count);

				for (size_t j = 0; j < cmesh.primitives_count; ++j)
				{
					const cgltf_primitive &cprim                                    = cmesh.primitives[j];
					auto                  &vertex_attribute_descriptor              = mesh.vertex_descriptor(j);
					auto                  &morph_target_vertex_attribute_descriptor = mesh.target_descriptor(j);

					std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>> attribs_data;

					assert(cprim.type == cgltf_primitive_type_triangles && "Mesh primitive type is not triangles which is the only supported type at the moment");
					mesh.primitive_type(j, cglf_primitive_to_primitive_topology(cprim.type));

					if (cprim.has_draco_mesh_compression)
						ror::log_critical("Mesh has draco mesh compression but its not supported");

					if (cprim.material)
						mesh.material(j, find_safe_index(material_to_index, cprim.material));
					else
						mesh.material(j, 0);

					mesh.program(j, -1);

					std::array<std::vector<uint8_t>, 2>   weights_uint8_pointer{};
					std::array<std::vector<uint16_t>, 2>  weights_uint16_pointer{};
					std::array<std::vector<float32_t>, 2> weights_float32_pointer{};

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
								{
									auto &mesh_bbox = mesh.bounding_box(j);
									mesh_bbox.create_from_min_max({attrib.data->min[0], attrib.data->min[1], attrib.data->min[2]},
									                              {attrib.data->max[0], attrib.data->max[1], attrib.data->max[2]});
								}
								break;
							case cgltf_attribute_type_normal:
								assert((attrib.data->component_type == cgltf_component_type_r_32f || attrib.data->component_type == cgltf_component_type_r_8) &&
								       (attrib.data->type == cgltf_type_vec3 || attrib.data->type == cgltf_type_vec2) && "Normal not in the right format, float3 required");
								assert(attrib.index == 0 && "Don't suport more than 1 normal");
								current_index = rhi::BufferSemantic::vertex_normal;
								break;
							case cgltf_attribute_type_tangent:
								assert((attrib.data->component_type == cgltf_component_type_r_32f || attrib.data->component_type == cgltf_component_type_r_8) &&
								       (attrib.data->type == cgltf_type_vec4) && "Tangent not in the right format, float4 required");        // If its 3D only need to add w=[+1, -1] to denote handedness
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
						// NOTE: cgltf buffer_view->stride vs accessor->stride notes
						// If buffer_view is valid and has a stride and not zero (0), accessor->stride == buffer_view->stride
						// If buffer_view does not have a stride or its zero. accessor->stride is calculated from format byte size * number of components
						// What this means is that if (buffer_view->stride == 0) it means tightly packed data, use accessor->stride as stride from one element to another
						// If buffer->view->stride != 0 that means not-tightly packed data, stride is accessor->stride which is equal to buffer_view->stride
						// This buffer_view->stride is only valid for attributes or if enabled by extensions

						const auto *attrib_accessor  = attrib.data;
						auto        attrib_format    = get_format_from_gltf_type_format(attrib_accessor->type, attrib_accessor->component_type);
						auto        buffer_index     = find_safe_index(buffer_to_index, attrib_accessor->buffer_view->buffer);
						auto        attrib_byte_size = cgltf_calc_size(attrib_accessor->type, attrib_accessor->component_type);
						auto        stride           = attrib_accessor->buffer_view->stride;
						auto        offset           = attrib_accessor->buffer_view->offset + attrib_accessor->offset;

						if (stride == 0)
							stride = attrib_byte_size;

						if (attrib_accessor->normalized)
							ror::log_critical("Attribute accessor data is normalised but there is no support at the moment for normalised data");

						assert(buffer_index >= 0 && "Not a valid buffer index returned, possibly no buffer associated with this attribute");
						uint8_t *data_pointer = reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data());

						// Special consideration to weights, here we are normalizing them
						if (attrib.type == cgltf_attribute_type_weights)
						{
							assert(attrib.index >= 0 && attrib.index < 2 && "Attribute index out of bounds");
							offset     = 0;
							stride     = attrib_byte_size;
							auto index = static_cast<size_t>(attrib.index);

							if (attrib_format == rhi::Format::float32_4)
							{
								weights_float32_pointer[index] = unpack_normalized<float32_t>(attrib_accessor);
								data_pointer                   = reinterpret_cast<uint8_t *>(weights_float32_pointer[index].data());
							}
							else if (attrib_format == rhi::Format::uint16_4)
							{
								weights_uint16_pointer[index] = unpack_normalized<uint16_t>(attrib_accessor);
								data_pointer                  = reinterpret_cast<uint8_t *>(weights_uint16_pointer[index].data());
							}
							else if (attrib_format == rhi::Format::uint8_4)
							{
								weights_uint8_pointer[index] = unpack_normalized<uint8_t>(attrib_accessor);
								data_pointer                 = reinterpret_cast<uint8_t *>(weights_uint8_pointer[index].data());
							}
							else
							{
								assert(0 && "Shouldn't reach here, can't have any other type of weights");
							}
						}

						std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer + offset, attrib_accessor->count * attrib_byte_size, stride};
						attribs_data.emplace(current_index, std::move(data_tuple));

						vertex_attribute_descriptor.add(current_index, attrib_format, &a_buffers_pack);
					}

					// Read vertex indices buffer
					std::vector<uint16_t> index_data_pointer{};
					if (cprim.indices)
					{
						assert(cprim.indices->type == cgltf_type_scalar && "Indices are not the right type, only SCALAR indices supported");
						assert((cprim.indices->component_type == cgltf_component_type_r_32u || cprim.indices->component_type == cgltf_component_type_r_16u || cprim.indices->component_type == cgltf_component_type_r_8u) &&
						       "Indices are not in the right component type , only uint8_t, uint16_t and uint32_t supported");

						assert(cprim.indices->buffer_view && "Indices doesn't have a valid buffer view");
						// assert(cprim.indices->buffer_view->type == cgltf_buffer_view_type_indices && "Indices buffer view type is wrong"); type is always invalid, because no such thing in bufferView in glTF

						if (cprim.indices->buffer_view->has_meshopt_compression)
							ror::log_critical("Mesh has meshopt_compression but its not supported");

						mesh.has_indices(j, true);

						auto index_format = get_format_from_gltf_type_format(cprim.indices->type, cprim.indices->component_type);

						// TODO: GL expects stride to be zero if data is tightly packed
						assert(!cprim.indices->is_sparse && "Sparse index buffers not supported");

						auto attrib_accessor   = cprim.indices;
						auto buffer_index      = find_safe_index(buffer_to_index, cprim.indices->buffer_view->buffer);
						auto indices_byte_size = cgltf_calc_size(attrib_accessor->type, attrib_accessor->component_type);
						auto offset            = attrib_accessor->buffer_view->offset + attrib_accessor->offset;
						auto stride            = cprim.indices->stride;

						if (stride == 0)
							stride = indices_byte_size;

						if (attrib_accessor->normalized)
							ror::log_critical("Attribute Index data is normalised but there is no support at the moment for normalised data");

						assert(buffer_index >= 0 && "Not a valid buffer index returned, possibly no buffer associated with this index buffer");

						if (index_format == rhi::Format::uint8_1)
						{
							index_data_pointer = unpack_uint8_to_uint16(cprim.indices);
							indices_byte_size  = sizeof(uint16_t);
							index_format       = rhi::Format::uint16_1;
							offset             = 0;
							stride             = indices_byte_size;
						}
						uint8_t *data_pointer = (index_data_pointer.size() ? reinterpret_cast<uint8_t *>(index_data_pointer.data()) : reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data()));

						std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer + offset, attrib_accessor->count * indices_byte_size, stride};
						attribs_data.emplace(rhi::BufferSemantic::vertex_index, std::move(data_tuple));

						vertex_attribute_descriptor.add(rhi::BufferSemantic::vertex_index, index_format, &a_buffers_pack);
					}
					else
						mesh.has_indices(j, false);

					// Now upload data from all the attributes into vertex_attribute_descriptor
					vertex_attribute_descriptor.upload(attribs_data, &a_buffers_pack);

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
										auto &mesh_bbox = mesh.bounding_box(j);
										mesh_bbox.add_bounding(total_bbox);
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

							if (stride == 0)
								stride = attrib_byte_size;

							if (attrib_accessor->normalized)
								ror::log_critical("Attribute morph taget data is normalised but there is no support at the moment for normalised data");

							assert(buffer_index >= 0 && "Not a valid buffer index returned, possibly no buffer associated with this morph target attribute");
							uint8_t *data_pointer = reinterpret_cast<uint8_t *>(this->m_buffers[static_cast<size_t>(buffer_index)].data());

							std::tuple<uint8_t *, uint32_t, uint32_t> data_tuple{data_pointer + offset, attrib_accessor->count * attrib_byte_size, stride};

							morph_targets_attribs_data.emplace(current_index, std::move(data_tuple));

							target_vertex_descriptor.add(current_index, attrib_format, &a_buffers_pack);
						}

						// Now copy all the buffer_views for morph_targets
						target_vertex_descriptor.upload(morph_targets_attribs_data, &a_buffers_pack);
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
							auto location   = morph_attributes.back().location();
							location_offset = location + 1;
						}
					}

					// Save Morph target weights
					assert(cmesh.weights_count == cprim.targets_count && "Targets count and weights don't match data error");
					auto &mesh_weights = mesh.weights();
					mesh_weights.resize(cmesh.weights_count);

					// TODO: Do a bulk copy please once tested and it works
					for (size_t m = 0; m < cmesh.weights_count; ++m)
						mesh_weights[m] = cmesh.weights[m];

					// Add mesh primitive bounding box to the model bounding box
					this->m_bounding_box.add_bounding(mesh.bounding_box(j));
				}

				mesh_to_index.emplace(&cmesh, i);
			}

			// Read all the nodes
			this->m_nodes.resize(data->nodes_count);                  // Will fill the empty Node just created later
			this->m_nodes_side_data.resize(data->nodes_count);        // Will fill the empty Node side data just created later

			// This bit of code reorders these nodes by their parent, each parent is first in the index order
			// This is necessary for how I traverse the node graph, each parent is calculated first before its child
			std::vector<const cgltf_node *> temp_nodes;
			temp_nodes.reserve(data->nodes_count);
			auto node_in_children = [](const cgltf_node *a_present_node, const cgltf_node *a_current_node) {
				for (size_t ci = 0; ci < a_current_node->children_count; ++ci)
				{
					const cgltf_node *child = a_current_node->children[ci];
					if (child == a_present_node)
						return true;
				}
				return false;
			};

			auto find_suitable_index = [&node_in_children](size_t &a_new_index, std::vector<const cgltf_node *> &a_temp_nodes, const cgltf_node *a_cnode) {
				while (a_new_index < a_temp_nodes.size() && !node_in_children(a_temp_nodes[a_new_index], a_cnode))
				{
					a_new_index++;
				}
			};

			for (size_t i = 0; i < data->nodes_count; ++i)
			{
				const cgltf_node &cnode     = data->nodes[i];
				size_t            new_index = 0;
				find_suitable_index(new_index, temp_nodes, &cnode);
				temp_nodes.insert(std::next(temp_nodes.begin(), static_cast_safe<long>(new_index)), &cnode);
			}

			uint32_t indexx = 0;
			for (auto &temp : temp_nodes)
			{
				node_to_index.emplace(temp, indexx++);
			}

			// Read the skins
			this->m_skins.reserve(data->skins_count);
			for (size_t i = 0; i < data->skins_count; ++i)
			{
				const cgltf_skin &cskin = data->skins[i];
				assert(cskin.joints_count == cskin.inverse_bind_matrices->count && "Skin joints should be equal to bind matrices");

				Skin skin;
				if (cskin.skeleton)
					skin.root(find_safe_index(node_to_index, cskin.skeleton));

				// Important: skin.m_node_index is set when we read the scene in the next section

				if (cskin.inverse_bind_matrices)
				{
					assert(cskin.inverse_bind_matrices->count == cskin.joints_count && "Inverse bind matrix count must match joints count");
					assert(cskin.inverse_bind_matrices->type == cgltf_type_mat4 && "Inverse bind matrix must be 4x4 matrices");

					auto *inverse_bind_matrices_accessor = cskin.inverse_bind_matrices;
					auto &bind_matrices                  = skin.inverse_bind_matrices();
					bind_matrices.resize(cskin.inverse_bind_matrices->count);
					// TODO: Do this unpacking manually, there is a lot of overhead of doing it this way
					cgltf_accessor_unpack_floats(inverse_bind_matrices_accessor,
					                             reinterpret_cast<cgltf_float *>(bind_matrices.data()),
					                             bind_matrices.size() * 16);
				}

				auto &joints = skin.joints();
				joints.reserve(cskin.joints_count);
				for (size_t j = 0; j < cskin.joints_count; ++j)
				{
					auto ji = find_safe_index(node_to_index, cskin.joints[j]);
					assert(ji != -1 && "Can't find a joint that is suppose to be there");
					joints.emplace_back(static_cast_safe<uint16_t>(ji));
				}

				skin_to_index.emplace(&cskin, i);
				skin.fill_shader_buffers();
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

					assert(node_index.second == node.m_parent);
					assert(node.m_parent < ni && "Nodes are not in parent order");

					// Lets update Mesh skin_index as well
					if (node.m_mesh_index > -1)
						this->m_meshes[static_cast<size_t>(node.m_mesh_index)].skin_index(node.m_skin_index);

					if (cnode->name)
						node_side_data.m_name = cnode->name;

					if (node.m_skin_index != -1)
						this->m_skins[static_cast<size_t>(node.m_skin_index)].node_index(node_to_index[cnode]);        // Cast ok again, index not negative

					if (node.m_camera_index != -1)
					{
						node.m_camera_index = static_cast_safe<int32_t>(a_cameras.size());
						a_cameras.emplace_back(read_node_camera(cnode->camera));
					}

					if (cnode->has_translation)
						node.m_trs_transform.m_translation = ror::Vector3f(cnode->translation[0], cnode->translation[1], cnode->translation[2]);

					if (cnode->has_rotation)
						node.m_trs_transform.m_rotation = ror::Quaternionf(cnode->rotation[0], cnode->rotation[1], cnode->rotation[2], cnode->rotation[3]);

					if (cnode->has_scale)
					{
						node.m_trs_transform.m_scale = ror::Vector3f(cnode->scale[0], cnode->scale[1], cnode->scale[2]);

						if (cnode->scale[0] != cnode->scale[1] || cnode->scale[0] != cnode->scale[2] || cnode->scale[1] != cnode->scale[2])
							ror::log_critical("There is non-uniform scale in model {}, make sure the vertex shader normalisation code has no-uniform scale support", a_filename.c_str());
					}

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

					// lets see what windwing this node need
					ror::Matrix4f T = ror::matrix4_translation(node.m_trs_transform.translation());
					ror::Matrix4f R = ror::matrix4_rotation(node.m_trs_transform.rotation());
					ror::Matrix4f S = ror::matrix4_scaling(node.m_trs_transform.scale());

					auto M = T * R * S;
					auto d = M.determinant();

					if (d < 0)
						node.m_winding = rhi::PrimitiveWinding::clockwise;

					for (size_t k = 0; k < cnode->children_count; ++k)
					{
						assert(node_to_index.find(cnode->children[k]) != node_to_index.end() && "Children not found in the inserted nodes");

						node_stack.push(std::make_pair(cnode->children[k], node_to_index[cnode]));

						uint32_t child_index = static_cast<uint32_t>(node_to_index[cnode->children[k]]);

						assert(static_cast<uint32_t>(ni) < child_index && "Nodes are not in parent order");        // Static cast is ok because at this point ni is not negative

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

						assert(anim_sampler_accessor->has_min && anim_sampler_accessor->has_max && "Animation input sampler must provide min and max");

						animation_sampler.m_input.resize(anim_sampler_accessor->count);        // Don't need to multiply attrib_byte_size because m_input is float32_t
						animation_sampler.m_minimum.m_value = anim_sampler_accessor->min[0];
						animation_sampler.m_maximum.m_value = anim_sampler_accessor->max[0];
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
							ror::log_critical("This format isn't supported in node_transform.glsl.comp, add support there");
							// TODO: Don't cast to uint, use the provided precision
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
							                             animation_sampler.m_output.size() / sizeof(float32_t));
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
			this->update_hashes();

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

		// Pack textures when possible ORM[H] texture as well as other SpecGloss workflow textures
		pack_materials_textures(this->m_materials, this->m_textures, this->m_images);

		// Finally force update base color and emissive textures format to be sRGB according to the glTF spec
		update_materials_textures_to_linear(this->m_materials, this->m_textures, this->m_images);

#endif
		if (ror::settings().m_force_mipmapped_textures)
			for (auto &t : this->m_textures)
			{
				auto tih = t.texture_image();
				auto tsh = t.texture_sampler();

				assert(tih >= 0 && "Texture Image is invalid");
				assert(tsh >= 0 && "Texture Sampler is invalid");

				auto &ti = this->m_images[static_cast<size_t>(tih)];
				auto &ts = this->m_samplers[static_cast<size_t>(tsh)];

				force_mipmaps(ti, ts);
			}

		cgltf_free(data);
	}
}

void Model::upload(rhi::Device &a_device)
{
	// NOTE: Don't need to jobify all of these because its already in a job from scene
	for (auto &sampler : this->m_samplers)
		sampler.upload(a_device);

	for (auto &image : this->m_images)
		image.upload(a_device);

	for (auto &material : this->m_materials)
		material.upload(a_device);

	for (auto &skin : this->m_skins)
		skin.upload(a_device);

	for (auto &mesh : this->m_meshes)
		mesh.upload(a_device);
}

void Model::update_mesh_program(uint32_t a_mesh_index, uint32_t a_primitive_index, int32_t a_program_index)
{
	this->m_meshes[a_mesh_index].program(a_primitive_index, a_program_index);
}

ror::BoundingBoxf Model::bounding_box_scaled()
{
	ror::log_info("Recreating model bounding box");

	ror::BoundingBoxf bbox{};

	for (auto &node : this->m_nodes)
	{
		if (node.m_mesh_index != -1)
		{
			auto &mesh = this->m_meshes[static_cast<size_t>(node.m_mesh_index)];

			for (size_t prim_id = 0; prim_id < mesh.primitives_count(); ++prim_id)
			{
				auto &mesh_bound = mesh.bounding_box(prim_id);

				// TODO: These needs to be traversed to the root and skeletal/morph target animation bound adjusted to be correct
				auto T = node.m_trs_transform.translation();
				auto R = node.m_trs_transform.rotation();
				auto S = node.m_trs_transform.scale();

				auto min = T + (R * (S * mesh_bound.minimum()));
				auto max = T + (R * (S * mesh_bound.maximum()));

				bbox.add_point(min);
				bbox.add_point(max);
			}
		}
	}

	return bbox;
}

}        // namespace ror
