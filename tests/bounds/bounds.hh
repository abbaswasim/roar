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

namespace ror_test
{
template <class _type>
void BoundingTest<_type>::SetUp()
{
	this->m_vector1 = ror::Vector3f(1.0f, 1.0f, 1.0f);
	this->m_vector2 = ror::Vector3f(2.0f, 2.0f, 2.0f);
	this->m_vector3 = ror::Vector3f(3.0f, 3.0f, 3.0f);
	this->m_vector4 = ror::Vector3f(4.0f, 4.0f, 4.0f);

	this->m_vector_negative1 = -this->m_vector1;
	this->m_vector_negative2 = -this->m_vector2;
	this->m_vector_negative3 = -this->m_vector3;
	this->m_vector_negative4 = -this->m_vector4;
}

template <class _type>
void BoundingTest<_type>::TearDown()
{}

TYPED_TEST(BoundingTest, BoundingSphere_unit_test_constructors_work)
{
	ror::BoundingSphere bounding_sphere1;
	ASSERT_TRUE((bounding_sphere1.radius() - 0.0f) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_sphere1.center() == ror::zero_vector3f);
	ASSERT_TRUE(bounding_sphere1.m_type == ror::BoundingType::bounding_type_3d_sphere);

	ASSERT_TRUE((bounding_sphere1.radius() - this->m_bounding_sphere.radius()) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_sphere1.center() == this->m_bounding_sphere.center());
	ASSERT_TRUE(bounding_sphere1.m_type == this->m_bounding_sphere.m_type);

	ror::BoundingSphere bounding_sphere2(bounding_sphere1);
	ASSERT_TRUE((bounding_sphere1.radius() - bounding_sphere2.radius()) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_sphere1.center() == bounding_sphere1.center());
	ASSERT_TRUE(bounding_sphere1.m_type == bounding_sphere1.m_type);
}
//----------------------------------------------------------------------------//
TYPED_TEST(BoundingTest, BoundingSphere_unit_test_accessors_mutators)
{
	ror::BoundingSphere bounding_sphere1;
	ror::Vector3f       center(2.5f, 3.0f, 1.2f);
	bounding_sphere1.set_radius(5.0f);
	bounding_sphere1.set_center(center);
	ASSERT_TRUE((bounding_sphere1.radius() - 5.0f) < ror::ror_epsilon_squared);
	ASSERT_TRUE(bounding_sphere1.center() == center);
}
//----------------------------------------------------------------------------//
TYPED_TEST(BoundingTest, BoundingSphere_unit_test_add_point)
{
	ror::BoundingSphere bounding_sphere1;
	bounding_sphere1.set_center(this->m_vector2);
	bounding_sphere1.set_radius(1.0f);
	bounding_sphere1.add_point(this->m_vector3);
	EXPECT_GT(bounding_sphere1.radius(), 1.0f);
	float32_t nextRadius = bounding_sphere1.radius();
	bounding_sphere1.add_point(this->m_vector4);
	EXPECT_GT(bounding_sphere1.radius(), nextRadius);

	nextRadius = bounding_sphere1.radius();
	bounding_sphere1.add_point(this->m_vector4);
	ASSERT_TRUE(abs(bounding_sphere1.radius() - nextRadius) < ror::ror_epsilon_squared);

	nextRadius = bounding_sphere1.radius();
	bounding_sphere1.add_point(this->m_vector2);
	ASSERT_TRUE(abs(bounding_sphere1.radius() - nextRadius) < ror::ror_epsilon_squared);

	ror::Vector3f pointx(40.0f, 0.0f, 0.0f);
	ror::Vector3f pointypositive(20.0f, 20.0f, 0.0f);
	ror::Vector3f pointynegative(20.0f, -20.0f, 0.0f);
	ror::Vector3f pointzpositive(20.0f, 0.0f, 20.0f);
	ror::Vector3f pointznegative(20.0f, 0.0f, -20.0f);
	ror::Vector3f pointallpositive(20.0f, 20.0f, 20.0f);
	ror::Vector3f pointallnegative(-20.0f, -20.0f, -20.0f);

	ror::Vector3f point1(0.0f, 0.0f, 0.0f);

	this->m_bounding_sphere.add_point(point1);
	this->m_bounding_sphere.add_point(pointx);
	ASSERT_TRUE(abs(this->m_bounding_sphere.radius() - 20.0f) < ror::ror_epsilon_squared);
	this->m_bounding_sphere.add_point(pointypositive);
	ASSERT_TRUE(abs(this->m_bounding_sphere.radius() - 20.0f) < ror::ror_epsilon_squared);
	this->m_bounding_sphere.add_point(pointynegative);
	ASSERT_TRUE(abs(this->m_bounding_sphere.radius() - 20.0f) < ror::ror_epsilon_squared);
	this->m_bounding_sphere.add_point(pointzpositive);
	ASSERT_TRUE(abs(this->m_bounding_sphere.radius() - 20.0f) < ror::ror_epsilon_squared);
	this->m_bounding_sphere.add_point(pointznegative);
	ASSERT_TRUE(abs(this->m_bounding_sphere.radius() - 20.0f) < ror::ror_epsilon_squared);

	this->m_bounding_sphere.add_point(pointallpositive);
	EXPECT_GT(this->m_bounding_sphere.radius(), 24.0f);
	this->m_bounding_sphere.add_point(pointallnegative);
	EXPECT_GT(this->m_bounding_sphere.radius(), 37.0f);

	ror::BoundingSphere bounding_sphere2;
	ror::Vector3f       pointSameCenter1(2.0f, 0.0f, 0.0f);
	ror::Vector3f       pointSameCenter2(-2.0f, 0.0f, 0.0f);

	bounding_sphere2.set_radius(1.0f);
	bounding_sphere2.set_center(point1);

	bounding_sphere2.add_point(pointSameCenter1);
	ASSERT_FALSE(bounding_sphere2.center() == point1);
	bounding_sphere2.add_point(pointSameCenter2);
	ASSERT_TRUE(abs(bounding_sphere2.radius() - 2.0f) < ror::ror_epsilon);
	ASSERT_TRUE(bounding_sphere2.center() == point1);
}
//----------------------------------------------------------------------------//
TYPED_TEST(BoundingTest, BoundingSphere_unit_test_is_point_inside)
{
	ror::BoundingSphere bounding_sphere1;
	ror::BoundingSphere bounding_sphere2;

	ror::Vector3f point0(0.0f, 0.0f, 0.0f);
	ror::Vector3f point1(1.0f, 1.0f, 1.0f);

	ASSERT_TRUE(abs(this->m_bounding_sphere.radius() - 0.0f) < ror::ror_epsilon_squared);
	ASSERT_TRUE(this->m_bounding_sphere.center() == point0);

	this->m_bounding_sphere.set_radius(2.0f);
	this->m_bounding_sphere.set_center(point0);

	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector1) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector2) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector3) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector4) == true);

	this->m_bounding_sphere.add_point(this->m_vector4);
	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector3) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector_negative3) == true);
	ASSERT_FALSE(this->m_bounding_sphere.is_point_inside(this->m_vector_negative2) == true);

	this->m_bounding_sphere.add_point(this->m_vector_negative4);
	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector_negative3) == true);
	EXPECT_GT(this->m_bounding_sphere.radius(), 6.0f);
	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(this->m_vector_negative2) == true);
}
//----------------------------------------------------------------------------//
TYPED_TEST(BoundingTest, BoundingSphere_unit_test_add_bounding_sphere)
{
	ror::Vector3f center;
	float    radius;

	ror::BoundingSphere bounding_sphere1;
	ror::BoundingSphere bounding_sphere2;
	ror::BoundingSphere bounding_sphere3;
	ror::BoundingSphere bounding_sphere4;

	bounding_sphere1.set_radius(1.0f);
	bounding_sphere2.set_radius(1.0f);
	bounding_sphere3.set_radius(1.0f);
	bounding_sphere4.set_radius(1.0f);

	bounding_sphere1.set_center(this->m_vector1);
	bounding_sphere2.set_center(this->m_vector2);
	bounding_sphere3.set_center(this->m_vector3);
	bounding_sphere4.set_center(this->m_vector4);

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

	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector1) == true);
	ASSERT_FALSE(bounding_sphere1.is_point_inside(this->m_vector2) == true);
	ASSERT_FALSE(bounding_sphere1.is_point_inside(this->m_vector3) == true);
	ASSERT_FALSE(bounding_sphere1.is_point_inside(this->m_vector4) == true);

	ASSERT_FALSE(bounding_sphere2.is_point_inside(this->m_vector1) == true);
	ASSERT_TRUE(bounding_sphere2.is_point_inside(this->m_vector2) == true);
	ASSERT_FALSE(bounding_sphere2.is_point_inside(this->m_vector3) == true);
	ASSERT_FALSE(bounding_sphere2.is_point_inside(this->m_vector4) == true);

	ASSERT_FALSE(bounding_sphere3.is_point_inside(this->m_vector1) == true);
	ASSERT_FALSE(bounding_sphere3.is_point_inside(this->m_vector2) == true);
	ASSERT_TRUE(bounding_sphere3.is_point_inside(this->m_vector3) == true);
	ASSERT_FALSE(bounding_sphere3.is_point_inside(this->m_vector4) == true);

	ASSERT_FALSE(bounding_sphere4.is_point_inside(this->m_vector1) == true);
	ASSERT_FALSE(bounding_sphere4.is_point_inside(this->m_vector2) == true);
	ASSERT_FALSE(bounding_sphere4.is_point_inside(this->m_vector3) == true);
	ASSERT_TRUE(bounding_sphere4.is_point_inside(this->m_vector4) == true);

	bounding_sphere1.add_bounding(bounding_sphere2);
	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector1) == true);
	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector2) == true);
	ASSERT_FALSE(bounding_sphere1.is_point_inside(this->m_vector3) == true);
	ASSERT_FALSE(bounding_sphere1.is_point_inside(this->m_vector4) == true);
	bounding_sphere1.add_bounding(bounding_sphere3);
	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector1) == true);
	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector2) == true);
	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3) == true);
	ASSERT_FALSE(bounding_sphere1.is_point_inside(this->m_vector4) == true);
	bounding_sphere1.add_bounding(bounding_sphere4);
	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector1) == true);
	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector2) == true);
	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector3) == true);
	ASSERT_TRUE(bounding_sphere1.is_point_inside(this->m_vector4) == true);

	bounding_sphere2.add_bounding(bounding_sphere3);
	ASSERT_FALSE(bounding_sphere2.is_point_inside(this->m_vector1) == true);
	ASSERT_TRUE(bounding_sphere2.is_point_inside(this->m_vector2) == true);
	ASSERT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3) == true);
	ASSERT_FALSE(bounding_sphere2.is_point_inside(this->m_vector4) == true);
	bounding_sphere2.add_bounding(bounding_sphere4);
	ASSERT_FALSE(bounding_sphere2.is_point_inside(this->m_vector1) == true);
	ASSERT_TRUE(bounding_sphere2.is_point_inside(this->m_vector2) == true);
	ASSERT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3) == true);
	ASSERT_TRUE(bounding_sphere2.is_point_inside(this->m_vector4) == true);

	bounding_sphere3.add_bounding(bounding_sphere4);
	ASSERT_FALSE(bounding_sphere3.is_point_inside(this->m_vector1) == true);
	ASSERT_FALSE(bounding_sphere3.is_point_inside(this->m_vector2) == true);
	ASSERT_TRUE(bounding_sphere3.is_point_inside(this->m_vector3) == true);
	ASSERT_TRUE(bounding_sphere3.is_point_inside(this->m_vector4) == true);

	bounding_sphere2.add_bounding(bounding_sphere4);
	ASSERT_FALSE(bounding_sphere2.is_point_inside(this->m_vector1) == true);
	ASSERT_TRUE(bounding_sphere2.is_point_inside(this->m_vector2) == true);
	ASSERT_TRUE(bounding_sphere2.is_point_inside(this->m_vector3) == true);
	ASSERT_TRUE(bounding_sphere2.is_point_inside(this->m_vector4) == true);
}

/*
//----------------------------------------------------------------------------//
TYPED_TEST(BoundingTest, BoundingSphere_unit_test_bounding_sphere_collision)
{
	ror::Vector3f center;

	ror::BoundingSphere bounding_sphere1;
	ror::BoundingSphere bounding_sphere2;
	ror::BoundingSphere bounding_sphere3;
	ror::BoundingSphere bounding_sphere4;
	ror::BoundingSphere bounding_sphere5;

	bounding_sphere1.set_radius(1.0f);
	bounding_sphere2.set_radius(1.0f);
	bounding_sphere3.set_radius(1.0f);
	bounding_sphere4.set_radius(1.0f);
	bounding_sphere5.set_radius(6.99f);


bounding_sphere1.set_center(this->m_vector1);
	bounding_sphere2.set_center(this->m_vector2);
	bounding_sphere3.set_center(this->m_vector3);
	bounding_sphere4.set_center(this->m_vector4);

	ASSERT_TRUE(bounding_sphere1.bounding_sphere_collision(&bounding_sphere1) == ror::BoundingType::BOUNDING3DCOLLISION_INSIDE);
	ASSERT_TRUE(bounding_sphere1.bounding_sphere_collision(&bounding_sphere2) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);
	ASSERT_FALSE(bounding_sphere1.bounding_sphere_collision(&bounding_sphere3) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);
	ASSERT_FALSE(bounding_sphere1.bounding_sphere_collision(&bounding_sphere4) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);

	ASSERT_TRUE(bounding_sphere2.bounding_sphere_collision(&bounding_sphere1) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);
	ASSERT_TRUE(bounding_sphere2.bounding_sphere_collision(&bounding_sphere2) == ror::BoundingType::BOUNDING3DCOLLISION_INSIDE);
	ASSERT_TRUE(bounding_sphere2.bounding_sphere_collision(&bounding_sphere3) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);
	ASSERT_FALSE(bounding_sphere2.bounding_sphere_collision(&bounding_sphere4) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);

	ASSERT_FALSE(bounding_sphere3.bounding_sphere_collision(&bounding_sphere1) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);
	ASSERT_TRUE(bounding_sphere3.bounding_sphere_collision(&bounding_sphere2) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);
	ASSERT_TRUE(bounding_sphere3.bounding_sphere_collision(&bounding_sphere3) == ror::BoundingType::BOUNDING3DCOLLISION_INSIDE);
	ASSERT_TRUE(bounding_sphere3.bounding_sphere_collision(&bounding_sphere4) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);

	ASSERT_FALSE(bounding_sphere4.bounding_sphere_collision(&bounding_sphere1) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);
	ASSERT_FALSE(bounding_sphere4.bounding_sphere_collision(&bounding_sphere2) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);
	ASSERT_TRUE(bounding_sphere4.bounding_sphere_collision(&bounding_sphere3) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);
	ASSERT_TRUE(bounding_sphere4.bounding_sphere_collision(&bounding_sphere4) == ror::BoundingType::BOUNDING3DCOLLISION_INSIDE);

	ASSERT_TRUE(bounding_sphere5.bounding_sphere_collision(&bounding_sphere1) == ror::BoundingType::BOUNDING3DCOLLISION_INSIDE);
	ASSERT_TRUE(bounding_sphere5.bounding_sphere_collision(&bounding_sphere2) == ror::BoundingType::BOUNDING3DCOLLISION_INSIDE);
	ASSERT_TRUE(bounding_sphere5.bounding_sphere_collision(&bounding_sphere3) == ror::BoundingType::BOUNDING3DCOLLISION_INSIDE);
	ASSERT_TRUE(bounding_sphere5.bounding_sphere_collision(&bounding_sphere4) == ror::BoundingType::BOUNDING3DCOLLISION_INTERSECTS);
	bounding_sphere5.set_radius(8.0f);
	ASSERT_TRUE(bounding_sphere5.bounding_sphere_collision(&bounding_sphere4) == ror::BoundingType::BOUNDING3DCOLLISION_INSIDE);
}
//----------------------------------------------------------------------------//
TYPED_TEST(BoundingTest, BoundingSphere_unit_test_create_from_points)
{
	ror::Vector3f center;
	uint32_t pointscount = 4;
	uint32_t components  = 3;
	// four points and 3 components each
	float32_t *points = new float32_t[pointscount * components];

	points[0] = this->m_vector1.x;
	points[1] = this->m_vector1.y;
	points[2] = this->m_vector1.z;

	points[3] = this->m_vector2.x;
	points[4] = this->m_vector2.y;
	points[5] = this->m_vector2.z;

	points[6] = this->m_vector3.x;
	points[7] = this->m_vector3.y;
	points[8] = this->m_vector3.z;

	points[9]  = this->m_vector4.x;
	points[10] = this->m_vector4.y;
	points[11] = this->m_vector4.z;

	ror::BoundingSphere bounding_sphere1;
	this->m_bounding_sphere.create_from_points(points, pointscount * components);

	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(&this->m_vector1) == true);
	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(&this->m_vector2) == true);
	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(&this->m_vector3) == true);
	ASSERT_TRUE(this->m_bounding_sphere.is_point_inside(&this->m_vector4) == true);

	float32_t distanceFromCenterToCenter = (this->m_vector4 - this->m_vector1).length() / 2;

	EXPECT_GT(this->m_bounding_sphere.radius(), distanceFromCenterToCenter);
}
*/
}        // namespace ror_test
