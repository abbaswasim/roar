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
#include <iostream>
#include <type_traits>

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
	ASSERT_TRUE((bounding_sphere1.radius()) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_sphere1.center() == ror::Vector3<TypeParam>(0.0f, 0.0f, 0.0f));
	ASSERT_TRUE(bounding_sphere1.m_type == ror::BoundingType::bounding_type_3d_sphere);

	ASSERT_TRUE((bounding_sphere1.radius() - this->m_bounding_sphere.radius()) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_sphere1.center() == this->m_bounding_sphere.center());
	ASSERT_TRUE(bounding_sphere1.m_type == this->m_bounding_sphere.m_type);

	ror::Round<ror::Vector3<TypeParam>> bounding_sphere2(bounding_sphere1);
	ASSERT_TRUE((bounding_sphere1.radius() - bounding_sphere2.radius()) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_sphere1.center() == bounding_sphere2.center());
	ASSERT_TRUE(bounding_sphere1.m_type == bounding_sphere2.m_type);
}

TYPED_TEST(BoundingTest, BoundingSphere_accessors_mutators)
{
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere1;
	ror::Vector3<TypeParam>             center(static_cast<TypeParam>(2.5f), static_cast<TypeParam>(3.0f), static_cast<TypeParam>(1.2f));

	bounding_sphere1.set_radius(static_cast<TypeParam>(5.0f));
	bounding_sphere1.set_center(center);

	ASSERT_TRUE((bounding_sphere1.radius() - static_cast<TypeParam>(5.0f)) < ror::ror_epsilon_squared);
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
	ASSERT_TRUE(abs(bounding_sphere1.radius() - nextRadius) < ror::ror_epsilon_squared);

	nextRadius = bounding_sphere1.radius();

	bounding_sphere1.add_point(this->m_vector3_2);
	ASSERT_TRUE(abs(bounding_sphere1.radius() - nextRadius) < ror::ror_epsilon_squared);

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
	EXPECT_TRUE(abs(this->m_bounding_sphere.radius() - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_sphere.add_point(pointypositive);
	EXPECT_TRUE(abs(this->m_bounding_sphere.radius() - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_sphere.add_point(pointynegative);
	EXPECT_TRUE(abs(this->m_bounding_sphere.radius() - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_sphere.add_point(pointzpositive);
	EXPECT_TRUE(abs(this->m_bounding_sphere.radius() - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_sphere.add_point(pointznegative);
	EXPECT_TRUE(abs(this->m_bounding_sphere.radius() - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_sphere.add_point(pointallpositive);
	EXPECT_GE(this->m_bounding_sphere.radius(), 24.0f);

	this->m_bounding_sphere.add_point(pointallnegative);
	EXPECT_GE(this->m_bounding_sphere.radius(), 37.0f);

	ror::Round<ror::Vector3<TypeParam>> bounding_sphere2;
	ror::Vector3<TypeParam>             pointSameCenter1(20.0f, 0.0f, 0.0f);
	ror::Vector3<TypeParam>             pointSameCenter2(-20.0f, 0.0f, 0.0f);

	bounding_sphere2.set_radius(10.0f);
	bounding_sphere2.set_center(point1);

	bounding_sphere2.add_point(pointSameCenter1);
	EXPECT_FALSE(bounding_sphere2.center() == point1);

	bounding_sphere2.add_point(pointSameCenter2);
	EXPECT_TRUE(abs(bounding_sphere2.radius() - 20.0f) < ror::ror_epsilon);
	EXPECT_TRUE(bounding_sphere2.center() == point1);
}

TYPED_TEST(BoundingTest, BoundingSphere_is_point_inside)
{
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere1;
	ror::Round<ror::Vector3<TypeParam>> bounding_sphere2;

	ASSERT_TRUE(abs(this->m_bounding_sphere.radius() - 0.0f) < ror::ror_epsilon_squared);
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
		EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_2) == true);
	EXPECT_FALSE(bounding_sphere1.is_point_inside(this->m_vector3_3) == true);
	EXPECT_FALSE(bounding_sphere1.is_point_inside(this->m_vector3_4) == true);

	bounding_sphere1.add_bounding(bounding_sphere3);
	EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_2) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
		EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_3) == true);
	EXPECT_FALSE(bounding_sphere1.is_point_inside(this->m_vector3_4) == true);

	bounding_sphere1.add_bounding(bounding_sphere4);
	EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
		EXPECT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3_4) == true);

	bounding_sphere2.add_bounding(bounding_sphere3);
	EXPECT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_2) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
		EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_3) == true);
	EXPECT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3_4) == true);

	bounding_sphere2.add_bounding(bounding_sphere4);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
		EXPECT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
		EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_4) == true);

	bounding_sphere3.add_bounding(bounding_sphere4);
	EXPECT_FALSE(bounding_sphere3.is_point_inside(this->m_vector3_1) == true);
	EXPECT_FALSE(bounding_sphere3.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_sphere3.is_point_inside(this->m_vector3_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
		EXPECT_TRUE(bounding_sphere3.is_point_inside(this->m_vector3_4) == true);

	bounding_sphere2.add_bounding(bounding_sphere4);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
		EXPECT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3_1) == true);
	EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_2) == true);
	EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_3) == true);
	if (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
		EXPECT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3_4) == true);
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

	EXPECT_TRUE(bounding_sphere1.collision(bounding_sphere1) == ror::BoundingCollisionType::bounding_collision_type_inside);
	EXPECT_TRUE(bounding_sphere1.collision(bounding_sphere2) == ror::BoundingCollisionType::bounding_collision_type_intersects);
	EXPECT_FALSE(bounding_sphere1.collision(bounding_sphere3) == ror::BoundingCollisionType::bounding_collision_type_intersects);
	EXPECT_FALSE(bounding_sphere1.collision(bounding_sphere4) == ror::BoundingCollisionType::bounding_collision_type_intersects);

	EXPECT_TRUE(bounding_sphere2.collision(bounding_sphere1) == ror::BoundingCollisionType::bounding_collision_type_intersects);
	EXPECT_TRUE(bounding_sphere2.collision(bounding_sphere2) == ror::BoundingCollisionType::bounding_collision_type_inside);
	EXPECT_TRUE(bounding_sphere2.collision(bounding_sphere3) == ror::BoundingCollisionType::bounding_collision_type_intersects);
	EXPECT_FALSE(bounding_sphere2.collision(bounding_sphere4) == ror::BoundingCollisionType::bounding_collision_type_intersects);

	EXPECT_FALSE(bounding_sphere3.collision(bounding_sphere1) == ror::BoundingCollisionType::bounding_collision_type_intersects);
	EXPECT_TRUE(bounding_sphere3.collision(bounding_sphere2) == ror::BoundingCollisionType::bounding_collision_type_intersects);
	EXPECT_TRUE(bounding_sphere3.collision(bounding_sphere3) == ror::BoundingCollisionType::bounding_collision_type_inside);
	EXPECT_TRUE(bounding_sphere3.collision(bounding_sphere4) == ror::BoundingCollisionType::bounding_collision_type_intersects);

	EXPECT_FALSE(bounding_sphere4.collision(bounding_sphere1) == ror::BoundingCollisionType::bounding_collision_type_intersects);
	EXPECT_FALSE(bounding_sphere4.collision(bounding_sphere2) == ror::BoundingCollisionType::bounding_collision_type_intersects);
	EXPECT_TRUE(bounding_sphere4.collision(bounding_sphere3) == ror::BoundingCollisionType::bounding_collision_type_intersects);
	EXPECT_TRUE(bounding_sphere4.collision(bounding_sphere4) == ror::BoundingCollisionType::bounding_collision_type_inside);

	EXPECT_TRUE(bounding_sphere5.collision(bounding_sphere1) == ror::BoundingCollisionType::bounding_collision_type_inside);
	EXPECT_TRUE(bounding_sphere5.collision(bounding_sphere2) == ror::BoundingCollisionType::bounding_collision_type_inside);
	EXPECT_TRUE(bounding_sphere5.collision(bounding_sphere3) == ror::BoundingCollisionType::bounding_collision_type_intersects);
	EXPECT_TRUE(bounding_sphere5.collision(bounding_sphere4) == ror::BoundingCollisionType::bounding_collision_type_intersects);

	bounding_sphere5.set_radius(80.0f);
	EXPECT_TRUE(bounding_sphere5.collision(bounding_sphere4) == ror::BoundingCollisionType::bounding_collision_type_inside);
}

TYPED_TEST(BoundingTest, BoundingSphere_create_from_points)
{
	std::vector<ror::Vector3<TypeParam>> points;
	points.reserve(4);

	points.push_back(this->m_vector3_1);
	points.push_back(this->m_vector3_2);
	points.push_back(this->m_vector3_3);
	points.push_back(this->m_vector3_4);

	ror::Round<ror::Vector3<TypeParam>> bounding_sphere1;

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

}        // namespace ror_test
