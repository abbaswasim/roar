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
#include <iostream>
#include <sys/types.h>

namespace ror
{
void Bounding::_force_compiler_vtable()
{}

// Bounding methods
template <class _type, int _points_count>
void BoundingBase<_type, _points_count>::create_from_points(const std::vector<_type> &a_points)
{
	auto min = std::numeric_limits<typename _type::value_type>::min();
	auto max = std::numeric_limits<typename _type::value_type>::max();

	_type minimum(max), maximum(min);

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

	// FIXME: This is not efficient for Sphere creation, its conservative but night not be a big issue
	this->create_from_min_max(minimum, maximum);
}

// Bounding sphere code
template <class _type>
FORCE_INLINE BoundingSphere<_type>::Round()
{
	this->set(_type(0.0, 0.0, 0.0), static_cast<typename _type::value_type>(0.0));
}

template <class _type>
FORCE_INLINE BoundingSphere<_type>::Round(_type a_center, typename _type::value_type a_radius)
{
	this->set(a_center, a_radius);
}

template <class _type>
FORCE_INLINE void BoundingSphere<_type>::set(_type a_center, typename _type::value_type a_radius)
{
	this->m_type      = BoundingType::sphere_3d;
	this->m_points[0] = a_center;
	this->m_radius    = a_radius;
}

template <class _type>
FORCE_INLINE auto BoundingSphere<_type>::radius() const noexcept -> typename _type::value_type
{
	return this->m_radius;
}

template <class _type>
FORCE_INLINE _type BoundingSphere<_type>::center() const noexcept
{
	return this->m_points[0];
}

template <class _type>
FORCE_INLINE void BoundingSphere<_type>::set_radius(typename _type::value_type a_radius)
{
	this->m_radius = a_radius;
}

template <class _type>
FORCE_INLINE void BoundingSphere<_type>::set_center(_type a_center)
{
	this->m_points[0] = a_center;
}

template <class _type>
FORCE_INLINE CollisionType BoundingSphere<_type>::collision(const BoundingSphere<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	auto center_to_center_distance = distance(this->m_points[0], a_bounding.m_points[0]);
	if (center_to_center_distance > this->m_radius + a_bounding.m_radius)
	{
		return CollisionType::outside;
	}
	else if (center_to_center_distance + a_bounding.m_radius <= this->m_radius)
	{
		return CollisionType::inside;
	}

	return CollisionType::intersects;
}

template <class _type>
FORCE_INLINE CollisionType BoundingSphere<_type>::collision(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	// auto closest = vector_maximum(a_bounding.m_points[0], vector_minimum(this->m_points[0], a_bounding.m_points[1]));
	auto closest = vector_clamp(this->m_points[0], a_bounding.m_points[0], a_bounding.m_points[1]);

	auto dist = distance(this->m_points[0], closest);

	if (dist < this->m_radius)
	{
		if (distance(this->m_points[0], a_bounding.m_points[0]) < this->m_radius && distance(this->m_points[0], a_bounding.m_points[1]) < this->m_radius)
		{
			return CollisionType::inside;
		}
		else
		{
			return CollisionType::intersects;
		}
	}
	else
	{
		return CollisionType::outside;
	}
}

template <class _type>
FORCE_INLINE CollisionType BoundingSphere<_type>::collision(const BoundingCircle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	Vector3<typename _type::value_type> circle_center_3d(a_bounding.m_points[0]);

	auto center_to_center_distance = distance(this->m_points[0], circle_center_3d);

	if ((center_to_center_distance > this->m_radius + a_bounding.m_radius) || (std::abs(this->m_points[0].z) > this->m_radius))
	{
		return CollisionType::outside;
	}
	else if (center_to_center_distance + a_bounding.m_radius <= this->m_radius)
	{
		return CollisionType::inside;
	}

	return CollisionType::intersects;
}

template <class _type>
FORCE_INLINE CollisionType BoundingSphere<_type>::collision(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	Vector3<typename _type::value_type> rec_min(a_bounding.m_points[0]);
	Vector3<typename _type::value_type> rec_max(a_bounding.m_points[1]);

	auto closest = vector_clamp(this->m_points[0], rec_min, rec_max);
	// auto closest = vector_maximum(rec_min, vector_minimum(this->m_points[0], rec_max));

	auto dist = distance(this->m_points[0], closest);

	if (dist < this->m_radius)
	{
		if (this->m_radius > distance(this->m_points[0], rec_min) && this->m_radius > distance(this->m_points[0], rec_max))
		{
			return CollisionType::inside;
		}

		return CollisionType::intersects;
	}

	return CollisionType::outside;
}

template <class _type>
FORCE_INLINE bool BoundingSphere<_type>::intersects(const BoundingSphere<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	auto center_to_center_distance = distance(this->m_points[0], a_bounding.m_points[0]);
	if (center_to_center_distance > this->m_radius + a_bounding.m_radius)
	{
		return false;
	}

	return true;
}

template <class _type>
FORCE_INLINE bool BoundingSphere<_type>::intersects(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	auto closest = vector_clamp(this->m_points[0], a_bounding.m_points[0], a_bounding.m_points[1]);
	auto dist    = distance(this->m_points[0], closest);

	if (dist < this->m_radius)
	{
		return true;
	}

	return false;
}

template <class _type>
FORCE_INLINE bool BoundingSphere<_type>::intersects(const BoundingCircle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	Vector3<typename _type::value_type> circle_center_3d(a_bounding.m_points[0]);

	auto center_to_center_distance = distance(this->m_points[0], circle_center_3d);

	if ((center_to_center_distance > this->m_radius + a_bounding.m_radius) || (std::abs(this->m_points[0].z) > this->m_radius))
	{
		return false;
	}

	return true;
}

template <class _type>
FORCE_INLINE bool BoundingSphere<_type>::intersects(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	Vector3<typename _type::value_type> rec_min(a_bounding.m_points[0]);
	Vector3<typename _type::value_type> rec_max(a_bounding.m_points[1]);

	auto closest = vector_clamp(this->m_points[0], rec_min, rec_max);
	auto dist    = distance(this->m_points[0], closest);

	if (dist < this->m_radius)
	{
		return true;
	}

	return false;
}

template <class _type>
FORCE_INLINE void BoundingSphere<_type>::add_bounding(const BoundingSphere<Vector3<typename _type::value_type>> &a_sphere)
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
		this->m_radius  = static_cast<typename _type::value_type>((center_to_center_distance + this->m_radius + a_sphere.m_radius) * static_cast<ror_precision<typename _type::value_type>>(0.5f));
		if (center_to_center_distance > ror::ror_epsilon)
		{
			this->m_points[0] = this->m_points[0] + direction_to_center * static_cast<typename _type::value_type>((this->m_radius - old_radius) / center_to_center_distance);
		}
	}
}

template <class _type>
FORCE_INLINE void BoundingSphere<_type>::add_bounding(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding)
{
	this->add_point(a_bounding.m_points[0]);
	this->add_point(a_bounding.m_points[1]);
}

template <class _type>
FORCE_INLINE void BoundingSphere<_type>::add_bounding(const BoundingCircle<Vector2<typename _type::value_type>> &a_bounding)
{
	Vector3<typename _type::value_type> a_bounding_m_points_0_(a_bounding.m_points[0]);

	auto direction_to_center = a_bounding_m_points_0_ - this->m_points[0];

	// Distance of a_bounding to the center of the sphere
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
		this->m_radius  = static_cast<typename _type::value_type>((center_to_center_distance + this->m_radius + a_bounding.m_radius) * static_cast<ror_precision<typename _type::value_type>>(0.5f));
		if (center_to_center_distance > ror::ror_epsilon)
		{
			this->m_points[0] = this->m_points[0] + direction_to_center * static_cast<typename _type::value_type>((this->m_radius - old_radius) / center_to_center_distance);
		}
	}
}

template <class _type>
FORCE_INLINE void BoundingSphere<_type>::add_bounding(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding)
{
	this->add_point(Vector3<typename _type::value_type>(a_bounding.m_points[0], static_cast<typename _type::value_type>(0)));
	this->add_point(Vector3<typename _type::value_type>(a_bounding.m_points[1], static_cast<typename _type::value_type>(0)));
}

template <class _type>
FORCE_INLINE CollisionType BoundingSphere<_type>::collision(const Bounding &a_bounding) const
{
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingSphere<_type> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingBox<_type> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingCircle<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingRectangle<Vector2<typename _type::value_type>> &>(a_bounding));
	}

	return CollisionType::outside;
}

template <class _type>
FORCE_INLINE bool BoundingSphere<_type>::intersects(const Bounding &a_bounding) const
{
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingSphere<_type> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingBox<_type> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingCircle<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingRectangle<Vector2<typename _type::value_type>> &>(a_bounding));
	}

	return false;
}

template <class _type>
FORCE_INLINE void BoundingSphere<_type>::add_bounding(const Bounding &a_bounding)
{
	// TODO: write tests
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingSphere<_type> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingBox<_type> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingCircle<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingRectangle<Vector2<typename _type::value_type>> &>(a_bounding));
	}
}

template <class _type>
FORCE_INLINE void BoundingSphere<_type>::add_point(const _type &a_point)
{
	auto direction_to_point = a_point - this->m_points[0];

	// Distance of a_point to the center of the sphere
	auto center_to_point_distance = direction_to_point.length();

	// Consider only if the point is outside the current sphere
	if (center_to_point_distance > this->m_radius)
	{
		auto old_radius = this->m_radius;
		auto new_radius = (center_to_point_distance + this->m_radius) * static_cast<ror_precision<typename _type::value_type>>(0.5f);

		// Could call normalized but length is already calculated so lets use it
		Vector3<ror_precision<typename _type::value_type>> normalized_direction(direction_to_point.x / center_to_point_distance,
																				direction_to_point.y / center_to_point_distance,
																				direction_to_point.z / center_to_point_distance);

		auto radius_diff = (new_radius - old_radius);

		this->m_points[0] = this->m_points[0] + _type(static_cast<typename _type::value_type>(normalized_direction.x * radius_diff),
													  static_cast<typename _type::value_type>(normalized_direction.y * radius_diff),
													  static_cast<typename _type::value_type>(normalized_direction.z * radius_diff));

		this->m_radius = static_cast<typename _type::value_type>(new_radius);
	}
}

template <class _type>
FORCE_INLINE bool BoundingSphere<_type>::is_point_inside(const _type &a_point) const
{
	// Perhaps one day use radius * radius against squared distance
	auto center_to_point_distance = distance(this->m_points[0], a_point);

	return !(center_to_point_distance > this->m_radius);
}

template <class _type>
FORCE_INLINE void BoundingSphere<_type>::create_from_min_max(_type a_minimum, _type a_maximum)
{
	this->m_points[0] = (a_minimum + a_maximum) / 2;
	this->m_radius    = static_cast<typename _type::value_type>(distance(a_minimum, this->m_points[0]));
}

// Bounding circle code
template <class _type>
FORCE_INLINE BoundingCircle<_type>::Round()
{
	this->set(_type(0.0, 0.0), static_cast<typename _type::value_type>(0.0));
}

template <class _type>
FORCE_INLINE BoundingCircle<_type>::Round(_type a_center, typename _type::value_type a_radius)
{
	this->set(a_center, a_radius);
}

template <class _type>
FORCE_INLINE void BoundingCircle<_type>::set(_type a_center, typename _type::value_type a_radius)
{
	this->m_type      = BoundingType::circle_2d;
	this->m_points[0] = a_center;
	this->m_radius    = a_radius;
}

template <class _type>
FORCE_INLINE auto BoundingCircle<_type>::radius() const noexcept -> typename _type::value_type
{
	return this->m_radius;
}

template <class _type>
FORCE_INLINE _type BoundingCircle<_type>::center() const noexcept
{
	return this->m_points[0];
}

template <class _type>
FORCE_INLINE void BoundingCircle<_type>::set_radius(typename _type::value_type a_radius)
{
	this->m_radius = a_radius;
}

template <class _type>
FORCE_INLINE void BoundingCircle<_type>::set_center(_type a_center)
{
	this->m_points[0] = a_center;
}

template <class _type>
FORCE_INLINE CollisionType BoundingCircle<_type>::collision(const BoundingSphere<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	// Note collision is performed in 3D
	Vector3<typename _type::value_type> circle_center_3d(this->m_points[0]);

	auto center_to_center_distance = distance(a_bounding.m_points[0], circle_center_3d);

	if ((center_to_center_distance > this->m_radius + a_bounding.m_radius) || (std::abs(a_bounding.m_points[0].z) > a_bounding.m_radius))
	{
		return CollisionType::outside;
	}
	else if (center_to_center_distance + a_bounding.m_radius <= this->m_radius)
	{
		return CollisionType::inside;
	}

	return CollisionType::intersects;
}

template <class _type>
FORCE_INLINE CollisionType BoundingCircle<_type>::collision(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	// Note collision is performed in 3D
	Vector3<typename _type::value_type> circle_center(this->m_points[0]);

	// auto closest = vector_maximum(box_min, vector_minimum(this->m_points[0], box_max));
	auto closest = vector_clamp(circle_center, a_bounding.m_points[0], a_bounding.m_points[1]);
	auto dist    = distance(circle_center, closest);

	if (dist < this->m_radius && ror::equal_zero(closest.z))
	{
		if (distance(circle_center, a_bounding.m_points[0]) < this->m_radius &&
			distance(circle_center, a_bounding.m_points[1]) < this->m_radius)
		{
			return CollisionType::inside;
		}
		else
		{
			return CollisionType::intersects;
		}
	}
	else
	{
		return CollisionType::outside;
	}
}

template <class _type>
FORCE_INLINE CollisionType BoundingCircle<_type>::collision(const BoundingCircle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	auto center_to_center_distance = distance(this->m_points[0], a_bounding.m_points[0]);

	if (center_to_center_distance > this->m_radius + a_bounding.m_radius)
	{
		return CollisionType::outside;
	}
	else if (center_to_center_distance + a_bounding.m_radius <= this->m_radius)
	{
		return CollisionType::inside;
	}

	return CollisionType::intersects;
}

template <class _type>
FORCE_INLINE CollisionType BoundingCircle<_type>::collision(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	auto closest = vector_clamp(this->m_points[0], a_bounding.m_points[0], a_bounding.m_points[1]);
	auto dist    = distance(this->m_points[0], closest);

	if (dist < this->m_radius)
	{
		if (distance(this->m_points[0], a_bounding.m_points[0]) < this->m_radius && distance(this->m_points[0], a_bounding.m_points[1]) < this->m_radius)
		{
			return CollisionType::inside;
		}
		else
		{
			return CollisionType::intersects;
		}
	}
	else
		return CollisionType::outside;
}

template <class _type>
FORCE_INLINE bool BoundingCircle<_type>::intersects(const BoundingSphere<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	// Note collision is performed in 3D
	Vector3<typename _type::value_type> circle_center_3d(this->m_points[0]);

	auto center_to_center_distance = distance(a_bounding.m_points[0], circle_center_3d);

	if ((center_to_center_distance > this->m_radius + a_bounding.m_radius) || (std::abs(a_bounding.m_points[0].z) > a_bounding.m_radius))
	{
		return false;
	}

	return true;
}

template <class _type>
FORCE_INLINE bool BoundingCircle<_type>::intersects(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	// Note collision is performed in 3D
	Vector3<typename _type::value_type> circle_center(this->m_points[0]);

	auto closest = vector_clamp(circle_center, a_bounding.m_points[0], a_bounding.m_points[1]);
	auto dist    = distance(circle_center, closest);

	if (dist < this->m_radius && ror::equal_zero(closest.z))
	{
		return true;
	}

	return false;
}

template <class _type>
FORCE_INLINE bool BoundingCircle<_type>::intersects(const BoundingCircle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	auto center_to_center_distance = distance(this->m_points[0], a_bounding.m_points[0]);

	if (center_to_center_distance > this->m_radius + a_bounding.m_radius)
	{
		return false;
	}

	return true;
}

template <class _type>
FORCE_INLINE bool BoundingCircle<_type>::intersects(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	auto closest = vector_clamp(this->m_points[0], a_bounding.m_points[0], a_bounding.m_points[1]);
	auto dist    = distance(this->m_points[0], closest);

	if (dist < this->m_radius)
	{
		return true;
	}

	return false;
}

template <class _type>
FORCE_INLINE void BoundingCircle<_type>::add_bounding(const BoundingSphere<Vector3<typename _type::value_type>> &a_sphere)
{
	Vector2<typename _type::value_type> sphere_center(a_sphere.m_points[0]);

	auto direction_to_center = sphere_center - this->m_points[0];

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
		this->m_radius  = static_cast<typename _type::value_type>((center_to_center_distance + this->m_radius + a_sphere.m_radius) * static_cast<ror_precision<typename _type::value_type>>(0.5f));
		if (center_to_center_distance > ror::ror_epsilon)
		{
			this->m_points[0] = this->m_points[0] + direction_to_center * static_cast<typename _type::value_type>((this->m_radius - old_radius) / center_to_center_distance);
		}
	}
}

template <class _type>
FORCE_INLINE void BoundingCircle<_type>::add_bounding(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding)
{
	this->add_point(Vector2<typename _type::value_type>(a_bounding.m_points[0]));
	this->add_point(Vector2<typename _type::value_type>(a_bounding.m_points[1]));
}

template <class _type>
FORCE_INLINE void BoundingCircle<_type>::add_bounding(const BoundingCircle<Vector2<typename _type::value_type>> &a_circle)
{
	auto direction_to_center = a_circle.m_points[0] - this->m_points[0];

	// Distance of a_circle to the center of the sphere
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

		this->m_radius = static_cast<typename _type::value_type>((center_to_center_distance + this->m_radius + a_circle.m_radius) * static_cast<ror_precision<typename _type::value_type>>(0.5f));
		if (center_to_center_distance > ror::ror_epsilon)
		{
			this->m_points[0] = this->m_points[0] + direction_to_center * static_cast<typename _type::value_type>((this->m_radius - old_radius) / center_to_center_distance);
		}
	}
}

template <class _type>
FORCE_INLINE void BoundingCircle<_type>::add_bounding(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding)
{
	this->add_point(a_bounding.m_points[0]);
	this->add_point(a_bounding.m_points[1]);
}

template <class _type>
FORCE_INLINE CollisionType BoundingCircle<_type>::collision(const Bounding &a_bounding) const
{
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingSphere<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingBox<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingCircle<_type> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingRectangle<_type> &>(a_bounding));
	}

	return CollisionType::outside;
}

template <class _type>
FORCE_INLINE bool BoundingCircle<_type>::intersects(const Bounding &a_bounding) const
{
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingSphere<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingBox<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingCircle<_type> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingRectangle<_type> &>(a_bounding));
	}

	return false;
}

template <class _type>
FORCE_INLINE void BoundingCircle<_type>::add_bounding(const Bounding &a_bounding)
{
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingSphere<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingBox<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingCircle<_type> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingRectangle<_type> &>(a_bounding));
	}
}

template <class _type>
FORCE_INLINE void BoundingCircle<_type>::add_point(const _type &a_point)
{
	auto direction_to_point = a_point - this->m_points[0];

	// Distance of a_point to the center of the sphere
	auto center_to_point_distance = direction_to_point.length();

	// Consider only if the point is outside the current sphere
	if (center_to_point_distance > this->m_radius)
	{
		auto old_radius = this->m_radius;
		auto new_radius = (center_to_point_distance + this->m_radius) * static_cast<ror_precision<typename _type::value_type>>(0.5f);

		// Could call normalized but length is already calculated so lets use it
		Vector2<ror_precision<typename _type::value_type>> normalized_direction(direction_to_point.x / center_to_point_distance,
																				direction_to_point.y / center_to_point_distance);

		auto radius_diff = (new_radius - old_radius);

		this->m_points[0] = this->m_points[0] + _type(static_cast<typename _type::value_type>(normalized_direction.x * radius_diff),
													  static_cast<typename _type::value_type>(normalized_direction.y * radius_diff));

		this->m_radius = static_cast<typename _type::value_type>(new_radius);
	}
}

template <class _type>
FORCE_INLINE bool BoundingCircle<_type>::is_point_inside(const _type &a_point) const
{
	// Perhaps one day use radius * radius against squared distance
	auto center_to_point_distance = distance(this->m_points[0], a_point);

	return !(center_to_point_distance > this->m_radius);
}

template <class _type>
FORCE_INLINE void BoundingCircle<_type>::create_from_min_max(_type a_minimum, _type a_maximum)
{
	this->m_points[0] = (a_minimum + a_maximum) / 2;        // This will be the midpoint
	this->m_radius    = static_cast<typename _type::value_type>(distance(a_minimum, this->m_points[0]));
}

// Bounding rectangle code
template <class _type>
FORCE_INLINE BoundingRectangle<_type>::Box()
{
	auto min = std::numeric_limits<typename _type::value_type>::min();
	auto max = std::numeric_limits<typename _type::value_type>::max();

	this->set(_type(max, max), _type(min, min));
}

template <class _type>
FORCE_INLINE BoundingRectangle<_type>::Box(_type a_minimum, _type a_maximum)
{
	this->set(a_minimum, a_maximum);
}

template <class _type>
FORCE_INLINE void BoundingRectangle<_type>::set(_type a_minimum, _type a_maximum)
{
	this->m_type      = BoundingType::rectangle_2d;
	this->m_points[0] = a_minimum;
	this->m_points[1] = a_maximum;
}

template <class _type>
FORCE_INLINE _type BoundingRectangle<_type>::minimum() const noexcept
{
	return this->m_points[0];
}

template <class _type>
FORCE_INLINE _type BoundingRectangle<_type>::maximum() const noexcept
{
	return this->m_points[1];
}

template <class _type>
FORCE_INLINE _type BoundingRectangle<_type>::extent() const noexcept
{
	return this->m_points[1] - this->m_points[0];
}

template <class _type>
FORCE_INLINE typename _type::value_type BoundingRectangle<_type>::diagonal() const noexcept
{
	return length(this->extent());
}

template <class _type>
FORCE_INLINE _type BoundingRectangle<_type>::center() const noexcept
{
	return (this->m_points[0] + this->m_points[1]) * static_cast<ror::ror_precision<typename _type::value_type>>(0.5f);
}

template <class _type>
FORCE_INLINE CollisionType BoundingRectangle<_type>::collision(const BoundingSphere<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	// Collision is done in 3D
	Vector3<typename _type::value_type> points_0(this->m_points[0]);
	Vector3<typename _type::value_type> points_1(this->m_points[1]);

	auto closest = vector_clamp(a_bounding.m_points[0], points_0, points_1);
	auto dist    = distance(a_bounding.m_points[0], closest);

	if (dist < a_bounding.m_radius)
	{
		// This check done in 2D
		if (this->m_points[0].x <= a_bounding.m_points[0].x - a_bounding.m_radius &&
			this->m_points[1].x >= a_bounding.m_points[0].x + a_bounding.m_radius &&
			this->m_points[0].y <= a_bounding.m_points[0].y - a_bounding.m_radius &&
			this->m_points[1].y >= a_bounding.m_points[0].y + a_bounding.m_radius)
		{
			return CollisionType::inside;
		}

		return CollisionType::intersects;
	}

	return CollisionType::outside;
}

template <class _type>
FORCE_INLINE CollisionType BoundingRectangle<_type>::collision(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y ||
		a_bounding.m_points[0].z > 0 || 0 > a_bounding.m_points[1].z)
	{
		return CollisionType::outside;
	}
	else if (this->m_points[0].x <= a_bounding.m_points[0].x && a_bounding.m_points[1].x <= this->m_points[1].x &&
			 this->m_points[0].y <= a_bounding.m_points[0].y && a_bounding.m_points[1].y <= this->m_points[1].y &&
			 0 > a_bounding.m_points[0].z && a_bounding.m_points[1].z > 0)
	{
		return CollisionType::inside;
	}

	return CollisionType::intersects;
}

template <class _type>
FORCE_INLINE CollisionType BoundingRectangle<_type>::collision(const BoundingCircle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	auto closest = vector_clamp(a_bounding.m_points[0], this->m_points[0], this->m_points[1]);
	auto dist    = distance(a_bounding.m_points[0], closest);

	if (dist < a_bounding.m_radius)
	{
		if (this->m_points[0].x <= a_bounding.m_points[0].x - a_bounding.m_radius &&
			this->m_points[1].x >= a_bounding.m_points[0].x + a_bounding.m_radius &&
			this->m_points[0].y <= a_bounding.m_points[0].y - a_bounding.m_radius &&
			this->m_points[1].y >= a_bounding.m_points[0].y + a_bounding.m_radius)
		{
			return CollisionType::inside;
		}

		return CollisionType::intersects;
	}

	return CollisionType::outside;
}

template <class _type>
FORCE_INLINE CollisionType BoundingRectangle<_type>::collision(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y)
	{
		return CollisionType::outside;
	}
	else if (a_bounding.m_points[0].x >= this->m_points[0].x && this->m_points[1].x >= a_bounding.m_points[1].x &&
			 a_bounding.m_points[0].y >= this->m_points[0].y && this->m_points[1].y >= a_bounding.m_points[1].y)
	{
		return CollisionType::inside;
	}

	return CollisionType::intersects;
}

template <class _type>
FORCE_INLINE bool BoundingRectangle<_type>::intersects(const BoundingSphere<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	// Collision is done in 3D
	Vector3<typename _type::value_type> points_0(this->m_points[0]);
	Vector3<typename _type::value_type> points_1(this->m_points[1]);

	auto closest = vector_clamp(a_bounding.m_points[0], points_0, points_1);
	auto dist    = distance(a_bounding.m_points[0], closest);

	if (dist < a_bounding.m_radius)
		return true;

	return false;
}

template <class _type>
FORCE_INLINE bool BoundingRectangle<_type>::intersects(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y ||
		a_bounding.m_points[0].z > 0 || 0 > a_bounding.m_points[1].z)
	{
		return false;
	}

	return true;
}

template <class _type>
FORCE_INLINE bool BoundingRectangle<_type>::intersects(const BoundingCircle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	// auto closest = vector_maximum(this->m_points[0], vector_minimum(a_bounding.m_points[0], this->m_points[1]));
	auto closest = vector_clamp(a_bounding.m_points[0], this->m_points[0], this->m_points[1]);
	auto dist    = distance(a_bounding.m_points[0], closest);

	if (dist < a_bounding.m_radius)
	{
		return true;
	}

	return false;
}

template <class _type>
FORCE_INLINE bool BoundingRectangle<_type>::intersects(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y)
	{
		return false;
	}

	return true;
}

template <class _type>
FORCE_INLINE void BoundingRectangle<_type>::add_bounding(const BoundingSphere<Vector3<typename _type::value_type>> &a_sphere)
{
	_type min = _type(a_sphere.m_points[0]) - _type(a_sphere.m_radius);
	_type max = _type(a_sphere.m_points[0]) + _type(a_sphere.m_radius);

	this->add_point(Vector2<typename _type::value_type>(min));
	this->add_point(Vector2<typename _type::value_type>(max));
}

template <class _type>
FORCE_INLINE void BoundingRectangle<_type>::add_bounding(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding)
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
FORCE_INLINE void BoundingRectangle<_type>::add_bounding(const BoundingCircle<Vector2<typename _type::value_type>> &a_circle)
{
	_type min = a_circle.m_points[0] - _type(a_circle.m_radius);
	_type max = a_circle.m_points[0] + _type(a_circle.m_radius);

	this->add_point(min);
	this->add_point(max);
}

template <class _type>
FORCE_INLINE void BoundingRectangle<_type>::add_bounding(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding)
{
	this->add_point(a_bounding.m_points[0]);
	this->add_point(a_bounding.m_points[1]);
}

template <class _type>
FORCE_INLINE CollisionType BoundingRectangle<_type>::collision(const Bounding &a_bounding) const
{
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingSphere<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingBox<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingCircle<_type> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingRectangle<_type> &>(a_bounding));
	}

	return CollisionType::outside;
}

template <class _type>
FORCE_INLINE bool BoundingRectangle<_type>::intersects(const Bounding &a_bounding) const
{
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingSphere<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingBox<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingCircle<_type> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingRectangle<_type> &>(a_bounding));
	}

	return false;
}

template <class _type>
FORCE_INLINE void BoundingRectangle<_type>::add_bounding(const Bounding &a_bounding)
{
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingSphere<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingBox<Vector3<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingCircle<_type> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingRectangle<_type> &>(a_bounding));
	}
}

template <class _type>
FORCE_INLINE void BoundingRectangle<_type>::add_point(const _type &a_point)
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
FORCE_INLINE bool BoundingRectangle<_type>::is_point_inside(const _type &a_point) const
{
	return !(a_point.x < this->m_points[0].x || a_point.x > this->m_points[1].x ||
			 a_point.y < this->m_points[0].y || a_point.y > this->m_points[1].y);
}

template <class _type>
FORCE_INLINE void BoundingRectangle<_type>::create_from_min_max(_type a_minimum, _type a_maximum)
{
	this->m_points[0] = a_minimum;
	this->m_points[1] = a_maximum;
}

// Bounding Box code
template <class _type>
FORCE_INLINE BoundingBox<_type>::Box()
{
	auto min = std::numeric_limits<typename _type::value_type>::min();
	auto max = std::numeric_limits<typename _type::value_type>::max();

	this->set(_type(max, max, max), _type(min, min, min));
}

template <class _type>
FORCE_INLINE BoundingBox<_type>::Box(_type a_minimum, _type a_maximum)
{
	this->set(a_minimum, a_maximum);
}

template <class _type>
FORCE_INLINE void BoundingBox<_type>::set(_type a_minimum, _type a_maximum)
{
	this->m_type      = BoundingType::box_3d;
	this->m_points[0] = a_minimum;
	this->m_points[1] = a_maximum;
}

template <class _type>
FORCE_INLINE _type BoundingBox<_type>::minimum() const noexcept
{
	return this->m_points[0];
}

template <class _type>
FORCE_INLINE _type BoundingBox<_type>::maximum() const noexcept
{
	return this->m_points[1];
}

template <class _type>
FORCE_INLINE _type BoundingBox<_type>::extent() const noexcept
{
	return this->m_points[1] - this->m_points[0];
}

template <class _type>
FORCE_INLINE typename _type::value_type BoundingBox<_type>::diagonal() const noexcept
{
	return length(this->extent());
}

template <class _type>
FORCE_INLINE _type BoundingBox<_type>::center() const noexcept
{
	return (this->m_points[0] + this->m_points[1]) * static_cast<ror::ror_precision<typename _type::value_type>>(0.5f);
}

template <class _type>
FORCE_INLINE CollisionType BoundingBox<_type>::collision(const BoundingSphere<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	auto closest = vector_clamp(a_bounding.m_points[0], this->m_points[0], this->m_points[1]);
	auto dist    = distance(a_bounding.m_points[0], closest);

	if (dist < a_bounding.m_radius)
	{
		if (this->m_points[0].x <= a_bounding.m_points[0].x - a_bounding.m_radius &&
			this->m_points[1].x >= a_bounding.m_points[0].x + a_bounding.m_radius &&
			this->m_points[0].y <= a_bounding.m_points[0].y - a_bounding.m_radius &&
			this->m_points[1].y >= a_bounding.m_points[0].y + a_bounding.m_radius &&
			this->m_points[0].z <= a_bounding.m_points[0].z - a_bounding.m_radius &&
			this->m_points[1].z >= a_bounding.m_points[0].z + a_bounding.m_radius)
		{
			return CollisionType::inside;
		}
		else
		{
			return CollisionType::intersects;
		}
	}
	else
	{
		return CollisionType::outside;
	}
}

template <class _type>
FORCE_INLINE CollisionType BoundingBox<_type>::collision(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y ||
		a_bounding.m_points[0].z > this->m_points[1].z || this->m_points[0].z > a_bounding.m_points[1].z)
	{
		return CollisionType::outside;
	}
	else if (this->m_points[0].x <= a_bounding.m_points[0].x && a_bounding.m_points[1].x <= this->m_points[1].x &&
			 this->m_points[0].y <= a_bounding.m_points[0].y && a_bounding.m_points[1].y <= this->m_points[1].y &&
			 this->m_points[0].z <= a_bounding.m_points[0].z && a_bounding.m_points[1].z <= this->m_points[1].z)
	{
		return CollisionType::inside;
	}

	return CollisionType::intersects;
}

template <class _type>
FORCE_INLINE CollisionType BoundingBox<_type>::collision(const BoundingCircle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	// Collision done in 3D
	Vector3<typename _type::value_type> a_bounding_m_points_0_(a_bounding.m_points[0]);

	auto closest = vector_clamp(a_bounding_m_points_0_, this->m_points[0], this->m_points[1]);
	auto dist    = distance(a_bounding_m_points_0_, closest);

	if (dist < a_bounding.m_radius && ror::equal_zero(closest.z))
	{
		// This check is done in 2D
		if (this->m_points[0].x <= a_bounding.m_points[0].x - a_bounding.m_radius &&
			this->m_points[1].x >= a_bounding.m_points[0].x + a_bounding.m_radius &&
			this->m_points[0].y <= a_bounding.m_points[0].y - a_bounding.m_radius &&
			this->m_points[1].y >= a_bounding.m_points[0].y + a_bounding.m_radius)
		{
			return CollisionType::inside;
		}
		else
		{
			return CollisionType::intersects;
		}
	}

	return CollisionType::outside;
}

template <class _type>
FORCE_INLINE CollisionType BoundingBox<_type>::collision(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y ||
		0 > this->m_points[1].z || this->m_points[0].z > 0)
	{
		return CollisionType::outside;
	}
	else if (this->m_points[0].x <= a_bounding.m_points[0].x && a_bounding.m_points[1].x <= this->m_points[1].x &&
			 this->m_points[0].y <= a_bounding.m_points[0].y && a_bounding.m_points[1].y <= this->m_points[1].y &&
			 this->m_points[0].z <= 0 && 0 <= this->m_points[1].z)
	{
		return CollisionType::inside;
	}

	return CollisionType::intersects;
}

template <class _type>
FORCE_INLINE bool BoundingBox<_type>::intersects(const BoundingSphere<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	auto closest = vector_clamp(a_bounding.m_points[0], this->m_points[0], this->m_points[1]);
	auto dist    = distance(a_bounding.m_points[0], closest);

	if (dist < a_bounding.m_radius)
	{
		return true;
	}

	return false;
}

template <class _type>
FORCE_INLINE bool BoundingBox<_type>::intersects(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y ||
		a_bounding.m_points[0].z > this->m_points[1].z || this->m_points[0].z > a_bounding.m_points[1].z)
	{
		return false;
	}

	return true;
}

template <class _type>
FORCE_INLINE bool BoundingBox<_type>::intersects(const BoundingCircle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	// Collision done in 3D
	Vector3<typename _type::value_type> a_bounding_m_points_0_(a_bounding.m_points[0]);

	auto closest = vector_clamp(a_bounding_m_points_0_, this->m_points[0], this->m_points[1]);
	auto dist    = distance(a_bounding_m_points_0_, closest);

	if (dist < a_bounding.m_radius && ror::equal_zero(closest.z))
	{
		return true;
	}

	return false;
}

template <class _type>
FORCE_INLINE bool BoundingBox<_type>::intersects(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding) const noexcept
{
	if (a_bounding.m_points[0].x > this->m_points[1].x || this->m_points[0].x > a_bounding.m_points[1].x ||
		a_bounding.m_points[0].y > this->m_points[1].y || this->m_points[0].y > a_bounding.m_points[1].y ||
		0 > this->m_points[1].z || this->m_points[0].z > 0)
	{
		return false;
	}

	return true;
}

template <class _type>
FORCE_INLINE void BoundingBox<_type>::add_bounding(const BoundingSphere<Vector3<typename _type::value_type>> &a_sphere)
{
	_type min = a_sphere.m_points[0] - _type(a_sphere.m_radius);
	_type max = a_sphere.m_points[0] + _type(a_sphere.m_radius);

	this->add_point(min);
	this->add_point(max);
}

template <class _type>
FORCE_INLINE void BoundingBox<_type>::add_bounding(const BoundingBox<Vector3<typename _type::value_type>> &a_bounding)
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
FORCE_INLINE void BoundingBox<_type>::add_bounding(const BoundingCircle<Vector2<typename _type::value_type>> &a_circle)
{
	auto min = a_circle.m_points[0] - Vector2<typename _type::value_type>(a_circle.m_radius);
	auto max = a_circle.m_points[0] + Vector2<typename _type::value_type>(a_circle.m_radius);

	this->add_point(Vector3<typename _type::value_type>(min));
	this->add_point(Vector3<typename _type::value_type>(max));
}

template <class _type>
FORCE_INLINE void BoundingBox<_type>::add_bounding(const BoundingRectangle<Vector2<typename _type::value_type>> &a_bounding)
{
	this->add_point(Vector3<typename _type::value_type>(a_bounding.m_points[0]));
	this->add_point(Vector3<typename _type::value_type>(a_bounding.m_points[1]));
}

template <class _type>
FORCE_INLINE CollisionType BoundingBox<_type>::collision(const Bounding &a_bounding) const
{
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingSphere<_type> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingBox<_type> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingCircle<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->collision(static_cast<const BoundingRectangle<Vector2<typename _type::value_type>> &>(a_bounding));
	}

	return CollisionType::outside;
}

template <class _type>
FORCE_INLINE bool BoundingBox<_type>::intersects(const Bounding &a_bounding) const
{
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingSphere<_type> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingBox<_type> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingCircle<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->intersects(static_cast<const BoundingRectangle<Vector2<typename _type::value_type>> &>(a_bounding));
	}

	return false;
}

template <class _type>
FORCE_INLINE void BoundingBox<_type>::add_bounding(const Bounding &a_bounding)
{
	if (BoundingType::sphere_3d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingSphere<_type> &>(a_bounding));
	}
	else if (BoundingType::box_3d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingBox<_type> &>(a_bounding));
	}
	else if (BoundingType::circle_2d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingCircle<Vector2<typename _type::value_type>> &>(a_bounding));
	}
	else if (BoundingType::rectangle_2d == a_bounding.m_type)
	{
		return this->add_bounding(static_cast<const BoundingRectangle<Vector2<typename _type::value_type>> &>(a_bounding));
	}
}

template <class _type>
FORCE_INLINE void BoundingBox<_type>::add_point(const _type &a_point)
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
FORCE_INLINE bool BoundingBox<_type>::is_point_inside(const _type &a_point) const
{
	return !(a_point.x < this->m_points[0].x || a_point.x > this->m_points[1].x ||
			 a_point.y < this->m_points[0].y || a_point.y > this->m_points[1].y ||
			 a_point.z < this->m_points[0].z || a_point.z > this->m_points[1].z);
}

template <class _type>
FORCE_INLINE void BoundingBox<_type>::create_from_min_max(_type a_minimum, _type a_maximum)
{
	this->m_points[0] = a_minimum;
	this->m_points[1] = a_maximum;
}

}        // namespace ror
