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

#include "foundation/rorsystem.hpp"
#include "rormacros.hpp"
#include "rortypes.hpp"
#include <atomic>
#include <cstdlib>
#include <functional>
#include <mutex>
#include <random>
#include <stdexcept>
#include <type_traits>

namespace ror
{
#define BUFFER_OFFSET(i) ((char *) nullptr + (i))
// #define CONSTANT_SEED

constexpr float32_t ror_pi = 3.1415926535897932384626433832795f;        // even though float32_t can't hold all of it :), also known as 180 degrees

template <class _type>
constexpr _type ror_pi_typed = static_cast<_type>(3.1415926535897932384626433832795L);

constexpr float32_t ror_two_pi           = 6.283185307179586f;           // 360 degrees
constexpr float32_t ror_half_pi          = 1.570796326794897f;           // 90 degrees
constexpr float32_t ror_third_pi         = 1.047197551196598f;           // 60 degrees
constexpr float32_t ror_quarter_pi       = 0.785398163397448f;           // 45 degrees
constexpr float32_t ror_sixth_pi         = 0.523598775598299f;           // 30 degrees
constexpr float32_t ror_two_third_pi     = 2.0f * ror_third_pi;          // 120 degrees
constexpr float32_t ror_three_quarter_pi = 3.0f * ror_quarter_pi;        // 135 degrees
constexpr float32_t ror_five_sixth_pi    = 5.0f * ror_sixth_pi;          // 150 degrees
constexpr float32_t ror_epsilon          = 1e-5f;
constexpr float32_t ror_epsilon_squared  = 1e-10f;
constexpr float32_t ror_epsilon_relaxed  = 1e-4f;        // Should be used for PBR shaders otherwise you will have artifacts

// Convineance function for roar precision type cast
template <class _type>
FORCE_INLINE auto ror_precision_cast(_type a_value) -> ror_precision<_type>
{
	return ror_precision<_type>(a_value);
}

template <class _type>
FORCE_INLINE constexpr bool decimal_equal(_type a_first, _type a_second)
{
	if (std::abs(static_cast<ror_precision<_type>>(a_first - a_second)) <= static_cast<ror_precision<_type>>(ror_epsilon))
		return true;
	else
		return false;
}

template <class _type>
FORCE_INLINE constexpr bool equal_zero(_type a_value)
{
	return decimal_equal<_type>(a_value, static_cast<_type>(0));
}

// NOTE: Only valid for integer types can't be used with floats.
// uses Cantor pairing function from https://en.wikipedia.org/wiki/Pairing_function
FORCE_INLINE uint32_t pair(uint32_t a_x, uint32_t a_y)
{
	return ((a_x * a_x) + a_x + (2 * a_x * a_y) + (3 * a_y) + (a_y * a_y)) / 2;
}

// Alternate way of equal_zero but need to be called equal_zero_copy<_type>()
template <class _type>
std::function<bool(_type)> equal_zero_copy = std::bind(decimal_equal<_type>, std::placeholders::_1, 0);

template <class _type>
FORCE_INLINE constexpr _type to_radians(_type a_degrees)
{
	return a_degrees * static_cast<_type>(0.017453292519943295769236907684886);
}

template <class _type>
FORCE_INLINE constexpr _type to_degrees(_type a_radians)
{
	return a_radians * static_cast<_type>(57.295779513082320876798154814105);
}

template <class _type>
FORCE_INLINE constexpr _type fov_to_zoom_(_type a_fov)
{
	return (static_cast<_type>(1.0) / std::tan(a_fov * static_cast<_type>(0.5)));
}

template <class _type>
FORCE_INLINE constexpr _type zoom_to_fov_(_type a_zoom)
{
	return (static_cast<_type>(2.0) * std::atan(static_cast<_type>(1.0) / a_zoom));
}

template <class _type, class _type2 = float32_t>
FORCE_INLINE constexpr _type2 interpolate(_type a_from, _type a_to, _type2 a_t)
{
	static_assert(std::is_same<_type2, float32_t>::value || std::is_same<_type2, double64_t>::value, "T should be single or double precisions float");
	// return static_cast<_type2>(((static_cast<_type2>(1) - a_t) * (static_cast<_type2>(a_from)) + (a_t * static_cast<_type2>(a_to))));
	return static_cast<_type2>(static_cast<_type2>(a_from) + ((static_cast<_type2>(a_to) - static_cast<_type2>(a_from)) * a_t));
}

template <class _type>
const auto lerp = interpolate<_type>;

// https://www.gamedev.net/articles/programming/general-and-gameplay-programming/inverse-lerp-a-super-useful-yet-often-overlooked-function-r5230
template <class _type>
FORCE_INLINE constexpr auto interpolate_inverse(_type a_from, _type a_to, _type a_v) -> ror_precision<_type>
{
	if (a_from < a_to)
		return (static_cast<ror_precision<_type>>(a_v) - static_cast<ror_precision<_type>>(a_from)) / (static_cast<ror_precision<_type>>(a_to) - static_cast<ror_precision<_type>>(a_from));
	else
		return (static_cast<ror_precision<_type>>(a_v) - static_cast<ror_precision<_type>>(a_to)) / (static_cast<ror_precision<_type>>(a_from) - static_cast<ror_precision<_type>>(a_to));
}

template <class _type>
const auto lerp_inv = interpolate_inverse<_type>;

// This function takes a_value in range 1 (a_from1 -> a_to1) and remaps it into range 2 (a_from2 -> a_to2)
template <class _type>
FORCE_INLINE constexpr auto remap(_type a_from1, _type a_to1, _type a_from2, _type a_to2, _type a_v) -> ror_precision<_type>
{
	auto t = interpolate_inverse(a_from1, a_to1, a_v);
	return interpolate(a_from2, a_to2, t);
}

template <class _type, class _type2 = float32_t>
FORCE_INLINE constexpr _type2 interpolate_clamp(_type a_from, _type a_to, _type2 a_t)
{
	if (a_from < a_to)
		return std::clamp(interpolate(a_from, a_to, a_t), static_cast<_type2>(a_from), static_cast<_type2>(a_to));
	else
		return std::clamp(interpolate(a_from, a_to, a_t), static_cast<_type2>(a_to), static_cast<_type2>(a_from));
}

template <class _type>
const auto lerp_clamp = interpolate_clamp<_type>;

template <class _type>
FORCE_INLINE constexpr auto interpolate_inverse_clamp(_type a_from, _type a_to, _type a_v) -> ror_precision<_type>
{
	return std::clamp<ror_precision<_type>>(interpolate_inverse(a_from, a_to, a_v), static_cast<ror_precision<_type>>(0.0), static_cast<ror_precision<_type>>(1.0));
}

template <class _type>
const auto lerp_inv_clamp = interpolate_inverse_clamp<_type>;

// This function takes a_value in range 1 (a_from1 -> a_to1) and remaps it into range 2 (a_from2 -> a_to2)
template <class _type>
FORCE_INLINE constexpr auto remap_clamp(_type a_from1, _type a_to1, _type a_from2, _type a_to2, _type a_v) -> ror_precision<_type>
{
	auto t = interpolate_inverse_clamp(a_from1, a_to1, a_v);
	return interpolate_clamp(a_from2, a_to2, t);
}

template <class _type, class _genarator, typename std::enable_if<std::is_integral<_type>::value, _type>::type * = nullptr>
FORCE_INLINE constexpr _type _random(_type a_min, _type a_max, _genarator &a_engine)
{
	std::uniform_int_distribution<_type> dist(a_min, a_max);
	return dist(a_engine);
}

template <class _type, class _genarator, typename std::enable_if<std::is_floating_point<_type>::value, _type>::type * = nullptr>
// template <class _type, class _genarator, typename std::enable_if<std::is_floating_point<_type>::value>::type>
FORCE_INLINE constexpr _type _random(_type a_min, _type a_max, _genarator &a_engine)
{
	std::uniform_real_distribution<_type> dist(a_min, a_max);
	return dist(a_engine);
}

// Not very high quality and slow random generators, better use ror::Random
template <class _type>
FORCE_INLINE _type random()
{
#ifdef CONSTANT_SEED
	static std::mt19937 engine(1);
#else
	static std::random_device device;
	static std::mt19937       engine(device());
#endif
	return _random(std::numeric_limits<_type>::min(), std::numeric_limits<_type>::max(), engine);
}

// Not very high quality and slow random generators, better user ror::Random
template <class _type>
FORCE_INLINE _type random(_type a_min, _type a_max)
{
#ifdef CONSTANT_SEED
	static std::mt19937 engine(static_cast<_type>(1));
#else
	static std::random_device device;
	static std::mt19937       engine(device());
#endif
	if (a_min < a_max)
		return _random<_type, std::mt19937>(a_min, a_max, engine);
	else
		return _random<_type, std::mt19937>(a_max, a_min, engine);
}

template <class _type>
FORCE_INLINE constexpr _type closest(_type a_value, _type a_first, _type a_second)
{
	// Converts to decimal from all types including unsigned values, in which case you might loose precision, truncation
	return (std::abs(ror_precision_cast(a_value) - ror_precision_cast(a_first)) <
	                std::abs(ror_precision_cast(a_value) - ror_precision_cast(a_second)) ?
	            a_first :
	            a_second);
}

FORCE_INLINE constexpr uint32_t power_of_two(uint32_t a_value)
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
FORCE_INLINE constexpr bool is_nan(_type a_value)
{
	return a_value != a_value;
}

template <typename _e>
constexpr auto enum_to_type_cast(_e a_enum) noexcept
{
	return static_cast<std::underlying_type_t<_e>>(a_enum);
}

template <typename _type>
FORCE_INLINE void vector_remove_duplicates(std::vector<_type> &a_vector)
{
	std::sort(a_vector.begin(), a_vector.end());
	auto last = std::unique(a_vector.begin(), a_vector.end());
	a_vector.erase(last, a_vector.end());
}

template <typename _type, typename _predicate>
FORCE_INLINE void vector_remove_duplicates(std::vector<_type> &a_vector, _predicate a_predicate)
{
	std::sort(a_vector.begin(), a_vector.end(), a_predicate);
	auto last = std::unique(a_vector.begin(), a_vector.end(), a_predicate);
	a_vector.erase(last, a_vector.end());
}

template <class _type_to,
          class _type_from,
          class _type_big = typename std::conditional<std::is_integral<_type_from>::value, unsigned long long, long double>::type>
constexpr FORCE_INLINE _type_to static_cast_safe(_type_from a_value)
{
	if constexpr (get_build() == BuildType::build_debug)
	{
		if (a_value < static_cast<_type_from>(0))
			throw std::runtime_error("Safe casting from negative values is undefined.");

		if (static_cast<_type_big>(a_value) > static_cast<_type_big>(std::numeric_limits<_type_to>::max()))
			throw std::runtime_error("Loss of data doing safe casting.");
	}

	return static_cast<_type_to>(a_value);
}

/**
 * Only valid for positive _type(s) so the compiler will complain otherwise
 * returns _alignment aligned value/pointer given of the input value/pointer
 */
template <class _type>
constexpr FORCE_INLINE _type align(_type a_input, uint32_t _alignment)
{
	return static_cast<_type>(static_cast<uint64_t>(a_input + _alignment - 1) & ~uint64_t(_alignment - 1));
}

template <class _type,
          size_t _alignment>
constexpr FORCE_INLINE _type align(_type a_input)
{
	return static_cast<_type>(static_cast<uint64_t>(a_input + _alignment - 1) & ~uint64_t(_alignment - 1));
}

template <class _type,
          size_t _alignment>
constexpr FORCE_INLINE _type *align(_type *a_input)
{
	return reinterpret_cast<_type *>(reinterpret_cast<uintptr_t>(a_input + _alignment - 1) & ~uintptr_t(_alignment - 1));
}

template <class _type>
constexpr FORCE_INLINE _type align4(_type a_input)
{
	return align<_type, 4>(a_input);
}

template <class _type>
constexpr FORCE_INLINE _type align8(_type a_input)
{
	return align<_type, 8>(a_input);
}

template <class _type>
constexpr FORCE_INLINE _type align16(_type a_input)
{
	return align<_type, 16>(a_input);
}

template <class _type>
constexpr FORCE_INLINE _type *align4(_type *a_input)
{
	return align<_type, 4>(a_input);
}

template <class _type>
constexpr FORCE_INLINE _type *align8(_type *a_input)
{
	return align<_type, 8>(a_input);
}

template <class _type>
constexpr FORCE_INLINE _type *align16(_type *a_input)
{
	return align<_type, 16>(a_input);
}

template <bool>
class ConditionalMutex
{};

template <>
class ConditionalMutex<true>
{
  public:
	std::mutex m_mutex{};        //! Use to synchronize access from different threads
};

template <typename _type>
struct MemoryLayout
{
  public:
	// clang-format off
	FORCE_INLINE static constexpr uint8_t size()          { return sizeof(_type);  }
	FORCE_INLINE static constexpr uint8_t stride()        { return sizeof(_type);  }
	FORCE_INLINE static constexpr uint8_t alignment()     { return alignof(_type); }
	// clang-format on
};

template <typename _type, std::enable_if_t<std::is_floating_point<_type>::value, bool> = true>
constexpr _type srgb_to_linear(_type a_srgb)
{
	if (a_srgb < static_cast<_type>(0.04045f))
		a_srgb = a_srgb / static_cast<_type>(12.92f);
	else
		a_srgb = std::pow((a_srgb + static_cast<_type>(0.055f)) / static_cast<_type>(1.055f), static_cast<_type>(2.4f));

	return a_srgb;
}

}        // namespace ror
