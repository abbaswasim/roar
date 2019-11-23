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

#include "bounds/rorbounding.hpp"

namespace ror
{
// Bounding methods
template <class _type, int _points_count>
void BoundingBase<_type, _points_count>::create_from_points(const std::vector<_type> &a_points)
{
	_type minimum, maximum;

	for (auto &p : a_points)
	{
		if (p.x < minimum.x)
		{
			minimum.x = p.x;
		}
		if (p.x > maximum.x)
		{
			maximum.x = p.x;
		}

		if (p.y < minimum.y)
		{
			minimum.y = p.y;
		}
		if (p.y > maximum.y)
		{
			maximum.y = p.y;
		}

		if constexpr (std::is_same<_type, Vector3<typename _type::value_type>>::value)
		{
			if (p.z < minimum.z)
			{
				minimum.z = p.z;
			}
			if (p.z > maximum.z)
			{
				maximum.z = p.z;
			}
		}
	}

	this->create_from_min_max(minimum, maximum);
}

// Bounding sphere code
template <class _type>
FORCE_INLINE Round<_type, vector3_typename<_type>>::Round()
{
	this->set(_type(0.0, 0.0, 0.0), static_cast<typename _type::value_type>(0.0));
}

template <class _type>
FORCE_INLINE Round<_type, vector3_typename<_type>>::Round(_type a_center, typename _type::value_type a_radius)
{
	this->set(a_center, a_radius);
}

template <class _type>
FORCE_INLINE void Round<_type, vector3_typename<_type>>::set(_type a_center, typename _type::value_type a_radius)
{
	this->m_type      = BoundingType::bounding_type_3d_sphere;
	this->m_points[0] = a_center;
	this->m_radius    = a_radius;
}

template <class _type>
FORCE_INLINE auto Round<_type, vector3_typename<_type>>::radius() const noexcept -> typename _type::value_type
{
	return this->m_radius;
}

template <class _type>
FORCE_INLINE _type Round<_type, vector3_typename<_type>>::center() const noexcept
{
	return this->m_points[0];
}

template <class _type>
FORCE_INLINE void Round<_type, vector3_typename<_type>>::set_radius(typename _type::value_type a_radius)
{
	this->m_radius = a_radius;
}

template <class _type>
FORCE_INLINE void Round<_type, vector3_typename<_type>>::set_center(_type a_center)
{
	this->m_points[0] = a_center;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Round<_type, vector3_typename<_type>>::collision(const sphere_typename<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	auto center_to_center_distance = distance(this->m_points[0], a_bounding.m_points[0]);
	if (center_to_center_distance > this->m_radius + a_bounding.m_radius)
	{
		return BoundingCollisionType::bounding_collision_type_outside;
	}
	else if (center_to_center_distance + a_bounding.m_radius <= this->m_radius)
	{
		return BoundingCollisionType::bounding_collision_type_inside;
	}

	return BoundingCollisionType::bounding_collision_type_intersects;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Round<_type, vector3_typename<_type>>::collision(const box_typename<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	auto closest = vector_maximum(a_bounding.m_points[0], vector_minimum(this->m_points[0], a_bounding.m_points[1]));

	auto dist = distance(this->m_points[0], closest);

	if (dist < this->m_radius)
		return BoundingCollisionType::bounding_collision_type_intersects;
	else
		return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Round<_type, vector3_typename<_type>>::collision(const circle_typename<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	Vector3<typename _type::value_type> circle_center_3d(a_bounding.m_points[0]);

	auto center_to_center_distance = distance(this->m_points[0], circle_center_3d);

	if (center_to_center_distance > this->m_radius + a_bounding.m_radius)
	{
		return BoundingCollisionType::bounding_collision_type_outside;
	}
	else if (center_to_center_distance + a_bounding.m_radius <= this->m_radius)
	{
		return BoundingCollisionType::bounding_collision_type_inside;
	}

	return BoundingCollisionType::bounding_collision_type_intersects;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Round<_type, vector3_typename<_type>>::collision(const rectangle_typename<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	Vector3<typename _type::value_type> rec_min(a_bounding.m_points[0]);
	Vector3<typename _type::value_type> rec_max(a_bounding.m_points[1]);

	auto closest = vector_maximum(rec_min, vector_minimum(this->m_points[0], rec_max));

	auto dist = distance(this->m_points[0], closest);

	if (dist < this->m_radius)
		return BoundingCollisionType::bounding_collision_type_intersects;
	else
		return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE void Round<_type, vector3_typename<_type>>::add_bounding(const sphere_typename<Vector3<typename _type::value_type>> &a_sphere)
{
	auto direction_to_center = a_sphere.m_points[0] - this->m_points[0];

	// Distance of a_sphere to the center of the sphere
	auto center_to_center_distance = direction_to_center.length();

	if (center_to_center_distance + a_sphere.m_radius < this->m_radius || center_to_center_distance + this->m_radius < a_sphere.m_radius)
	{
		if (this->m_radius < a_sphere.m_radius)
		{
			this->m_radius    = a_sphere.m_radius;
			this->m_points[0] = a_sphere.m_points[0];
		}
	}
	else
	{
		auto old_radius = this->m_radius;
		this->m_radius  = (center_to_center_distance + this->m_radius + a_sphere.m_radius) * static_cast<ror_precision<_type>>(0.5f);
		if (center_to_center_distance > ror::ror_epsilon)
		{
			this->m_points[0] = this->m_points[0] + direction_to_center * ((this->m_radius - old_radius) / center_to_center_distance);
		}
	}
}

template <class _type>
FORCE_INLINE void Round<_type, vector3_typename<_type>>::add_bounding(const box_typename<Vector3<typename _type::value_type>> &a_bounding)
{
	this->add_point(a_bounding.m_points[0]);
	this->add_point(a_bounding.m_points[1]);
}

template <class _type>
FORCE_INLINE void Round<_type, vector3_typename<_type>>::add_bounding(const circle_typename<Vector2<typename _type::value_type>> &a_bounding)
{
	Vector3<typename _type::value_type> a_bounding_m_points_0_(a_bounding.m_points[0]);

	auto direction_to_center = a_bounding_m_points_0_ - this->m_points[0];

	// Distance of a_boundingto the center of the sphere
	auto center_to_center_distance = direction_to_center.length();

	if (center_to_center_distance + a_bounding.m_radius < this->m_radius || center_to_center_distance + this->m_radius < a_bounding.m_radius)
	{
		if (this->m_radius < a_bounding.m_radius)
		{
			this->m_radius    = a_bounding.m_radius;
			this->m_points[0] = a_bounding_m_points_0_;
		}
	}
	else
	{
		auto old_radius = this->m_radius;
		this->m_radius  = (center_to_center_distance + this->m_radius + a_bounding.m_radius) * static_cast<ror_precision<_type>>(0.5f);
		if (center_to_center_distance > ror::ror_epsilon)
		{
			this->m_points[0] = this->m_points[0] + direction_to_center * ((this->m_radius - old_radius) / center_to_center_distance);
		}
	}
}

template <class _type>
FORCE_INLINE void Round<_type, vector3_typename<_type>>::add_bounding(const rectangle_typename<Vector2<typename _type::value_type>> &a_bounding)
{
	this->add_point(Vector3<typename _type::value_type>(a_bounding.m_points[0], static_cast<typename _type::value_type>(0)));
	this->add_point(Vector3<typename _type::value_type>(a_bounding.m_points[1], static_cast<typename _type::value_type>(0)));
}

template <class _type>
FORCE_INLINE BoundingCollisionType Round<_type, vector3_typename<_type>>::collision(const Bounding &a_bounding) const
{
	if (BoundingType::bounding_type_3d_sphere == this->m_type)
	{
		return this->collision(static_cast<const sphere_typename<_type> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_3d_box == this->m_type)
	{
		return this->collision(static_cast<const box_typename<_type> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_circle == this->m_type)
	{
		return this->collision(static_cast<const circle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_rectangle == this->m_type)
	{
		return this->collision(static_cast<const rectangle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}

	return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE void Round<_type, vector3_typename<_type>>::add_bounding(const Bounding &a_bounding)
{
	if (BoundingType::bounding_type_3d_sphere == this->m_type)
	{
		return this->add_bounding(static_cast<const sphere_typename<_type> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_3d_box == this->m_type)
	{
		return this->add_bounding(static_cast<const box_typename<_type> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_circle == this->m_type)
	{
		return this->add_bounding(static_cast<const circle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_rectangle == this->m_type)
	{
		return this->add_bounding(static_cast<const rectangle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
}

template <class _type>
FORCE_INLINE void Round<_type, vector3_typename<_type>>::add_point(const _type &a_point)
{
	auto direction_to_point = a_point - this->m_points[0];

	// Distance of a_point to the center of the sphere
	auto center_to_point_distance = direction_to_point.length();

	// Consider only if the point is outside the current sphere
	if (center_to_point_distance > this->m_radius)
	{
		auto old_radius   = this->m_radius;
		this->m_radius    = (center_to_point_distance + this->m_radius) * static_cast<ror_precision<_type>>(0.5f);
		this->m_points[0] = this->m_points[0] + direction_to_point * ((this->m_radius - old_radius) / center_to_point_distance);
	}
}

template <class _type>
FORCE_INLINE bool Round<_type, vector3_typename<_type>>::is_point_inside(const _type &a_point) const
{
	// Perhaps one day use radius * radius against squared distance
	auto center_to_point_distance = distance(this->m_points[0], a_point);

	return (center_to_point_distance < this->m_radius);
}

template <class _type>
FORCE_INLINE void Round<_type, vector3_typename<_type>>::create_from_min_max(_type a_minimum, _type a_maximum)
{
	this->m_points[0] = (a_minimum + a_maximum) * 0.5f;        // This will be the midpoint
	this->m_radius    = distance(a_minimum, a_maximum) * 0.5f;
}

// Bounding circle code
template <class _type>
FORCE_INLINE Round<_type, vector2_typename<_type>>::Round()
{
	this->set(_type(0.0, 0.0), static_cast<typename _type::value_type>(0.0));
}

template <class _type>
FORCE_INLINE Round<_type, vector2_typename<_type>>::Round(_type a_center, typename _type::value_type a_radius)
{
	this->set(a_center, a_radius);
}

template <class _type>
FORCE_INLINE void Round<_type, vector2_typename<_type>>::set(_type a_center, typename _type::value_type a_radius)
{
	this->m_type      = BoundingType::bounding_type_2d_circle;
	this->m_points[0] = a_center;
	this->m_radius    = a_radius;
}

template <class _type>
FORCE_INLINE auto Round<_type, vector2_typename<_type>>::radius() const noexcept -> typename _type::value_type
{
	return this->m_radius;
}

template <class _type>
FORCE_INLINE _type Round<_type, vector2_typename<_type>>::center() const noexcept
{
	return this->m_points[0];
}

template <class _type>
FORCE_INLINE void Round<_type, vector2_typename<_type>>::set_radius(typename _type::value_type a_radius)
{
	this->m_radius = a_radius;
}

template <class _type>
FORCE_INLINE void Round<_type, vector2_typename<_type>>::set_center(_type a_center)
{
	this->m_points[0] = a_center;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Round<_type, vector2_typename<_type>>::collision(const sphere_typename<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	Vector2<_type> sphere_center_2d(a_bounding.m_points[0]);

	auto center_to_center_distance = distance(this->m_points[0], sphere_center_2d);

	if (center_to_center_distance > this->m_radius + a_bounding.m_radius)
	{
		return BoundingCollisionType::bounding_collision_type_outside;
	}
	else if (center_to_center_distance + a_bounding.m_radius <= this->m_radius)
	{
		return BoundingCollisionType::bounding_collision_type_inside;
	}

	return BoundingCollisionType::bounding_collision_type_intersects;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Round<_type, vector2_typename<_type>>::collision(const box_typename<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	Vector2<_type> box_min(a_bounding.m_points[0]);
	Vector2<_type> box_max(a_bounding.m_points[1]);

	auto closest = vector_maximum(box_min, vector_minimum(this->m_points[0], box_max));

	auto dist = distance(this->m_points[0], closest);

	if (dist < this->m_radius)
		return BoundingCollisionType::bounding_collision_type_intersects;
	else
		return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Round<_type, vector2_typename<_type>>::collision(const circle_typename<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	auto center_to_center_distance = distance(this->m_points[0], a_bounding.m_points[0]);

	if (center_to_center_distance > this->m_radius + a_bounding.m_radius)
	{
		return BoundingCollisionType::bounding_collision_type_outside;
	}
	else if (center_to_center_distance + a_bounding.m_radius <= this->m_radius)
	{
		return BoundingCollisionType::bounding_collision_type_inside;
	}

	return BoundingCollisionType::bounding_collision_type_intersects;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Round<_type, vector2_typename<_type>>::collision(const rectangle_typename<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	auto closest = vector_maximum(a_bounding.m_points[0], vector_minimum(this->m_points[0], a_bounding.m_points[1]));

	auto dist = distance(this->m_points[0], closest);

	if (dist < this->m_radius)
		return BoundingCollisionType::bounding_collision_type_intersects;
	else
		return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE void Round<_type, vector2_typename<_type>>::add_bounding(const sphere_typename<Vector3<typename _type::value_type>> &a_sphere)
{
	Vector3<typename _type::value_type> sphere_center(a_sphere.m_points[0]);
	auto                                direction_to_center = sphere_center - this->m_points[0];

	// Distance of a_sphere to the center of the sphere
	auto center_to_center_distance = direction_to_center.length();

	if (center_to_center_distance + a_sphere.m_radius < this->m_radius || center_to_center_distance + this->m_radius < a_sphere.m_radius)
	{
		if (this->m_radius < a_sphere.m_radius)
		{
			this->m_radius    = a_sphere.m_radius;
			this->m_points[0] = sphere_center;
		}
	}
	else
	{
		auto old_radius = this->m_radius;
		this->m_radius  = (center_to_center_distance + this->m_radius + a_sphere.m_radius) * static_cast<ror_precision<_type>>(0.5f);
		if (center_to_center_distance > ror::ror_epsilon)
		{
			this->m_points[0] = this->m_points[0] + direction_to_center * ((this->m_radius - old_radius) / center_to_center_distance);
		}
	}
}

template <class _type>
FORCE_INLINE void Round<_type, vector2_typename<_type>>::add_bounding(const box_typename<Vector3<typename _type::value_type>> &a_bounding)
{
	this->add_point(Vector2<typename _type::value_type>(a_bounding.m_points[0]));
	this->add_point(Vector2<typename _type::value_type>(a_bounding.m_points[1]));
}

template <class _type>
FORCE_INLINE void Round<_type, vector2_typename<_type>>::add_bounding(const circle_typename<Vector2<typename _type::value_type>> &a_circle)
{
	auto direction_to_center = a_circle.m_points[0] - this->m_points[0];

	// Distance of a_circleto the center of the sphere
	auto center_to_center_distance = direction_to_center.length();

	if (center_to_center_distance + a_circle.m_radius < this->m_radius || center_to_center_distance + this->m_radius < a_circle.m_radius)
	{
		if (this->m_radius < a_circle.m_radius)
		{
			this->m_radius    = a_circle.m_radius;
			this->m_points[0] = a_circle.m_points[0];
		}
	}
	else
	{
		auto old_radius = this->m_radius;
		this->m_radius  = (center_to_center_distance + this->m_radius + a_circle.m_radius) * static_cast<ror_precision<_type>>(0.5f);
		if (center_to_center_distance > ror::ror_epsilon)
		{
			this->m_points[0] = this->m_points[0] + direction_to_center * ((this->m_radius - old_radius) / center_to_center_distance);
		}
	}
}

template <class _type>
FORCE_INLINE void Round<_type, vector2_typename<_type>>::add_bounding(const rectangle_typename<Vector2<typename _type::value_type>> &a_bounding)
{
	this->add_point(a_bounding.m_points[0]);
	this->add_point(a_bounding.m_points[1]);
}

template <class _type>
FORCE_INLINE BoundingCollisionType Round<_type, vector2_typename<_type>>::collision(const Bounding &a_bounding) const
{
	if (BoundingType::bounding_type_3d_sphere == this->m_type)
	{
		return this->collision(static_cast<const sphere_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_3d_box == this->m_type)
	{
		return this->collision(static_cast<const box_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_circle == this->m_type)
	{
		return this->collision(static_cast<const circle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_rectangle == this->m_type)
	{
		return this->collision(static_cast<const rectangle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}

	return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE void Round<_type, vector2_typename<_type>>::add_bounding(const Bounding &a_bounding)
{
	if (BoundingType::bounding_type_3d_sphere == this->m_type)
	{
		return this->add_bounding(static_cast<const sphere_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_3d_box == this->m_type)
	{
		return this->add_bounding(static_cast<const box_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_circle == this->m_type)
	{
		return this->add_bounding(static_cast<const circle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_rectangle == this->m_type)
	{
		return this->add_bounding(static_cast<const rectangle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
}

template <class _type>
FORCE_INLINE void Round<_type, vector2_typename<_type>>::add_point(const _type &a_point)
{
	auto direction_to_point = a_point - this->m_points[0];

	// Distance of a_point to the center of the sphere
	auto center_to_point_distance = direction_to_point.length();

	// Consider only if the point is outside the current sphere
	if (center_to_point_distance > this->m_radius)
	{
		auto old_radius   = this->m_radius;
		this->m_radius    = (center_to_point_distance + this->m_radius) * static_cast<ror_precision<_type>>(0.5f);
		this->m_points[0] = this->m_points[0] + direction_to_point * ((this->m_radius - old_radius) / center_to_point_distance);
	}
}

template <class _type>
FORCE_INLINE bool Round<_type, vector2_typename<_type>>::is_point_inside(const _type &a_point) const
{
	// Perhaps one day use radius * radius against squared distance
	auto center_to_point_distance = distance(this->m_points[0], a_point);

	return (center_to_point_distance < this->m_radius);
}

template <class _type>
FORCE_INLINE void Round<_type, vector2_typename<_type>>::create_from_min_max(_type a_minimum, _type a_maximum)
{
	this->m_points[0] = (a_minimum + a_maximum) * 0.5f;        // This will be the midpoint
	this->m_radius    = distance(a_minimum, a_maximum) * 0.5f;
}

// Bounding rectangle code
template <class _type>
FORCE_INLINE Box<_type, vector2_typename<_type>>::Box()
{
	// TODO Use min and max range of _type
	this->set(_type(0.0, 0.0), _type(0.0, 0.0));
}

template <class _type>
FORCE_INLINE Box<_type, vector2_typename<_type>>::Box(_type a_minimum, _type a_maximum)
{
	this->set(a_minimum, a_maximum);
}

template <class _type>
FORCE_INLINE void Box<_type, vector2_typename<_type>>::set(_type a_minimum, _type a_maximum)
{
	this->m_type      = BoundingType::bounding_type_2d_rectangle;
	this->m_points[0] = a_minimum;
	this->m_points[1] = a_maximum;
}

template <class _type>
FORCE_INLINE _type Box<_type, vector2_typename<_type>>::minimum() const noexcept
{
	return this->m_points[0];
}

template <class _type>
FORCE_INLINE _type Box<_type, vector2_typename<_type>>::maximum() const noexcept
{
	return this->m_points[1];
}

template <class _type>
FORCE_INLINE _type Box<_type, vector2_typename<_type>>::center() const noexcept
{
	return this->m_points[0] + this->m_points[1] * static_cast<ror::ror_precision<_type>>(0.5f);
}

template <class _type>
FORCE_INLINE BoundingCollisionType Box<_type, vector2_typename<_type>>::collision(const sphere_typename<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	Vector2<typename _type::value_type> a_bounding_m_points_0_(a_bounding.m_points[0]);
	Vector2<typename _type::value_type> a_bounding_m_points_1_(a_bounding.m_points[1]);

	auto closest = vector_maximum(this->m_points[0], vector_minimum(a_bounding_m_points_0_, this->m_points[1]));

	auto dist = distance(a_bounding_m_points_0_, closest);

	if (dist < a_bounding.m_radius)
		return BoundingCollisionType::bounding_collision_type_intersects;
	else
		return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Box<_type, vector2_typename<_type>>::collision(const box_typename<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y ||
		a_bounding.m_points[0].z > 0 || 0 > a_bounding.m_points[1].z)
	{
		return BoundingCollisionType::bounding_collision_type_outside;
	}

	return BoundingCollisionType::bounding_collision_type_intersects;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Box<_type, vector2_typename<_type>>::collision(const circle_typename<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	auto closest = vector_maximum(this->m_points[0], vector_minimum(a_bounding.m_points[0], this->m_points[1]));

	auto dist = distance(a_bounding.m_points[0], closest);

	if (dist < a_bounding.m_radius)
		return BoundingCollisionType::bounding_collision_type_intersects;
	else
		return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Box<_type, vector2_typename<_type>>::collision(const rectangle_typename<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y)
	{
		return BoundingCollisionType::bounding_collision_type_outside;
	}

	return BoundingCollisionType::bounding_collision_type_intersects;
}

template <class _type>
FORCE_INLINE void Box<_type, vector2_typename<_type>>::add_bounding(const sphere_typename<Vector3<typename _type::value_type>> &a_sphere)
{
	_type min = a_sphere.m_points[0] - _type(a_sphere.m_radius);
	_type max = a_sphere.m_points[0] + _type(a_sphere.m_radius);

	this->add_point(Vector2<typename _type::value_type>(min));
	this->add_point(Vector2<typename _type::value_type>(max));
}

template <class _type>
FORCE_INLINE void Box<_type, vector2_typename<_type>>::add_bounding(const box_typename<Vector3<typename _type::value_type>> &a_bounding)
{
	if (a_bounding.m_points[0].x < this->m_points[0].x)
	{
		this->m_points[0].x = a_bounding.m_points[0].x;
	}
	if (a_bounding.m_points[1].x > this->m_points[1].x)
	{
		this->m_points[1].x = a_bounding.m_points[1].x;
	}
	if (a_bounding.m_points[0].y < this->m_points[0].y)
	{
		this->m_points[0].y = a_bounding.m_points[0].y;
	}
	if (a_bounding.m_points[1].y > this->m_points[1].y)
	{
		this->m_points[1].y = a_bounding.m_points[1].y;
	}
}

template <class _type>
FORCE_INLINE void Box<_type, vector2_typename<_type>>::add_bounding(const circle_typename<Vector2<typename _type::value_type>> &a_circle)
{
	_type min = a_circle.m_points[0] - _type(a_circle.m_radius);
	_type max = a_circle.m_points[0] + _type(a_circle.m_radius);

	this->add_point(min);
	this->add_point(max);
}

template <class _type>
FORCE_INLINE void Box<_type, vector2_typename<_type>>::add_bounding(const rectangle_typename<Vector2<typename _type::value_type>> &a_bounding)
{
	this->add_point(a_bounding.m_points[0]);
	this->add_point(a_bounding.m_points[1]);
}

template <class _type>
FORCE_INLINE BoundingCollisionType Box<_type, vector2_typename<_type>>::collision(const Bounding &a_bounding) const
{
	if (BoundingType::bounding_type_3d_sphere == this->m_type)
	{
		return this->collision(static_cast<const sphere_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_3d_box == this->m_type)
	{
		return this->collision(static_cast<const box_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_circle == this->m_type)
	{
		return this->collision(static_cast<const circle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_rectangle == this->m_type)
	{
		return this->collision(static_cast<const rectangle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}

	return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE void Box<_type, vector2_typename<_type>>::add_bounding(const Bounding &a_bounding)
{
	if (BoundingType::bounding_type_3d_sphere == this->m_type)
	{
		return this->add_bounding(static_cast<const sphere_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_3d_box == this->m_type)
	{
		return this->add_bounding(static_cast<const box_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_circle == this->m_type)
	{
		return this->add_bounding(static_cast<const circle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_rectangle == this->m_type)
	{
		return this->add_bounding(static_cast<const rectangle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
}

template <class _type>
FORCE_INLINE void Box<_type, vector2_typename<_type>>::add_point(const _type &a_point)
{
	if (a_point.x < this->m_points[0].x)
	{
		this->m_points[0].x = a_point.x;
	}
	if (a_point.x > this->m_points[1].x)
	{
		this->m_points[1].x = a_point.x;
	}
	if (a_point.y < this->m_points[0].y)
	{
		this->m_points[0].y = a_point.y;
	}
	if (a_point.y > this->m_points[1].y)
	{
		this->m_points[1].y = a_point.y;
	}
}

template <class _type>
FORCE_INLINE bool Box<_type, vector2_typename<_type>>::is_point_inside(const _type &a_point) const
{
	return !(a_point.x < this->m_points[0].x || a_point.x > this->m_points[1].x ||
			 a_point.y < this->m_points[0].y || a_point.y > this->m_points[1].y);
}

template <class _type>
FORCE_INLINE void Box<_type, vector2_typename<_type>>::create_from_min_max(_type a_minimum, _type a_maximum)
{
	this->m_points[0] = a_minimum;
	this->m_points[1] = a_maximum;
}

// Bounding Box code
template <class _type>
FORCE_INLINE Box<_type, vector3_typename<_type>>::Box()
{
	// TODO Use min and max range of _type
	this->set(_type(0.0, 0.0, 0.0), _type(0.0, 0.0, 0.0));
}

template <class _type>
FORCE_INLINE Box<_type, vector3_typename<_type>>::Box(_type a_minimum, _type a_maximum)
{
	this->set(a_minimum, a_maximum);
}

template <class _type>
FORCE_INLINE void Box<_type, vector3_typename<_type>>::set(_type a_minimum, _type a_maximum)
{
	this->m_type      = BoundingType::bounding_type_3d_box;
	this->m_points[0] = a_minimum;
	this->m_points[1] = a_maximum;
}

template <class _type>
FORCE_INLINE _type Box<_type, vector3_typename<_type>>::minimum() const noexcept
{
	return this->m_points[0];
}

template <class _type>
FORCE_INLINE _type Box<_type, vector3_typename<_type>>::maximum() const noexcept
{
	return this->m_points[1];
}

template <class _type>
FORCE_INLINE _type Box<_type, vector3_typename<_type>>::center() const noexcept
{
	return this->m_points[0] + this->m_points[1] * static_cast<ror::ror_precision<_type>>(0.5f);
}

template <class _type>
FORCE_INLINE BoundingCollisionType Box<_type, vector3_typename<_type>>::collision(const sphere_typename<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	auto closest = vector_maximum(this->m_points[0], vector_minimum(a_bounding.m_points[0], this->m_points[1]));

	auto dist = distance(a_bounding.m_points[0], closest);

	if (dist < a_bounding.m_radius)
		return BoundingCollisionType::bounding_collision_type_intersects;
	else
		return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Box<_type, vector3_typename<_type>>::collision(const box_typename<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y ||
		a_bounding.m_points[0].z > this->m_points[1].z || this->m_points[0].z > a_bounding.m_points[1].z)
	{
		return BoundingCollisionType::bounding_collision_type_outside;
	}

	return BoundingCollisionType::bounding_collision_type_intersects;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Box<_type, vector3_typename<_type>>::collision(const circle_typename<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	Vector3<typename _type::value_type> a_bounding_m_points_0_(a_bounding.m_points[0]);

	auto closest = vector_maximum(this->m_points[0], vector_minimum(a_bounding_m_points_0_, this->m_points[1]));

	auto dist = distance(a_bounding_m_points_0_, closest);

	if (dist < a_bounding.m_radius)
		return BoundingCollisionType::bounding_collision_type_intersects;
	else
		return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE BoundingCollisionType Box<_type, vector3_typename<_type>>::collision(const rectangle_typename<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y ||
		0 > this->m_points[1].z || this->m_points[0].z > 0)
	{
		return BoundingCollisionType::bounding_collision_type_outside;
	}

	return BoundingCollisionType::bounding_collision_type_intersects;
}

template <class _type>
FORCE_INLINE void Box<_type, vector3_typename<_type>>::add_bounding(const sphere_typename<Vector3<typename _type::value_type>> &a_sphere)
{
	_type min = a_sphere.m_points[0] - _type(a_sphere.m_radius);
	_type max = a_sphere.m_points[0] + _type(a_sphere.m_radius);

	this->add_point(min);
	this->add_point(max);
}

template <class _type>
FORCE_INLINE void Box<_type, vector3_typename<_type>>::add_bounding(const box_typename<Vector3<typename _type::value_type>> &a_bounding)
{
	if (a_bounding.m_points[0].x < this->m_points[0].x)
	{
		this->m_points[0].x = a_bounding.m_points[0].x;
	}
	if (a_bounding.m_points[1].x > this->m_points[1].x)
	{
		this->m_points[1].x = a_bounding.m_points[1].x;
	}
	if (a_bounding.m_points[0].y < this->m_points[0].y)
	{
		this->m_points[0].y = a_bounding.m_points[0].y;
	}
	if (a_bounding.m_points[1].y > this->m_points[1].y)
	{
		this->m_points[1].y = a_bounding.m_points[1].y;
	}
	if (a_bounding.m_points[0].z < this->m_points[0].z)
	{
		this->m_points[0].z = a_bounding.m_points[0].z;
	}
	if (a_bounding.m_points[1].z > this->m_points[1].z)
	{
		this->m_points[1].z = a_bounding.m_points[1].z;
	}
}

template <class _type>
FORCE_INLINE void Box<_type, vector3_typename<_type>>::add_bounding(const circle_typename<Vector2<typename _type::value_type>> &a_circle)
{
	auto min = a_circle.m_points[0] - Vector2<typename _type::value_type>(a_circle.m_radius);
	auto max = a_circle.m_points[0] + Vector2<typename _type::value_type>(a_circle.m_radius);

	this->add_point(Vector3<typename _type::value_type>(min));
	this->add_point(Vector3<typename _type::value_type>(max));
}

template <class _type>
FORCE_INLINE void Box<_type, vector3_typename<_type>>::add_bounding(const rectangle_typename<Vector2<typename _type::value_type>> &a_bounding)
{
	this->add_point(Vector3<typename _type::value_type>(a_bounding.m_points[0]));
	this->add_point(Vector3<typename _type::value_type>(a_bounding.m_points[1]));
}

template <class _type>
FORCE_INLINE BoundingCollisionType Box<_type, vector3_typename<_type>>::collision(const Bounding &a_bounding) const
{
	if (BoundingType::bounding_type_3d_sphere == this->m_type)
	{
		return this->collision(static_cast<const sphere_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_3d_box == this->m_type)
	{
		return this->collision(static_cast<const box_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_circle == this->m_type)
	{
		return this->collision(static_cast<const circle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_rectangle == this->m_type)
	{
		return this->collision(static_cast<const rectangle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}

	return BoundingCollisionType::bounding_collision_type_outside;
}

template <class _type>
FORCE_INLINE void Box<_type, vector3_typename<_type>>::add_bounding(const Bounding &a_bounding)
{
	if (BoundingType::bounding_type_3d_sphere == this->m_type)
	{
		return this->add_bounding(static_cast<const sphere_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_3d_box == this->m_type)
	{
		return this->add_bounding(static_cast<const box_typename<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_circle == this->m_type)
	{
		return this->add_bounding(static_cast<const circle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::bounding_type_2d_rectangle == this->m_type)
	{
		return this->add_bounding(static_cast<const rectangle_typename<Vector2<typename _type::value_type>> &>(a_bounding));
	}
}

template <class _type>
FORCE_INLINE void Box<_type, vector3_typename<_type>>::add_point(const _type &a_point)
{
	if (a_point.x < this->m_points[0].x)
	{
		this->m_points[0].x = a_point.x;
	}
	if (a_point.x > this->m_points[1].x)
	{
		this->m_points[1].x = a_point.x;
	}
	if (a_point.y < this->m_points[0].y)
	{
		this->m_points[0].y = a_point.y;
	}
	if (a_point.y > this->m_points[1].y)
	{
		this->m_points[1].y = a_point.y;
	}
	if (a_point.z < this->m_points[0].z)
	{
		this->m_points[0].z = a_point.z;
	}
	if (a_point.z > this->m_points[1].z)
	{
		this->m_points[1].z = a_point.z;
	}
}

template <class _type>
FORCE_INLINE bool Box<_type, vector3_typename<_type>>::is_point_inside(const _type &a_point) const
{
	return !(a_point.x < this->m_points[0].x || a_point.x > this->m_points[1].x ||
			 a_point.y < this->m_points[0].y || a_point.y > this->m_points[1].y ||
			 a_point.z < this->m_points[0].z || a_point.z > this->m_points[1].z);
}

template <class _type>
FORCE_INLINE void Box<_type, vector3_typename<_type>>::create_from_min_max(_type a_minimum, _type a_maximum)
{
	this->m_points[0] = a_minimum;
	this->m_points[1] = a_maximum;
}

}        // namespace ror
