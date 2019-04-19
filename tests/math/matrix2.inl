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

namespace ror_test
{
template <class _type>
void Matrix2Test<_type>::SetUp()
{
	this->m_c = static_cast<_type>(3.0);
	this->m_a = ror::Matrix2<_type>(static_cast<_type>(1), static_cast<_type>(2),
									static_cast<_type>(3), static_cast<_type>(4));

	this->m_b = ror::Matrix2<_type>(static_cast<_type>(2), static_cast<_type>(4),
									static_cast<_type>(6), static_cast<_type>(8));
}

template <class _type>
void Matrix2TestSigned<_type>::SetUp()
{
	Matrix2Test<_type>::SetUp();
}

template <class _type>
void Matrix2Test<_type>::TearDown()
{}

template <class _type>
void Matrix2TestSigned<_type>::TearDown()
{
	Matrix2Test<_type>::TearDown();
}

TYPED_TEST(Matrix2Test, constructors)
{
	ror::Matrix2<TypeParam> d;
	ror::Matrix2<TypeParam> f(static_cast<TypeParam>(4));
	ror::Matrix2<TypeParam> g(this->m_a);

	// TODO test matrix2 constructors too
	// ror::Matrix2<TypeParam> h();

	TypeParam values[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
						  static_cast<TypeParam>(4)};

	ror::Matrix2<TypeParam> h(values);

	const uint32_t matrix_size = 4;

	for (uint32_t index = 0; index < matrix_size; ++index)
	{
		EXPECT_DOUBLE_EQ(this->m_a.m_values[index], static_cast<TypeParam>(index + 1));
		EXPECT_DOUBLE_EQ(this->m_a.m_values[index], g.m_values[index]);
		EXPECT_DOUBLE_EQ(h.m_values[index], static_cast<TypeParam>(index + 1));

		if (index % 3 == 0)
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

TYPED_TEST(Matrix2Test, sets)
{
	TypeParam values[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
						  static_cast<TypeParam>(4)};

	ror::Matrix2<TypeParam> h(values);
	ror::Matrix2<TypeParam> i;
	ror::Matrix2<TypeParam> j;
	ror::Matrix2<TypeParam> k;

	i.set(h);
	j.set(values);
	k.set(static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
		  static_cast<TypeParam>(4));

	TypeParam identity[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(0),
							static_cast<TypeParam>(0), static_cast<TypeParam>(1)};

	ror::Matrix2<TypeParam> ident = h;
	ident.identity();

	const uint32_t matrix_size = 4;

	for (uint32_t index = 0; index < matrix_size; ++index)
	{
		EXPECT_DOUBLE_EQ(h.m_values[index], static_cast<TypeParam>(index + 1));
		EXPECT_DOUBLE_EQ(i.m_values[index], static_cast<TypeParam>(index + 1));
		EXPECT_DOUBLE_EQ(j.m_values[index], static_cast<TypeParam>(index + 1));
		EXPECT_DOUBLE_EQ(k.m_values[index], static_cast<TypeParam>(index + 1));

		EXPECT_DOUBLE_EQ(k.m_values[index], k.get(index % 2, index / 2));
		EXPECT_DOUBLE_EQ(k.get(index % 2, index / 2), static_cast<TypeParam>(index + 1));

		EXPECT_DOUBLE_EQ(k.m_values[index], static_cast<const TypeParam*>(k)[index]);
		EXPECT_DOUBLE_EQ(ident.m_values[index], identity[index]);
	}
}

TYPED_TEST(Matrix2Test, axis_sets)
{
	ror::Matrix2<TypeParam> i;
	ror::Matrix2<TypeParam> j;

	ror::Vector2<TypeParam> x_axis(static_cast<TypeParam>(2), static_cast<TypeParam>(4));
	ror::Vector2<TypeParam> y_axis(static_cast<TypeParam>(6), static_cast<TypeParam>(8));

	ror::Matrix2<TypeParam> mat_res(static_cast<TypeParam>(2), static_cast<TypeParam>(4),
									static_cast<TypeParam>(6), static_cast<TypeParam>(8));

	this->m_a.set_x_axis(x_axis);
	this->m_a.set_y_axis(y_axis);

	i.set_column(0, x_axis);
	i.set_column(1, y_axis);

	j.set_axis(0, x_axis);
	j.set_axis(1, y_axis);

	test_matrix2_equal(this->m_a, mat_res);
	test_matrix2_equal(i, this->m_b);
	test_matrix2_equal(j, this->m_b);

	test_matrix2_equal(j, i);
}

TYPED_TEST(Matrix2Test, axis_gets)
{
	ror::Vector2<TypeParam> x_axis(static_cast<TypeParam>(2), static_cast<TypeParam>(4));
	ror::Vector2<TypeParam> y_axis(static_cast<TypeParam>(6), static_cast<TypeParam>(8));

	auto x = this->m_b.x_axis();
	auto y = this->m_b.y_axis();

	auto xc = this->m_b.column(0);
	auto yc = this->m_b.column(1);

	test_vector2_equal(ror::Vector2<TypeParam>(x), ror::Vector2<TypeParam>(x_axis));
	test_vector2_equal(ror::Vector2<TypeParam>(y), ror::Vector2<TypeParam>(y_axis));

	test_vector2_equal(xc, x_axis);
	test_vector2_equal(yc, y_axis);
}

TYPED_TEST(Matrix2TestSigned, linear_algebra)
{
	ror::Matrix2<TypeParam> sum_ab_res{3, 6, 9, 12};
	ror::Matrix2<TypeParam> diff_ab_res{-1, -2, -3, -4};
	ror::Matrix2<TypeParam> neg_a_res{-1, -2, -3, -4};
	ror::Matrix2<TypeParam> mul_ab_res{14, 20, 30, 44};
	ror::Matrix2<TypeParam> sum_ac_res{4, 5, 6, 7};
	ror::Matrix2<TypeParam> diff_ac_res{-2, -1, 0, 1};
	ror::Matrix2<TypeParam> mul_ac_res{3, 6, 9, 12};
	ror::Matrix2<TypeParam> div_ac_res{static_cast<TypeParam>(1.0 / 3.0), static_cast<TypeParam>(2.0 / 3.0),
									   static_cast<TypeParam>(1.0), static_cast<TypeParam>(4.0 / 3.0)};

	ror::Matrix2<TypeParam> sum_ab  = this->m_a + this->m_b;
	ror::Matrix2<TypeParam> diff_ab = this->m_a - this->m_b;
	ror::Matrix2<TypeParam> mul_ab  = this->m_a * this->m_b;

	ror::Matrix2<TypeParam> sum_ac  = this->m_a + this->m_c;
	ror::Matrix2<TypeParam> diff_ac = this->m_a - this->m_c;
	ror::Matrix2<TypeParam> mul_ac  = this->m_a * this->m_c;
	ror::Matrix2<TypeParam> div_ac  = this->m_a / this->m_c;

	test_matrix2_equal(sum_ab, sum_ab_res);
	test_matrix2_equal(diff_ab, diff_ab_res);
	test_matrix2_equal(mul_ab, mul_ab_res);

	test_matrix2_equal(sum_ac, sum_ac_res);
	test_matrix2_equal(diff_ac, diff_ac_res);
	test_matrix2_equal(mul_ac, mul_ac_res);
	test_matrix2_equal(div_ac, div_ac_res);

	auto eq_set = this->m_b;

	eq_set += this->m_a;
	test_matrix2_equal(eq_set, sum_ab_res);

	eq_set -= this->m_b;
	test_matrix2_equal(eq_set, this->m_a);

	eq_set *= this->m_b;
	test_matrix2_equal(eq_set, mul_ab);

	eq_set = this->m_a;
	eq_set *= this->m_c;
	test_matrix2_equal(eq_set, mul_ac);

	test_matrix2_equal(-this->m_a, neg_a_res);
}

#define printM(m)                          \
	{                                      \
		printf("mat");                     \
		for (size_t i = 0; i < 4; ++i)     \
		{                                  \
			printf(" %f,", m.m_values[i]); \
		}                                  \
		printf("\n");                      \
	}

TYPED_TEST(Matrix2TestSigned, other_member_funcs)
{
	TypeParam det_a = static_cast<TypeParam>(-2);
	TypeParam det_b = static_cast<TypeParam>(-8);

	ror::Matrix2<TypeParam> mdet{3, static_cast<TypeParam>(0.4),
								 0, 4};

	ror::Matrix2<TypeParam> mdet_trans{3, 0,
									   static_cast<TypeParam>(0.4), 4};

	ror::Matrix2<TypeParam> mdet_inverse{static_cast<TypeParam>(0.33333333333333333333), static_cast<TypeParam>(0),
										 static_cast<TypeParam>(-0.033333333333333333332), static_cast<TypeParam>(0.25)};

	auto mdet_det     = mdet.determinant();
	auto mdet_det_res = static_cast<TypeParam>(12);

	EXPECT_DOUBLE_EQ(det_a, this->m_a.determinant());
	EXPECT_DOUBLE_EQ(det_b, this->m_b.determinant());

	EXPECT_DOUBLE_EQ(ror::identity_matrix2f.determinant(), static_cast<float32_t>(1));
	EXPECT_DOUBLE_EQ(ror::identity_matrix2d.determinant(), static_cast<double64_t>(1));
	EXPECT_DOUBLE_EQ(ror::identity_matrix2i.determinant(), static_cast<int32_t>(1));

	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(mdet_det_res), mdet_det);

	auto mdet_trans_res = mdet_trans.transposed();
	mdet_trans.transpose();

	test_matrix2_equal(mdet_trans, mdet_trans_res);
	test_matrix2_equal(mdet_trans, mdet);

	ror::Matrix2<TypeParam> mdet_inv;

	ASSERT_TRUE(mdet.inverse(mdet_inv));
	test_matrix2_equal(mdet_inv, mdet_inverse);

	ASSERT_TRUE(mdet.invert());
	test_matrix2_equal(mdet, mdet_inverse);

	ASSERT_FALSE(this->m_a.is_orthogonal());
	ASSERT_FALSE(this->m_b.is_orthogonal());

	auto mtr = ror::matrix2_rotation_around_z(35.0f);

	ASSERT_TRUE(mtr.is_orthogonal());

	ASSERT_TRUE(ror::identity_matrix2d.is_orthogonal());
	ASSERT_TRUE(ror::identity_matrix2f.is_orthogonal());
	ASSERT_TRUE(ror::identity_matrix2i.is_orthogonal());

	ror::Matrix2<TypeParam> newm{};

	ror::Vector2<TypeParam> vx(1, -1);
	ror::Vector2<TypeParam> vy(1, 0);

	newm.set_x_axis(vx);
	newm.set_y_axis(vy);

	newm.orthogonalize();
	ASSERT_TRUE(newm.is_orthogonal());

	ASSERT_FALSE(this->m_a.is_orthogonal());
	ASSERT_FALSE(this->m_b.is_orthogonal());

	this->m_a.orthogonalize();
	this->m_b.orthogonalize();

	ASSERT_TRUE(this->m_a.is_orthogonal());
	ASSERT_TRUE(this->m_b.is_orthogonal());
}

TYPED_TEST(Matrix2TestSigned, other_globabl_operators)
{
	ror::Matrix2<TypeParam> a_copy_res{1, 2, 3, 4};

	ror::Matrix2<TypeParam> mat{static_cast<TypeParam>(1.8616338968276978), static_cast<TypeParam>(-0.69396829605102539),
								static_cast<TypeParam>(0.71927881240844727), static_cast<TypeParam>(1.6272891759872437)};

	ror::Vector2<TypeParam> vec2_in{3.5f, 74.2f};
	ror::Vector2<TypeParam> vec2_res{static_cast<TypeParam>(59.88620651960373196), static_cast<TypeParam>(118.31596782207489364)};

	ASSERT_TRUE(this->m_a == a_copy_res);
	ASSERT_FALSE(this->m_a != a_copy_res);
	ASSERT_TRUE(this->m_a != this->m_b);

	auto vec2_cal = mat * vec2_in;
	test_vector2_equal(vec2_cal, vec2_res);

	mat.transpose();

	auto vec2_cal_p = vec2_in * mat;
	test_vector2_equal(vec2_cal_p, vec2_res);
}

TYPED_TEST(Matrix2TestSigned, multiplication_operators)
{
	ror::Matrix2<TypeParam> mul_res{static_cast<TypeParam>(0.87858455152492099853), static_cast<TypeParam>(-0.46155643736730679445),
									static_cast<TypeParam>(0.4743464138478188364), static_cast<TypeParam>(0.87149509366880279515)};

	ror::Matrix2<TypeParam> mat1{static_cast<TypeParam>(0.89662456512451172), static_cast<TypeParam>(-0.43889886140823364),
								 static_cast<TypeParam>(0.44266623258590698), static_cast<TypeParam>(0.89163810014724731)};

	ror::Matrix2<TypeParam> mat2{static_cast<TypeParam>(0.99390649795532227), static_cast<TypeParam>(-0.028410637751221657),
								 static_cast<TypeParam>(0.037397608160972595), static_cast<TypeParam>(0.99581754207611084)};

	auto mul = mat1 * mat2;

	test_matrix2_equal(mul, mul_res);

	// (AB)′ = B′A′
	auto mul1 = mat2.transposed() * mat1.transposed();
	mul_res.transpose();
	test_matrix2_equal(mul1, mul_res);
}

TYPED_TEST(Matrix2TestSigned, other_rotation_funcs)
{
	{
		TypeParam cosOfAngle90 = -0.000000043711388286737929000000f;
		TypeParam sinOfAngle90 = 1.0f;

		ror::Matrix2<TypeParam> mat{cosOfAngle90, sinOfAngle90,
									-sinOfAngle90, cosOfAngle90};

		auto matrixTest = ror::matrix2_rotation_around_z(ror::to_radians(90.0f));
		test_matrix2_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngle45 = 0.70710676908493042f;
		TypeParam sinOfAngle45 = 0.70710676908493042f;

		ror::Matrix2<TypeParam> mat{cosOfAngle45, sinOfAngle45,
									-sinOfAngle45, cosOfAngle45};

		auto matrixTest = ror::matrix2_rotation_around_z(ror::to_radians(45.0f));
		test_matrix2_equal(matrixTest, mat);
	}        // namespace ror_test
	{
		TypeParam cosOfAngleNeg45 = 0.70710676908493042f;
		TypeParam sinOfAngleNeg45 = -0.70710676908493042f;

		ror::Matrix2<TypeParam> mat{cosOfAngleNeg45, sinOfAngleNeg45,
									-sinOfAngleNeg45, cosOfAngleNeg45};

		auto matrixTest = ror::matrix2_rotation_around_z(ror::to_radians(-45.0f));
		test_matrix2_equal(matrixTest, mat);
	}
}        // namespace ror_test

TYPED_TEST(Matrix2TestSigned, normalize)
{
	ror::Matrix2<TypeParam> mat_res{3.0, 4.0,
									0.0, 1.0};

	ror::Matrix2<TypeParam> mat{1.0, 2.0,
								4.0, 5.0};

	ror::Vector2<TypeParam> x_axis = mat_res.x_axis();
	ror::Vector2<TypeParam> y_axis = mat_res.y_axis();

	EXPECT_NEAR(x_axis.length(), 5.0, test_epsilon);
	EXPECT_NEAR(y_axis.length(), 1.0, test_epsilon);

	mat_res.normalize();

	x_axis = mat_res.x_axis();
	y_axis = mat_res.y_axis();

	EXPECT_NEAR(x_axis.length(), 1.0, test_epsilon);
	EXPECT_NEAR(y_axis.length(), 1.0, test_epsilon);

	mat.normalize();

	x_axis = mat.x_axis();
	y_axis = mat.y_axis();

	EXPECT_NEAR(x_axis.length(), 1.0, test_epsilon);
	EXPECT_NEAR(y_axis.length(), 1.0, test_epsilon);
}

TYPED_TEST(Matrix2TestSigned, untestable_funcs)
{
	{
		ror::Vector2<TypeParam> vec_sca{static_cast<TypeParam>(3), static_cast<TypeParam>(1)};
		ror::Matrix2<TypeParam> mat_rot = ror::matrix2_rotation_around_z(ror::to_radians(static_cast<TypeParam>(45)));

		ror::Vector2<TypeParam> vec_sca_res{};
		ror::Matrix2<TypeParam> mat_rot_res{};

		{
			ror::Matrix2<TypeParam> mat_sca = ror::matrix2_scaling(vec_sca);
			ror::Matrix2<TypeParam> mat_res = mat_rot * mat_sca;
			ror::decompose_into_rotation_scaling(mat_res, mat_rot_res, vec_sca_res);

			test_matrix2_equal(mat_rot, mat_rot_res);
			test_vector2_equal(vec_sca, vec_sca_res);
		}
		{
			ror::Matrix2<TypeParam> mat_sca = ror::matrix2_scaling(vec_sca.x, vec_sca.y);
			ror::Matrix2<TypeParam> mat_res = mat_rot * mat_sca;
			ror::decompose_into_rotation_scaling(mat_res, mat_rot_res, vec_sca_res);

			test_matrix2_equal(mat_rot, mat_rot_res);
			test_vector2_equal(vec_sca, vec_sca_res);
		}
		{
			ror::Matrix2<TypeParam> mat_res = matrix2_transformation(mat_rot, vec_sca);
			ror::decompose_into_rotation_scaling(mat_res, mat_rot_res, vec_sca_res);

			test_matrix2_equal(mat_rot, mat_rot_res);
			test_vector2_equal(vec_sca, vec_sca_res);
		}
	}
	{
		ror::Matrix2<TypeParam> mat_a{static_cast<TypeParam>(0), static_cast<TypeParam>(4),
									  static_cast<TypeParam>(1), static_cast<TypeParam>(4)};

		ror::Matrix2<TypeParam> mat_b{static_cast<TypeParam>(2), static_cast<TypeParam>(-6),
									  static_cast<TypeParam>(2), static_cast<TypeParam>(6)};

		ror::Matrix2<TypeParam> mat_r{static_cast<TypeParam>(1), static_cast<TypeParam>(-1),
									  static_cast<TypeParam>(1.5), static_cast<TypeParam>(5)};

		{
			auto mat = ror::matrix2_interpolate(mat_a, mat_b, static_cast<TypeParam>(0.5));
			test_matrix2_equal(mat, mat_r);
		}
		{
			auto mat = ror::matrix2_interpolate(mat_a, mat_b, static_cast<TypeParam>(0));
			test_matrix2_equal(mat, mat_a);
		}
		{
			auto mat = ror::matrix2_interpolate(mat_a, mat_b, static_cast<TypeParam>(1));
			test_matrix2_equal(mat, mat_b);
		}
	}
}

}        // namespace ror_test
