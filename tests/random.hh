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

#include "random.hpp"

namespace ror_test
{
#define BETWEEN_MIN_MAX()                                  \
	for (size_t i = 0; i < 2000; ++i)                      \
	{                                                      \
		TypeParam next_rand = rands.next();                \
		ASSERT_TRUE(next_rand >= min && next_rand <= max); \
	}

TYPED_TEST(RandomTest, constructors_work)
{
	{
		TypeParam min = 10;
		TypeParam max = 15;

		ror::Random<TypeParam> rands(min, max);
	}
	{
		TypeParam min = 10;

		ror::Random<TypeParam> rands(min, min);

		for (size_t i = 0; i < 2000; ++i)
		{
			TypeParam next_rand = rands.next();
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(next_rand), static_cast<double64_t>(min));
		}
	}
	{
		TypeParam min = 10;
		TypeParam max = 15;

		ror::Random<TypeParam> rands(min, max);
		BETWEEN_MIN_MAX();

		min += 20;
		max += 20;

		rands.reset(min, max);
		BETWEEN_MIN_MAX();

		min = 1;
		max = 5;

		rands.reset(min, max);
		BETWEEN_MIN_MAX();
	}
}

TYPED_TEST(RandomTestSigned, constructors_work)
{
	{
		TypeParam min = -10;
		TypeParam max = 10;

		ror::Random<TypeParam> rands(min, max);
		BETWEEN_MIN_MAX();
	}
	{
		TypeParam min = -10;
		TypeParam max = -5;

		ror::Random<TypeParam> rands(min, max);
		BETWEEN_MIN_MAX();
	}
}

}        // namespace ror_test
