#include "bounds/rorbounding.hpp"
#include "common.hpp"
#include "configuration/rorsettings_configuration.hpp"
#include "foundation/rorcommon.hpp"
#include "foundation/rorcrtp.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"
#include "math/rorquaternion.hpp"
#include "math/rortransform.hpp"
#include "math/rorvector3.hpp"
#include "profiling/rorlog.hpp"
#include "profiling/rortimer.hpp"
#include <atomic>
#include <cstdint>
#include <filesystem>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "graphics/rormodel.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rorrender_data.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"
#include "shader_system/rorshader_system.hpp"

namespace ror_test
{
TEST(RoarGeneral, RoarGeneral_standard_types_atomic_is_lock_free)
{
	std::cout << ror::get_os_string() << std::endl;
	std::cout << ror::get_arch_string() << std::endl;
	std::cout << ror::get_build_string() << std::endl;
	std::cout << ror::get_compiler_string() << std::endl;

	std::atomic<bool>       bool_t_type;
	std::atomic<char16_t>   char16_t_type;
	std::atomic<char32_t>   char32_t_type;
	std::atomic<uchar8_t>   uchar8_t_type;
	std::atomic<uchar32_t>  uchar32_t_type;
	std::atomic<int8_t>     int8_t_type;
	std::atomic<int32_t>    int32_t_type;
	std::atomic<size_t>     size_t_type;
	std::atomic<uint8_t>    uint8_t_type;
	std::atomic<uint32_t>   uint32_t_type;
	std::atomic<double64_t> double64_t_type;
	std::atomic<intptr_t>   intptr_t_type;
	// std::atomic<double128_t> double128_t_type;
	std::atomic<float32_t> float32_t_type;
	std::atomic<uint64_t>  uint64_t_type;
	std::atomic<uint16_t>  uint16_t_type;
	std::atomic<int64_t>   int64_t_type;
	std::atomic<int16_t>   int16_t_type;
	std::atomic<uchar16_t> uchar16_t_type;
	std::atomic<char8_t>   char8_t_type;
	std::atomic<ptrdiff_t> ptrdiff_t_type;
	// std::atomic<hash_128_t>  hash128_t_type;

	EXPECT_TRUE(std::atomic_is_lock_free<bool>(&bool_t_type)) << "atomic bool_t is not lock_free";

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
	// EXPECT_TRUE(std::atomic_is_lock_free<double128_t>(&double128_t_type)) << "atomic double128_t is not lock_free";

	EXPECT_TRUE(std::atomic_is_lock_free<intptr_t>(&intptr_t_type)) << "atomic intptr_t is not lock_free";
	EXPECT_TRUE(std::atomic_is_lock_free<ptrdiff_t>(&ptrdiff_t_type)) << "atomic ptrdiff_t is not lock_free";

	// EXPECT_TRUE(std::atomic_is_lock_free<hash_128_t>(&hash128_t_type)) << "atomic hash128_t is not lock_free";
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
		auto   s   = map_hasher(str);
		size_t sum = s;
		{
			auto t = timer("loop test: ");

			for (size_t i = 0; i < count; ++i)
			{
				sum += map_hasher(str);
			}
		}
		std::cout << "sum = " << sum << std::endl;
	}
	{
		auto   s   = ror::hash_64(str.c_str(), str.size());
		size_t sum = s;
		{
			auto t = timer("loop test: ");

			for (size_t i = 0; i < count; ++i)
			{
				sum += ror::hash_64(str.c_str(), str.size());
			}
		}
		std::cout << "sum = " << sum << std::endl;
	}
	{
		auto   s   = ror::hash_32(str.c_str(), str.size());
		size_t sum = s;
		{
			auto t = timer("loop test: ");

			for (size_t i = 0; i < count; ++i)
			{
				sum += ror::hash_32(str.c_str(), str.size());
			}
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
		auto   s   = map_hasher(str);
		size_t sum = s;
		{
			auto t = timer("loop test: ");

			for (size_t i = 0; i < count; ++i)
			{
				sum += map_hasher(str);
			}
		}
		std::cout << "sum = " << sum << std::endl;
	}
	{
		auto   s   = ror::hash_64(&str, str_size);
		size_t sum = s;
		{
			auto t = timer("loop test: ");

			for (size_t i = 0; i < count; ++i)
			{
				sum += ror::hash_64(&str, str_size);
			}
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
	EXPECT_EQ(config.get<std::string>("buffers_format"), "buffers_format.json");
	EXPECT_EQ(config.get<std::string>("roar_cache"), ".roar_cache");
	EXPECT_EQ(config.get<std::string>("renderer_config"), "renderer.json");

	// Nested roar_args
	EXPECT_EQ(config.get<std::string>("roar_args:arg1"), "arguno");
	EXPECT_EQ(config.get<uint32_t>("roar_args:arg2"), 2);
	EXPECT_EQ(config.get<std::string>("roar_args:arg3"), "thresh");
	EXPECT_EQ(config.get<std::string>("roar_args:arg4"), "4");

	// Nested:Nest roar_args
	EXPECT_EQ(config.get<std::string>("roar_args:roar_args2:arg21"), "arg2uno");
	EXPECT_EQ(config.get<uint32_t>("roar_args:roar_args2:arg22"), 22);
	EXPECT_EQ(config.get<std::string>("roar_args:roar_args2:arg23"), "23");

	// Lets also test settings

	auto &sett = ror::settings();
	EXPECT_EQ(sett.m_unit, 1024);
	EXPECT_EQ(sett.m_visualise_mipmaps, false);
	EXPECT_EQ(sett.m_multisample_count, 8);
	EXPECT_EQ(sett.m_vertical_sync, false);
	EXPECT_EQ(sett.m_window_transparent, true);
	EXPECT_EQ(sett.m_window_premultiplied, false);
	EXPECT_EQ(sett.m_window_prerotated, false);
	EXPECT_FLOAT_EQ(sett.m_zoom_speed, 3.1400001049041748f);
	EXPECT_EQ(sett.m_buffers_format, "buffers_format.json");
	EXPECT_EQ(sett.m_roar_cache, ".roar_cache");
	EXPECT_EQ(sett.m_renderer_config, "renderer.json");
}

TEST(RoarGeneral, buffer_semantic_indices)
{
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_position), 0);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_texture_coord_0), 1);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_texture_coord_1), 2);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_texture_coord_2), 3);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_normal), 4);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_bent_normal), 5);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_tangent), 6);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_color_0), 7);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_color_1), 8);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_bone_id_0), 9);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_bone_id_1), 10);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_weight_0), 11);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_weight_1), 12);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_morph_target), 13);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_morph_weight), 14);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::vertex_index), 15);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::instance_translation), 16);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::instance_rotation), 17);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::instance_scale), 18);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::instance_transform), 19);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::mesh_index), 20);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::meshlet_data), 21);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::drawcall_data), 22);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::texture_image_data), 23);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::texture_sampler_data), 24);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::texture_data), 25);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::material_data), 26);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::mesh_data), 27);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::skin_data), 28);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::node_data), 29);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::node_suplementory_data), 30);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::animation_sampler_data), 31);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::animation_channel_data), 32);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::animation_input_data), 33);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::animation_output_data), 34);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::animation_data), 35);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::bounding_box_data), 36);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::index_data), 37);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::joint_index_data), 38);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::instance_trs), 39);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::gltf_bin_buffer), 40);
	EXPECT_EQ(rhi::semantic_to_index(rhi::BufferSemantic::custom), 41);
}

TEST(RoarGeneral, memory_layout)
{
	class C1
	{
	  public:
		double64_t m1;        //  8 bytes
		double64_t m2;        //  8 bytes
		bool       m3;        //  4 bytes + 4
	};                        //  8 bytes alignment

	EXPECT_EQ(ror::MemoryLayout<C1>::size(), 24);
	EXPECT_EQ(ror::MemoryLayout<C1>::stride(), ror::MemoryLayout<C1>::size());
	EXPECT_EQ(ror::MemoryLayout<C1>::alignment(), 8);

	class C2
	{
	  public:
		float m1;        //  4 bytes
		char  m2;        //  1 byte + 1 byte
		short m3;        //  2 bytes
		char  m4;        //  1 byte + 3 byte
	};                   //  4 bytes alignment

	EXPECT_EQ(ror::MemoryLayout<C2>::size(), 12);
	EXPECT_EQ(ror::MemoryLayout<C2>::stride(), ror::MemoryLayout<C2>::size());
	EXPECT_EQ(ror::MemoryLayout<C2>::alignment(), 4);

	class C3
	{
	  public:
		char  *m1;         //  8 byte
		int    m2;         //  4 bytes + 4
		char  *m3;         //  8 byte
		double m4;         //  8 bytes
		char  *m5;         //  8 bytes
		short  m6;         //  2 bytes + 6
		char  *m7;         //  8 byte
		int    m8;         //  4 bytes + 4
		char  *m9;         //  8 bytes
		char   m10;        //  1 byte + 7
	};                     //  8 bytes alignment

	EXPECT_EQ(ror::MemoryLayout<C3>::size(), 80);
	EXPECT_EQ(ror::MemoryLayout<C3>::stride(), ror::MemoryLayout<C3>::size());
	EXPECT_EQ(ror::MemoryLayout<C3>::alignment(), 8);

	class C4
	{
	  public:
		char  *m1;         //  8 byte
		char  *m3;         //  8 byte
		char  *m5;         //  8 bytes
		char  *m7;         //  8 byte
		char  *m9;         //  8 bytes
		double m4;         //  8 bytes
		int    m2;         //  4 bytes
		int    m8;         //  4 bytes
		short  m6;         //  2 bytes
		char   m10;        //  1 byte + 7
	};                     //  8 bytes alignment

	EXPECT_EQ(ror::MemoryLayout<C4>::size(), 64);
	EXPECT_EQ(ror::MemoryLayout<C4>::stride(), ror::MemoryLayout<C4>::size());
	EXPECT_EQ(ror::MemoryLayout<C4>::alignment(), 8);

	struct C5
	{
		char      m1;
		short int m2;
	};        // 4 bytes alignment

	EXPECT_EQ(ror::MemoryLayout<C5>::size(), 4);
	EXPECT_EQ(ror::MemoryLayout<C5>::stride(), ror::MemoryLayout<C5>::size());
	EXPECT_EQ(ror::MemoryLayout<C5>::alignment(), 2);

	struct C6
	{
		short int m1;
		char      m2;
		int       m3;
	};        // 4 bytes alignment

	EXPECT_EQ(ror::MemoryLayout<C6>::size(), 8);
	EXPECT_EQ(ror::MemoryLayout<C6>::stride(), ror::MemoryLayout<C6>::size());
	EXPECT_EQ(ror::MemoryLayout<C6>::alignment(), 4);

	// structure C
	struct C7
	{
		char   m1;
		double m2;
		int    m3;
	};        // 8 bytes alignment

	EXPECT_EQ(ror::MemoryLayout<C7>::size(), 24);
	EXPECT_EQ(ror::MemoryLayout<C7>::stride(), ror::MemoryLayout<C7>::size());
	EXPECT_EQ(ror::MemoryLayout<C7>::alignment(), 8);

	struct C8
	{
		double m1;
		int    m2;
		char   m3;
	};        // 8 bytes alignment

	EXPECT_EQ(ror::MemoryLayout<C8>::size(), 16);
	EXPECT_EQ(ror::MemoryLayout<C8>::stride(), ror::MemoryLayout<C8>::size());
	EXPECT_EQ(ror::MemoryLayout<C8>::alignment(), 8);

	{
		struct C9
		{
			double m1;         //  8 bytes
			C5     mc5;        //  2 bytes + 2
			int    m2;         //  4 bytes
			char   m3;         //  1 bytes + 7
		};                     //  8 bytes alignment

		EXPECT_EQ(ror::MemoryLayout<C9>::size(), 24);
		EXPECT_EQ(ror::MemoryLayout<C9>::stride(), ror::MemoryLayout<C9>::size());
		EXPECT_EQ(ror::MemoryLayout<C9>::alignment(), 8);
	}
	{
		struct C9
		{
			C5     mc5;        //  2 bytes + 6
			double m1;         //  8 bytes
			int    m2;         //  4 bytes
			char   m3;         //  1 bytes + 3
		};                     //  8 bytes alignment

		EXPECT_EQ(ror::MemoryLayout<C9>::size(), 24);
		EXPECT_EQ(ror::MemoryLayout<C9>::stride(), ror::MemoryLayout<C9>::size());
		EXPECT_EQ(ror::MemoryLayout<C9>::alignment(), 8);
	}
	{
		struct C9
		{
			char   m3;         //  1 bytes + 1
			C5     mc5;        //  2 bytes + 4
			double m1;         //  8 bytes
			int    m2;         //  4 bytes
		};                     //  8 bytes alignment

		EXPECT_EQ(ror::MemoryLayout<C9>::size(), 24);
		EXPECT_EQ(ror::MemoryLayout<C9>::stride(), ror::MemoryLayout<C9>::size());
		EXPECT_EQ(ror::MemoryLayout<C9>::alignment(), 8);
	}
	{
		struct C9
		{
			char   m3;         //  1 bytes + 7
			C8     mc8;        //  16 bytes
			double m1;         //  8 bytes
			int    m2;         //  4 bytes + 4
		};                     //  8 bytes alignment

		EXPECT_EQ(ror::MemoryLayout<C9>::size(), 40);
		EXPECT_EQ(ror::MemoryLayout<C9>::stride(), ror::MemoryLayout<C9>::size());
		EXPECT_EQ(ror::MemoryLayout<C9>::alignment(), 8);
	}
}

}        // namespace ror_test
