#include "carray_vs_stdarray.hpp"
#include "foundation/rortypes.hpp"
#include "math/rormatrix.hpp"
#include "profiling/rortimer.hpp"
#include <gtest/gtest.h>

#include <array>
#include <iostream>

using namespace std;

namespace ror_test
{
TEST(ArrayTest, DISABLED_ArrayTest_carray_vs_stdarray)
{
	const uint32_t max_size = 100000;

	double64_t carray_time   = 0.0;
	double64_t stdarray_time = 0.0;

	{
		ror::Timer t;

		size_t int_array[max_size];
		size_t total = 0;

		for (size_t j = 0; j < 100; ++j)
		{
			for (size_t i = 1; i < max_size; ++i)
			{
				int_array[i] = i + int_array[size_t(i) - 1];
			}

			for (size_t i = 0; i < max_size; ++i)
			{
				total += int_array[i];
			}
		}

		carray_time = static_cast<double64_t>(t.tick()) / 1000000.0;
	}
	{
		ror::Timer t;

		std::array<size_t, max_size> int_array;
		size_t                       total = 0;

		for (size_t j = 0; j < 100; ++j)
		{
			for (size_t i = 1; i < max_size; ++i)
			{
				int_array[i] = i + int_array[size_t(i) - 1];
			}

			for (size_t i = 0; i < max_size; ++i)
			{
				total += int_array[i];
			}
		}

		stdarray_time = static_cast<double64_t>(t.tick()) / 1000000.0;
	}

	if (ror::get_build() == ror::BuildType::build_release)
		EXPECT_GT(carray_time, stdarray_time) << "std::array is faster than c array on this platform";
	else
		EXPECT_LT(carray_time, stdarray_time) << "std::array is faster than c array on this platform";
}

inline float32_t clamp(const float32_t &v, const float32_t &min, const float32_t &max)
{
	return (v > max ? max : v < min ? min : v);
}

TEST(SDTClamp, DISABLED_STDClamp_vs_manual)
{
	const uint32_t max_size = 100000;

	double64_t std_time = 0.0;
	double64_t my_time  = 0.0;

	std::array<float32_t, 100> values = {{0.0f}};

	for (size_t j = 0; j < 100; ++j)
	{
		values[j] = static_cast<float32_t>(std::rand());
	}

	{
		ror::Timer t;

		float32_t total = 0;

		for (size_t j = 0; j < 100; ++j)
		{
			for (size_t i = 1; i < max_size; ++i)
			{
				total += std::clamp(values[j], 100.0f, 1000.0f);
			}
		}

		std_time = static_cast<double64_t>(t.tick()) / 1000000.0;
	}
	{
		ror::Timer t;

		float32_t total = 0;

		for (size_t j = 0; j < 100; ++j)
		{
			for (size_t i = 1; i < max_size; ++i)
			{
				total += clamp(values[j], 100.0f, 1000.0f);
			}
		}

		my_time = static_cast<double64_t>(t.tick()) / 1000000.0;
	}

	EXPECT_LT(my_time, std_time) << "my clamp is slower than std::clamp on this platform";
}

}        // namespace ror_test
