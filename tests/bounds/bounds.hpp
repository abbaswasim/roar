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

#include <gtest/gtest.h>

#include "bounds/rorbounding.hpp"
#include "common.hpp"

namespace ror_test
{
template <class _type>
class BoundingTest : public testing::Test
{
  public:
  protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

	ror::Box<ror::Vector3<_type>>   m_bounding_box_axis_aligned;
	ror::Box<ror::Vector2<_type>>   m_bounding_rectangle_axis_aligned;
	ror::Round<ror::Vector3<_type>> m_bounding_sphere;
	ror::Round<ror::Vector2<_type>> m_bounding_circle;

	ror::Vector3<_type> m_vector3_0, m_vector3_1, m_vector3_2, m_vector3_3, m_vector3_4;
	ror::Vector3<_type> m_vector_negative3_1, m_vector_negative3_2;
	ror::Vector3<_type> m_vector_negative3_3, m_vector_negative3_4;

	ror::Vector2<_type> m_vector2_1, m_vector2_2, m_vector2_3, m_vector2_4;
	ror::Vector2<_type> m_vector_negative2_1, m_vector_negative2_2;
	ror::Vector2<_type> m_vector_negative2_3, m_vector_negative2_4;
};

// using BoundingTypesToTest = ::testing::Types<int64_t, uint64_t, int32_t, int16_t, int8_t, uint32_t, uint16_t, uint8_t, float32_t, double64_t>;
// Bounds only defined in cartesian coordinate system so unsigned types shouldn't be used
using BoundingTypesToTest = ::testing::Types<int64_t, int32_t, int16_t, int8_t, float32_t, double64_t>;

TYPED_TEST_SUITE(BoundingTest, BoundingTypesToTest);
}        // namespace ror_test

#include "bounds.hh"
