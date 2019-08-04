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
void SegmentTest<_type>::SetUp()
{}

template <class _type>
void SegmentTest<_type>::TearDown()
{}

template <class _type>
class Bezier2 final
{
  public:
	FORCE_INLINE Bezier2()                           = default;                   //! Default constructor
	FORCE_INLINE Bezier2(const Bezier2 &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Bezier2(Bezier2 &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Bezier2 &operator=(const Bezier2 &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Bezier2 &operator=(Bezier2 &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Bezier2() noexcept                            = default;        //! Destructor

	FORCE_INLINE      Bezier2(ror::Vector2<_type> a_Point1, ror::Vector2<_type> a_ControlPoint, ror::Vector2<_type> a_Point2);
	FORCE_INLINE void set(ror::Vector2<_type> a_Point1, ror::Vector2<_type> a_ControlPoint, ror::Vector2<_type> a_Point2);

	FORCE_INLINE bool is_point_inside_my_2d_triangle(const ror::Vector2<_type> &a_Point);

	FORCE_INLINE void subdivide(std::vector<Bezier2<_type>> &vec);

	ror::Vector2<_type>    m_Points[3];        //!<    The three points on the bezier curve
	const ror::SegmentType m_type = ror::SegmentType::segment_type_quadratic_3d;
};

template <class _type>
FORCE_INLINE Bezier2<_type>::Bezier2(ror::Vector2<_type> a_Point1, ror::Vector2<_type> a_ControlPoint, ror::Vector2<_type> a_Point2)
{
	this->m_Points[0] = a_Point1;
	this->m_Points[1] = a_ControlPoint;
	this->m_Points[2] = a_Point2;
}

template <class _type>
FORCE_INLINE void Bezier2<_type>::set(ror::Vector2<_type> a_Point1, ror::Vector2<_type> a_ControlPoint, ror::Vector2<_type> a_Point2)
{
	this->m_Points[0] = a_Point1;
	this->m_Points[1] = a_ControlPoint;
	this->m_Points[2] = a_Point2;
}

template <class _type>
FORCE_INLINE bool Bezier2<_type>::is_point_inside_my_2d_triangle(const ror::Vector2<_type> &a_point)
{
	if (a_point == this->m_Points[0] ||
		a_point == this->m_Points[1] ||
		a_point == this->m_Points[2])
	{
		return false;
	}

#define SIGN(p1, p2, p3) (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y)

	bool b1 = SIGN(a_point, this->m_Points[0], this->m_Points[1]) < 0.0;
	bool b2 = SIGN(a_point, this->m_Points[1], this->m_Points[2]) < 0.0;
	bool b3 = SIGN(a_point, this->m_Points[2], this->m_Points[0]) < 0.0;

	return ((b1 == b2) && (b2 == b3));
}

template <class _type>
FORCE_INLINE void Bezier2<_type>::subdivide(std::vector<Bezier2<_type>> &vec)
{
	ror::Vector2<_type> p1 = this->m_Points[0];
	ror::Vector2<_type> p2 = this->m_Points[1];
	ror::Vector2<_type> p3 = this->m_Points[2];

	ror::Vector2<_type> q1 = (p1 + p2) / 2;
	ror::Vector2<_type> q2 = (p2 + p3) / 2;

	ror::Vector2<_type> r1 = (q1 + q2) / 2;

	vec.emplace_back(p1, q1, r1);
	vec.emplace_back(r1, q2, p3);
}

TYPED_TEST(SegmentTest, DISABLED_testing_soup)
{
	const uint32_t max_size = 10000;

	std::vector<ror::Vector2<TypeParam>> points_soup;
	ror::Vector2<TypeParam>              test_point;

	points_soup.reserve(max_size);

	ror::Random<float32_t> random(-500.0f, 500.0f);

	for (size_t i = 0; i < max_size; ++i)
	{
		points_soup[i] = ror::Vector2<TypeParam>(static_cast<TypeParam>(random.next()), static_cast<TypeParam>(random.next()));
	}

	test_point = ror::Vector2<TypeParam>(static_cast<TypeParam>(random.next()), static_cast<TypeParam>(random.next()));

	std::vector<ror::Bezier<ror::Vector2<TypeParam>, 2>> children_soup;
	children_soup.reserve(max_size * 4);

	std::vector<ror_test::Bezier2<TypeParam>> children2_soup;
	children2_soup.reserve(max_size * 4);

	double64_t bezier_time  = 0.0;
	double64_t bezier2_time = 0.0;

	uint32_t lucky_ones1 = 0;
	uint32_t lucky_ones2 = 0;

	{
		ror::Timer t;
		size_t     k = 0;

		for (size_t j = 0; j < 2; ++j)
		{
			for (size_t i = 1; i < max_size - 2; ++i)
			{
				ror::Bezier<ror::Vector2<TypeParam>, 2> bezier(points_soup[k], points_soup[k + 1], points_soup[k + 2]);

				bezier.subdivide(children_soup);
				if (bezier.is_point_inside_my_2d_triangle(test_point))
					++lucky_ones1;

				k += 2;
			}
			k = 0;
		}

		bezier_time = static_cast<double64_t>(t.tick()) / 1000000.0;
	}

	{
		ror::Timer t;
		size_t     k = 0;

		for (size_t j = 0; j < 2; ++j)
		{
			for (size_t i = 1; i < max_size - 2; ++i)
			{
				ror_test::Bezier2<TypeParam> bezier(points_soup[k], points_soup[k + 1], points_soup[k + 2]);

				bezier.subdivide(children2_soup);
				if (bezier.is_point_inside_my_2d_triangle(test_point))
					++lucky_ones2;

				k += 2;
			}
			k = 0;
		}

		bezier2_time = static_cast<double64_t>(t.tick()) / 1000000.0;
	}

	EXPECT_GT(bezier2_time, bezier_time) << "Templates are slower to run";
	EXPECT_EQ(lucky_ones1, lucky_ones2);
}

TYPED_TEST(SegmentTest, cubic_conversion)
{
	const uint32_t max_size = 20;

	std::vector<ror::Vector2<TypeParam>> points_soup;

	points_soup.reserve(max_size);

	ror::Random<float32_t> random(-500.0f, 500.0f);

	for (size_t i = 0; i < max_size; ++i)
	{
		points_soup[i] = ror::Vector2<TypeParam>(static_cast<TypeParam>(random.next()), static_cast<TypeParam>(random.next()));
	}

	for (size_t i = 0; i < max_size; i += 4)
	{
		std::vector<ror::Bezier<ror::Vector2<TypeParam>, 2>> quads;

		auto cubic = ror::Bezier<ror::Vector2<TypeParam>, 3>(points_soup[i], points_soup[i + 1], points_soup[i + 2], points_soup[i + 3]);

		convert_cubic_to_quadratic(cubic, quads);
	}
}

TYPED_TEST(SegmentTest, segments_ops)
{
	ror::Vector2<TypeParam> a(-20, -2);
	ror::Vector2<TypeParam> b(-12, 30);
	ror::Vector2<TypeParam> c(8, 14);
	ror::Vector2<TypeParam> d(-1, 50);

	ror::Vector3<TypeParam> e(-20, -2, -4);
	ror::Vector3<TypeParam> f(-12, 30, 2);
	ror::Vector3<TypeParam> g(8, 14, -4);
	ror::Vector3<TypeParam> h(-1, 50, 6);

	TypeParam s(2);
	TypeParam i(10);

	ror::Vector2<TypeParam> p(10, 20);
	ror::Vector2<TypeParam> q(-10, 20);
	ror::Vector3<TypeParam> r(10, 20, 0);
	ror::Vector3<TypeParam> t(-10, 20, 0);

	ror::Vector2<TypeParam> ab(-16, 14);
	ror::Vector2<TypeParam> bc(-2, 22);

	ror::Vector2<TypeParam> abbc(-9, 18);

	ror::Vector3<TypeParam> ef(-16, 14, -1);
	ror::Vector3<TypeParam> fg(-2, 22, -1);

	ror::Vector3<TypeParam> effg(-9, 18, -1);

	{
		ror::Line<ror::Vector2<TypeParam>, 1> l1(a, c);
		ror::Line<ror::Vector2<TypeParam>, 1> l2(s, i);

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

		test_vector2_equal(b1.m_points[0], a);
		test_vector2_equal(b1.m_points[1], b);
		test_vector2_equal(b1.m_points[2], c);

		EXPECT_FALSE(b1.is_point_inside_my_2d_triangle(p));
		EXPECT_TRUE(b1.is_point_inside_my_2d_triangle(q));

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

		test_vector3_equal(b1.m_points[0], e);
		test_vector3_equal(b1.m_points[1], f);
		test_vector3_equal(b1.m_points[2], g);

		EXPECT_FALSE(b1.is_point_inside_my_2d_triangle(r));
		EXPECT_TRUE(b1.is_point_inside_my_2d_triangle(t));

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

}        // namespace ror_test
