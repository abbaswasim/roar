// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2019
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

#include "utilities.hpp"
#include <sys/types.h>

namespace ror_test
{
TYPED_TEST(UtilitiesTest, functions_test)
{
	{
		float32_t v0 = 0.0f;
		float32_t v1 = 0.0001f;
		float32_t v2 = 0.0001f;
		float32_t v3 = 0.0002f;

		EXPECT_TRUE(ror::decimal_equal(v1, v2));
		EXPECT_FALSE(ror::decimal_equal(v1, v3));
		EXPECT_FALSE(ror::decimal_equal(v2, v3));

		EXPECT_TRUE(ror::equal_zero(v0));
		EXPECT_FALSE(ror::equal_zero(v1));
		EXPECT_FALSE(ror::equal_zero(v2));
		EXPECT_FALSE(ror::equal_zero(v3));

		EXPECT_TRUE(ror::equal_zero_copy<float32_t>(v0));
		EXPECT_FALSE(ror::equal_zero_copy<float32_t>(v1));
		EXPECT_FALSE(ror::equal_zero_copy<float32_t>(v2));
		EXPECT_FALSE(ror::equal_zero_copy<float32_t>(v3));
	}
	{
		double64_t v0 = 0.0;
		double64_t v1 = 0.0001;
		double64_t v2 = 0.0001;
		double64_t v3 = 0.0002;

		EXPECT_TRUE(ror::decimal_equal(v1, v2));
		EXPECT_FALSE(ror::decimal_equal(v1, v3));
		EXPECT_FALSE(ror::decimal_equal(v2, v3));

		EXPECT_TRUE(ror::equal_zero(v0));
		EXPECT_FALSE(ror::equal_zero(v1));
		EXPECT_FALSE(ror::equal_zero(v2));
		EXPECT_FALSE(ror::equal_zero(v3));

		EXPECT_TRUE(ror::equal_zero_copy<double64_t>(v0));
		EXPECT_FALSE(ror::equal_zero_copy<double64_t>(v1));
		EXPECT_FALSE(ror::equal_zero_copy<double64_t>(v2));
		EXPECT_FALSE(ror::equal_zero_copy<double64_t>(v3));
	}

	int min = -50;

	TypeParam min1 = (static_cast<TypeParam>(min) > static_cast<TypeParam>(0) ? static_cast<TypeParam>(0) : static_cast<TypeParam>(min));
	TypeParam max1 = static_cast<TypeParam>(100);

	TypeParam min2 = static_cast<TypeParam>(50);
	TypeParam max2 = static_cast<TypeParam>(100);

	float32_t v1_center      = (min1 < static_cast<TypeParam>(0) ? 25.0f : 50.0f);
	float32_t v1_twice       = (min1 < static_cast<TypeParam>(0) ? 250.0f : 200.0f);
	float32_t v1_twice_minus = (min1 < static_cast<TypeParam>(0) ? -200.0f : -100.0f);

	float32_t v2_center      = 75.0f;
	float32_t v2_twice       = 150.0f;
	float32_t v2_twice_minus = 0.0f;

	{
		auto iv0 = ror::interpolate(min1, max1, static_cast<float32_t>(0));
		auto iv1 = ror::interpolate(min1, max1, static_cast<float32_t>(1));
		auto iv2 = ror::interpolate(min1, max1, static_cast<float32_t>(0.5));
		auto iv3 = ror::interpolate(max1, min1, static_cast<float32_t>(0.5));
		auto iv4 = ror::interpolate(min1, max1, static_cast<float32_t>(2));
		auto iv5 = ror::interpolate(min1, max1, static_cast<float32_t>(-1));
		auto iv6 = ror::interpolate(max1, min1, static_cast<float32_t>(2));        // This one doesn't seem fully right, have a look later
		auto iv7 = ror::interpolate(max1, min1, static_cast<float32_t>(-1));

		EXPECT_DOUBLE_EQ(iv0, min1);
		EXPECT_DOUBLE_EQ(iv1, max1);

		EXPECT_DOUBLE_EQ(iv2, v1_center);
		EXPECT_DOUBLE_EQ(iv3, v1_center);

		EXPECT_DOUBLE_EQ(iv4, v1_twice);
		EXPECT_DOUBLE_EQ(iv6, v1_twice_minus);

		EXPECT_DOUBLE_EQ(iv5, v1_twice_minus);
		EXPECT_DOUBLE_EQ(iv7, v1_twice);
	}

	{
		auto iv0 = ror::interpolate(min2, max2, static_cast<float32_t>(0));
		auto iv1 = ror::interpolate(min2, max2, static_cast<float32_t>(1));
		auto iv2 = ror::interpolate(min2, max2, static_cast<float32_t>(0.5));
		auto iv3 = ror::interpolate(max2, min2, static_cast<float32_t>(0.5));
		auto iv4 = ror::interpolate(min2, max2, static_cast<float32_t>(2));
		auto iv5 = ror::interpolate(min2, max2, static_cast<float32_t>(-1));
		auto iv6 = ror::interpolate(max2, min2, static_cast<float32_t>(2));        // This one doesn't seem fully right, have a look later
		auto iv7 = ror::interpolate(max2, min2, static_cast<float32_t>(-1));

		EXPECT_DOUBLE_EQ(iv0, min2);
		EXPECT_DOUBLE_EQ(iv1, max2);

		EXPECT_DOUBLE_EQ(iv2, v2_center);
		EXPECT_DOUBLE_EQ(iv3, v2_center);

		EXPECT_DOUBLE_EQ(iv4, v2_twice);
		EXPECT_DOUBLE_EQ(iv6, v2_twice_minus);

		EXPECT_DOUBLE_EQ(iv5, v2_twice_minus);
		EXPECT_DOUBLE_EQ(iv7, v2_twice);
	}

	{
		auto iv0 = ror::interpolate_clamp(min1, max1, static_cast<float32_t>(0));
		auto iv1 = ror::interpolate_clamp(min1, max1, static_cast<float32_t>(1));
		auto iv2 = ror::interpolate_clamp(min1, max1, static_cast<float32_t>(0.5));
		auto iv3 = ror::interpolate_clamp(max1, min1, static_cast<float32_t>(0.5));
		auto iv4 = ror::interpolate_clamp(min1, max1, static_cast<float32_t>(2));
		auto iv5 = ror::interpolate_clamp(min1, max1, static_cast<float32_t>(-1));
		auto iv6 = ror::interpolate_clamp(max1, min1, static_cast<float32_t>(2));        // This one doesn't seem fully right, have a look later
		auto iv7 = ror::interpolate_clamp(max1, min1, static_cast<float32_t>(-1));

		EXPECT_DOUBLE_EQ(iv0, min1);
		EXPECT_DOUBLE_EQ(iv1, max1);

		EXPECT_DOUBLE_EQ(iv2, v1_center);
		EXPECT_DOUBLE_EQ(iv3, v1_center);

		EXPECT_DOUBLE_EQ(iv4, max1);
		EXPECT_DOUBLE_EQ(iv6, min1);

		EXPECT_DOUBLE_EQ(iv5, min1);
		EXPECT_DOUBLE_EQ(iv7, max1);
	}

	{
		auto iv0 = ror::interpolate_clamp(min2, max2, static_cast<float32_t>(0));
		auto iv1 = ror::interpolate_clamp(min2, max2, static_cast<float32_t>(1));
		auto iv2 = ror::interpolate_clamp(min2, max2, static_cast<float32_t>(0.5));
		auto iv3 = ror::interpolate_clamp(max2, min2, static_cast<float32_t>(0.5));
		auto iv4 = ror::interpolate_clamp(min2, max2, static_cast<float32_t>(2));
		auto iv5 = ror::interpolate_clamp(min2, max2, static_cast<float32_t>(-1));
		auto iv6 = ror::interpolate_clamp(max2, min2, static_cast<float32_t>(2));        // This one doesn't seem fully right, have a look later
		auto iv7 = ror::interpolate_clamp(max2, min2, static_cast<float32_t>(-1));

		EXPECT_DOUBLE_EQ(iv0, min2);
		EXPECT_DOUBLE_EQ(iv1, max2);

		EXPECT_DOUBLE_EQ(iv2, v2_center);
		EXPECT_DOUBLE_EQ(iv3, v2_center);

		EXPECT_DOUBLE_EQ(iv4, max2);
		EXPECT_DOUBLE_EQ(iv6, min2);

		EXPECT_DOUBLE_EQ(iv5, min2);
		EXPECT_DOUBLE_EQ(iv7, max2);
	}

	{
		float32_t v1_min_t         = 0.0f;
		float32_t v1_max_t         = 1.0f;
		float32_t v1_center_t      = 0.5f;
		float32_t v1_twice_t       = 2.0f;
		float32_t v1_twice_minus_t = -1.0;

		auto iv0 = ror::interpolate_inverse(min1, max1, min1);
		auto iv1 = ror::interpolate_inverse(min1, max1, max1);
		auto iv2 = ror::interpolate_inverse(min1, max1, static_cast<TypeParam>(v1_center));
		auto iv3 = ror::interpolate_inverse(max1, min1, static_cast<TypeParam>(v1_center));
		auto iv4 = ror::interpolate_inverse(min1, max1, static_cast<TypeParam>(v1_twice));
		auto iv5 = ror::interpolate_inverse(min1, max1, static_cast<TypeParam>(v1_twice_minus));
		auto iv6 = ror::interpolate_inverse(max1, min1, static_cast<TypeParam>(v1_twice));
		auto iv7 = ror::interpolate_inverse(max1, min1, static_cast<TypeParam>(v1_twice_minus));

		EXPECT_DOUBLE_EQ(iv0, v1_min_t);
		EXPECT_DOUBLE_EQ(iv1, v1_max_t);

		EXPECT_DOUBLE_EQ(iv2, v1_center_t);
		EXPECT_DOUBLE_EQ(iv3, v1_center_t);

		if (std::is_same<TypeParam, float32_t>::value ||
			std::is_same<TypeParam, double64_t>::value ||
			std::is_same<TypeParam, int32_t>::value ||
			std::is_same<TypeParam, int64_t>::value)
		{
			EXPECT_DOUBLE_EQ(iv4, v1_twice_t);
			EXPECT_DOUBLE_EQ(iv6, v1_twice_t);

			EXPECT_DOUBLE_EQ(iv5, v1_twice_minus_t);
			EXPECT_DOUBLE_EQ(iv7, v1_twice_minus_t);
		}
	}

	{
		float32_t v2_min_t         = 0.0f;
		float32_t v2_max_t         = 1.0f;
		float32_t v2_center_t      = 0.5f;
		float32_t v2_twice_t       = 2.0f;
		float32_t v2_twice_minus_t = -1.0;

		auto iv0 = ror::interpolate_inverse(min2, max2, min2);
		auto iv1 = ror::interpolate_inverse(min2, max2, max2);
		auto iv2 = ror::interpolate_inverse(min2, max2, static_cast<TypeParam>(v2_center));
		auto iv3 = ror::interpolate_inverse(max2, min2, static_cast<TypeParam>(v2_center));
		auto iv4 = ror::interpolate_inverse(min2, max2, static_cast<TypeParam>(v2_twice));
		auto iv5 = ror::interpolate_inverse(min2, max2, static_cast<TypeParam>(v2_twice_minus));
		auto iv6 = ror::interpolate_inverse(max2, min2, static_cast<TypeParam>(v2_twice));
		auto iv7 = ror::interpolate_inverse(max2, min2, static_cast<TypeParam>(v2_twice_minus));

		EXPECT_DOUBLE_EQ(iv0, v2_min_t);
		EXPECT_DOUBLE_EQ(iv1, v2_max_t);

		EXPECT_DOUBLE_EQ(iv2, v2_center_t);
		EXPECT_DOUBLE_EQ(iv3, v2_center_t);

		if (std::is_same<TypeParam, float32_t>::value ||
			std::is_same<TypeParam, double64_t>::value ||
			std::is_same<TypeParam, int32_t>::value ||
			std::is_same<TypeParam, int64_t>::value)
		{
			EXPECT_DOUBLE_EQ(iv4, v2_twice_t);
			EXPECT_DOUBLE_EQ(iv6, v2_twice_t);

			EXPECT_DOUBLE_EQ(iv5, v2_twice_minus_t);
			EXPECT_DOUBLE_EQ(iv7, v2_twice_minus_t);
		}
	}

	{
		float32_t v1_min_t         = 0.0f;
		float32_t v1_max_t         = 1.0f;
		float32_t v1_center_t      = 0.5f;
		float32_t v1_twice_t       = 1.0f;
		float32_t v1_twice_minus_t = 0.0;

		auto iv0 = ror::interpolate_inverse_clamp(min1, max1, min1);
		auto iv1 = ror::interpolate_inverse_clamp(min1, max1, max1);
		auto iv2 = ror::interpolate_inverse_clamp(min1, max1, static_cast<TypeParam>(v1_center));
		auto iv3 = ror::interpolate_inverse_clamp(max1, min1, static_cast<TypeParam>(v1_center));
		auto iv4 = ror::interpolate_inverse_clamp(min1, max1, static_cast<TypeParam>(v1_twice));
		auto iv5 = ror::interpolate_inverse_clamp(min1, max1, static_cast<TypeParam>(v1_twice_minus));
		auto iv6 = ror::interpolate_inverse_clamp(max1, min1, static_cast<TypeParam>(v1_twice));
		auto iv7 = ror::interpolate_inverse_clamp(max1, min1, static_cast<TypeParam>(v1_twice_minus));

		EXPECT_DOUBLE_EQ(iv0, v1_min_t);
		EXPECT_DOUBLE_EQ(iv1, v1_max_t);

		EXPECT_DOUBLE_EQ(iv2, v1_center_t);
		EXPECT_DOUBLE_EQ(iv3, v1_center_t);

		if (std::is_same<TypeParam, float32_t>::value ||
			std::is_same<TypeParam, double64_t>::value ||
			std::is_same<TypeParam, int32_t>::value ||
			std::is_same<TypeParam, int64_t>::value)
		{
			EXPECT_DOUBLE_EQ(iv4, v1_twice_t);
			EXPECT_DOUBLE_EQ(iv6, v1_twice_t);

			EXPECT_DOUBLE_EQ(iv5, v1_twice_minus_t);
			EXPECT_DOUBLE_EQ(iv7, v1_twice_minus_t);
		}
	}

	{
		float32_t v2_min_t         = 0.0f;
		float32_t v2_max_t         = 1.0f;
		float32_t v2_center_t      = 0.5f;
		float32_t v2_twice_t       = 1.0f;
		float32_t v2_twice_minus_t = 0.0;

		auto iv0 = ror::interpolate_inverse_clamp(min2, max2, min2);
		auto iv1 = ror::interpolate_inverse_clamp(min2, max2, max2);
		auto iv2 = ror::interpolate_inverse_clamp(min2, max2, static_cast<TypeParam>(v2_center));
		auto iv3 = ror::interpolate_inverse_clamp(max2, min2, static_cast<TypeParam>(v2_center));
		auto iv4 = ror::interpolate_inverse_clamp(min2, max2, static_cast<TypeParam>(v2_twice));
		auto iv5 = ror::interpolate_inverse_clamp(min2, max2, static_cast<TypeParam>(v2_twice_minus));
		auto iv6 = ror::interpolate_inverse_clamp(max2, min2, static_cast<TypeParam>(v2_twice));
		auto iv7 = ror::interpolate_inverse_clamp(max2, min2, static_cast<TypeParam>(v2_twice_minus));

		EXPECT_DOUBLE_EQ(iv0, v2_min_t);
		EXPECT_DOUBLE_EQ(iv1, v2_max_t);

		EXPECT_DOUBLE_EQ(iv2, v2_center_t);
		EXPECT_DOUBLE_EQ(iv3, v2_center_t);

		if (std::is_same<TypeParam, float32_t>::value ||
			std::is_same<TypeParam, double64_t>::value ||
			std::is_same<TypeParam, int32_t>::value ||
			std::is_same<TypeParam, int64_t>::value)
		{
			EXPECT_DOUBLE_EQ(iv4, v2_twice_t);
			EXPECT_DOUBLE_EQ(iv6, v2_twice_t);

			EXPECT_DOUBLE_EQ(iv5, v2_twice_minus_t);
			EXPECT_DOUBLE_EQ(iv7, v2_twice_minus_t);
		}
	}

	{
		auto iv0 = ror::remap(min1, max1, min2, max2, min1);
		auto iv1 = ror::remap(min1, max1, min2, max2, max1);
		auto iv2 = ror::remap(min1, max1, min2, max2, static_cast<TypeParam>(v1_center));
		auto iv3 = ror::remap(max1, min1, max2, min2, static_cast<TypeParam>(v1_center));
		auto iv4 = ror::remap(min1, max1, min2, max2, static_cast<TypeParam>(v1_twice));
		auto iv5 = ror::remap(min1, max1, min2, max2, static_cast<TypeParam>(v1_twice_minus));
		auto iv6 = ror::remap(max1, min1, max2, min2, static_cast<TypeParam>(v1_twice));        // This one doesn't seem fully right, have a look later
		auto iv7 = ror::remap(max1, min1, max2, min2, static_cast<TypeParam>(v1_twice_minus));

		EXPECT_DOUBLE_EQ(iv0, min2);
		EXPECT_DOUBLE_EQ(iv1, max2);

		EXPECT_DOUBLE_EQ(iv2, v2_center);
		EXPECT_DOUBLE_EQ(iv3, v2_center);

		if (std::is_same<TypeParam, float32_t>::value ||
			std::is_same<TypeParam, double64_t>::value ||
			std::is_same<TypeParam, int32_t>::value ||
			std::is_same<TypeParam, int64_t>::value)
		{
			EXPECT_DOUBLE_EQ(iv4, v2_twice);
			EXPECT_DOUBLE_EQ(iv6, v2_twice_minus);

			EXPECT_DOUBLE_EQ(iv5, v2_twice_minus);
			EXPECT_DOUBLE_EQ(iv7, v2_twice);
		}
	}

	{
		auto iv0 = ror::remap(min2, max2, min1, max1, min2);
		auto iv1 = ror::remap(min2, max2, min1, max1, max2);
		auto iv2 = ror::remap(min2, max2, min1, max1, static_cast<TypeParam>(v2_center));
		auto iv3 = ror::remap(max2, min2, max1, min1, static_cast<TypeParam>(v2_center));
		auto iv4 = ror::remap(min2, max2, min1, max1, static_cast<TypeParam>(v2_twice));
		auto iv5 = ror::remap(min2, max2, min1, max1, static_cast<TypeParam>(v2_twice_minus));
		auto iv6 = ror::remap(max2, min2, max1, min1, static_cast<TypeParam>(v2_twice));        // This one doesn't seem fully right, have a look later
		auto iv7 = ror::remap(max2, min2, max1, min1, static_cast<TypeParam>(v2_twice_minus));

		EXPECT_DOUBLE_EQ(iv0, min1);
		EXPECT_DOUBLE_EQ(iv1, max1);

		EXPECT_DOUBLE_EQ(iv2, v1_center);
		EXPECT_DOUBLE_EQ(iv3, v1_center);

		if (std::is_same<TypeParam, float32_t>::value ||
			std::is_same<TypeParam, double64_t>::value ||
			std::is_same<TypeParam, int32_t>::value ||
			std::is_same<TypeParam, int64_t>::value)
		{
			EXPECT_DOUBLE_EQ(iv4, v1_twice);
			EXPECT_DOUBLE_EQ(iv6, v1_twice_minus);

			EXPECT_DOUBLE_EQ(iv5, v1_twice_minus);
			EXPECT_DOUBLE_EQ(iv7, v1_twice);
		}
	}

	{
		auto iv0 = ror::remap_clamp(min1, max1, min2, max2, min1);
		auto iv1 = ror::remap_clamp(min1, max1, min2, max2, max1);
		auto iv2 = ror::remap_clamp(min1, max1, min2, max2, static_cast<TypeParam>(v1_center));
		auto iv3 = ror::remap_clamp(max1, min1, max2, min2, static_cast<TypeParam>(v1_center));
		auto iv4 = ror::remap_clamp(min1, max1, min2, max2, static_cast<TypeParam>(v1_twice));
		auto iv5 = ror::remap_clamp(min1, max1, min2, max2, static_cast<TypeParam>(v1_twice_minus));
		auto iv6 = ror::remap_clamp(max1, min1, max2, min2, static_cast<TypeParam>(v1_twice));        // This one doesn't seem fully right, have a look later
		auto iv7 = ror::remap_clamp(max1, min1, max2, min2, static_cast<TypeParam>(v1_twice_minus));

		EXPECT_DOUBLE_EQ(iv0, min2);
		EXPECT_DOUBLE_EQ(iv1, max2);

		EXPECT_DOUBLE_EQ(iv2, v2_center);
		EXPECT_DOUBLE_EQ(iv3, v2_center);

		if (std::is_same<TypeParam, float32_t>::value ||
			std::is_same<TypeParam, double64_t>::value ||
			std::is_same<TypeParam, int32_t>::value ||
			std::is_same<TypeParam, int64_t>::value)
		{
			EXPECT_DOUBLE_EQ(iv4, max2);
			EXPECT_DOUBLE_EQ(iv6, min2);

			EXPECT_DOUBLE_EQ(iv5, min2);
			EXPECT_DOUBLE_EQ(iv7, max2);
		}
	}

	{
		auto iv0 = ror::remap_clamp(min2, max2, min1, max1, min2);
		auto iv1 = ror::remap_clamp(min2, max2, min1, max1, max2);
		auto iv2 = ror::remap_clamp(min2, max2, min1, max1, static_cast<TypeParam>(v2_center));
		auto iv3 = ror::remap_clamp(max2, min2, max1, min1, static_cast<TypeParam>(v2_center));
		auto iv4 = ror::remap_clamp(min2, max2, min1, max1, static_cast<TypeParam>(v2_twice));
		auto iv5 = ror::remap_clamp(min2, max2, min1, max1, static_cast<TypeParam>(v2_twice_minus));
		auto iv6 = ror::remap_clamp(max2, min2, max1, min1, static_cast<TypeParam>(v2_twice));        // This one doesn't seem fully right, have a look later
		auto iv7 = ror::remap_clamp(max2, min2, max1, min1, static_cast<TypeParam>(v2_twice_minus));

		EXPECT_DOUBLE_EQ(iv0, min1);
		EXPECT_DOUBLE_EQ(iv1, max1);

		EXPECT_DOUBLE_EQ(iv2, v1_center);
		EXPECT_DOUBLE_EQ(iv3, v1_center);

		if (std::is_same<TypeParam, float32_t>::value ||
			std::is_same<TypeParam, double64_t>::value ||
			std::is_same<TypeParam, int32_t>::value ||
			std::is_same<TypeParam, int64_t>::value)
		{
			EXPECT_DOUBLE_EQ(iv4, max1);
			EXPECT_DOUBLE_EQ(iv6, min1);

			EXPECT_DOUBLE_EQ(iv5, min1);
			EXPECT_DOUBLE_EQ(iv7, max1);
		}
	}

	{
		auto r0 = ror::random(min1, max1);
		ASSERT_TRUE(r0 >= min1 && r0 <= max1);

		auto r1 = ror::random(min2, max2);
		ASSERT_TRUE(r1 >= min2 && r1 <= max2);

		auto r2 = ror::random(max1, min1);
		ASSERT_TRUE(r2 >= min1 && r2 <= max1);

		auto r3 = ror::random(max2, min2);
		ASSERT_TRUE(r3 >= min2 && r3 <= max2);
	}

	{
		TypeParam center      = (min1 < static_cast<TypeParam>(0) ? static_cast<TypeParam>(25.0f) : static_cast<TypeParam>(50.0f));

		auto c0 = ror::closest(static_cast<TypeParam>(center + 1), min1, max1);
		ASSERT_DOUBLE_EQ(c0, max1);

		auto c1 = ror::closest(static_cast<TypeParam>(center - 1), min1, max1);
		ASSERT_DOUBLE_EQ(c1, min1);

		auto c2 = ror::closest(center, min2, max2);
		ASSERT_DOUBLE_EQ(c2, min2);

		auto c3 = ror::closest(static_cast<TypeParam>(center - 1), min2, max2);
		ASSERT_DOUBLE_EQ(c3, min2);

		auto c4 = ror::closest(static_cast<TypeParam>(center + 1), min2, max2);
		ASSERT_DOUBLE_EQ(c4, min2);
	}

	{
		uint32_t v = 125;
		auto p = ror::power_of_two(v);
		EXPECT_EQ(p, 128);
	}
	{
		uint32_t v = 256;
		auto p = ror::power_of_two(v);
		EXPECT_EQ(p, 256);
	}
	{
		uint32_t v = 257;
		auto p = ror::power_of_two(v);
		EXPECT_EQ(p, 512);
	}
}

}        // namespace ror_test
