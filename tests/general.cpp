#include "bounds/rorbounding.hpp"
#include "common.hpp"
#include "configuration/rorsettings_configuration.hpp"
#include "foundation/rorcommon.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rortypes.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"
#include "math/rorquaternion.hpp"
#include "math/rortransform.hpp"
#include "math/rorvector3.hpp"
#include "profiling/rorlog.hpp"
#include "profiling/rortimer.hpp"
#include <atomic>
#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "fox.h"
#include "graphics/rormodel.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rortypes.hpp"

namespace ror_test
{
TEST(RoarGeneral, RoarGeneral_standard_types_atomic_is_lock_free)
{
	std::cout << ror::get_os_string() << std::endl;
	std::cout << ror::get_arch_string() << std::endl;
	std::cout << ror::get_build_string() << std::endl;
	std::cout << ror::get_compiler_string() << std::endl;

	std::atomic<char16_t>    char16_t_type;
	std::atomic<char32_t>    char32_t_type;
	std::atomic<uchar8_t>    uchar8_t_type;
	std::atomic<uchar32_t>   uchar32_t_type;
	std::atomic<int8_t>      int8_t_type;
	std::atomic<int32_t>     int32_t_type;
	std::atomic<size_t>      size_t_type;
	std::atomic<uint8_t>     uint8_t_type;
	std::atomic<uint32_t>    uint32_t_type;
	std::atomic<double64_t>  double64_t_type;
	std::atomic<intptr_t>    intptr_t_type;
	std::atomic<double128_t> double128_t_type;
	std::atomic<float32_t>   float32_t_type;
	std::atomic<uint64_t>    uint64_t_type;
	std::atomic<uint16_t>    uint16_t_type;
	std::atomic<int64_t>     int64_t_type;
	std::atomic<int16_t>     int16_t_type;
	std::atomic<uchar16_t>   uchar16_t_type;
	std::atomic<char8_t>     char8_t_type;
	std::atomic<ptrdiff_t>   ptrdiff_t_type;
	std::atomic<hash_128_t>  hash128_t_type;

	EXPECT_TRUE(std::atomic_is_lock_free<char8_t>(&char8_t_type)) << "atomic char8_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<char16_t>(&char16_t_type)) << "atomic char16_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<char32_t>(&char32_t_type)) << "atomic char32_t is not lock_free";

	EXPECT_TRUE(std::atomic_is_lock_free<uchar8_t>(&uchar8_t_type)) << "atomic uchar8_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<uchar16_t>(&uchar16_t_type)) << "atomic uchar16_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<uchar32_t>(&uchar32_t_type)) << "atomic uchar32_t is not lock_free";

	EXPECT_TRUE(std::atomic_is_lock_free<int8_t>(&int8_t_type)) << "atomic int8_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<int16_t>(&int16_t_type)) << "atomic int16_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<int32_t>(&int32_t_type)) << "atomic int32_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<int64_t>(&int64_t_type)) << "atomic int64_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<size_t>(&size_t_type)) << "atomic size_t is not lock_free";

	EXPECT_TRUE(std::atomic_is_lock_free<uint8_t>(&uint8_t_type)) << "atomic uint8_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<uint16_t>(&uint16_t_type)) << "atomic uint16_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<uint32_t>(&uint32_t_type)) << "atomic uint32_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<uint64_t>(&uint64_t_type)) << "atomic uint64_t is not lock_free";

	EXPECT_TRUE(std::atomic_is_lock_free<float32_t>(&float32_t_type)) << "atomic float32_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<double64_t>(&double64_t_type)) << "atomic double64_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<double128_t>(&double128_t_type)) << "atomic double128_t is not lock_free";

	EXPECT_TRUE(std::atomic_is_lock_free<intptr_t>(&intptr_t_type)) << "atomic intptr_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<ptrdiff_t>(&ptrdiff_t_type)) << "atomic ptrdiff_t is not lock_free";

	EXPECT_TRUE(std::atomic_is_lock_free<hash_128_t>(&hash128_t_type)) << "atomic hash128_t is not lock_free";
}

struct timer
{
	std::string                                    m_name;
	std::chrono::high_resolution_clock::time_point start;

	timer(const std::string &name) :
		m_name(name), start(std::chrono::high_resolution_clock::now())
	{}

	~timer()
	{
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << m_name << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;
	}
};

TEST(RoarGeneral, DISABLED_RoarGeneral_hash_performance_string)
{
	using str_map = std::unordered_map<std::string, std::string>;

	str_map map;

	str_map::hasher map_hasher = map.hash_function();

	uint32_t    count = 1000000;
	std::string str{"atomic intptr_t is not lock_free"};

	{
		auto s = map_hasher(str);
		auto t = timer("loop test: ");

		size_t sum = s;
		for (size_t i = 0; i < count; ++i)
		{
			sum += map_hasher(str);
		}
		std::cout << "sum = " << sum << std::endl;
	}
	{
		auto s = ror::hash_64(str.c_str(), str.size());
		auto t = timer("loop test: ");

		size_t sum = s;
		for (size_t i = 0; i < count; ++i)
		{
			sum += ror::hash_64(str.c_str(), str.size());
		}
		std::cout << "sum = " << sum << std::endl;
	}
	{
		auto s = ror::hash_32(str.c_str(), str.size());
		auto t = timer("loop test: ");

		size_t sum = s;
		for (size_t i = 0; i < count; ++i)
		{
			sum += ror::hash_32(str.c_str(), str.size());
		}
		std::cout << "sum = " << sum << std::endl;
	}
}

TEST(RoarGeneral, DISABLED_RoarGeneral_hash_performance_size_t)
{
	using str_map = std::unordered_map<std::size_t, std::string>;

	str_map map;

	str_map::hasher map_hasher = map.hash_function();

	uint32_t    count = 10000000;
	std::size_t str{987654321034567890};
	std::size_t str_size{sizeof(str)};

	{
		auto s = map_hasher(str);
		auto t = timer("loop test: ");

		size_t sum = s;
		for (size_t i = 0; i < count; ++i)
		{
			sum += map_hasher(str);
		}
		std::cout << "sum = " << sum << std::endl;
	}
	{
		auto s = ror::hash_64(&str, str_size);
		auto t = timer("loop test: ");

		size_t sum = s;
		for (size_t i = 0; i < count; ++i)
		{
			sum += ror::hash_64(&str, str_size);
		}
		std::cout << "sum = " << sum << std::endl;
	}
}

TEST(RoarGeneral, size_of_stuff)
{}

TEST(RoarGeneral, settings_test)
{
	auto &config = ror::get_settings();

	EXPECT_EQ(config.get<uint32_t>("unit"), 1024);
	EXPECT_EQ(config.get<bool>("visualise_mipmaps"), false);
	EXPECT_EQ(config.get<uint32_t>("multisample_count"), 8);
	EXPECT_EQ(config.get<bool>("vsync"), false);
	EXPECT_EQ(config.get<bool>("window_transparent"), true);
	EXPECT_EQ(config.get<bool>("window_premultiplied"), false);
	EXPECT_EQ(config.get<bool>("window_prerotated"), false);
	EXPECT_FLOAT_EQ(config.get<float32_t>("zoom_speed"), 3.1400001049041748f);
}

TEST(ModelTest, gltf_loader_test)
{
	ror::Model model;
	model.load_from_gltf_file("Fox/Fox.gltf");

	EXPECT_EQ(model.meshes().size(), 1);
	auto &m = model.meshes()[0];
	EXPECT_EQ(m.m_attribute_vertex_descriptors.size(), 1);
	EXPECT_EQ(m.m_morph_targets_vertex_descriptors.size(), 1);
	EXPECT_EQ(m.m_has_indices_states[0], false);
	EXPECT_EQ(m.m_primitive_types[0], rhi::PrimitiveTopology::triangles);

	auto &vd = m.m_attribute_vertex_descriptors[0];

	auto semantics = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0) |
					 ror::enum_to_type_cast(rhi::BufferSemantic::vertex_bone_id_0) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_weight_0);

	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4 + sizeof(uint16_t) * 4, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 1, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_bone_id_0, 2, sizeof(float32_t) * 3, 0, 2, 0, rhi::VertexFormat::uint16_4, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4 + sizeof(uint16_t) * 4, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, sizeof(float32_t) * 3 + sizeof(uint16_t) * 4, 0, 3, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4 + sizeof(uint16_t) * 4, 1, 1, semantics, __LINE__);

	{
		auto &attrib_pos = m.m_attribute_vertex_descriptors[0].attribute(rhi::BufferSemantic::vertex_position);
		EXPECT_EQ(attrib_pos.location(), 0);
		EXPECT_EQ(attrib_pos.offset(), 0);
		EXPECT_EQ(attrib_pos.buffer_offset(), 0);
		EXPECT_EQ(attrib_pos.binding(), 0);
		EXPECT_EQ(attrib_pos.buffer_index(), 0);
		EXPECT_EQ(attrib_pos.semantics(), rhi::BufferSemantic::vertex_position);
		EXPECT_EQ(attrib_pos.format(), rhi::VertexFormat::float32_3);
		EXPECT_EQ(attrib_pos.complete(), true);
	}
	{
		auto &attrib_uv = m.m_attribute_vertex_descriptors[0].attribute(rhi::BufferSemantic::vertex_texture_coord_0);
		EXPECT_EQ(attrib_uv.location(), 1);
		EXPECT_EQ(attrib_uv.offset(), 0);
		EXPECT_EQ(attrib_uv.buffer_offset(), 0);
		EXPECT_EQ(attrib_uv.binding(), 1);
		EXPECT_EQ(attrib_uv.buffer_index(), 1);
		EXPECT_EQ(attrib_uv.semantics(), rhi::BufferSemantic::vertex_texture_coord_0);
		EXPECT_EQ(attrib_uv.format(), rhi::VertexFormat::float32_2);
		EXPECT_EQ(attrib_uv.complete(), true);
	}
	{
		auto &attrib_bone = m.m_attribute_vertex_descriptors[0].attribute(rhi::BufferSemantic::vertex_bone_id_0);
		EXPECT_EQ(attrib_bone.location(), 2);
		EXPECT_EQ(attrib_bone.offset(), sizeof(float32_t) * 3);
		EXPECT_EQ(attrib_bone.buffer_offset(), 0);
		EXPECT_EQ(attrib_bone.binding(), 2);
		EXPECT_EQ(attrib_bone.buffer_index(), 0);
		EXPECT_EQ(attrib_bone.semantics(), rhi::BufferSemantic::vertex_bone_id_0);
		EXPECT_EQ(attrib_bone.format(), rhi::VertexFormat::uint16_4);
		EXPECT_EQ(attrib_bone.complete(), true);
	}
	{
		auto &attrib_weight = m.m_attribute_vertex_descriptors[0].attribute(rhi::BufferSemantic::vertex_weight_0);
		EXPECT_EQ(attrib_weight.location(), 3);
		EXPECT_EQ(attrib_weight.offset(), sizeof(float32_t) * 3 + sizeof(uint16_t) * 4);
		EXPECT_EQ(attrib_weight.buffer_offset(), 0);
		EXPECT_EQ(attrib_weight.binding(), 3);
		EXPECT_EQ(attrib_weight.buffer_index(), 0);
		EXPECT_EQ(attrib_weight.semantics(), rhi::BufferSemantic::vertex_weight_0);
		EXPECT_EQ(attrib_weight.format(), rhi::VertexFormat::float32_4);
		EXPECT_EQ(attrib_weight.complete(), true);
	}

	// Now lets see if the attributes data is copied correctly or not
	auto &bp = rhi::get_buffers_pack();

	const uint8_t *pos_buff    = bp.buffer(rhi::BufferSemantic::vertex_position).data().data();
	const uint8_t *uv_buff     = bp.buffer(rhi::BufferSemantic::vertex_texture_coord_0).data().data();
	const uint8_t *bone_buff   = bp.buffer(rhi::BufferSemantic::vertex_bone_id_0).data().data();
	const uint8_t *weight_buff = bp.buffer(rhi::BufferSemantic::vertex_weight_0).data().data();

	const float epsilon = 0.000001f;
	{
		auto  semantic = rhi::BufferSemantic::vertex_position;
		auto &attrib   = m.m_attribute_vertex_descriptors[0].attribute(semantic);
		auto &layout   = m.m_attribute_vertex_descriptors[0].layout(semantic);

		uint32_t comp_count = 3;
		uint8_t *ptr        = const_cast<uint8_t *>(pos_buff) + attrib.buffer_offset() + attrib.offset();        // const_cast only allowed in tests
		for (size_t i = 0; i < fox_attrib_count; ++i)
		{
			for (size_t j = 0; j < comp_count; ++j)
			{
				EXPECT_NEAR(fox_positions[i * comp_count + j], reinterpret_cast<float32_t *>(ptr)[j], epsilon);
			}

			ptr += layout.stride();
		}
	}
	{
		auto     semantic   = rhi::BufferSemantic::vertex_texture_coord_0;
		auto    &attrib     = m.m_attribute_vertex_descriptors[0].attribute(semantic);
		auto    &layout     = m.m_attribute_vertex_descriptors[0].layout(semantic);
		uint32_t comp_count = 2;
		uint8_t *ptr        = const_cast<uint8_t *>(uv_buff) + attrib.buffer_offset() + attrib.offset();        // const_cast only allowed in tests
		for (size_t i = 0; i < fox_attrib_count; ++i)
		{
			for (size_t j = 0; j < comp_count; ++j)
			{
				EXPECT_NEAR(fox_uvs[i * comp_count + j], reinterpret_cast<float32_t *>(ptr)[j], epsilon);
			}

			ptr += layout.stride();
		}
	}
	{
		auto     semantic   = rhi::BufferSemantic::vertex_bone_id_0;
		auto    &attrib     = m.m_attribute_vertex_descriptors[0].attribute(semantic);
		auto    &layout     = m.m_attribute_vertex_descriptors[0].layout(semantic);
		uint32_t comp_count = 4;
		uint8_t *ptr        = const_cast<uint8_t *>(bone_buff) + attrib.buffer_offset() + attrib.offset();        // const_cast only allowed in tests
		for (size_t i = 0; i < fox_attrib_count; ++i)
		{
			for (size_t j = 0; j < comp_count; ++j)
			{
				EXPECT_EQ(fox_joint_ids[i * comp_count + j], reinterpret_cast<uint16_t *>(ptr)[j]);
			}

			ptr += layout.stride();
		}
	}
	{
		auto     semantic   = rhi::BufferSemantic::vertex_weight_0;
		auto    &attrib     = m.m_attribute_vertex_descriptors[0].attribute(semantic);
		auto    &layout     = m.m_attribute_vertex_descriptors[0].layout(semantic);
		uint32_t comp_count = 4;
		uint8_t *ptr        = const_cast<uint8_t *>(weight_buff) + attrib.buffer_offset() + attrib.offset();        // const_cast only allowed in tests
		for (size_t i = 0; i < fox_attrib_count; ++i)
		{
			for (size_t j = 0; j < comp_count; ++j)
			{
				EXPECT_NEAR(fox_weights[i * comp_count + j], reinterpret_cast<float32_t *>(ptr)[j], epsilon);
			}

			ptr += layout.stride();
		}
	}

	// Signal free to BufferPack so allocator can free stuff
	bp.free();
}

}        // namespace ror_test
