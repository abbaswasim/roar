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

#include "ray.hpp"

namespace ror_test
{
template <class _type>
void Ray3Test<_type>::SetUp()
{
	this->m_origin    = ror::Vector3<_type>(static_cast<_type>(3.0), static_cast<_type>(3.0), static_cast<_type>(0.0));
	this->m_direction = ror::Vector3<_type>(static_cast<_type>(27.0), static_cast<_type>(-12.0), static_cast<_type>(-14.0));

	this->m_normalized_direction = ror::Vector3<_type>(static_cast<_type>(0.82580012083053589), static_cast<_type>(-0.36702227592468262), static_cast<_type>(-0.42819264531135559));
}

template <class _type>
void Ray3Test<_type>::TearDown()
{}

TYPED_TEST(Ray3Test, testing_setup)
{
	ror::Ray<ror::Vector3<TypeParam>> tmp0{};
	ror::Ray<ror::Vector3<TypeParam>> tmp1(this->m_origin, this->m_direction);
	ror::Ray<ror::Vector3<TypeParam>> tmp2{};
	ror::Ray<ror::Vector3<TypeParam>> tmp3{};
	ror::Ray<ror::Vector3<TypeParam>> tmp4{};
	ror::Ray<ror::Vector3<TypeParam>> tmp5{};

	tmp2.set(this->m_origin, this->m_direction);

	tmp3.set_normalized(this->m_origin, this->m_normalized_direction);

	tmp4.set_origin(this->m_origin);
	tmp4.set_direction(this->m_direction);

	tmp5.set_origin(this->m_origin);
	tmp5.set_direction_normalized(this->m_normalized_direction);

	{
		auto o = tmp0.origin();
		auto d = tmp0.direction();

		auto origin    = ror::Vector3<TypeParam>(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));
		auto direction = ror::Vector3<TypeParam>(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));

		test_vector3_equal(o, origin);
		test_vector3_equal(d, direction);
	}

	{
		auto o = tmp1.origin();
		auto d = tmp1.direction();

		test_vector3_equal(o, this->m_origin);
		test_vector3_equal(d, this->m_normalized_direction);
	}

	{
		auto o = tmp2.origin();
		auto d = tmp2.direction();

		test_vector3_equal(o, this->m_origin);
		test_vector3_equal(d, this->m_normalized_direction);
	}

	{
		auto o = tmp3.origin();
		auto d = tmp3.direction();

		test_vector3_equal(o, this->m_origin);
		test_vector3_equal(d, this->m_normalized_direction);
	}

	{
		auto o = tmp4.origin();
		auto d = tmp4.direction();

		test_vector3_equal(o, this->m_origin);
		test_vector3_equal(d, this->m_normalized_direction);
	}

	{
		auto o = tmp5.origin();
		auto d = tmp5.direction();

		test_vector3_equal(o, this->m_origin);
		test_vector3_equal(d, this->m_normalized_direction);
	}
}

TYPED_TEST(Ray3Test, point_on_ray)
{
	ror::Ray<ror::Vector3<TypeParam>> tmp(this->m_origin, this->m_direction);

	auto at_0 = this->m_origin;
	auto at_1 = this->m_origin + this->m_normalized_direction;

	auto point0 = tmp.point_on_ray(0.0f);
	auto point1 = tmp.point_on_ray(1.0f);

	test_vector3_equal(point0, at_0);
	test_vector3_equal(point1, at_1);
}

TYPED_TEST(Ray3Test, distance_to_point_from_ray)
{
	ror::Ray<ror::Vector3<TypeParam>> tmp(this->m_origin, this->m_direction);

	auto A = ror::Vector3<TypeParam>(static_cast<TypeParam>(-4.71), static_cast<TypeParam>(9.79), static_cast<TypeParam>(-2.48));
	auto B = ror::Vector3<TypeParam>(static_cast<TypeParam>(15), static_cast<TypeParam>(5), static_cast<TypeParam>(-4.33332));

	float32_t dist_to_A = 10.568756103515625f;
	float32_t dist_to_B = 6.7151737213134766f;

	auto dis_a = tmp.distance_to_point(A);
	auto dis_b = tmp.distance_to_point(B);

	EXPECT_NEAR(static_cast<double64_t>(dis_a), static_cast<double64_t>(dist_to_A), ror::ror_epsilon);
	EXPECT_NEAR(static_cast<double64_t>(dis_b), static_cast<double64_t>(dist_to_B), ror::ror_epsilon);
}

TYPED_TEST(Ray3Test, closest_point_on_ray)
{
	auto A = ror::Vector3<TypeParam>(static_cast<TypeParam>(-4.71), static_cast<TypeParam>(9.79), static_cast<TypeParam>(-2.48));
	auto B = ror::Vector3<TypeParam>(static_cast<TypeParam>(15), static_cast<TypeParam>(5), static_cast<TypeParam>(-4.33332));
	ror::Ray<ror::Vector3<TypeParam>> tmp(this->m_origin, this->m_direction);

	ror::Vector3<TypeParam> closest_to_A = this->m_origin;
	ror::Vector3<TypeParam> closest_to_B = ror::Vector3<TypeParam>(static_cast<TypeParam>(12.109443664550781), static_cast<TypeParam>(-1.0486416816711426), static_cast<TypeParam>(-4.7234153747558594));

	ror::Vector3<TypeParam> close_to_A = tmp.closest_point_on_ray(A);
	ror::Vector3<TypeParam> close_to_B = tmp.closest_point_on_ray(B);

	test_vector3_equal(closest_to_A, close_to_A);
	test_vector3_equal(closest_to_B, close_to_B);
}
template <class _type>
void Ray2Test<_type>::SetUp()
{
	this->m_origin    = ror::Vector2<_type>(static_cast<_type>(3.0), static_cast<_type>(3.0));
	this->m_direction = ror::Vector2<_type>(static_cast<_type>(27.0), static_cast<_type>(-12.0));

	this->m_normalized_direction = ror::Vector2<_type>(static_cast<_type>(0.91381156444549561), static_cast<_type>(-0.40613844990730286));
}

template <class _type>
void Ray2Test<_type>::TearDown()
{}

TYPED_TEST(Ray2Test, testing_setup)
{
	ror::Ray<ror::Vector2<TypeParam>> tmp0{};
	ror::Ray<ror::Vector2<TypeParam>> tmp1(this->m_origin, this->m_direction);
	ror::Ray<ror::Vector2<TypeParam>> tmp2{};
	ror::Ray<ror::Vector2<TypeParam>> tmp3{};
	ror::Ray<ror::Vector2<TypeParam>> tmp4{};
	ror::Ray<ror::Vector2<TypeParam>> tmp5{};

	tmp2.set(this->m_origin, this->m_direction);

	tmp3.set_normalized(this->m_origin, this->m_normalized_direction);

	tmp4.set_origin(this->m_origin);
	tmp4.set_direction(this->m_direction);

	tmp5.set_origin(this->m_origin);
	tmp5.set_direction_normalized(this->m_normalized_direction);

	{
		auto o = tmp0.origin();
		auto d = tmp0.direction();

		auto origin    = ror::Vector2<TypeParam>(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));
		auto direction = ror::Vector2<TypeParam>(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));

		test_vector2_equal(o, origin);
		test_vector2_equal(d, direction);
	}

	{
		auto o = tmp1.origin();
		auto d = tmp1.direction();

		test_vector2_equal(o, this->m_origin);
		test_vector2_equal(d, this->m_normalized_direction);
	}

	{
		auto o = tmp2.origin();
		auto d = tmp2.direction();

		test_vector2_equal(o, this->m_origin);
		test_vector2_equal(d, this->m_normalized_direction);
	}

	{
		auto o = tmp3.origin();
		auto d = tmp3.direction();

		test_vector2_equal(o, this->m_origin);
		test_vector2_equal(d, this->m_normalized_direction);
	}

	{
		auto o = tmp4.origin();
		auto d = tmp4.direction();

		test_vector2_equal(o, this->m_origin);
		test_vector2_equal(d, this->m_normalized_direction);
	}

	{
		auto o = tmp5.origin();
		auto d = tmp5.direction();

		test_vector2_equal(o, this->m_origin);
		test_vector2_equal(d, this->m_normalized_direction);
	}
}

TYPED_TEST(Ray2Test, point_on_ray)
{
	ror::Ray<ror::Vector2<TypeParam>> tmp(this->m_origin, this->m_direction);

	auto at_0 = this->m_origin;
	auto at_1 = this->m_origin + this->m_normalized_direction;

	auto point0 = tmp.point_on_ray(0.0f);
	auto point1 = tmp.point_on_ray(1.0f);

	test_vector2_equal(point0, at_0);
	test_vector2_equal(point1, at_1);
}

TYPED_TEST(Ray2Test, distance_to_point_from_ray)
{
	ror::Ray<ror::Vector2<TypeParam>> tmp(this->m_origin, this->m_direction);

	auto A = ror::Vector2<TypeParam>(static_cast<TypeParam>(-4.71), static_cast<TypeParam>(9.79));
	auto B = ror::Vector2<TypeParam>(static_cast<TypeParam>(15), static_cast<TypeParam>(5));

	float32_t dist_to_A = 10.273665428161621f;
	float32_t dist_to_B = 6.7012844085693359f;

	auto dis_a = tmp.distance_to_point(A);
	auto dis_b = tmp.distance_to_point(B);

	EXPECT_NEAR(static_cast<double64_t>(dis_a), static_cast<double64_t>(dist_to_A), ror::ror_epsilon);
	EXPECT_NEAR(static_cast<double64_t>(dis_b), static_cast<double64_t>(dist_to_B), ror::ror_epsilon);
}

TYPED_TEST(Ray2Test, closest_point_on_ray)
{
	auto A = ror::Vector2<TypeParam>(static_cast<TypeParam>(-4.71), static_cast<TypeParam>(9.79));
	auto B = ror::Vector2<TypeParam>(static_cast<TypeParam>(15), static_cast<TypeParam>(5));
	ror::Ray<ror::Vector2<TypeParam>> tmp(this->m_origin, this->m_direction);

	ror::Vector2<TypeParam> closest_to_A = this->m_origin;
	ror::Vector2<TypeParam> closest_to_B = ror::Vector2<TypeParam>(static_cast<TypeParam>(12.278350515463918), static_cast<TypeParam>(-1.1237113402061851));

	ror::Vector2<TypeParam> close_to_A = tmp.closest_point_on_ray(A);
	ror::Vector2<TypeParam> close_to_B = tmp.closest_point_on_ray(B);

	test_vector2_equal(closest_to_A, close_to_A);
	test_vector2_equal(closest_to_B, close_to_B);
}

}        // namespace ror_test
