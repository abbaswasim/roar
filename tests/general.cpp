#include "foundation/rorcommon.hpp"
#include "foundation/rortypes.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <atomic>

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

	// EXPECT_TRUE(atomic_is_lock_free < k_free&(&a)) << "Not enough threads available";
}

}        // namespace ror_test
