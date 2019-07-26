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

#include "common.hpp"
#include "math/rormatrix.hpp"
#include "math/rorquaternion.hpp"
#include <gtest/gtest.h>

namespace ror_test
{
template <class _type>
class Matrix3x4Test : public testing::Test
{
  public:
	_type m_c = 35.0;
	ror::Matrix3x4<_type> m_a;
	ror::Matrix3x4<_type> m_b;

  protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

  private:
};

using Matrix3x4TypesToTest = ::testing::Types<float32_t, int32_t, double64_t>;
TYPED_TEST_SUITE(Matrix3x4Test, Matrix3x4TypesToTest);

template <class _type>
class Matrix3x4TestSigned : public Matrix3x4Test<_type>
{
  public:
	using value_type = _type;

  protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

  private:
};

using Matrix3x4SignedTypesToTest = ::testing::Types<float32_t, double64_t>;
TYPED_TEST_SUITE(Matrix3x4TestSigned, Matrix3x4SignedTypesToTest);
}        // namespace ror_test

#include "matrix3x4.hh"
