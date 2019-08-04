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

namespace ror
{
template <class _type>
FORCE_INLINE Line<_type, 1, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::Line(_type a_point1, _type a_point2)
{
	this->set(a_point1, a_point2);
}

template <class _type>
FORCE_INLINE Line<_type, 1, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::Line(typename _type::value_type a_slope, typename _type::value_type a_intercept)
{
	this->set(a_slope, a_intercept);
}

template <class _type>
FORCE_INLINE void Line<_type, 1, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::set(_type a_point1, _type a_point2)
{
	this->m_points[0] = a_point1;
	this->m_points[1] = a_point2;
}

template <class _type>
FORCE_INLINE void Line<_type, 1, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::set(typename _type::value_type a_slope, typename _type::value_type a_intercept)
{
	/**
	 * Creates a line segment from slope intercept form
	 * Slope intercept form represents an infinite line but here we create a unit line
	 */
	this->m_points[0] = _type(static_cast<typename _type::value_type>(0), static_cast<typename _type::value_type>(a_intercept));
	this->m_points[1] = _type(static_cast<typename _type::value_type>(1), static_cast<typename _type::value_type>(a_slope));

	this->m_points[1] += this->m_points[0];
}

template <class _type>
FORCE_INLINE Line<_type, 1, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::Line(_type a_point1, _type a_point2)
{
	this->set(a_point1, a_point2);
}

template <class _type>
FORCE_INLINE Line<_type, 1, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::Line(typename _type::value_type a_slope, typename _type::value_type a_intercept)
{
	this->set(a_slope, a_intercept);
}

template <class _type>
FORCE_INLINE void Line<_type, 1, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::set(_type a_point1, _type a_point2)
{
	this->m_points[0] = a_point1;
	this->m_points[1] = a_point2;
}

template <class _type>
FORCE_INLINE void Line<_type, 1, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::set(typename _type::value_type a_slope, typename _type::value_type a_intercept)
{
	/**
	 * Creates a line segment from slope intercept form
	 * Slope intercept form represents an infinite line but here we create a unit line
	 */
	this->m_points[0] = _type(static_cast<typename _type::value_type>(0), static_cast<typename _type::value_type>(a_intercept), static_cast<typename _type::value_type>(0));
	this->m_points[1] = _type(static_cast<typename _type::value_type>(1), static_cast<typename _type::value_type>(a_slope), static_cast<typename _type::value_type>(0));

	this->m_points[1] += this->m_points[0];
}

template <class _type>
FORCE_INLINE Triangle<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::Triangle(_type a_point1, _type a_point2, _type a_point3)
{
	this->set(a_point1, a_point2, a_point3);
}

template <class _type>
FORCE_INLINE void Triangle<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::set(_type a_point1, _type a_point2, _type a_point3)
{
	this->m_points[0] = a_point1;
	this->m_points[1] = a_point2;
	this->m_points[2] = a_point3;
}

template <class _type>
FORCE_INLINE bool Triangle<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::is_point_inside_my_2d_triangle(const _type &a_point)
{
	if (a_point == this->m_points[0] ||
		a_point == this->m_points[1] ||
		a_point == this->m_points[2])
	{
		return false;
	}

#define SIGN(p1, p2, p3) (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y)

	bool b1 = SIGN(a_point, this->m_points[0], this->m_points[1]) < 0;
	bool b2 = SIGN(a_point, this->m_points[1], this->m_points[2]) < 0;
	bool b3 = SIGN(a_point, this->m_points[2], this->m_points[0]) < 0;

	return ((b1 == b2) && (b2 == b3));
}

template <class _type>
FORCE_INLINE Triangle<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::Triangle(_type a_point1, _type a_point2, _type a_point3)
{
	this->set(a_point1, a_point2, a_point3);
}

template <class _type>
FORCE_INLINE void Triangle<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::set(_type a_point1, _type a_point2, _type a_point3)
{
	this->m_points[0] = a_point1;
	this->m_points[1] = a_point2;
	this->m_points[2] = a_point3;
}

template <class _type>
FORCE_INLINE bool Triangle<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::is_point_inside_my_2d_triangle(const _type &a_point)
{
	if (a_point == this->m_points[0] ||
		a_point == this->m_points[1] ||
		a_point == this->m_points[2])
	{
		return false;
	}

#define SIGN(p1, p2, p3) (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y)

	bool b1 = SIGN(a_point, this->m_points[0], this->m_points[1]) < 0.0;
	bool b2 = SIGN(a_point, this->m_points[1], this->m_points[2]) < 0.0;
	bool b3 = SIGN(a_point, this->m_points[2], this->m_points[0]) < 0.0;

	return ((b1 == b2) && (b2 == b3));
}

template <class _type>
FORCE_INLINE Bezier<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::Bezier(_type a_point1, _type a_control_point, _type a_point2)
{
	this->set(a_point1, a_control_point, a_point2);
}

template <class _type>
FORCE_INLINE void Bezier<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::set(_type a_point1, _type a_control_point, _type a_point2)
{
	this->m_points[0] = a_point1;
	this->m_points[1] = a_control_point;
	this->m_points[2] = a_point2;
}

template <class _type>
FORCE_INLINE void Bezier<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::subdivide(std::vector<Bezier<_type, 2>> &a_children)
{
	_type p1 = this->m_points[0];
	_type p2 = this->m_points[1];
	_type p3 = this->m_points[2];

	_type q1 = (p1 + p2) / 2;
	_type q2 = (p2 + p3) / 2;

	_type r1 = (q1 + q2) / 2;

	a_children.emplace_back(p1, q1, r1);
	a_children.emplace_back(r1, q2, p3);
}

template <class _type>
FORCE_INLINE bool Bezier<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::is_point_inside_my_2d_triangle(const _type &a_point)
{
	if (a_point == this->m_points[0] ||
		a_point == this->m_points[1] ||
		a_point == this->m_points[2])
	{
		return false;
	}

#define SIGN(p1, p2, p3) (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y)

	bool b1 = SIGN(a_point, this->m_points[0], this->m_points[1]) < 0.0;
	bool b2 = SIGN(a_point, this->m_points[1], this->m_points[2]) < 0.0;
	bool b3 = SIGN(a_point, this->m_points[2], this->m_points[0]) < 0.0;

	return ((b1 == b2) && (b2 == b3));
}

template <class _type>
FORCE_INLINE Bezier<_type, 3, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::Bezier(_type a_point1, _type a_control_point1, _type a_control_point2, _type a_point2)
{
	this->set(a_point1, a_control_point1, a_control_point2, a_point2);
}

template <class _type>
FORCE_INLINE void Bezier<_type, 3, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type>::set(_type a_point1, _type a_control_point1, _type a_control_point2, _type a_point2)
{
	this->m_points[0] = a_point1;
	this->m_points[1] = a_control_point1;
	this->m_points[2] = a_control_point2;
	this->m_points[3] = a_point2;
}

template <class _type>
FORCE_INLINE Bezier<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::Bezier(_type a_point1, _type a_control_point, _type a_point2)
{
	this->set(a_point1, a_control_point, a_point2);
}

template <class _type>
FORCE_INLINE void Bezier<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::set(_type a_point1, _type a_control_point, _type a_point2)
{
	this->m_points[0] = a_point1;
	this->m_points[1] = a_control_point;
	this->m_points[2] = a_point2;
}

template <class _type>
FORCE_INLINE void Bezier<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::subdivide(std::vector<Bezier<_type, 2>> &a_children)
{
	_type p1 = this->m_points[0];
	_type p2 = this->m_points[1];
	_type p3 = this->m_points[2];

	_type q1 = (p1 + p2) / 2;
	_type q2 = (p2 + p3) / 2;

	_type r1 = (q1 + q2) / 2;

	a_children.emplace_back(p1, q1, r1);
	a_children.emplace_back(r1, q2, p3);
}

template <class _type>
FORCE_INLINE bool Bezier<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::is_point_inside_my_2d_triangle(const _type &a_point)
{
	if (a_point == this->m_points[0] ||
		a_point == this->m_points[1] ||
		a_point == this->m_points[2])
	{
		return false;
	}

#define SIGN(p1, p2, p3) (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y)

	bool b1 = SIGN(a_point, this->m_points[0], this->m_points[1]) < 0.0;
	bool b2 = SIGN(a_point, this->m_points[1], this->m_points[2]) < 0.0;
	bool b3 = SIGN(a_point, this->m_points[2], this->m_points[0]) < 0.0;

	return ((b1 == b2) && (b2 == b3));
}
template <class _type>
FORCE_INLINE Bezier<_type, 3, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::Bezier(_type a_point1, _type a_control_point1, _type a_control_point2, _type a_point2)
{
	this->set(a_point1, a_control_point1, a_control_point2, a_point2);
}

template <class _type>
FORCE_INLINE void Bezier<_type, 3, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type>::set(_type a_point1, _type a_control_point1, _type a_control_point2, _type a_point2)
{
	this->m_points[0] = a_point1;
	this->m_points[1] = a_control_point1;
	this->m_points[2] = a_control_point2;
	this->m_points[3] = a_point2;
}

}        // namespace ror
