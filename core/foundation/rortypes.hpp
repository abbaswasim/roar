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

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include "rorcompiler_workarounds.hpp"

using ::int8_t;
using ::int16_t;
using ::int32_t;
using ::int64_t;

using ::uint8_t;
using ::uint16_t;
using ::uint32_t;
using ::uint64_t;

using ::intptr_t;
using ::uintptr_t;

using ::intmax_t;
using ::uintmax_t;

using ::ptrdiff_t;
using ::size_t;

using char8_t = int8_t;

using uchar8_t  = uint8_t;
using uchar16_t = uint16_t;
using uchar32_t = uint32_t;

using float32_t   = float;              // Just for consistancy reasons, also compile time checked for size
using double64_t  = double;             // Just for consistancy reasons, also compile time checked for size
using double128_t = long double;        // Just for consistancy reasons, also compile time checked for size

// Check sizes you are getting are what you expect
static_assert(sizeof(char8_t) == 1, "char8_t is not 1 byte");
static_assert(sizeof(char16_t) == 2, "char16_t is not 2 bytes");
static_assert(sizeof(char32_t) == 4, "char32_t is not 4 bytes");

static_assert(sizeof(uchar8_t) == 1, "uchar8_t is not 1 byte");
static_assert(sizeof(uchar16_t) == 2, "uchar16_t is not 2 bytes");
static_assert(sizeof(uchar32_t) == 4, "uchar32_t is not 4 bytes");

static_assert(sizeof(int8_t) == 1, "int8_t is not 1 byte");
static_assert(sizeof(int16_t) == 2, "int16_t is not 2 bytes");
static_assert(sizeof(int32_t) == 4, "int32_t is not 4 bytes");
static_assert(sizeof(int64_t) == 8, "int64_t is not 8 bytes");

static_assert(sizeof(uint8_t) == 1, "uint8_t is not 1 byte");
static_assert(sizeof(uint16_t) == 2, "uint16_t is not 2 bytes");
static_assert(sizeof(uint32_t) == 4, "uint32_t is not 4 bytes");
static_assert(sizeof(uint64_t) == 8, "uint64_t is not 8 bytes");

static_assert(sizeof(uint64_t) == sizeof(size_t), "size_t is not same as uint64_t");
static_assert(sizeof(ptrdiff_t) == sizeof(size_t), "size_t is not same as ptrdiff_t");
static_assert(sizeof(intptr_t) == sizeof(size_t), "size_t is not same as intptr_t");        // This will fail on 32bit

static_assert(sizeof(float32_t) == 4, "uint32_t is not 4 bytes");
static_assert(sizeof(double64_t) == 8, "double64_t is not 8 bytes");
static_assert(sizeof(double128_t) == 16, "double64_t is not 16 bytes");

#if 1

static_assert(std::alignment_of_v<char8_t> == 1, "char8_t is not 1 bytes aligned");
static_assert(std::alignment_of_v<char16_t> == 2, "char16_t is not 2 bytes aligned");
static_assert(std::alignment_of_v<char32_t> == 4, "char32_t is not 4 bytes aligned");

static_assert(std::alignment_of_v<uchar8_t> == 1, "uchar8_t is not 1 bytes aligned");
static_assert(std::alignment_of_v<uchar16_t> == 2, "uchar16_t is not 2 bytes aligned");
static_assert(std::alignment_of_v<uchar32_t> == 4, "uchar32_t is not 4 bytes aligned");

static_assert(std::alignment_of_v<int8_t> == 1, "int8_t is not 1 bytes aligned");
static_assert(std::alignment_of_v<int16_t> == 2, "int16_t is not 2 bytes aligned");
static_assert(std::alignment_of_v<int32_t> == 4, "int32_t is not 4 bytes aligned");
static_assert(std::alignment_of_v<int64_t> == 8, "int64_t is not 8 bytes aligned");
static_assert(std::alignment_of_v<size_t> == 8, "size_t is not 8 bytes aligned");

static_assert(std::alignment_of_v<uint8_t> == 1, "uint8_t is not 1 bytes aligned");
static_assert(std::alignment_of_v<uint16_t> == 2, "uint16_t is not 2 bytes aligned");
static_assert(std::alignment_of_v<uint32_t> == 4, "uint32_t is not 4 bytes aligned");
static_assert(std::alignment_of_v<uint64_t> == 8, "uint64_t is not 8 bytes aligned");

static_assert(std::alignment_of_v<float32_t> == 4, "float32_t is not 4 bytes aligned");
static_assert(std::alignment_of_v<double64_t> == 8, "double64_t is not 8 bytes aligned");
static_assert(std::alignment_of_v<double128_t> == 16, "double128_t is not 16 bytes aligned");

static_assert(std::alignment_of_v<intptr_t> == 8, "intptr_t is not 8 bytes aligned");          // This will fail on 32bit
static_assert(std::alignment_of_v<ptrdiff_t> == 8, "ptrdiff_t is not 8 bytes aligned");        // This will fail on 32bit

#else

static_assert(std::alignment_of<char8_t>() == 1, "char8_t is not 1 bytes aligned");
static_assert(std::alignment_of<char16_t>() == 2, "char16_t is not 2 bytes aligned");
static_assert(std::alignment_of<char32_t>() == 4, "char32_t is not 4 bytes aligned");

static_assert(std::alignment_of<uchar8_t>() == 1, "uchar8_t is not 1 bytes aligned");
static_assert(std::alignment_of<uchar16_t>() == 2, "uchar16_t is not 2 bytes aligned");
static_assert(std::alignment_of<uchar32_t>() == 4, "uchar32_t is not 4 bytes aligned");

static_assert(std::alignment_of<int8_t>() == 1, "int8_t is not 1 bytes aligned");
static_assert(std::alignment_of<int16_t>() == 2, "int16_t is not 2 bytes aligned");
static_assert(std::alignment_of<int32_t>() == 4, "int32_t is not 4 bytes aligned");
static_assert(std::alignment_of<int64_t>() == 8, "int64_t is not 8 bytes aligned");
static_assert(std::alignment_of<size_t>() == 8, "size_t is not 8 bytes aligned");

static_assert(std::alignment_of<uint8_t>() == 1, "uint8_t is not 1 bytes aligned");
static_assert(std::alignment_of<uint16_t>() == 2, "uint16_t is not 2 bytes aligned");
static_assert(std::alignment_of<uint32_t>() == 4, "uint32_t is not 4 bytes aligned");
static_assert(std::alignment_of<uint64_t>() == 8, "uint64_t is not 8 bytes aligned");

static_assert(std::alignment_of<float32_t>() == 4, "float32_t is not 4 bytes aligned");
static_assert(std::alignment_of<double64_t>() == 8, "double64_t is not 8 bytes aligned");
static_assert(std::alignment_of<double128_t>() == 16, "double128_t is not 16 bytes aligned");

static_assert(std::alignment_of<intptr_t>() == 8, "intptr_t is not 8 bytes aligned");          // This will fail on 32bit
static_assert(std::alignment_of<ptrdiff_t>() == 8, "ptrdiff_t is not 8 bytes aligned");        // This will fail on 32bit

#endif
