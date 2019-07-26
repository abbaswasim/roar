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

namespace ror_test
{
template <class _type>
void AxisAngleTest<_type>::SetUp()
{}

template <class _type>
void AxisAngleTest<_type>::TearDown()
{}

TYPED_TEST(AxisAngleTest, constructors_work)
{
	{
		// Lets also test AxisAngle
		ror::AxisAngle<TypeParam> aa;
		ASSERT_TRUE(aa.m_axis == ror::Vector3<TypeParam>(0, 0, 0));
		EXPECT_DOUBLE_EQ(aa.m_angle_radians, static_cast<TypeParam>(0));
	}
	{
		ror::Vector3<TypeParam>   axis(1, 0, 0);
		TypeParam                 angle = ror::ror_pi;
		ror::AxisAngle<TypeParam> aa(axis, angle);
		ASSERT_TRUE(aa.m_axis == axis);
		EXPECT_DOUBLE_EQ(aa.m_angle_radians, static_cast<TypeParam>(ror::ror_pi));
	}
	{
		ror::Vector3<TypeParam>   axis(1, 4, 2);
		TypeParam                 angle = ror::ror_pi;
		ror::AxisAngle<TypeParam> aa(axis, angle);
		axis.normalize();
		ASSERT_TRUE(aa.m_axis == axis);
		EXPECT_DOUBLE_EQ(aa.m_angle_radians, static_cast<TypeParam>(ror::ror_pi));
	}
	{
		ror::Vector3<TypeParam>   axis(1, 4, 2);
		TypeParam                 angle = ror::ror_pi;
		ror::AxisAngle<TypeParam> aa(axis.x, axis.y, axis.z, angle);
		axis.normalize();
		ASSERT_TRUE(aa.m_axis == axis);
		EXPECT_DOUBLE_EQ(aa.m_angle_radians, static_cast<TypeParam>(ror::ror_pi));
	}
	{
		ror::Vector3<TypeParam>   axis(1, 4, 2);
		TypeParam                 angle = ror::ror_pi;
		ror::AxisAngle<TypeParam> aa;
		aa.set(axis, angle);
		axis.normalize();
		ASSERT_TRUE(aa.m_axis == axis);
		EXPECT_DOUBLE_EQ(aa.m_angle_radians, static_cast<TypeParam>(ror::ror_pi));

		ASSERT_TRUE(aa.axis() == axis);
		EXPECT_DOUBLE_EQ(aa.angle(), static_cast<TypeParam>(ror::ror_pi));
	}
	{
		ror::Vector3<TypeParam>   axis(1, 4, 2);
		TypeParam                 angle = ror::ror_pi;
		ror::AxisAngle<TypeParam> aa;
		aa.set(axis.x, axis.y, axis.z, angle);
		axis.normalize();
		ASSERT_TRUE(aa.m_axis == axis);
		EXPECT_DOUBLE_EQ(aa.m_angle_radians, static_cast<TypeParam>(ror::ror_pi));

		ASSERT_TRUE(aa.axis() == axis);
		EXPECT_DOUBLE_EQ(aa.angle(), static_cast<TypeParam>(ror::ror_pi));
	}
	{
		ror::Vector3<TypeParam>   axis(1, 4, 2);
		TypeParam                 angle = ror::ror_pi;
		ror::AxisAngle<TypeParam> aa;
		ror::AxisAngle<TypeParam> bb(axis, angle);
		aa.set_axis(axis);
		aa.set_angle(angle);
		axis.normalize();

		ASSERT_TRUE(aa.m_axis == axis);
		EXPECT_DOUBLE_EQ(aa.m_angle_radians, static_cast<TypeParam>(ror::ror_pi));
		ASSERT_TRUE(aa.axis() == axis);
		EXPECT_DOUBLE_EQ(aa.angle(), static_cast<TypeParam>(ror::ror_pi));

		ASSERT_FALSE(aa != bb);
	}
}

TYPED_TEST(AxisAngleTest, equal_not_equal_work)
{
	ror::Vector3<TypeParam>   axis(1, 4, 2);
	TypeParam                 angle = ror::ror_pi;
	ror::AxisAngle<TypeParam> aa(axis, angle);
	ror::AxisAngle<TypeParam> bb(axis, angle);

	ASSERT_TRUE(aa == bb);
	ASSERT_FALSE(aa != bb);
}

}        // namespace ror_test
