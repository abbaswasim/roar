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

#include "vector3.hpp"

namespace ror_test
{
template <class _type>
void Vector3Test<_type>::SetUp()
{
	this->m_scalar  = static_cast<_type>(4.0);
	this->m_a       = ror::Vector3<_type>(static_cast<_type>(3.0), static_cast<_type>(3.0), static_cast<_type>(0.0));
	this->m_b       = ror::Vector3<_type>(static_cast<_type>(27.0), static_cast<_type>(-12.0), static_cast<_type>(-14.0));
	this->m_sum_ab  = ror::Vector3<_type>(static_cast<_type>(30.0), static_cast<_type>(-9.0), static_cast<_type>(-14.0));
	this->m_diff_ab = ror::Vector3<_type>(static_cast<_type>(-24.0), static_cast<_type>(15.0), static_cast<_type>(14.0));
	this->m_mul_ab  = ror::Vector3<_type>(static_cast<_type>(81.0), static_cast<_type>(-36.0), static_cast<_type>(0.0));
	this->m_div_ab  = ror::Vector3<_type>(static_cast<_type>(0.1111111112), static_cast<_type>(-0.25), static_cast<_type>(0.0));

	this->m_sum_ac  = ror::Vector3<_type>(static_cast<_type>(7.0), static_cast<_type>(7.0), static_cast<_type>(4.0));
	this->m_diff_ac = ror::Vector3<_type>(static_cast<_type>(-1.0), static_cast<_type>(-1.0), static_cast<_type>(-4.0));
	this->m_mul_ac  = ror::Vector3<_type>(static_cast<_type>(12.0), static_cast<_type>(12.0), static_cast<_type>(0.0));
	this->m_div_ac  = ror::Vector3<_type>(static_cast<_type>(0.75), static_cast<_type>(0.75), static_cast<_type>(0.0));

	this->m_normalized_a = ror::Vector3<typename ror::Vector3<_type>::precision>(static_cast<typename ror::Vector3<_type>::precision>(0.7071068286895752), static_cast<typename ror::Vector3<_type>::precision>(0.7071068286895752), static_cast<typename ror::Vector3<_type>::precision>(0));
	this->m_normalized_b = ror::Vector3<typename ror::Vector3<_type>::precision>(static_cast<typename ror::Vector3<_type>::precision>(0.82580012083053589), static_cast<typename ror::Vector3<_type>::precision>(-0.36702227592468262), static_cast<typename ror::Vector3<_type>::precision>(-0.42819264531135559));
	this->m_inverse_a    = ror::Vector3<_type>(static_cast<_type>(0.333333334), static_cast<_type>(0.333333334), static_cast<_type>(0.0));
	this->m_inverse_b    = ror::Vector3<_type>(static_cast<_type>(0.037037037312984467), static_cast<_type>(-0.083333334), static_cast<_type>(-0.071428574621677399));
	this->m_cross_ab     = ror::Vector3<_type>(static_cast<_type>(-42.0), static_cast<_type>(42.0), static_cast<_type>(-117.0));

	this->m_dot_ab      = static_cast<_type>(45.0);
	this->m_min_a       = static_cast<_type>(0.0);
	this->m_max_a       = static_cast<_type>(3.0);
	this->m_min_b       = static_cast<_type>(-14.0);
	this->m_max_b       = static_cast<_type>(27.0);
	this->m_length_a    = static_cast<_type>(4.2426406871192848);
	this->m_length_b    = static_cast<_type>(32.695565448543633);
	this->m_length_sq_a = static_cast<_type>(18);
	this->m_length_sq_b = static_cast<_type>(1069);
}

template <class _type>
void Vector3TestSigned<_type>::SetUp()
{
	Vector3Test<_type>::SetUp();
}

template <class _type>
void Vector3Test<_type>::TearDown()
{}

template <class _type>
void Vector3TestSigned<_type>::TearDown()
{
	Vector3Test<_type>::TearDown();
}

TYPED_TEST(Vector3Test, testing_setup)
{
	TypeParam x = static_cast<TypeParam>(std::rand());
	TypeParam y = static_cast<TypeParam>(std::rand());
	TypeParam z = static_cast<TypeParam>(std::rand());

	ror::Vector3<TypeParam> tmp0(static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0));
	ror::Vector3<TypeParam> tmp1(x, y, z);
	ror::Vector3<TypeParam> tmp2 = tmp1;
	ror::Vector3<TypeParam> tmp3(y, z, x);
	ror::Vector3<TypeParam> tmp4(x);
	ror::Vector3<TypeParam> tmp5(x, x, x);
	ror::Vector3<TypeParam> tmp6;
	ror::Vector3<TypeParam> tmp7;
	ror::Vector3<TypeParam> tmp8;
	ror::Vector3<TypeParam> tmp9;
	ror::Vector3<TypeParam> tmp10(static_cast<TypeParam>(20), static_cast<TypeParam>(40), static_cast<TypeParam>(60));
	ror::Vector3<TypeParam> tmp11 = tmp10;

	tmp6.set(x, y, z);
	tmp7.set(x);
	tmp8.set(tmp6);
	tmp9.zero();

	test_vector3_equal_seperate(tmp1, x, y, z);

	EXPECT_DOUBLE_EQ(static_cast<double64_t>(tmp1[0]), static_cast<double64_t>(x));
	EXPECT_DOUBLE_EQ(static_cast<double64_t>(tmp1[1]), static_cast<double64_t>(y));
	EXPECT_DOUBLE_EQ(static_cast<double64_t>(tmp1[2]), static_cast<double64_t>(z));

	test_vector3_equal(tmp1, tmp2);
	test_vector3_not_equal(tmp2, tmp3);
	test_vector3_equal(tmp1, tmp6);
	test_vector3_equal(tmp8, tmp6);
	test_vector3_equal(tmp7, tmp4);
	test_vector3_equal(tmp5, tmp4);
	test_vector3_equal(tmp9, tmp0);

	ror::Vector3<TypeParam> tmp12 = tmp1;
	test_vector3_equal(tmp12, tmp1);

	tmp12[0] = y;
	tmp12[1] = z;
	tmp12[2] = x;

	test_vector3_equal_seperate(tmp12, y, z, x);

	TypeParam l = static_cast<TypeParam>(30);
	TypeParam m = static_cast<TypeParam>(40);
	TypeParam r = static_cast<TypeParam>(50);

	tmp11.clamp(l, r);
	test_vector3_equal_seperate(tmp11, l, m, r);

	TypeParam to[3];

	tmp1.to(to);
	tmp3.from(to);

	test_vector3_equal_seperate(tmp1, to[0], to[1], to[2]);
	test_vector3_equal(tmp1, tmp2);
}

TYPED_TEST(Vector3TestSigned, testing_linear_algebra)
{
	ror::Vector3<TypeParam> sum_ab   = this->m_a + this->m_b;
	ror::Vector3<TypeParam> diff_ab  = this->m_a - this->m_b;
	ror::Vector3<TypeParam> mul_ab   = this->m_a * this->m_b;
	ror::Vector3<TypeParam> div_ab   = this->m_a / this->m_b;
	ror::Vector3<TypeParam> sum_ac   = this->m_a + this->m_scalar;
	ror::Vector3<TypeParam> diff_ac  = this->m_a - this->m_scalar;
	ror::Vector3<TypeParam> mul_ac   = this->m_a * this->m_scalar;
	ror::Vector3<TypeParam> div_ac   = this->m_a / this->m_scalar;
	ror::Vector3<TypeParam> cross_ab = this->m_a.cross_product(this->m_b);

	ror::Vector3<typename ror::Vector3<TypeParam>::precision> normalize_a = this->m_a.normalized();
	ror::Vector3<typename ror::Vector3<TypeParam>::precision> normalize_b = this->m_b.normalized();

	ror::Vector3<TypeParam> inverse_a = this->m_a.inversed();
	ror::Vector3<TypeParam> inverse_b = this->m_b.inversed();

	typename ror::Vector3<TypeParam>::precision dot_ab   = this->m_a.dot_product(this->m_b);
	typename ror::Vector3<TypeParam>::precision length_a = this->m_a.length();
	typename ror::Vector3<TypeParam>::precision length_b = this->m_b.length();

	TypeParam min_a       = this->m_a.minimum();
	TypeParam max_a       = this->m_a.maximum();
	TypeParam min_b       = this->m_b.minimum();
	TypeParam max_b       = this->m_b.maximum();
	TypeParam length_sq_a = this->m_a.length_squared();
	TypeParam length_sq_b = this->m_b.length_squared();

	test_vector3_equal(this->m_sum_ab, sum_ab);
	test_vector3_equal(this->m_diff_ab, diff_ab);
	test_vector3_equal(this->m_mul_ab, mul_ab);
	test_vector3_equal(this->m_div_ab, div_ab);
	test_vector3_equal(this->m_sum_ac, sum_ac);
	test_vector3_equal(this->m_diff_ac, diff_ac);
	test_vector3_equal(this->m_mul_ac, mul_ac);
	test_vector3_equal(this->m_div_ac, div_ac);
	test_vector3_equal(this->m_cross_ab, cross_ab);

	test_vector3_equal(this->m_normalized_a, normalize_a);
	test_vector3_equal(this->m_normalized_b, normalize_b);

	test_vector3_equal(this->m_inverse_a, inverse_a);
	test_vector3_equal(this->m_inverse_b, inverse_b);

	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(this->m_dot_ab), static_cast<TypeParam>(dot_ab));

	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(this->m_min_a), static_cast<TypeParam>(min_a));
	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(this->m_max_a), static_cast<TypeParam>(max_a));
	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(this->m_min_b), static_cast<TypeParam>(min_b));
	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(this->m_max_b), static_cast<TypeParam>(max_b));

	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(this->m_length_a), static_cast<TypeParam>(length_a));
	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(this->m_length_b), static_cast<TypeParam>(length_b));

	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(this->m_length_sq_a), static_cast<TypeParam>(length_sq_a));
	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(this->m_length_sq_b), static_cast<TypeParam>(length_sq_b));

	this->m_a += this->m_b;
	test_vector3_equal(this->m_sum_ab, this->m_a);

	if (this->m_a != this->m_sum_ab)
	{
		FAIL();
	}

	if (this->m_a == this->m_b)
	{
		FAIL();
	}

	this->m_a -= this->m_b;

	this->m_sum_ab -= this->m_b;
	test_vector3_equal(this->m_sum_ab, this->m_a);

	this->m_a *= this->m_b;
	test_vector3_equal(this->m_mul_ab, this->m_a);
	this->m_a /= this->m_b;

	this->m_a += this->m_scalar;
	test_vector3_equal(this->m_sum_ac, this->m_a);
	this->m_a -= this->m_scalar;

	this->m_sum_ac -= this->m_scalar;
	test_vector3_equal(this->m_sum_ac, this->m_a);

	this->m_a *= this->m_scalar;
	test_vector3_equal(this->m_mul_ac, this->m_a);
	this->m_a /= this->m_scalar;

	this->m_a.normalize();
	this->m_b.normalize();

	ror::Vector3<TypeParam> normalized_a(static_cast<TypeParam>(0.7071068286895752), static_cast<TypeParam>(0.7071068286895752), static_cast<TypeParam>(0));
	ror::Vector3<TypeParam> normalized_b(static_cast<TypeParam>(0.82580012083053589), static_cast<TypeParam>(-0.36702227592468262), static_cast<TypeParam>(-0.42819264531135559));

	test_vector3_equal(normalized_a, this->m_a);
	test_vector3_equal(normalized_b, this->m_b);
}

TYPED_TEST(Vector3TestSigned, testing_global_methods)
{
	auto dis       = distance(this->m_a, this->m_b);
	auto dis_sq    = distance_squared(this->m_a, this->m_b);
	auto angl      = angle(this->m_a, this->m_b);
	auto scal_proj = scalar_projection(this->m_a, this->m_b);
	auto vec_proj  = vector_projection(this->m_a, this->m_b);
	auto vec_rejec = vector_rejection(this->m_a, this->m_b);
	auto lerp_a    = vector_lerp(this->m_a, this->m_b, 0.0f);
	auto lerp_b    = vector_lerp(this->m_a, this->m_b, 1.0f);
	auto lerp_ab   = vector_lerp(this->m_a, this->m_b, 0.5f);
	auto reflect_a = vector_reflect(-this->m_a, static_cast<ror::Vector3<TypeParam>>(ror::yaxis3f));
	auto reflect_b = vector_reflect(-this->m_b, static_cast<ror::Vector3<TypeParam>>(ror::yaxis3f));

	ror::Vector3<float32_t> vec_proj_res(1.1365762395f, -0.5051449953f, -0.5893358279f);
	ror::Vector3<float32_t> vec_rej_res(1.8634238243103027, 3.5051450729370117, 0.58933579921722412);
	ror::Vector3<float32_t> lerp_ab_res(15.0, -4.5f, -7.0f);
	ror::Vector3<float32_t> a_reflect_yaxis(-3.0f, 3.0f, 0.0f);
	ror::Vector3<float32_t> b_reflect_yaxis(-27.0f, -12.0f, 14.0f);

	EXPECT_NEAR(dis, static_cast<decltype(dis)>(31.5753), test_epsilon);
	EXPECT_NEAR(dis_sq, static_cast<decltype(dis)>(997.0), test_epsilon);
	EXPECT_NEAR(angl, static_cast<decltype(angl)>(1.24041378819899), test_epsilon);
	EXPECT_NEAR(scal_proj, static_cast<float32_t>(1.376333438), test_epsilon);

	test_vector3_equal(vec_proj, vec_proj_res);
	test_vector3_equal(vec_rejec, vec_rej_res);

	test_vector3_equal(this->m_a, lerp_a);
	test_vector3_equal(this->m_b, lerp_b);

	test_vector3_equal(lerp_ab, lerp_ab_res);

	test_vector3_equal(reflect_a, a_reflect_yaxis);
	test_vector3_equal(reflect_b, b_reflect_yaxis);

	{
		auto reflect_axix = vector_reflect(-static_cast<ror::Vector3<TypeParam>>(ror::xaxis3f), static_cast<ror::Vector3<TypeParam>>(ror::yaxis3f));

		ror::Vector3<float32_t> axis_reflect_yaxis(-1.0f, 0.0f, 0.0f);
		test_vector3_equal(reflect_axix, axis_reflect_yaxis);
	}

	{
		auto reflect_axix = vector_reflect(-static_cast<ror::Vector3<TypeParam>>(ror::zaxis3f), static_cast<ror::Vector3<TypeParam>>(ror::yaxis3f));

		ror::Vector3<float32_t> axis_reflect_yaxis(0.0f, 0.0f, -1.0f);
		test_vector3_equal(reflect_axix, axis_reflect_yaxis);
	}

	{
		auto reflect_axix = vector_reflect(-static_cast<ror::Vector3<TypeParam>>(ror::yaxis3f), static_cast<ror::Vector3<TypeParam>>(ror::xaxis3f));

		ror::Vector3<float32_t> axis_reflect_xaxis(0.0f, -1.0f, 0.0f);
		test_vector3_equal(reflect_axix, axis_reflect_xaxis);
	}

	{
		auto ang = angle(ror::Vector3<TypeParam>(1, 0, 0), ror::Vector3<TypeParam>(0, 1, 0));
		EXPECT_NEAR(ang, 1.5707963267949f, test_epsilon);
		EXPECT_NEAR(ang, static_cast<decltype(angl)>(ror::ror_half_pi), test_epsilon);
	}
	{
		auto ang = angle(ror::Vector3<TypeParam>(1, 0, 0), ror::Vector3<TypeParam>(0, 0, 1));
		EXPECT_NEAR(ang, static_cast<decltype(angl)>(ror::ror_half_pi), test_epsilon);
	}
	{
		auto ang = angle(ror::Vector3<TypeParam>(0, 1, 0), ror::Vector3<TypeParam>(0, 0, 1));
		EXPECT_NEAR(ang, static_cast<decltype(angl)>(ror::ror_half_pi), test_epsilon);
	}
	{
		auto ang = angle(ror::Vector3<TypeParam>(0, 1, 0), ror::Vector3<TypeParam>(1, 0, 0));
		EXPECT_NEAR(ang, static_cast<decltype(angl)>(ror::ror_half_pi), test_epsilon);
	}
	{
		auto ang = angle(ror::Vector3<TypeParam>(1, 0, 0), ror::Vector3<TypeParam>(-1, 0, 0));
		EXPECT_NEAR(ang, static_cast<decltype(angl)>(ror::ror_pi), test_epsilon);
	}
	{
		auto ang = angle(ror::Vector3<TypeParam>(0, 1, 0), ror::Vector3<TypeParam>(0, -1, 0));
		EXPECT_NEAR(ang, static_cast<decltype(angl)>(ror::ror_pi), test_epsilon);
	}
	{
		auto ang = angle(ror::Vector3<TypeParam>(0, 0, 1), ror::Vector3<TypeParam>(0, 0, -1));
		EXPECT_NEAR(ang, static_cast<decltype(angl)>(ror::ror_pi), test_epsilon);
	}
	{
		auto ang = angle(ror::Vector3<TypeParam>(1, 1, 1), ror::Vector3<TypeParam>(-1, -1, -1));
		EXPECT_NEAR(ang, static_cast<decltype(angl)>(ror::ror_pi), test_epsilon);
	}
	{
		auto ang = angle(ror::Vector3<TypeParam>(-1, -1, -1), ror::Vector3<TypeParam>(-1, -1, -1));
		EXPECT_NEAR(ang, static_cast<decltype(angl)>(0.0), test_epsilon);
	}
	{
		auto ang = angle(ror::Vector3<TypeParam>(1, 1, 1), ror::Vector3<TypeParam>(1, 1, 1));
		EXPECT_NEAR(ang, static_cast<decltype(angl)>(0.0), test_epsilon);
	}

	ror::Vector3<TypeParam> v(2, 4, 6);

	static_assert(sizeof(ror::Vector3f) == 12, "ror::Vector3f is not tightly packed into 12 bytes");
	static_assert(sizeof(v) == 3 * sizeof(TypeParam), "ror::Vector3f is not tightly packed into 12 bytes");
}

TYPED_TEST(Vector3TestSigned, testing_min_max_clamp)
{
	ror::Vector3<TypeParam> a{2, 3, 4};
	ror::Vector3<TypeParam> b{12, 13, 14};

	ror::Vector3<TypeParam> a1{-2, 3, -4};
	ror::Vector3<TypeParam> b1{-12, -13, -14};

	ror::Vector3<TypeParam> c1{2, 3, -4};
	ror::Vector3<TypeParam> c2{2, -13, -14};

	// Min/Max funcs
	{
		ror::Vector3<TypeParam> min = a1;
		ror::Vector3<TypeParam> max = a;

		auto min_res = ror::vector_minimum(a, a1);
		auto max_res = ror::vector_maximum(a, a1);

		test_vector3_equal(min_res, min);
		test_vector3_equal(max_res, max);
	}

	{
		ror::Vector3<TypeParam> min{-12, -13, -14};
		ror::Vector3<TypeParam> max{-2, 3, -4};

		auto min_res = ror::vector_minimum(a1, b1);
		auto max_res = ror::vector_maximum(a1, b1);

		test_vector3_equal(min_res, min);
		test_vector3_equal(max_res, max);
	}

	// Clamp funcs
	{
		ror::Vector3<TypeParam> clampc1{2, 3, -4};
		ror::Vector3<TypeParam> clampc2{2, 3, -4};

		auto clamp_res = ror::vector_clamp(c1, a1, a);
		test_vector3_equal(clamp_res, clampc1);

		clamp_res = ror::vector_clamp(c2, a1, a);
		test_vector3_equal(clamp_res, clampc2);

		clamp_res = ror::vector_clamp_safe(c1, a, a1);
		test_vector3_equal(clamp_res, clampc1);

		clamp_res = ror::vector_clamp_safe(c2, a, a1);
		test_vector3_equal(clamp_res, clampc2);
	}

	// Select funcs
	{
		auto select_res = ror::vector_select(ror::Vector3<uint32_t>(1, 1, 1), a, a1);
		test_vector3_equal(select_res, a);

		select_res = ror::vector_select(ror::Vector3<uint32_t>(0, 0, 0), a, a1);
		test_vector3_equal(select_res, a1);

		ror::Vector3<TypeParam> ab1{2, -13, 4};

		select_res = ror::vector_select(ror::Vector3<uint32_t>(1, 0, 1), a, b1);
		test_vector3_equal(select_res, ab1);
	}
}
}        // namespace ror_test
