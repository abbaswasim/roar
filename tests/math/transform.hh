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

#include "foundation/rorutilities.hpp"
#include "math/roreuler_angle.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorquaternion.hpp"
#include "math/rorvector3.hpp"
#include "transform.hpp"

namespace ror_test
{
template <class _type>
void TransformTest<_type>::SetUp()
{}

template <class _type>
void TransformTest<_type>::TearDown()
{}

TYPED_TEST(TransformTest, constructors)
{
	ror::Quaternion<TypeParam> oq(static_cast<TypeParam>(-0.2767354f),
								  static_cast<TypeParam>(-0.0149011f),
								  static_cast<TypeParam>(-0.211809f),
								  static_cast<TypeParam>(0.9371939f));

	ror::Vector3<TypeParam> ot(10, 20, 30);
	ror::Vector3<TypeParam> os(1, 2, 3);

	auto t4x4 = ror::matrix4_translation(ot.x, ot.y, ot.z);
	auto r4x4 = ror::matrix4_rotation(oq);
	auto s4x4 = ror::matrix4_scaling(os.x, os.y, os.z);

	auto mres = t4x4 * r4x4 * s4x4;

	{
		ror::Transform<TypeParam> q(oq, ot, os);

		auto r = q.rotation();
		auto t = q.translation();
		auto s = q.scale();

		test_vector3_equal(t, ot);
		test_vector3_equal(s, os);
		test_quaternion_equal(r, oq);
	}
	{
		ror::Matrix3<TypeParam>   m3x3(mres);
		ror::Transform<TypeParam> tm33(m3x3);

		auto r = tm33.rotation();
		auto t = tm33.translation();
		auto s = tm33.scale();

		test_vector3_equal(t, ror::zero_vector3f);
		test_vector3_equal(s, os);

		auto q_dot = std::abs(r.dot_product(oq));
		EXPECT_GE(q_dot, (1 - ror::ror_epsilon));
	}
	{
		ror::Matrix3x4<TypeParam> m3x4(mres);
		ror::Transform<TypeParam> tm34(m3x4);

		auto r = tm34.rotation();
		auto t = tm34.translation();
		auto s = tm34.scale();

		test_vector3_equal(t, ot);
		test_vector3_equal(s, os);
		auto q_dot = std::abs(r.dot_product(oq));
		EXPECT_GE(q_dot, (1 - ror::ror_epsilon));
	}
	{
		ror::Matrix4<TypeParam>   m4x4(mres);
		ror::Transform<TypeParam> tm44(m4x4);

		auto r = tm44.rotation();
		auto t = tm44.translation();
		auto s = tm44.scale();

		test_vector3_equal(t, ot);
		test_vector3_equal(s, os);
		auto q_dot = std::abs(r.dot_product(oq));
		EXPECT_GE(q_dot, (1 - ror::ror_epsilon));
	}
}

TYPED_TEST(TransformTest, sets)
{
	ror::Quaternion<TypeParam> oq(static_cast<TypeParam>(0.4590013),
								  static_cast<TypeParam>(0.357001),
								  static_cast<TypeParam>(0.7038019),
								  static_cast<TypeParam>(0.4080821));

	ror::Vector3<TypeParam> ot(10, 20, 30);
	ror::Vector3<TypeParam> os(1, 2, 3);

	auto t4x4 = ror::matrix4_translation(ot.x, ot.y, ot.z);
	auto r4x4 = ror::matrix4_rotation(oq);
	auto s4x4 = ror::matrix4_scaling(os.x, os.y, os.z);

	auto mres = t4x4 * r4x4 * s4x4;

	{
		ror::Transform<TypeParam> q;
		q.set(oq, ot, os);

		auto r = q.rotation();
		auto t = q.translation();
		auto s = q.scale();

		test_vector3_equal(t, ot);
		test_vector3_equal(s, os);
		test_quaternion_equal(r, oq);
	}
	{
		ror::Transform<TypeParam> q;
		q.translation(ot);
		q.scale(os);
		q.rotation(oq);

		auto r = q.rotation();
		auto t = q.translation();
		auto s = q.scale();

		test_vector3_equal(t, ot);
		test_vector3_equal(s, os);
		test_quaternion_equal(r, oq);
	}
	{
		ror::Matrix3<TypeParam>   m3x3(mres);
		ror::Transform<TypeParam> tm33;
		tm33.set(m3x3);

		auto r = tm33.rotation();
		auto t = tm33.translation();
		auto s = tm33.scale();

		test_vector3_equal(t, ror::zero_vector3f);
		test_vector3_equal(s, os);

		auto q_dot = std::abs(r.dot_product(oq));
		EXPECT_GE(q_dot, (1 - ror::ror_epsilon));
	}
	{
		ror::Matrix3x4<TypeParam> m3x4(mres);
		ror::Transform<TypeParam> tm34;
		tm34.set(m3x4);

		auto r = tm34.rotation();
		auto t = tm34.translation();
		auto s = tm34.scale();

		test_vector3_equal(t, ot);
		test_vector3_equal(s, os);
		auto q_dot = std::abs(r.dot_product(oq));
		EXPECT_GE(q_dot, (1 - ror::ror_epsilon));
	}
	{
		ror::Matrix4<TypeParam>   m4x4(mres);
		ror::Transform<TypeParam> tm44;
		tm44.set(m4x4);

		auto r = tm44.rotation();
		auto t = tm44.translation();
		auto s = tm44.scale();

		test_vector3_equal(t, ot);
		test_vector3_equal(s, os);
		auto q_dot = std::abs(r.dot_product(oq));
		EXPECT_GE(q_dot, (1 - ror::ror_epsilon));
	}
}

}        // namespace ror_test
