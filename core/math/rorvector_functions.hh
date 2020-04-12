// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2019
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of a_vector.oftware and associated documentation files (the 'Software'),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and a_vector.ermission notice shall be included in
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

#include "rorvector_functions.hpp"
#include <type_traits>

namespace ror
{
template <class _type>
FORCE_INLINE auto distance(const _type &a_first, const _type &a_second) -> scalar_precision<_type>
{
	_type tmp{a_second - a_first};
	return tmp.length();
}

template <class _type>
FORCE_INLINE auto distance_squared(const _type &a_first, const _type &a_second) -> scalar_precision<_type>
{
	_type tmp{a_second - a_first};
	return static_cast<scalar_precision<_type>>(tmp.length_squared());
}

template <class _type>
FORCE_INLINE auto angle(const _type &a_first, const _type &a_second) -> scalar_precision<_type>
{
	using precision = scalar_precision<_type>;

	precision angl = std::acos(static_cast<precision>(std::clamp(static_cast<precision>(a_first.dot_product(a_second) / (a_first.length() * a_second.length())),
																 static_cast<precision>(-1),
																 static_cast<precision>(1))));
	if (std::isnan(angl))
		return static_cast<precision>(0);

	return angl;
}

template <class _type>
FORCE_INLINE auto scalar_projection(const _type &a_first, const _type &a_second) -> scalar_precision<_type>
{
	return a_first.dot_product(a_second) / a_second.length();
}

template <template <class> class _type, class _precision>
FORCE_INLINE auto vector_rejection(const _type<_precision> &a_first, const _type<_precision> &a_second) -> vector_precision<_type, _precision>
{
	return static_cast<vector_precision<_type, _precision>>(a_first) - vector_projection(a_first, a_second);
}

template <template <class> class _type, class _precision>
FORCE_INLINE auto vector_projection(const _type<_precision> &a_first, const _type<_precision> &a_second) -> vector_precision<_type, _precision>
{
	using vec_prec = vector_precision<_type, _precision>;
	return static_cast<vec_prec>(a_second) * (a_first.dot_product(a_second) / a_second.dot_product(a_second));
}

template <template <class> class _type, class _precision>
FORCE_INLINE auto vector_lerp(const _type<_precision> &a_from, const _type<_precision> &a_to, scalar_precision<_type<_precision>> a_t) -> vector_precision<_type, _precision>
{
	using vec_prec = vector_precision<_type, _precision>;
	return static_cast<vec_prec>(a_from) * (static_cast<vec_prec>(1.0) - a_t) + (static_cast<vec_prec>(a_to) * a_t);
}

template <template <class> class _type, class _precision>
FORCE_INLINE auto vector_reflect(const _type<_precision> &a_incident, const _type<_precision> &a_normal) -> vector_precision<_type, _precision>
{
	using sclr_prec = scalar_precision<_type<_precision>>;
	using vec_prec  = vector_precision<_type, _precision>;
	return static_cast<vec_prec>(a_incident) - static_cast<vec_prec>(a_normal) * a_incident.dot_product(a_normal) * static_cast<sclr_prec>(2);
}

// Need a better way of dealing with stuff like this
template <class _type>
FORCE_INLINE _type vector_minimum(const _type &a_first, const _type &a_second)
{
	if constexpr (std::is_same<_type, Vector4<typename _type::value_type>>::value)
	{
		return _type(std::min(a_first.x, a_second.x), std::min(a_first.y, a_second.y), std::min(a_first.z, a_second.z), std::min(a_first.w, a_second.w));
	}

	if constexpr (std::is_same<_type, Vector3<typename _type::value_type>>::value)
	{
		return _type(std::min(a_first.x, a_second.x), std::min(a_first.y, a_second.y), std::min(a_first.z, a_second.z));
	}

	if constexpr (std::is_same<_type, Vector2<typename _type::value_type>>::value)
	{
		return _type(std::min(a_first.x, a_second.x), std::min(a_first.y, a_second.y));
	}

	return _type();
}

template <class _type>
FORCE_INLINE _type vector_maximum(const _type &a_first, const _type &a_second)
{
	if constexpr (std::is_same<_type, Vector4<typename _type::value_type>>::value)
	{
		return _type(std::max(a_first.x, a_second.x), std::max(a_first.y, a_second.y), std::max(a_first.z, a_second.z), std::max(a_first.w, a_second.w));
	}

	if constexpr (std::is_same<_type, Vector3<typename _type::value_type>>::value)
	{
		return _type(std::max(a_first.x, a_second.x), std::max(a_first.y, a_second.y), std::max(a_first.z, a_second.z));
	}

	if constexpr (std::is_same<_type, Vector2<typename _type::value_type>>::value)
	{
		return _type(std::max(a_first.x, a_second.x), std::max(a_first.y, a_second.y));
	}

	return _type();
}

template <class _type>
FORCE_INLINE _type vector_clamp(const _type &a_vector, const _type &a_first, const _type &a_second)
{
	if constexpr (std::is_same<_type, Vector4<typename _type::value_type>>::value)
	{
		return _type(std::clamp(a_vector.x, a_first.x, a_second.x), std::clamp(a_vector.y, a_first.y, a_second.y), std::clamp(a_vector.z, a_first.z, a_second.z), std::clamp(a_vector.w, a_first.w, a_second.w));
	}

	if constexpr (std::is_same<_type, Vector3<typename _type::value_type>>::value)
	{
		return _type(std::clamp(a_vector.x, a_first.x, a_second.x), std::clamp(a_vector.y, a_first.y, a_second.y), std::clamp(a_vector.z, a_first.z, a_second.z));
	}

	if constexpr (std::is_same<_type, Vector2<typename _type::value_type>>::value)
	{
		return _type(std::clamp(a_vector.x, a_first.x, a_second.x), std::clamp(a_vector.y, a_first.y, a_second.y));
	}

	return _type();
}

}        // namespace ror
