// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2019
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

#include "bounds.hpp"
#include <cstddef>
#include <cstdio>

namespace ror_test
{
template <class _type>
void BoundingTest<_type>::SetUp()
{
	this->m_vector3_0 = ror::Vector3<_type>(0.0f, 0.0f, 0.0f);
	this->m_vector3_1 = ror::Vector3<_type>(10.0f, 10.0f, 10.0f);
	this->m_vector3_2 = ror::Vector3<_type>(20.0f, 20.0f, 20.0f);
	this->m_vector3_3 = ror::Vector3<_type>(30.0f, 30.0f, 30.0f);
	this->m_vector3_4 = ror::Vector3<_type>(40.0f, 40.0f, 40.0f);

	this->m_vector_negative3_1 = -this->m_vector3_1;
	this->m_vector_negative3_2 = -this->m_vector3_2;
	this->m_vector_negative3_3 = -this->m_vector3_3;
	this->m_vector_negative3_4 = -this->m_vector3_4;

	this->m_vector2_0 = ror::Vector2<_type>(this->m_vector3_0);
	this->m_vector2_1 = ror::Vector2<_type>(this->m_vector3_1);
	this->m_vector2_2 = ror::Vector2<_type>(this->m_vector3_2);
	this->m_vector2_3 = ror::Vector2<_type>(this->m_vector3_3);
	this->m_vector2_4 = ror::Vector2<_type>(this->m_vector3_4);

	this->m_vector_negative2_1 = -this->m_vector2_1;
	this->m_vector_negative2_2 = -this->m_vector2_2;
	this->m_vector_negative2_3 = -this->m_vector2_3;
	this->m_vector_negative2_4 = -this->m_vector2_4;
}

template <class _type>
void BoundingTest<_type>::TearDown()
{}

TYPED_TEST(BoundingTest, BoundingSphere_constructors_work)
{
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere1;
	ASSERT_TRUE(static_cast<float32_t>(bounding_sphere1.radius()) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_sphere1.center() == ror::Vector3<TypeParam>(0.0f, 0.0f, 0.0f));
	ASSERT_TRUE(bounding_sphere1.m_type == ror::BoundingType::sphere_3d);

	ASSERT_TRUE(static_cast<float32_t>(bounding_sphere1.radius() - this->m_bounding_sphere.radius()) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_sphere1.center() == this->m_bounding_sphere.center());
	ASSERT_TRUE(bounding_sphere1.m_type == this->m_bounding_sphere.m_type);

	ror::Round<ror::Vector3<TypeParam>> bounding_sphere2(bounding_sphere1);
	ASSERT_TRUE(static_cast<float32_t>(bounding_sphere1.radius() - bounding_sphere2.radius()) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_sphere1.center() == bounding_sphere2.center());
	ASSERT_TRUE(bounding_sphere1.m_type == bounding_sphere2.m_type);
}

TYPED_TEST(BoundingTest, BoundingSphere_accessors_mutators)
{
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere1;
	ror::Vector3<TypeParam>             center(static_cast<TypeParam>(2.5f), static_cast<TypeParam>(3.0f), static_cast<TypeParam>(1.2f));

	bounding_sphere1.set_radius(static_cast<TypeParam>(5.0f));
	bounding_sphere1.set_center(center);

	ASSERT_TRUE(static_cast<float32_t>(bounding_sphere1.radius() - static_cast<TypeParam>(5.0f)) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_sphere1.center() == center);
}

TYPED_TEST(BoundingTest, BoundingSphere_add_point)
{
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere1;

	bounding_sphere1.set_center(this->m_vector3_2);
	bounding_sphere1.set_radius(1.0f);
	bounding_sphere1.add_point(this->m_vector3_3);

	// if (std::is_same<TypeParam, float32_t>::value ||
	//	std::is_same<TypeParam, double64_t>::value)
	EXPECT_GE(bounding_sphere1.radius(), 9.0f);

	TypeParam nextRadius = bounding_sphere1.radius();

	bounding_sphere1.add_point(this->m_vector3_4);
	EXPECT_GT(bounding_sphere1.radius(), nextRadius);

	nextRadius = bounding_sphere1.radius();

	bounding_sphere1.add_point(this->m_vector3_4);
	ASSERT_TRUE(abs(static_cast<float32_t>(bounding_sphere1.radius() - nextRadius)) < ror::ror_epsilon_squared);

	nextRadius = bounding_sphere1.radius();

	bounding_sphere1.add_point(this->m_vector3_2);
	ASSERT_TRUE(abs(static_cast<float32_t>(bounding_sphere1.radius() - nextRadius)) < ror::ror_epsilon_squared);

	ror::Vector3<TypeParam> pointx(40.0f, 0.0f, 0.0f);
	ror::Vector3<TypeParam> pointypositive(20.0f, 20.0f, 0.0f);
	ror::Vector3<TypeParam> pointynegative(20.0f, -20.0f, 0.0f);
	ror::Vector3<TypeParam> pointzpositive(20.0f, 0.0f, 20.0f);
	ror::Vector3<TypeParam> pointznegative(20.0f, 0.0f, -20.0f);
	ror::Vector3<TypeParam> pointallpositive(20.0f, 20.0f, 20.0f);
	ror::Vector3<TypeParam> pointallnegative(-20.0f, -20.0f, -20.0f);

	ror::Vector3<TypeParam> point1(0.0f, 0.0f, 0.0f);

	this->m_bounding_sphere.add_point(point1);
	this->m_bounding_sphere.add_point(pointx);
	EXPECT_TRUE(abs(static_cast<float32_t>(this->m_bounding_sphere.radius()) - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_sphere.add_point(pointypositive);
	EXPECT_TRUE(abs(static_cast<float32_t>(this->m_bounding_sphere.radius()) - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_sphere.add_point(pointynegative);
	EXPECT_TRUE(abs(static_cast<float32_t>(this->m_bounding_sphere.radius()) - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_sphere.add_point(pointzpositive);
	EXPECT_TRUE(abs(static_cast<float32_t>(this->m_bounding_sphere.radius()) - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_sphere.add_point(pointznegative);
	EXPECT_TRUE(abs(static_cast<float32_t>(this->m_bounding_sphere.radius()) - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_sphere.add_point(pointallpositive);
	EXPECT_GE(static_cast<float32_t>(this->m_bounding_sphere.radius()), 24.0f);

	this->m_bounding_sphere.add_point(pointallnegative);
	EXPECT_GE(static_cast<float32_t>(this->m_bounding_sphere.radius()), 37.0f);

	ror::Round<ror::Vector3<TypeParam>> bounding_sphere2;
	ror::Vector3<TypeParam>             pointSameCenter1(20.0f, 0.0f, 0.0f);
	ror::Vector3<TypeParam>             pointSameCenter2(-20.0f, 0.0f, 0.0f);

	bounding_sphere2.set_radius(10.0f);
	bounding_sphere2.set_center(point1);

	bounding_sphere2.add_point(pointSameCenter1);
	EXPECT_FALSE(bounding_sphere2.center() == point1);

	bounding_sphere2.add_point(pointSameCenter2);
	EXPECT_TRUE(abs(static_cast<float32_t>(bounding_sphere2.radius()) - 20.0f) < ror::ror_epsilon);
	EXPECT_TRUE(bounding_sphere2.center() == point1);
}

TYPED_TEST(BoundingTest, BoundingSphere_is_point_inside)
{
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere1;
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere2;

	ASSERT_TRUE(abs(static_cast<float32_t>(this->m_bounding_sphere.radius()) - 0.0f) < ror::ror_epsilon_squared);
	ASSERT_TRUE(this->m_bounding_sphere.center() == this->m_vector3_0);

	this->m_bounding_sphere.set_radius(20.0f);
	this->m_bounding_sphere.set_center(this->m_vector3_0);

	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector3_1) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector3_2) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector3_4) == true);

	this->m_bounding_sphere.add_point(this->m_vector3_3);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector3_4) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector_negative3_3) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector_negative3_2) == true);

	this->m_bounding_sphere.add_point(this->m_vector3_4);
	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector_negative3_2) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector_negative3_3) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector_negative3_4) == true);

	this->m_bounding_sphere.add_point(this->m_vector_negative3_3);
	this->m_bounding_sphere.add_point(this->m_vector_negative3_4);
	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector_negative3_2) == true);
	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector_negative3_3) == true);

	EXPECT_GT(this->m_bounding_sphere.radius(), 6.0f);
}

TYPED_TEST(BoundingTest, BoundingSphere_add_bounding_sphere)
{
	ror::Vector3<TypeParam> center;
	TypeParam               radius;

	ror::Round<ror::Vector3<TypeParam>> bounding_sphere1;
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere2;
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere3;
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere4;

	bounding_sphere1.set_radius(1.0f);
	bounding_sphere2.set_radius(1.0f);
	bounding_sphere3.set_radius(1.0f);
	bounding_sphere4.set_radius(1.0f);

	bounding_sphere1.set_center(this->m_vector3_1);
	bounding_sphere2.set_center(this->m_vector3_2);
	bounding_sphere3.set_center(this->m_vector3_3);
	bounding_sphere4.set_center(this->m_vector3_4);

	radius = bounding_sphere1.radius();
	center = bounding_sphere1.center();

	bounding_sphere1.add_bounding(bounding_sphere1);

	ASSERT_TRUE(bounding_sphere1.center() == center);
	ASSERT_TRUE(bounding_sphere1.radius() == radius);

	radius = bounding_sphere2.radius();
	center = bounding_sphere2.center();

	bounding_sphere2.add_bounding(bounding_sphere2);

	ASSERT_TRUE(bounding_sphere2.center() == center);
	ASSERT_TRUE(bounding_sphere2.radius() == radius);

	radius = bounding_sphere3.radius();
	center = bounding_sphere3.center();

	bounding_sphere3.add_bounding(bounding_sphere3);

	ASSERT_TRUE(bounding_sphere3.center() == center);
	ASSERT_TRUE(bounding_sphere3.radius() == radius);

	radius = bounding_sphere4.radius();
	center = bounding_sphere4.center();

	bounding_sphere4.add_bounding(bounding_sphere4);

	ASSERT_TRUE(bounding_sphere4.center() == center);
	ASSERT_TRUE(bounding_sphere4.radius() == radius);

	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_1) == true);
	ASSERT_FALSE(bounding_sphere1.is_point_inside(this->m_vector3_2) == true);
	ASSERT_FALSE(bounding_sphere1.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(bounding_sphere1.is_point_inside(this->m_vector3_4) == true);

	ASSERT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3_1) == true);
	ASSERT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_2) == true);
	ASSERT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3_4) == true);

	ASSERT_FALSE(bounding_sphere3.is_point_inside(this->m_vector3_1) == true);
	ASSERT_FALSE(bounding_sphere3.is_point_inside(this->m_vector3_2) == true);
	ASSERT_TRUE(bounding_sphere3.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(bounding_sphere3.is_point_inside(this->m_vector3_4) == true);

	ASSERT_FALSE(bounding_sphere4.is_point_inside(this->m_vector3_1) == true);
	ASSERT_FALSE(bounding_sphere4.is_point_inside(this->m_vector3_2) == true);
	ASSERT_FALSE(bounding_sphere4.is_point_inside(this->m_vector3_3) == true);
	ASSERT_TRUE(bounding_sphere4.is_point_inside(this->m_vector3_4) == true);

	bounding_sphere1.add_bounding(bounding_sphere2);
	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_1) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_2) == true);
	}
	EXPECT_FALSE(bounding_sphere1.is_point_inside(this->m_vector3_3) == true);
	EXPECT_FALSE(bounding_sphere1.is_point_inside(this->m_vector3_4) == true);

	bounding_sphere1.add_bounding(bounding_sphere3);
	EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_2) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_3) == true);
	}
	EXPECT_FALSE(bounding_sphere1.is_point_inside(this->m_vector3_4) == true);

	bounding_sphere1.add_bounding(bounding_sphere4);
	EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_4) == true);
	}

	bounding_sphere2.add_bounding(bounding_sphere3);
	EXPECT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_2) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_3) == true);
	}
	EXPECT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3_4) == true);

	bounding_sphere2.add_bounding(bounding_sphere4);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3_1) == true);
	}
	EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_4) == true);
	}

	bounding_sphere3.add_bounding(bounding_sphere4);
	EXPECT_FALSE(bounding_sphere3.is_point_inside(this->m_vector3_1) == true);
	EXPECT_FALSE(bounding_sphere3.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_sphere3.is_point_inside(this->m_vector3_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_sphere3.is_point_inside(this->m_vector3_4) == true);
	}

	bounding_sphere2.add_bounding(bounding_sphere4);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3_1) == true);
	}
	EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_4) == true);
	}
}

TYPED_TEST(BoundingTest, BoundingSphere_bounding_sphere_collision)
{
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere1;
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere2;
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere3;
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere4;
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere5;

	bounding_sphere1.set_radius(10.0f);
	bounding_sphere2.set_radius(10.0f);
	bounding_sphere3.set_radius(10.0f);
	bounding_sphere4.set_radius(10.0f);
	bounding_sphere5.set_radius(static_cast<TypeParam>(60));

	bounding_sphere1.set_center(this->m_vector3_1);
	bounding_sphere2.set_center(this->m_vector3_2);
	bounding_sphere3.set_center(this->m_vector3_3);
	bounding_sphere4.set_center(this->m_vector3_4);

	EXPECT_TRUE(bounding_sphere1.collision(bounding_sphere1) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_sphere1.collision(bounding_sphere2) == ror::CollisionType::intersects);
	EXPECT_FALSE(bounding_sphere1.collision(bounding_sphere3) == ror::CollisionType::intersects);
	EXPECT_FALSE(bounding_sphere1.collision(bounding_sphere4) == ror::CollisionType::intersects);

	EXPECT_TRUE(bounding_sphere2.collision(bounding_sphere1) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_sphere2.collision(bounding_sphere2) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_sphere2.collision(bounding_sphere3) == ror::CollisionType::intersects);
	EXPECT_FALSE(bounding_sphere2.collision(bounding_sphere4) == ror::CollisionType::intersects);

	EXPECT_FALSE(bounding_sphere3.collision(bounding_sphere1) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_sphere3.collision(bounding_sphere2) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_sphere3.collision(bounding_sphere3) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_sphere3.collision(bounding_sphere4) == ror::CollisionType::intersects);

	EXPECT_FALSE(bounding_sphere4.collision(bounding_sphere1) == ror::CollisionType::intersects);
	EXPECT_FALSE(bounding_sphere4.collision(bounding_sphere2) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_sphere4.collision(bounding_sphere3) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_sphere4.collision(bounding_sphere4) == ror::CollisionType::inside);

	EXPECT_TRUE(bounding_sphere5.collision(bounding_sphere1) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_sphere5.collision(bounding_sphere2) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_sphere5.collision(bounding_sphere3) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_sphere5.collision(bounding_sphere4) == ror::CollisionType::intersects);

	bounding_sphere5.set_radius(80.0f);
	EXPECT_TRUE(bounding_sphere5.collision(bounding_sphere4) == ror::CollisionType::inside);
}

TYPED_TEST(BoundingTest, BoundingSphere_create_from_points)
{
	std::vector<ror::Vector3<TypeParam>> points;
	points.reserve(4);

	points.push_back(this->m_vector3_1);
	points.push_back(this->m_vector3_2);
	points.push_back(this->m_vector3_3);
	points.push_back(this->m_vector3_4);

	this->m_bounding_sphere.set_radius(0);
	this->m_bounding_sphere.set_center(this->m_vector3_0);

	this->m_bounding_sphere.create_from_points(points);
	this->m_bounding_sphere.set_radius(this->m_bounding_sphere.radius() + static_cast<TypeParam>(2));

	EXPECT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector3_3) == true);
	EXPECT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector3_4) == true);

	auto distanceFromCenterToCenter = (this->m_vector3_4 - this->m_vector3_1).length() / 2;

	EXPECT_GT(this->m_bounding_sphere.radius(), distanceFromCenterToCenter);
}

TYPED_TEST(BoundingTest, BoundingSphere_system_test)
{
	// These tests are only valid for decimal types
	if constexpr (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		ror::Round<ror::Vector3<TypeParam>> boundingSphere0;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereSmall1;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereSmall2;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereSmall3;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereSmall4;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereSmall5;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereSame1;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereSame2;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereSame3;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereSame4;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereBig;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereSmallInsideBig;
		ror::Round<ror::Vector3<TypeParam>> boundingSphereHuge;

		boundingSphere0.set_radius(1.0f);
		boundingSphereSmall1.set_radius(0.2f);
		boundingSphereSmall2.set_radius(0.2f);
		boundingSphereSmall3.set_radius(0.2f);
		boundingSphereSmall4.set_radius(0.2f);
		boundingSphereSmall5.set_radius(0.2f);
		boundingSphereSame1.set_radius(1.5f);
		boundingSphereSame2.set_radius(1.5f);
		boundingSphereSame3.set_radius(1.5f);
		boundingSphereSame4.set_radius(1.5f);
		boundingSphereBig.set_radius(3.0f);
		boundingSphereSmallInsideBig.set_radius(0.8f);
		boundingSphereHuge.set_radius(11.0f);

		boundingSphere0.set_center(ror::Vector3<TypeParam>(0.0f, 0.0f, 0.0f));
		boundingSphereSmall1.set_center(ror::Vector3<TypeParam>(4.0f, 2.0f, 0.0f));
		boundingSphereSmall2.set_center(ror::Vector3<TypeParam>(2.0f, 4.0f, 0.0f));
		boundingSphereSmall3.set_center(ror::Vector3<TypeParam>(0.0f, 4.5f, 0.0f));
		boundingSphereSmall4.set_center(ror::Vector3<TypeParam>(-2.6f, -0.3f, 0.0f));
		boundingSphereSmall5.set_center(ror::Vector3<TypeParam>(-0.8f, -2.3f, 0.0f));
		boundingSphereSame1.set_center(ror::Vector3<TypeParam>(2.391f, -1.634f, 0.0f));
		boundingSphereSame2.set_center(ror::Vector3<TypeParam>(2.0f, 2.0f, 0.0f));
		boundingSphereSame3.set_center(ror::Vector3<TypeParam>(-2.0f, 3.0f, 0.0f));
		boundingSphereSame4.set_center(ror::Vector3<TypeParam>(-3.338f, -1.991f, 0.0f));
		boundingSphereBig.set_center(ror::Vector3<TypeParam>(6.0f, 5.0f, 0.0f));
		boundingSphereSmallInsideBig.set_center(ror::Vector3<TypeParam>(7.9f, 4.5f, 0.0f));
		boundingSphereHuge.set_center(ror::Vector3<TypeParam>(0.0f, 0.0f, 0.0f));

		// Tests in 2-Dimensions
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphere0), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmall1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmall2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmall3), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmall4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmall5), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSame1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSame2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSame3), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSame4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereBig), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmallInsideBig), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereHuge), ror::CollisionType::inside);

		EXPECT_EQ(boundingSphere0.collision(boundingSphere0), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall1), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall2), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall3), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall4), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall5), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame1), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame2), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame3), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame4), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereBig), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmallInsideBig), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereHuge), ror::CollisionType::intersects);

		boundingSphere0.add_bounding(boundingSphereSame1);
		boundingSphere0.add_bounding(boundingSphereSame2);
		boundingSphere0.add_bounding(boundingSphereSame3);
		boundingSphere0.add_bounding(boundingSphereSame4);

		boundingSphere0.add_bounding(boundingSphereSmall1);
		boundingSphere0.add_bounding(boundingSphereSmall2);
		boundingSphere0.add_bounding(boundingSphereSmall3);
		boundingSphere0.add_bounding(boundingSphereSmall4);
		boundingSphere0.add_bounding(boundingSphereSmall5);

		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall3), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall5), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame3), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereBig), ror::CollisionType::intersects);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmallInsideBig), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereHuge), ror::CollisionType::intersects);

		boundingSphere0.add_bounding(boundingSphereBig);
		boundingSphere0.add_bounding(boundingSphereSmallInsideBig);

		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall3), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall5), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame3), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereBig), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmallInsideBig), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereHuge), ror::CollisionType::intersects);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphere0), ror::CollisionType::inside);

		// Tests in 3-Dimensions
		boundingSphere0.set_radius(1.0f);
		boundingSphereSmall1.set_radius(0.2f);
		boundingSphereSmall2.set_radius(0.2f);
		boundingSphereSmall3.set_radius(0.2f);
		boundingSphereSmall4.set_radius(0.2f);
		boundingSphereSmall5.set_radius(0.2f);
		boundingSphereSame1.set_radius(1.5f);
		boundingSphereSame2.set_radius(1.5f);
		boundingSphereSame3.set_radius(1.5f);
		boundingSphereSame4.set_radius(1.5f);
		boundingSphereBig.set_radius(3.0f);
		boundingSphereSmallInsideBig.set_radius(0.8f);
		boundingSphereHuge.set_radius(12.0f);

		boundingSphere0.set_center(ror::Vector3<TypeParam>(0.0f, 0.0f, 0.0f));
		boundingSphereSmall1.set_center(ror::Vector3<TypeParam>(4.0f, 2.0f, 0.0f));
		boundingSphereSmall2.set_center(ror::Vector3<TypeParam>(2.0f, 4.0f, -5.315f));
		boundingSphereSmall3.set_center(ror::Vector3<TypeParam>(0.0f, 4.5f, 8.043f));
		boundingSphereSmall4.set_center(ror::Vector3<TypeParam>(-2.6f, -0.3f, 2.3f));
		boundingSphereSmall5.set_center(ror::Vector3<TypeParam>(-0.8f, -2.3f, -5.67f));
		boundingSphereSame1.set_center(ror::Vector3<TypeParam>(2.391f, -1.634f, 0.0f));
		boundingSphereSame2.set_center(ror::Vector3<TypeParam>(2.0f, 2.0f, 7.052f));
		boundingSphereSame3.set_center(ror::Vector3<TypeParam>(-2.0f, 3.0f, -5.383f));
		boundingSphereSame4.set_center(ror::Vector3<TypeParam>(-3.338f, -1.991f, 5.383f));
		boundingSphereBig.set_center(ror::Vector3<TypeParam>(6.0f, 5.0f, 2.0f));
		boundingSphereSmallInsideBig.set_center(ror::Vector3<TypeParam>(7.9f, 4.5f, 2.23f));
		boundingSphereHuge.set_center(ror::Vector3<TypeParam>(0.0f, 0.0f, 0.0f));

		EXPECT_EQ(boundingSphereHuge.collision(boundingSphere0), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmall1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmall2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmall3), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmall4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmall5), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSame1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSame2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSame3), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSame4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereBig), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereSmallInsideBig), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphereHuge), ror::CollisionType::inside);

		EXPECT_EQ(boundingSphere0.collision(boundingSphere0), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall1), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall2), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall3), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall4), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall5), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame1), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame2), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame3), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame4), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereBig), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmallInsideBig), ror::CollisionType::outside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereHuge), ror::CollisionType::intersects);

		boundingSphere0.add_bounding(boundingSphereSame1);
		boundingSphere0.add_bounding(boundingSphereSame2);
		boundingSphere0.add_bounding(boundingSphereSame3);
		boundingSphere0.add_bounding(boundingSphereSame4);

		boundingSphere0.add_bounding(boundingSphereSmall1);
		boundingSphere0.add_bounding(boundingSphereSmall2);
		boundingSphere0.add_bounding(boundingSphereSmall3);
		boundingSphere0.add_bounding(boundingSphereSmall4);
		boundingSphere0.add_bounding(boundingSphereSmall5);

		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall3), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall5), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereBig), ror::CollisionType::intersects);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmallInsideBig), ror::CollisionType::intersects);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereHuge), ror::CollisionType::intersects);

		boundingSphere0.add_bounding(boundingSphereBig);
		boundingSphere0.add_bounding(boundingSphereSmallInsideBig);

		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall3), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmall5), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame1), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame2), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame3), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSame4), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereBig), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereSmallInsideBig), ror::CollisionType::inside);
		EXPECT_EQ(boundingSphere0.collision(boundingSphereHuge), ror::CollisionType::intersects);
		EXPECT_EQ(boundingSphereHuge.collision(boundingSphere0), ror::CollisionType::inside);

		// Testing creating from points
		std::vector<ror::Vector3<TypeParam>> points;

		points.push_back(ror::Vector3<TypeParam>(7.9f, 4.5f, 2.23f));
		points.push_back(ror::Vector3<TypeParam>(0.0f, 0.0f, 0.0f));
		points.push_back(ror::Vector3<TypeParam>(4.0f, 2.0f, 0.0f));
		points.push_back(ror::Vector3<TypeParam>(2.0f, 4.0f, -5.315f));
		points.push_back(ror::Vector3<TypeParam>(0.0f, 4.5f, 8.043f));
		points.push_back(ror::Vector3<TypeParam>(-2.6f, -0.3f, 2.3f));
		points.push_back(ror::Vector3<TypeParam>(-0.8f, -2.3f, -5.67f));
		points.push_back(ror::Vector3<TypeParam>(2.391f, -1.634f, 0.0f));
		points.push_back(ror::Vector3<TypeParam>(2.0f, 2.0f, 7.052f));
		points.push_back(ror::Vector3<TypeParam>(-2.0f, 3.0f, -5.383f));
		points.push_back(ror::Vector3<TypeParam>(-3.338f, -1.991f, 5.383f));
		points.push_back(ror::Vector3<TypeParam>(6.0f, 5.0f, 2.0f));

		ror::Round<ror::Vector3<TypeParam>> boundingSphereFromPoints;
		boundingSphereFromPoints.create_from_points(points);

		EXPECT_EQ(boundingSphere0.collision(boundingSphereFromPoints), ror::CollisionType::intersects);
	}
}

// Bounding Circle tests
TYPED_TEST(BoundingTest, BoundingCircle_constructors_work)
{
	ror::Round<ror::Vector2<TypeParam>> bounding_circle1;
	ASSERT_TRUE((static_cast<float32_t>(bounding_circle1.radius()) - 0.0f) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_circle1.center() == ror::Vector2<TypeParam>(0.0f, 0.0f));
	ASSERT_TRUE(bounding_circle1.m_type == ror::BoundingType::circle_2d);

	ASSERT_TRUE(static_cast<float32_t>(bounding_circle1.radius() - this->m_bounding_circle.radius()) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_circle1.center() == this->m_bounding_circle.center());
	ASSERT_TRUE(bounding_circle1.m_type == this->m_bounding_circle.m_type);

	ror::Round<ror::Vector2<TypeParam>> bounding_circle2(bounding_circle1);
	ASSERT_TRUE(static_cast<float32_t>(bounding_circle1.radius() - bounding_circle2.radius()) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_circle1.center() == bounding_circle1.center());
	ASSERT_TRUE(bounding_circle1.m_type == bounding_circle1.m_type);
}

TYPED_TEST(BoundingTest, BoundingCircle_accessors_mutators)
{
	ror::Round<ror::Vector2<TypeParam>> bounding_circle1;
	ror::Vector2<TypeParam>             center(static_cast<TypeParam>(2.5f), static_cast<TypeParam>(3.0f));

	bounding_circle1.set_radius(5.0f);
	bounding_circle1.set_center(center);
	ASSERT_TRUE((static_cast<float32_t>(bounding_circle1.radius()) - 5.0f) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_circle1.center() == center);
}

TYPED_TEST(BoundingTest, BoundingCircle_add_point)
{
	ror::Round<ror::Vector2<TypeParam>> bounding_circle1;

	bounding_circle1.set_center(this->m_vector2_2);
	bounding_circle1.set_radius(1.0f);
	bounding_circle1.add_point(this->m_vector2_3);

	EXPECT_GT(bounding_circle1.radius(), 1.0f);

	TypeParam nextRadius = bounding_circle1.radius();
	bounding_circle1.add_point(this->m_vector2_4);
	EXPECT_GT(bounding_circle1.radius(), nextRadius);

	nextRadius = bounding_circle1.radius();
	bounding_circle1.add_point(this->m_vector2_4);
	ASSERT_TRUE(abs(static_cast<float32_t>(bounding_circle1.radius() - nextRadius)) < ror::ror_epsilon_squared);

	nextRadius = bounding_circle1.radius();
	bounding_circle1.add_point(this->m_vector2_2);
	ASSERT_TRUE(abs(static_cast<float32_t>(bounding_circle1.radius() - nextRadius)) < ror::ror_epsilon_squared);

	ror::Vector2<TypeParam> pointx(40.0f, 0.0f);
	ror::Vector2<TypeParam> pointypositive(20.0f, 20.0f);
	ror::Vector2<TypeParam> pointynegative(20.0f, -20.0f);
	ror::Vector2<TypeParam> pointzpositive(20.0f, 0.0f);
	ror::Vector2<TypeParam> pointznegative(20.0f, 0.0f);
	ror::Vector2<TypeParam> pointallpositive(20.0f, 20.0f);
	ror::Vector2<TypeParam> pointallnegative(-20.0f, -20.0f);

	ror::Vector2<TypeParam> point1(0.0f, 0.0f);

	this->m_bounding_circle.add_point(point1);
	this->m_bounding_circle.add_point(pointx);
	ASSERT_TRUE(abs(static_cast<float32_t>(this->m_bounding_circle.radius()) - 20.0f) < ror::ror_epsilon_squared);
	this->m_bounding_circle.add_point(pointypositive);
	ASSERT_TRUE(abs(static_cast<float32_t>(this->m_bounding_circle.radius()) - 20.0f) < ror::ror_epsilon_squared);
	this->m_bounding_circle.add_point(pointynegative);
	ASSERT_TRUE(abs(static_cast<float32_t>(this->m_bounding_circle.radius()) - 20.0f) < ror::ror_epsilon_squared);
	this->m_bounding_circle.add_point(pointzpositive);
	ASSERT_TRUE(abs(static_cast<float32_t>(this->m_bounding_circle.radius()) - 20.0f) < ror::ror_epsilon_squared);
	this->m_bounding_circle.add_point(pointznegative);
	ASSERT_TRUE(abs(static_cast<float32_t>(this->m_bounding_circle.radius()) - 20.0f) < ror::ror_epsilon_squared);

	ror::Round<ror::Vector2<TypeParam>> bounding_circle2;
	ror::Vector2<TypeParam>             pointSameCenter1(20.0f, 0.0f);
	ror::Vector2<TypeParam>             pointSameCenter2(-20.0f, 0.0f);

	bounding_circle2.set_radius(10.0f);
	bounding_circle2.set_center(point1);

	bounding_circle2.add_point(pointSameCenter1);
	EXPECT_FALSE(bounding_circle2.center() == point1);

	bounding_circle2.add_point(pointSameCenter2);
	EXPECT_TRUE(abs(static_cast<float32_t>(bounding_circle2.radius()) - 20.0f) < ror::ror_epsilon);
	EXPECT_TRUE(bounding_circle2.center() == point1);
}

TYPED_TEST(BoundingTest, BoundingCircle_is_point_inside)
{
	ror::Round<ror::Vector2<TypeParam>> bounding_circle1;
	ror::Round<ror::Vector2<TypeParam>> bounding_circle2;

	ror::Vector2<TypeParam> point1(10.0f, 10.0f);

	ASSERT_TRUE(abs(static_cast<float32_t>(this->m_bounding_circle.radius()) - 0.0f) < ror::ror_epsilon_squared);
	ASSERT_TRUE(this->m_bounding_circle.center() == this->m_vector2_0);

	this->m_bounding_circle.set_radius(20.0f);
	this->m_bounding_circle.set_center(this->m_vector2_0);

	ASSERT_TRUE(this->m_bounding_circle.is_point_inside(this->m_vector2_1) == true);
	ASSERT_FALSE(this->m_bounding_circle.is_point_inside(this->m_vector2_2) == true);
	ASSERT_FALSE(this->m_bounding_circle.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(this->m_bounding_circle.is_point_inside(this->m_vector2_4) == true);

	this->m_bounding_circle.add_point(this->m_vector2_4);
	ASSERT_TRUE(this->m_bounding_circle.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(this->m_bounding_circle.is_point_inside(this->m_vector_negative2_3) == true);
	ASSERT_FALSE(this->m_bounding_circle.is_point_inside(this->m_vector_negative2_2) == true);

	this->m_bounding_circle.add_point(this->m_vector_negative2_4);
	ASSERT_TRUE(this->m_bounding_circle.is_point_inside(this->m_vector_negative2_3) == true);
	EXPECT_GT(this->m_bounding_circle.radius(), 5.0f);
	ASSERT_TRUE(this->m_bounding_circle.is_point_inside(this->m_vector_negative2_2) == true);
}

TYPED_TEST(BoundingTest, BoundingCircle_add_bounding_circle)
{
	ror::Vector2<TypeParam> center;
	TypeParam               radius;

	ror::Round<ror::Vector2<TypeParam>> bounding_circle1;
	ror::Round<ror::Vector2<TypeParam>> bounding_circle2;
	ror::Round<ror::Vector2<TypeParam>> bounding_circle3;
	ror::Round<ror::Vector2<TypeParam>> bounding_circle4;

	bounding_circle1.set_radius(1.0f);
	bounding_circle2.set_radius(1.0f);
	bounding_circle3.set_radius(1.0f);
	bounding_circle4.set_radius(1.0f);

	bounding_circle1.set_center(this->m_vector2_1);
	bounding_circle2.set_center(this->m_vector2_2);
	bounding_circle3.set_center(this->m_vector2_3);
	bounding_circle4.set_center(this->m_vector2_4);

	radius = bounding_circle1.radius();
	center = bounding_circle1.center();

	bounding_circle1.add_bounding(bounding_circle1);

	ASSERT_TRUE(bounding_circle1.center() == center);
	ASSERT_TRUE(bounding_circle1.radius() == radius);

	radius = bounding_circle2.radius();
	center = bounding_circle2.center();

	bounding_circle2.add_bounding(bounding_circle2);

	ASSERT_TRUE(bounding_circle2.center() == center);
	ASSERT_TRUE(bounding_circle2.radius() == radius);

	radius = bounding_circle3.radius();
	center = bounding_circle3.center();

	bounding_circle3.add_bounding(bounding_circle3);

	ASSERT_TRUE(bounding_circle3.center() == center);
	ASSERT_TRUE(bounding_circle3.radius() == radius);

	radius = bounding_circle4.radius();
	center = bounding_circle4.center();

	bounding_circle4.add_bounding(bounding_circle4);

	ASSERT_TRUE(bounding_circle4.center() == center);
	ASSERT_TRUE(bounding_circle4.radius() == radius);

	ASSERT_TRUE(bounding_circle1.is_point_inside(this->m_vector2_1) == true);
	ASSERT_FALSE(bounding_circle1.is_point_inside(this->m_vector2_2) == true);
	ASSERT_FALSE(bounding_circle1.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(bounding_circle1.is_point_inside(this->m_vector2_4) == true);

	ASSERT_FALSE(bounding_circle2.is_point_inside(this->m_vector2_1) == true);
	ASSERT_TRUE(bounding_circle2.is_point_inside(this->m_vector2_2) == true);
	ASSERT_FALSE(bounding_circle2.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(bounding_circle2.is_point_inside(this->m_vector2_4) == true);

	ASSERT_FALSE(bounding_circle3.is_point_inside(this->m_vector2_1) == true);
	ASSERT_FALSE(bounding_circle3.is_point_inside(this->m_vector2_2) == true);
	ASSERT_TRUE(bounding_circle3.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(bounding_circle3.is_point_inside(this->m_vector2_4) == true);

	ASSERT_FALSE(bounding_circle4.is_point_inside(this->m_vector2_1) == true);
	ASSERT_FALSE(bounding_circle4.is_point_inside(this->m_vector2_2) == true);
	ASSERT_FALSE(bounding_circle4.is_point_inside(this->m_vector2_3) == true);
	ASSERT_TRUE(bounding_circle4.is_point_inside(this->m_vector2_4) == true);

	bounding_circle1.add_bounding(bounding_circle2);
	EXPECT_TRUE(bounding_circle1.is_point_inside(this->m_vector2_1) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_circle1.is_point_inside(this->m_vector2_2) == true);
	}
	EXPECT_FALSE(bounding_circle1.is_point_inside(this->m_vector2_3) == true);
	EXPECT_FALSE(bounding_circle1.is_point_inside(this->m_vector2_4) == true);
	bounding_circle1.add_bounding(bounding_circle3);
	EXPECT_TRUE(bounding_circle1.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_circle1.is_point_inside(this->m_vector2_2) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_circle1.is_point_inside(this->m_vector2_3) == true);
	}
	EXPECT_FALSE(bounding_circle1.is_point_inside(this->m_vector2_4) == true);
	bounding_circle1.add_bounding(bounding_circle4);
	EXPECT_TRUE(bounding_circle1.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_circle1.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_circle1.is_point_inside(this->m_vector2_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_circle1.is_point_inside(this->m_vector2_4) == true);
	}

	bounding_circle2.add_bounding(bounding_circle3);
	EXPECT_FALSE(bounding_circle2.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_circle2.is_point_inside(this->m_vector2_2) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_circle2.is_point_inside(this->m_vector2_3) == true);
	}
	EXPECT_FALSE(bounding_circle2.is_point_inside(this->m_vector2_4) == true);

	bounding_circle2.add_bounding(bounding_circle4);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_FALSE(bounding_circle2.is_point_inside(this->m_vector2_1) == true);
	}
	EXPECT_TRUE(bounding_circle2.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_circle2.is_point_inside(this->m_vector2_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_circle2.is_point_inside(this->m_vector2_4) == true);
	}

	bounding_circle3.add_bounding(bounding_circle4);
	EXPECT_FALSE(bounding_circle3.is_point_inside(this->m_vector2_1) == true);
	EXPECT_FALSE(bounding_circle3.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_circle3.is_point_inside(this->m_vector2_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_circle3.is_point_inside(this->m_vector2_4) == true);
	}

	bounding_circle2.add_bounding(bounding_circle4);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_FALSE(bounding_circle2.is_point_inside(this->m_vector2_1) == true);
	}
	EXPECT_TRUE(bounding_circle2.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_circle2.is_point_inside(this->m_vector2_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		EXPECT_TRUE(bounding_circle2.is_point_inside(this->m_vector2_4) == true);
	}
}

TYPED_TEST(BoundingTest, BoundingCircle_bounding_circle_collision)
{
	ror::Round<ror::Vector2<TypeParam>> bounding_circle1;
	ror::Round<ror::Vector2<TypeParam>> bounding_circle2;
	ror::Round<ror::Vector2<TypeParam>> bounding_circle3;
	ror::Round<ror::Vector2<TypeParam>> bounding_circle4;
	ror::Round<ror::Vector2<TypeParam>> bounding_circle5;

	bounding_circle1.set_radius(10.0f);
	bounding_circle2.set_radius(10.0f);
	bounding_circle3.set_radius(10.0f);
	bounding_circle4.set_radius(10.0f);
	bounding_circle5.set_radius(static_cast<TypeParam>(60.99f));

	bounding_circle1.set_center(this->m_vector2_1);
	bounding_circle2.set_center(this->m_vector2_2);
	bounding_circle3.set_center(this->m_vector2_3);
	bounding_circle4.set_center(this->m_vector2_4);

	EXPECT_TRUE(bounding_circle1.collision(bounding_circle1) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_circle1.collision(bounding_circle2) == ror::CollisionType::intersects);
	EXPECT_FALSE(bounding_circle1.collision(bounding_circle3) == ror::CollisionType::intersects);
	EXPECT_FALSE(bounding_circle1.collision(bounding_circle4) == ror::CollisionType::intersects);

	EXPECT_TRUE(bounding_circle2.collision(bounding_circle1) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_circle2.collision(bounding_circle2) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_circle2.collision(bounding_circle3) == ror::CollisionType::intersects);
	EXPECT_FALSE(bounding_circle2.collision(bounding_circle4) == ror::CollisionType::intersects);

	EXPECT_FALSE(bounding_circle3.collision(bounding_circle1) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_circle3.collision(bounding_circle2) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_circle3.collision(bounding_circle3) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_circle3.collision(bounding_circle4) == ror::CollisionType::intersects);

	EXPECT_FALSE(bounding_circle4.collision(bounding_circle1) == ror::CollisionType::intersects);
	EXPECT_FALSE(bounding_circle4.collision(bounding_circle2) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_circle4.collision(bounding_circle3) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_circle4.collision(bounding_circle4) == ror::CollisionType::inside);

	EXPECT_TRUE(bounding_circle5.collision(bounding_circle1) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_circle5.collision(bounding_circle2) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_circle5.collision(bounding_circle3) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_circle5.collision(bounding_circle4) == ror::CollisionType::intersects);

	bounding_circle5.set_radius(80.0f);
	EXPECT_TRUE(bounding_circle5.collision(bounding_circle4) == ror::CollisionType::inside);
}

TYPED_TEST(BoundingTest, BoundingCircle_create_from_points)
{
	std::vector<ror::Vector2<TypeParam>> points;
	points.reserve(4);

	points.push_back(this->m_vector2_1);
	points.push_back(this->m_vector2_2);
	points.push_back(this->m_vector2_3);
	points.push_back(this->m_vector2_4);

	ror::Round<ror::Vector2<TypeParam>> bounding_circle1;

	this->m_bounding_circle.set_radius(0);
	this->m_bounding_circle.set_center(this->m_vector2_0);

	this->m_bounding_circle.create_from_points(points);
	this->m_bounding_circle.set_radius(this->m_bounding_circle.radius() + static_cast<TypeParam>(2));

	EXPECT_TRUE(this->m_bounding_circle.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(this->m_bounding_circle.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(this->m_bounding_circle.is_point_inside(this->m_vector2_3) == true);
	EXPECT_TRUE(this->m_bounding_circle.is_point_inside(this->m_vector2_4) == true);

	auto distanceFromCenterToCenter = (this->m_vector2_4 - this->m_vector2_1).length() / 2;

	EXPECT_GT(this->m_bounding_circle.radius(), distanceFromCenterToCenter);
}

TYPED_TEST(BoundingTest, BoundingCircle_system_test)
{
	if constexpr (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		ror::Round<ror::Vector2<TypeParam>> bounding_circle0;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_small1;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_small2;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_small3;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_small4;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_small5;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_same1;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_same2;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_same3;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_same4;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_big;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_small_inside_big;
		ror::Round<ror::Vector2<TypeParam>> bounding_circle_huge;

		bounding_circle0.set_radius(1.0f);
		bounding_circle_small1.set_radius(0.2f);
		bounding_circle_small2.set_radius(0.2f);
		bounding_circle_small3.set_radius(0.2f);
		bounding_circle_small4.set_radius(0.2f);
		bounding_circle_small5.set_radius(0.2f);
		bounding_circle_same1.set_radius(1.5f);
		bounding_circle_same2.set_radius(1.5f);
		bounding_circle_same3.set_radius(1.5f);
		bounding_circle_same4.set_radius(1.5f);
		bounding_circle_big.set_radius(3.0f);
		bounding_circle_small_inside_big.set_radius(0.8f);
		bounding_circle_huge.set_radius(11.0f);

		bounding_circle0.set_center(ror::Vector2<TypeParam>(0.0f, 0.0f));
		bounding_circle_small1.set_center(ror::Vector2<TypeParam>(4.0f, 2.0f));
		bounding_circle_small2.set_center(ror::Vector2<TypeParam>(2.0f, 4.0f));
		bounding_circle_small3.set_center(ror::Vector2<TypeParam>(0.0f, 4.5f));
		bounding_circle_small4.set_center(ror::Vector2<TypeParam>(-2.6f, -0.3f));
		bounding_circle_small5.set_center(ror::Vector2<TypeParam>(-0.8f, -2.3f));
		bounding_circle_same1.set_center(ror::Vector2<TypeParam>(2.391f, -1.634f));
		bounding_circle_same2.set_center(ror::Vector2<TypeParam>(2.0f, 2.0f));
		bounding_circle_same3.set_center(ror::Vector2<TypeParam>(-2.0f, 3.0f));
		bounding_circle_same4.set_center(ror::Vector2<TypeParam>(-3.338f, -1.991f));
		bounding_circle_big.set_center(ror::Vector2<TypeParam>(6.0f, 5.0f));
		bounding_circle_small_inside_big.set_center(ror::Vector2<TypeParam>(7.9f, 4.5f));
		bounding_circle_huge.set_center(ror::Vector2<TypeParam>(0.0f, 0.0f));

		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle0), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_small_inside_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle_huge), ror::CollisionType::inside);

		EXPECT_EQ(bounding_circle0.collision(bounding_circle0), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small1), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small2), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small3), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small4), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small5), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same1), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same2), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same3), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same4), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small_inside_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_huge), ror::CollisionType::intersects);

		bounding_circle0.add_bounding(bounding_circle_same1);
		bounding_circle0.add_bounding(bounding_circle_same2);
		bounding_circle0.add_bounding(bounding_circle_same3);
		bounding_circle0.add_bounding(bounding_circle_same4);

		bounding_circle0.add_bounding(bounding_circle_small1);
		bounding_circle0.add_bounding(bounding_circle_small2);
		bounding_circle0.add_bounding(bounding_circle_small3);
		bounding_circle0.add_bounding(bounding_circle_small4);
		bounding_circle0.add_bounding(bounding_circle_small5);

		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_big), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small_inside_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_huge), ror::CollisionType::intersects);

		bounding_circle0.add_bounding(bounding_circle_big);
		bounding_circle0.add_bounding(bounding_circle_small_inside_big);

		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_small_inside_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_circle0.collision(bounding_circle_huge), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_circle_huge.collision(bounding_circle0), ror::CollisionType::inside);

		// Testing creating from points
		std::vector<ror::Vector2<TypeParam>> points;

		points.push_back(ror::Vector2<TypeParam>(7.9f, 4.5f));
		points.push_back(ror::Vector2<TypeParam>(0.0f, 0.0f));
		points.push_back(ror::Vector2<TypeParam>(4.0f, 2.0f));
		points.push_back(ror::Vector2<TypeParam>(2.0f, 4.0f));
		points.push_back(ror::Vector2<TypeParam>(0.0f, 4.5f));
		points.push_back(ror::Vector2<TypeParam>(-2.6f, -0.3f));
		points.push_back(ror::Vector2<TypeParam>(-0.8f, -2.3f));
		points.push_back(ror::Vector2<TypeParam>(2.391f, -1.634f));
		points.push_back(ror::Vector2<TypeParam>(2.0f, 2.0f));
		points.push_back(ror::Vector2<TypeParam>(-2.0f, 3.0f));
		points.push_back(ror::Vector2<TypeParam>(-3.338f, -1.991f));
		points.push_back(ror::Vector2<TypeParam>(0.0f, 0.0f));

		ror::Round<ror::Vector2<TypeParam>> bounding_circle_from_points;
		bounding_circle_from_points.create_from_points(points);

		EXPECT_EQ(bounding_circle0.collision(bounding_circle_from_points), ror::CollisionType::inside);
	}
}

// Bounding Box tests

template <class _type>
ror::Vector3<_type> min_vector3()
{
	return ror::Vector3<_type>(std::numeric_limits<_type>::min(), std::numeric_limits<_type>::min(), std::numeric_limits<_type>::min());
}

template <class _type>
ror::Vector3<_type> max_vector3()
{
	return ror::Vector3<_type>(std::numeric_limits<_type>::max(), std::numeric_limits<_type>::max(), std::numeric_limits<_type>::max());
}

template <class _type>
ror::Vector3<_type> zero_vector3()
{
	return ror::Vector3<_type>(0.0f, 0.0f, 0.0f);
}

TYPED_TEST(BoundingTest, BoundingBox_constructors_work)
{
	ror::Box<ror::Vector3<TypeParam>> bounding_box1;
	EXPECT_TRUE(bounding_box1.minimum() == max_vector3<TypeParam>());
	EXPECT_TRUE(bounding_box1.maximum() == -min_vector3<TypeParam>());
	EXPECT_TRUE(bounding_box1.m_type == ror::BoundingType::box_3d);

	EXPECT_TRUE(bounding_box1.minimum() == this->m_bounding_box.minimum());
	EXPECT_TRUE(bounding_box1.maximum() == this->m_bounding_box.maximum());
	EXPECT_TRUE(bounding_box1.m_type == this->m_bounding_box.m_type);

	ror::Box<ror::Vector3<TypeParam>> bounding_box2(bounding_box1);
	EXPECT_TRUE(bounding_box1.minimum() == bounding_box2.minimum());
	EXPECT_TRUE(bounding_box1.maximum() == bounding_box1.maximum());
	EXPECT_TRUE(bounding_box1.m_type == bounding_box1.m_type);
}

TYPED_TEST(BoundingTest, BoundingBox_accessors_mutators)
{
	ror::Box<ror::Vector3<TypeParam>> bounding_box1;

	ror::Vector3<TypeParam> maximum(static_cast<TypeParam>(2.5f), static_cast<TypeParam>(3.0f), static_cast<TypeParam>(0.01f));
	ror::Vector3<TypeParam> minimum(static_cast<TypeParam>(-2.5f), static_cast<TypeParam>(-3.0f), static_cast<TypeParam>(5.05f));

	bounding_box1.set(minimum, maximum);

	EXPECT_TRUE(bounding_box1.minimum() == minimum);
	EXPECT_TRUE(bounding_box1.maximum() == maximum);
}

TYPED_TEST(BoundingTest, BoundingBox_add_point)
{
	ror::Box<ror::Vector3<TypeParam>> bounding_box1;
	bounding_box1.set(this->m_vector3_1, this->m_vector3_3);
	bounding_box1.add_point(this->m_vector3_2);

	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_3) == true);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_1) == true);

	bounding_box1.add_point(this->m_vector3_4);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_4) == true);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_3) == true);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_1) == true);

	ror::Vector3<TypeParam> minimum = bounding_box1.minimum();
	ror::Vector3<TypeParam> maximum = bounding_box1.maximum();

	bounding_box1.add_point(this->m_vector3_2);
	bounding_box1.add_point(this->m_vector3_4);
	EXPECT_TRUE(bounding_box1.minimum() == minimum);
	EXPECT_TRUE(bounding_box1.maximum() == maximum);

	ror::Vector3<TypeParam> pointx(40.0f, 0.0f, 0.0f);
	ror::Vector3<TypeParam> pointypositive(20.0f, 20.0f, 0.0f);
	ror::Vector3<TypeParam> pointynegative(20.0f, -20.0f, 0.0f);
	ror::Vector3<TypeParam> pointzpositive(20.0f, 20.0f, 20.0f);
	ror::Vector3<TypeParam> pointznegative(20.0f, 20.0f, -20.0f);

	ror::Vector3<TypeParam> point1(0.0f, 0.0f, 0.0f);

	this->m_bounding_box.add_point(point1);
	this->m_bounding_box.add_point(pointx);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().x - this->m_bounding_box.minimum().x)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().y - this->m_bounding_box.minimum().y)) - 0.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().z - this->m_bounding_box.minimum().z)) - 0.0f) < ror::ror_epsilon_squared);

	this->m_bounding_box.add_point(pointypositive);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().x - this->m_bounding_box.minimum().x)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().y - this->m_bounding_box.minimum().y)) - 20.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().z - this->m_bounding_box.minimum().z)) - 0.0f) < ror::ror_epsilon_squared);

	this->m_bounding_box.add_point(pointynegative);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().x - this->m_bounding_box.minimum().x)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().y - this->m_bounding_box.minimum().y)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().z - this->m_bounding_box.minimum().z)) - 0.0f) < ror::ror_epsilon_squared);

	this->m_bounding_box.add_point(pointzpositive);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().x - this->m_bounding_box.minimum().x)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().y - this->m_bounding_box.minimum().y)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().z - this->m_bounding_box.minimum().z)) - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_box.add_point(pointznegative);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().x - this->m_bounding_box.minimum().x)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().y - this->m_bounding_box.minimum().y)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_box.maximum().z - this->m_bounding_box.minimum().z)) - 40.0f) < ror::ror_epsilon_squared);
}

TYPED_TEST(BoundingTest, BoundingBox_is_point_inside)
{
	ror::Box<ror::Vector3<TypeParam>> bounding_box1;
	ror::Box<ror::Vector3<TypeParam>> bounding_box2;

	ror::Vector3<TypeParam> point0(0.0f, 0.0f, 0.0f);
	ror::Vector3<TypeParam> point1(10.0f, 10.0f, 10.0f);

	this->m_bounding_box.set(point0, point1);

	EXPECT_TRUE(this->m_bounding_box.is_point_inside(this->m_vector3_1) == true);
	ASSERT_FALSE(this->m_bounding_box.is_point_inside(this->m_vector3_2) == true);
	ASSERT_FALSE(this->m_bounding_box.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(this->m_bounding_box.is_point_inside(this->m_vector3_4) == true);

	this->m_bounding_box.add_point(this->m_vector3_4);
	EXPECT_TRUE(this->m_bounding_box.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(this->m_bounding_box.is_point_inside(this->m_vector_negative3_3) == true);
	ASSERT_FALSE(this->m_bounding_box.is_point_inside(this->m_vector_negative3_2) == true);

	this->m_bounding_box.add_point(this->m_vector_negative3_4);
	EXPECT_TRUE(this->m_bounding_box.is_point_inside(this->m_vector_negative3_3) == true);
	EXPECT_TRUE(this->m_bounding_box.is_point_inside(this->m_vector_negative3_2) == true);
}

TYPED_TEST(BoundingTest, BoundingBox_add_bounding_box)
{
	ror::Vector3<TypeParam> minimum;
	ror::Vector3<TypeParam> maximum;

	ror::Box<ror::Vector3<TypeParam>> bounding_box1;
	ror::Box<ror::Vector3<TypeParam>> bounding_box2;
	ror::Box<ror::Vector3<TypeParam>> bounding_box3;
	ror::Box<ror::Vector3<TypeParam>> bounding_box4;

	bounding_box1.set(-this->m_vector3_1, this->m_vector3_1);
	bounding_box2.set(-this->m_vector3_2, this->m_vector3_2);
	bounding_box3.set(-this->m_vector3_3, this->m_vector3_3);
	bounding_box4.set(-this->m_vector3_4, this->m_vector3_4);

	minimum = bounding_box1.minimum();
	maximum = bounding_box1.maximum();

	bounding_box1.add_bounding(bounding_box1);

	EXPECT_TRUE(bounding_box1.maximum() == maximum);
	EXPECT_TRUE(bounding_box1.minimum() == minimum);

	minimum = bounding_box2.minimum();
	maximum = bounding_box2.maximum();

	bounding_box2.add_bounding(bounding_box2);

	EXPECT_TRUE(bounding_box2.maximum() == maximum);
	EXPECT_TRUE(bounding_box2.minimum() == minimum);

	minimum = bounding_box3.minimum();
	maximum = bounding_box3.maximum();

	bounding_box3.add_bounding(bounding_box3);

	EXPECT_TRUE(bounding_box3.maximum() == maximum);
	EXPECT_TRUE(bounding_box3.minimum() == minimum);

	minimum = bounding_box4.minimum();
	maximum = bounding_box4.maximum();

	bounding_box4.add_bounding(bounding_box4);

	EXPECT_TRUE(bounding_box4.maximum() == maximum);
	EXPECT_TRUE(bounding_box4.minimum() == minimum);

	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_1) == true);
	ASSERT_FALSE(bounding_box1.is_point_inside(this->m_vector3_2) == true);
	ASSERT_FALSE(bounding_box1.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(bounding_box1.is_point_inside(this->m_vector3_4) == true);

	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_2) == true);
	ASSERT_FALSE(bounding_box2.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(bounding_box2.is_point_inside(this->m_vector3_4) == true);

	EXPECT_TRUE(bounding_box3.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_box3.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_box3.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(bounding_box3.is_point_inside(this->m_vector3_4) == true);

	EXPECT_TRUE(bounding_box4.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_box4.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_box4.is_point_inside(this->m_vector3_3) == true);
	EXPECT_TRUE(bounding_box4.is_point_inside(this->m_vector3_4) == true);

	bounding_box1.add_bounding(bounding_box2);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_2) == true);
	ASSERT_FALSE(bounding_box1.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(bounding_box1.is_point_inside(this->m_vector3_4) == true);

	bounding_box1.add_bounding(bounding_box3);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(bounding_box1.is_point_inside(this->m_vector3_4) == true);

	bounding_box1.add_bounding(bounding_box4);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_3) == true);
	EXPECT_TRUE(bounding_box1.is_point_inside(this->m_vector3_4) == true);

	bounding_box2.add_bounding(bounding_box3);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_3) == true);
	ASSERT_FALSE(bounding_box2.is_point_inside(this->m_vector3_4) == true);

	bounding_box2.add_bounding(bounding_box4);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_3) == true);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_4) == true);

	bounding_box3.add_bounding(bounding_box4);
	EXPECT_TRUE(bounding_box3.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_box3.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_box3.is_point_inside(this->m_vector3_3) == true);
	EXPECT_TRUE(bounding_box3.is_point_inside(this->m_vector3_4) == true);

	bounding_box2.add_bounding(bounding_box4);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_3) == true);
	EXPECT_TRUE(bounding_box2.is_point_inside(this->m_vector3_4) == true);
}

TYPED_TEST(BoundingTest, BoundingBox_collision)
{
	ror::Box<ror::Vector3<TypeParam>> bounding_box1;
	ror::Box<ror::Vector3<TypeParam>> bounding_box2;
	ror::Box<ror::Vector3<TypeParam>> bounding_box3;
	ror::Box<ror::Vector3<TypeParam>> bounding_box4;
	ror::Box<ror::Vector3<TypeParam>> boundingBoxAxisAligned5;

	bounding_box1.set(zero_vector3<TypeParam>(), this->m_vector3_1);
	bounding_box2.set(this->m_vector3_1, this->m_vector3_2);
	bounding_box3.set(this->m_vector3_2, this->m_vector3_3);
	bounding_box4.set(this->m_vector3_3, this->m_vector3_4);
	boundingBoxAxisAligned5.set(zero_vector3<TypeParam>(), this->m_vector3_4);

	EXPECT_TRUE(bounding_box1.collision(bounding_box1) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_box1.collision(bounding_box2) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_box1.collision(bounding_box3) == ror::CollisionType::outside);
	EXPECT_TRUE(bounding_box1.collision(bounding_box4) == ror::CollisionType::outside);

	EXPECT_TRUE(bounding_box2.collision(bounding_box1) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_box2.collision(bounding_box2) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_box2.collision(bounding_box3) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_box2.collision(bounding_box4) == ror::CollisionType::outside);

	EXPECT_TRUE(bounding_box3.collision(bounding_box1) == ror::CollisionType::outside);
	EXPECT_TRUE(bounding_box3.collision(bounding_box2) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_box3.collision(bounding_box3) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_box3.collision(bounding_box4) == ror::CollisionType::intersects);

	EXPECT_TRUE(bounding_box4.collision(bounding_box1) == ror::CollisionType::outside);
	EXPECT_TRUE(bounding_box4.collision(bounding_box2) == ror::CollisionType::outside);
	EXPECT_TRUE(bounding_box4.collision(bounding_box3) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_box4.collision(bounding_box4) == ror::CollisionType::inside);

	EXPECT_TRUE(boundingBoxAxisAligned5.collision(bounding_box1) == ror::CollisionType::inside);
	EXPECT_TRUE(boundingBoxAxisAligned5.collision(bounding_box2) == ror::CollisionType::inside);
	EXPECT_TRUE(boundingBoxAxisAligned5.collision(bounding_box3) == ror::CollisionType::inside);
	EXPECT_TRUE(boundingBoxAxisAligned5.collision(bounding_box4) == ror::CollisionType::inside);

	bounding_box3.set(bounding_box3.minimum() - (this->m_vector3_1 * static_cast<TypeParam>(0.5)), bounding_box3.maximum() + (this->m_vector3_1 * static_cast<TypeParam>(0.5)));

	EXPECT_TRUE(bounding_box3.collision(bounding_box1) == ror::CollisionType::outside);
	EXPECT_TRUE(bounding_box3.collision(bounding_box2) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_box3.collision(bounding_box3) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_box3.collision(bounding_box4) == ror::CollisionType::intersects);
}

TYPED_TEST(BoundingTest, BoundingBox_create_from_points)
{
	std::vector<ror::Vector3<TypeParam>> points;
	points.reserve(4);

	points.push_back(this->m_vector3_1);
	points.push_back(this->m_vector3_2);
	points.push_back(this->m_vector3_3);
	points.push_back(this->m_vector3_4);

	this->m_bounding_box.create_from_points(points);

	EXPECT_TRUE(this->m_bounding_box.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(this->m_bounding_box.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(this->m_bounding_box.is_point_inside(this->m_vector3_3) == true);
	EXPECT_TRUE(this->m_bounding_box.is_point_inside(this->m_vector3_4) == true);
}

TYPED_TEST(BoundingTest, BoundingBox_system_test)
{
	// These tests are only valid for decimal types
	if constexpr (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		ror::Box<ror::Vector3<TypeParam>> bounding_box0;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_small1;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_small2;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_small3;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_small4;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_small5;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_same1;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_same2;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_same3;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_same4;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_big;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_small_inside_big;
		ror::Box<ror::Vector3<TypeParam>> bounding_box_huge;

		bounding_box0.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-1.0f)),
						  ror::Vector3<TypeParam>(static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f)));
		bounding_box_big.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(3.3f), static_cast<TypeParam>(2.3f), static_cast<TypeParam>(-1.0f)),
							 ror::Vector3<TypeParam>(static_cast<TypeParam>(9.3f), static_cast<TypeParam>(8.3f), static_cast<TypeParam>(1.0f)));
		bounding_box_huge.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-5.0f), static_cast<TypeParam>(-5.0f), static_cast<TypeParam>(-1.0f)),
							  ror::Vector3<TypeParam>(static_cast<TypeParam>(10.0f), static_cast<TypeParam>(10.0f), static_cast<TypeParam>(1.0f)));
		bounding_box_same1.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(-3.0f), static_cast<TypeParam>(-1.0f)),
							   ror::Vector3<TypeParam>(static_cast<TypeParam>(4.0f), static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(1.0f)));
		bounding_box_same2.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(0.7f), static_cast<TypeParam>(1.5f), static_cast<TypeParam>(-1.0f)),
							   ror::Vector3<TypeParam>(static_cast<TypeParam>(3.2f), static_cast<TypeParam>(4.0f), static_cast<TypeParam>(1.0f)));
		bounding_box_same3.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-3.0f), static_cast<TypeParam>(1.5f), static_cast<TypeParam>(-1.0f)),
							   ror::Vector3<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.0f), static_cast<TypeParam>(1.0f)));
		bounding_box_same4.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-4.5f), static_cast<TypeParam>(-3.5f), static_cast<TypeParam>(-1.0f)),
							   ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.0f), static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(1.0f)));
		bounding_box_small1.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(3.5f), static_cast<TypeParam>(1.8f), static_cast<TypeParam>(-1.0f)),
								ror::Vector3<TypeParam>(static_cast<TypeParam>(3.9f), static_cast<TypeParam>(2.2f), static_cast<TypeParam>(1.0f)));
		bounding_box_small2.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(4.5f), static_cast<TypeParam>(-1.0f)),
								ror::Vector3<TypeParam>(static_cast<TypeParam>(1.9f), static_cast<TypeParam>(4.9f), static_cast<TypeParam>(1.0f)));
		bounding_box_small3.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.5f), static_cast<TypeParam>(-1.0f)),
								ror::Vector3<TypeParam>(static_cast<TypeParam>(-0.1f), static_cast<TypeParam>(4.9f), static_cast<TypeParam>(1.0f)));
		bounding_box_small4.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.5f), static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(-1.0f)),
								ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-0.1f), static_cast<TypeParam>(1.0f)));
		bounding_box_small5.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-2.5f), static_cast<TypeParam>(-1.0f)),
								ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(1.0f)));
		bounding_box_small_inside_big.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(6.0f), static_cast<TypeParam>(3.0f), static_cast<TypeParam>(-1.0f)),
										  ror::Vector3<TypeParam>(static_cast<TypeParam>(6.4f), static_cast<TypeParam>(3.4f), static_cast<TypeParam>(1.0f)));

		// Tests in 2-Dimensions
		EXPECT_EQ(bounding_box_huge.collision(bounding_box0), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small_inside_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_huge), ror::CollisionType::inside);

		EXPECT_EQ(bounding_box0.collision(bounding_box0), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small1), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small2), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small3), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small4), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small5), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same1), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same2), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same3), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same4), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small_inside_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_huge), ror::CollisionType::intersects);

		bounding_box0.add_bounding(bounding_box_same1);
		bounding_box0.add_bounding(bounding_box_same2);
		bounding_box0.add_bounding(bounding_box_same3);
		bounding_box0.add_bounding(bounding_box_same4);

		bounding_box0.add_bounding(bounding_box_small1);
		bounding_box0.add_bounding(bounding_box_small2);
		bounding_box0.add_bounding(bounding_box_small3);
		bounding_box0.add_bounding(bounding_box_small4);
		bounding_box0.add_bounding(bounding_box_small5);

		EXPECT_EQ(bounding_box0.collision(bounding_box_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_big), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small_inside_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_huge), ror::CollisionType::intersects);

		bounding_box0.add_bounding(bounding_box_big);
		bounding_box0.add_bounding(bounding_box_small_inside_big);

		EXPECT_EQ(bounding_box0.collision(bounding_box_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small_inside_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_huge), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box0), ror::CollisionType::inside);

		// Tests in 3 dimensions
		bounding_box0.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-0.5f)),
						  ror::Vector3<TypeParam>(static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f)));
		bounding_box_big.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(3.3f), static_cast<TypeParam>(2.3f), static_cast<TypeParam>(-1.0f)),
							 ror::Vector3<TypeParam>(static_cast<TypeParam>(9.3f), static_cast<TypeParam>(8.3f), static_cast<TypeParam>(1.0f)));
		bounding_box_huge.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-5.0f), static_cast<TypeParam>(-5.0f), static_cast<TypeParam>(-5.0f)),
							  ror::Vector3<TypeParam>(static_cast<TypeParam>(10.0f), static_cast<TypeParam>(10.0f), static_cast<TypeParam>(10.0f)));
		bounding_box_same1.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(-3.0f), static_cast<TypeParam>(-0.7f)),
							   ror::Vector3<TypeParam>(static_cast<TypeParam>(4.0f), static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(1.4f)));
		bounding_box_same2.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(0.7f), static_cast<TypeParam>(1.5f), static_cast<TypeParam>(1.0f)),
							   ror::Vector3<TypeParam>(static_cast<TypeParam>(3.2f), static_cast<TypeParam>(4.0f), static_cast<TypeParam>(1.7f)));
		bounding_box_same3.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-3.0f), static_cast<TypeParam>(1.5f), static_cast<TypeParam>(1.5f)),
							   ror::Vector3<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.0f), static_cast<TypeParam>(12.2f)));
		bounding_box_same4.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-4.5f), static_cast<TypeParam>(-3.5f), static_cast<TypeParam>(0.2f)),
							   ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.0f), static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(1.0f)));
		bounding_box_small1.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(3.5f), static_cast<TypeParam>(1.8f), static_cast<TypeParam>(-21.0f)),
								ror::Vector3<TypeParam>(static_cast<TypeParam>(3.9f), static_cast<TypeParam>(2.2f), static_cast<TypeParam>(0.0f)));
		bounding_box_small2.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(4.5f), static_cast<TypeParam>(3.2f)),
								ror::Vector3<TypeParam>(static_cast<TypeParam>(1.9f), static_cast<TypeParam>(4.9f), static_cast<TypeParam>(4.5f)));
		bounding_box_small3.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.5f), static_cast<TypeParam>(2.0f)),
								ror::Vector3<TypeParam>(static_cast<TypeParam>(-0.1f), static_cast<TypeParam>(4.9f), static_cast<TypeParam>(5.7f)));
		bounding_box_small4.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.5f), static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(-1.0f)),
								ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-0.1f), static_cast<TypeParam>(0.1f)));
		bounding_box_small5.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-2.5f), static_cast<TypeParam>(-1.0f)),
								ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-0.2f)));
		bounding_box_small_inside_big.set(ror::Vector3<TypeParam>(static_cast<TypeParam>(6.0f), static_cast<TypeParam>(3.0f), static_cast<TypeParam>(-7.0f)),
										  ror::Vector3<TypeParam>(static_cast<TypeParam>(6.4f), static_cast<TypeParam>(3.4f), static_cast<TypeParam>(1.0f)));

		// Tests in 2-Dimensions
		EXPECT_EQ(bounding_box_huge.collision(bounding_box0), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small1), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_same3), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_small_inside_big), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box_huge), ror::CollisionType::inside);

		EXPECT_EQ(bounding_box0.collision(bounding_box0), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small1), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small2), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small3), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small4), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small5), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same1), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same2), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same3), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same4), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small_inside_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_huge), ror::CollisionType::intersects);

		bounding_box0.add_bounding(bounding_box_same1);
		bounding_box0.add_bounding(bounding_box_same2);
		bounding_box0.add_bounding(bounding_box_same3);
		bounding_box0.add_bounding(bounding_box_same4);

		bounding_box0.add_bounding(bounding_box_small1);
		bounding_box0.add_bounding(bounding_box_small2);
		bounding_box0.add_bounding(bounding_box_small3);
		bounding_box0.add_bounding(bounding_box_small4);
		bounding_box0.add_bounding(bounding_box_small5);

		EXPECT_EQ(bounding_box0.collision(bounding_box_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_big), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small_inside_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_huge), ror::CollisionType::intersects);

		bounding_box0.add_bounding(bounding_box_big);
		bounding_box0.add_bounding(bounding_box_small_inside_big);

		EXPECT_EQ(bounding_box0.collision(bounding_box_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_small_inside_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_box0.collision(bounding_box_huge), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_box_huge.collision(bounding_box0), ror::CollisionType::intersects);

		// Testing creating from points
		std::vector<ror::Vector3<TypeParam>> points;
		points.reserve(27);

		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-0.5f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(3.5f), static_cast<TypeParam>(1.8f), static_cast<TypeParam>(-21.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(4.5f), static_cast<TypeParam>(3.2f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.5f), static_cast<TypeParam>(2.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.5f), static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(-1.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-2.5f), static_cast<TypeParam>(-1.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(-3.0f), static_cast<TypeParam>(-0.7f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(0.7f), static_cast<TypeParam>(1.5f), static_cast<TypeParam>(1.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-3.0f), static_cast<TypeParam>(1.5f), static_cast<TypeParam>(1.5f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-4.5f), static_cast<TypeParam>(-3.5f), static_cast<TypeParam>(0.2f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(3.3f), static_cast<TypeParam>(2.3f), static_cast<TypeParam>(-1.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(6.0f), static_cast<TypeParam>(3.0f), static_cast<TypeParam>(-7.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-5.0f), static_cast<TypeParam>(-5.0f), static_cast<TypeParam>(-5.0f)));

		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(3.9f), static_cast<TypeParam>(2.2f), static_cast<TypeParam>(0.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(1.9f), static_cast<TypeParam>(4.9f), static_cast<TypeParam>(4.5f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-0.1f), static_cast<TypeParam>(4.9f), static_cast<TypeParam>(5.7f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-0.1f), static_cast<TypeParam>(0.1f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-0.2f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(4.0f), static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(1.4f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(3.2f), static_cast<TypeParam>(4.0f), static_cast<TypeParam>(1.7f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.0f), static_cast<TypeParam>(12.2f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(-2.0f), static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(1.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(9.3f), static_cast<TypeParam>(8.3f), static_cast<TypeParam>(1.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(6.4f), static_cast<TypeParam>(3.4f), static_cast<TypeParam>(1.0f)));
		points.push_back(ror::Vector3<TypeParam>(static_cast<TypeParam>(10.0f), static_cast<TypeParam>(10.0f), static_cast<TypeParam>(10.0f)));

		ror::Box<ror::Vector3<TypeParam>> boundingBoxFromPoints;
		boundingBoxFromPoints.create_from_points(points);

		EXPECT_EQ(bounding_box0.collision(boundingBoxFromPoints), ror::CollisionType::intersects);

		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_small1), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_small2), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_small3), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_small4), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_small5), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_same1), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_same2), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_same3), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_same4), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_big), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_small_inside_big), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box_huge), ror::CollisionType::inside);
		EXPECT_EQ(boundingBoxFromPoints.collision(bounding_box0), ror::CollisionType::inside);
	}
}

// Bounding Rectangle tests
template <class _type>
ror::Vector2<_type> min_vector2()
{
	return ror::Vector2<_type>(std::numeric_limits<_type>::min(), std::numeric_limits<_type>::min());
}

template <class _type>
ror::Vector2<_type> max_vector2()
{
	return ror::Vector2<_type>(std::numeric_limits<_type>::max(), std::numeric_limits<_type>::max());
}

template <class _type>
ror::Vector2<_type> zero_vector2()
{
	return ror::Vector2<_type>(0.0f, 0.0f);
}

TYPED_TEST(BoundingTest, BoundingRectangle_constructors_work)
{
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle1;
	EXPECT_TRUE(bounding_rectangle1.minimum() == max_vector2<TypeParam>());
	EXPECT_TRUE(bounding_rectangle1.maximum() == -min_vector2<TypeParam>());
	EXPECT_TRUE(bounding_rectangle1.m_type == ror::BoundingType::rectangle_2d);

	EXPECT_TRUE(bounding_rectangle1.minimum() == this->m_bounding_rectangle.minimum());
	EXPECT_TRUE(bounding_rectangle1.maximum() == this->m_bounding_rectangle.maximum());
	EXPECT_TRUE(bounding_rectangle1.m_type == this->m_bounding_rectangle.m_type);

	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle2(bounding_rectangle1);
	EXPECT_TRUE(bounding_rectangle1.minimum() == bounding_rectangle2.minimum());
	EXPECT_TRUE(bounding_rectangle1.maximum() == bounding_rectangle1.maximum());
	EXPECT_TRUE(bounding_rectangle1.m_type == bounding_rectangle1.m_type);
}

TYPED_TEST(BoundingTest, BoundingRectangle_accessors_mutators)
{
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle1;

	ror::Vector2<TypeParam> maximum(static_cast<TypeParam>(2.5f), static_cast<TypeParam>(3.0f));
	ror::Vector2<TypeParam> minimum(static_cast<TypeParam>(-2.5f), static_cast<TypeParam>(-3.0f));

	bounding_rectangle1.set(minimum, maximum);

	EXPECT_TRUE(bounding_rectangle1.minimum() == minimum);
	EXPECT_TRUE(bounding_rectangle1.maximum() == maximum);
}

TYPED_TEST(BoundingTest, BoundingRectangle_add_point)
{
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle1;
	bounding_rectangle1.set(this->m_vector2_1, this->m_vector2_3);
	bounding_rectangle1.add_point(this->m_vector2_2);

	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_3) == true);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_1) == true);

	bounding_rectangle1.add_point(this->m_vector2_4);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_4) == true);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_3) == true);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_1) == true);

	ror::Vector2<TypeParam> minimum = bounding_rectangle1.minimum();
	ror::Vector2<TypeParam> maximum = bounding_rectangle1.maximum();

	bounding_rectangle1.add_point(this->m_vector2_2);
	bounding_rectangle1.add_point(this->m_vector2_4);
	EXPECT_TRUE(bounding_rectangle1.minimum() == minimum);
	EXPECT_TRUE(bounding_rectangle1.maximum() == maximum);

	ror::Vector2<TypeParam> pointx(40.0f, 0.0f);
	ror::Vector2<TypeParam> pointypositive(20.0f, 20.0f);
	ror::Vector2<TypeParam> pointynegative(20.0f, -20.0f);
	ror::Vector2<TypeParam> pointzpositive(20.0f, 20.0f);
	ror::Vector2<TypeParam> pointznegative(20.0f, 20.0f);

	ror::Vector2<TypeParam> point1(0.0f, 0.0f);

	this->m_bounding_rectangle.add_point(point1);
	this->m_bounding_rectangle.add_point(pointx);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_rectangle.maximum().x - this->m_bounding_rectangle.minimum().x)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_rectangle.maximum().y - this->m_bounding_rectangle.minimum().y)) - 0.0f) < ror::ror_epsilon_squared);

	this->m_bounding_rectangle.add_point(pointypositive);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_rectangle.maximum().x - this->m_bounding_rectangle.minimum().x)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_rectangle.maximum().y - this->m_bounding_rectangle.minimum().y)) - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_rectangle.add_point(pointynegative);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_rectangle.maximum().x - this->m_bounding_rectangle.minimum().x)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_rectangle.maximum().y - this->m_bounding_rectangle.minimum().y)) - 40.0f) < ror::ror_epsilon_squared);

	this->m_bounding_rectangle.add_point(pointzpositive);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_rectangle.maximum().x - this->m_bounding_rectangle.minimum().x)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_rectangle.maximum().y - this->m_bounding_rectangle.minimum().y)) - 40.0f) < ror::ror_epsilon_squared);

	this->m_bounding_rectangle.add_point(pointznegative);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_rectangle.maximum().x - this->m_bounding_rectangle.minimum().x)) - 40.0f) < ror::ror_epsilon_squared);
	EXPECT_TRUE(abs(static_cast<float32_t>((this->m_bounding_rectangle.maximum().y - this->m_bounding_rectangle.minimum().y)) - 40.0f) < ror::ror_epsilon_squared);
}

TYPED_TEST(BoundingTest, BoundingRectangle_is_point_inside)
{
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle1;
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle2;

	ror::Vector2<TypeParam> point0(0.0f, 0.0f);
	ror::Vector2<TypeParam> point1(10.0f, 10.0f);

	this->m_bounding_rectangle.set(point0, point1);

	EXPECT_TRUE(this->m_bounding_rectangle.is_point_inside(this->m_vector2_1) == true);
	ASSERT_FALSE(this->m_bounding_rectangle.is_point_inside(this->m_vector2_2) == true);
	ASSERT_FALSE(this->m_bounding_rectangle.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(this->m_bounding_rectangle.is_point_inside(this->m_vector2_4) == true);

	this->m_bounding_rectangle.add_point(this->m_vector2_4);
	EXPECT_TRUE(this->m_bounding_rectangle.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(this->m_bounding_rectangle.is_point_inside(this->m_vector_negative2_3) == true);
	ASSERT_FALSE(this->m_bounding_rectangle.is_point_inside(this->m_vector_negative2_2) == true);

	this->m_bounding_rectangle.add_point(this->m_vector_negative2_4);
	EXPECT_TRUE(this->m_bounding_rectangle.is_point_inside(this->m_vector_negative2_3) == true);
	EXPECT_TRUE(this->m_bounding_rectangle.is_point_inside(this->m_vector_negative2_2) == true);
}

TYPED_TEST(BoundingTest, BoundingRectangle_add_bounding_box)
{
	ror::Vector2<TypeParam> minimum;
	ror::Vector2<TypeParam> maximum;

	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle1;
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle2;
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle3;
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle4;

	bounding_rectangle1.set(-this->m_vector2_1, this->m_vector2_1);
	bounding_rectangle2.set(-this->m_vector2_2, this->m_vector2_2);
	bounding_rectangle3.set(-this->m_vector2_3, this->m_vector2_3);
	bounding_rectangle4.set(-this->m_vector2_4, this->m_vector2_4);

	minimum = bounding_rectangle1.minimum();
	maximum = bounding_rectangle1.maximum();

	bounding_rectangle1.add_bounding(bounding_rectangle1);

	EXPECT_TRUE(bounding_rectangle1.maximum() == maximum);
	EXPECT_TRUE(bounding_rectangle1.minimum() == minimum);

	minimum = bounding_rectangle2.minimum();
	maximum = bounding_rectangle2.maximum();

	bounding_rectangle2.add_bounding(bounding_rectangle2);

	EXPECT_TRUE(bounding_rectangle2.maximum() == maximum);
	EXPECT_TRUE(bounding_rectangle2.minimum() == minimum);

	minimum = bounding_rectangle3.minimum();
	maximum = bounding_rectangle3.maximum();

	bounding_rectangle3.add_bounding(bounding_rectangle3);

	EXPECT_TRUE(bounding_rectangle3.maximum() == maximum);
	EXPECT_TRUE(bounding_rectangle3.minimum() == minimum);

	minimum = bounding_rectangle4.minimum();
	maximum = bounding_rectangle4.maximum();

	bounding_rectangle4.add_bounding(bounding_rectangle4);

	EXPECT_TRUE(bounding_rectangle4.maximum() == maximum);
	EXPECT_TRUE(bounding_rectangle4.minimum() == minimum);

	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_1) == true);
	ASSERT_FALSE(bounding_rectangle1.is_point_inside(this->m_vector2_2) == true);
	ASSERT_FALSE(bounding_rectangle1.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(bounding_rectangle1.is_point_inside(this->m_vector2_4) == true);

	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_2) == true);
	ASSERT_FALSE(bounding_rectangle2.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(bounding_rectangle2.is_point_inside(this->m_vector2_4) == true);

	EXPECT_TRUE(bounding_rectangle3.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_rectangle3.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_rectangle3.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(bounding_rectangle3.is_point_inside(this->m_vector2_4) == true);

	EXPECT_TRUE(bounding_rectangle4.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_rectangle4.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_rectangle4.is_point_inside(this->m_vector2_3) == true);
	EXPECT_TRUE(bounding_rectangle4.is_point_inside(this->m_vector2_4) == true);

	bounding_rectangle1.add_bounding(bounding_rectangle2);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_2) == true);
	ASSERT_FALSE(bounding_rectangle1.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(bounding_rectangle1.is_point_inside(this->m_vector2_4) == true);

	bounding_rectangle1.add_bounding(bounding_rectangle3);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(bounding_rectangle1.is_point_inside(this->m_vector2_4) == true);

	bounding_rectangle1.add_bounding(bounding_rectangle4);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_3) == true);
	EXPECT_TRUE(bounding_rectangle1.is_point_inside(this->m_vector2_4) == true);

	bounding_rectangle2.add_bounding(bounding_rectangle3);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_3) == true);
	ASSERT_FALSE(bounding_rectangle2.is_point_inside(this->m_vector2_4) == true);

	bounding_rectangle2.add_bounding(bounding_rectangle4);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_3) == true);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_4) == true);

	bounding_rectangle3.add_bounding(bounding_rectangle4);
	EXPECT_TRUE(bounding_rectangle3.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_rectangle3.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_rectangle3.is_point_inside(this->m_vector2_3) == true);
	EXPECT_TRUE(bounding_rectangle3.is_point_inside(this->m_vector2_4) == true);

	bounding_rectangle2.add_bounding(bounding_rectangle4);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_3) == true);
	EXPECT_TRUE(bounding_rectangle2.is_point_inside(this->m_vector2_4) == true);
}

TYPED_TEST(BoundingTest, BoundingRectangle_collision)
{
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle1;
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle2;
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle3;
	ror::Box<ror::Vector2<TypeParam>> bounding_rectangle4;
	ror::Box<ror::Vector2<TypeParam>> boundingBoxAxisAligned5;

	bounding_rectangle1.set(zero_vector2<TypeParam>(), this->m_vector2_1);
	bounding_rectangle2.set(this->m_vector2_1, this->m_vector2_2);
	bounding_rectangle3.set(this->m_vector2_2, this->m_vector2_3);
	bounding_rectangle4.set(this->m_vector2_3, this->m_vector2_4);
	boundingBoxAxisAligned5.set(zero_vector2<TypeParam>(), this->m_vector2_4);

	EXPECT_TRUE(bounding_rectangle1.collision(bounding_rectangle1) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_rectangle1.collision(bounding_rectangle2) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_rectangle1.collision(bounding_rectangle3) == ror::CollisionType::outside);
	EXPECT_TRUE(bounding_rectangle1.collision(bounding_rectangle4) == ror::CollisionType::outside);

	EXPECT_TRUE(bounding_rectangle2.collision(bounding_rectangle1) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_rectangle2.collision(bounding_rectangle2) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_rectangle2.collision(bounding_rectangle3) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_rectangle2.collision(bounding_rectangle4) == ror::CollisionType::outside);

	EXPECT_TRUE(bounding_rectangle3.collision(bounding_rectangle1) == ror::CollisionType::outside);
	EXPECT_TRUE(bounding_rectangle3.collision(bounding_rectangle2) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_rectangle3.collision(bounding_rectangle3) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_rectangle3.collision(bounding_rectangle4) == ror::CollisionType::intersects);

	EXPECT_TRUE(bounding_rectangle4.collision(bounding_rectangle1) == ror::CollisionType::outside);
	EXPECT_TRUE(bounding_rectangle4.collision(bounding_rectangle2) == ror::CollisionType::outside);
	EXPECT_TRUE(bounding_rectangle4.collision(bounding_rectangle3) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_rectangle4.collision(bounding_rectangle4) == ror::CollisionType::inside);

	EXPECT_TRUE(boundingBoxAxisAligned5.collision(bounding_rectangle1) == ror::CollisionType::inside);
	EXPECT_TRUE(boundingBoxAxisAligned5.collision(bounding_rectangle2) == ror::CollisionType::inside);
	EXPECT_TRUE(boundingBoxAxisAligned5.collision(bounding_rectangle3) == ror::CollisionType::inside);
	EXPECT_TRUE(boundingBoxAxisAligned5.collision(bounding_rectangle4) == ror::CollisionType::inside);

	bounding_rectangle3.set(bounding_rectangle3.minimum() - (this->m_vector2_1 * static_cast<TypeParam>(0.5)), bounding_rectangle3.maximum() + (this->m_vector2_1 * static_cast<TypeParam>(0.5)));

	EXPECT_TRUE(bounding_rectangle3.collision(bounding_rectangle1) == ror::CollisionType::outside);
	EXPECT_TRUE(bounding_rectangle3.collision(bounding_rectangle2) == ror::CollisionType::intersects);
	EXPECT_TRUE(bounding_rectangle3.collision(bounding_rectangle3) == ror::CollisionType::inside);
	EXPECT_TRUE(bounding_rectangle3.collision(bounding_rectangle4) == ror::CollisionType::intersects);
}

TYPED_TEST(BoundingTest, BoundingRectangle_create_from_points)
{
	std::vector<ror::Vector2<TypeParam>> points;
	points.reserve(4);

	points.push_back(this->m_vector2_1);
	points.push_back(this->m_vector2_2);
	points.push_back(this->m_vector2_3);
	points.push_back(this->m_vector2_4);

	this->m_bounding_rectangle.create_from_points(points);

	EXPECT_TRUE(this->m_bounding_rectangle.is_point_inside(this->m_vector2_1) == true);
	EXPECT_TRUE(this->m_bounding_rectangle.is_point_inside(this->m_vector2_2) == true);
	EXPECT_TRUE(this->m_bounding_rectangle.is_point_inside(this->m_vector2_3) == true);
	EXPECT_TRUE(this->m_bounding_rectangle.is_point_inside(this->m_vector2_4) == true);
}

TYPED_TEST(BoundingTest, BoundingRectangle_system_test)
{
	// These tests are only valid for decimal types
	if constexpr (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle0;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_small1;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_small2;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_small3;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_small4;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_small5;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_same1;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_same2;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_same3;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_same4;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_big;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_small_inside_big;
		ror::Box<ror::Vector2<TypeParam>> bounding_rectangle_huge;

		bounding_rectangle0.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-1.0f)),
								ror::Vector2<TypeParam>(static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f)));
		bounding_rectangle_big.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(3.3f), static_cast<TypeParam>(2.3f)),
								   ror::Vector2<TypeParam>(static_cast<TypeParam>(9.3f), static_cast<TypeParam>(8.3f)));
		bounding_rectangle_huge.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-5.0f), static_cast<TypeParam>(-5.0f)),
									ror::Vector2<TypeParam>(static_cast<TypeParam>(10.0f), static_cast<TypeParam>(10.0f)));
		bounding_rectangle_same1.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(-3.0f)),
									 ror::Vector2<TypeParam>(static_cast<TypeParam>(4.0f), static_cast<TypeParam>(-0.5f)));
		bounding_rectangle_same2.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(0.7f), static_cast<TypeParam>(1.5f)),
									 ror::Vector2<TypeParam>(static_cast<TypeParam>(3.2f), static_cast<TypeParam>(4.0f)));
		bounding_rectangle_same3.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-3.0f), static_cast<TypeParam>(1.5f)),
									 ror::Vector2<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.0f)));
		bounding_rectangle_same4.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-4.5f), static_cast<TypeParam>(-3.5f)),
									 ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.0f), static_cast<TypeParam>(-1.0f)));
		bounding_rectangle_small1.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(3.5f), static_cast<TypeParam>(1.8f)),
									  ror::Vector2<TypeParam>(static_cast<TypeParam>(3.9f), static_cast<TypeParam>(2.2f)));
		bounding_rectangle_small2.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(4.5f)),
									  ror::Vector2<TypeParam>(static_cast<TypeParam>(1.9f), static_cast<TypeParam>(4.9f)));
		bounding_rectangle_small3.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.5f)),
									  ror::Vector2<TypeParam>(static_cast<TypeParam>(-0.1f), static_cast<TypeParam>(4.9f)));
		bounding_rectangle_small4.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.5f), static_cast<TypeParam>(-0.5f)),
									  ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-0.1f)));
		bounding_rectangle_small5.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-2.5f)),
									  ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-2.1f)));
		bounding_rectangle_small_inside_big.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(6.0f), static_cast<TypeParam>(3.0f)),
												ror::Vector2<TypeParam>(static_cast<TypeParam>(6.4f), static_cast<TypeParam>(3.4f)));

		// Tests in 2-Dimensions
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle0), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small_inside_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_huge), ror::CollisionType::inside);

		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle0), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small1), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small2), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small3), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small4), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small5), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same1), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same2), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same3), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same4), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small_inside_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_huge), ror::CollisionType::intersects);

		bounding_rectangle0.add_bounding(bounding_rectangle_same1);
		bounding_rectangle0.add_bounding(bounding_rectangle_same2);
		bounding_rectangle0.add_bounding(bounding_rectangle_same3);
		bounding_rectangle0.add_bounding(bounding_rectangle_same4);

		bounding_rectangle0.add_bounding(bounding_rectangle_small1);
		bounding_rectangle0.add_bounding(bounding_rectangle_small2);
		bounding_rectangle0.add_bounding(bounding_rectangle_small3);
		bounding_rectangle0.add_bounding(bounding_rectangle_small4);
		bounding_rectangle0.add_bounding(bounding_rectangle_small5);

		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_big), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small_inside_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_huge), ror::CollisionType::intersects);

		bounding_rectangle0.add_bounding(bounding_rectangle_big);
		bounding_rectangle0.add_bounding(bounding_rectangle_small_inside_big);

		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small_inside_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_huge), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle0), ror::CollisionType::inside);

		// Tests in 3 dimensions
		bounding_rectangle0.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-1.0f)),
								ror::Vector2<TypeParam>(static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f)));
		bounding_rectangle_big.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(3.3f), static_cast<TypeParam>(2.3f)),
								   ror::Vector2<TypeParam>(static_cast<TypeParam>(9.3f), static_cast<TypeParam>(8.3f)));
		bounding_rectangle_huge.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-5.0f), static_cast<TypeParam>(-5.0f)),
									ror::Vector2<TypeParam>(static_cast<TypeParam>(10.0f), static_cast<TypeParam>(10.0f)));
		bounding_rectangle_same1.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(-3.0f)),
									 ror::Vector2<TypeParam>(static_cast<TypeParam>(4.0f), static_cast<TypeParam>(-0.5f)));
		bounding_rectangle_same2.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(0.7f), static_cast<TypeParam>(1.5f)),
									 ror::Vector2<TypeParam>(static_cast<TypeParam>(3.2f), static_cast<TypeParam>(4.0f)));
		bounding_rectangle_same3.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-3.0f), static_cast<TypeParam>(1.5f)),
									 ror::Vector2<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.0f)));
		bounding_rectangle_same4.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-4.5f), static_cast<TypeParam>(-3.5f)),
									 ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.0f), static_cast<TypeParam>(-1.0f)));
		bounding_rectangle_small1.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(3.5f), static_cast<TypeParam>(1.8f)),
									  ror::Vector2<TypeParam>(static_cast<TypeParam>(3.9f), static_cast<TypeParam>(2.2f)));
		bounding_rectangle_small2.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(4.5f)),
									  ror::Vector2<TypeParam>(static_cast<TypeParam>(1.9f), static_cast<TypeParam>(4.9f)));
		bounding_rectangle_small3.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.5f)),
									  ror::Vector2<TypeParam>(static_cast<TypeParam>(-0.1f), static_cast<TypeParam>(4.9f)));
		bounding_rectangle_small4.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.5f), static_cast<TypeParam>(-0.5f)),
									  ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-0.1f)));
		bounding_rectangle_small5.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-2.5f)),
									  ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-2.1f)));
		bounding_rectangle_small_inside_big.set(ror::Vector2<TypeParam>(static_cast<TypeParam>(6.0f), static_cast<TypeParam>(3.0f)),
												ror::Vector2<TypeParam>(static_cast<TypeParam>(6.4f), static_cast<TypeParam>(3.4f)));

		// Tests in 2-Dimensions
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle0), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_small_inside_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle_huge), ror::CollisionType::inside);

		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle0), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small1), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small2), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small3), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small4), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small5), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same1), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same2), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same3), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same4), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small_inside_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_huge), ror::CollisionType::intersects);

		bounding_rectangle0.add_bounding(bounding_rectangle_same1);
		bounding_rectangle0.add_bounding(bounding_rectangle_same2);
		bounding_rectangle0.add_bounding(bounding_rectangle_same3);
		bounding_rectangle0.add_bounding(bounding_rectangle_same4);

		bounding_rectangle0.add_bounding(bounding_rectangle_small1);
		bounding_rectangle0.add_bounding(bounding_rectangle_small2);
		bounding_rectangle0.add_bounding(bounding_rectangle_small3);
		bounding_rectangle0.add_bounding(bounding_rectangle_small4);
		bounding_rectangle0.add_bounding(bounding_rectangle_small5);

		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_big), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small_inside_big), ror::CollisionType::outside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_huge), ror::CollisionType::intersects);

		bounding_rectangle0.add_bounding(bounding_rectangle_big);
		bounding_rectangle0.add_bounding(bounding_rectangle_small_inside_big);

		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small5), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same1), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same2), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same3), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_same4), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_small_inside_big), ror::CollisionType::inside);
		EXPECT_EQ(bounding_rectangle0.collision(bounding_rectangle_huge), ror::CollisionType::intersects);
		EXPECT_EQ(bounding_rectangle_huge.collision(bounding_rectangle0), ror::CollisionType::inside);

		// Testing creating from points
		std::vector<ror::Vector2<TypeParam>> points;
		points.reserve(27);

		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-1.0f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(3.5f), static_cast<TypeParam>(1.8f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(4.5f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.5f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.5f), static_cast<TypeParam>(-0.5f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-1.0f), static_cast<TypeParam>(-2.5f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(1.5f), static_cast<TypeParam>(-3.0f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(0.7f), static_cast<TypeParam>(1.5f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-3.0f), static_cast<TypeParam>(1.5f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-4.5f), static_cast<TypeParam>(-3.5f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(3.3f), static_cast<TypeParam>(2.3f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(6.0f), static_cast<TypeParam>(3.0f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-5.0f), static_cast<TypeParam>(-5.0f)));

		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(1.0f), static_cast<TypeParam>(1.0f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(3.9f), static_cast<TypeParam>(2.2f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(1.9f), static_cast<TypeParam>(4.9f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-0.1f), static_cast<TypeParam>(4.9f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-0.1f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.1f), static_cast<TypeParam>(-2.1f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(4.0f), static_cast<TypeParam>(-0.5f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(3.2f), static_cast<TypeParam>(4.0f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-0.5f), static_cast<TypeParam>(4.0f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(-2.0f), static_cast<TypeParam>(-1.0f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(9.3f), static_cast<TypeParam>(8.3f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(6.4f), static_cast<TypeParam>(3.4f)));
		points.push_back(ror::Vector2<TypeParam>(static_cast<TypeParam>(10.0f), static_cast<TypeParam>(10.0f)));

		ror::Box<ror::Vector2<TypeParam>> boundingRectangleFromPoints;
		boundingRectangleFromPoints.create_from_points(points);

		EXPECT_EQ(bounding_rectangle0.collision(boundingRectangleFromPoints), ror::CollisionType::intersects);

		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_small1), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_small2), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_small3), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_small4), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_small5), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_same1), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_same2), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_same3), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_same4), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_big), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_small_inside_big), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle_huge), ror::CollisionType::inside);
		EXPECT_EQ(boundingRectangleFromPoints.collision(bounding_rectangle0), ror::CollisionType::inside);
	}
}

// General mixed collisions of each one of the types with all others
TYPED_TEST(BoundingTest, boundings_mix_collision)
{
	// Collision with other type
#if defined (ROR_COMPILER_TYPE_GCC) && defined (ROR_OS_TYPE_LINUX)
		// Some weird GCC 11 issue only shows up when built in release  like
		// rorvector3.hh:100:76: error: array subscript ‘const ror::Vector3<float>[15]’ is partly outside array bounds of ‘ror::Round<ror::Vector3<float> > [6]’ [-Werror=array-bounds]
		// This check protects against that error but don't want to mask an actual issue on other compilers
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere[20];
#else
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere[6];
#endif
	ror::Round<ror::Vector2<TypeParam>> bounding_circle[4];
	ror::Box<ror::Vector3<TypeParam>>   bounding_box[6];
	ror::Box<ror::Vector2<TypeParam>>   bounding_rectangle[4];

	// Init stuff
	for (auto &bs : bounding_sphere)
	{
		bs.set_radius(10.0f);
	}

	bounding_sphere[0].set_center(this->m_vector3_1 - this->m_vector3_4);
	bounding_sphere[1].set_center(this->m_vector3_1 - this->m_vector3_3);
	bounding_sphere[2].set_center(this->m_vector3_3);
	bounding_sphere[3].set_center(this->m_vector3_4);
	bounding_sphere[4].set_center(ror::Vector3<TypeParam>(this->m_vector2_1 - this->m_vector2_3));
	bounding_sphere[5].set_center(ror::Vector3<TypeParam>(this->m_vector2_4));        // Zero out z

	for (auto &bc : bounding_circle)
	{
		bc.set_radius(12.0f);
	}

	bounding_circle[0].set_center(this->m_vector2_1 - this->m_vector2_4);
	bounding_circle[1].set_center(this->m_vector2_1 - this->m_vector2_3);
	bounding_circle[2].set_center(this->m_vector2_3);
	bounding_circle[3].set_center(this->m_vector2_4);

	// Init boxes
	auto vector3_min = ror::Vector3<TypeParam>(0.0f, 0.0f, 0.0f);
	auto vector3_max = ror::Vector3<TypeParam>(0.0f, 0.0f, 0.0f);

	float extents         = 5.0f;
	auto  vector3_extents = ror::Vector3<TypeParam>(static_cast<TypeParam>(extents), static_cast<TypeParam>(extents), static_cast<TypeParam>(extents));

	bounding_box[0].create_from_min_max((this->m_vector3_1 - this->m_vector3_4) - vector3_extents,
										(this->m_vector3_1 - this->m_vector3_4) + vector3_extents);
	bounding_box[1].create_from_min_max((this->m_vector3_1 - this->m_vector3_3) - vector3_extents * 2.0f,
										(this->m_vector3_1 - this->m_vector3_3) + vector3_extents * 2.0f);
	bounding_box[2].create_from_min_max((this->m_vector3_3) - vector3_extents,
										(this->m_vector3_3) + vector3_extents);
	bounding_box[3].create_from_min_max((this->m_vector3_4) - vector3_extents * 2.0f,
										(this->m_vector3_4) + vector3_extents * 2.0f);
	bounding_box[4].create_from_min_max(ror::Vector3<TypeParam>(this->m_vector2_1 - this->m_vector2_3) - vector3_extents,
										ror::Vector3<TypeParam>(this->m_vector2_1 - this->m_vector2_3) + vector3_extents);
	bounding_box[5].create_from_min_max(ror::Vector3<TypeParam>(this->m_vector2_4) - vector3_extents * 2.0f,
										ror::Vector3<TypeParam>(this->m_vector2_4) + vector3_extents * 2.0f);

	auto vector2_min = ror::Vector2<TypeParam>(0.0f, 0.0f);
	auto vector2_max = ror::Vector2<TypeParam>(0.0f, 0.0f);

	extents              = 5.0f;
	auto vector2_extents = ror::Vector2<TypeParam>(static_cast<TypeParam>(extents), static_cast<TypeParam>(extents));

	bounding_rectangle[0].create_from_min_max((this->m_vector2_1 - this->m_vector2_4) - vector2_extents,
											  ((this->m_vector2_1 - this->m_vector2_4) + vector2_extents));
	bounding_rectangle[1].create_from_min_max((this->m_vector2_1 - this->m_vector2_3) - vector2_extents * 2.0f,
											  ((this->m_vector2_1 - this->m_vector2_3) + vector2_extents * 2.0f));
	bounding_rectangle[2].create_from_min_max((this->m_vector2_3) - vector2_extents,
											  ((this->m_vector2_3) + vector2_extents));
	bounding_rectangle[3].create_from_min_max((this->m_vector2_4) - (vector2_extents + 1.0f) * 2.0f,
											  ((this->m_vector2_4) + (vector2_extents + 1.0f) * 2.0f));

	// Sphere with Sphere
	for (auto bs = 0; bs < 6; bs++)
	{
		for (auto bsi = 0; bsi < 6; bsi++)
		{
			ror::Bounding &bounding = bounding_sphere[bsi];
			if ((bs == 0 && bsi == 1) || (bs == 1 && bsi == 0) || (bs == 2 && bsi == 3) || (bs == 3 && bsi == 2) || (bs == 1 && bsi == 4) || (bs == 4 && bsi == 1))
			{
				EXPECT_TRUE(bounding_sphere[bs].collision(bounding_sphere[bsi]) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_sphere[bs].collision(bounding) == ror::CollisionType::intersects);
			}
			else if (bs == bsi)
			{
				EXPECT_TRUE(bounding_sphere[bs].collision(bounding_sphere[bsi]) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_sphere[bs].collision(bounding) == ror::CollisionType::inside);
			}
			else
			{
				EXPECT_TRUE(bounding_sphere[bs].collision(bounding_sphere[bsi]) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_sphere[bs].collision(bounding) == ror::CollisionType::outside);
			}
		}
	}

	for (auto bs = 0; bs < 6; bs++)
	{
		for (auto bsi = 0; bsi < 6; bsi++)
		{
			ror::Bounding &bounding = bounding_sphere[bsi];
			if ((bs == 0 && bsi == 1) || (bs == 1 && bsi == 0) || (bs == 2 && bsi == 3) || (bs == 3 && bsi == 2) || (bs == 1 && bsi == 4) || (bs == 4 && bsi == 1) || (bs == bsi))
			{
				EXPECT_TRUE(bounding_sphere[bs].intersects(bounding_sphere[bsi]));
				EXPECT_TRUE(bounding_sphere[bs].intersects(bounding));
			}
			else
			{
				EXPECT_FALSE(bounding_sphere[bs].intersects(bounding_sphere[bsi]));
				EXPECT_FALSE(bounding_sphere[bs].intersects(bounding));
			}
		}
	}

	// Circle with Circle
	for (auto bc = 0; bc < 4; bc++)
	{
		for (auto bci = 0; bci < 4; bci++)
		{
			ror::Bounding &bounding = bounding_circle[bci];
			if ((bc == 0 && bci == 1) || (bc == 1 && bci == 0) || (bc == 2 && bci == 3) || (bc == 3 && bci == 2))
			{
				EXPECT_TRUE(bounding_circle[bc].collision(bounding_circle[bci]) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_circle[bc].collision(bounding) == ror::CollisionType::intersects);
			}
			else if (bc == bci)
			{
				EXPECT_TRUE(bounding_circle[bc].collision(bounding_circle[bci]) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_circle[bc].collision(bounding) == ror::CollisionType::inside);
			}
			else
			{
				EXPECT_TRUE(bounding_circle[bc].collision(bounding_circle[bci]) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_circle[bc].collision(bounding) == ror::CollisionType::outside);
			}
		}
	}

	for (auto bc = 0; bc < 4; bc++)
	{
		for (auto bci = 0; bci < 4; bci++)
		{
			ror::Bounding &bounding = bounding_circle[bci];
			if ((bc == 0 && bci == 1) || (bc == 1 && bci == 0) || (bc == 2 && bci == 3) || (bc == 3 && bci == 2) || (bc == bci))
			{
				EXPECT_TRUE(bounding_circle[bc].intersects(bounding_circle[bci]));
				EXPECT_TRUE(bounding_circle[bc].intersects(bounding));
			}
			else
			{
				EXPECT_FALSE(bounding_circle[bc].intersects(bounding_circle[bci]));
				EXPECT_FALSE(bounding_circle[bc].intersects(bounding));
			}
		}
	}

	// Box with Box
	for (auto bx = 0; bx < 6; bx++)
	{
		for (auto bxi = 0; bxi < 6; bxi++)
		{
			ror::Bounding &bounding = bounding_box[bxi];
			if ((bx == 0 && bxi == 1) || (bx == 1 && bxi == 0) || (bx == 2 && bxi == 3) || (bx == 3 && bxi == 2))
			{
				EXPECT_TRUE(bounding_box[bx].collision(bounding_box[bxi]) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_box[bx].collision(bounding) == ror::CollisionType::intersects);
			}
			else if (bx == bxi)
			{
				EXPECT_TRUE(bounding_box[bx].collision(bounding_box[bxi]) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_box[bx].collision(bounding) == ror::CollisionType::inside);
			}
			else
			{
				EXPECT_TRUE(bounding_box[bx].collision(bounding_box[bxi]) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_box[bx].collision(bounding) == ror::CollisionType::outside);
			}
		}
	}

	for (auto bx = 0; bx < 6; bx++)
	{
		for (auto bxi = 0; bxi < 6; bxi++)
		{
			ror::Bounding &bounding = bounding_box[bxi];
			if ((bx == 0 && bxi == 1) || (bx == 1 && bxi == 0) || (bx == 2 && bxi == 3) || (bx == 3 && bxi == 2) || (bx == bxi))
			{
				EXPECT_TRUE(bounding_box[bx].intersects(bounding_box[bxi]));
				EXPECT_TRUE(bounding_box[bx].intersects(bounding));
			}
			else
			{
				EXPECT_FALSE(bounding_box[bx].intersects(bounding_box[bxi]));
				EXPECT_FALSE(bounding_box[bx].intersects(bounding));
			}
		}
	}

	// Rectangle with Rectangle
	for (auto br = 0; br < 4; br++)
	{
		for (auto bri = 0; bri < 4; bri++)
		{
			ror::Bounding &bounding = bounding_rectangle[bri];
			if ((br == 0 && bri == 1) || (br == 1 && bri == 0) || (br == 2 && bri == 3) || (br == 3 && bri == 2))
			{
				EXPECT_TRUE(bounding_rectangle[br].collision(bounding_rectangle[bri]) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_rectangle[br].collision(bounding) == ror::CollisionType::intersects);
			}
			else if (br == bri)
			{
				EXPECT_TRUE(bounding_rectangle[br].collision(bounding_rectangle[bri]) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_rectangle[br].collision(bounding) == ror::CollisionType::inside);
			}
			else
			{
				EXPECT_TRUE(bounding_rectangle[br].collision(bounding_rectangle[bri]) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_rectangle[br].collision(bounding) == ror::CollisionType::outside);
			}
		}
	}

	for (auto br = 0; br < 4; br++)
	{
		for (auto bri = 0; bri < 4; bri++)
		{
			ror::Bounding &bounding = bounding_rectangle[bri];
			if ((br == 0 && bri == 1) || (br == 1 && bri == 0) || (br == 2 && bri == 3) || (br == 3 && bri == 2) || (br == bri))
			{
				EXPECT_TRUE(bounding_rectangle[br].intersects(bounding_rectangle[bri]));
				EXPECT_TRUE(bounding_rectangle[br].intersects(bounding));
			}
			else
			{
				EXPECT_FALSE(bounding_rectangle[br].intersects(bounding_rectangle[bri]));
				EXPECT_FALSE(bounding_rectangle[br].intersects(bounding));
			}
		}
	}

	// sphere->circle and circle->sphere
	for (auto bs = 0; bs < 4; bs++)
	{
		for (auto bc = 0; bc < 4; bc++)
		{
			EXPECT_TRUE(bounding_sphere[bs].collision(bounding_circle[bc]) == ror::CollisionType::outside);
			EXPECT_TRUE(bounding_circle[bc].collision(bounding_sphere[bs]) == ror::CollisionType::outside);
			EXPECT_FALSE(bounding_sphere[bs].intersects(bounding_circle[bc]));
			EXPECT_FALSE(bounding_circle[bc].intersects(bounding_sphere[bs]));

			ror::Bounding &boundings = bounding_sphere[bs];
			ror::Bounding &boundingc = bounding_circle[bc];

			EXPECT_TRUE(bounding_sphere[bs].collision(boundingc) == ror::CollisionType::outside);
			EXPECT_TRUE(bounding_circle[bc].collision(boundings) == ror::CollisionType::outside);
			EXPECT_FALSE(bounding_sphere[bs].intersects(boundingc));
			EXPECT_FALSE(bounding_circle[bc].intersects(boundings));
		}
	}

	for (auto bc = 0; bc < 4; bc++)
	{
		ror::Bounding &boundingc = bounding_circle[bc];

		if (bc < 2)
		{
			EXPECT_TRUE(bounding_sphere[4].collision(bounding_circle[bc]) == ror::CollisionType::intersects);
			EXPECT_TRUE(bounding_sphere[4].intersects(bounding_circle[bc]));

			EXPECT_TRUE(bounding_sphere[4].collision(boundingc) == ror::CollisionType::intersects);
			EXPECT_TRUE(bounding_sphere[4].intersects(boundingc));
		}
		else
		{
			EXPECT_TRUE(bounding_sphere[4].collision(bounding_circle[bc]) == ror::CollisionType::outside);
			EXPECT_FALSE(bounding_sphere[4].intersects(bounding_circle[bc]));

			EXPECT_TRUE(bounding_sphere[4].collision(boundingc) == ror::CollisionType::outside);
			EXPECT_FALSE(bounding_sphere[4].intersects(boundingc));
		}
	}

	for (auto bc = 0; bc < 4; bc++)
	{
		ror::Bounding &boundingc = bounding_circle[bc];

		if (bc > 1)
		{
			EXPECT_TRUE(bounding_sphere[5].collision(bounding_circle[bc]) == ror::CollisionType::intersects);
			EXPECT_TRUE(bounding_sphere[5].intersects(bounding_circle[bc]));

			EXPECT_TRUE(bounding_sphere[5].collision(boundingc) == ror::CollisionType::intersects);
			EXPECT_TRUE(bounding_sphere[5].intersects(boundingc));
		}
		else
		{
			EXPECT_TRUE(bounding_sphere[5].collision(bounding_circle[bc]) == ror::CollisionType::outside);
			EXPECT_FALSE(bounding_sphere[5].intersects(bounding_circle[bc]));

			EXPECT_TRUE(bounding_sphere[5].collision(boundingc) == ror::CollisionType::outside);
			EXPECT_FALSE(bounding_sphere[5].intersects(boundingc));
		}
	}

	for (auto bc = 0; bc < 4; bc++)
	{
		ror::Bounding &boundings = bounding_sphere[4];

		if (bc == 0)
		{
			EXPECT_TRUE(bounding_circle[bc].collision(bounding_sphere[4]) == ror::CollisionType::intersects);
			EXPECT_TRUE(bounding_circle[bc].intersects(bounding_sphere[4]));

			EXPECT_TRUE(bounding_circle[bc].collision(boundings) == ror::CollisionType::intersects);
			EXPECT_TRUE(bounding_circle[bc].intersects(boundings));
		}
		else if (bc == 1)
		{
			EXPECT_TRUE(bounding_circle[bc].collision(bounding_sphere[4]) == ror::CollisionType::inside);
			EXPECT_TRUE(bounding_circle[bc].intersects(bounding_sphere[4]));

			EXPECT_TRUE(bounding_circle[bc].collision(boundings) == ror::CollisionType::inside);
			EXPECT_TRUE(bounding_circle[bc].intersects(boundings));
		}
		else
		{
			EXPECT_TRUE(bounding_circle[bc].collision(bounding_sphere[4]) == ror::CollisionType::outside);
			EXPECT_FALSE(bounding_circle[bc].intersects(bounding_sphere[4]));

			EXPECT_TRUE(bounding_circle[bc].collision(boundings) == ror::CollisionType::outside);
			EXPECT_FALSE(bounding_circle[bc].intersects(boundings));
		}
	}

	for (auto bc = 0; bc < 4; bc++)
	{
		ror::Bounding &boundings = bounding_sphere[5];

		if (bc == 2)
		{
			EXPECT_TRUE(bounding_circle[bc].collision(bounding_sphere[5]) == ror::CollisionType::intersects);
			EXPECT_TRUE(bounding_circle[bc].intersects(bounding_sphere[5]));

			EXPECT_TRUE(bounding_circle[bc].collision(boundings) == ror::CollisionType::intersects);
			EXPECT_TRUE(bounding_circle[bc].intersects(boundings));
		}
		else if (bc == 3)
		{
			EXPECT_TRUE(bounding_circle[bc].collision(bounding_sphere[5]) == ror::CollisionType::inside);
			EXPECT_TRUE(bounding_circle[bc].intersects(bounding_sphere[5]));

			EXPECT_TRUE(bounding_circle[bc].collision(boundings) == ror::CollisionType::inside);
			EXPECT_TRUE(bounding_circle[bc].intersects(boundings));
		}
		else
		{
			EXPECT_TRUE(bounding_circle[bc].collision(bounding_sphere[5]) == ror::CollisionType::outside);
			EXPECT_FALSE(bounding_circle[bc].intersects(bounding_sphere[5]));

			EXPECT_TRUE(bounding_circle[bc].collision(boundings) == ror::CollisionType::outside);
			EXPECT_FALSE(bounding_circle[bc].intersects(boundings));
		}
	}

	// sphere->box and box->sphere
	for (auto bs = 0; bs < 6; bs++)
	{
		for (auto bx = 0; bx < 6; bx++)
		{
			ror::Bounding &boundings = bounding_sphere[bs];
			ror::Bounding &boundingx = bounding_box[bx];

			if ((bs == 0 && bx == 0) || (bs == 2 && bx == 2) || (bs == 4 && bx == 4))
			{
				EXPECT_TRUE(bounding_sphere[bs].collision(bounding_box[bx]) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_box[bx].collision(bounding_sphere[bs]) == ror::CollisionType::intersects);

				EXPECT_TRUE(bounding_sphere[bs].intersects(bounding_box[bx]));
				EXPECT_TRUE(bounding_box[bx].intersects(bounding_sphere[bs]));

				EXPECT_TRUE(bounding_sphere[bs].collision(boundingx) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_box[bx].collision(boundings) == ror::CollisionType::intersects);

				EXPECT_TRUE(bounding_sphere[bs].intersects(boundingx));
				EXPECT_TRUE(bounding_box[bx].intersects(boundings));
			}
			else if ((bs == 0 && bx == 1) || (bs == 2 && bx == 3) ||
					 (bs == 1 && bx == 0) || (bs == 3 && bx == 2) ||
					 (bs == 1 && bx == 1) || (bs == 3 && bx == 3) || (bs == 5 && bx == 5))
			{
				EXPECT_TRUE(bounding_sphere[bs].collision(bounding_box[bx]) == ror::CollisionType::intersects);

				EXPECT_TRUE(bounding_sphere[bs].collision(boundingx) == ror::CollisionType::intersects);

				if ((bs == 1 && bx == 1) || (bs == 3 && bx == 3) || (bs == 5 && bx == 5))
				{
					EXPECT_TRUE(bounding_box[bx].collision(bounding_sphere[bs]) == ror::CollisionType::inside);
					EXPECT_TRUE(bounding_box[bx].collision(boundings) == ror::CollisionType::inside);
				}
				else
				{
					EXPECT_TRUE(bounding_box[bx].collision(bounding_sphere[bs]) == ror::CollisionType::intersects);
					EXPECT_TRUE(bounding_box[bx].collision(boundings) == ror::CollisionType::intersects);
				}

				EXPECT_TRUE(bounding_sphere[bs].intersects(bounding_box[bx]));
				EXPECT_TRUE(bounding_box[bx].intersects(bounding_sphere[bs]));

				EXPECT_TRUE(bounding_sphere[bs].intersects(boundingx));
				EXPECT_TRUE(bounding_box[bx].intersects(boundings));
			}
			else
			{
				EXPECT_TRUE(bounding_sphere[bs].collision(bounding_box[bx]) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_box[bx].collision(bounding_sphere[bs]) == ror::CollisionType::outside);

				EXPECT_FALSE(bounding_sphere[bs].intersects(bounding_box[bx]));
				EXPECT_FALSE(bounding_box[bx].intersects(bounding_sphere[bs]));

				EXPECT_TRUE(bounding_sphere[bs].collision(boundingx) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_box[bx].collision(boundings) == ror::CollisionType::outside);

				EXPECT_FALSE(bounding_sphere[bs].intersects(boundingx));
				EXPECT_FALSE(bounding_box[bx].intersects(boundings));
			}
		}
	}

	// sphere->rectangle and rectangle->sphere
	for (auto bs = 0; bs < 6; bs++)
	{
		for (auto br = 0; br < 4; br++)
		{
			ror::Bounding &boundings = bounding_sphere[bs];
			ror::Bounding &boundingr = bounding_rectangle[br];

			if (((br == 0 || br == 1) && bs == 4) || ((br == 2 || br == 3) && bs == 5))
			{
				EXPECT_TRUE(bounding_sphere[bs].collision(bounding_rectangle[br]) == ror::CollisionType::intersects);

				EXPECT_TRUE(bounding_sphere[bs].collision(boundingr) == ror::CollisionType::intersects);

				if ((br == 1 && bs == 4) || (br == 3 && bs == 5))
				{
					EXPECT_TRUE(bounding_rectangle[br].collision(bounding_sphere[bs]) == ror::CollisionType::inside);

					EXPECT_TRUE(bounding_rectangle[br].collision(boundings) == ror::CollisionType::inside);
				}
				else
				{
					EXPECT_TRUE(bounding_rectangle[br].collision(bounding_sphere[bs]) == ror::CollisionType::intersects);

					EXPECT_TRUE(bounding_rectangle[br].collision(boundings) == ror::CollisionType::intersects);
				}

				EXPECT_TRUE(bounding_sphere[bs].intersects(bounding_rectangle[br]));
				EXPECT_TRUE(bounding_rectangle[br].intersects(bounding_sphere[bs]));

				EXPECT_TRUE(bounding_sphere[bs].intersects(boundingr));
				EXPECT_TRUE(bounding_rectangle[br].intersects(boundings));
			}
			else
			{
				EXPECT_TRUE(bounding_sphere[bs].collision(bounding_rectangle[br]) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_rectangle[br].collision(bounding_sphere[bs]) == ror::CollisionType::outside);

				EXPECT_FALSE(bounding_sphere[bs].intersects(bounding_rectangle[br]));
				EXPECT_FALSE(bounding_rectangle[br].intersects(bounding_sphere[bs]));

				EXPECT_TRUE(bounding_sphere[bs].collision(bounding_rectangle[br]) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_rectangle[br].collision(bounding_sphere[bs]) == ror::CollisionType::outside);

				EXPECT_FALSE(bounding_sphere[bs].intersects(bounding_rectangle[br]));
				EXPECT_FALSE(bounding_rectangle[br].intersects(bounding_sphere[bs]));

				EXPECT_TRUE(bounding_sphere[bs].collision(boundingr) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_rectangle[br].collision(boundings) == ror::CollisionType::outside);

				EXPECT_FALSE(bounding_sphere[bs].intersects(boundingr));
				EXPECT_FALSE(bounding_rectangle[br].intersects(boundings));

				EXPECT_TRUE(bounding_sphere[bs].collision(boundingr) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_rectangle[br].collision(boundings) == ror::CollisionType::outside);

				EXPECT_FALSE(bounding_sphere[bs].intersects(boundingr));
				EXPECT_FALSE(bounding_rectangle[br].intersects(boundings));
			}
		}
	}

	// Circle->box and box->circle
	for (auto bc = 0; bc < 4; bc++)
	{
		for (auto bx = 0; bx < 6; bx++)
		{
			ror::Bounding &boundingc = bounding_circle[bc];
			ror::Bounding &boundingx = bounding_box[bx];

			if ((bc == 0 && bx == 4) || ((bc == 2 || bc == 3) && bx == 5))
			{
				EXPECT_TRUE(bounding_circle[bc].collision(bounding_box[bx]) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_box[bx].collision(bounding_circle[bc]) == ror::CollisionType::intersects);

				EXPECT_TRUE(bounding_circle[bc].intersects(bounding_box[bx]));
				EXPECT_TRUE(bounding_box[bx].intersects(bounding_circle[bc]));

				EXPECT_TRUE(bounding_circle[bc].collision(boundingx) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_box[bx].collision(boundingc) == ror::CollisionType::intersects);

				EXPECT_TRUE(bounding_circle[bc].intersects(boundingx));
				EXPECT_TRUE(bounding_box[bx].intersects(boundingc));
			}
			else if (bc == 1 && bx == 4)
			{
				EXPECT_TRUE(bounding_circle[bc].collision(bounding_box[bx]) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_box[bx].collision(bounding_circle[bc]) == ror::CollisionType::intersects);

				EXPECT_TRUE(bounding_circle[bc].intersects(bounding_box[bx]));
				EXPECT_TRUE(bounding_box[bx].intersects(bounding_circle[bc]));

				EXPECT_TRUE(bounding_circle[bc].collision(boundingx) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_box[bx].collision(boundingc) == ror::CollisionType::intersects);

				EXPECT_TRUE(bounding_circle[bc].intersects(boundingx));
				EXPECT_TRUE(bounding_box[bx].intersects(boundingc));
			}
			else
			{
				EXPECT_TRUE(bounding_circle[bc].collision(bounding_box[bx]) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_box[bx].collision(bounding_circle[bc]) == ror::CollisionType::outside);

				EXPECT_FALSE(bounding_circle[bc].intersects(bounding_box[bx]));
				EXPECT_FALSE(bounding_box[bx].intersects(bounding_circle[bc]));

				EXPECT_TRUE(bounding_circle[bc].collision(boundingx) == ror::CollisionType::outside);
				EXPECT_TRUE(bounding_box[bx].collision(boundingc) == ror::CollisionType::outside);

				EXPECT_FALSE(bounding_circle[bc].intersects(boundingx));
				EXPECT_FALSE(bounding_box[bx].intersects(boundingc));
			}
		}
	}

	// Circle->rectangle and rectangle->circle
	for (auto bc = 0; bc < 4; bc++)
	{
		for (auto br = 0; br < 4; br++)
		{
			ror::Bounding &boundingr = bounding_rectangle[br];

			if ((bc == 0 && br == 0) || (bc == 2 && br == 2))
			{
				EXPECT_TRUE(bounding_circle[bc].collision(bounding_rectangle[br]) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_circle[bc].intersects(bounding_rectangle[br]));

				EXPECT_TRUE(bounding_circle[bc].collision(boundingr) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_circle[bc].intersects(boundingr));
			}
			else if ((bc == 0 && br == 1) || (bc == 1 && br == 0) ||
					 (bc == 1 && br == 1) || (bc == 3 && br == 3) ||
					 (bc == 2 && br == 3) || (bc == 3 && br == 2))
			{
				EXPECT_TRUE(bounding_circle[bc].collision(bounding_rectangle[br]) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_circle[bc].intersects(bounding_rectangle[br]));

				EXPECT_TRUE(bounding_circle[bc].collision(boundingr) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_circle[bc].intersects(boundingr));
			}
			else
			{
				EXPECT_TRUE(bounding_circle[bc].collision(bounding_rectangle[br]) == ror::CollisionType::outside);
				EXPECT_FALSE(bounding_circle[bc].intersects(bounding_rectangle[br]));

				EXPECT_TRUE(bounding_circle[bc].collision(boundingr) == ror::CollisionType::outside);
				EXPECT_FALSE(bounding_circle[bc].intersects(boundingr));
			}
		}
	}

	for (auto bc = 0; bc < 4; bc++)
	{
		for (auto br = 0; br < 4; br++)
		{
			ror::Bounding &boundingc = bounding_circle[bc];

			if ((br == 0 && bc == 0) || (br == 2 && bc == 2) ||
				(br == 0 && bc == 1) || (br == 2 && bc == 3) ||
				(br == 1 && bc == 0) || (br == 3 && bc == 2) ||
				(br == 1 && bc == 1))
			{
				EXPECT_TRUE(bounding_rectangle[br].collision(bounding_circle[bc]) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_rectangle[br].intersects(bounding_circle[bc]));

				EXPECT_TRUE(bounding_rectangle[br].collision(boundingc) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_rectangle[br].intersects(boundingc));
			}
			else if (br == 3 && bc == 3)
			{
				EXPECT_TRUE(bounding_rectangle[br].collision(bounding_circle[bc]) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_rectangle[br].intersects(bounding_circle[bc]));

				EXPECT_TRUE(bounding_rectangle[br].collision(boundingc) == ror::CollisionType::inside);
				EXPECT_TRUE(bounding_rectangle[br].intersects(boundingc));
			}
			else
			{
				EXPECT_TRUE(bounding_rectangle[br].collision(bounding_circle[bc]) == ror::CollisionType::outside);
				EXPECT_FALSE(bounding_rectangle[br].intersects(bounding_circle[bc]));

				EXPECT_TRUE(bounding_rectangle[br].collision(boundingc) == ror::CollisionType::outside);
				EXPECT_FALSE(bounding_rectangle[br].intersects(boundingc));
			}
		}
	}

	// Box->rectangle and rectangle->box
	for (auto bx = 0; bx < 6; bx++)
	{
		for (auto br = 0; br < 4; br++)
		{
			ror::Bounding &boundingr = bounding_rectangle[br];
			ror::Bounding &boundingx = bounding_box[bx];

			if ((bx == 4 && br == 0) || (bx == 4 && br == 1) ||
				(bx == 5 && br == 2) || (bx == 5 && br == 3))
			{
				// auto c = bounding_box[bx].collision(bounding_rectangle[br]);
				// printf("br=%d, bx=%d, c=%d\n", br, bx, c);

				EXPECT_TRUE(bounding_box[bx].collision(bounding_rectangle[br]) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_box[bx].intersects(bounding_rectangle[br]));

				EXPECT_TRUE(bounding_box[bx].collision(boundingr) == ror::CollisionType::intersects);
				EXPECT_TRUE(bounding_box[bx].intersects(boundingr));

				if ((bx == 4 && br == 1) || (bx == 5 && br == 3))
				{
					EXPECT_TRUE(bounding_rectangle[br].collision(bounding_box[bx]) == ror::CollisionType::inside);
					EXPECT_TRUE(bounding_rectangle[br].collision(boundingx) == ror::CollisionType::inside);
				}
				else
				{
					EXPECT_TRUE(bounding_rectangle[br].collision(bounding_box[bx]) == ror::CollisionType::intersects);
					EXPECT_TRUE(bounding_rectangle[br].collision(boundingx) == ror::CollisionType::intersects);
				}

				EXPECT_TRUE(bounding_rectangle[br].intersects(bounding_box[bx]));
				EXPECT_TRUE(bounding_rectangle[br].intersects(boundingx));
			}
			else
			{
				EXPECT_TRUE(bounding_box[bx].collision(bounding_rectangle[br]) == ror::CollisionType::outside);
				EXPECT_FALSE(bounding_box[bx].intersects(bounding_rectangle[br]));

				EXPECT_TRUE(bounding_box[bx].collision(boundingr) == ror::CollisionType::outside);
				EXPECT_FALSE(bounding_box[bx].intersects(boundingr));

				EXPECT_TRUE(bounding_rectangle[br].collision(bounding_box[bx]) == ror::CollisionType::outside);
				EXPECT_FALSE(bounding_rectangle[br].intersects(bounding_box[bx]));

				EXPECT_TRUE(bounding_rectangle[br].collision(boundingx) == ror::CollisionType::outside);
				EXPECT_FALSE(bounding_rectangle[br].intersects(boundingx));
			}
		}
	}

	// One final inside check for box->rectangle
	bounding_box[5].set(ror::Vector3<TypeParam>(28, 28, -10), ror::Vector3<TypeParam>(52, 52, 10));
	EXPECT_TRUE(bounding_box[5].collision(bounding_rectangle[3]) == ror::CollisionType::inside);

	bounding_box[5].set(ror::Vector3<TypeParam>(26, 26, -10), ror::Vector3<TypeParam>(54, 54, 10));
	EXPECT_TRUE(bounding_box[5].collision(bounding_rectangle[3]) == ror::CollisionType::inside);

	bounding_box[5].set(ror::Vector3<TypeParam>(32, 32, -10), ror::Vector3<TypeParam>(48, 48, 10));
	EXPECT_TRUE(bounding_box[5].collision(bounding_rectangle[3]) == ror::CollisionType::intersects);
}

}        // namespace ror_test
