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

#include "foundation/rortypes.hpp"
#include <filesystem>
#include <fstream>
#include <functional>
#include <istream>
#include "include/gtest/gtest.h"
#include <iostream>

#include "graphics/rormesh.hpp"
#include "include/gtest/gtest.h"
#include "profiling/rorlog.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_attribute.hpp"
#include "rhi/rorvertex_description.hpp"
#include "rhi/rorvertex_layout.hpp"

namespace fs = std::filesystem;

namespace ror_test
{
const std::string green = "2";
const std::string red   = "1";

constexpr double test_epsilon = 1e-5;

void print_with_gtest_header(const char *txt, const std::string &color);
void print_gtest_header(const std::string &color);

using RoarTestTypes = testing::Types<float32_t, double64_t, int32_t, int16_t, int8_t, uint32_t, uint64_t, uint8_t>;

#define EXPECT_DOUBLE_NE(a, b)                                    \
	{                                                             \
		EXPECT_FALSE(std::abs(a - b) < ror::ror_epsilon_squared); \
	} (void) 1

#define test_vector4_equal_seperate(v, _x, _y, _z, _w)                                            \
	{                                                                                             \
		if (std::is_same<TypeParam, float>::value || std::is_same<TypeParam, double>::value)      \
		{                                                                                         \
			EXPECT_NEAR(static_cast<double64_t>(v.x), static_cast<double64_t>(_x), test_epsilon); \
			EXPECT_NEAR(static_cast<double64_t>(v.y), static_cast<double64_t>(_y), test_epsilon); \
			EXPECT_NEAR(static_cast<double64_t>(v.z), static_cast<double64_t>(_z), test_epsilon); \
			EXPECT_NEAR(static_cast<double64_t>(v.w), static_cast<double64_t>(_w), test_epsilon); \
		}                                                                                         \
		else                                                                                      \
		{                                                                                         \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(v.x), static_cast<double64_t>(_x));          \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(v.y), static_cast<double64_t>(_y));          \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(v.z), static_cast<double64_t>(_z));          \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(v.w), static_cast<double64_t>(_w));          \
		}                                                                                         \
	}(void) 1

#define test_vector4_equal(a, b)                                                                   \
	{                                                                                              \
		if (std::is_same<TypeParam, float>::value || std::is_same<TypeParam, double>::value)       \
		{                                                                                          \
			EXPECT_NEAR(static_cast<double64_t>(a.x), static_cast<double64_t>(b.x), test_epsilon); \
			EXPECT_NEAR(static_cast<double64_t>(a.y), static_cast<double64_t>(b.y), test_epsilon); \
			EXPECT_NEAR(static_cast<double64_t>(a.z), static_cast<double64_t>(b.z), test_epsilon); \
			EXPECT_NEAR(static_cast<double64_t>(a.w), static_cast<double64_t>(b.w), test_epsilon); \
		}                                                                                          \
		else                                                                                       \
		{                                                                                          \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(a.x), static_cast<double64_t>(b.x));          \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(a.y), static_cast<double64_t>(b.y));          \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(a.z), static_cast<double64_t>(b.z));          \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(a.w), static_cast<double64_t>(b.w));          \
		}                                                                                          \
	}(void) 1

#define test_vector4_not_equal(a, b)                                     \
	{                                                                    \
		EXPECT_NE(static_cast<int32_t>(a.x), static_cast<int32_t>(b.x)); \
		EXPECT_NE(static_cast<int32_t>(a.y), static_cast<int32_t>(b.y)); \
		EXPECT_NE(static_cast<int32_t>(a.z), static_cast<int32_t>(b.z)); \
		EXPECT_NE(static_cast<int32_t>(a.w), static_cast<int32_t>(b.w)); \
	}(void) 1

/**
 * All these Macros are gross but if I don't create
 * those the line numbers aren't correct in error reporting
 */
#define test_vector3_equal_seperate(v, _x, _y, _z)                                            \
	{                                                                                         \
		EXPECT_NEAR(static_cast<double64_t>(v.x), static_cast<double64_t>(_x), test_epsilon); \
		EXPECT_NEAR(static_cast<double64_t>(v.y), static_cast<double64_t>(_y), test_epsilon); \
		EXPECT_NEAR(static_cast<double64_t>(v.z), static_cast<double64_t>(_z), test_epsilon); \
	}(void) 1

#define test_vector3_equal(a, b)                                                               \
	{                                                                                          \
		EXPECT_NEAR(static_cast<double64_t>(a.x), static_cast<double64_t>(b.x), test_epsilon); \
		EXPECT_NEAR(static_cast<double64_t>(a.y), static_cast<double64_t>(b.y), test_epsilon); \
		EXPECT_NEAR(static_cast<double64_t>(a.z), static_cast<double64_t>(b.z), test_epsilon); \
	}(void) 1

#define test_vector3_not_equal(a, b)                                     \
	{                                                                    \
		EXPECT_NE(static_cast<int32_t>(a.x), static_cast<int32_t>(b.x)); \
		EXPECT_NE(static_cast<int32_t>(a.y), static_cast<int32_t>(b.y)); \
		EXPECT_NE(static_cast<int32_t>(a.z), static_cast<int32_t>(b.z)); \
	}(void) 1

#define test_vector2_equal_seperate(v, _x, _y)                                                    \
	{                                                                                             \
		if (std::is_same<TypeParam, float>::value || std::is_same<TypeParam, double>::value)      \
		{                                                                                         \
			EXPECT_NEAR(static_cast<double64_t>(v.x), static_cast<double64_t>(_x), test_epsilon); \
			EXPECT_NEAR(static_cast<double64_t>(v.y), static_cast<double64_t>(_y), test_epsilon); \
		}                                                                                         \
		else                                                                                      \
		{                                                                                         \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(v.x), static_cast<double64_t>(_x));          \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(v.y), static_cast<double64_t>(_y));          \
		}                                                                                         \
	} (void) 1

#define test_vector2_equal(a, b)                                                                   \
	{                                                                                              \
		if (std::is_same<TypeParam, float>::value || std::is_same<TypeParam, double>::value)       \
		{                                                                                          \
			EXPECT_NEAR(static_cast<double64_t>(a.x), static_cast<double64_t>(b.x), test_epsilon); \
			EXPECT_NEAR(static_cast<double64_t>(a.y), static_cast<double64_t>(b.y), test_epsilon); \
		}                                                                                          \
		else                                                                                       \
		{                                                                                          \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(a.x), static_cast<double64_t>(b.x));          \
			EXPECT_DOUBLE_EQ(static_cast<double64_t>(a.y), static_cast<double64_t>(b.y));          \
		}                                                                                          \
	}(void) 1

#define test_vector2_not_equal(a, b)                                     \
	{                                                                    \
		EXPECT_NE(static_cast<int32_t>(a.x), static_cast<int32_t>(b.x)); \
		EXPECT_NE(static_cast<int32_t>(a.y), static_cast<int32_t>(b.y)); \
	} (void) 1

#define test_matrix2_equal(a, b)                                                                                               \
	{                                                                                                                          \
		const uint32_t matrix_size = 4;                                                                                        \
		for (uint32_t index = 0; index < matrix_size; ++index)                                                                 \
		{                                                                                                                      \
			EXPECT_NEAR(static_cast<double64_t>(a.m_values[index]), static_cast<double64_t>(b.m_values[index]), test_epsilon); \
		}                                                                                                                      \
	}(void) 1

#define test_matrix3_equal(a, b)                                                                                               \
	{                                                                                                                          \
		const uint32_t matrix_size = 9;                                                                                        \
		for (uint32_t index = 0; index < matrix_size; ++index)                                                                 \
		{                                                                                                                      \
			EXPECT_NEAR(static_cast<double64_t>(a.m_values[index]), static_cast<double64_t>(b.m_values[index]), test_epsilon); \
		}                                                                                                                      \
	}(void) 1

#define test_matrix4_equal(a, b)                                                                                               \
	{                                                                                                                          \
		const uint32_t matrix_size = 16;                                                                                       \
		for (uint32_t index = 0; index < matrix_size; ++index)                                                                 \
		{                                                                                                                      \
			EXPECT_NEAR(static_cast<double64_t>(a.m_values[index]), static_cast<double64_t>(b.m_values[index]), test_epsilon); \
		}                                                                                                                      \
	}(void) 1

#define test_matrix3x4_equal(a, b)                                                                                             \
	{                                                                                                                          \
		const uint32_t matrix_size = 12;                                                                                       \
		for (uint32_t index = 0; index < matrix_size; ++index)                                                                 \
		{                                                                                                                      \
			EXPECT_NEAR(static_cast<double64_t>(a.m_values[index]), static_cast<double64_t>(b.m_values[index]), test_epsilon); \
		}                                                                                                                      \
	}(void) 1

// a is 3x4 and b is 4x4
#define test_matrix3x4_equal_matrix4(a, b)                                                                                                 \
	{                                                                                                                                      \
		const uint32_t matrix_size = 12;                                                                                                   \
		for (uint32_t index = 0; index < matrix_size; ++index)                                                                             \
		{                                                                                                                                  \
			EXPECT_NEAR(static_cast<double64_t>(a.m_values[index]), static_cast<double64_t>(b.m_values[index + index / 3]), test_epsilon); \
		}                                                                                                                                  \
	}(void) 1

#define test_quaternion_equal(a, b)                                                            \
	{                                                                                          \
		EXPECT_NEAR(static_cast<double64_t>(a.x), static_cast<double64_t>(b.x), test_epsilon); \
		EXPECT_NEAR(static_cast<double64_t>(a.y), static_cast<double64_t>(b.y), test_epsilon); \
		EXPECT_NEAR(static_cast<double64_t>(a.z), static_cast<double64_t>(b.z), test_epsilon); \
		EXPECT_NEAR(static_cast<double64_t>(a.w), static_cast<double64_t>(b.w), test_epsilon); \
	}(void) 1

void write_file(fs::path, std::string data);

fs::path get_root_dir();
fs::path create_root_dir();

void teardown_environment();

void test_one_vertex_description(const rhi::VertexDescriptor &vd,
								 rhi::BufferSemantic    semantic,
								 uint32_t               location,
								 uint32_t               offset,
								 uint64_t               buffer_offset,
								 uint32_t               binding,
								 uint32_t               buffer_index,
								 rhi::VertexFormat      format,

								 rhi::StepFunction function,
								 uint64_t          stride,
								 rhi::Rate         rate,
								 uint32_t          multiplier,
								 uint64_t          semantic_type,
								 uint32_t          line);

}        // namespace ror_test
