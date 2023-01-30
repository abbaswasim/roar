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

#include "matrix4.hpp"

namespace ror_test
{
template <class _type>
void Matrix4Test<_type>::SetUp()
{
	this->m_c = static_cast<_type>(3.0);
	this->m_a = ror::Matrix4<_type>(static_cast<_type>(1), static_cast<_type>(2), static_cast<_type>(3), static_cast<_type>(4),
	                                static_cast<_type>(5), static_cast<_type>(6), static_cast<_type>(7), static_cast<_type>(8),
	                                static_cast<_type>(9), static_cast<_type>(10), static_cast<_type>(11), static_cast<_type>(12),
	                                static_cast<_type>(13), static_cast<_type>(14), static_cast<_type>(15), static_cast<_type>(16));

	this->m_b = ror::Matrix4<_type>(static_cast<_type>(2), static_cast<_type>(4), static_cast<_type>(6), static_cast<_type>(8),
	                                static_cast<_type>(10), static_cast<_type>(12), static_cast<_type>(14), static_cast<_type>(16),
	                                static_cast<_type>(18), static_cast<_type>(20), static_cast<_type>(22), static_cast<_type>(24),
	                                static_cast<_type>(26), static_cast<_type>(28), static_cast<_type>(30), static_cast<_type>(1));
}

template <class _type>
void Matrix4TestSigned<_type>::SetUp()
{
	Matrix4Test<_type>::SetUp();
}

template <class _type>
void Matrix4Test<_type>::TearDown()
{}

template <class _type>
void Matrix4TestSigned<_type>::TearDown()
{
	Matrix4Test<_type>::TearDown();
}

TYPED_TEST(Matrix4Test, constructors)
{
	ror::Matrix4<TypeParam> d;
	ror::Matrix4<TypeParam> f(static_cast<TypeParam>(4));
	ror::Matrix4<TypeParam> g(this->m_a);

	ror::Vector3<TypeParam> ori{static_cast<TypeParam>(13), static_cast<TypeParam>(14), static_cast<TypeParam>(15)};

	ror::Matrix3<TypeParam> mat3{static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
	                             static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7),
	                             static_cast<TypeParam>(9), static_cast<TypeParam>(10), static_cast<TypeParam>(11)};

	ror::Matrix4<TypeParam> mat3_res{static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(0),
	                                 static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7), static_cast<TypeParam>(0),
	                                 static_cast<TypeParam>(9), static_cast<TypeParam>(10), static_cast<TypeParam>(11), static_cast<TypeParam>(0),
	                                 static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1)};

	ror::Matrix4<TypeParam> mat3_ori_res{static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(0),
	                                     static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7), static_cast<TypeParam>(0),
	                                     static_cast<TypeParam>(9), static_cast<TypeParam>(10), static_cast<TypeParam>(11), static_cast<TypeParam>(0),
	                                     static_cast<TypeParam>(13), static_cast<TypeParam>(14), static_cast<TypeParam>(15), static_cast<TypeParam>(1)};

	ror::Matrix3x4<TypeParam> mat3x4_ori{static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
	                                     static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7),
	                                     static_cast<TypeParam>(9), static_cast<TypeParam>(10), static_cast<TypeParam>(11),
	                                     static_cast<TypeParam>(13), static_cast<TypeParam>(14), static_cast<TypeParam>(15)};

	TypeParam values[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
	                      static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7), static_cast<TypeParam>(8),
	                      static_cast<TypeParam>(9), static_cast<TypeParam>(10), static_cast<TypeParam>(11), static_cast<TypeParam>(12),
	                      static_cast<TypeParam>(13), static_cast<TypeParam>(14), static_cast<TypeParam>(15), static_cast<TypeParam>(16)};

	ror::Matrix4<TypeParam> h(values);

	ror::Matrix4<TypeParam> z(mat3);
	ror::Matrix4<TypeParam> l(mat3, ori);

	const uint32_t mat_size = 16;

	for (uint32_t index = 0; index < mat_size; ++index)
	{
		EXPECT_DOUBLE_EQ(this->m_a.m_values[index], static_cast<TypeParam>(index + 1));
		EXPECT_DOUBLE_EQ(this->m_a.m_values[index], g.m_values[index]);
		EXPECT_DOUBLE_EQ(h.m_values[index], static_cast<TypeParam>(index + 1));

		if (index % 5 == 0)
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

	ror::Matrix4<TypeParam> x(mat3x4_ori);

	test_matrix4_equal(mat3_ori_res, l);
	test_matrix4_equal(mat3_res, z);

	test_matrix4_equal(mat3_ori_res, x);
}

TYPED_TEST(Matrix4Test, sets)
{
	TypeParam values[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
	                      static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7), static_cast<TypeParam>(8),
	                      static_cast<TypeParam>(9), static_cast<TypeParam>(10), static_cast<TypeParam>(11), static_cast<TypeParam>(12),
	                      static_cast<TypeParam>(13), static_cast<TypeParam>(14), static_cast<TypeParam>(15), static_cast<TypeParam>(16)};

	ror::Matrix4<TypeParam> h(values);
	ror::Matrix4<TypeParam> i;
	ror::Matrix4<TypeParam> j;
	ror::Matrix4<TypeParam> k;

	i.set(h);
	j.set(values);
	k.set(static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
	      static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7), static_cast<TypeParam>(8),
	      static_cast<TypeParam>(9), static_cast<TypeParam>(10), static_cast<TypeParam>(11), static_cast<TypeParam>(12),
	      static_cast<TypeParam>(13), static_cast<TypeParam>(14), static_cast<TypeParam>(15), static_cast<TypeParam>(16));

	TypeParam identity[] = {static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
	                        static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
	                        static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0),
	                        static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1)};

	ror::Matrix4<TypeParam> ident = h;
	ident.identity();

	const uint32_t matrix_size = 16;

	for (uint32_t index = 0; index < matrix_size; ++index)
	{
		EXPECT_DOUBLE_EQ(h.m_values[index], static_cast<TypeParam>(index + 1));
		EXPECT_DOUBLE_EQ(i.m_values[index], static_cast<TypeParam>(index + 1));
		EXPECT_DOUBLE_EQ(j.m_values[index], static_cast<TypeParam>(index + 1));
		EXPECT_DOUBLE_EQ(k.m_values[index], static_cast<TypeParam>(index + 1));

		EXPECT_DOUBLE_EQ(k.m_values[index], k.get(index % 4, index / 4));
		EXPECT_DOUBLE_EQ(k.get(index % 4, index / 4), static_cast<TypeParam>(index + 1));

		EXPECT_DOUBLE_EQ(k.m_values[index], static_cast<const TypeParam *>(k)[index]);
		EXPECT_DOUBLE_EQ(ident.m_values[index], identity[index]);
	}
}

TYPED_TEST(Matrix4Test, axis_sets)
{
	ror::Matrix4<TypeParam> i;
	ror::Matrix4<TypeParam> j;

	ror::Vector3<TypeParam> x_axis(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(6));
	ror::Vector3<TypeParam> y_axis(static_cast<TypeParam>(10), static_cast<TypeParam>(12), static_cast<TypeParam>(14));
	ror::Vector3<TypeParam> z_axis(static_cast<TypeParam>(18), static_cast<TypeParam>(20), static_cast<TypeParam>(22));
	ror::Vector3<TypeParam> origin(static_cast<TypeParam>(26), static_cast<TypeParam>(28), static_cast<TypeParam>(30));

	ror::Matrix4<TypeParam> mat_res(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(6), 0.0,
	                                static_cast<TypeParam>(10), static_cast<TypeParam>(12), static_cast<TypeParam>(14), 0.0,
	                                static_cast<TypeParam>(18), static_cast<TypeParam>(20), static_cast<TypeParam>(22), 0.0,
	                                static_cast<TypeParam>(26), static_cast<TypeParam>(28), static_cast<TypeParam>(30), 1.0);

	this->m_a.set_x_axis(x_axis);
	this->m_a.set_y_axis(y_axis);
	this->m_a.set_z_axis(z_axis);
	this->m_a.set_origin(origin);

	ror::Vector4<TypeParam> x_axis_4(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(8));
	ror::Vector4<TypeParam> y_axis_4(static_cast<TypeParam>(10), static_cast<TypeParam>(12), static_cast<TypeParam>(14), static_cast<TypeParam>(16));
	ror::Vector4<TypeParam> z_axis_4(static_cast<TypeParam>(18), static_cast<TypeParam>(20), static_cast<TypeParam>(22), static_cast<TypeParam>(24));

	i.set_column(0, x_axis_4);
	i.set_column(1, y_axis_4);
	i.set_column(2, z_axis_4);
	i.set_column(3, ror::Vector4<TypeParam>(origin));

	j.set_axis(0, x_axis_4);
	j.set_axis(1, y_axis_4);
	j.set_axis(2, z_axis_4);
	j.set_axis(3, ror::Vector4<TypeParam>(origin));

	test_matrix4_equal(this->m_a, mat_res);
	test_matrix4_equal(i, this->m_b);
	test_matrix4_equal(j, this->m_b);

	test_matrix4_equal(j, i);
}

TYPED_TEST(Matrix4Test, axis_gets)
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

	test_vector4_equal(ror::Vector4<TypeParam>(x), ror::Vector4<TypeParam>(x_axis));
	test_vector4_equal(ror::Vector4<TypeParam>(y), ror::Vector4<TypeParam>(y_axis));
	test_vector4_equal(ror::Vector4<TypeParam>(z), ror::Vector4<TypeParam>(z_axis));
	test_vector3_equal(o, origin);

	test_vector4_equal(xc, ror::Vector4<TypeParam>(x_axis, 8));
	test_vector4_equal(yc, ror::Vector4<TypeParam>(y_axis, 16));
	test_vector4_equal(zc, ror::Vector4<TypeParam>(z_axis, 24));
	test_vector3_equal(oc, origin);
}

TYPED_TEST(Matrix4TestSigned, linear_algebra)
{
	ror::Matrix4<TypeParam> sum_ab_res(3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 17);
	ror::Matrix4<TypeParam> diff_ab_res(-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, 15);
	ror::Matrix4<TypeParam> neg_a_res(-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16);
	ror::Matrix4<TypeParam> mul_ab_res(180, 200, 220, 240, 404, 456, 508, 560, 628, 712, 796, 880, 449, 534, 619, 704);
	ror::Matrix4<TypeParam> sum_ac_res(4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19);
	ror::Matrix4<TypeParam> diff_ac_res(-2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
	ror::Matrix4<TypeParam> mul_ac_res(3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48);
	ror::Matrix4<TypeParam> div_ac_res(static_cast<TypeParam>(1.0 / 3.0), static_cast<TypeParam>(2.0 / 3.0), static_cast<TypeParam>(1.0), static_cast<TypeParam>(4.0 / 3.0),
	                                   static_cast<TypeParam>(5.0 / 3.0), 2.0, static_cast<TypeParam>(7.0 / 3.0), static_cast<TypeParam>(8.0 / 3.0), static_cast<TypeParam>(3.0),
	                                   static_cast<TypeParam>(10.0 / 3.0), static_cast<TypeParam>(11.0 / 3.0), static_cast<TypeParam>(12.0 / 3.0), static_cast<TypeParam>(13.0 / 3.0),
	                                   static_cast<TypeParam>(14.0 / 3.0), static_cast<TypeParam>(5.0), static_cast<TypeParam>(16.0 / 3.0));

	ror::Matrix4<TypeParam> sum_ab  = this->m_a + this->m_b;
	ror::Matrix4<TypeParam> diff_ab = this->m_a - this->m_b;
	ror::Matrix4<TypeParam> mul_ab  = this->m_a * this->m_b;

	ror::Matrix4<TypeParam> sum_ac  = this->m_a + this->m_c;
	ror::Matrix4<TypeParam> diff_ac = this->m_a - this->m_c;
	ror::Matrix4<TypeParam> mul_ac  = this->m_a * this->m_c;
	ror::Matrix4<TypeParam> div_ac  = this->m_a / this->m_c;

	test_matrix4_equal(sum_ab, sum_ab_res);
	test_matrix4_equal(diff_ab, diff_ab_res);
	test_matrix4_equal(mul_ab, mul_ab_res);

	test_matrix4_equal(sum_ac, sum_ac_res);
	test_matrix4_equal(diff_ac, diff_ac_res);
	test_matrix4_equal(mul_ac, mul_ac_res);
	test_matrix4_equal(div_ac, div_ac_res);

	auto eq_set = this->m_b;

	eq_set += this->m_a;
	test_matrix4_equal(eq_set, sum_ab_res);

	eq_set -= this->m_b;
	test_matrix4_equal(eq_set, this->m_a);

	eq_set *= this->m_b;
	test_matrix4_equal(eq_set, mul_ab);

	eq_set = this->m_a;
	eq_set *= this->m_c;
	test_matrix4_equal(eq_set, mul_ac);

	test_matrix4_equal(-this->m_a, neg_a_res);

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

TYPED_TEST(Matrix4TestSigned, other_member_funcs)
{
	TypeParam det_a = static_cast<TypeParam>(0);
	TypeParam det_b = static_cast<TypeParam>(0);

	ror::Matrix4<TypeParam> mdet{3, static_cast<TypeParam>(0.4), 0, 12,
	                             0, 4, static_cast<TypeParam>(0.1), 13,
	                             1, static_cast<TypeParam>(0.1), 3, 14,
	                             0, 0, 0, 1};

	ror::Matrix4<TypeParam> mdet_trans{3, 0, 1, 0,
	                                   static_cast<TypeParam>(0.4), 4, static_cast<TypeParam>(0.1), 0,
	                                   0, static_cast<TypeParam>(0.1), 3, 0,
	                                   12, 13, 14, 1};

	ror::Matrix4<TypeParam> mdet_inverse{static_cast<TypeParam>(0.33296306581505137461), static_cast<TypeParam>(-0.033324076645376284365), static_cast<TypeParam>(0.00111080255484587616), static_cast<TypeParam>(-3.577895029158567065),
	                                     static_cast<TypeParam>(0.0027770063871146903637), static_cast<TypeParam>(0.24993057484032213274), static_cast<TypeParam>(-0.0083310191613440710912), static_cast<TypeParam>(-3.1657872813107470147),
	                                     static_cast<TypeParam>(-0.11108025548458761455), static_cast<TypeParam>(0.0027770063871146903637), static_cast<TypeParam>(0.33324076645376284365), static_cast<TypeParam>(-3.3685087475701194112),
	                                     0, 0, 0, 1};

	auto mdet_det     = mdet.determinant();
	auto mdet_det_res = static_cast<double64_t>(36.009999999999998);

	EXPECT_DOUBLE_EQ(det_a, this->m_a.determinant());
	EXPECT_DOUBLE_EQ(det_b, this->m_b.determinant());

	EXPECT_DOUBLE_EQ(ror::identity_matrix4f.determinant(), static_cast<float32_t>(1));
	EXPECT_DOUBLE_EQ(ror::identity_matrix4d.determinant(), static_cast<double64_t>(1));
	EXPECT_DOUBLE_EQ(ror::identity_matrix4i.determinant(), static_cast<int32_t>(1));

	EXPECT_DOUBLE_EQ(static_cast<TypeParam>(mdet_det_res), mdet_det);

	auto mdet_trans_res = mdet_trans.transposed();
	mdet_trans.transpose();

	test_matrix4_equal(mdet_trans, mdet_trans_res);
	test_matrix4_equal(mdet_trans, mdet);

	ror::Matrix4<TypeParam> mdet_inv;

	ASSERT_TRUE(mdet.inverse(mdet_inv));
	test_matrix4_equal(mdet_inv, mdet_inverse);

	ror::Matrix4<TypeParam> mdet_inv_mes;
	ror::mesa_glu_invert_matrix(mdet.m_values, mdet_inv_mes.m_values);
	test_matrix4_equal(mdet_inverse, mdet_inv_mes);

	ASSERT_TRUE(mdet.invert());
	test_matrix4_equal(mdet, mdet_inverse);

	ASSERT_FALSE(this->m_a.is_orthogonal());
	ASSERT_FALSE(this->m_b.is_orthogonal());

	auto mtr = ror::matrix4_rotation_around_x(35.0f);

	ASSERT_TRUE(mtr.is_orthogonal());

	ASSERT_TRUE(ror::identity_matrix4d.is_orthogonal());
	ASSERT_TRUE(ror::identity_matrix4f.is_orthogonal());
	ASSERT_TRUE(ror::identity_matrix4i.is_orthogonal());

	ror::Matrix4<TypeParam> newm{};

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

TYPED_TEST(Matrix4TestSigned, other_globabl_operators)
{
	ror::Matrix4<TypeParam> a_copy_res(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

	ror::Matrix4<TypeParam> mat(static_cast<TypeParam>(1.8616338968276978), static_cast<TypeParam>(-0.69396829605102539), static_cast<TypeParam>(0.22959759831428528), static_cast<TypeParam>(-32.392974853515625),
	                            static_cast<TypeParam>(0.71927881240844727), static_cast<TypeParam>(1.6272891759872437), static_cast<TypeParam>(-0.91354060173034668), static_cast<TypeParam>(110.86034393310547),
	                            static_cast<TypeParam>(0.13017317652702332), static_cast<TypeParam>(0.93291282653808594), static_cast<TypeParam>(1.7642892599105835), static_cast<TypeParam>(74.088020324707031),
	                            static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0));

	ror::Vector4<TypeParam> vec4_in(3.5f, 74.2f, 7.0f, 8.24f);
	ror::Vector4<TypeParam> vec4_res(static_cast<TypeParam>(60.797418755292900), static_cast<TypeParam>(124.846357607841000), static_cast<TypeParam>(-54.631096234917600), static_cast<TypeParam>(8639.3179115742678));

	ror::Vector3<TypeParam> vec3_in(3.5f, 74.2f, 7.0f);
	ror::Vector3<TypeParam> vec3_res(static_cast<TypeParam>(60.797418755292900), static_cast<TypeParam>(124.846357607841000), static_cast<TypeParam>(-54.631096234917600));

	ASSERT_TRUE(this->m_a == a_copy_res);
	ASSERT_FALSE(this->m_a != a_copy_res);
	ASSERT_TRUE(this->m_a != this->m_b);

	auto vec4_cal = mat * vec4_in;
	test_vector3_equal(vec4_cal, vec4_res);

	auto vec3_cal = mat * vec3_in;
	test_vector3_equal(vec3_cal, vec3_res);

	mat.transpose();

	auto vec4_cal_p = vec4_in * mat;
	test_vector3_equal(vec4_cal_p, vec4_res);

	auto vec3_cal_p = vec3_in * mat;
	test_vector3_equal(vec3_cal_p, vec3_res);
}

TYPED_TEST(Matrix4TestSigned, multiplication_operators)
{
	ror::Matrix4<TypeParam> mul_res(static_cast<TypeParam>(0.86867272853851318), static_cast<TypeParam>(0.48316597938537598), static_cast<TypeParam>(-0.10935358703136444), static_cast<TypeParam>(0.0),
	                                static_cast<TypeParam>(-0.45744970440864563), static_cast<TypeParam>(0.86708182096481323), static_cast<TypeParam>(0.19725330173969269), static_cast<TypeParam>(0.0),
	                                static_cast<TypeParam>(0.19012460112571716), static_cast<TypeParam>(-0.12132479250431061), static_cast<TypeParam>(0.97423440217971802), static_cast<TypeParam>(0.0),
	                                static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0));

	ror::Matrix4<TypeParam> mat1(static_cast<TypeParam>(0.89662456512451172), static_cast<TypeParam>(0.44266623258590698), static_cast<TypeParam>(-0.010533910244703293), static_cast<TypeParam>(0.0),
	                             static_cast<TypeParam>(-0.43889886140823364), static_cast<TypeParam>(0.89163810014724731), static_cast<TypeParam>(0.11112714558839798), static_cast<TypeParam>(0.0),
	                             static_cast<TypeParam>(0.058584671467542648), static_cast<TypeParam>(-0.095016010105609894), static_cast<TypeParam>(0.99375033378601074), static_cast<TypeParam>(0.0),
	                             static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0));

	ror::Matrix4<TypeParam> mat2(static_cast<TypeParam>(0.99390649795532227), static_cast<TypeParam>(0.037397608160972595), static_cast<TypeParam>(-0.10368777066469193), static_cast<TypeParam>(0.0),
	                             static_cast<TypeParam>(-0.028410637751221657), static_cast<TypeParam>(0.99581754207611084), static_cast<TypeParam>(0.086834348738193512), static_cast<TypeParam>(0.0),
	                             static_cast<TypeParam>(0.10650149732828140), static_cast<TypeParam>(-0.083359383046627045), static_cast<TypeParam>(0.99081206321716309), static_cast<TypeParam>(0.0),
	                             static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0));

	auto mul = mat1 * mat2;

	test_matrix4_equal(mul, mul_res);

	// (AB)′ = B′A′
	auto mul1 = mat2.transposed() * mat1.transposed();
	mul_res.transpose();
	test_matrix4_equal(mul1, mul_res);
}

TYPED_TEST(Matrix4TestSigned, other_rotation_funcs)
{
	{
		TypeParam cosOfAngle90 = -0.000000043711388286737929000000f;
		TypeParam sinOfAngle90 = 1.0f;

		ror::Matrix4<TypeParam> mat_res(1.0f, 0.0f, 0.0f, 0.0f,
		                                0.0f, cosOfAngle90, sinOfAngle90, 0.0f,
		                                0.0f, -sinOfAngle90, cosOfAngle90, 0.0f,
		                                0.0f, 0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix4_rotation_around_x(ror::to_radians(90.0f));

		test_matrix4_equal(matrixTest, mat_res);
	}

	{
		TypeParam cosOfAngle45 = 0.70710676908493042f;
		TypeParam sinOfAngle45 = 0.70710676908493042f;

		ror::Matrix4<TypeParam> mat(1.0f, 0.0f, 0.0f, 0.0f,
		                            0.0f, cosOfAngle45, sinOfAngle45, 0.0f,
		                            0.0f, -sinOfAngle45, cosOfAngle45, 0.0f,
		                            0.0f, 0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix4_rotation_around_x(ror::to_radians(45.0f));
		test_matrix4_equal(matrixTest, mat);
	}
	{
		TypeParam cosOfAngleNeg45 = 0.70710676908493042f;
		TypeParam sinOfAngleNeg45 = -0.70710676908493042f;

		ror::Matrix4<TypeParam> mat(1.0f, 0.0f, 0.0f, 0.0f,
		                            0.0f, cosOfAngleNeg45, sinOfAngleNeg45, 0.0f,
		                            0.0f, -sinOfAngleNeg45, cosOfAngleNeg45, 0.0f,
		                            0.0f, 0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix4_rotation_around_x(ror::to_radians(-45.0f));
		test_matrix4_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngle90 = -0.000000043711388286737929000000f;
		TypeParam sinOfAngle90 = 1.0f;

		ror::Matrix4<TypeParam> mat(cosOfAngle90, 0.0f, -sinOfAngle90, 0.0f,
		                            0.0f, 1.0f, 0.0f, 0.0f,
		                            sinOfAngle90, 0.0f, cosOfAngle90, 0.0f,
		                            0.0f, 0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix4_rotation_around_y(ror::to_radians(90.0f));
		test_matrix4_equal(matrixTest, mat);
	}
	{
		TypeParam cosOfAngle45 = 0.70710676908493042f;
		TypeParam sinOfAngle45 = 0.70710676908493042f;

		ror::Matrix4<TypeParam> mat(cosOfAngle45, 0.0f, -sinOfAngle45, 0.0f,
		                            0.0f, 1.0f, 0.0f, 0.0f,
		                            sinOfAngle45, 0.0f, cosOfAngle45, 0.0f,
		                            0.0f, 0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix4_rotation_around_y(ror::to_radians(45.0f));
		test_matrix4_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngleNeg45 = 0.70710676908493042f;
		TypeParam sinOfAngleNeg45 = -0.70710676908493042f;

		ror::Matrix4<TypeParam> mat(cosOfAngleNeg45, 0.0f, -sinOfAngleNeg45, 0.0f,
		                            0.0f, 1.0f, 0.0f, 0.0f,
		                            sinOfAngleNeg45, 0.0f, cosOfAngleNeg45, 0.0f,
		                            0.0f, 0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix4_rotation_around_y(ror::to_radians(-45.0f));
		test_matrix4_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngle90 = -0.000000043711388286737929000000f;
		TypeParam sinOfAngle90 = 1.0f;

		ror::Matrix4<TypeParam> mat(cosOfAngle90, sinOfAngle90, 0.0f, 0.0f,
		                            -sinOfAngle90, cosOfAngle90, 0.0f, 0.0f,
		                            0.0f, 0.0f, 1.0f, 0.0f,
		                            0.0f, 0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix4_rotation_around_z(ror::to_radians(90.0f));
		test_matrix4_equal(matrixTest, mat);
	}

	{
		TypeParam cosOfAngle45 = 0.70710676908493042f;
		TypeParam sinOfAngle45 = 0.70710676908493042f;

		ror::Matrix4<TypeParam> mat(cosOfAngle45, sinOfAngle45, 0.0f, 0.0f,
		                            -sinOfAngle45, cosOfAngle45, 0.0f, 0.0f,
		                            0.0f, 0.0f, 1.0f, 0.0f,
		                            0.0f, 0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix4_rotation_around_z(ror::to_radians(45.0f));
		test_matrix4_equal(matrixTest, mat);
	}
	{
		TypeParam cosOfAngleNeg45 = 0.70710676908493042f;
		TypeParam sinOfAngleNeg45 = -0.70710676908493042f;

		ror::Matrix4<TypeParam> mat(cosOfAngleNeg45, sinOfAngleNeg45, 0.0f, 0.0f,
		                            -sinOfAngleNeg45, cosOfAngleNeg45, 0.0f, 0.0f,
		                            0.0f, 0.0f, 1.0f, 0.0f,
		                            0.0f, 0.0f, 0.0f, 1.0f);

		auto matrixTest = ror::matrix4_rotation_around_z(ror::to_radians(-45.0f));
		test_matrix4_equal(matrixTest, mat);
	}
}

TYPED_TEST(Matrix4TestSigned, rotation_axis_and_angle)
{
	{
		ror::Matrix4<TypeParam> mat_res(0.89662456512451172f, 0.44266623258590698f, -0.010533910244703293f, 0.0f,
		                                -0.43889886140823364f, 0.89163810014724731f, 0.11112714558839798f, 0.0f,
		                                0.058584671467542648f, -0.095016010105609894f, 0.99375033378601074f, 0.0f,
		                                0.0f, 0.0f, 0.0f, 1.0f);

		ror::Vector3<TypeParam> axis(1.70f, 0.57f, 7.27f);

		auto matrixTest = ror::matrix4_rotation(ror::AxisAngle<TypeParam>(axis, ror::to_radians(static_cast<TypeParam>(27.0))));

		test_matrix4_equal(matrixTest, mat_res);
	}

	{
		ror::Matrix4<TypeParam> mat_res(static_cast<TypeParam>(0.9838201), static_cast<TypeParam>(-0.1776680), static_cast<TypeParam>(-0.0230676), static_cast<TypeParam>(0.0f),
		                                static_cast<TypeParam>(0.1778581), static_cast<TypeParam>(0.9840350), static_cast<TypeParam>(0.0064516), static_cast<TypeParam>(0.0),
		                                static_cast<TypeParam>(0.0215531), static_cast<TypeParam>(-0.0104500), static_cast<TypeParam>(0.9997131), static_cast<TypeParam>(0.0),
		                                static_cast<TypeParam>(0.0), 0.0, 0.0, static_cast<TypeParam>(1.0));

		ror::Vector3<TypeParam> axis{static_cast<TypeParam>(0.0471168), static_cast<TypeParam>(0.1243911), static_cast<TypeParam>(-0.991114)};
		TypeParam               angle = static_cast<TypeParam>(0.1803327);

		{
			auto mat = ror::matrix4_rotation(ror::AxisAngle<TypeParam>(axis, angle));
			test_matrix4_equal(mat, mat_res);
		}
	}
}

TYPED_TEST(Matrix4TestSigned, rotation_from_quaternion_eulers)
{
	{
		ror::Matrix4<TypeParam> mat_res(0.89662456512451172f, 0.44266623258590698f, -0.010533910244703293f, 0.0f,
		                                -0.43889886140823364f, 0.89163810014724731f, 0.11112714558839798f, 0.0f,
		                                0.058584671467542648f, -0.095016010105609894f, 0.99375033378601074f, 0.0f,
		                                0.0f, 0.0f, 0.0f, 1.0f);

		ror::Quaternion<TypeParam> quat(0.0530002f, 0.0177707f, 0.2266537f, 0.9723699f);
		auto                       mat = ror::matrix4_rotation(quat);
		test_matrix4_equal(mat, mat_res);
	}
	{
		ror::Matrix4<TypeParam> mat_res(static_cast<TypeParam>(0.9838201), static_cast<TypeParam>(-0.1776680), static_cast<TypeParam>(-0.0230676), static_cast<TypeParam>(0.0f),
		                                static_cast<TypeParam>(0.1778581), static_cast<TypeParam>(0.9840350), static_cast<TypeParam>(0.0064516), static_cast<TypeParam>(0.0),
		                                static_cast<TypeParam>(0.0215531), static_cast<TypeParam>(-0.0104500), static_cast<TypeParam>(0.9997131), static_cast<TypeParam>(0.0),
		                                static_cast<TypeParam>(0.0), 0.0, 0.0, static_cast<TypeParam>(1.0));

		ror::Quaternion<TypeParam> quat(static_cast<TypeParam>(0.0042426), static_cast<TypeParam>(0.0112007), static_cast<TypeParam>(-0.0892441), static_cast<TypeParam>(0.9959378));

		{
			auto mat = ror::matrix4_rotation(quat);
			test_matrix4_equal(mat, mat_res);
		}
	}
	{
		ror::Matrix4<TypeParam> eul_mat_res{};        // identity

		TypeParam bank     = static_cast<TypeParam>(0.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix4_rotation(ror::EulerAngle<TypeParam>(bank, heading, attitude));
		test_matrix4_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix4<TypeParam> eul_mat_res(static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1));

		TypeParam bank     = static_cast<TypeParam>(90.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix4_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), heading, attitude));
		test_matrix4_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix4<TypeParam> eul_mat_res(static_cast<TypeParam>(-1), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1));

		TypeParam bank     = static_cast<TypeParam>(180.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix4_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), heading, attitude));
		test_matrix4_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix4<TypeParam> eul_mat_res(static_cast<TypeParam>(0), static_cast<TypeParam>(-1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1));

		TypeParam bank     = static_cast<TypeParam>(90.0);
		TypeParam heading  = static_cast<TypeParam>(-90.0);
		TypeParam attitude = static_cast<TypeParam>(0.0);

		auto mat = ror::matrix4_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), ror::to_radians(heading), attitude));
		test_matrix4_equal(mat, eul_mat_res);
	}
	{
		ror::Matrix4<TypeParam> eul_mat_res(static_cast<TypeParam>(-1), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(-1), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(0), static_cast<TypeParam>(-1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
		                                    static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1));

		TypeParam bank     = static_cast<TypeParam>(180.0);
		TypeParam heading  = static_cast<TypeParam>(0.0);
		TypeParam attitude = static_cast<TypeParam>(90.0);

		auto mat = ror::matrix4_rotation(ror::EulerAngle<TypeParam>(ror::to_radians(bank), ror::to_radians(heading), ror::to_radians(attitude)));
		test_matrix4_equal(mat, eul_mat_res);
	}
}

TYPED_TEST(Matrix4TestSigned, rotation_from_quat_axis_and_rot_tran_inverse)
{
	{
		ror::Matrix4<TypeParam> mat_res(static_cast<TypeParam>(0.0750698), static_cast<TypeParam>(0.9720048), static_cast<TypeParam>(0.2226457), static_cast<TypeParam>(0.0f),
		                                static_cast<TypeParam>(-0.4305823), static_cast<TypeParam>(0.2329847), static_cast<TypeParam>(-0.8719616), static_cast<TypeParam>(0.0f),
		                                static_cast<TypeParam>(-0.8994240), static_cast<TypeParam>(-0.0304093), static_cast<TypeParam>(0.4360182), static_cast<TypeParam>(0.0f),
		                                static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0));

		ror::Quaternion<TypeParam> quat(static_cast<TypeParam>(-0.3186169), static_cast<TypeParam>(-0.4248225), static_cast<TypeParam>(0.5310282), static_cast<TypeParam>(0.6603167));
		ror::Vector3<TypeParam>    axis(static_cast<TypeParam>(3.0f), static_cast<TypeParam>(4.0f), static_cast<TypeParam>(-5.0f));

		auto mat = ror::matrix4_rotation(quat, axis);
		test_matrix4_equal(mat, mat_res);
		ror::Matrix4<TypeParam> inv{};
		ASSERT_TRUE(mat.inverse(inv));
		auto inv_rt = ror::inverse_rotation_translation(mat);
		test_matrix4_equal(inv, inv_rt);
	}        // namespace ror_test
	{
		ror::Matrix4<TypeParam> mat_res(static_cast<TypeParam>(-0.2626574), static_cast<TypeParam>(-0.8384869), static_cast<TypeParam>(0.4774419), static_cast<TypeParam>(0.0),
		                                static_cast<TypeParam>(0.6841009), static_cast<TypeParam>(-0.5107779), static_cast<TypeParam>(-0.5206841), static_cast<TypeParam>(0.0),
		                                static_cast<TypeParam>(0.6804536), static_cast<TypeParam>(0.1898569), static_cast<TypeParam>(0.7077692), static_cast<TypeParam>(0.0),
		                                static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0));

		ror::Quaternion<TypeParam> quat(static_cast<TypeParam>(-0.3675429), static_cast<TypeParam>(0.1050123), static_cast<TypeParam>(-0.787592), static_cast<TypeParam>(0.4833048));
		ror::Vector3<TypeParam>    axis(static_cast<TypeParam>(7.0f), static_cast<TypeParam>(-2.0f), static_cast<TypeParam>(15.0f));

		auto mat = ror::matrix4_rotation(quat, axis);
		test_matrix4_equal(mat, mat_res);
		ror::Matrix4<TypeParam> inv{};
		ASSERT_TRUE(mat.inverse(inv));
		auto inv_rt = ror::inverse_rotation_translation(mat);
		test_matrix4_equal(inv, inv_rt);
	}
}

TYPED_TEST(Matrix4TestSigned, normalize)
{
	ror::Matrix4<TypeParam> mat_res(0.896624565124511720, 0.44266623258590698, -0.010533910244703293, 0.0,
	                                -0.43889886140823364, 0.89163810014724731, 0.11112714558839798, 0.0,
	                                0.058584671467542648, -0.095016010105609894, 0.99375033378601074, 0.0,
	                                2.0, 3.0, 4.0, 1.0);

	ror::Matrix4<TypeParam> mat(1.0, 2.0, 3.0, 0.0,
	                            4.0, 5.0, 6.0, 0.0,
	                            7.0, 8.0, 9.0, 0.0,
	                            12.0, 13.0, 14.0, 1.0);

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

TYPED_TEST(Matrix4TestSigned, untestable_funcs)
{
	ror::Vector3<TypeParam> view{static_cast<TypeParam>(-20), static_cast<TypeParam>(98), static_cast<TypeParam>(-123)};
	ror::Vector3<TypeParam> up{static_cast<TypeParam>(-2), static_cast<TypeParam>(1), static_cast<TypeParam>(-3)};
	ror::Vector3<TypeParam> target{static_cast<TypeParam>(-34), static_cast<TypeParam>(1), static_cast<TypeParam>(-12)};

	TypeParam l = static_cast<TypeParam>(-32);
	TypeParam r = static_cast<TypeParam>(43);
	TypeParam t = static_cast<TypeParam>(45);
	TypeParam b = static_cast<TypeParam>(-3);
	TypeParam n = static_cast<TypeParam>(0.01);
	TypeParam f = static_cast<TypeParam>(1242);
	TypeParam a = static_cast<TypeParam>(ror::to_radians(94.0f));
	// TypeParam a = static_cast<TypeParam>(94.0f);
	TypeParam o = static_cast<TypeParam>(1.2);

	{
		ror::Matrix4<TypeParam> mat = ror::make_look_at(view.x, view.y, view.z, target.x, target.y, target.z, up.x, up.y, up.z);
		ror::Matrix4<TypeParam> mat_res{static_cast<TypeParam>(0.4721174), static_cast<TypeParam>(-0.8764508), static_cast<TypeParam>(0.0945471), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(-0.6924389), static_cast<TypeParam>(-0.3023295), static_cast<TypeParam>(0.6550765), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(-0.5455579), static_cast<TypeParam>(-0.3747412), static_cast<TypeParam>(-0.7496236), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(10.1977368), static_cast<TypeParam>(-33.9938908), static_cast<TypeParam>(-154.51025390625), static_cast<TypeParam>(1.0000000)};
		test_matrix4_equal(mat, mat_res);
	}
	{
		ror::Matrix4<TypeParam> mat = ror::make_look_at(view, target, up);
		ror::Matrix4<TypeParam> mat_res{static_cast<TypeParam>(0.4721174), static_cast<TypeParam>(-0.8764508), static_cast<TypeParam>(0.0945471), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(-0.6924389), static_cast<TypeParam>(-0.3023295), static_cast<TypeParam>(0.6550765), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(-0.5455579), static_cast<TypeParam>(-0.3747412), static_cast<TypeParam>(-0.7496236), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(10.1977368), static_cast<TypeParam>(-33.9938908), static_cast<TypeParam>(-154.51025390625), static_cast<TypeParam>(1.0000000)};

		test_matrix4_equal(mat, mat_res);
	}
	{
		ror::Matrix4<TypeParam> mat = ror::make_frustum(l, r, t, b, n, f);

		ror::Matrix4<TypeParam> mat_res{static_cast<TypeParam>(0.0002667), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(-0.0004167), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(0.1466667), static_cast<TypeParam>(-0.8750000), static_cast<TypeParam>(-1.0000161), static_cast<TypeParam>(-1.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(-0.0200002), static_cast<TypeParam>(0.0000000)};
		test_matrix4_equal(mat, mat_res);
		frustum_update_depth(n, f, mat);
		test_matrix4_equal(mat, mat_res);
	}
	{
		ror::Matrix4<TypeParam> mat = ror::make_infinite_frustum(l, r, t, b, n);
		ror::Matrix4<TypeParam> mat_res{static_cast<TypeParam>(0.0002667), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(-0.0004167), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(0.1466667), static_cast<TypeParam>(-0.8750000), static_cast<TypeParam>(-0.9999999), static_cast<TypeParam>(-1.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(-0.0200000), static_cast<TypeParam>(0.0000000)};
		test_matrix4_equal(mat, mat_res);
	}
	{
		ror::Matrix4<TypeParam> mat = ror::make_ortho(l, r, t, b, n, f);
		ror::Matrix4<TypeParam> mat_res{static_cast<TypeParam>(0.0266667), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(-0.0416667), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(-0.0016103), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(-0.1466667), static_cast<TypeParam>(0.8750000), static_cast<TypeParam>(-1.0000161), static_cast<TypeParam>(1.0000000)};

		test_matrix4_equal(mat, mat_res);
		ortho_update_depth(n, f, mat);
		test_matrix4_equal(mat, mat_res);
	}
	{
		ror::Matrix4<TypeParam> mat = ror::make_perspective(a, o, n, f);
		ror::Matrix4<TypeParam> mat_res{static_cast<TypeParam>(0.7770959), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.9325151), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(-1.0000161), static_cast<TypeParam>(-1.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(-0.0200002), static_cast<TypeParam>(0.0000000)};
		test_matrix4_equal(mat, mat_res);
		perspective_update_depth(n, f, mat);
		test_matrix4_equal(mat, mat_res);
	}
	{
		ror::Matrix4<TypeParam> mat = ror::make_infinite_perspective(a, o, n);
		ror::Matrix4<TypeParam> mat_res{static_cast<TypeParam>(0.7770959), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.9325151), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(-0.999989), static_cast<TypeParam>(-1.0000000),
		                                static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(0.0000000), static_cast<TypeParam>(-0.019999), static_cast<TypeParam>(0.0000000)};
		test_matrix4_equal(mat, mat_res);
	}
	{
		ror::Vector3<TypeParam> vec_tra(static_cast<TypeParam>(10), static_cast<TypeParam>(1), static_cast<TypeParam>(210));
		ror::Vector3<TypeParam> vec_sca(static_cast<TypeParam>(3), static_cast<TypeParam>(1), static_cast<TypeParam>(2));
		ror::Matrix4<TypeParam> mat_rot = ror::matrix4_rotation_around_x(ror::to_radians(static_cast<TypeParam>(45))) *
		                                  ror::matrix4_rotation_around_y(ror::to_radians(static_cast<TypeParam>(45))) *
		                                  ror::matrix4_rotation_around_z(ror::to_radians(static_cast<TypeParam>(45)));

		ror::Vector3<TypeParam> vec_tra_res{};
		ror::Vector3<TypeParam> vec_sca_res{};
		ror::Matrix3<TypeParam> mat_rot_res{};

		{
			ror::Matrix4<TypeParam> mat_tra = ror::matrix4_translation(vec_tra);
			ror::Matrix4<TypeParam> mat_sca = ror::matrix4_scaling(vec_sca);
			ror::Matrix4<TypeParam> mat_res = mat_tra * mat_rot * mat_sca;
			ror::decompose_into_translation_rotation_scaling(mat_res, vec_tra_res, mat_rot_res, vec_sca_res);

			ror::Matrix3<TypeParam> mat_rot_res_comp{mat_rot};
			test_matrix3_equal(mat_rot_res_comp, mat_rot_res);
			test_vector3_equal(vec_tra, vec_tra_res);
			test_vector3_equal(vec_sca, vec_sca_res);
		}
		{
			ror::Matrix4<TypeParam> mat_tra = ror::matrix4_translation(vec_tra.x, vec_tra.y, vec_tra.z);
			ror::Matrix4<TypeParam> mat_sca = ror::matrix4_scaling(vec_sca.x, vec_sca.y, vec_sca.z);
			ror::Matrix4<TypeParam> mat_res = mat_tra * mat_rot * mat_sca;
			ror::decompose_into_translation_rotation_scaling(mat_res, vec_tra_res, mat_rot_res, vec_sca_res);

			ror::Matrix3<TypeParam> mat_rot_res_comp{mat_rot};
			test_matrix3_equal(mat_rot_res_comp, mat_rot_res);
			test_vector3_equal(vec_tra, vec_tra_res);
			test_vector3_equal(vec_sca, vec_sca_res);
		}
		{
			ror::Matrix4<TypeParam> mat_res = matrix4_transformation(vec_tra, ror::Quaternion<TypeParam>(mat_rot));
			ror::decompose_into_translation_rotation_scaling(mat_res, vec_tra_res, mat_rot_res, vec_sca_res);

			ror::Matrix3<TypeParam> mat_rot_res_comp{mat_rot};
			test_matrix3_equal(mat_rot_res_comp, mat_rot_res);
			test_vector3_equal(vec_tra, vec_tra_res);

			ror::Vector3<TypeParam> iden_sca(1, 1, 1);
			test_vector3_equal(iden_sca, vec_sca_res);
		}
		{
			ror::Matrix4<TypeParam> mat_res = matrix4_transformation(vec_tra, ror::Quaternion<TypeParam>(mat_rot), vec_sca);
			ror::decompose_into_translation_rotation_scaling(mat_res, vec_tra_res, mat_rot_res, vec_sca_res);

			ror::Matrix3<TypeParam> mat_rot_res_comp{mat_rot};
			test_matrix3_equal(mat_rot_res_comp, mat_rot_res);
			test_vector3_equal(vec_tra, vec_tra_res);
			test_vector3_equal(vec_sca, vec_sca_res);
		}
	}
	{
		ror::Matrix4<TypeParam> mat_a{static_cast<TypeParam>(0), static_cast<TypeParam>(4), static_cast<TypeParam>(10), static_cast<TypeParam>(0),
		                              static_cast<TypeParam>(1), static_cast<TypeParam>(4), static_cast<TypeParam>(10), static_cast<TypeParam>(0),
		                              static_cast<TypeParam>(1), static_cast<TypeParam>(4), static_cast<TypeParam>(10), static_cast<TypeParam>(0),
		                              static_cast<TypeParam>(20), static_cast<TypeParam>(12), static_cast<TypeParam>(0), static_cast<TypeParam>(1)};

		ror::Matrix4<TypeParam> mat_b{static_cast<TypeParam>(2), static_cast<TypeParam>(-6), static_cast<TypeParam>(30), static_cast<TypeParam>(0),
		                              static_cast<TypeParam>(2), static_cast<TypeParam>(6), static_cast<TypeParam>(-30), static_cast<TypeParam>(0),
		                              static_cast<TypeParam>(2), static_cast<TypeParam>(-8), static_cast<TypeParam>(30), static_cast<TypeParam>(0),
		                              static_cast<TypeParam>(20), static_cast<TypeParam>(14), static_cast<TypeParam>(10), static_cast<TypeParam>(1)};

		ror::Matrix4<TypeParam> mat_r{static_cast<TypeParam>(1), static_cast<TypeParam>(-1), static_cast<TypeParam>(20), static_cast<TypeParam>(0),
		                              static_cast<TypeParam>(1.5), static_cast<TypeParam>(5), static_cast<TypeParam>(-10), static_cast<TypeParam>(0),
		                              static_cast<TypeParam>(1.5), static_cast<TypeParam>(-2), static_cast<TypeParam>(20), static_cast<TypeParam>(0),
		                              static_cast<TypeParam>(20), static_cast<TypeParam>(13), static_cast<TypeParam>(5), static_cast<TypeParam>(1)};

		{
			auto mat = ror::matrix4_interpolate(mat_a, mat_b, static_cast<TypeParam>(0.5));
			test_matrix4_equal(mat, mat_r);
		}
		{
			auto mat = ror::matrix4_interpolate(mat_a, mat_b, static_cast<TypeParam>(0));
			test_matrix4_equal(mat, mat_a);
		}
		{
			auto mat = ror::matrix4_interpolate(mat_a, mat_b, static_cast<TypeParam>(1));
			test_matrix4_equal(mat, mat_b);
		}
	}
}

TYPED_TEST(Matrix4TestSigned, perspective_transform)
{
	ror::Vector3<TypeParam> v3(static_cast<TypeParam>(200), static_cast<TypeParam>(300), static_cast<TypeParam>(500));
	ror::Vector3<TypeParam> v3_res(static_cast<TypeParam>(-0.0945111), static_cast<TypeParam>(-0.122537), static_cast<TypeParam>(-0.0789223));

	ror::Matrix4<TypeParam> mat_res{static_cast<TypeParam>(0.4721174), static_cast<TypeParam>(-0.8764508), static_cast<TypeParam>(0.0945471), static_cast<TypeParam>(1.3300000),
	                                static_cast<TypeParam>(-0.6924389), static_cast<TypeParam>(-0.3023295), static_cast<TypeParam>(0.6550765), static_cast<TypeParam>(0.7040000),
	                                static_cast<TypeParam>(-0.5455579), static_cast<TypeParam>(-0.3747412), static_cast<TypeParam>(-0.7496236), static_cast<TypeParam>(7.0000000),
	                                static_cast<TypeParam>(10.1977368), static_cast<TypeParam>(-33.9938908), static_cast<TypeParam>(-154.51025390625), static_cast<TypeParam>(0.0000000)};

	auto res = mat_res.perspective_transform(v3);

	test_vector3_equal(v3_res, res);
}

TYPED_TEST(Matrix4TestSigned, matrix_assosiative)
{
	ror::Matrix4<TypeParam> m0{};
	ror::Matrix4<TypeParam> m1{};
	ror::Matrix4<TypeParam> m2{};
	ror::Matrix4<TypeParam> m3{};

	for (size_t j = 0; j < 16; ++j)
	{
		m0.m_values[j] = ror::random<TypeParam>(-10.0f, 10.0f);
		m1.m_values[j] = ror::random<TypeParam>(-10.0f, 10.0f);
		m2.m_values[j] = ror::random<TypeParam>(-10.0f, 10.0f);
		m3.m_values[j] = ror::random<TypeParam>(-10.0f, 10.0f);
	}

	ror::Matrix4<TypeParam> res0{};
	ror::Matrix4<TypeParam> res1{};

	res0 = m0 * (m1 * (m2 * m3));
	res1 = ((m0 * m1) * m2) * m3;

#define test_matrix4_equal_relaxed(a, b)                                                                               \
	{                                                                                                                  \
		const uint32_t matrix_size = 16;                                                                               \
		for (uint32_t index = 0; index < matrix_size; ++index)                                                         \
		{                                                                                                              \
			EXPECT_NEAR(static_cast<double64_t>(a.m_values[index]), static_cast<double64_t>(b.m_values[index]), 0.01); \
		}                                                                                                              \
	}                                                                                                                  \
	(void) 1

	test_matrix4_equal_relaxed(res0, res1);
}

}        // namespace ror_test
