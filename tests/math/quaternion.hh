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

#include "quaternion.hpp"

namespace ror_test
{
const int QUATERNION_SIZE = 4;

template <class _type>
void QuaternionTest<_type>::SetUp()
{
	this->m_quaternion1 = ror::Quaternion<_type>(static_cast<_type>(1), static_cast<_type>(2), static_cast<_type>(3), static_cast<_type>(4));
	this->m_quaternion2 = ror::Quaternion<_type>(static_cast<_type>(2), static_cast<_type>(4), static_cast<_type>(6), static_cast<_type>(8));
}

template <class _type>
void QuaternionTest<_type>::TearDown()
{}

TYPED_TEST(QuaternionTest, constructors_work)
{
	ror::Quaternion<TypeParam> quaternion1;        // identity quat
	ror::Quaternion<TypeParam> quaternion2(static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4));
	ror::Quaternion<TypeParam> quaternion3(quaternion2);

	ror::Matrix3<TypeParam> matrix1_3x3(0.9098298f, -0.3887649f, 0.1451605f,
	                                    0.4052595f, 0.7571089f, -0.5123971f,
	                                    0.0892997f, 0.5250219f, 0.8463910f);

	ror::Matrix4<TypeParam>    matrix1_4x4(matrix1_3x3);
	ror::Quaternion<TypeParam> quaternion4(matrix1_3x3);
	ror::Quaternion<TypeParam> quaternion5(matrix1_3x3);
	ror::Quaternion<TypeParam> quaternion6(-0.2767354f, -0.0149011f, -0.211809f, 0.9371939f);

	ror::Quaternion<TypeParam> quaternion7(matrix1_4x4);

	EXPECT_DOUBLE_EQ(quaternion2.x, 1);
	EXPECT_DOUBLE_EQ(quaternion2.y, 2);
	EXPECT_DOUBLE_EQ(quaternion2.z, 3);
	EXPECT_DOUBLE_EQ(quaternion2.w, 4);

	test_quaternion_equal(quaternion2, quaternion3);
	test_quaternion_equal(quaternion4, quaternion5);
	test_quaternion_equal(quaternion5, quaternion6);
	test_quaternion_equal(quaternion7, quaternion6);
	test_quaternion_equal(quaternion7, quaternion5);
}

TYPED_TEST(QuaternionTest, pointer_operators)
{
	TypeParam       *pointerToValue;
	const TypeParam *pointerToValueConst;

	pointerToValue      = static_cast<TypeParam *>(this->m_quaternion1);
	pointerToValueConst = static_cast<const TypeParam *>(this->m_quaternion2);

	EXPECT_DOUBLE_EQ(this->m_quaternion1.x, pointerToValue[0]);
	EXPECT_DOUBLE_EQ(this->m_quaternion1.y, pointerToValue[1]);
	EXPECT_DOUBLE_EQ(this->m_quaternion1.z, pointerToValue[2]);
	EXPECT_DOUBLE_EQ(this->m_quaternion1.w, pointerToValue[3]);

	EXPECT_DOUBLE_EQ(this->m_quaternion2.x, pointerToValueConst[0]);
	EXPECT_DOUBLE_EQ(this->m_quaternion2.y, pointerToValueConst[1]);
	EXPECT_DOUBLE_EQ(this->m_quaternion2.z, pointerToValueConst[2]);
	EXPECT_DOUBLE_EQ(this->m_quaternion2.w, pointerToValueConst[3]);
}

TYPED_TEST(QuaternionTest, rotation_axis_angle)
{
	{
		ror::Matrix3<TypeParam> matrix0(0.9205049f, 0.0000000f, -0.3907311f,
		                                0.0000000f, 1.0000000f, 0.0000000f,
		                                0.3907311f, 0.0000000f, 0.9205049f);

		ror::Quaternion<TypeParam> quaternionFromMatrix(matrix0);
		ror::AxisAngle<TypeParam>  axis_angle(ror::Vector3<TypeParam>(0.0f, 1.0f, 0.0f), ror::to_radians(23.0f));
		ror::Quaternion<TypeParam> quaternionFromAxisAngle(axis_angle);
		ror::Quaternion<TypeParam> quaternionFromAxisAngle_res(0.0f, 0.1993679f, 0.0f, 0.9799247f);

		test_quaternion_equal(quaternionFromAxisAngle, quaternionFromMatrix);
		test_quaternion_equal(quaternionFromAxisAngle, quaternionFromAxisAngle_res);

		ror::Matrix3<TypeParam> matrix2(quaternionFromAxisAngle);

		test_matrix3_equal(matrix2, matrix0);
	}
	{
		// test for quaternion from two vectors
		ror::Vector3<TypeParam> axis(0.0f, 1.0f, 0.0f);

		ror::Vector3<TypeParam> vector1(1.0f, 0.0f, 1.0f);
		ror::Vector3<TypeParam> vector2(0.0f, 0.0f, 1.0f);

		TypeParam radians1 = ror::to_radians(45.0f);

		ror::AxisAngle<TypeParam> axis_angle1(axis, radians1);

		ror::Quaternion<TypeParam> quaternionFromAxisAngle1(axis_angle1);
		ror::Quaternion<TypeParam> quaternionFromTwoVectors(vector2, vector1);

		test_quaternion_equal(quaternionFromTwoVectors, quaternionFromAxisAngle1);
	}
	{
		// test for quaternion from two vectors
		ror::Vector3<TypeParam> vector1(1.3f, 5.2f, -1.3f);
		ror::Vector3<TypeParam> vector2(1.0f, 0.0f, 0.0f);

		vector1.normalize();
		vector2.normalize();
		ror::Vector3<TypeParam> axis = vector1.cross_product(vector2);

		TypeParam radians1 = ror::angle(vector1, vector2);

		ror::AxisAngle<TypeParam> axis_angle1(axis, radians1);

		ror::Quaternion<TypeParam> quaternionFromAxisAngle1(axis_angle1);
		ror::Quaternion<TypeParam> quaternionFromTwoVectors(vector1, vector2);

		EXPECT_NEAR(quaternionFromAxisAngle1.y, quaternionFromTwoVectors.y, 0.1);
		EXPECT_NEAR(quaternionFromAxisAngle1.z, quaternionFromTwoVectors.z, 0.1);
	}
}

TYPED_TEST(QuaternionTest, rotation_euler_angles)
{
	{
		ror::AxisAngle<TypeParam>  axis_angle(ror::Vector3<TypeParam>(0.0f, 0.0f, 1.0f), ror::to_radians(23.0f));
		ror::Quaternion<TypeParam> quaternionFromAxisAngle(axis_angle);
		ror::Quaternion<TypeParam> quaternionFromEulerAngle(ror::EulerAngle<TypeParam>(0.0f, ror::to_radians(23.0f), 0.0f));

		test_quaternion_equal(quaternionFromAxisAngle, quaternionFromEulerAngle);
	}
	{
		ror::AxisAngle<TypeParam>  axis_angle(ror::Vector3<TypeParam>(0.0f, 1.0f, 0.0f), ror::to_radians(23.0f));
		ror::Quaternion<TypeParam> quaternionFromAxisAngle(axis_angle);
		ror::Quaternion<TypeParam> quaternionFromEulerAngle(ror::EulerAngle<TypeParam>(ror::to_radians(23.0f), 0.0f, 0.0f));

		test_quaternion_equal(quaternionFromAxisAngle, quaternionFromEulerAngle);
	}
	{
		ror::AxisAngle<TypeParam>  axis_angle(ror::Vector3<TypeParam>(1.0f, 0.0f, 0.0f), ror::to_radians(23.0f));
		ror::Quaternion<TypeParam> quaternionFromAxisAngle(axis_angle);
		ror::Quaternion<TypeParam> quaternionFromEulerAngle(ror::EulerAngle<TypeParam>(0.0f, 0.0f, ror::to_radians(23.0f)));

		test_quaternion_equal(quaternionFromAxisAngle, quaternionFromEulerAngle);
	}
	{
		ror::AxisAngle<TypeParam>  axis_angle(ror::Vector3<TypeParam>(0.3828782f, 0.3462004f, 0.8564751f), 1.2948347f);
		ror::Quaternion<TypeParam> quaternionFromAxisAngle(axis_angle);
		ror::Quaternion<TypeParam> quaternionFromEulerAngle(ror::EulerAngle<TypeParam>(ror::to_radians(14.0f), ror::to_radians(67.0f), ror::to_radians(23.0f)));
		ror::Quaternion<TypeParam> result(0.2309247f, 0.2088033f, 0.5165644f, 0.7976441f);

		test_quaternion_equal(quaternionFromEulerAngle, result);
		test_quaternion_equal(quaternionFromAxisAngle, result);
	}
}

TYPED_TEST(QuaternionTest, multiplication_equal_operator_scalar)
{
	ror::Quaternion<TypeParam> quaternion0(static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4));

	this->m_quaternion1 *= static_cast<TypeParam>(2.0f);

	test_quaternion_equal(this->m_quaternion1, this->m_quaternion2);

	this->m_quaternion1 *= static_cast<TypeParam>(0.5f);

	test_quaternion_equal(this->m_quaternion1, quaternion0);
}

TYPED_TEST(QuaternionTest, multiplication_operator_scalar)
{
	ror::Quaternion<TypeParam> quaternion0;

	quaternion0 = this->m_quaternion1 * static_cast<TypeParam>(2.0f);
	test_quaternion_equal(this->m_quaternion2, quaternion0);

	quaternion0 = this->m_quaternion2 * static_cast<TypeParam>(0.5f);
	test_quaternion_equal(this->m_quaternion1, quaternion0);

	// Uniary negation operator
	this->m_quaternion2 = ror::Quaternion<TypeParam>(-1.0f, -2.0f, -3.0f, -4.0f);
	quaternion0         = this->m_quaternion1 * static_cast<TypeParam>(-1.0f);
	test_quaternion_equal(this->m_quaternion2, quaternion0);
}

TYPED_TEST(QuaternionTest, multiplication_operator_quaternion)
{
	ror::Matrix3<TypeParam> matrix1(0.868672728538513180f, -0.457449704408645630f, 0.190124601125717160f,
	                                0.483165979385375980f, 0.867081820964813230f, -0.121324792504310610f,
	                                -0.109353587031364440f, 0.197253301739692690f, 0.974234402179718020f);

	ror::Matrix3<TypeParam> matrix2(0.896624565124511720f, -0.438898861408233640f, 0.058584671467542648f,
	                                0.442666232585906980f, 0.891638100147247310f, -0.095016010105609894f,
	                                -0.010533910244703293f, 0.111127145588397980f, 0.993750333786010740f);

	ror::Matrix3<TypeParam> matrix3(0.993906497955322270f, -0.028410637751221657f, 0.106501497328281400f,
	                                0.037397608160972595f, 0.995817542076110840f, -0.083359383046627045f,
	                                -0.103687770664691930f, 0.086834348738193512f, 0.990812063217163090f);

	ror::Matrix3<TypeParam> matrix4 = matrix3 * matrix2;

	ror::Quaternion<TypeParam> quaternionFromMatrix1(matrix1);
	ror::Quaternion<TypeParam> quaternionFromMatrix2(matrix2);
	ror::Quaternion<TypeParam> quaternionFromMatrix3(matrix3);
	ror::Quaternion<TypeParam> quaternionFromMatrix4(matrix4);

	ror::Quaternion<TypeParam> quaternionMultiplicationResult = quaternionFromMatrix3 * quaternionFromMatrix2;

	test_quaternion_equal(quaternionMultiplicationResult, quaternionFromMatrix1);
	test_quaternion_equal(quaternionFromMatrix4, quaternionFromMatrix1);
}

TYPED_TEST(QuaternionTest, arthimatic_operators_quaternions)
{
	{
		ror::Quaternion<TypeParam> quaternion1 = this->m_quaternion1 + this->m_quaternion1;
		ror::Quaternion<TypeParam> quaternion2 = this->m_quaternion2;
		test_quaternion_equal(quaternion1, quaternion2);

		ror::Quaternion<TypeParam> quaternion3 = this->m_quaternion2 - this->m_quaternion1;
		test_quaternion_equal(quaternion3, this->m_quaternion1);
	}
	{
		ror::Quaternion<TypeParam> quaternion1 = this->m_quaternion1;
		quaternion1 += this->m_quaternion1;
		test_quaternion_equal(quaternion1, this->m_quaternion2);

		this->m_quaternion2 -= this->m_quaternion1;
		test_quaternion_equal(this->m_quaternion2, this->m_quaternion1);
	}
}

TYPED_TEST(QuaternionTest, arthimatic_operators_scalars)
{
	{
		ror::Quaternion<TypeParam> quaternion1 = this->m_quaternion1 + static_cast<TypeParam>(2);
		ror::Quaternion<TypeParam> quaternion2(3.0f, 4.0f, 5.0f, 6.0f);
		test_quaternion_equal(quaternion1, quaternion2);

		ror::Quaternion<TypeParam> quaternion3 = quaternion2 - static_cast<TypeParam>(2);
		test_quaternion_equal(quaternion3, this->m_quaternion1);
	}
	{
		ror::Quaternion<TypeParam> quaternion1 = this->m_quaternion1;
		quaternion1 += static_cast<TypeParam>(2);
		ror::Quaternion<TypeParam> quaternion2(3.0f, 4.0f, 5.0f, 6.0f);
		test_quaternion_equal(quaternion1, quaternion2);

		ror::Quaternion<TypeParam> quaternion3 = quaternion2;
		quaternion3 -= static_cast<TypeParam>(2);
		test_quaternion_equal(quaternion3, this->m_quaternion1);
	}
}

TYPED_TEST(QuaternionTest, assignment_operator_scalar_array)
{
	TypeParam *pointerToValue = new TypeParam[4];

	for (int index = 0; index < 4; ++index)
	{
		pointerToValue[index] = static_cast<TypeParam>(index + 1);
	}

	// test_quaternion_equal(quaternion1, quaternion2); // should be not equal

	this->m_quaternion2 = pointerToValue;
	test_quaternion_equal(this->m_quaternion1, this->m_quaternion2);
}

TYPED_TEST(QuaternionTest, assignment_operator_quaternion)
{
	this->m_quaternion2 = this->m_quaternion1;
	test_quaternion_equal(this->m_quaternion1, this->m_quaternion2);
}

TYPED_TEST(QuaternionTest, multiplication_equal_operator_quaternion)
{
	ror::Matrix3<TypeParam> matrix1(0.868672728538513180f, -0.457449704408645630f, 0.190124601125717160f,
	                                0.483165979385375980f, 0.867081820964813230f, -0.121324792504310610f,
	                                -0.109353587031364440f, 0.197253301739692690f, 0.974234402179718020f);

	ror::Matrix3<TypeParam> matrix2(0.896624565124511720f, -0.438898861408233640f, 0.058584671467542648f,
	                                0.442666232585906980f, 0.891638100147247310f, -0.095016010105609894f,
	                                -0.010533910244703293f, 0.111127145588397980f, 0.993750333786010740f);

	ror::Matrix3<TypeParam> matrix3(0.993906497955322270f, -0.028410637751221657f, 0.106501497328281400f,
	                                0.037397608160972595f, 0.995817542076110840f, -0.083359383046627045f,
	                                -0.103687770664691930f, 0.086834348738193512f, 0.990812063217163090f);

	ror::Quaternion<TypeParam> quaternionFromMatrix1(matrix1);
	ror::Quaternion<TypeParam> quaternionFromMatrix2(matrix2);
	ror::Quaternion<TypeParam> quaternionFromMatrix3(matrix3);

	quaternionFromMatrix3 *= quaternionFromMatrix2;
	test_quaternion_equal(quaternionFromMatrix3, quaternionFromMatrix1);
}

TYPED_TEST(QuaternionTest, identity)
{
	ror::Quaternion<TypeParam> quaternion1;
	ror::Quaternion<TypeParam> quaternion2(static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1));

	ror::Quaternion<TypeParam> quaternion3;
	quaternion3.identity();

	test_quaternion_equal(quaternion1, quaternion2);
	test_quaternion_equal(quaternion1, quaternion3);
}

TYPED_TEST(QuaternionTest, conjugate)
{
	ror::Quaternion<TypeParam> quaternion0;
	ror::Quaternion<TypeParam> quaternion3(static_cast<TypeParam>(-1), static_cast<TypeParam>(-2), static_cast<TypeParam>(-3), static_cast<TypeParam>(4));

	quaternion0 = this->m_quaternion1.conjugate();
	test_quaternion_equal(quaternion0, quaternion3);
}

TYPED_TEST(QuaternionTest, magnitude)
{
	{
		ror::Quaternion<TypeParam> quaternion2(static_cast<TypeParam>(2), static_cast<TypeParam>(2), static_cast<TypeParam>(2), static_cast<TypeParam>(2));
		TypeParam                  quaternionMagnitude = quaternion2.magnitude();
		EXPECT_DOUBLE_EQ(quaternionMagnitude, static_cast<TypeParam>(4));
	}
	{
		ror::Quaternion<TypeParam> quaternion2(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(8), static_cast<TypeParam>(9));
		TypeParam                  quaternionMagnitude  = quaternion2.magnitude_squared();
		TypeParam                  quaternionDotProduct = quaternion2.dot_product(quaternion2);

		EXPECT_DOUBLE_EQ(quaternionMagnitude, static_cast<TypeParam>(4 + 16 + 64 + 81));
		EXPECT_DOUBLE_EQ(quaternionDotProduct, static_cast<TypeParam>(4 + 16 + 64 + 81));
	}
}

TYPED_TEST(QuaternionTest, normalize)
{
	ror::Quaternion<TypeParam> quaternion1(static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(5), static_cast<TypeParam>(5));
	ror::Quaternion<TypeParam> quaternion2 = quaternion1.normalized();

	EXPECT_FALSE(std::abs(quaternion1.magnitude() - 1.0f) < ror::ror_epsilon);

	quaternion1.normalize();

	EXPECT_TRUE(std::abs(quaternion2.magnitude() - 1.0f) < ror::ror_epsilon);
	EXPECT_TRUE(std::abs(quaternion1.magnitude() - 1.0f) < ror::ror_epsilon);
}

TYPED_TEST(QuaternionTest, rotation_get_axis_angle)
{
	ror::Vector3<TypeParam> axis(3.0f, 2.0f, 1.0f);
	TypeParam               angle = ror::ror_sixth_pi;
	ror::Vector3<TypeParam> axisFromQuaternion;
	TypeParam               angleFromQuaternion;

	ror::AxisAngle<TypeParam>  axis_angle(axis, angle);
	ror::Quaternion<TypeParam> quaternionFromAxisAngle(axis_angle);

	ror::AxisAngle<TypeParam> axis_angle_res;
	axis_angle_res = quaternionFromAxisAngle.axis_angle();

	axis.normalize();

	EXPECT_TRUE(axis == axis_angle_res.m_axis);
	EXPECT_NEAR(angle, axis_angle_res.m_angle_radians, test_epsilon);

	axisFromQuaternion  = quaternionFromAxisAngle.axis();
	angleFromQuaternion = quaternionFromAxisAngle.angle();

	EXPECT_TRUE(axis == axisFromQuaternion);
	EXPECT_NEAR(angle, angleFromQuaternion, test_epsilon);
}

TYPED_TEST(QuaternionTest, rotation_around_axis)
{
	TypeParam angle = ror::ror_sixth_pi;

	{
		ror::Vector3<TypeParam>    axis(1.0f, 0.0f, 0.0f);
		ror::AxisAngle<TypeParam>  axis_angle(axis, angle);
		ror::Quaternion<TypeParam> quaternionFromAxisAngle(axis_angle);
		ror::Quaternion<TypeParam> quaternion = ror::quaternion_rotation_around_x(angle);

		test_quaternion_equal(quaternion, quaternionFromAxisAngle);
	}

	{
		ror::Vector3<TypeParam>    axis(0.0f, 1.0f, 0.0f);
		ror::AxisAngle<TypeParam>  axis_angle(axis, angle);
		ror::Quaternion<TypeParam> quaternionFromAxisAngle(axis_angle);
		ror::Quaternion<TypeParam> quaternion = ror::quaternion_rotation_around_y(angle);

		test_quaternion_equal(quaternion, quaternionFromAxisAngle);
	}
	{
		ror::Vector3<TypeParam>    axis(0.0f, 0.0f, 1.0f);
		ror::AxisAngle<TypeParam>  axis_angle(axis, angle);
		ror::Quaternion<TypeParam> quaternionFromAxisAngle(axis_angle);
		ror::Quaternion<TypeParam> quaternion = ror::quaternion_rotation_around_z(angle);

		test_quaternion_equal(quaternion, quaternionFromAxisAngle);
	}
}

TYPED_TEST(QuaternionTest, invert)
{
	ror::Matrix3<TypeParam> matrix1(0.9205048680305481f, 0.0f, 0.39073112607002258f,
	                                0.0f, 1.0f, 0.0f,
	                                -0.39073112607002258f, 0.0f, 0.9205048680305481f);

	ror::Matrix3<TypeParam> matrix1Inverse;

	ror::Quaternion<TypeParam> quaternionFromMatrix1(matrix1);
	ror::Quaternion<TypeParam> quaternionFromMatrix1QuaternionInverse;

	ASSERT_TRUE(quaternionFromMatrix1.inverse(quaternionFromMatrix1QuaternionInverse));
	ASSERT_TRUE(matrix1.inverse(matrix1Inverse));

	ror::Quaternion<TypeParam> quaternionFromMatrix1Inverse(matrix1Inverse);
	test_quaternion_equal(quaternionFromMatrix1QuaternionInverse, quaternionFromMatrix1Inverse);

	ror::Quaternion<TypeParam> qinvtemp;

	ror::AxisAngle<TypeParam>  axis_angle(ror::Vector3<TypeParam>(0.0f, 0.0f, 1.0f), ror::to_radians(23.0f));
	ror::AxisAngle<TypeParam>  axis_angle_inverse(ror::Vector3<TypeParam>(0.0f, 0.0f, 1.0f), ror::to_radians(-23.0f));
	ror::Quaternion<TypeParam> quaternionFromAxisAngle(axis_angle);
	ror::Quaternion<TypeParam> quaternionFromAxisAngleInverse(axis_angle_inverse);

	ASSERT_TRUE(quaternionFromAxisAngle.inverse(qinvtemp));
	test_quaternion_equal(quaternionFromAxisAngleInverse, qinvtemp);

	ASSERT_TRUE(quaternionFromAxisAngle.invert());
	test_quaternion_equal(quaternionFromAxisAngleInverse, quaternionFromAxisAngle);

	{
		ror::Vector3<TypeParam> eu1(12.0f, -10.0f, 23.0f);
		ror::Matrix3<TypeParam> rotx = ror::matrix3_rotation_around_x(eu1.x);
		ror::Matrix3<TypeParam> roty = ror::matrix3_rotation_around_y(eu1.y);
		ror::Matrix3<TypeParam> rotz = ror::matrix3_rotation_around_z(eu1.z);

		// ror::Vector3<TypeParam> eu1_inv(-12.0f, 10.0f, -23.0f);
		ror::Matrix3<TypeParam> rotx_inv = ror::matrix3_rotation_around_x(-eu1.x);
		ror::Matrix3<TypeParam> roty_inv = ror::matrix3_rotation_around_y(-eu1.y);
		ror::Matrix3<TypeParam> rotz_inv = ror::matrix3_rotation_around_z(-eu1.z);

		ror::Matrix3<TypeParam> rot = roty * rotz * rotx;
		ror::Matrix3<TypeParam> rot_inv_res;
		ror::Matrix3<TypeParam> rot_inv = rotx_inv * rotz_inv * roty_inv;

		ASSERT_TRUE(rot.inverse(rot_inv_res));

		test_matrix3_equal(rot_inv_res, rot_inv);

		ror::Quaternion<TypeParam> quaternionFromEulerAngle(eu1);
		ror::Quaternion<TypeParam> quaternionFromEulerAngleInverse(rot_inv);

		ASSERT_TRUE(quaternionFromEulerAngle.inverse(qinvtemp));
		test_quaternion_equal(quaternionFromEulerAngleInverse, qinvtemp);
	}
}

TYPED_TEST(QuaternionTest, transform_vector)
{
	ror::Vector3<TypeParam> eu1(12.0f, -10.0f, 23.0f);
	ror::Matrix3<TypeParam> rotx = ror::matrix3_rotation_around_x(ror::to_radians(eu1.x));
	ror::Matrix3<TypeParam> roty = ror::matrix3_rotation_around_y(ror::to_radians(eu1.y));
	ror::Matrix3<TypeParam> rotz = ror::matrix3_rotation_around_z(ror::to_radians(eu1.z));

	ror::Matrix3<TypeParam>    rot = roty * rotz * rotx;
	ror::Quaternion<TypeParam> q(rot);

	{
		ror::Vector3<TypeParam> v1(2.0f, 3.0f, 4.0f);
		auto                    v1_m_res = rot * v1;
		auto                    v1_q_res = q * v1;
		test_vector3_equal(v1_m_res, v1_q_res);
	}
	{
		ror::Vector3<TypeParam> v1(-2.0f, 13.0f, -3.0f);
		auto                    v1_m_res = rot * v1;
		auto                    v1_q_res = q * v1;
		test_vector3_equal(v1_m_res, v1_q_res);
	}
	{
		ror::Vector3<TypeParam> v1(10.0f, -103.0f, -8.0f);
		auto                    v1_m_res = rot * v1;
		auto                    v1_q_res = q * v1;
		test_vector3_equal(v1_m_res, v1_q_res);
	}
}

TYPED_TEST(QuaternionTest, transform_interpolation)
{
	ror::Quaternion<TypeParam> q1(-0.844362f, 0.514381f, -0.108946f, -0.102932f);
	ror::Quaternion<TypeParam> q2(0.698255f, 0.700878f, 0.098375f, 0.107389f);

	{
		auto qs = ror::quaternion_slerp(q1, q2, static_cast<TypeParam>(0.0f));
		test_quaternion_equal(qs, q1);
	}
	{
		auto qs = ror::quaternion_slerp(q1, q2, static_cast<TypeParam>(1.0f));
		qs *= -1;        // negative of a quaternion represents the same rotation
		test_quaternion_equal(qs, q2);
	}
	{
		auto                       qs = ror::quaternion_slerp(q1, q2, static_cast<TypeParam>(0.1f));
		ror::Quaternion<TypeParam> qsres(-0.903051398717475f, 0.397321541966272f, -0.117642117523596f, -0.11310794005944f);
		test_quaternion_equal(qs, qsres);
	}
	{
		auto                       qs = ror::quaternion_slerp(q1, q2, static_cast<TypeParam>(0.4f));
		ror::Quaternion<TypeParam> qsres(-0.982539943856746f, 0.0135099785659152f, -0.131076373212593f, -0.131344134926088f);
		test_quaternion_equal(qs, qsres);
	}
	{
		auto                       qs = ror::quaternion_slerp(q1, q2, static_cast<TypeParam>(0.7f));
		ror::Quaternion<TypeParam> qsres(-0.91057696285685f, -0.372384147086745f, -0.124306141339709f, -0.129334569856764f);
		test_quaternion_equal(qs, qsres);
	}
	{
		auto                       qs = ror::quaternion_slerp(q1, q2, static_cast<TypeParam>(0.9f));
		ror::Quaternion<TypeParam> qsres(-0.783006930918638f, -0.601159986092791f, -0.108956771053529f, -0.116769904255543f);
		test_quaternion_equal(qs, qsres);
	}
	{
		ror::Quaternion<TypeParam> q3(0.698257f, 0.700872f, 0.098371f, 0.107386f);
		auto                       qs = ror::quaternion_slerp(q2, q3, static_cast<TypeParam>(0.5f));
		test_quaternion_equal(qs, q2);
		test_quaternion_equal(qs, q3);
	}
}

TYPED_TEST(QuaternionTest, angle)
{
	ror::Quaternion<TypeParam> q1(0.1115928f, 0.2231855f, 0.2901412f, 0.9238795f);
	ror::Quaternion<TypeParam> q2(0.0876876f, 0.1753752f, 0.2279877f, 0.953717f);

	TypeParam angle = quaternion_angle(q1, q2);

	EXPECT_NEAR(angle, static_cast<TypeParam>(ror::to_radians(10.0f)), test_epsilon);
}

TYPED_TEST(QuaternionTest, gltf_example)
{
	ror::Matrix4f matrix{2.0f, 0.0f, 0.0f, 0.0f,
	                     0.0f, 0.866f, 0.5f, 0.0f,
	                     0.0f, -0.25f, 0.433f, 0.0f,
	                     10.0f, 20.0f, 30.0f, 1.0f};

	ror::Vector3f    translation{10.0f, 20.0f, 30.0f};
	ror::Quaternionf rotation{0.259f, 0.0f, 0.0f, 0.966f};
	ror::Vector3f    scale{2.0f, 1.0f, 0.5f};

	auto translation_matrix = ror::matrix4_translation(translation);
	auto rotation_matrix    = ror::matrix4_rotation(rotation);
	auto scale_matrix       = ror::matrix4_scaling(scale);

	auto local_transform = translation_matrix * rotation_matrix * scale_matrix;

	constexpr double higher_test_epsilon = 0.001;

#define test_matrix4_equal_higher_epsilon(a, b)                                                                                       \
	{                                                                                                                                 \
		const uint32_t matrix_size = 16;                                                                                              \
		for (uint32_t index = 0; index < matrix_size; ++index)                                                                        \
		{                                                                                                                             \
			EXPECT_NEAR(static_cast<double64_t>(a.m_values[index]), static_cast<double64_t>(b.m_values[index]), higher_test_epsilon); \
		}                                                                                                                             \
	}                                                                                                                                 \
	(void) 1

	test_matrix4_equal_higher_epsilon(matrix, local_transform);
}

void node_transform_test(
    ror::EulerAngle<float32_t> angle_20,
    ror::EulerAngle<float32_t> angle_30,
    ror::EulerAngle<float32_t> angle_25,
    ror::EulerAngle<float32_t> angle_45,
    ror::EulerAngle<float32_t> angle_15,
    ror::EulerAngle<float32_t> angle_4,

    ror::Matrix4f m_rotation_20,
    ror::Matrix4f m_rotation_30,
    ror::Matrix4f m_rotation_25,
    ror::Matrix4f m_rotation_45,
    ror::Matrix4f m_rotation_15,
    ror::Matrix4f m_rotation_4)
{
	ror::Quaternionf q_rotation_20{angle_20};
	ror::Quaternionf q_rotation_30{angle_30};
	ror::Quaternionf q_rotation_25{angle_25};
	ror::Quaternionf q_rotation_45{angle_45};
	ror::Quaternionf q_rotation_15{angle_15};
	ror::Quaternionf q_rotation_4{angle_4};

	ror::Quaternionf q_m_rotation_20{m_rotation_20};
	ror::Quaternionf q_m_rotation_30{m_rotation_30};
	ror::Quaternionf q_m_rotation_25{m_rotation_25};
	ror::Quaternionf q_m_rotation_45{m_rotation_45};
	ror::Quaternionf q_m_rotation_15{m_rotation_15};
	ror::Quaternionf q_m_rotation_4{m_rotation_4};

	ror::Matrix4f m_q_rotation_20{q_rotation_20};
	ror::Matrix4f m_q_rotation_30{q_rotation_30};
	ror::Matrix4f m_q_rotation_25{q_rotation_25};
	ror::Matrix4f m_q_rotation_45{q_rotation_45};
	ror::Matrix4f m_q_rotation_15{q_rotation_15};
	ror::Matrix4f m_q_rotation_4{q_rotation_4};

	ror::Quaternionf q_rotation   = q_rotation_4 * q_rotation_15 * q_rotation_45 * q_rotation_25 * q_rotation_30 * q_rotation_20;
	ror::Quaternionf q_m_rotation = q_m_rotation_4 * q_m_rotation_15 * q_m_rotation_45 * q_m_rotation_25 * q_m_rotation_30 * q_m_rotation_20;
	ror::Matrix4f    m_rotation   = m_rotation_4 * m_rotation_15 * m_rotation_45 * m_rotation_25 * m_rotation_30 * m_rotation_20;
	ror::Matrix4f    m_q_rotation = m_q_rotation_4 * m_q_rotation_15 * m_q_rotation_45 * m_q_rotation_25 * m_q_rotation_30 * m_q_rotation_20;

	ror::Matrix4f m_result = m_rotation;
	ror::Matrix4f q_result{q_rotation};
	ror::Matrix4f q_m_result{q_m_rotation};
	ror::Matrix4f m_q_result{m_q_rotation};

	test_matrix4_equal(m_result, q_result);
	test_matrix4_equal(m_q_result, q_result);
	test_matrix4_equal(m_q_result, m_result);
	test_matrix4_equal(m_q_result, q_m_result);
}

TYPED_TEST(QuaternionTest, node_traversal_test)
{
	float r20 = ror::to_radians(20.0f);
	float r30 = ror::to_radians(-30.0f);
	float r25 = ror::to_radians(25.0f);
	float r45 = ror::to_radians(45.0f);
	float r15 = ror::to_radians(-15.0f);
	float r4  = ror::to_radians(4.0f);

	{
		ror::EulerAngle<float32_t> angle_20{r20, 0, 0};        // angle around y
		ror::EulerAngle<float32_t> angle_30{r30, 0, 0};        // angle around y
		ror::EulerAngle<float32_t> angle_25{r25, 0, 0};        // angle around y
		ror::EulerAngle<float32_t> angle_45{r45, 0, 0};        // angle around y
		ror::EulerAngle<float32_t> angle_15{r15, 0, 0};        // angle around y
		ror::EulerAngle<float32_t> angle_4{r4, 0, 0};          // angle around y

		ror::Matrix4f m_rotation_20 = ror::matrix4_rotation_around_y(r20);
		ror::Matrix4f m_rotation_30 = ror::matrix4_rotation_around_y(r30);
		ror::Matrix4f m_rotation_25 = ror::matrix4_rotation_around_y(r25);
		ror::Matrix4f m_rotation_45 = ror::matrix4_rotation_around_y(r45);
		ror::Matrix4f m_rotation_15 = ror::matrix4_rotation_around_y(r15);
		ror::Matrix4f m_rotation_4  = ror::matrix4_rotation_around_y(r4);

		node_transform_test(
		    angle_20,
		    angle_30,
		    angle_25,
		    angle_45,
		    angle_15,
		    angle_4,
		    m_rotation_20,
		    m_rotation_30,
		    m_rotation_25,
		    m_rotation_45,
		    m_rotation_15,
		    m_rotation_4);
	}

	{
		ror::EulerAngle<float32_t> angle_20{0, r20, 0};        // angle around z
		ror::EulerAngle<float32_t> angle_30{0, r30, 0};        // angle around z
		ror::EulerAngle<float32_t> angle_25{0, r25, 0};        // angle around z
		ror::EulerAngle<float32_t> angle_45{0, r45, 0};        // angle around z
		ror::EulerAngle<float32_t> angle_15{0, r15, 0};        // angle around z
		ror::EulerAngle<float32_t> angle_4{0, r4, 0};          // angle around z

		ror::Matrix4f m_rotation_20 = ror::matrix4_rotation_around_z(r20);
		ror::Matrix4f m_rotation_30 = ror::matrix4_rotation_around_z(r30);
		ror::Matrix4f m_rotation_25 = ror::matrix4_rotation_around_z(r25);
		ror::Matrix4f m_rotation_45 = ror::matrix4_rotation_around_z(r45);
		ror::Matrix4f m_rotation_15 = ror::matrix4_rotation_around_z(r15);
		ror::Matrix4f m_rotation_4  = ror::matrix4_rotation_around_z(r4);

		node_transform_test(
		    angle_20,
		    angle_30,
		    angle_25,
		    angle_45,
		    angle_15,
		    angle_4,
		    m_rotation_20,
		    m_rotation_30,
		    m_rotation_25,
		    m_rotation_45,
		    m_rotation_15,
		    m_rotation_4);
	}

	{
		ror::EulerAngle<float32_t> angle_20{0, 0, r20};        // angle around x
		ror::EulerAngle<float32_t> angle_30{0, 0, r30};        // angle around x
		ror::EulerAngle<float32_t> angle_25{0, 0, r25};        // angle around x
		ror::EulerAngle<float32_t> angle_45{0, 0, r45};        // angle around x
		ror::EulerAngle<float32_t> angle_15{0, 0, r15};        // angle around x
		ror::EulerAngle<float32_t> angle_4{0, 0, r4};          // angle around x

		ror::Matrix4f m_rotation_20 = ror::matrix4_rotation_around_x(r20);
		ror::Matrix4f m_rotation_30 = ror::matrix4_rotation_around_x(r30);
		ror::Matrix4f m_rotation_25 = ror::matrix4_rotation_around_x(r25);
		ror::Matrix4f m_rotation_45 = ror::matrix4_rotation_around_x(r45);
		ror::Matrix4f m_rotation_15 = ror::matrix4_rotation_around_x(r15);
		ror::Matrix4f m_rotation_4  = ror::matrix4_rotation_around_x(r4);

		node_transform_test(
		    angle_20,
		    angle_30,
		    angle_25,
		    angle_45,
		    angle_15,
		    angle_4,
		    m_rotation_20,
		    m_rotation_30,
		    m_rotation_25,
		    m_rotation_45,
		    m_rotation_15,
		    m_rotation_4);
	}

	{
		ror::EulerAngle<float32_t> angle_0{r20, 0, r20}; 
		ror::EulerAngle<float32_t> angle_1{r30, 0, r30};
		ror::EulerAngle<float32_t> angle_2{r20, 0, r25};  
		ror::EulerAngle<float32_t> angle_3{r25, 0, r45}; 
		ror::EulerAngle<float32_t> angle_4{r20, 0, r15};
		ror::EulerAngle<float32_t> angle_5{r45, 0, r4};  

		ror::Matrix4f m_rotation_0 = ror::matrix4_rotation(angle_0);
		ror::Matrix4f m_rotation_1 = ror::matrix4_rotation(angle_1);
		ror::Matrix4f m_rotation_2 = ror::matrix4_rotation(angle_2);
		ror::Matrix4f m_rotation_3 = ror::matrix4_rotation(angle_3);
		ror::Matrix4f m_rotation_4 = ror::matrix4_rotation(angle_4);
		ror::Matrix4f m_rotation_5 = ror::matrix4_rotation(angle_5);

		node_transform_test(
		    angle_0,
		    angle_1,
		    angle_2,
		    angle_3,
		    angle_4,
		    angle_5,
		    m_rotation_0,
		    m_rotation_1,
		    m_rotation_2,
		    m_rotation_3,
		    m_rotation_4,
		    m_rotation_5);
	}
	{
		ror::EulerAngle<float32_t> angle_0{r20, r45, r20}; 
		ror::EulerAngle<float32_t> angle_1{r30, r15, r30};
		ror::EulerAngle<float32_t> angle_2{r20, r30, r25};  
		ror::EulerAngle<float32_t> angle_3{r25, r45, r45}; 
		ror::EulerAngle<float32_t> angle_4{r20, r20, r15};
		ror::EulerAngle<float32_t> angle_5{r45, r4, r4};  

		ror::Matrix4f m_rotation_0 = ror::matrix4_rotation(angle_0);
		ror::Matrix4f m_rotation_1 = ror::matrix4_rotation(angle_1);
		ror::Matrix4f m_rotation_2 = ror::matrix4_rotation(angle_2);
		ror::Matrix4f m_rotation_3 = ror::matrix4_rotation(angle_3);
		ror::Matrix4f m_rotation_4 = ror::matrix4_rotation(angle_4);
		ror::Matrix4f m_rotation_5 = ror::matrix4_rotation(angle_5);

		node_transform_test(
		    angle_0,
		    angle_1,
		    angle_2,
		    angle_3,
		    angle_4,
		    angle_5,
		    m_rotation_0,
		    m_rotation_1,
		    m_rotation_2,
		    m_rotation_3,
		    m_rotation_4,
		    m_rotation_5);
	}
}

}        // namespace ror_test
