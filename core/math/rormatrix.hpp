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

#include "rormatrix2.hpp"
#include "rormatrix3.hpp"
#include "rormatrix3x4.hpp"
#include "rormatrix4.hpp"

#include "rormatrix2_functions.hpp"
#include "rormatrix3_functions.hpp"
#include "rormatrix3x4_functions.hpp"
#include "rormatrix4_functions.hpp"

static_assert(sizeof(ror::Matrix2f) == 16, "Matrix is not tightly packed");
static_assert(sizeof(ror::Matrix3f) == 36, "Matrix is not tightly packed");
static_assert(sizeof(ror::Matrix3x4f) == 48, "Matrix is not tightly packed");
static_assert(sizeof(ror::Matrix4f) == 64, "Matrix is not tightly packed");

static_assert(sizeof(ror::Matrix2i) == 16, "Matrix is not tightly packed");
static_assert(sizeof(ror::Matrix3i) == 36, "Matrix is not tightly packed");
static_assert(sizeof(ror::Matrix3x4i) == 48, "Matrix is not tightly packed");
static_assert(sizeof(ror::Matrix4i) == 64, "Matrix is not tightly packed");

static_assert(sizeof(ror::Matrix2d) == 32, "Matrix is not tightly packed");
static_assert(sizeof(ror::Matrix3d) == 72, "Matrix is not tightly packed");
static_assert(sizeof(ror::Matrix3x4d) == 96, "Matrix is not tightly packed");
static_assert(sizeof(ror::Matrix4d) == 128, "Matrix is not tightly packed");

namespace ror
{}        // namespace ror
