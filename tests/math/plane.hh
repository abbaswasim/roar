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
TYPED_TEST(PlaneTest, constructors_work)
{
	ror::Vector3<TypeParam> normal(static_cast<TypeParam>(0.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(0.0f));

	ror::Vector3<TypeParam> point1(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(0.0f));
	ror::Vector3<TypeParam> point2(static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(0.0f));
	ror::Vector3<TypeParam> point3(static_cast<TypeParam>(0.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f));

	ror::Vector3<TypeParam> point4(static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f));

	TypeParam distance = static_cast<TypeParam>(0.5f);

	ror::Plane<TypeParam> plane1;        // Undetermined state plane
	ror::Plane<TypeParam> plane2(normal, distance);
	ror::Plane<TypeParam> plane3(plane2);
	ror::Plane<TypeParam> plane4(point1, point2, point3);
	ror::Plane<TypeParam> plane5(normal, point4);
	ror::Plane<TypeParam> plane6(normal.x, normal.y, normal.z, distance);

	ror::Vector3<TypeParam> normalFromPlane   = plane2.normal();
	TypeParam               distanceFromPlane = plane2.distance();

	EXPECT_TRUE(plane2 == plane3);
	EXPECT_FALSE(plane2 != plane3);

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_DOUBLE_EQ(distanceFromPlane, distance);

	normalFromPlane   = plane3.normal();
	distanceFromPlane = plane3.distance();

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_DOUBLE_EQ(distanceFromPlane, distance);

	plane1.set(normal, distance);

	normalFromPlane   = plane1.normal();
	distanceFromPlane = plane1.distance();

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_DOUBLE_EQ(distanceFromPlane, distance);

	normalFromPlane   = plane4.normal();
	distanceFromPlane = plane4.distance();

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_DOUBLE_EQ((distanceFromPlane), static_cast<TypeParam>(1.0f));

	normalFromPlane   = plane5.normal();
	distanceFromPlane = plane5.distance();

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_DOUBLE_EQ((distanceFromPlane), static_cast<TypeParam>(1.0f));

	normalFromPlane   = plane6.normal();
	distanceFromPlane = plane6.distance();

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_DOUBLE_EQ((distanceFromPlane), distance);
}

TYPED_TEST(PlaneTest, sets)
{
	ror::Vector3<TypeParam> normal(static_cast<TypeParam>(2.7f), static_cast<TypeParam>(1.8f), static_cast<TypeParam>(0.45f));
	normal.normalize();
	ror::Vector3<TypeParam> normalFromPlane;

	TypeParam distance = static_cast<TypeParam>(0.5f);
	TypeParam distanceFromPlane;

	ror::Vector3<TypeParam> point1(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(0.0f));
	ror::Vector3<TypeParam> point2(static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(0.0f));
	ror::Vector3<TypeParam> point3(static_cast<TypeParam>(0.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f));
	ror::Vector3<TypeParam> point4(static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f));

	ror::Plane<TypeParam> plane1(normal, distance);
	ror::Plane<TypeParam> plane2(point1, point2, point3);
	ror::Plane<TypeParam> plane3(point1, point2, point3);
	ror::Plane<TypeParam> plane4(point1, point2, point3);

	ror::Plane<TypeParam> plane5(normal, point4);
	ror::Plane<TypeParam> plane6(normal.x, normal.y, normal.z, distance);

	normalFromPlane   = plane1.normal();
	distanceFromPlane = plane1.distance();

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_DOUBLE_EQ(distanceFromPlane, distance);

	normalFromPlane   = plane6.normal();
	distanceFromPlane = plane6.distance();

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_DOUBLE_EQ(distanceFromPlane, distance);

	normalFromPlane   = plane2.normal();
	distanceFromPlane = plane2.distance();

	EXPECT_FALSE(normalFromPlane == normal);
	EXPECT_DOUBLE_NE(distanceFromPlane, distance);

	normalFromPlane   = plane3.normal();
	distanceFromPlane = plane3.distance();

	EXPECT_FALSE(normalFromPlane == normal);
	EXPECT_DOUBLE_NE(distanceFromPlane, distance);

	normalFromPlane   = plane4.normal();
	distanceFromPlane = plane4.distance();

	EXPECT_FALSE(normalFromPlane == normal);
	EXPECT_DOUBLE_NE(distanceFromPlane, distance);

	plane2 = plane1;

	normalFromPlane   = plane2.normal();
	distanceFromPlane = plane2.distance();

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_DOUBLE_EQ(distanceFromPlane, distance);

	plane3.set(normal, distance);

	normalFromPlane   = plane3.normal();
	distanceFromPlane = plane3.distance();

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_DOUBLE_EQ(distanceFromPlane, distance);

	plane1.set(point1, point2, point3);

	normalFromPlane   = plane1.normal();
	distanceFromPlane = plane1.distance();

	EXPECT_TRUE(normalFromPlane == plane4.normal());
	EXPECT_DOUBLE_EQ(distanceFromPlane, plane4.distance());

	plane1.set(normal.x, normal.y, normal.z, distance);

	normalFromPlane   = plane1.normal();
	distanceFromPlane = plane1.distance();

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_TRUE((std::abs(distanceFromPlane) - distance) < ror::ror_epsilon_squared);

	normalFromPlane   = plane5.normal();
	distanceFromPlane = plane5.distance();

	EXPECT_TRUE(normalFromPlane == normal);
	EXPECT_NEAR(distanceFromPlane, static_cast<TypeParam>(1.5109661945101902), test_epsilon);
}

TYPED_TEST(PlaneTest, get_normal)
{
	ror::Vector3<TypeParam> normal1(static_cast<TypeParam>(2.7f), static_cast<TypeParam>(1.8f), static_cast<TypeParam>(0.45f));
	ror::Vector3<TypeParam> normal2(static_cast<TypeParam>(7.2f), static_cast<TypeParam>(8.1f), static_cast<TypeParam>(45.0f));
	ror::Vector3<TypeParam> normalFromPlane;

	TypeParam distance = static_cast<TypeParam>(0.5f);

	ror::Plane<TypeParam> plane1(normal1, distance);
	ror::Plane<TypeParam> plane2(normal2, distance);

	normal1.normalize();
	normal2.normalize();

	normalFromPlane = plane1.normal();

	EXPECT_TRUE(normalFromPlane == normal1);

	normalFromPlane = plane2.normal();

	EXPECT_TRUE(normalFromPlane == normal2);
}

TYPED_TEST(PlaneTest, get_distance)
{
	ror::Vector3<TypeParam> normal(static_cast<TypeParam>(2.7f), static_cast<TypeParam>(1.8f), static_cast<TypeParam>(0.45f));

	TypeParam distance1 = static_cast<TypeParam>(0.5f);
	TypeParam distance2 = static_cast<TypeParam>(5.0f);
	TypeParam distanceFromPlane;

	ror::Plane<TypeParam> plane1(normal, distance1);
	ror::Plane<TypeParam> plane2(normal, distance2);

	distanceFromPlane = plane1.distance();

	EXPECT_DOUBLE_EQ(distanceFromPlane, distance1);

	distanceFromPlane = plane2.distance();

	EXPECT_DOUBLE_EQ(distanceFromPlane, distance2);
}

TYPED_TEST(PlaneTest, set_normal)
{
	ror::Vector3<TypeParam> normal1(static_cast<TypeParam>(2.7f), static_cast<TypeParam>(1.8f), static_cast<TypeParam>(0.45f));
	ror::Vector3<TypeParam> normal2(static_cast<TypeParam>(7.2f), static_cast<TypeParam>(8.1f), static_cast<TypeParam>(45.0f));
	ror::Vector3<TypeParam> normalFromPlane;

	TypeParam distance = static_cast<TypeParam>(0.5f);

	ror::Plane<TypeParam> plane1(normal1, distance);

	normal1.normalize();

	normalFromPlane = plane1.normal();
	EXPECT_TRUE(normalFromPlane == normal1);

	plane1.set_normal(normal2);

	normal2.normalize();

	normalFromPlane = plane1.normal();
	EXPECT_TRUE(normalFromPlane == normal2);
}

TYPED_TEST(PlaneTest, set_distance)
{
	ror::Vector3<TypeParam> normal(static_cast<TypeParam>(2.7f), static_cast<TypeParam>(1.8f), static_cast<TypeParam>(0.45f));

	TypeParam distance1 = static_cast<TypeParam>(0.5f);
	TypeParam distance2 = static_cast<TypeParam>(5.0f);
	TypeParam distanceFromPlane;

	ror::Plane<TypeParam> plane1(normal, distance1);

	distanceFromPlane = plane1.distance();
	EXPECT_DOUBLE_EQ(distanceFromPlane, distance1);

	plane1.set_distance(distance2);
	distanceFromPlane = plane1.distance();
	EXPECT_DOUBLE_EQ(distanceFromPlane, distance2);
}

TYPED_TEST(PlaneTest, distance_to_point)
{
	ror::Vector3<TypeParam> point1(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(0.0f));
	ror::Vector3<TypeParam> point2(static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(0.0f));
	ror::Vector3<TypeParam> point3(static_cast<TypeParam>(0.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f));

	ror::Vector3<TypeParam> point4(static_cast<TypeParam>(0.0f), static_cast<TypeParam>(3.5f), static_cast<TypeParam>(0.0f));
	ror::Vector3<TypeParam> point5(static_cast<TypeParam>(0.0f), static_cast<TypeParam>(3.5f), static_cast<TypeParam>(6.0f));
	ror::Vector3<TypeParam> point6(static_cast<TypeParam>(100.0f), static_cast<TypeParam>(3.5f), static_cast<TypeParam>(6.0f));

	ror::Plane<TypeParam> plane1(point1, point2, point3);

	TypeParam distance_to_point = plane1.distance_to_point(point4);
	EXPECT_DOUBLE_EQ(distance_to_point, static_cast<TypeParam>(2.5f));

	distance_to_point = plane1.distance_to_point(point5);
	EXPECT_DOUBLE_EQ(distance_to_point, static_cast<TypeParam>(2.5f));

	distance_to_point = plane1.distance_to_point(point6);
	EXPECT_DOUBLE_EQ(distance_to_point, static_cast<TypeParam>(2.5f));
}

TYPED_TEST(PlaneTest, normalize_plane)
{
	ror::Vector3<TypeParam> normal(static_cast<TypeParam>(25.7f), static_cast<TypeParam>(11.8f), static_cast<TypeParam>(80.45f));
	ror::Vector3<TypeParam> normalFromPlane;

	TypeParam distance = static_cast<TypeParam>(28.5f);
	TypeParam distanceFromPlane;

	ror::Plane<TypeParam> plane1(normal, distance);

	distanceFromPlane = plane1.distance();
	EXPECT_DOUBLE_EQ(distanceFromPlane, distance);

	plane1.normalize();

	distanceFromPlane = plane1.distance();
	EXPECT_NEAR(distanceFromPlane, distance, test_epsilon);

	normalFromPlane = plane1.normal();
	EXPECT_FALSE(normalFromPlane == normal);

	normal.normalize();
	normalFromPlane = plane1.normal();
	EXPECT_TRUE(normalFromPlane == normal);
}

TYPED_TEST(PlaneTest, line_intersection_reflection_matrix)
{
	{
		ror::Vector3<TypeParam> point1(static_cast<TypeParam>(-10.0f), static_cast<TypeParam>(10.0f), static_cast<TypeParam>(0.0f));
		ror::Vector3<TypeParam> point2(static_cast<TypeParam>(10.0f), static_cast<TypeParam>(10.0f), static_cast<TypeParam>(0.0f));
		ror::Vector3<TypeParam> point3(static_cast<TypeParam>(0.0f), static_cast<TypeParam>(10.0f), static_cast<TypeParam>(10.0f));

		ror::Vector3<TypeParam> origin(static_cast<TypeParam>(0.0f), static_cast<TypeParam>(0.0f), static_cast<TypeParam>(0.0f));
		ror::Vector3<TypeParam> line1p1(static_cast<TypeParam>(5.0f), static_cast<TypeParam>(5.0f), static_cast<TypeParam>(5.0f));
		ror::Vector3<TypeParam> line2p1(static_cast<TypeParam>(5.0f), static_cast<TypeParam>(7.0f), static_cast<TypeParam>(3.0f));
		ror::Vector3<TypeParam> line2p2(static_cast<TypeParam>(5.0f), static_cast<TypeParam>(17.0f), static_cast<TypeParam>(3.0f));
		ror::Vector3<TypeParam> point_plane(static_cast<TypeParam>(0.0f), static_cast<TypeParam>(10.0f), static_cast<TypeParam>(0.0f));

		ror::Plane<TypeParam> plane1(point1, point2, point3);

		ror::Vector3<TypeParam> intersection;

		bool intersects = plane1.line_intersection(origin, line1p1, intersection);

		ASSERT_TRUE(intersects);
		EXPECT_DOUBLE_EQ(intersection.x, static_cast<TypeParam>(10));
		EXPECT_DOUBLE_EQ(intersection.y, static_cast<TypeParam>(10));
		EXPECT_DOUBLE_EQ(intersection.z, static_cast<TypeParam>(10));

		intersects = plane1.line_intersection(origin, line2p1, intersection);

		ASSERT_TRUE(intersects);
		EXPECT_NEAR(intersection.x, static_cast<TypeParam>(7.1428570747375488), test_epsilon);
		EXPECT_NEAR(intersection.y, static_cast<TypeParam>(10.0f), test_epsilon);
		EXPECT_NEAR(intersection.z, static_cast<TypeParam>(4.2857141494750977), test_epsilon);

		auto same_side = plane1.segment_on_same_side(origin, line1p1);
		ASSERT_TRUE(same_side);

		same_side = plane1.segment_on_same_side(origin, line2p1);
		ASSERT_TRUE(same_side);

		same_side = plane1.segment_on_same_side(line2p2, line2p1);
		ASSERT_FALSE(same_side);

		same_side = plane1.segment_on_same_side(line2p1, line2p2);
		ASSERT_FALSE(same_side);

		auto front = plane1.in_front(line1p1);
		ASSERT_FALSE(front);

		front = plane1.in_front(origin);
		ASSERT_FALSE(front);

		front = plane1.in_front(line2p2);
		ASSERT_TRUE(front);

		intersection = plane1.point_on_plane();
		test_vector3_equal(intersection, point_plane);

		ror::Vector3<TypeParam> vec_p1(static_cast<TypeParam>(-10.0f), static_cast<TypeParam>(15.0f), static_cast<TypeParam>(-5.0f));
		ror::Vector3<TypeParam> vec_p2(static_cast<TypeParam>(10.0f), static_cast<TypeParam>(5.0f), static_cast<TypeParam>(8.0f));
		ror::Vector3<TypeParam> ref_res(static_cast<TypeParam>(20), static_cast<TypeParam>(10), static_cast<TypeParam>(13));

		auto reflect = plane1.reflected_vector(vec_p1, vec_p2);

		auto ref_mat     = plane1.reflection_matrix();
		auto ref_mat_res = ref_mat * (vec_p2 - vec_p1);

		test_vector3_equal(reflect, ref_res);
		test_vector3_equal(ref_mat_res, ref_res);

		ror::Vector3<TypeParam> ref_at_res(static_cast<TypeParam>(20), static_cast<TypeParam>(20), static_cast<TypeParam>(14.5));

		reflect = plane1.reflected_at_intersection_vector(vec_p1, vec_p2);
		test_vector3_equal(reflect, ref_at_res);
	}
	{
		ror::Vector3<TypeParam> point1(static_cast<TypeParam>(-5.93f), static_cast<TypeParam>(5.86f), static_cast<TypeParam>(1.43f));
		ror::Vector3<TypeParam> point2(static_cast<TypeParam>(8.61f), static_cast<TypeParam>(7.05f), static_cast<TypeParam>(-0.41f));
		ror::Vector3<TypeParam> point3(static_cast<TypeParam>(2.79f), static_cast<TypeParam>(4.63f), static_cast<TypeParam>(9.45f));

		ror::Vector3<TypeParam> origin(static_cast<TypeParam>(0.0f), static_cast<TypeParam>(0.0f), static_cast<TypeParam>(0.0f));
		ror::Vector3<TypeParam> line1p1(static_cast<TypeParam>(-2.14f), static_cast<TypeParam>(4.03f), static_cast<TypeParam>(-2.15f));
		ror::Vector3<TypeParam> intersection_res(static_cast<TypeParam>(-3.764508f), static_cast<TypeParam>(7.089238f), static_cast<TypeParam>(-3.782099f));

		ror::Plane<TypeParam> plane1(point1, point2, point3);

		ror::Vector3<TypeParam> intersection;

		bool intersects = plane1.line_intersection(origin, line1p1, intersection);

		ASSERT_TRUE(intersects);
		test_vector3_equal(intersection, intersection_res);

		ror::Vector3<TypeParam> vec_p1(static_cast<TypeParam>(2.9f), static_cast<TypeParam>(6.37f), static_cast<TypeParam>(4.7f));
		ror::Vector3<TypeParam> vec_p2(static_cast<TypeParam>(5.07f), static_cast<TypeParam>(7.29f), static_cast<TypeParam>(9.3f));
		ror::Vector3<TypeParam> vec_intersection_res(static_cast<TypeParam>(2.0202239f), static_cast<TypeParam>(5.99700f), static_cast<TypeParam>(2.83503f));
		ror::Vector3<TypeParam> ref_res(static_cast<TypeParam>(2.3564585f), static_cast<TypeParam>(-2.47735118f), static_cast<TypeParam>(3.8762273f));

		intersects = plane1.line_intersection(vec_p1, vec_p2, intersection);

		ASSERT_TRUE(intersects);
		test_vector3_equal(intersection, vec_intersection_res);

		auto reflect = plane1.reflected_vector(vec_p1, vec_p2);

		auto ref_mat     = plane1.reflection_matrix();
		auto ref_mat_res = ref_mat * (vec_p2 - vec_p1);

		test_vector3_equal(reflect, ref_res);
		test_vector3_equal(ref_mat_res, ref_res);

		ror::Vector3<TypeParam> ref_at_res(static_cast<TypeParam>(4.376681f), static_cast<TypeParam>(3.5196556f), static_cast<TypeParam>(6.711262f));

		reflect = plane1.reflected_at_intersection_vector(vec_p1, vec_p2);
		test_vector3_equal(reflect, ref_at_res);
	}
}

TYPED_TEST(PlaneTest, projection_matrix)
{
	ror::Vector3<TypeParam> point1(static_cast<TypeParam>(-5.93f), static_cast<TypeParam>(5.86f), static_cast<TypeParam>(1.43f));
	ror::Vector3<TypeParam> point2(static_cast<TypeParam>(8.61f), static_cast<TypeParam>(7.05f), static_cast<TypeParam>(-0.41f));
	ror::Vector3<TypeParam> point3(static_cast<TypeParam>(2.79f), static_cast<TypeParam>(4.63f), static_cast<TypeParam>(9.45f));

	ror::Plane<TypeParam> plane1(point1, point2, point3);

	ror::Vector3<TypeParam> test_p1(static_cast<TypeParam>(2.9), static_cast<TypeParam>(6.37), static_cast<TypeParam>(6.0));
	ror::Vector3<TypeParam> test_p2(static_cast<TypeParam>(2.33), static_cast<TypeParam>(10.66), static_cast<TypeParam>(9.35));
	ror::Vector3<TypeParam> test_p3(static_cast<TypeParam>(9.02), static_cast<TypeParam>(7.75), static_cast<TypeParam>(3.36));
	ror::Vector3<TypeParam> test_p4(static_cast<TypeParam>(-9.32662), static_cast<TypeParam>(4.52093), static_cast<TypeParam>(13.29809));

	ror::Vector3<TypeParam> test_p1_res(static_cast<TypeParam>(3.1259422), static_cast<TypeParam>(5.3771352), static_cast<TypeParam>(6.02953481));
	ror::Vector3<TypeParam> test_p2_res(static_cast<TypeParam>(3.9196352), static_cast<TypeParam>(4.3805432), static_cast<TypeParam>(10.911956));
	ror::Vector3<TypeParam> test_p3_res(static_cast<TypeParam>(9.820400), static_cast<TypeParam>(6.3457484), static_cast<TypeParam>(3.20754265));
	ror::Vector3<TypeParam> test_p4_res(static_cast<TypeParam>(-9.8540954), static_cast<TypeParam>(3.0000281), static_cast<TypeParam>(13.843626));

	ror::Vector3<TypeParam> projection_point(static_cast<TypeParam>(-1.69), static_cast<TypeParam>(26.54), static_cast<TypeParam>(5.4));

	auto projection = plane1.projection_matrix(projection_point);

	auto p1_ = projection.perspective_transform(test_p1);
	auto p2_ = projection.perspective_transform(test_p2);
	auto p3_ = projection.perspective_transform(test_p3);
	auto p4_ = projection.perspective_transform(test_p4);

	test_vector3_equal(p1_, test_p1_res);
	test_vector3_equal(p2_, test_p2_res);
	test_vector3_equal(p3_, test_p3_res);
	test_vector3_equal(p4_, test_p4_res);
}

}        // namespace ror_test
