#include "bounds/rorbounding.hpp"
#include "foundation/rorcommon.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rortypes.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"
#include "math/rorquaternion.hpp"
#include "math/rortransform.hpp"
#include "math/rorvector3.hpp"
#include "profiling/rortimer.hpp"
#include <atomic>
#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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

	uint32_t count = 1000000;
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

	uint32_t count = 10000000;
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
{
	// std::cout << " sizeof Vec3 = " << sizeof(ror::Vector3f);
	// std::cout << " sizeof BoundingBox = " << sizeof(ror::BoundingBoxf);
	// std::cout << " Max vertex attributes = " << ror::max_vertex_attributes << std::endl;
	// std::cout << " sizeof Bufferview = " << sizeof(rhi::BufferView) << std::endl;
	// std::cout << " sizeof Drawable = " << sizeof(ror::Mesh::Drawable) << std::endl;
	// std::cout << " sizeof Mesh " << sizeof(ror::Mesh) << std::endl;
	// std::cout << " sizeof std::vector " << sizeof(std::vector<ror::Mesh>) << std::endl;

	// std::cout << " sizeof transform " << sizeof(ror::Transformf) << std::endl;
	// std::cout << " sizeof quat " << sizeof(ror::Quaternionf) << std::endl;
	// std::cout << " sizeof scale/trans " << sizeof(ror::Vector3f) << std::endl;

	std::cout << " sizeof Material " << sizeof(ror::Material) << std::endl;
	std::cout << " sizeof Material::Comp " << sizeof(ror::Material::MaterialComponent<ror::Color3f>) << std::endl;

}
}        // namespace ror_test
