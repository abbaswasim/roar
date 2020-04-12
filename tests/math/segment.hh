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

#include "segment.hpp"

namespace ror_test
{
template <class _type>
void SegmentTest<_type>::SetUp()
{}

template <class _type>
void SegmentTest<_type>::TearDown()
{}

TYPED_TEST(SegmentTest, segment_ops)
{
	ror::Vector2<TypeParam> a(static_cast<TypeParam>(-20), static_cast<TypeParam>(-2));
	ror::Vector2<TypeParam> b(static_cast<TypeParam>(-12), static_cast<TypeParam>(50));
	ror::Vector2<TypeParam> c(static_cast<TypeParam>(38), static_cast<TypeParam>(1));
	ror::Vector2<TypeParam> d(static_cast<TypeParam>(-1), static_cast<TypeParam>(50));

	ror::Vector3<TypeParam> e(static_cast<TypeParam>(-20), static_cast<TypeParam>(-2), static_cast<TypeParam>(-4));
	ror::Vector3<TypeParam> f(static_cast<TypeParam>(-12), static_cast<TypeParam>(50), static_cast<TypeParam>(2));
	ror::Vector3<TypeParam> g(static_cast<TypeParam>(38), static_cast<TypeParam>(1), static_cast<TypeParam>(-4));
	ror::Vector3<TypeParam> h(static_cast<TypeParam>(-1), static_cast<TypeParam>(50), static_cast<TypeParam>(6));

	TypeParam s(static_cast<TypeParam>(2));         // Slope
	TypeParam i(static_cast<TypeParam>(10));        // Intercept

	ror::Vector2<TypeParam> p(static_cast<TypeParam>(-20), static_cast<TypeParam>(50));
	ror::Vector2<TypeParam> q(static_cast<TypeParam>(30), static_cast<TypeParam>(5));
	ror::Vector3<TypeParam> r(static_cast<TypeParam>(-20), static_cast<TypeParam>(50), static_cast<TypeParam>(0));
	ror::Vector3<TypeParam> t(static_cast<TypeParam>(30), static_cast<TypeParam>(5), static_cast<TypeParam>(0));

	ror::Vector2<TypeParam> ab(static_cast<TypeParam>(-16), static_cast<TypeParam>(24));
	ror::Vector2<TypeParam> bc(static_cast<TypeParam>(13), static_cast<TypeParam>(25.5));

	ror::Vector2<TypeParam> abbc(static_cast<TypeParam>(-1.5), static_cast<TypeParam>(24.75));

	ror::Vector3<TypeParam> ef(static_cast<TypeParam>(-16), static_cast<TypeParam>(24), static_cast<TypeParam>(-1));
	ror::Vector3<TypeParam> fg(static_cast<TypeParam>(13), static_cast<TypeParam>(25.5), static_cast<TypeParam>(-1));

	ror::Vector3<TypeParam> effg(static_cast<TypeParam>(-1.5), static_cast<TypeParam>(24.75), static_cast<TypeParam>(-1));

	{
		ror::Line<ror::Vector2<TypeParam>, 1> l1(a, c);        // two point form
		ror::Line<ror::Vector2<TypeParam>, 1> l2(s, i);        // slope intercept form

		ror::Vector2<TypeParam> j(0, 10);
		ror::Vector2<TypeParam> k(1, 12);

		test_vector2_equal(l1.m_points[0], a);
		test_vector2_equal(l1.m_points[1], c);

		test_vector2_equal(l2.m_points[0], j);
		test_vector2_equal(l2.m_points[1], k);
	}
	{
		ror::Triangle<ror::Vector2<TypeParam>, 2> t1(a, b, c);

		test_vector2_equal(t1.m_points[0], a);
		test_vector2_equal(t1.m_points[1], b);
		test_vector2_equal(t1.m_points[2], c);

		EXPECT_FALSE(t1.is_point_inside_my_2d_triangle(p));
		EXPECT_TRUE(t1.is_point_inside_my_2d_triangle(q));
	}
	{
		ror::Bezier<ror::Vector2<TypeParam>, 2> b1(a, b, c);
		ror::Bezier<ror::Vector2<TypeParam>, 2> b2(c, b, a);

		test_vector2_equal(b1.m_points[0], a);
		test_vector2_equal(b1.m_points[1], b);
		test_vector2_equal(b1.m_points[2], c);

		test_vector2_equal(b2.m_points[0], c);
		test_vector2_equal(b2.m_points[1], b);
		test_vector2_equal(b2.m_points[2], a);

		EXPECT_FALSE(b1.is_point_inside_my_2d_triangle(p));
		EXPECT_TRUE(b1.is_point_inside_my_2d_triangle(q));

		EXPECT_FALSE(b2.is_point_inside_my_2d_triangle(p));
		EXPECT_TRUE(b2.is_point_inside_my_2d_triangle(q));

		EXPECT_EQ(b1.m_type, ror::SegmentType::quadratic_2d_outer);
		EXPECT_EQ(b2.m_type, ror::SegmentType::quadratic_2d_inner);

		std::vector<ror::Bezier<ror::Vector2<TypeParam>, 2>> kids;

		b1.subdivide(kids);

		test_vector2_equal(kids[0].m_points[0], a);
		test_vector2_equal(kids[0].m_points[1], ab);
		test_vector2_equal(kids[0].m_points[2], abbc);

		test_vector2_equal(kids[1].m_points[0], abbc);
		test_vector2_equal(kids[1].m_points[1], bc);
		test_vector2_equal(kids[1].m_points[2], c);
	}
	{
		ror::Bezier<ror::Vector2<TypeParam>, 3> b1(a, b, c, d);

		test_vector2_equal(b1.m_points[0], a);
		test_vector2_equal(b1.m_points[1], b);
		test_vector2_equal(b1.m_points[2], c);
		test_vector2_equal(b1.m_points[3], d);
	}
	{
		ror::Line<ror::Vector3<TypeParam>, 1> l1(e, f);
		ror::Line<ror::Vector3<TypeParam>, 1> l2(s, i);

		test_vector3_equal(l1.m_points[0], e);
		test_vector3_equal(l1.m_points[1], f);

		ror::Vector3<TypeParam> j(0, 10, 0);
		ror::Vector3<TypeParam> k(1, 12, 0);

		test_vector3_equal(l2.m_points[0], j);
		test_vector3_equal(l2.m_points[1], k);
	}
	{
		ror::Triangle<ror::Vector3<TypeParam>, 2> t1(e, f, g);

		test_vector3_equal(t1.m_points[0], e);
		test_vector3_equal(t1.m_points[1], f);
		test_vector3_equal(t1.m_points[2], g);

		EXPECT_FALSE(t1.is_point_inside_my_2d_triangle(r));
		EXPECT_TRUE(t1.is_point_inside_my_2d_triangle(t));
	}
	{
		ror::Bezier<ror::Vector3<TypeParam>, 2> b1(e, f, g);
		ror::Bezier<ror::Vector3<TypeParam>, 2> b2(g, f, e);

		test_vector3_equal(b1.m_points[0], e);
		test_vector3_equal(b1.m_points[1], f);
		test_vector3_equal(b1.m_points[2], g);

		test_vector3_equal(b2.m_points[0], g);
		test_vector3_equal(b2.m_points[1], f);
		test_vector3_equal(b2.m_points[2], e);

		EXPECT_FALSE(b1.is_point_inside_my_2d_triangle(r));
		EXPECT_TRUE(b1.is_point_inside_my_2d_triangle(t));

		EXPECT_FALSE(b2.is_point_inside_my_2d_triangle(r));
		EXPECT_TRUE(b2.is_point_inside_my_2d_triangle(t));

		EXPECT_EQ(b1.m_type, ror::SegmentType::quadratic_3d_outer);
		EXPECT_EQ(b2.m_type, ror::SegmentType::quadratic_3d_inner);

		std::vector<ror::Bezier<ror::Vector3<TypeParam>, 2>> kids;

		b1.subdivide(kids);

		test_vector3_equal(kids[0].m_points[0], e);
		test_vector3_equal(kids[0].m_points[1], ef);
		test_vector3_equal(kids[0].m_points[2], effg);

		test_vector3_equal(kids[1].m_points[0], effg);
		test_vector3_equal(kids[1].m_points[1], fg);
		test_vector3_equal(kids[1].m_points[2], g);
	}
	{
		ror::Bezier<ror::Vector3<TypeParam>, 3> b1(e, f, g, h);

		test_vector3_equal(b1.m_points[0], e);
		test_vector3_equal(b1.m_points[1], f);
		test_vector3_equal(b1.m_points[2], g);
		test_vector3_equal(b1.m_points[3], h);
	}
}

TYPED_TEST(SegmentTest, segments_ops)
{
	ror::Vector2<TypeParam> a(static_cast<TypeParam>(-20), static_cast<TypeParam>(-2));
	ror::Vector2<TypeParam> b(static_cast<TypeParam>(-12), static_cast<TypeParam>(50));
	ror::Vector2<TypeParam> c(static_cast<TypeParam>(38), static_cast<TypeParam>(1));
	ror::Vector2<TypeParam> d(static_cast<TypeParam>(-1), static_cast<TypeParam>(50));

	ror::Vector3<TypeParam> e(static_cast<TypeParam>(-20), static_cast<TypeParam>(-2), static_cast<TypeParam>(-4));
	ror::Vector3<TypeParam> f(static_cast<TypeParam>(-12), static_cast<TypeParam>(50), static_cast<TypeParam>(2));
	ror::Vector3<TypeParam> g(static_cast<TypeParam>(38), static_cast<TypeParam>(1), static_cast<TypeParam>(-4));
	ror::Vector3<TypeParam> h(static_cast<TypeParam>(-1), static_cast<TypeParam>(50), static_cast<TypeParam>(6));

	ror::Vector2<TypeParam> p(static_cast<TypeParam>(-20), static_cast<TypeParam>(50));
	ror::Vector2<TypeParam> q(static_cast<TypeParam>(30), static_cast<TypeParam>(5));
	ror::Vector3<TypeParam> r(static_cast<TypeParam>(-20), static_cast<TypeParam>(50), static_cast<TypeParam>(0));
	ror::Vector3<TypeParam> t(static_cast<TypeParam>(30), static_cast<TypeParam>(5), static_cast<TypeParam>(0));

	ror::Vector2<TypeParam> ab(static_cast<TypeParam>(-16), static_cast<TypeParam>(24));
	ror::Vector2<TypeParam> bc(static_cast<TypeParam>(13), static_cast<TypeParam>(25.5));

	ror::Vector2<TypeParam> abbc(static_cast<TypeParam>(-1.5), static_cast<TypeParam>(24.75));

	ror::Vector3<TypeParam> ef(static_cast<TypeParam>(-16), static_cast<TypeParam>(24), static_cast<TypeParam>(-1));
	ror::Vector3<TypeParam> fg(static_cast<TypeParam>(13), static_cast<TypeParam>(25.5), static_cast<TypeParam>(-1));

	ror::Vector3<TypeParam> effg(static_cast<TypeParam>(-1.5), static_cast<TypeParam>(24.75), static_cast<TypeParam>(-1));

	{
		ror::Lines<ror::Vector2<TypeParam>> l1;

		EXPECT_EQ(l1.get_count(), 0);

		l1.add_line(a, c);
		l1.add_line(a, c);
		l1.add_line(a, c);

		test_vector2_equal(l1.m_points[0], a);
		test_vector2_equal(l1.m_points[1], c);

		test_vector2_equal(l1.m_points[2], a);
		test_vector2_equal(l1.m_points[3], c);

		test_vector2_equal(l1.m_points[4], a);
		test_vector2_equal(l1.m_points[5], c);

		EXPECT_EQ(l1.get_count(), 3);
	}
	{
		ror::LineStrip<ror::Vector2<TypeParam>> l1;

		EXPECT_EQ(l1.get_count(), 0);

		l1.add_line(a);
		l1.add_line(b);
		l1.add_line(c);
		l1.add_line(d);

		test_vector2_equal(l1.m_points[0], a);
		test_vector2_equal(l1.m_points[1], b);
		test_vector2_equal(l1.m_points[2], c);
		test_vector2_equal(l1.m_points[3], d);

		EXPECT_EQ(l1.get_count(), 3);
	}
	{
		ror::Triangles<ror::Vector2<TypeParam>> t1;

		EXPECT_EQ(t1.get_count(), 0);

		t1.add_triangle(a, b, c);
		t1.add_triangle(a, b, c);
		t1.add_triangle(a, b, c);

		test_vector2_equal(t1.m_points[0], a);
		test_vector2_equal(t1.m_points[1], b);
		test_vector2_equal(t1.m_points[2], c);

		test_vector2_equal(t1.m_points[3], a);
		test_vector2_equal(t1.m_points[4], b);
		test_vector2_equal(t1.m_points[5], c);

		test_vector2_equal(t1.m_points[6], a);
		test_vector2_equal(t1.m_points[7], b);
		test_vector2_equal(t1.m_points[8], c);

		EXPECT_EQ(t1.get_count(), 3);
	}
	{
		ror::TriangleStrip<ror::Vector2<TypeParam>> t1;

		EXPECT_EQ(t1.get_count(), 0);

		t1.add_triangle(a);
		t1.add_triangle(b);
		t1.add_triangle(c);
		t1.add_triangle(d);
		t1.add_triangle(a);

		test_vector2_equal(t1.m_points[0], a);
		test_vector2_equal(t1.m_points[1], b);
		test_vector2_equal(t1.m_points[2], c);
		test_vector2_equal(t1.m_points[3], d);
		test_vector2_equal(t1.m_points[4], a);

		EXPECT_EQ(t1.get_count(), 3);
	}
	{
		ror::QuadraticBeziers<ror::Vector2<TypeParam>> b1;

		EXPECT_EQ(b1.get_count(), 0);

		b1.add_bezier(a, b, c);
		b1.add_bezier(a, b, c);
		b1.add_bezier(a, b, c);

		test_vector2_equal(b1.m_points[0], a);
		test_vector2_equal(b1.m_points[1], b);
		test_vector2_equal(b1.m_points[2], c);

		test_vector2_equal(b1.m_points[3], a);
		test_vector2_equal(b1.m_points[4], b);
		test_vector2_equal(b1.m_points[5], c);

		test_vector2_equal(b1.m_points[6], a);
		test_vector2_equal(b1.m_points[7], b);
		test_vector2_equal(b1.m_points[8], c);

		EXPECT_EQ(b1.get_count(), 3);
	}
	{
		ror::CubicBeziers<ror::Vector2<TypeParam>> b1;

		EXPECT_EQ(b1.get_count(), 0);

		b1.add_bezier(a, b, c, d);
		b1.add_bezier(a, b, c, d);
		b1.add_bezier(a, b, c, d);

		test_vector2_equal(b1.m_points[0], a);
		test_vector2_equal(b1.m_points[1], b);
		test_vector2_equal(b1.m_points[2], c);
		test_vector2_equal(b1.m_points[3], d);

		test_vector2_equal(b1.m_points[4], a);
		test_vector2_equal(b1.m_points[5], b);
		test_vector2_equal(b1.m_points[6], c);
		test_vector2_equal(b1.m_points[7], d);

		test_vector2_equal(b1.m_points[8], a);
		test_vector2_equal(b1.m_points[9], b);
		test_vector2_equal(b1.m_points[10], c);
		test_vector2_equal(b1.m_points[11], d);

		EXPECT_EQ(b1.get_count(), 3);
	}
	{
		ror::Lines<ror::Vector3<TypeParam>> l1;

		EXPECT_EQ(l1.get_count(), 0);

		l1.add_line(e, g);
		l1.add_line(e, g);
		l1.add_line(e, g);

		test_vector3_equal(l1.m_points[0], e);
		test_vector3_equal(l1.m_points[1], g);

		test_vector3_equal(l1.m_points[2], e);
		test_vector3_equal(l1.m_points[3], g);

		test_vector3_equal(l1.m_points[4], e);
		test_vector3_equal(l1.m_points[5], g);

		EXPECT_EQ(l1.get_count(), 3);
	}
	{
		ror::LineStrip<ror::Vector3<TypeParam>> l1;

		EXPECT_EQ(l1.get_count(), 0);

		l1.add_line(e);
		l1.add_line(f);
		l1.add_line(g);
		l1.add_line(h);

		test_vector3_equal(l1.m_points[0], e);
		test_vector3_equal(l1.m_points[1], f);
		test_vector3_equal(l1.m_points[2], g);
		test_vector3_equal(l1.m_points[3], h);

		EXPECT_EQ(l1.get_count(), 3);
	}
	{
		ror::Triangles<ror::Vector3<TypeParam>> t1;

		EXPECT_EQ(t1.get_count(), 0);

		t1.add_triangle(e, f, g);
		t1.add_triangle(e, f, g);
		t1.add_triangle(e, f, g);

		test_vector3_equal(t1.m_points[0], e);
		test_vector3_equal(t1.m_points[1], f);
		test_vector3_equal(t1.m_points[2], g);

		test_vector3_equal(t1.m_points[3], e);
		test_vector3_equal(t1.m_points[4], f);
		test_vector3_equal(t1.m_points[5], g);

		test_vector3_equal(t1.m_points[6], e);
		test_vector3_equal(t1.m_points[7], f);
		test_vector3_equal(t1.m_points[8], g);

		EXPECT_EQ(t1.get_count(), 3);
	}
	{
		ror::TriangleStrip<ror::Vector3<TypeParam>> t1;

		EXPECT_EQ(t1.get_count(), 0);

		t1.add_triangle(e);
		t1.add_triangle(f);
		t1.add_triangle(g);
		t1.add_triangle(h);
		t1.add_triangle(e);

		test_vector3_equal(t1.m_points[0], e);
		test_vector3_equal(t1.m_points[1], f);
		test_vector3_equal(t1.m_points[2], g);
		test_vector3_equal(t1.m_points[3], h);
		test_vector3_equal(t1.m_points[4], e);

		EXPECT_EQ(t1.get_count(), 3);
	}
	{
		ror::QuadraticBeziers<ror::Vector3<TypeParam>> b1;

		EXPECT_EQ(b1.get_count(), 0);

		b1.add_bezier(e, f, g);
		b1.add_bezier(e, f, g);
		b1.add_bezier(e, f, g);

		test_vector3_equal(b1.m_points[0], e);
		test_vector3_equal(b1.m_points[1], f);
		test_vector3_equal(b1.m_points[2], g);

		test_vector3_equal(b1.m_points[3], e);
		test_vector3_equal(b1.m_points[4], f);
		test_vector3_equal(b1.m_points[5], g);

		test_vector3_equal(b1.m_points[6], e);
		test_vector3_equal(b1.m_points[7], f);
		test_vector3_equal(b1.m_points[8], g);

		EXPECT_EQ(b1.get_count(), 3);
	}
	{
		ror::CubicBeziers<ror::Vector3<TypeParam>> b1;

		EXPECT_EQ(b1.get_count(), 0);

		b1.add_bezier(e, f, g, h);
		b1.add_bezier(e, f, g, h);
		b1.add_bezier(e, f, g, h);

		test_vector3_equal(b1.m_points[0], e);
		test_vector3_equal(b1.m_points[1], f);
		test_vector3_equal(b1.m_points[2], g);
		test_vector3_equal(b1.m_points[3], h);

		test_vector3_equal(b1.m_points[4], e);
		test_vector3_equal(b1.m_points[5], f);
		test_vector3_equal(b1.m_points[6], g);
		test_vector3_equal(b1.m_points[7], h);

		test_vector3_equal(b1.m_points[8], e);
		test_vector3_equal(b1.m_points[9], f);
		test_vector3_equal(b1.m_points[10], g);
		test_vector3_equal(b1.m_points[11], h);

		EXPECT_EQ(b1.get_count(), 3);
	}
}

}        // namespace ror_test
