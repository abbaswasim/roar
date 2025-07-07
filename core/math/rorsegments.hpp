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

#include "math/rorsegment.hpp"
#include "math/rorvector.hpp"

namespace ror
{
/**
 * @brief      Lines, LineStrip and Triangle soups
 * @details    Use this for debugging purposes to draw wire-mesh of objects or render objects
 *             as line or triangle soup
 */

template <class _type>
class ROAR_ENGINE_ITEM Segments
{
  public:
	std::vector<_type> m_points{};        //!< All points in the line or bezier curve, 3D or 2D quadratic or cubic curve points

	FORCE_INLINE           Segments()                             = default;        //! Default constructor
	FORCE_INLINE           Segments(const Segments &a_other)      = default;        //! Copy constructor
	FORCE_INLINE           Segments(Segments &&a_other) noexcept  = default;        //! Move constructor
	FORCE_INLINE Segments &operator=(const Segments &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Segments &operator=(Segments &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~Segments() noexcept                     = default;        //! Destructor

	FORCE_INLINE virtual size_t get_count() const noexcept = 0;
};

template <class _type, typename enable = void>
class ROAR_ENGINE_ITEM Lines final : public Segments<_type>
{};

template <class _type>
class ROAR_ENGINE_ITEM Lines<_type, vector2_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::line_2d;

	FORCE_INLINE        Lines()                             = default;        //! Default constructor
	FORCE_INLINE        Lines(const Lines &a_other)         = default;        //! Copy constructor
	FORCE_INLINE        Lines(Lines &&a_other) noexcept     = default;        //! Move constructor
	FORCE_INLINE Lines &operator=(const Lines &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Lines &operator=(Lines &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Lines() noexcept override                 = default;        //! Destructor

	FORCE_INLINE void   add_line(_type a_point1, _type a_point2);
	FORCE_INLINE size_t get_count() const noexcept override;
};

template <class _type>
class ROAR_ENGINE_ITEM Lines<_type, vector3_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::line_3d;

	FORCE_INLINE        Lines()                             = default;        //! Default constructor
	FORCE_INLINE        Lines(const Lines &a_other)         = default;        //! Copy constructor
	FORCE_INLINE        Lines(Lines &&a_other) noexcept     = default;        //! Move constructor
	FORCE_INLINE Lines &operator=(const Lines &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Lines &operator=(Lines &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Lines() noexcept override                 = default;        //! Destructor

	FORCE_INLINE void   add_line(_type a_point1, _type a_point2);
	FORCE_INLINE size_t get_count() const noexcept override;
};

template <class _type, typename enable = void>
class ROAR_ENGINE_ITEM LineStrip final : public Segments<_type>
{};

template <class _type>
class ROAR_ENGINE_ITEM LineStrip<_type, vector2_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::line_strip_2d;

	FORCE_INLINE            LineStrip()                             = default;        //! Default constructor
	FORCE_INLINE            LineStrip(const LineStrip &a_other)     = default;        //! Copy constructor
	FORCE_INLINE            LineStrip(LineStrip &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE LineStrip &operator=(const LineStrip &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE LineStrip &operator=(LineStrip &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~LineStrip() noexcept override                     = default;        //! Destructor

	FORCE_INLINE void add_line(_type a_point1);
	// FORCE_INLINE void     add_line(_type a_point1, _type a_point2);
	FORCE_INLINE size_t get_count() const noexcept override;
};

template <class _type>
class ROAR_ENGINE_ITEM LineStrip<_type, vector3_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::line_strip_3d;

	FORCE_INLINE            LineStrip()                             = default;        //! Default constructor
	FORCE_INLINE            LineStrip(const LineStrip &a_other)     = default;        //! Copy constructor
	FORCE_INLINE            LineStrip(LineStrip &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE LineStrip &operator=(const LineStrip &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE LineStrip &operator=(LineStrip &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~LineStrip() noexcept override                     = default;        //! Destructor

	FORCE_INLINE void add_line(_type a_point1);
	// FORCE_INLINE void     add_line(_type a_point1, _type a_point2);
	FORCE_INLINE size_t get_count() const noexcept override;
};

template <class _type, typename enable = void>
class ROAR_ENGINE_ITEM Triangles final : public Segments<_type>
{};

template <class _type>
class ROAR_ENGINE_ITEM Triangles<_type, vector2_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::triangle_2d;

	FORCE_INLINE            Triangles()                             = default;        //! Default constructor
	FORCE_INLINE            Triangles(const Triangles &a_other)     = default;        //! Copy constructor
	FORCE_INLINE            Triangles(Triangles &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE Triangles &operator=(const Triangles &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Triangles &operator=(Triangles &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Triangles() noexcept override                     = default;        //! Destructor

	FORCE_INLINE void   add_triangle(_type a_point1, _type a_point2, _type a_point3);
	FORCE_INLINE size_t get_count() const noexcept override;
};

template <class _type>
class ROAR_ENGINE_ITEM Triangles<_type, vector3_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::triangle_3d;

	FORCE_INLINE            Triangles()                             = default;        //! Default constructor
	FORCE_INLINE            Triangles(const Triangles &a_other)     = default;        //! Copy constructor
	FORCE_INLINE            Triangles(Triangles &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE Triangles &operator=(const Triangles &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Triangles &operator=(Triangles &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Triangles() noexcept override                     = default;        //! Destructor

	FORCE_INLINE void   add_triangle(_type a_point1, _type a_point2, _type a_point3);
	FORCE_INLINE size_t get_count() const noexcept override;
};

template <class _type, typename enable = void>
class ROAR_ENGINE_ITEM TriangleStrip final : public Segments<_type>
{};

template <class _type>
class ROAR_ENGINE_ITEM TriangleStrip<_type, vector2_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::triangle_strip_2d;

	FORCE_INLINE                TriangleStrip()                                 = default;        //! Default constructor
	FORCE_INLINE                TriangleStrip(const TriangleStrip &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                TriangleStrip(TriangleStrip &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE TriangleStrip &operator=(const TriangleStrip &a_other)         = default;        //! Copy assignment operator
	FORCE_INLINE TriangleStrip &operator=(TriangleStrip &&a_other) noexcept     = default;        //! Move assignment operator
	FORCE_INLINE ~TriangleStrip() noexcept override                             = default;        //! Destructor

	FORCE_INLINE void add_triangle(_type a_point1);
	// FORCE_INLINE void     add_triangle(_type a_point1, _type a_point2, _type a_point3);
	FORCE_INLINE size_t get_count() const noexcept override;
};

template <class _type>
class ROAR_ENGINE_ITEM TriangleStrip<_type, vector3_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::triangle_strip_3d;

	FORCE_INLINE                TriangleStrip()                                 = default;        //! Default constructor
	FORCE_INLINE                TriangleStrip(const TriangleStrip &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                TriangleStrip(TriangleStrip &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE TriangleStrip &operator=(const TriangleStrip &a_other)         = default;        //! Copy assignment operator
	FORCE_INLINE TriangleStrip &operator=(TriangleStrip &&a_other) noexcept     = default;        //! Move assignment operator
	FORCE_INLINE ~TriangleStrip() noexcept override                             = default;        //! Destructor

	FORCE_INLINE void add_triangle(_type a_point1);
	// FORCE_INLINE void     add_triangle(_type a_point1, _type a_point2, _type a_point3);
	FORCE_INLINE size_t get_count() const noexcept override;
};

template <class _type, typename enable = void>
class ROAR_ENGINE_ITEM QuadraticBeziers final : public Segments<_type>
{};

template <class _type>
class ROAR_ENGINE_ITEM QuadraticBeziers<_type, vector2_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::quadratic_2d_outer;

	FORCE_INLINE                   QuadraticBeziers()                                    = default;        //! Default constructor
	FORCE_INLINE                   QuadraticBeziers(const QuadraticBeziers &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                   QuadraticBeziers(QuadraticBeziers &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE QuadraticBeziers &operator=(const QuadraticBeziers &a_other)            = default;        //! Copy assignment operator
	FORCE_INLINE QuadraticBeziers &operator=(QuadraticBeziers &&a_other) noexcept        = default;        //! Move assignment operator
	FORCE_INLINE ~QuadraticBeziers() noexcept override                                   = default;        //! Destructor

	FORCE_INLINE void   add_bezier(_type a_point1, _type a_point2, _type a_point3);
	FORCE_INLINE size_t get_count() const noexcept override;
};

template <class _type>
class ROAR_ENGINE_ITEM QuadraticBeziers<_type, vector3_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::quadratic_3d_outer;

	FORCE_INLINE                   QuadraticBeziers()                                    = default;        //! Default constructor
	FORCE_INLINE                   QuadraticBeziers(const QuadraticBeziers &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                   QuadraticBeziers(QuadraticBeziers &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE QuadraticBeziers &operator=(const QuadraticBeziers &a_other)            = default;        //! Copy assignment operator
	FORCE_INLINE QuadraticBeziers &operator=(QuadraticBeziers &&a_other) noexcept        = default;        //! Move assignment operator
	FORCE_INLINE ~QuadraticBeziers() noexcept override                                   = default;        //! Destructor

	FORCE_INLINE void   add_bezier(_type a_point1, _type a_point2, _type a_point3);
	FORCE_INLINE size_t get_count() const noexcept override;
};

template <class _type, typename enable = void>
class ROAR_ENGINE_ITEM CubicBeziers final : public Segments<_type>
{};

template <class _type>
class ROAR_ENGINE_ITEM CubicBeziers<_type, vector2_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::cubic_2d;

	FORCE_INLINE               CubicBeziers()                                = default;        //! Default constructor
	FORCE_INLINE               CubicBeziers(const CubicBeziers &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               CubicBeziers(CubicBeziers &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE CubicBeziers &operator=(const CubicBeziers &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE CubicBeziers &operator=(CubicBeziers &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE ~CubicBeziers() noexcept override                           = default;        //! Destructor

	FORCE_INLINE void   add_bezier(_type a_point1, _type a_point2, _type a_point3, _type a_point4);
	FORCE_INLINE size_t get_count() const noexcept override;
};

// TODO: Use Seiler's points when need to draw these Cubic Beziers https://cemyuksel.com/research/seilers_interpolation/
template <class _type>
class ROAR_ENGINE_ITEM CubicBeziers<_type, vector3_typename<_type>> final : public Segments<_type>
{
  public:
	const SegmentType m_type = SegmentType::cubic_3d;

	FORCE_INLINE               CubicBeziers()                                = default;        //! Default constructor
	FORCE_INLINE               CubicBeziers(const CubicBeziers &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               CubicBeziers(CubicBeziers &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE CubicBeziers &operator=(const CubicBeziers &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE CubicBeziers &operator=(CubicBeziers &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE ~CubicBeziers() noexcept override                           = default;        //! Destructor

	FORCE_INLINE void   add_bezier(_type a_point1, _type a_point2, _type a_point3, _type a_point4);
	FORCE_INLINE size_t get_count() const noexcept override;
};

using Lines2f = Lines<ror::Vector2<float32_t>>;
using Lines2d = Lines<ror::Vector2<double64_t>>;
using Lines2i = Lines<ror::Vector2<int32_t>>;
using Lines3f = Lines<ror::Vector3<float32_t>>;
using Lines3d = Lines<ror::Vector3<double64_t>>;
using Lines3i = Lines<ror::Vector3<int32_t>>;

using LineStrip2f = LineStrip<ror::Vector2<float32_t>>;
using LineStrip2d = LineStrip<ror::Vector2<double64_t>>;
using LineStrip2i = LineStrip<ror::Vector2<int32_t>>;
using LineStrip3f = LineStrip<ror::Vector3<float32_t>>;
using LineStrip3d = LineStrip<ror::Vector3<double64_t>>;
using LineStrip3i = LineStrip<ror::Vector3<int32_t>>;

using Triangles2f = Triangles<ror::Vector2<float32_t>>;
using Triangles2d = Triangles<ror::Vector2<double64_t>>;
using Triangles2i = Triangles<ror::Vector2<int32_t>>;
using Triangles3f = Triangles<ror::Vector3<float32_t>>;
using Triangles3d = Triangles<ror::Vector3<double64_t>>;
using Triangles3i = Triangles<ror::Vector3<int32_t>>;

using TriangleStrip2f = TriangleStrip<ror::Vector2<float32_t>>;
using TriangleStrip2d = TriangleStrip<ror::Vector2<double64_t>>;
using TriangleStrip2i = TriangleStrip<ror::Vector2<int32_t>>;
using TriangleStrip3f = TriangleStrip<ror::Vector3<float32_t>>;
using TriangleStrip3d = TriangleStrip<ror::Vector3<double64_t>>;
using TriangleStrip3i = TriangleStrip<ror::Vector3<int32_t>>;

using QuadraticBeizers2f = QuadraticBeziers<ror::Vector2<float32_t>>;
using QuadraticBeizers2d = QuadraticBeziers<ror::Vector2<double64_t>>;
using QuadraticBeizers2i = QuadraticBeziers<ror::Vector2<int32_t>>;
using QuadraticBeizers3f = QuadraticBeziers<ror::Vector3<float32_t>>;
using QuadraticBeizers3d = QuadraticBeziers<ror::Vector3<double64_t>>;
using QuadraticBeizers3i = QuadraticBeziers<ror::Vector3<int32_t>>;

using CubicBeizers2f = CubicBeziers<ror::Vector2<float32_t>>;
using CubicBeizers2d = CubicBeziers<ror::Vector2<double64_t>>;
using CubicBeizers2i = CubicBeziers<ror::Vector2<int32_t>>;
using CubicBeizers3f = CubicBeziers<ror::Vector3<float32_t>>;
using CubicBeizers3d = CubicBeziers<ror::Vector3<double64_t>>;
using CubicBeizers3i = CubicBeziers<ror::Vector3<int32_t>>;

}        // namespace ror

#include "rorsegments.hh"
