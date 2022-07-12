#include "bounds/rorbounding.hpp"
#include "common.hpp"
#include "configuration/rorsettings_configuration.hpp"
#include "foundation/rorcommon.hpp"
#include "foundation/rorcrtp.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
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

#include "fox.h"
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

	std::atomic<bool>        bool_t_type;
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
	// std::atomic<double128_t> double128_t_type;
	std::atomic<float32_t>   float32_t_type;
	std::atomic<uint64_t>    uint64_t_type;
	std::atomic<uint16_t>    uint16_t_type;
	std::atomic<int64_t>     int64_t_type;
	std::atomic<int16_t>     int16_t_type;
	std::atomic<uchar16_t>   uchar16_t_type;
	std::atomic<char8_t>     char8_t_type;
	std::atomic<ptrdiff_t>   ptrdiff_t_type;
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

	// Nested roar_args
	EXPECT_EQ(config.get<std::string>("roar_args:arg1"), "arguno");
	EXPECT_EQ(config.get<uint32_t>("roar_args:arg2"), 2);
	EXPECT_EQ(config.get<std::string>("roar_args:arg3"), "thresh");
	EXPECT_EQ(config.get<std::string>("roar_args:arg4"), "4");

	// Nested:Nest roar_args
	EXPECT_EQ(config.get<std::string>("roar_args:roar_args2:arg21"), "arg2uno");
	EXPECT_EQ(config.get<uint32_t>("roar_args:roar_args2:arg22"), 22);
	EXPECT_EQ(config.get<std::string>("roar_args:roar_args2:arg23"), "23");
}

}        // namespace ror_test
