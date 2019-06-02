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

#pragma once

#include "rormacros.hpp"
#include <cstdlib>
#include <functional>
#include <random>

namespace ror
{
#define BUFFER_OFFSET(i) ((char *) nullptr + (i))
// #define CONSTANT_SEED

constexpr float32_t ror_pi               = 3.1415926535897932384626433832795f;        //even though float32_t can't hold all of it :), also known as 180 degrees
constexpr float32_t ror_two_pi           = 6.283185307179586f;                        // 360 degrees
constexpr float32_t ror_half_pi          = 1.570796326794897f;                        // 90 degrees
constexpr float32_t ror_third_pi         = 1.047197551196598f;                        // 60 degrees
constexpr float32_t ror_quarter_pi       = 0.785398163397448f;                        // 45 degrees
constexpr float32_t ror_sixth_pi         = 0.523598775598299f;                        // 30 degrees
constexpr float32_t ror_two_third_pi     = 2.0f * ror_third_pi;                       // 120 degrees
constexpr float32_t ror_three_quarter_pi = 3.0f * ror_quarter_pi;                     // 135 degrees
constexpr float32_t ror_five_sixth_pi    = 5.0f * ror_sixth_pi;                       // 150 degrees
constexpr float32_t ror_epsilon          = 1e-5f;
constexpr float32_t ror_epsilon_squared  = 1e-10f;
constexpr float32_t ror_epsilon_relaxed  = 1e-4f;

template <class _type>
FORCE_INLINE bool decimal_equal(_type a_first, _type a_second)
{
	if (std::abs(static_cast<_type>(a_first - a_second)) <= static_cast<_type>(ror_epsilon))
		return true;
	else
		return false;
}

template <class _type>
FORCE_INLINE bool equal_zero(_type a_value)
{
	return decimal_equal<_type>(a_value, static_cast<_type>(0));
}

// Alternate way of equal_zero but need to be called equal_zero_copy<_type>()
template <class _type>
std::function<bool(_type)> equal_zero_copy = std::bind(decimal_equal<_type>, std::placeholders::_1, 0);

template <class _type>
FORCE_INLINE _type to_radians(_type a_degrees)
{
	return a_degrees * static_cast<_type>(0.017453292519943295769236907684886);
}

template <class _type>
FORCE_INLINE _type to_degrees(_type a_radians)
{
	return a_radians * static_cast<_type>(57.295779513082320876798154814105);
}

template <class _type>
FORCE_INLINE _type fov_to_zoom_(_type a_fov)
{
	return (static_cast<_type>(1.0) / std::tan(a_fov * static_cast<_type>(0.5)));
}

template <class _type>
FORCE_INLINE _type zoom_to_fov_(_type a_zoom)
{
	return (static_cast<_type>(2.0) * std::atan(static_cast<_type>(1.0) / a_zoom));
}

template <class _type>
FORCE_INLINE _type interpolate(_type a_value1, _type a_value2, _type a_t)
{
	return a_value1 + a_t * (a_value2 - a_value1);
}

template <class _type>
FORCE_INLINE _type clamp(_type a_x, _type a_min, _type a_max)
{
	return (a_x < a_min) ? a_min : (a_x > a_max) ? a_max : a_x;
}

template <class _type, class _Genarator, typename std::enable_if<std::is_integral<_type>::value>::type>
FORCE_INLINE _type _random(_type a_min, _type a_max, const _Genarator &a_engine)
{
	std::uniform_int_distribution<_type> dist(a_min, a_max);
	return dist(a_engine);
}

template <class _type, class _Genarator, typename std::enable_if<std::is_floating_point<_type>::value>::type>
FORCE_INLINE _type _random(_type a_min, _type a_max, const _Genarator &a_engine)
{
	std::uniform_real_distribution<_type> dist(a_min, a_max);
	return dist(a_engine);
}

// Not very high quality and slow random generators, better user ror::Random
template <class _type>
FORCE_INLINE _type random()
{
#ifdef CONSTANT_SEED
	static std::mt19937 engine(1);
#else
	static std::random_device device;
	static std::mt19937       engine(device);
#endif
	return _random(std::numeric_limits<_type>::min(), std::numeric_limits<_type>::max(), engine);
	// return static_cast<_type>(std::rand() / static_cast<float32_t>(RAND_MAX));
}

template <class _type>
FORCE_INLINE _type random(_type a_min, _type a_max)
{
#ifdef CONSTANT_SEED
	static std::mt19937 engine(1);
#else
	static std::random_device device;
	static std::mt19937       engine(device);
#endif
	return _random(a_min, a_max, engine);
	// return static_cast<_type>(a_min + (std::rand() / static_cast<float32_t>(RAND_MAX)) * (a_max - a_min));
}

template <class _type>
FORCE_INLINE _type closest(_type a_value, _type a_first, _type a_second)
{
	return (std::abs(a_value - a_first) < std::abs(a_value - a_second) ? a_first : a_second);
}

FORCE_INLINE uint32_t power_of_two(uint32_t a_value)
{
	uint32_t result = 2;

	while (result < a_value)
		result = result << 1;

	return result;
}

template <typename _type>
void bubble_sort(_type a_items[], int a_size)
{
	for (int i = a_size - 1; i > 0; --i)
	{
		for (int j = i; j > 0; --j)
		{
			if (a_items[j] < a_items[j - 1])
			{
				_type temp(a_items[j]);
				a_items[j]     = a_items[j - 1];
				a_items[j - 1] = temp;
			}
		}
	}
}

template <typename _type>
FORCE_INLINE bool is_nan(_type a_value)
{
	return a_value != a_value;
}

}        // namespace ror
