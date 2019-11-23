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

#include "math/rorvector2.hpp"
#include "math/rorvector3.hpp"

namespace ror
{
enum class BoundingType
{
	bounding_type_2d_circle,
	bounding_type_2d_rectangle,
	bounding_type_3d_sphere,
	bounding_type_3d_box,
	bounding_type_max
};

enum class BoundingCollisionType
{
	bounding_collision_type_outside    = -1,
	bounding_collision_type_intersects = 0,
	bounding_collision_type_inside     = 1,
	bounding_collision_type_max        = 4
};

class ROAR_ENGINE_ITEM Bounding
{
  public:
	BoundingType m_type;        //!< Bounding type defined by BoundingType enum

	FORCE_INLINE Bounding()                            = default;                   //! Default constructor
	FORCE_INLINE Bounding(const Bounding &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Bounding(Bounding &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Bounding &operator=(const Bounding &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Bounding &operator=(Bounding &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~Bounding() noexcept                     = default;        //! Destructor

	FORCE_INLINE virtual BoundingCollisionType collision(const Bounding &a_point) const = 0;
	FORCE_INLINE virtual void                  add_bounding(const Bounding &a_point)    = 0;
};

template <class _type, int _point_count = 1>
class ROAR_ENGINE_ITEM BoundingBase : public Bounding
{
  public:
	_type m_points[_point_count];        //!< All points in the bounding

	FORCE_INLINE BoundingBase()                                = default;                   //! Default constructor
	FORCE_INLINE BoundingBase(const BoundingBase &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE BoundingBase(BoundingBase &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE BoundingBase &operator=(const BoundingBase &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE BoundingBase &operator=(BoundingBase &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~BoundingBase() noexcept                         = default;        //! Destructor

	// These are defined here and not in Bounding because I can't templatize Bounding
	// Otherwise I can't cast from Bounding<Vector3> => Bounding<Vector2> etc
	FORCE_INLINE virtual void add_point(const _type &a_point)                       = 0;
	FORCE_INLINE virtual bool is_point_inside(const _type &a_point) const           = 0;
	FORCE_INLINE virtual void create_from_min_max(_type a_minimum, _type a_maximum) = 0;

	void create_from_points(const std::vector<_type> &a_points);
};

template <class _type, typename enable = void>
class ROAR_ENGINE_ITEM Round
{};

template <class _type>
using sphere_typename = Round<_type, vector3_typename<_type>>;

template <class _type>
using circle_typename = Round<_type, vector2_typename<_type>>;

template <class _type, typename enable = void>
class ROAR_ENGINE_ITEM Box
{};

template <class _type>
using box_typename = Box<_type, vector3_typename<_type>>;

template <class _type>
using rectangle_typename = Box<_type, vector2_typename<_type>>;

#define ROUND_COMMON()                                                                                                                       \
	FORCE_INLINE Round();                                                                                                                    \
	FORCE_INLINE Round(const Round &a_other)     = default;                                                                                  \
	FORCE_INLINE Round(Round &&a_other) noexcept = default;                                                                                  \
	FORCE_INLINE Round &operator=(const Round &a_other) = default;                                                                           \
	FORCE_INLINE Round &operator=(Round &&a_other) noexcept = default;                                                                       \
	FORCE_INLINE ~Round() noexcept                          = default;                                                                       \
	FORCE_INLINE                       Round(_type a_center, typename _type::value_type a_radius);                                           \
	FORCE_INLINE void                  set(_type a_center, typename _type::value_type a_radius);                                             \
	FORCE_INLINE auto                  radius() const noexcept->typename _type::value_type;                                                  \
	FORCE_INLINE _type                 center() const noexcept;                                                                              \
	FORCE_INLINE void                  set_radius(typename _type::value_type a_radius);                                                      \
	FORCE_INLINE void                  set_center(_type a_center);                                                                           \
	FORCE_INLINE BoundingCollisionType collision(const sphere_typename<Vector3<typename _type::value_type>> &a_sphere) const noexcept;       \
	FORCE_INLINE BoundingCollisionType collision(const box_typename<Vector3<typename _type::value_type>> &a_box) const noexcept;             \
	FORCE_INLINE BoundingCollisionType collision(const circle_typename<Vector2<typename _type::value_type>> &a_circle) const noexcept;       \
	FORCE_INLINE BoundingCollisionType collision(const rectangle_typename<Vector2<typename _type::value_type>> &a_rectangle) const noexcept; \
	FORCE_INLINE void                  add_bounding(const sphere_typename<Vector3<typename _type::value_type>> &a_sphere);                   \
	FORCE_INLINE void                  add_bounding(const box_typename<Vector3<typename _type::value_type>> &a_box);                         \
	FORCE_INLINE void                  add_bounding(const circle_typename<Vector2<typename _type::value_type>> &a_circle);                   \
	FORCE_INLINE void                  add_bounding(const rectangle_typename<Vector2<typename _type::value_type>> &a_rectangle);             \
	FORCE_INLINE BoundingCollisionType collision(const Bounding &a_point) const override;                                                    \
	FORCE_INLINE void                  add_bounding(const Bounding &a_point) override;                                                       \
	FORCE_INLINE void                  add_point(const _type &a_point) override;                                                             \
	FORCE_INLINE bool                  is_point_inside(const _type &a_point) const override;                                                 \
	FORCE_INLINE void                  create_from_min_max(_type a_minimum, _type a_maximum) override;

template <class _type>
class ROAR_ENGINE_ITEM Round<_type, vector2_typename<_type>> final : public BoundingBase<_type, 1>
{
  public:
	typename _type::value_type m_radius;

	ROUND_COMMON()
};

template <class _type>
class ROAR_ENGINE_ITEM Round<_type, vector3_typename<_type>> final : public BoundingBase<_type, 1>
{
  public:
	typename _type::value_type m_radius;

	ROUND_COMMON()
};

#define BOX_COMMON()                                                                                                                         \
	FORCE_INLINE Box();                                                                                                                      \
	FORCE_INLINE Box(const Box &a_other)     = default;                                                                                      \
	FORCE_INLINE Box(Box &&a_other) noexcept = default;                                                                                      \
	FORCE_INLINE Box &operator=(const Box &a_other) = default;                                                                               \
	FORCE_INLINE Box &operator=(Box &&a_other) noexcept = default;                                                                           \
	FORCE_INLINE ~Box() noexcept                        = default;                                                                           \
	FORCE_INLINE                       Box(_type a_minimum, _type a_maximum);                                                                \
	FORCE_INLINE void                  set(_type a_minimum, _type a_maximum);                                                                \
	FORCE_INLINE _type                 minimum() const noexcept;                                                                             \
	FORCE_INLINE _type                 maximum() const noexcept;                                                                             \
	FORCE_INLINE _type                 center() const noexcept;                                                                              \
	FORCE_INLINE BoundingCollisionType collision(const sphere_typename<Vector3<typename _type::value_type>> &a_sphere) const noexcept;       \
	FORCE_INLINE BoundingCollisionType collision(const box_typename<Vector3<typename _type::value_type>> &a_box) const noexcept;             \
	FORCE_INLINE BoundingCollisionType collision(const circle_typename<Vector2<typename _type::value_type>> &a_circle) const noexcept;       \
	FORCE_INLINE BoundingCollisionType collision(const rectangle_typename<Vector2<typename _type::value_type>> &a_rectangle) const noexcept; \
	FORCE_INLINE void                  add_bounding(const sphere_typename<Vector3<typename _type::value_type>> &a_sphere);                   \
	FORCE_INLINE void                  add_bounding(const box_typename<Vector3<typename _type::value_type>> &a_box);                         \
	FORCE_INLINE void                  add_bounding(const circle_typename<Vector2<typename _type::value_type>> &a_circle);                   \
	FORCE_INLINE void                  add_bounding(const rectangle_typename<Vector2<typename _type::value_type>> &a_rectangle);             \
	FORCE_INLINE BoundingCollisionType collision(const Bounding &a_point) const override;                                                    \
	FORCE_INLINE void                  add_bounding(const Bounding &a_point) override;                                                       \
	FORCE_INLINE void                  add_point(const _type &a_point) override;                                                             \
	FORCE_INLINE bool                  is_point_inside(const _type &a_point) const override;                                                 \
	FORCE_INLINE void                  create_from_min_max(_type a_minimum, _type a_maximum) override;

template <class _type>
class ROAR_ENGINE_ITEM Box<_type, vector2_typename<_type>> final : public BoundingBase<_type, 2>
{
  public:
	BOX_COMMON()
};

template <class _type>
class ROAR_ENGINE_ITEM Box<_type, vector3_typename<_type>> final : public BoundingBase<_type, 2>
{
  public:
	BOX_COMMON()
};

using BoundingRectangleAxisAligned  = Box<Vector2f>;
using BoundingBoxAxisAligned        = Box<Vector3f>;
using BoundingRectangleAxisAlignedd = Box<Vector2d>;
using BoundingBoxAxisAlignedd       = Box<Vector3d>;
using BoundingRectangleAxisAlignedi = Box<Vector2d>;
using BoundingBoxAxisAlignedi       = Box<Vector3d>;

using BoundingCircle  = Round<Vector2f>;
using BoundingSphere  = Round<Vector3f>;
using BoundingCircled = Round<Vector2d>;
using BoundingSphered = Round<Vector3d>;
using BoundingCirclei = Round<Vector2i>;
using BoundingSpherei = Round<Vector3i>;

}        // namespace ror

#include "bounds/rorbounding.hh"
