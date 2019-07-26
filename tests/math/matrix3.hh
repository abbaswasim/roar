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
//
// Version: 1.0.0

#include "matrix3.hpp"

namespace ror_test
{
template <class _type>
void Matrix3Test<_type>::SetUp()
{
	this->m_c = static_cast<_type>(3.0);
	this->m_a = ror::Matrix3<_type>(static_cast<_type>(1), static_cast<_type>(2), static_cast<_type>(3),
									static_cast<_type>(4), static_cast<_type>(5), static_cast<_type>(6),
									static_cast<_type>(7), static_cast<_type>(8), static_cast<_type>(9));

	this->m_b = ror::Matrix3<_type>(static_cast<_type>(2), static_cast<_type>(4), static_cast<_type>(6),
									static_cast<_type>(8), static_cast<_type>(10), static_cast<_type>(12),
									static_cast<_type>(14), static_cast<_type>(16), static_cast<_type>(18));
}

template <class _type>
void Matrix3TestSigned<_type>::SetUp()
{
	Matrix3Test<_type>::SetUp();
}

template <class _type>
void Matrix3Test<_type>::TearDown()
{}

template <class _type>
void Matrix3TestSigned<_type>::TearDown()
{
	Matrix3Test<_type>::TearDown();
}

TYPED_TEST(Matrix3Test, constructors)
{
	ror::Matrix3<TypeParam> d;
	ror::Matrix3<TypeParam> f(static_cast<TypeParam>(4));
	ror::Matrix3<TypeParam> g(this->m_a);

	// TODO test matrix3 constructors too
	// ror::Matrix3<TypeParam> h();

	TypeParam values[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
						  static_cast<TypeParam>(4), static_cast<TypeParam>(5), static_cast<TypeParam>(6),
						  static_cast<TypeParam>(7), static_cast<TypeParam>(8), static_cast<TypeParam>(9)};

	ror::Matrix3<TypeParam> h(values);

	const uint32_t matrix_size = 9;

	for (uint32_t index = 0; index < matrix_size; ++index)
	{
		EXPECT_DOUBLE_EQ(this->m_a.m_values[index], static_cast<TypeParam>(index + 1));
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
}

TYPED_TEST(Matrix3Test, sets)
{
	TypeParam values[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
						  static_cast<TypeParam>(4), static_cast<TypeParam>(5), static_cast<TypeParam>(6),
						  static_cast<TypeParam>(7), static_cast<TypeParam>(8), static_cast<TypeParam>(9)};

	ror::Matrix3<TypeParam> h(values);
	ror::Matrix3<TypeParam> i;
	ror::Matrix3<TypeParam> j;
	ror::Matrix3<TypeParam> k;

	i.set(h);
	j.set(values);
	k.set(static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
		  static_cast<TypeParam>(4), static_cast<TypeParam>(5), static_cast<TypeParam>(6),
		  static_cast<TypeParam>(7), static_cast<TypeParam>(8), static_cast<TypeParam>(9));

	TypeParam identity[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
							static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0),
							static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1)};

	ror::Matrix3<TypeParam> ident = h;
	ident.identity();

	const uint32_t matrix_size = 9;

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

TYPED_TEST(Matrix3Test, axis_sets)
{
	ror::Matrix3<TypeParam> i;
	ror::Matrix3<TypeParam> j;

	ror::Vector3<TypeParam> x_axis(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(6));
	ror::Vector3<TypeParam> y_axis(static_cast<TypeParam>(8), static_cast<TypeParam>(10), static_cast<TypeParam>(12));
	ror::Vector3<TypeParam> z_axis(static_cast<TypeParam>(14), static_cast<TypeParam>(16), static_cast<TypeParam>(18));

	ror::Matrix3<TypeParam> mat_res(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(6),
									static_cast<TypeParam>(8), static_cast<TypeParam>(10), static_cast<TypeParam>(12),
									static_cast<TypeParam>(14), static_cast<TypeParam>(16), static_cast<TypeParam>(18));

	this->m_a.set_x_axis(x_axis);
	this->m_a.set_y_axis(y_axis);
	this->m_a.set_z_axis(z_axis);

	i.set_column(0, x_axis);
	i.set_column(1, y_axis);
	i.set_column(2, z_axis);

	j.set_axis(0, x_axis);
	j.set_axis(1, y_axis);
	j.set_axis(2, z_axis);

	test_matrix3_equal(this->m_a, mat_res);
	test_matrix3_equal(i, this->m_b);
	test_matrix3_equal(j, this->m_b);

	test_matrix3_equal(j, i);
}

TYPED_TEST(Matrix3Test, axis_gets)
{
	ror::Vector3<TypeParam> x_axis(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(6));
	ror::Vector3<TypeParam> y_axis(static_cast<TypeParam>(8), static_cast<TypeParam>(10), static_cast<TypeParam>(12));
	ror::Vector3<TypeParam> z_axis(static_cast<TypeParam>(14), static_cast<TypeParam>(16), static_cast<TypeParam>(18));

	auto x = this->m_b.x_axis();
	auto y = this->m_b.y_axis();
	auto z = this->m_b.z_axis();

	auto xc = this->m_b.column(0);
	auto yc = this->m_b.column(1);
	auto zc = this->m_b.column(2);

	test_vector3_equal(ror::Vector3<TypeParam>(x), ror::Vector3<TypeParam>(x_axis));
	test_vector3_equal(ror::Vector3<TypeParam>(y), ror::Vector3<TypeParam>(y_axis));
	test_vector3_equal(ror::Vector3<TypeParam>(z), ror::Vector3<TypeParam>(z_axis));

	test_vector3_equal(xc, x_axis);
	test_vector3_equal(yc, y_axis);
	test_vector3_equal(zc, z_axis);
}

TYPED_TEST(Matrix3TestSigned, linear_algebra)
{
	ror::Matrix3<TypeParam> sum_ab_res(3, 6, 9, 12, 15, 18, 21, 24, 27);
	ror::Matrix3<TypeParam> diff_ab_res(-1, -2, -3, -4, -5, -6, -7, -8, -9);
	ror::Matrix3<TypeParam> neg_a_res(-1, -2, -3, -4, -5, -6, -7, -8, -9);
	ror::Matrix3<TypeParam> mul_ab_res(60, 72, 84, 132, 162, 192, 204, 252, 300);
	ror::Matrix3<TypeParam> sum_ac_res(4, 5, 6, 7, 8, 9, 10, 11, 12);
	ror::Matrix3<TypeParam> diff_ac_res(-2, -1, 0, 1, 2, 3, 4, 5, 6);
	ror::Matrix3<TypeParam> mul_ac_res(3, 6, 9, 12, 15, 18, 21, 24, 27);
	ror::Matrix3<TypeParam> div_ac_res(static_cast<TypeParam>(1.0 / 3.0), static_cast<TypeParam>(2.0 / 3.0), static_cast<TypeParam>(1.0),
									   static_cast<TypeParam>(4.0 / 3.0), static_cast<TypeParam>(5.0 / 3.0), 2.0,
									   static_cast<TypeParam>(7.0 / 3.0), static_cast<TypeParam>(8.0 / 3.0), static_cast<TypeParam>(3.0));

	ror::Matrix3<TypeParam> sum_ab  = this->m_a + this->m_b;
	ror::Matrix3<TypeParam> diff_ab = this->m_a - this->m_b;
	ror::Matrix3<TypeParam> mul_ab  = this->m_a * this->m_b;

	ror::Matrix3<TypeParam> sum_ac  = this->m_a + this->m_c;
	ror::Matrix3<TypeParam> diff_ac = this->m_a - this->m_c;
	ror::Matrix3<TypeParam> mul_ac  = this->m_a * this->m_c;
	ror::Matrix3<TypeParam> div_ac  = this->m_a / this->m_c;

	test_matrix3_equal(sum_ab, sum_ab_res);
	test_matrix3_equal(diff_ab, diff_ab_res);
	test_matrix3_equal(mul_ab, mul_ab_res);

	test_matrix3_equal(sum_ac, sum_ac_res);
	test_matrix3_equal(diff_ac, diff_ac_res);
	test_matrix3_equal(mul_ac, mul_ac_res);
	test_matrix3_equal(div_ac, div_ac_res);

	auto eq_set = this->m_b;

	eq_set += this->m_a;
	test_matrix3_equal(eq_set, sum_ab_res);

	eq_set -= this->m_b;
	test_matrix3_equal(eq_set, this->m_a);

	eq_set *= this->m_b;
	test_matrix3_equal(eq_set, mul_ab);

	eq_set = this->m_a;
	eq_set *= this->m_c;
	test_matrix3_equal(eq_set, mul_ac);

	test_matrix3_equal(-this->m_a, neg_a_res);
}

TYPED_TEST(Matrix3TestSigned, other_member_funcs)
{
	TypeParam det_a = static_cast<TypeParam>(0);
	TypeParam det_b = static_cast<TypeParam>(0);

	ror::Matrix3<TypeParam> mdet{3, static_cast<TypeParam>(0.4), 0,
								 0, 4, static_cast<TypeParam>(0.1),
								 1, static_cast<TypeParam>(0.1), 3};

	ror::Matrix3<TypeParam> mdet_trans{3, 0, 1,
									   static_cast<TypeParam>(0.4), 4, static_cast<TypeParam>(0.1),
									   0, static_cast<TypeParam>(0.1), 3};

	ror::Matrix3<TypeParam> mdet_inverse{static_cast<TypeParam>(0.33296306581505137461), static_cast<TypeParam>(-0.033324076645376284364), static_cast<TypeParam>(0.0011108025548458761454),
										 static_cast<TypeParam>(0.0027770063871146903637), static_cast<TypeParam>(0.24993057484032213274), static_cast<TypeParam>(-0.0083310191613440710914),
										 static_cast<TypeParam>(-0.11108025548458761454), static_cast<TypeParam>(0.0027770063871146903634), static_cast<TypeParam>(0.33324076645376284365)};

	auto mdet_det = static_cast<int32_t>(mdet.determinant());
	// auto mdet_det_res = static_cast<double64_t>(36.009999999999999999);
	auto mdet_det_res = static_cast<int32_t>(36);

	EXPECT_DOUBLE_EQ(det_a, this->m_a.determinant());
	EXPECT_DOUBLE_EQ(det_b, this->m_b.determinant());

	EXPECT_DOUBLE_EQ(ror::identity_matrix3f.determinant(), static_cast<float32_t>(1));
	EXPECT_DOUBLE_EQ(ror::identity_matrix3d.determinant(), static_cast<double64_t>(1));
	EXPECT_DOUBLE_EQ(ror::identity_matrix3i.determinant(), static_cast<int32_t>(1));

	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(mdet_det_res), mdet_det);

	auto mdet_trans_res = mdet_trans.transposed();
	mdet_trans.transpose();

	test_matrix3_equal(mdet_trans, mdet_trans_res);
	test_matrix3_equal(mdet_trans, mdet);

	ror::Matrix3<TypeParam> mdet_inv;

	ASSERT_TRUE(mdet.inverse(mdet_inv));
	test_matrix3_equal(mdet_inv, mdet_inverse);

	ASSERT_TRUE(mdet.invert());
	test_matrix3_equal(mdet, mdet_inverse);

	ASSERT_FALSE(this->m_a.is_orthogonal());
	ASSERT_FALSE(this->m_b.is_orthogonal());

	auto mtr = ror::matrix3_rotation_around_x(35.0f);

	ASSERT_TRUE(mtr.is_orthogonal());

	ASSERT_TRUE(ror::identity_matrix3d.is_orthogonal());
	ASSERT_TRUE(ror::identity_matrix3f.is_orthogonal());
	ASSERT_TRUE(ror::identity_matrix3i.is_orthogonal());

	ror::Matrix3<TypeParam> newm{};

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

TYPED_TEST(Matrix3TestSigned, other_globabl_operators)
{
	ror::Matrix3<TypeParam> a_copy_res{1, 2, 3, 4, 5, 6, 7, 8, 9};

	ror::Matrix3<TypeParam> mat{1.8616338968276978, -0.69396829605102539, 0.22959759831428528,
								0.71927881240844727, 1.6272891759872437, -0.91354060173034668,
								0.13017317652702332, 0.93291282653808594, 1.7642892599105835};

	ror::Vector3<TypeParam> vec3_in{3.5f, 74.2f, 7.0f};
	ror::Vector3<TypeParam> vec3_res{static_cast<TypeParam>(60.797418755292900), static_cast<TypeParam>(124.846357607841000), static_cast<TypeParam>(-54.631096234917600)};

	ASSERT_TRUE(this->m_a == a_copy_res);
	ASSERT_FALSE(this->m_a != a_copy_res);
	ASSERT_TRUE(this->m_a != this->m_b);

	auto vec3_cal = mat * vec3_in;
	test_vector3_equal(vec3_cal, vec3_res);

	mat.transpose();

	auto vec3_cal_p = vec3_in * mat;
	test_vector3_equal(vec3_cal_p, vec3_res);
}

TYPED_TEST(Matrix3TestSigned, multiplication_operators)
{
	ror::Matrix3<TypeParam> mul_res{static_cast<TypeParam>(0.8774626743111383747), static_cast<TypeParam>(-0.44972122996832448881), static_cast<TypeParam>(0.16676304961453487466),
									static_cast<TypeParam>(0.47522451410688584697), static_cast<TypeParam>(0.86223160337282123771), static_cast<TypeParam>(-0.17526609778024642677),
									static_cast<TypeParam>(-0.064967493609271456029), static_cast<TypeParam>(0.23303937462301238597), static_cast<TypeParam>(0.97029465120444663339)};

	ror::Matrix3<TypeParam> mat1{static_cast<TypeParam>(0.89662456512451172), static_cast<TypeParam>(-0.43889886140823364), static_cast<TypeParam>(0.058584671467542648),
								 static_cast<TypeParam>(0.44266623258590698), static_cast<TypeParam>(0.89163810014724731), static_cast<TypeParam>(-0.095016010105609894),
								 static_cast<TypeParam>(-0.010533910244703293), static_cast<TypeParam>(0.11112714558839798), static_cast<TypeParam>(0.99375033378601074)};

	ror::Matrix3<TypeParam> mat2{static_cast<TypeParam>(0.99390649795532227), static_cast<TypeParam>(-0.028410637751221657), static_cast<TypeParam>(0.10650149732828140),
								 static_cast<TypeParam>(0.037397608160972595), static_cast<TypeParam>(0.99581754207611084), static_cast<TypeParam>(-0.083359383046627045),
								 static_cast<TypeParam>(-0.10368777066469193), static_cast<TypeParam>(0.086834348738193512), static_cast<TypeParam>(0.99081206321716309)};

	auto mul = mat1 * mat2;

	test_matrix3_equal(mul, mul_res);

	// (AB)′ = B′A′
	auto mul1 = mat2.transposed() * mat1.transposed();
	mul_res.transpose();
	test_matrix3_equal(mul1, mul_res);
}

TYPED_TEST(Matrix3TestSigned, other_rotation_funcs)
{
	{
		TypeParam cosOfAngle90 = -0.000000043711388286737929000000f;
		TypeParam sinOfAngle90 = 1.0f;

		ror::Matrix3<TypeParam> mat_res(1.0f, 0.0f, 0.0f,
										0.0f, cosOfAngle90, sinOfAngle90,
										0.0f, -sinOfAngle90, cosOfAngle90);

		auto matrixTest = ror::matrix3_rotation_around_x(ror::to_radians(90.0f));

		test_matrix3_equal(matrixTest, mat_res);
	}

	{
		TypeParam cosOfAngle45 = 0.70710676908493042f;
		TypeParam sinOfAngle45 = 0.70710676908493042f;

		ror::Matrix3<TypeParam> mat(1.0f, 0.0f, 0.0f,
									0.0f, cosOfAngle45, sinOfAngle45,
									0.0f, -sinOfAngle45, cosOfAngle45);

		auto matrixTest = ror::matrix3_rotation_around_x(ror::to_radians(45.0f));
		test_matrix3_equal(matrixTest, mat);
	}
	{
		TypeParam cosOfAngleNeg45 = 0.70710676908493042f;
		TypeParam sinOfAngleNeg45 = -0.70710676908493042f;

		ror::Matrix3<TypeParam> mat(1.0f, 0.0f, 0.0f,
									0.0f, cosOfAngleNeg45, sinOfAngleNeg45,
									0.0f, -sinOfAngleNeg45, cosOfAngleNeg45);

		auto matrixTest = ror::matrix3_rotation_around_x(ror::to_radians(-45.0f));
		test_matrix3_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngle90 = -0.000000043711388286737929000000f;
		TypeParam sinOfAngle90 = 1.0f;

		ror::Matrix3<TypeParam> mat(cosOfAngle90, 0.0f, -sinOfAngle90,
									0.0f, 1.0f, 0.0f,
									sinOfAngle90, 0.0f, cosOfAngle90);

		auto matrixTest = ror::matrix3_rotation_around_y(ror::to_radians(90.0f));
		test_matrix3_equal(matrixTest, mat);
	}
	{
		TypeParam cosOfAngle45 = 0.70710676908493042f;
		TypeParam sinOfAngle45 = 0.70710676908493042f;

		ror::Matrix3<TypeParam> mat(cosOfAngle45, 0.0f, -sinOfAngle45,
									0.0f, 1.0f, 0.0f,
									sinOfAngle45, 0.0f, cosOfAngle45);

		auto matrixTest = ror::matrix3_rotation_around_y(ror::to_radians(45.0f));
		test_matrix3_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngleNeg45 = 0.70710676908493042f;
		TypeParam sinOfAngleNeg45 = -0.70710676908493042f;

		ror::Matrix3<TypeParam> mat(cosOfAngleNeg45, 0.0f, -sinOfAngleNeg45,
									0.0f, 1.0f, 0.0f,
									sinOfAngleNeg45, 0.0f, cosOfAngleNeg45);

		auto matrixTest = ror::matrix3_rotation_around_y(ror::to_radians(-45.0f));
		test_matrix3_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngle90 = -0.000000043711388286737929000000f;
		TypeParam sinOfAngle90 = 1.0f;

		ror::Matrix3<TypeParam> mat(cosOfAngle90, sinOfAngle90, 0.0f,
									-sinOfAngle90, cosOfAngle90, 0.0f,
									0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix3_rotation_around_z(ror::to_radians(90.0f));
		test_matrix3_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngle45 = 0.70710676908493042f;
		TypeParam sinOfAngle45 = 0.70710676908493042f;

		ror::Matrix3<TypeParam> mat(cosOfAngle45, sinOfAngle45, 0.0f,
									-sinOfAngle45, cosOfAngle45, 0.0f,
									0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix3_rotation_around_z(ror::to_radians(45.0f));
		test_matrix3_equal(matrixTest, mat);
	}
	{
		TypeParam cosOfAngleNeg45 = 0.70710676908493042f;
		TypeParam sinOfAngleNeg45 = -0.70710676908493042f;

		ror::Matrix3<TypeParam> mat(cosOfAngleNeg45, sinOfAngleNeg45, 0.0f,
									-sinOfAngleNeg45, cosOfAngleNeg45, 0.0f,
									0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix3_rotation_around_z(ror::to_radians(-45.0f));
		test_matrix3_equal(matrixTest, mat);
	}
}

TYPED_TEST(Matrix3TestSigned, rotation_axis_and_angle)
{
	{
		ror::Matrix3<TypeParam> mat_res(0.89662456512451172f, 0.44266623258590698f, -0.010533910244703293f,
										-0.43889886140823364f, 0.89163810014724731f, 0.11112714558839798f,
										0.058584671467542648f, -0.095016010105609894f, 0.99375033378601074f);

		ror::Vector3<TypeParam> axis(1.70f, 0.57f, 7.27f);

		auto matrixTest = ror::matrix3_rotation(ror::AxisAngle<TypeParam>(axis, ror::to_radians(static_cast<TypeParam>(27.0))));

		test_matrix3_equal(matrixTest, mat_res);
	}

	{
		ror::Matrix3<TypeParam> mat_res(static_cast<TypeParam>(0.9838201), static_cast<TypeParam>(-0.1776680), static_cast<TypeParam>(-0.0230676),
										static_cast<TypeParam>(0.1778581), static_cast<TypeParam>(0.9840350), static_cast<TypeParam>(0.0064516),
										static_cast<TypeParam>(0.0215531), static_cast<TypeParam>(-0.0104500), static_cast<TypeParam>(0.9997131));

		ror::Vector3<TypeParam> axis{static_cast<TypeParam>(0.0471168), static_cast<TypeParam>(0.1243911), static_cast<TypeParam>(-0.991114)};
		TypeParam               angle = static_cast<TypeParam>(0.1803327);

		{
			auto mat = ror::matrix3_rotation(ror::AxisAngle<TypeParam>(axis, angle));
			test_matrix3_equal(mat, mat_res);
		}
	}
}

TYPED_TEST(Matrix3TestSigned, rotation_from_quaternion_eulers)
{
	{
		ror::Matrix3<TypeParam> mat_res(0.89662456512451172f, 0.44266623258590698f, -0.010533910244703293f,
										-0.43889886140823364f, 0.89163810014724731f, 0.11112714558839798f,
										0.058584671467542648f, -0.095016010105609894f, 0.99375033378601074f);

		ror::Quaternion<TypeParam> quat(0.0530002f, 0.0177707f, 0.2266537f, 0.9723699f);
		auto                       mat = ror::matrix3_rotation(quat);
		test_matrix3_equal(mat, mat_res);
	}
	{
		ror::Matrix3<TypeParam> mat_res(static_cast<TypeParam>(0.9838201), static_cast<TypeParam>(-0.1776680), static_cast<TypeParam>(-0.0230676),
										static_cast<TypeParam>(0.1778581), static_cast<TypeParam>(0.9840350), static_cast<TypeParam>(0.0064516),
										static_cast<TypeParam>(0.0215531), static_cast<TypeParam>(-0.0104500), static_cast<TypeParam>(0.9997131));

		ror::Quaternion<TypeParam> quat(static_cast<TypeParam>(0.0042426), static_cast<TypeParam>(0.0112007), static_cast<TypeParam>(-0.0892441), static_cast<TypeParam>(0.9959378));

		{
			auto mat = ror::matrix3_rotation(quat);
			test_matrix3_equal(mat, mat_res);
		}
	}
	{
		ror::Matrix3<TypeParam> eul_mat_res{};        // identity

		TypeParam bank     = static_cast<TypeParam>(0.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix3_rotation(ror::EulerAngle<TypeParam>(bank, heading, attitude));
		test_matrix3_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix3<TypeParam> eul_mat_res(static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1),
											static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0),
											static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0));

		TypeParam bank     = static_cast<TypeParam>(90.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix3_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), heading, attitude));
		test_matrix3_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix3<TypeParam> eul_mat_res(static_cast<TypeParam>(-1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
											static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0),
											static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1));

		TypeParam bank     = static_cast<TypeParam>(180.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix3_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), heading, attitude));
		test_matrix3_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix3<TypeParam> eul_mat_res(static_cast<TypeParam>(0), static_cast<TypeParam>(-1), static_cast<TypeParam>(0),
											static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1),
											static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0));

		TypeParam bank     = static_cast<TypeParam>(90.0);
		TypeParam heading  = static_cast<TypeParam>(-90.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix3_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), ror::to_radians(heading), attitude));
		test_matrix3_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix3<TypeParam> eul_mat_res(static_cast<TypeParam>(-1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
											static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1),
											static_cast<TypeParam>(0), static_cast<TypeParam>(-1), static_cast<TypeParam>(0));

		TypeParam bank     = static_cast<TypeParam>(180.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(90.0);

		auto mat = ror::matrix3_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), ror::to_radians(heading), ror::to_radians(attitude)));
		test_matrix3_equal(mat, eul_mat_res);
	}
}

TYPED_TEST(Matrix3TestSigned, rotation_from_quat_axis_and_rot_tran_inverse)
{
	{
		ror::Matrix3<TypeParam> mat_res(static_cast<TypeParam>(0.0750698), static_cast<TypeParam>(0.9720048), static_cast<TypeParam>(0.2226457),
										static_cast<TypeParam>(-0.4305823), static_cast<TypeParam>(0.2329847), static_cast<TypeParam>(-0.8719616),
										static_cast<TypeParam>(-0.8994240), static_cast<TypeParam>(-0.0304093), static_cast<TypeParam>(0.4360182));

		ror::Quaternion<TypeParam> quat(static_cast<TypeParam>(-0.3186169), static_cast<TypeParam>(-0.4248225), static_cast<TypeParam>(0.5310282), static_cast<TypeParam>(0.6603167));
		// ror::Vector3<TypeParam>    axis(static_cast<TypeParam>(3.0f), static_cast<TypeParam>(4.0f), static_cast<TypeParam>(-5.0f));

		auto mat = ror::matrix3_rotation(quat);
		test_matrix3_equal(mat, mat_res);

		ror::Matrix3<TypeParam> inv{};
		ASSERT_TRUE(mat.inverse(inv));

		auto inv_rt = ror::inverse_rotation(mat);
		test_matrix3_equal(inv, inv_rt);
	}
	{
		ror::Matrix3<TypeParam> mat_res(static_cast<TypeParam>(-0.2626574), static_cast<TypeParam>(-0.8384869), static_cast<TypeParam>(0.4774419),
										static_cast<TypeParam>(0.6841009), static_cast<TypeParam>(-0.5107779), static_cast<TypeParam>(-0.5206841),
										static_cast<TypeParam>(0.6804536), static_cast<TypeParam>(0.1898569), static_cast<TypeParam>(0.7077692));

		ror::Quaternion<TypeParam> quat(static_cast<TypeParam>(-0.3675429), static_cast<TypeParam>(0.1050123), static_cast<TypeParam>(-0.787592), static_cast<TypeParam>(0.4833048));
		ror::Vector3<TypeParam>    axis(static_cast<TypeParam>(7.0f), static_cast<TypeParam>(-2.0f), static_cast<TypeParam>(15.0f));

		auto mat = ror::matrix3_rotation(quat);
		test_matrix3_equal(mat, mat_res);
		ror::Matrix3<TypeParam> inv{};
		ASSERT_TRUE(mat.inverse(inv));
		auto inv_rt = ror::inverse_rotation(mat);
		test_matrix3_equal(inv, inv_rt);
	}
}

TYPED_TEST(Matrix3TestSigned, normalize)
{
	ror::Matrix3<TypeParam> mat_res(0.896624565124511720, 0.44266623258590698, -0.010533910244703293,
									-0.43889886140823364, 0.89163810014724731, 0.11112714558839798,
									0.058584671467542648, -0.095016010105609894, 0.99375033378601074);

	ror::Matrix3<TypeParam> mat(1.0, 2.0, 3.0,
								4.0, 5.0, 6.0,
								7.0, 8.0, 9.0);

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

TYPED_TEST(Matrix3TestSigned, untestable_funcs)
{
	{
		ror::Vector3<TypeParam> vec_sca(static_cast<TypeParam>(3), static_cast<TypeParam>(1), static_cast<TypeParam>(2));
		ror::Matrix3<TypeParam> mat_rot = ror::matrix3_rotation_around_x(ror::to_radians(static_cast<TypeParam>(45))) *
										  ror::matrix3_rotation_around_y(ror::to_radians(static_cast<TypeParam>(45))) *
										  ror::matrix3_rotation_around_z(ror::to_radians(static_cast<TypeParam>(45)));

		ror::Vector3<TypeParam> vec_sca_res{};
		ror::Matrix3<TypeParam> mat_rot_res{};

		{
			ror::Matrix3<TypeParam> mat_sca = ror::matrix3_scaling(vec_sca);
			ror::Matrix3<TypeParam> mat_res = mat_rot * mat_sca;
			ror::decompose_into_rotation_scaling(mat_res, mat_rot_res, vec_sca_res);

			test_matrix3_equal(mat_rot, mat_rot_res);
			test_vector3_equal(vec_sca, vec_sca_res);
		}
		{
			ror::Matrix3<TypeParam> mat_sca = ror::matrix3_scaling(vec_sca.x, vec_sca.y, vec_sca.z);
			ror::Matrix3<TypeParam> mat_res = mat_rot * mat_sca;
			ror::decompose_into_rotation_scaling(mat_res, mat_rot_res, vec_sca_res);

			test_matrix3_equal(mat_rot, mat_rot_res);
			test_vector3_equal(vec_sca, vec_sca_res);
		}
		{
			ror::Matrix3<TypeParam> mat_res = matrix3_transformation(ror::Quaternion<TypeParam>(mat_rot), vec_sca);
			ror::decompose_into_rotation_scaling(mat_res, mat_rot_res, vec_sca_res);

			test_matrix3_equal(mat_rot, mat_rot_res);
			test_vector3_equal(vec_sca, vec_sca_res);
		}
	}
	{
		ror::Matrix3<TypeParam> mat_a{static_cast<TypeParam>(0), static_cast<TypeParam>(4), static_cast<TypeParam>(10),
									  static_cast<TypeParam>(1), static_cast<TypeParam>(4), static_cast<TypeParam>(10),
									  static_cast<TypeParam>(1), static_cast<TypeParam>(4), static_cast<TypeParam>(10)};

		ror::Matrix3<TypeParam> mat_b{static_cast<TypeParam>(2), static_cast<TypeParam>(-6), static_cast<TypeParam>(30),
									  static_cast<TypeParam>(2), static_cast<TypeParam>(6), static_cast<TypeParam>(-30),
									  static_cast<TypeParam>(2), static_cast<TypeParam>(-8), static_cast<TypeParam>(30)};

		ror::Matrix3<TypeParam> mat_r{static_cast<TypeParam>(1), static_cast<TypeParam>(-1), static_cast<TypeParam>(20),
									  static_cast<TypeParam>(1.5), static_cast<TypeParam>(5), static_cast<TypeParam>(-10),
									  static_cast<TypeParam>(1.5), static_cast<TypeParam>(-2), static_cast<TypeParam>(20)};

		{
			auto mat = ror::matrix3_interpolate(mat_a, mat_b, static_cast<TypeParam>(0.5));
			test_matrix3_equal(mat, mat_r);
		}
		{
			auto mat = ror::matrix3_interpolate(mat_a, mat_b, static_cast<TypeParam>(0));
			test_matrix3_equal(mat, mat_a);
		}
		{
			auto mat = ror::matrix3_interpolate(mat_a, mat_b, static_cast<TypeParam>(1));
			test_matrix3_equal(mat, mat_b);
		}
	}
}

}        // namespace ror_test
