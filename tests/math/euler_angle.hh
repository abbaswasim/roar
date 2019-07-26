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
void EulerAngleTest<_type>::SetUp()
{}

template <class _type>
void EulerAngleTest<_type>::TearDown()
{}

TYPED_TEST(EulerAngleTest, constructors_work)
{
	{
		// Lets test EulerAngle
		ror::EulerAngle<TypeParam> ea;

		ASSERT_DOUBLE_EQ(ea.m_heading, static_cast<TypeParam>(0));
		ASSERT_DOUBLE_EQ(ea.m_attitude, static_cast<TypeParam>(0));
		ASSERT_DOUBLE_EQ(ea.m_bank, static_cast<TypeParam>(0));
	}
	{
		TypeParam x = 1, y = 2, z = 4;
		ror::Vector3<TypeParam>    euler(x, y, z);
		ror::EulerAngle<TypeParam> ea(euler);
		ror::EulerAngle<TypeParam> ba(euler.y, euler.z, euler.x);
		ror::EulerAngle<TypeParam> ca;

		ca.set_heading(y);
		ca.set_attitude(z);
		ca.set_bank(x);

		ASSERT_DOUBLE_EQ(ea.m_heading, static_cast<TypeParam>(euler.y));
		ASSERT_DOUBLE_EQ(ea.m_attitude, static_cast<TypeParam>(euler.z));
		ASSERT_DOUBLE_EQ(ea.m_bank, static_cast<TypeParam>(euler.x));

		ASSERT_DOUBLE_EQ(ba.m_heading, static_cast<TypeParam>(euler.y));
		ASSERT_DOUBLE_EQ(ba.m_attitude, static_cast<TypeParam>(euler.z));
		ASSERT_DOUBLE_EQ(ba.m_bank, static_cast<TypeParam>(euler.x));

		ASSERT_DOUBLE_EQ(ea.heading(), static_cast<TypeParam>(euler.y));
		ASSERT_DOUBLE_EQ(ea.attitude(), static_cast<TypeParam>(euler.z));
		ASSERT_DOUBLE_EQ(ea.bank(), static_cast<TypeParam>(euler.x));

		ASSERT_DOUBLE_EQ(ca.heading(), static_cast<TypeParam>(euler.y));
		ASSERT_DOUBLE_EQ(ca.attitude(), static_cast<TypeParam>(euler.z));
		ASSERT_DOUBLE_EQ(ca.bank(), static_cast<TypeParam>(euler.x));

	}
	{
		TypeParam x = 1, y = 2, z = 4;
		ror::Vector3<TypeParam>    euler(x, y, z);
		ror::EulerAngle<TypeParam> ea(euler);
		ror::EulerAngle<TypeParam> ba;
		ror::EulerAngle<TypeParam> ca;

		ba.set(euler);
		ca.set(y, z, x);

		ASSERT_DOUBLE_EQ(ea.heading(), ba.heading());
		ASSERT_DOUBLE_EQ(ea.attitude(), ba.attitude());
		ASSERT_DOUBLE_EQ(ea.bank(), ba.bank());

		ASSERT_DOUBLE_EQ(ea.heading(), ca.heading());
		ASSERT_DOUBLE_EQ(ea.attitude(), ca.attitude());
		ASSERT_DOUBLE_EQ(ea.bank(), ca.bank());

	}
}

TYPED_TEST(EulerAngleTest, linear_algebra_works)
{

	{
		TypeParam x = 1, y = 2, z = 4;
		ror::Vector3<TypeParam>    euler(x, y, z);
		ror::EulerAngle<TypeParam> ea(y, z, x);
		ror::EulerAngle<TypeParam> ba = ea;
		ror::EulerAngle<TypeParam> ca(y + y, z + z, x + x);
		ror::EulerAngle<TypeParam> da(z, x, y);

		ASSERT_TRUE(ea == ba);
		ASSERT_TRUE(ea != da);

		ba += ea;
		ASSERT_TRUE(ca == ba);

		ba -= ea;
		ASSERT_TRUE(ea == ba);

		ba += ba;
		ASSERT_TRUE(ca == ba);

		ba = ea + ea;
		ASSERT_TRUE(ca == ba);

		ba = ba - ea;
		ASSERT_TRUE(ea == ba);
	}
}

}        // namespace ror_test
