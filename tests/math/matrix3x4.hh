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

#include "matrix3x4.hpp"

namespace ror_test
{
template <class TypeParam>
void Matrix3x4Test<TypeParam>::SetUp()
{
	this->m_c = static_cast<TypeParam>(3.0);
	this->m_a = ror::Matrix3x4<TypeParam>(static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
										  static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7),
										  static_cast<TypeParam>(9), static_cast<TypeParam>(10), static_cast<TypeParam>(11),
										  static_cast<TypeParam>(13), static_cast<TypeParam>(14), static_cast<TypeParam>(15));

	this->m_b = ror::Matrix3x4<TypeParam>(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(6),
										  static_cast<TypeParam>(10), static_cast<TypeParam>(12), static_cast<TypeParam>(14),
										  static_cast<TypeParam>(18), static_cast<TypeParam>(20), static_cast<TypeParam>(22),
										  static_cast<TypeParam>(26), static_cast<TypeParam>(28), static_cast<TypeParam>(30));
}

template <class TypeParam>
void Matrix3x4TestSigned<TypeParam>::SetUp()
{
	Matrix3x4Test<TypeParam>::SetUp();
}

template <class TypeParam>
void Matrix3x4Test<TypeParam>::TearDown()
{}

template <class TypeParam>
void Matrix3x4TestSigned<TypeParam>::TearDown()
{
	Matrix3x4Test<TypeParam>::TearDown();
}

TYPED_TEST(Matrix3x4Test, constructors)
{
	ror::Matrix3x4<TypeParam> d;
	ror::Matrix3x4<TypeParam> f(static_cast<TypeParam>(4));
	ror::Matrix3x4<TypeParam> g(this->m_a);

	ror::Vector3<TypeParam> ori{static_cast<TypeParam>(13), static_cast<TypeParam>(14), static_cast<TypeParam>(15)};

	ror::Matrix3<TypeParam> mat3{static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
								 static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7),
								 static_cast<TypeParam>(9), static_cast<TypeParam>(10), static_cast<TypeParam>(11)};

	ror::Matrix3x4<TypeParam> mat3_res{static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
									   static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7),
									   static_cast<TypeParam>(9), static_cast<TypeParam>(10), static_cast<TypeParam>(11),
									   static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0)};

	ror::Matrix3x4<TypeParam> mat3_ori_res{static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
										   static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7),
										   static_cast<TypeParam>(9), static_cast<TypeParam>(10), static_cast<TypeParam>(11),
										   static_cast<TypeParam>(13), static_cast<TypeParam>(14), static_cast<TypeParam>(15)};

	TypeParam values[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
						  static_cast<TypeParam>(4), static_cast<TypeParam>(5), static_cast<TypeParam>(6),
						  static_cast<TypeParam>(7), static_cast<TypeParam>(8), static_cast<TypeParam>(9),
						  static_cast<TypeParam>(10), static_cast<TypeParam>(11), static_cast<TypeParam>(12)};

	ror::Matrix3x4<TypeParam> h(values);

	ror::Matrix3x4<TypeParam> z(mat3);
	ror::Matrix3x4<TypeParam> l(mat3, ori);

	constexpr uint32_t mat_size = 12;

	for (uint32_t index = 0; index < mat_size; ++index)
	{
		EXPECT_DOUBLE_EQ(this->m_a.m_values[index], static_cast<TypeParam>((index + 1) + (index / 3)));
		EXPECT_DOUBLE_EQ(this->m_a.m_values[index], g.m_values[index]);
		EXPECT_DOUBLE_EQ(h.m_values[index], static_cast<TypeParam>(index + 1));

		if (index % 4 == 0)
		{
			EXPECT_DOUBLE_EQ(d.m_values[index], static_cast<TypeParam>(1));
			EXPECT_DOUBLE_EQ(f.m_values[index], static_cast<TypeParam>(4));
		}
		else
		{
			EXPECT_DOUBLE_EQ(d.m_values[index], static_cast<TypeParam>(0));
			EXPECT_DOUBLE_EQ(f.m_values[index], static_cast<TypeParam>(0));
		}
	}

	test_matrix3x4_equal(mat3_ori_res, l);
	test_matrix3x4_equal(mat3_res, z);
}

TYPED_TEST(Matrix3x4Test, sets)
{
	TypeParam values[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
						  static_cast<TypeParam>(4), static_cast<TypeParam>(5), static_cast<TypeParam>(6),
						  static_cast<TypeParam>(7), static_cast<TypeParam>(8), static_cast<TypeParam>(9),
						  static_cast<TypeParam>(10), static_cast<TypeParam>(11), static_cast<TypeParam>(12)};

	ror::Matrix3x4<TypeParam> h(values);
	ror::Matrix3x4<TypeParam> i;
	ror::Matrix3x4<TypeParam> j;
	ror::Matrix3x4<TypeParam> k;

	i.set(h);
	j.set(values);
	k.set(static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
		  static_cast<TypeParam>(4), static_cast<TypeParam>(5), static_cast<TypeParam>(6),
		  static_cast<TypeParam>(7), static_cast<TypeParam>(8), static_cast<TypeParam>(9),
		  static_cast<TypeParam>(10), static_cast<TypeParam>(11), static_cast<TypeParam>(12));

	TypeParam identity[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
							static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0),
							static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1),
							static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0)};

	ror::Matrix3x4<TypeParam> ident = h;
	ident.identity();

	constexpr uint32_t matrix_size = 12;

	for (uint32_t index = 0; index < matrix_size; ++index)
	{
		EXPECT_DOUBLE_EQ(h.m_values[index], static_cast<TypeParam>(index + 1));
		EXPECT_DOUBLE_EQ(i.m_values[index], static_cast<TypeParam>(index + 1));
		EXPECT_DOUBLE_EQ(j.m_values[index], static_cast<TypeParam>(index + 1));
		EXPECT_DOUBLE_EQ(k.m_values[index], static_cast<TypeParam>(index + 1));

		EXPECT_DOUBLE_EQ(k.m_values[index], k.get(index % 3, index / 3));
		EXPECT_DOUBLE_EQ(k.get(index % 3, index / 3), static_cast<TypeParam>(index + 1));

		EXPECT_DOUBLE_EQ(k.m_values[index], static_cast<const TypeParam *>(k)[index]);
		EXPECT_DOUBLE_EQ(ident.m_values[index], identity[index]);
	}
}

TYPED_TEST(Matrix3x4Test, axis_sets)
{
	ror::Matrix3x4<TypeParam> i;
	ror::Matrix3x4<TypeParam> j;

	ror::Vector3<TypeParam> x_axis(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(6));
	ror::Vector3<TypeParam> y_axis(static_cast<TypeParam>(10), static_cast<TypeParam>(12), static_cast<TypeParam>(14));
	ror::Vector3<TypeParam> z_axis(static_cast<TypeParam>(18), static_cast<TypeParam>(20), static_cast<TypeParam>(22));
	ror::Vector3<TypeParam> origin(static_cast<TypeParam>(26), static_cast<TypeParam>(28), static_cast<TypeParam>(30));

	ror::Matrix3x4<TypeParam> mat_res(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(6),
									  static_cast<TypeParam>(10), static_cast<TypeParam>(12), static_cast<TypeParam>(14),
									  static_cast<TypeParam>(18), static_cast<TypeParam>(20), static_cast<TypeParam>(22),
									  static_cast<TypeParam>(26), static_cast<TypeParam>(28), static_cast<TypeParam>(30));

	this->m_a.set_x_axis(x_axis);
	this->m_a.set_y_axis(y_axis);
	this->m_a.set_z_axis(z_axis);
	this->m_a.set_origin(origin);

	ror::Vector3<TypeParam> x_axis_4(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(6));
	ror::Vector3<TypeParam> y_axis_4(static_cast<TypeParam>(10), static_cast<TypeParam>(12), static_cast<TypeParam>(14));
	ror::Vector3<TypeParam> z_axis_4(static_cast<TypeParam>(18), static_cast<TypeParam>(20), static_cast<TypeParam>(22));

	i.set_column(0, x_axis_4);
	i.set_column(1, y_axis_4);
	i.set_column(2, z_axis_4);
	i.set_column(3, origin);

	j.set_axis(0, x_axis_4);
	j.set_axis(1, y_axis_4);
	j.set_axis(2, z_axis_4);
	j.set_axis(3, origin);

	test_matrix3x4_equal(this->m_a, mat_res);
	test_matrix3x4_equal(i, this->m_b);
	test_matrix3x4_equal(j, this->m_b);

	test_matrix3x4_equal(j, i);
}

TYPED_TEST(Matrix3x4Test, axis_gets)
{
	ror::Vector3<TypeParam> x_axis(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(6));
	ror::Vector3<TypeParam> y_axis(static_cast<TypeParam>(10), static_cast<TypeParam>(12), static_cast<TypeParam>(14));
	ror::Vector3<TypeParam> z_axis(static_cast<TypeParam>(18), static_cast<TypeParam>(20), static_cast<TypeParam>(22));
	ror::Vector3<TypeParam> origin(static_cast<TypeParam>(26), static_cast<TypeParam>(28), static_cast<TypeParam>(30));

	auto x = this->m_b.x_axis();
	auto y = this->m_b.y_axis();
	auto z = this->m_b.z_axis();
	auto o = this->m_b.origin();

	auto xc = this->m_b.column(0);
	auto yc = this->m_b.column(1);
	auto zc = this->m_b.column(2);
	auto oc = this->m_b.column(3);

	test_vector3_equal(x, x_axis);
	test_vector3_equal(y, y_axis);
	test_vector3_equal(z, z_axis);
	test_vector3_equal(o, origin);

	test_vector3_equal(xc, x_axis);
	test_vector3_equal(yc, y_axis);
	test_vector3_equal(zc, z_axis);
	test_vector3_equal(oc, origin);
}

TYPED_TEST(Matrix3x4TestSigned, linear_algebra)
{
	ror::Matrix3x4<TypeParam> sum_ab_res(3, 6, 9, 15, 18, 21, 27, 30, 33, 39, 42, 45);
	ror::Matrix3x4<TypeParam> diff_ab_res(-1, -2, -3, -5, -6, -7, -9, -10, -11, -13, -14, -15);
	ror::Matrix3x4<TypeParam> neg_a_res(-1, -2, -3, -5, -6, -7, -9, -10, -11, -13, -14, -15);
	ror::Matrix3x4<TypeParam> mul_ab_res(76, 88, 100, 196, 232, 268, 316, 376, 436, 449, 534, 619);
	ror::Matrix3x4<TypeParam> sum_ac_res(4, 5, 6, 8, 9, 10, 12, 13, 14, 16, 17, 18);
	ror::Matrix3x4<TypeParam> diff_ac_res(-2, -1, 0, 2, 3, 4, 6, 7, 8, 10, 11, 12);
	ror::Matrix3x4<TypeParam> mul_ac_res(3, 6, 9, 15, 18, 21, 27, 30, 33, 39, 42, 45);
	ror::Matrix3x4<TypeParam> div_ac_res(static_cast<TypeParam>(1.0 / 3.0), static_cast<TypeParam>(2.0 / 3.0), static_cast<TypeParam>(1.0),
										 static_cast<TypeParam>(5.0 / 3.0), 2.0, static_cast<TypeParam>(7.0 / 3.0),
										 static_cast<TypeParam>(3.0), static_cast<TypeParam>(10.0 / 3.0), static_cast<TypeParam>(11.0 / 3.0),
										 static_cast<TypeParam>(13.0 / 3.0), static_cast<TypeParam>(14.0 / 3.0), static_cast<TypeParam>(5.0));

	ror::Matrix3x4<TypeParam> sum_ab  = this->m_a + this->m_b;
	ror::Matrix3x4<TypeParam> diff_ab = this->m_a - this->m_b;
	ror::Matrix3x4<TypeParam> mul_ab  = this->m_a * this->m_b;

	ror::Matrix3x4<TypeParam> sum_ac  = this->m_a + this->m_c;
	ror::Matrix3x4<TypeParam> diff_ac = this->m_a - this->m_c;
	ror::Matrix3x4<TypeParam> mul_ac  = this->m_a * this->m_c;
	ror::Matrix3x4<TypeParam> div_ac  = this->m_a / this->m_c;

	test_matrix3x4_equal(sum_ab, sum_ab_res);
	test_matrix3x4_equal(diff_ab, diff_ab_res);
	test_matrix3x4_equal(mul_ab, mul_ab_res);

	test_matrix3x4_equal(sum_ac, sum_ac_res);
	test_matrix3x4_equal(diff_ac, diff_ac_res);
	test_matrix3x4_equal(mul_ac, mul_ac_res);
	test_matrix3x4_equal(div_ac, div_ac_res);

	auto eq_set = this->m_b;

	eq_set += this->m_a;
	test_matrix3x4_equal(eq_set, sum_ab_res);

	eq_set -= this->m_b;
	test_matrix3x4_equal(eq_set, this->m_a);

	eq_set *= this->m_b;
	test_matrix3x4_equal(eq_set, mul_ab);

	eq_set = this->m_a;
	eq_set *= this->m_c;
	test_matrix3x4_equal(eq_set, mul_ac);
	test_matrix3x4_equal(-this->m_a, neg_a_res);

	ror::Vector3<TypeParam> v1(1, 1, 1);
	ror::Vector3<TypeParam> v2(4, 4, 4);
	ror::Vector3<TypeParam> v3(6, 6, 6);
	ror::Vector3<TypeParam> v4(3, 3, 3);
	ror::Vector3<TypeParam> v5(8, 8, 8);
	ror::Vector3<TypeParam> v5_res(19, 19, 19);
	ror::Vector3<TypeParam> v6_res(42, 42, 42);
	ror::Vector3<TypeParam> v7_res(546, 546, 546);

	v5 += v1 + v2 + v3;
	test_vector3_equal(v5, v5_res);

	v3 *= v2 + v4;
	test_vector3_equal(v3, v6_res);

	v3 *= v2 * v4 + v1;
	test_vector3_equal(v3, v7_res);
}

TYPED_TEST(Matrix3x4TestSigned, other_member_funcs)
{
	TypeParam det_a = static_cast<TypeParam>(0);
	TypeParam det_b = static_cast<TypeParam>(0);

	ror::Matrix3x4<TypeParam> mdet{3, static_cast<TypeParam>(0.4), 0,
								   0, 4, static_cast<TypeParam>(0.1),
								   1, static_cast<TypeParam>(0.1), 3,
								   12, 13, 14};

	ror::Matrix4<TypeParam> mdet_mat4{3, static_cast<TypeParam>(0.4), 0, 0,
									  0, 4, static_cast<TypeParam>(0.1), 0,
									  1, static_cast<TypeParam>(0.1), 3, 0,
									  12, 13, 14, 1};

	ror::Matrix3x4<TypeParam> mdet_inverse{static_cast<TypeParam>(0.33296306581505136), static_cast<TypeParam>(-0.03332407664537628), static_cast<TypeParam>(0.0011108025548458762),
										   static_cast<TypeParam>(0.0027770063871146904), static_cast<TypeParam>(0.24993057484032213), static_cast<TypeParam>(-0.00833101916134407),
										   static_cast<TypeParam>(-0.11108025548458761), static_cast<TypeParam>(0.0027770063871146904), static_cast<TypeParam>(0.3332407664537628),
										   static_cast<TypeParam>(-2.476534296028881), static_cast<TypeParam>(-2.888086642599278), static_cast<TypeParam>(-4.5703971119133575)};

	auto mdet_det     = mdet.determinant();
	auto mdet_det_res = mdet_mat4.determinant();

	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(mdet_det_res), mdet_det);

	EXPECT_DOUBLE_EQ(det_a, this->m_a.determinant());
	EXPECT_DOUBLE_EQ(det_b, this->m_b.determinant());

	EXPECT_DOUBLE_EQ(ror::identity_matrix4f.determinant(), static_cast<float32_t>(1));
	EXPECT_DOUBLE_EQ(ror::identity_matrix4d.determinant(), static_cast<double64_t>(1));
	EXPECT_DOUBLE_EQ(ror::identity_matrix4i.determinant(), static_cast<int32_t>(1));

	ror::Matrix3x4<TypeParam> mdet_inv;

	ASSERT_TRUE(mdet.inverse(mdet_inv));
	test_matrix3x4_equal(mdet_inv, mdet_inverse);

	ror::Matrix4<TypeParam> mdet_inv_mes;
	ror::mesa_glu_invert_matrix(mdet_mat4.m_values, mdet_inv_mes.m_values);
	test_matrix3x4_equal_matrix4(mdet_inverse, mdet_inv_mes);

	EXPECT_DOUBLE_EQ(mdet_inv_mes.m_values[3], 0.0);
	EXPECT_DOUBLE_EQ(mdet_inv_mes.m_values[7], 0.0);
	EXPECT_DOUBLE_EQ(mdet_inv_mes.m_values[11], 0.0);
	EXPECT_DOUBLE_EQ(mdet_inv_mes.m_values[15], 1.0);

	ASSERT_TRUE(mdet.invert());
	test_matrix3x4_equal(mdet, mdet_inverse);

	ASSERT_FALSE(this->m_a.is_orthogonal());
	ASSERT_FALSE(this->m_b.is_orthogonal());

	auto mtr = ror::matrix3x4_rotation_around_x(ror::to_radians(35.0f));

	ASSERT_TRUE(mtr.is_orthogonal());

	ASSERT_TRUE(ror::identity_matrix3x4d.is_orthogonal());
	ASSERT_TRUE(ror::identity_matrix3x4f.is_orthogonal());
	ASSERT_TRUE(ror::identity_matrix3x4i.is_orthogonal());

	ror::Matrix3x4<TypeParam> newm{};

	ror::Vector3<TypeParam> vx(1, -1, 1);
	ror::Vector3<TypeParam> vy(1, 0, 1);
	ror::Vector3<TypeParam> vz(1, 1, 2);

	newm.set_x_axis(vx);
	newm.set_y_axis(vy);
	newm.set_z_axis(vz);

	newm.orthogonalize();
	ASSERT_TRUE(newm.is_orthogonal());

	this->m_a.orthogonalize();
	this->m_b.orthogonalize();

	ASSERT_FALSE(this->m_a.is_orthogonal());
	ASSERT_FALSE(this->m_b.is_orthogonal());
}

TYPED_TEST(Matrix3x4TestSigned, other_globabl_operators)
{
	ror::Matrix3x4<TypeParam> a_copy_res(1, 2, 3,
										 5, 6, 7,
										 9, 10, 11,
										 13, 14, 15);

	ror::Matrix3x4<TypeParam> mat(static_cast<TypeParam>(1.8616338968276978), static_cast<TypeParam>(-0.69396829605102539), static_cast<TypeParam>(0.22959759831428528),
								  static_cast<TypeParam>(0.71927881240844727), static_cast<TypeParam>(1.6272891759872437), static_cast<TypeParam>(-0.91354060173034668),
								  static_cast<TypeParam>(0.13017317652702332), static_cast<TypeParam>(0.93291282653808594), static_cast<TypeParam>(1.7642892599105835),
								  static_cast<TypeParam>(-32.392974853515625), static_cast<TypeParam>(11.86034393310547), static_cast<TypeParam>(14.088020324707031));

	ror::Matrix4<TypeParam> mat4(static_cast<TypeParam>(1.8616338968276978), static_cast<TypeParam>(-0.69396829605102539), static_cast<TypeParam>(0.22959759831428528), 0,
								 static_cast<TypeParam>(0.71927881240844727), static_cast<TypeParam>(1.6272891759872437), static_cast<TypeParam>(-0.91354060173034668), 0,
								 static_cast<TypeParam>(0.13017317652702332), static_cast<TypeParam>(0.93291282653808594), static_cast<TypeParam>(1.7642892599105835), 0,
								 static_cast<TypeParam>(-32.392974853515625), static_cast<TypeParam>(11.86034393310547), static_cast<TypeParam>(14.088020324707031), 1);

	ror::Vector4<TypeParam> vec4_in(3.5f, 14.2f, 7.0f, 8.24f);
	ror::Vector4<TypeParam> vec4_res = mat4 * vec4_in;

	ror::Vector3<TypeParam> vec3_in(3.5f, 14.2f, 7.0f);
	ror::Vector3<TypeParam> vec3_res = mat4 * vec3_in;

	ASSERT_TRUE(this->m_a == a_copy_res);
	ASSERT_FALSE(this->m_a != a_copy_res);
	ASSERT_TRUE(this->m_a != this->m_b);

	auto vec4_cal = mat * vec4_in;
	test_vector3_equal(vec4_cal, vec4_res);

	auto vec3_cal = mat * vec3_in;
	test_vector3_equal(vec3_cal, vec3_res);
}

TYPED_TEST(Matrix3x4TestSigned, multiplication_operators)
{
	ror::Matrix3x4<TypeParam> mul_res(static_cast<TypeParam>(0.86867272853851318), static_cast<TypeParam>(0.48316597938537598), static_cast<TypeParam>(-0.10935358703136444),
									  static_cast<TypeParam>(-0.45744970440864563), static_cast<TypeParam>(0.86708182096481323), static_cast<TypeParam>(0.19725330173969269),
									  static_cast<TypeParam>(0.19012460112571716), static_cast<TypeParam>(-0.12132479250431061), static_cast<TypeParam>(0.97423440217971802),
									  static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));

	ror::Matrix3x4<TypeParam> mat1(static_cast<TypeParam>(0.89662456512451172), static_cast<TypeParam>(0.44266623258590698), static_cast<TypeParam>(-0.010533910244703293),
								   static_cast<TypeParam>(-0.43889886140823364), static_cast<TypeParam>(0.89163810014724731), static_cast<TypeParam>(0.11112714558839798),
								   static_cast<TypeParam>(0.058584671467542648), static_cast<TypeParam>(-0.095016010105609894), static_cast<TypeParam>(0.99375033378601074),
								   static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));

	ror::Matrix3x4<TypeParam> mat2(static_cast<TypeParam>(0.99390649795532227), static_cast<TypeParam>(0.037397608160972595), static_cast<TypeParam>(-0.10368777066469193),
								   static_cast<TypeParam>(-0.028410637751221657), static_cast<TypeParam>(0.99581754207611084), static_cast<TypeParam>(0.086834348738193512),
								   static_cast<TypeParam>(0.10650149732828140), static_cast<TypeParam>(-0.083359383046627045), static_cast<TypeParam>(0.99081206321716309),
								   static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));

	auto mul = mat1 * mat2;

	test_matrix3x4_equal(mul, mul_res);

	// Tranposed not defined for 3x4
	// (AB)′ = B′A′
	// auto mul1 = mat2.transposed() * mat1.transposed();
	// mul_res.transpose();
	// test_matrix3x4_equal(mul1, mul_res);
}

TYPED_TEST(Matrix3x4TestSigned, other_rotation_funcs)
{
	{
		TypeParam cosOfAngle90 = -0.000000043711388286737929000000f;
		TypeParam sinOfAngle90 = 1.0f;

		ror::Matrix3x4<TypeParam> mat_res(1.0f, 0.0f, 0.0f,
										  0.0f, cosOfAngle90, sinOfAngle90,
										  0.0f, -sinOfAngle90, cosOfAngle90,
										  0.0f, 0.0f, 0.0f);

		auto matrixTest = ror::matrix3x4_rotation_around_x(ror::to_radians(90.0f));

		test_matrix3x4_equal(matrixTest, mat_res);
	}

	{
		TypeParam cosOfAngle45 = 0.70710676908493042f;
		TypeParam sinOfAngle45 = 0.70710676908493042f;

		ror::Matrix3x4<TypeParam> mat(1.0f, 0.0f, 0.0f,
									  0.0f, cosOfAngle45, sinOfAngle45,
									  0.0f, -sinOfAngle45, cosOfAngle45,
									  0.0f, 0.0f, 0.0f);

		auto matrixTest = ror::matrix3x4_rotation_around_x(ror::to_radians(45.0f));
		test_matrix3x4_equal(matrixTest, mat);
	}
	{
		TypeParam cosOfAngleNeg45 = 0.70710676908493042f;
		TypeParam sinOfAngleNeg45 = -0.70710676908493042f;

		ror::Matrix3x4<TypeParam> mat(1.0f, 0.0f, 0.0f,
									  0.0f, cosOfAngleNeg45, sinOfAngleNeg45,
									  0.0f, -sinOfAngleNeg45, cosOfAngleNeg45,
									  0.0f, 0.0f, 0.0f);

		auto matrixTest = ror::matrix3x4_rotation_around_x(ror::to_radians(-45.0f));
		test_matrix3x4_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngle90 = -0.000000043711388286737929000000f;
		TypeParam sinOfAngle90 = 1.0f;

		ror::Matrix3x4<TypeParam> mat(cosOfAngle90, 0.0f, -sinOfAngle90,
									  0.0f, 1.0f, 0.0f,
									  sinOfAngle90, 0.0f, cosOfAngle90,
									  0.0f, 0.0f, 0.0f);

		auto matrixTest = ror::matrix3x4_rotation_around_y(ror::to_radians(90.0f));
		test_matrix3x4_equal(matrixTest, mat);
	}
	{
		TypeParam cosOfAngle45 = 0.70710676908493042f;
		TypeParam sinOfAngle45 = 0.70710676908493042f;

		ror::Matrix3x4<TypeParam> mat(cosOfAngle45, 0.0f, -sinOfAngle45,
									  0.0f, 1.0f, 0.0f,
									  sinOfAngle45, 0.0f, cosOfAngle45,
									  0.0f, 0.0f, 0.0f);

		auto matrixTest = ror::matrix3x4_rotation_around_y(ror::to_radians(45.0f));
		test_matrix3x4_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngleNeg45 = 0.70710676908493042f;
		TypeParam sinOfAngleNeg45 = -0.70710676908493042f;

		ror::Matrix3x4<TypeParam> mat(cosOfAngleNeg45, 0.0f, -sinOfAngleNeg45,
									  0.0f, 1.0f, 0.0f,
									  sinOfAngleNeg45, 0.0f, cosOfAngleNeg45,
									  0.0f, 0.0f, 0.0f);

		auto matrixTest = ror::matrix3x4_rotation_around_y(ror::to_radians(-45.0f));
		test_matrix3x4_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngle90 = -0.000000043711388286737929000000f;
		TypeParam sinOfAngle90 = 1.0f;

		ror::Matrix3x4<TypeParam> mat(cosOfAngle90, sinOfAngle90, 0.0f,
									  -sinOfAngle90, cosOfAngle90, 0.0f,
									  0.0f, 0.0f, 1.0f,
									  0.0f, 0.0f, 0.0f);

		auto matrixTest = ror::matrix3x4_rotation_around_z(ror::to_radians(90.0f));
		test_matrix3x4_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngle45 = 0.70710676908493042f;
		TypeParam sinOfAngle45 = 0.70710676908493042f;

		ror::Matrix3x4<TypeParam> mat(cosOfAngle45, sinOfAngle45, 0.0f,
									  -sinOfAngle45, cosOfAngle45, 0.0f,
									  0.0f, 0.0f, 1.0f,
									  0.0f, 0.0f, 0.0f);

		auto matrixTest = ror::matrix3x4_rotation_around_z(ror::to_radians(45.0f));
		test_matrix3x4_equal(matrixTest, mat);
	}
	{
		TypeParam cosOfAngleNeg45 = 0.70710676908493042f;
		TypeParam sinOfAngleNeg45 = -0.70710676908493042f;

		ror::Matrix3x4<TypeParam> mat(cosOfAngleNeg45, sinOfAngleNeg45, 0.0f,
									  -sinOfAngleNeg45, cosOfAngleNeg45, 0.0f,
									  0.0f, 0.0f, 1.0f,
									  0.0f, 0.0f, 0.0f);

		auto matrixTest = ror::matrix3x4_rotation_around_z(ror::to_radians(-45.0f));
		test_matrix3x4_equal(matrixTest, mat);
	}
}

TYPED_TEST(Matrix3x4TestSigned, rotation_axis_and_angle)
{
	{
		ror::Matrix3x4<TypeParam> mat_res(0.89662456512451172f, 0.44266623258590698f, -0.010533910244703293f,
										  -0.43889886140823364f, 0.89163810014724731f, 0.11112714558839798f,
										  0.058584671467542648f, -0.095016010105609894f, 0.99375033378601074f,
										  0.0f, 0.0f, 0.0f);

		ror::Vector3<TypeParam> axis(1.70f, 0.57f, 7.27f);

		auto matrixTest = ror::matrix3x4_rotation(ror::AxisAngle<TypeParam>(axis, ror::to_radians(static_cast<TypeParam>(27.0))));

		test_matrix3x4_equal(matrixTest, mat_res);
	}

	{
		ror::Matrix3x4<TypeParam> mat_res(static_cast<TypeParam>(0.9838201), static_cast<TypeParam>(-0.1776680), static_cast<TypeParam>(-0.0230676),
										  static_cast<TypeParam>(0.1778581), static_cast<TypeParam>(0.9840350), static_cast<TypeParam>(0.0064516),
										  static_cast<TypeParam>(0.0215531), static_cast<TypeParam>(-0.0104500), static_cast<TypeParam>(0.9997131),
										  static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));

		ror::Vector3<TypeParam> axis{static_cast<TypeParam>(0.0471168), static_cast<TypeParam>(0.1243911), static_cast<TypeParam>(-0.991114)};
		TypeParam               angle = static_cast<TypeParam>(0.1803327);

		{
			auto mat = ror::matrix3x4_rotation(ror::AxisAngle<TypeParam>(axis, angle));
			test_matrix3x4_equal(mat, mat_res);
		}
	}
}

TYPED_TEST(Matrix3x4TestSigned, rotation_from_quaternion_eulers)
{
	{
		ror::Matrix3x4<TypeParam> mat_res(0.89662456512451172f, 0.44266623258590698f, -0.010533910244703293f,
										  -0.43889886140823364f, 0.89163810014724731f, 0.11112714558839798f,
										  0.058584671467542648f, -0.095016010105609894f, 0.99375033378601074f,
										  0.0f, 0.0f, 0.0f);

		ror::Quaternion<TypeParam> quat(0.0530002f, 0.0177707f, 0.2266537f, 0.9723699f);
		auto                       mat = ror::matrix3x4_rotation(quat);
		test_matrix3x4_equal(mat, mat_res);
	}
	{
		ror::Matrix3x4<TypeParam> mat_res(static_cast<TypeParam>(0.9838201), static_cast<TypeParam>(-0.1776680), static_cast<TypeParam>(-0.0230676),
										  static_cast<TypeParam>(0.1778581), static_cast<TypeParam>(0.9840350), static_cast<TypeParam>(0.0064516),
										  static_cast<TypeParam>(0.0215531), static_cast<TypeParam>(-0.0104500), static_cast<TypeParam>(0.9997131),
										  static_cast<TypeParam>(0.0), 0.0, 0.0);

		ror::Quaternion<TypeParam> quat(static_cast<TypeParam>(0.0042426), static_cast<TypeParam>(0.0112007), static_cast<TypeParam>(-0.0892441), static_cast<TypeParam>(0.9959378));

		{
			auto mat = ror::matrix3x4_rotation(quat);
			test_matrix3x4_equal(mat, mat_res);
		}
	}
	{
		ror::Matrix3x4<TypeParam> eul_mat_res{};        // identity

		TypeParam bank     = static_cast<TypeParam>(0.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix3x4_rotation(ror::EulerAngle<TypeParam>(bank, heading, attitude));
		test_matrix3x4_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix3x4<TypeParam> eul_mat_res(static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1),
											  static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0),
											  static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
											  static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0));

		TypeParam bank     = static_cast<TypeParam>(90.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix3x4_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), heading, attitude));
		test_matrix3x4_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix3x4<TypeParam> eul_mat_res(static_cast<TypeParam>(-1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
											  static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0),
											  static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1),
											  static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0));

		TypeParam bank     = static_cast<TypeParam>(180.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix3x4_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), heading, attitude));
		test_matrix3x4_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix3x4<TypeParam> eul_mat_res(static_cast<TypeParam>(0), static_cast<TypeParam>(-1), static_cast<TypeParam>(0),
											  static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1),
											  static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
											  static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0));

		TypeParam bank     = static_cast<TypeParam>(90.0);
		TypeParam heading  = static_cast<TypeParam>(-90.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix3x4_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), ror::to_radians(heading), attitude));
		test_matrix3x4_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix3x4<TypeParam> eul_mat_res(static_cast<TypeParam>(-1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
											  static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1),
											  static_cast<TypeParam>(0), static_cast<TypeParam>(-1), static_cast<TypeParam>(0),
											  static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0));

		TypeParam bank     = static_cast<TypeParam>(180.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(90.0);

		auto mat = ror::matrix3x4_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), ror::to_radians(heading), ror::to_radians(attitude)));
		test_matrix3x4_equal(mat, eul_mat_res);
	}
}

TYPED_TEST(Matrix3x4TestSigned, rotation_from_quat_axis_and_rot_tran_inverse)
{
	{
		ror::Matrix3x4<TypeParam> mat_res(static_cast<TypeParam>(0.0750698), static_cast<TypeParam>(0.9720048), static_cast<TypeParam>(0.2226457),
										  static_cast<TypeParam>(-0.4305823), static_cast<TypeParam>(0.2329847), static_cast<TypeParam>(-0.8719616),
										  static_cast<TypeParam>(-0.8994240), static_cast<TypeParam>(-0.0304093), static_cast<TypeParam>(0.4360182),
										  static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));

		ror::Quaternion<TypeParam> quat(static_cast<TypeParam>(-0.3186169), static_cast<TypeParam>(-0.4248225), static_cast<TypeParam>(0.5310282), static_cast<TypeParam>(0.6603167));
		ror::Vector3<TypeParam>    axis(static_cast<TypeParam>(3.0f), static_cast<TypeParam>(4.0f), static_cast<TypeParam>(-5.0f));

		auto mat = ror::matrix3x4_rotation(quat, axis);
		test_matrix3x4_equal(mat, mat_res);

		ror::Matrix3x4<TypeParam> inv{};

		ASSERT_TRUE(mat.inverse(inv));

		auto inv_rt = ror::inverse_rotation_translation(mat);
		test_matrix3x4_equal(inv, inv_rt);
	}        // namespace ror_test
	{
		ror::Matrix3x4<TypeParam> mat_res(static_cast<TypeParam>(-0.2626574), static_cast<TypeParam>(-0.8384869), static_cast<TypeParam>(0.4774419),
										  static_cast<TypeParam>(0.6841009), static_cast<TypeParam>(-0.5107779), static_cast<TypeParam>(-0.5206841),
										  static_cast<TypeParam>(0.6804536), static_cast<TypeParam>(0.1898569), static_cast<TypeParam>(0.7077692),
										  static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));

		ror::Quaternion<TypeParam> quat(static_cast<TypeParam>(-0.3675429), static_cast<TypeParam>(0.1050123), static_cast<TypeParam>(-0.787592), static_cast<TypeParam>(0.4833048));
		ror::Vector3<TypeParam>    axis(static_cast<TypeParam>(7.0f), static_cast<TypeParam>(-2.0f), static_cast<TypeParam>(15.0f));

		auto mat = ror::matrix3x4_rotation(quat, axis);
		test_matrix3x4_equal(mat, mat_res);

		ror::Matrix3x4<TypeParam> inv{};

		ASSERT_TRUE(mat.inverse(inv));

		auto inv_rt = ror::inverse_rotation_translation(mat);
		test_matrix3x4_equal(inv, inv_rt);
	}
}

TYPED_TEST(Matrix3x4TestSigned, normalize)
{
	ror::Matrix3x4<TypeParam> mat_res(0.896624565124511720, 0.44266623258590698, -0.010533910244703293,
									  -0.43889886140823364, 0.89163810014724731, 0.11112714558839798,
									  0.058584671467542648, -0.095016010105609894, 0.99375033378601074,
									  2.0, 3.0, 4.0);

	ror::Matrix3x4<TypeParam> mat(1.0, 2.0, 3.0,
								  4.0, 5.0, 6.0,
								  7.0, 8.0, 9.0,
								  12.0, 13.0, 14.0);

	ror::Vector3<TypeParam> x_axis = mat_res.x_axis();
	ror::Vector3<TypeParam> y_axis = mat_res.y_axis();
	ror::Vector3<TypeParam> z_axis = mat_res.z_axis();

	EXPECT_NEAR(x_axis.length(), 1.0, test_epsilon);
	EXPECT_NEAR(y_axis.length(), 1.0, test_epsilon);
	EXPECT_NEAR(z_axis.length(), 1.0, test_epsilon);

	mat_res.normalize();

	x_axis = mat_res.x_axis();
	y_axis = mat_res.y_axis();
	z_axis = mat_res.z_axis();

	EXPECT_NEAR(x_axis.length(), 1.0, test_epsilon);
	EXPECT_NEAR(y_axis.length(), 1.0, test_epsilon);
	EXPECT_NEAR(z_axis.length(), 1.0, test_epsilon);

	mat.normalize();

	x_axis = mat.x_axis();
	y_axis = mat.y_axis();
	z_axis = mat.z_axis();

	EXPECT_NEAR(x_axis.length(), 1.0, test_epsilon);
	EXPECT_NEAR(y_axis.length(), 1.0, test_epsilon);
	EXPECT_NEAR(z_axis.length(), 1.0, test_epsilon);
}

TYPED_TEST(Matrix3x4TestSigned, other_global_funcs)
{
	{
		ror::Vector3<TypeParam>   vec_tra(static_cast<TypeParam>(10), static_cast<TypeParam>(1), static_cast<TypeParam>(210));
		ror::Vector3<TypeParam>   vec_sca(static_cast<TypeParam>(3), static_cast<TypeParam>(1), static_cast<TypeParam>(2));
		ror::Matrix3x4<TypeParam> mat_rot = ror::matrix3x4_rotation_around_x(ror::to_radians(static_cast<TypeParam>(45))) *
											ror::matrix3x4_rotation_around_y(ror::to_radians(static_cast<TypeParam>(45))) *
											ror::matrix3x4_rotation_around_z(ror::to_radians(static_cast<TypeParam>(45)));

		ror::Vector3<TypeParam> vec_tra_res{};
		ror::Vector3<TypeParam> vec_sca_res{};
		ror::Matrix3<TypeParam> mat_rot_res{};

		{
			ror::Matrix3x4<TypeParam> mat_tra = ror::matrix3x4_translation(vec_tra);
			ror::Matrix3x4<TypeParam> mat_sca = ror::matrix3x4_scaling(vec_sca);
			ror::Matrix3x4<TypeParam> mat_res = mat_tra * mat_rot * mat_sca;
			ror::decompose_into_translation_rotation_scaling(mat_res, vec_tra_res, mat_rot_res, vec_sca_res);

			ror::Matrix3<TypeParam> mat_rot_res_comp{mat_rot};
			test_matrix3_equal(mat_rot_res_comp, mat_rot_res);
			test_vector3_equal(vec_tra, vec_tra_res);
			test_vector3_equal(vec_sca, vec_sca_res);
		}
		{
			ror::Matrix3x4<TypeParam> mat_tra = ror::matrix3x4_translation(vec_tra.x, vec_tra.y, vec_tra.z);
			ror::Matrix3x4<TypeParam> mat_sca = ror::matrix3x4_scaling(vec_sca.x, vec_sca.y, vec_sca.z);
			ror::Matrix3x4<TypeParam> mat_res = mat_tra * mat_rot * mat_sca;
			ror::decompose_into_translation_rotation_scaling(mat_res, vec_tra_res, mat_rot_res, vec_sca_res);

			ror::Matrix3<TypeParam> mat_rot_res_comp{mat_rot};
			test_matrix3_equal(mat_rot_res_comp, mat_rot_res);
			test_vector3_equal(vec_tra, vec_tra_res);
			test_vector3_equal(vec_sca, vec_sca_res);
		}
		{
			ror::Matrix3x4<TypeParam> mat_res = ror::matrix3x4_transformation(vec_tra, ror::Quaternion<TypeParam>(mat_rot));
			ror::decompose_into_translation_rotation_scaling(mat_res, vec_tra_res, mat_rot_res, vec_sca_res);

			ror::Matrix3<TypeParam> mat_rot_res_comp{mat_rot};
			test_matrix3_equal(mat_rot_res_comp, mat_rot_res);
			test_vector3_equal(vec_tra, vec_tra_res);

			ror::Vector3<TypeParam> iden_sca(1, 1, 1);
			test_vector3_equal(iden_sca, vec_sca_res);
		}
		{
			ror::Matrix3x4<TypeParam> mat_res = ror::matrix3x4_transformation(vec_tra, ror::Quaternion<TypeParam>(mat_rot), vec_sca);
			ror::decompose_into_translation_rotation_scaling(mat_res, vec_tra_res, mat_rot_res, vec_sca_res);

			ror::Matrix3<TypeParam> mat_rot_res_comp{mat_rot};
			test_matrix3_equal(mat_rot_res_comp, mat_rot_res);
			test_vector3_equal(vec_tra, vec_tra_res);
			test_vector3_equal(vec_sca, vec_sca_res);
		}
	}
	{
		ror::Matrix3x4<TypeParam> mat_a{static_cast<TypeParam>(0), static_cast<TypeParam>(4), static_cast<TypeParam>(10),
										static_cast<TypeParam>(1), static_cast<TypeParam>(4), static_cast<TypeParam>(10),
										static_cast<TypeParam>(1), static_cast<TypeParam>(4), static_cast<TypeParam>(10),
										static_cast<TypeParam>(20), static_cast<TypeParam>(12), static_cast<TypeParam>(0)};

		ror::Matrix3x4<TypeParam> mat_b{static_cast<TypeParam>(2), static_cast<TypeParam>(-6), static_cast<TypeParam>(30),
										static_cast<TypeParam>(2), static_cast<TypeParam>(6), static_cast<TypeParam>(-30),
										static_cast<TypeParam>(2), static_cast<TypeParam>(-8), static_cast<TypeParam>(30),
										static_cast<TypeParam>(20), static_cast<TypeParam>(14), static_cast<TypeParam>(10)};

		ror::Matrix3x4<TypeParam> mat_r{static_cast<TypeParam>(1), static_cast<TypeParam>(-1), static_cast<TypeParam>(20),
										static_cast<TypeParam>(1.5), static_cast<TypeParam>(5), static_cast<TypeParam>(-10),
										static_cast<TypeParam>(1.5), static_cast<TypeParam>(-2), static_cast<TypeParam>(20),
										static_cast<TypeParam>(20), static_cast<TypeParam>(13), static_cast<TypeParam>(5)};

		{
			auto mat = ror::matrix3x4_interpolate(mat_a, mat_b, static_cast<TypeParam>(0.5));
			test_matrix3x4_equal(mat, mat_r);
		}
		{
			auto mat = ror::matrix3x4_interpolate(mat_a, mat_b, static_cast<TypeParam>(0));
			test_matrix3x4_equal(mat, mat_a);
		}
		{
			auto mat = ror::matrix3x4_interpolate(mat_a, mat_b, static_cast<TypeParam>(1));
			test_matrix3x4_equal(mat, mat_b);
		}
	}
}
}        // namespace ror_test
