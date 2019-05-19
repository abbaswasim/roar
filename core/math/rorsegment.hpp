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

#pragma once

#include "math/rorvector.hpp"

namespace ror
{
enum class SegmentType
{
	segment_type_unknown,
	segment_type_line_2d,
	segment_type_line_3d,
	segment_type_quadratic_2d,
	segment_type_quadratic_3d,
	segment_type_cubic_2d,
	segment_type_cubic_3d,
	segment_type_triangle_2d,
	segment_type_triangle_3d,
	segment_type_max
};

/**
 * @brief      Bezier curves
 * @details    Bezier curve implementation that can be used to represent
 *             Quadratic as well as Cubic bezier curves in 2D or 3D
 */

template <class _type, int _degree>
class ROAR_ENGINE_ITEM Segment
{
  public:
	_type m_points[_degree + 1];        //!< All points in the bezier curve, 3D or 2D quadratic or cubic curve points

	FORCE_INLINE Segment()                           = default;                   //! Default constructor
	FORCE_INLINE Segment(const Segment &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Segment(Segment &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Segment &operator=(const Segment &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Segment &operator=(Segment &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~Segment() noexcept                    = default;        //! Destructor
};

template <class _type, int _degree, typename enable = void>
class ROAR_ENGINE_ITEM Line final : public Segment<_type, _degree>
{
  public:
	FORCE_INLINE Line()                        = default;                   //! Default constructor
	FORCE_INLINE Line(const Line &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Line(Line &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Line &operator=(const Line &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Line &operator=(Line &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Line() noexcept                         = default;        //! Destructor
};

template <class _type>
class ROAR_ENGINE_ITEM Line<_type, 1, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type> final : public Segment<_type, 1>
{
  public:
	const SegmentType m_type = SegmentType::segment_type_line_2d;

	FORCE_INLINE Line()                        = default;                   //! Default constructor
	FORCE_INLINE Line(const Line &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Line(Line &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Line &operator=(const Line &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Line &operator=(Line &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Line() noexcept                         = default;        //! Destructor

	FORCE_INLINE      Line(_type a_point1, _type a_point2);
	FORCE_INLINE      Line(typename _type::value_type a_slope, typename _type::value_type a_intercept);
	FORCE_INLINE void set(_type a_point1, _type a_point2);
	FORCE_INLINE void set(typename _type::value_type a_slope, typename _type::value_type a_intercept);
};

template <class _type>
class ROAR_ENGINE_ITEM Line<_type, 1, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type> final : public Segment<_type, 1>
{
  public:
	const SegmentType m_type = SegmentType::segment_type_line_3d;

	FORCE_INLINE Line()                        = default;                   //! Default constructor
	FORCE_INLINE Line(const Line &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Line(Line &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Line &operator=(const Line &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Line &operator=(Line &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Line() noexcept                         = default;        //! Destructor

	FORCE_INLINE      Line(_type a_point1, _type a_point2);
	FORCE_INLINE      Line(typename _type::value_type a_slope, typename _type::value_type a_intercept);
	FORCE_INLINE void set(_type a_point1, _type a_point2);
	FORCE_INLINE void set(typename _type::value_type a_slope, typename _type::value_type a_intercept);
};

template <class _type, int _degree, typename enable = void>
class ROAR_ENGINE_ITEM Triangle final : public Segment<_type, _degree>
{
  public:
	FORCE_INLINE Triangle()                            = default;                   //! Default constructor
	FORCE_INLINE Triangle(const Triangle &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Triangle(Triangle &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Triangle &operator=(const Triangle &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Triangle &operator=(Triangle &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Triangle() noexcept                             = default;        //! Destructor
};

template <class _type>
class ROAR_ENGINE_ITEM Triangle<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type> final : public Segment<_type, 2>
{
  public:
	const SegmentType m_type = SegmentType::segment_type_triangle_2d;

	FORCE_INLINE Triangle()                            = default;                   //! Default constructor
	FORCE_INLINE Triangle(const Triangle &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Triangle(Triangle &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Triangle &operator=(const Triangle &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Triangle &operator=(Triangle &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Triangle() noexcept                             = default;        //! Destructor

	FORCE_INLINE      Triangle(_type a_point1, _type a_point2, _type a_point3);
	FORCE_INLINE void set(_type a_point1, _type a_point2, _type a_point3);
	FORCE_INLINE bool is_point_inside_my_2d_triangle(const _type &a_point);
};

template <class _type>
class ROAR_ENGINE_ITEM Triangle<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type> final : public Segment<_type, 2>
{
  public:
	const SegmentType m_type = SegmentType::segment_type_triangle_3d;

	FORCE_INLINE Triangle()                            = default;                   //! Default constructor
	FORCE_INLINE Triangle(const Triangle &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Triangle(Triangle &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Triangle &operator=(const Triangle &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Triangle &operator=(Triangle &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Triangle() noexcept                             = default;        //! Destructor

	FORCE_INLINE      Triangle(_type a_point1, _type a_point2, _type a_point3);
	FORCE_INLINE void set(_type a_point1, _type a_point2, _type a_point3);
	FORCE_INLINE bool is_point_inside_my_2d_triangle(const _type &a_point);
};

template <class _type, int _degree, typename enable = void>
class ROAR_ENGINE_ITEM Bezier final : public Segment<_type, _degree>
{
  public:
	FORCE_INLINE Bezier()                          = default;                   //! Default constructor
	FORCE_INLINE Bezier(const Bezier &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Bezier(Bezier &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Bezier &operator=(const Bezier &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Bezier &operator=(Bezier &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Bezier() noexcept                           = default;        //! Destructor
};

template <class _type>
class ROAR_ENGINE_ITEM Bezier<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type> final : public Segment<_type, 2>
{
  public:
	const SegmentType m_type = SegmentType::segment_type_quadratic_2d;

	FORCE_INLINE Bezier()                          = default;                   //! Default constructor
	FORCE_INLINE Bezier(const Bezier &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Bezier(Bezier &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Bezier &operator=(const Bezier &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Bezier &operator=(Bezier &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Bezier() noexcept                           = default;        //! Destructor

	FORCE_INLINE      Bezier(_type a_point1, _type a_control_point, _type a_point2);
	FORCE_INLINE void set(_type a_point1, _type a_control_point, _type a_point2);

	/**
	 * @brief      Subdivides the bezier into two
	 * @details    This method will subdivide the bezier curve in 2 bezier curves
	 *             the new bezier curves are created as P1 Q1 C1 and C1 Q2 P2 where Q1 is center of P1-C1 and Q2 is center of C1-P2
	 * @param      a_children where the result is appended
	 * @return     void
	 */
	FORCE_INLINE void subdivide(std::vector<Bezier<_type, 2>> &a_children);
	FORCE_INLINE bool is_point_inside_my_2d_triangle(const _type &a_point);
};

template <class _type>
class ROAR_ENGINE_ITEM Bezier<_type, 3, typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type> final : public Segment<_type, 3>
{
  public:
	const SegmentType m_type = SegmentType::segment_type_cubic_2d;

	FORCE_INLINE Bezier()                          = default;                   //! Default constructor
	FORCE_INLINE Bezier(const Bezier &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Bezier(Bezier &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Bezier &operator=(const Bezier &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Bezier &operator=(Bezier &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Bezier() noexcept                           = default;        //! Destructor

	FORCE_INLINE      Bezier(_type a_point1, _type a_control_point1, _type a_control_point2, _type a_point2);
	FORCE_INLINE void set(_type a_point1, _type a_control_point1, _type a_control_point2, _type a_point2);
};

template <class _type>
class ROAR_ENGINE_ITEM Bezier<_type, 2, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type> final : public Segment<_type, 2>
{
  public:
	const SegmentType m_type = SegmentType::segment_type_quadratic_3d;

	FORCE_INLINE Bezier()                          = default;                   //! Default constructor
	FORCE_INLINE Bezier(const Bezier &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Bezier(Bezier &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Bezier &operator=(const Bezier &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Bezier &operator=(Bezier &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Bezier() noexcept                           = default;        //! Destructor

	FORCE_INLINE      Bezier(_type a_point1, _type a_control_point, _type a_point2);
	FORCE_INLINE void set(_type a_point1, _type a_control_point, _type a_point2);

	FORCE_INLINE void subdivide(std::vector<Bezier<_type, 2>> &a_children);
	FORCE_INLINE bool is_point_inside_my_2d_triangle(const _type &a_point);
};

template <class _type>
class ROAR_ENGINE_ITEM Bezier<_type, 3, typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type> final : public Segment<_type, 3>
{
  public:
	const SegmentType m_type = SegmentType::segment_type_cubic_3d;

	FORCE_INLINE Bezier()                          = default;                   //! Default constructor
	FORCE_INLINE Bezier(const Bezier &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Bezier(Bezier &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Bezier &operator=(const Bezier &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Bezier &operator=(Bezier &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Bezier() noexcept                           = default;        //! Destructor

	FORCE_INLINE      Bezier(_type a_point1, _type a_control_point1, _type a_control_point2, _type a_point2);
	FORCE_INLINE void set(_type a_point1, _type a_control_point1, _type a_control_point2, _type a_point2);
};

using Line2f = Line<ror::Vector2<float32_t>, 1>;
using Line2d = Line<ror::Vector2<double64_t>, 1>;
using Line2i = Line<ror::Vector2<int32_t>, 1>;
using Line3f = Line<ror::Vector3<float32_t>, 1>;
using Line3d = Line<ror::Vector3<double64_t>, 1>;
using Line3i = Line<ror::Vector3<int32_t>, 1>;

using Triangle2f = Triangle<ror::Vector2<float32_t>, 2>;
using Triangle2d = Triangle<ror::Vector2<double64_t>, 2>;
using Triangle2i = Triangle<ror::Vector2<int32_t>, 2>;
using Triangle3f = Triangle<ror::Vector3<float32_t>, 2>;
using Triangle3d = Triangle<ror::Vector3<double64_t>, 2>;
using Triangle3i = Triangle<ror::Vector3<int32_t>, 2>;

using QuadraticBezier2f = Bezier<ror::Vector2<float32_t>, 2>;
using QuadraticBezier2d = Bezier<ror::Vector2<double64_t>, 2>;
using QuadraticBezier2i = Bezier<ror::Vector2<int32_t>, 2>;
using QuadraticBezier3f = Bezier<ror::Vector3<float32_t>, 2>;
using QuadraticBezier3d = Bezier<ror::Vector3<double64_t>, 2>;
using QuadraticBezier3i = Bezier<ror::Vector3<int32_t>, 2>;

using CubicBezier2f = Bezier<ror::Vector2<float32_t>, 3>;
using CubicBezier2d = Bezier<ror::Vector2<double64_t>, 3>;
using CubicBezier2i = Bezier<ror::Vector2<int32_t>, 3>;
using CubicBezier3f = Bezier<ror::Vector3<float32_t>, 3>;
using CubicBezier3d = Bezier<ror::Vector3<double64_t>, 3>;
using CubicBezier3i = Bezier<ror::Vector3<int32_t>, 3>;

}        // namespace ror

#include "rorsegment.inl"
