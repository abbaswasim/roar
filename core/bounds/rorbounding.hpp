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

#include "foundation/rortypes.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector3.hpp"

namespace ror
{
enum class BoundingType : int32_t
{
	circle_2d,
	rectangle_2d,
	sphere_3d,
	box_3d
};

enum class CollisionType : int32_t
{
	outside    = -1,
	intersects = 0,
	inside     = 1
};

class ROAR_ENGINE_ITEM Bounding
{
  public:
	BoundingType m_type{};        //!< Bounding type defined by BoundingType enum

	FORCE_INLINE Bounding()                            = default;                   //! Default constructor
	FORCE_INLINE Bounding(const Bounding &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Bounding(Bounding &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Bounding &operator=(const Bounding &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Bounding &operator=(Bounding &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~Bounding() noexcept                     = default;        //! Destructor

	FORCE_INLINE virtual CollisionType collision(const Bounding &a_bounding) const  = 0;
	FORCE_INLINE virtual bool          intersects(const Bounding &a_bounding) const = 0;
	FORCE_INLINE virtual void          add_bounding(const Bounding &a_bounding)     = 0;

  private:
	virtual void _force_compiler_vtable();
};

template <class _type, uint32_t _point_count = 1>
class ROAR_ENGINE_ITEM BoundingBase : public Bounding
{
  public:
	_type m_points[_point_count];        //!< All points in the bounding

	FORCE_INLINE BoundingBase()                                = default;                   //! Default constructor
	FORCE_INLINE BoundingBase(const BoundingBase &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE BoundingBase(BoundingBase &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE BoundingBase &operator=(const BoundingBase &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE BoundingBase &operator=(BoundingBase &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~BoundingBase() noexcept override                = default;        //! Destructor

	// These are defined here and not in Bounding because I can't templatize Bounding, Otherwise
	// I can't cast from Bounding<Vector3> => Bounding<Vector2> etc if I want to create an array of Boundings
	FORCE_INLINE virtual void add_point(const _type &a_point)                       = 0;
	FORCE_INLINE virtual bool is_point_inside(const _type &a_point) const           = 0;
	FORCE_INLINE virtual void create_from_min_max(_type a_minimum, _type a_maximum) = 0;

	void create_from_points(const std::vector<_type> &a_points);
};

template <class _type, typename enable = void>
class ROAR_ENGINE_ITEM Round
{};

template <class _type>
using BoundingSphere = Round<_type, vector3_typename<_type>>;

template <class _type>
using BoundingCircle = Round<_type, vector2_typename<_type>>;

template <class _type, typename enable = void>
class ROAR_ENGINE_ITEM Box
{};

template <class _type>
using BoundingBox = Box<_type, vector3_typename<_type>>;

template <class _type>
using BoundingRectangle = Box<_type, vector2_typename<_type>>;

#define BOUND_COMMON()                                                                                                               \
	FORCE_INLINE CollisionType collision(const Bounding &a_bounding) const override;                                                 \
	FORCE_INLINE bool          intersects(const Bounding &a_bounding) const override;                                                \
	FORCE_INLINE void          add_bounding(const Bounding &a_bounding) override;                                                    \
	FORCE_INLINE void          add_point(const _type &a_point) override;                                                             \
	FORCE_INLINE bool          is_point_inside(const _type &a_point) const override;                                                 \
	FORCE_INLINE void          create_from_min_max(_type a_minimum, _type a_maximum) override;                                       \
	FORCE_INLINE _type         center() const noexcept;                                                                              \
	FORCE_INLINE CollisionType collision(const BoundingSphere<Vector3<typename _type::value_type>> &a_sphere) const noexcept;        \
	FORCE_INLINE CollisionType collision(const BoundingBox<Vector3<typename _type::value_type>> &a_box) const noexcept;              \
	FORCE_INLINE CollisionType collision(const BoundingCircle<Vector2<typename _type::value_type>> &a_circle) const noexcept;        \
	FORCE_INLINE CollisionType collision(const BoundingRectangle<Vector2<typename _type::value_type>> &a_rectangle) const noexcept;  \
	FORCE_INLINE bool          intersects(const BoundingSphere<Vector3<typename _type::value_type>> &a_sphere) const noexcept;       \
	FORCE_INLINE bool          intersects(const BoundingBox<Vector3<typename _type::value_type>> &a_box) const noexcept;             \
	FORCE_INLINE bool          intersects(const BoundingCircle<Vector2<typename _type::value_type>> &a_circle) const noexcept;       \
	FORCE_INLINE bool          intersects(const BoundingRectangle<Vector2<typename _type::value_type>> &a_rectangle) const noexcept; \
	FORCE_INLINE void          add_bounding(const BoundingSphere<Vector3<typename _type::value_type>> &a_sphere);                    \
	FORCE_INLINE void          add_bounding(const BoundingBox<Vector3<typename _type::value_type>> &a_box);                          \
	FORCE_INLINE void          add_bounding(const BoundingCircle<Vector2<typename _type::value_type>> &a_circle);                    \
	FORCE_INLINE void          add_bounding(const BoundingRectangle<Vector2<typename _type::value_type>> &a_rectangle);

// TODO: Use squared radius instead
#define ROUND_COMMON()                                                            \
	typename _type::value_type m_radius{};                                        \
																				  \
	FORCE_INLINE Round();                                                         \
	FORCE_INLINE Round(const Round &a_other)     = default;                       \
	FORCE_INLINE Round(Round &&a_other) noexcept = default;                       \
	FORCE_INLINE Round &operator=(const Round &a_other) = default;                \
	FORCE_INLINE Round &operator=(Round &&a_other) noexcept = default;            \
	FORCE_INLINE ~Round() noexcept override                 = default;            \
	FORCE_INLINE      Round(_type a_center, typename _type::value_type a_radius); \
	FORCE_INLINE void set(_type a_center, typename _type::value_type a_radius);   \
	FORCE_INLINE auto radius() const noexcept->typename _type::value_type;        \
	FORCE_INLINE void set_radius(typename _type::value_type a_radius);            \
	FORCE_INLINE void set_center(_type a_center);                                 \
	BOUND_COMMON()

// Bounding circle declration
template <class _type>
class ROAR_ENGINE_ITEM Round<_type, vector2_typename<_type>> final : public BoundingBase<_type, 1>
{
  public:
	ROUND_COMMON()
};

// Bounding sphere declration
template <class _type>
class ROAR_ENGINE_ITEM Round<_type, vector3_typename<_type>> final : public BoundingBase<_type, 1>
{
  public:
	ROUND_COMMON()
};

#define BOX_COMMON()                                                               \
	FORCE_INLINE Box();                                                            \
	FORCE_INLINE Box(const Box &a_other)     = default;                            \
	FORCE_INLINE Box(Box &&a_other) noexcept = default;                            \
	FORCE_INLINE Box &operator=(const Box &a_other) = default;                     \
	FORCE_INLINE Box &operator=(Box &&a_other) noexcept = default;                 \
	FORCE_INLINE ~Box() noexcept override               = default;                 \
	FORCE_INLINE                            Box(_type a_minimum, _type a_maximum); \
	FORCE_INLINE void                       set(_type a_minimum, _type a_maximum); \
	FORCE_INLINE _type                      minimum() const noexcept;              \
	FORCE_INLINE _type                      maximum() const noexcept;              \
	FORCE_INLINE _type                      extent() const noexcept;               \
	FORCE_INLINE typename _type::value_type diagonal() const noexcept;             \
	BOUND_COMMON()

// Bounding rectangle declration
template <class _type>
class ROAR_ENGINE_ITEM Box<_type, vector2_typename<_type>> final : public BoundingBase<_type, 2>
{
  public:
	BOX_COMMON()
};

// Bounding box declration
template <class _type>
class ROAR_ENGINE_ITEM Box<_type, vector3_typename<_type>> final : public BoundingBase<_type, 2>
{
  public:
	BOX_COMMON()
};

using BoundingRectanglef = Box<Vector2f>;
using BoundingBoxf       = Box<Vector3f>;
using BoundingRectangled = Box<Vector2d>;
using BoundingBoxd       = Box<Vector3d>;
using BoundingRectanglei = Box<Vector2i>;
using BoundingBoxi       = Box<Vector3i>;

using BoundingCirclef = Round<Vector2f>;
using BoundingSpheref = Round<Vector3f>;
using BoundingCircled = Round<Vector2d>;
using BoundingSphered = Round<Vector3d>;
using BoundingCirclei = Round<Vector2i>;
using BoundingSpherei = Round<Vector3i>;

}        // namespace ror

#include "bounds/rorbounding.hh"
