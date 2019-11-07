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

	ror::BoundingBoxAxisAligned m_bounding_box_axis_aligned;                       //!< standard member variable used for comparisions
	ror::BoundingSphere         m_bounding_sphere;                                 //!< standard member variable used for comparisions
	ror::Vector3f               m_vector1, m_vector2, m_vector3, m_vector4;        //!< standard member variable used for reference
	ror::Vector3f               m_vector_negative1, m_vector_negative2;            //!< standard member variable used for reference
	ror::Vector3f               m_vector_negative3, m_vector_negative4;            //!< standard member variable used for reference

  private:
};

// using BoundingTypesToTest = ::testing::Types<int64_t, uint64_t, int32_t, int16_t, int8_t, uint32_t, uint16_t, uint8_t, float32_t, double64_t>;
using BoundingTypesToTest = ::testing::Types<int64_t, int32_t, int16_t, int8_t, float32_t, double64_t>;

TYPED_TEST_SUITE(BoundingTest, BoundingTypesToTest);
}        // namespace ror_test

#include "bounds.hh"
