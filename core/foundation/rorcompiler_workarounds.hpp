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

#include <string_view>
#include <utility>

static_assert(__cplusplus >= 201703L, "Minimum supported compiler 2017 not found");

#if (__cplusplus < 202001L)        // TODO: Change this to 2020 when available

namespace std
{
enum class endian
{
#	ifdef _WIN32
	little = 0,
	big    = 1,
	native = little
#	else
	little = __ORDER_LITTLE_ENDIAN__,
	big    = __ORDER_BIG_ENDIAN__,
	native = __BYTE_ORDER__
#	endif
};
}        // namespace std
#else

#	include <bit>
static_assert(std::endian::native == std::endian::little, "Building on an unsupported non little-endian system");        // Only C++20

#endif

// Before std::overload is available in C++ here is a workaround, keeping it outside std for now
// https://arne-mertz.de/2018/05/overload-build-a-variant-visitor-on-the-fly/
namespace ror
{
template <class... Fs>
struct Overload : Fs...
{
	template <class... Ts>
	Overload(Ts &&...ts) :
	    Fs{std::forward<Ts>(ts)}...
	{}

	using Fs::operator()...;
};

template <class... Ts>
Overload(Ts &&...) -> Overload<std::remove_reference_t<Ts>...>;

// From https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
template <typename T>
constexpr auto type_name()
{
	std::string_view name, prefix, suffix;
#ifdef __clang__
	name   = __PRETTY_FUNCTION__;
	prefix = "auto type_name() [T = ";
	suffix = "]";
#elif defined(__GNUC__)
	name   = __PRETTY_FUNCTION__;
	prefix = "constexpr auto type_name() [with T = ";
	suffix = "]";
#elif defined(_MSC_VER)
	name   = __FUNCSIG__;
	prefix = "auto __cdecl type_name<";
	suffix = ">(void)";
#endif
	name.remove_prefix(prefix.size());
	name.remove_suffix(suffix.size());
	return name;
}

}        // namespace ror
