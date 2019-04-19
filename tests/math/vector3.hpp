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

#include <gtest/gtest.h>

#include "common.hpp"
#include "math/rorvector.hpp"
#include <cmath>

namespace ror_test
{
template <class _type>
class Vector3Test : public testing::Test
{
  public:
	using value_type = _type;

	ror::Vector3<_type> m_a;
	ror::Vector3<_type> m_b;
	ror::Vector3<_type> m_sum_ab;
	ror::Vector3<_type> m_diff_ab;
	ror::Vector3<_type> m_mul_ab;
	ror::Vector3<_type> m_div_ab;
	ror::Vector3<_type> m_sum_ac;
	ror::Vector3<_type> m_diff_ac;
	ror::Vector3<_type> m_mul_ac;
	ror::Vector3<_type> m_div_ac;
	ror::Vector3<typename ror::Vector3<_type>::precision> m_normalized_a;
	ror::Vector3<typename ror::Vector3<_type>::precision> m_normalized_b;
	ror::Vector3<_type> m_inverse_a;
	ror::Vector3<_type> m_inverse_b;
	ror::Vector3<_type> m_cross_ab;

	_type m_scalar;
	_type m_dot_ab;
	_type m_min_a;
	_type m_max_a;
	_type m_min_b;
	_type m_max_b;
	_type m_length_a;
	_type m_length_b;
	_type m_length_sq_a;
	_type m_length_sq_b;

  protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

  private:
};

using TypesToTest = ::testing::Types<float32_t, int32_t, uint32_t, double64_t, char8_t, uchar8_t>;
TYPED_TEST_CASE(Vector3Test, TypesToTest);

template <class _type>
class Vector3TestSigned : public Vector3Test<_type>
{
  public:
	using value_type = _type;

  protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

  private:
};

using SignedTypesToTest = ::testing::Types<float32_t, int32_t, double64_t>;
TYPED_TEST_CASE(Vector3TestSigned, SignedTypesToTest);
}        // namespace ror_test

#include "vector3.inl"
