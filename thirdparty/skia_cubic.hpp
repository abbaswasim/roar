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

// Copyright (c) 2011 Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "foundation/rorutilities.hpp"
#include "math/rorsegment.hpp"

/**
 * @brief      Strip out of Skia's cubic bezier functions
 * @details    Instead of using the whole Skia library, I have stripped out some of its
 *             Cubic bezier functions
 */

namespace ror
{
typedef float32_t SkScalar;
#define SK_Scalar1 1.0f

class SkPoint
{
  public:
	SkScalar fX;        //!< x-axis value
	SkScalar fY;        //!< y-axis value

	SkPoint() :
		fX(0),
		fY(0)
	{}

	SkPoint(SkScalar a, SkScalar b)
	{
		fX = a;
		fY = b;
	}

	SkPoint operator-(const SkPoint &b) const
	{
		return {fX - b.fX, fY - b.fY};
	}

	SkPoint operator+(const SkPoint &b) const
	{
		return {fX + b.fX, fY + b.fY};
	}

	SkPoint operator*(SkScalar scale) const
	{
		return {fX * scale, fY * scale};
	}

	void scale(SkScalar factor)
	{
		fX *= factor;
		fY *= factor;
	}
};

SkPoint interpolate(SkPoint p1, SkPoint p2, SkScalar t)
{
	return SkPoint(ror::interpolate(p1.fX, p2.fY, t), ror::interpolate(p1.fY, p2.fY, t));
}

SkScalar length_sqd(SkPoint p)
{
	return p.fX * p.fX + p.fY * p.fY;
}

SkScalar distance_to_sqd(const SkPoint &pt, const SkPoint &a)
{
	SkScalar dx = pt.fX - a.fX;
	SkScalar dy = pt.fY - a.fY;

	return dx * dx + dy * dy;
}

void sk_chop_cubic_at(const SkPoint src[4], SkPoint dst[7], SkScalar tt)
{
	assert(tt > 0 && tt < SK_Scalar1);

	SkPoint ab   = interpolate(src[0], src[1], tt);
	SkPoint bc   = interpolate(src[1], src[2], tt);
	SkPoint cd   = interpolate(src[2], src[3], tt);
	SkPoint abc  = interpolate(ab, bc, tt);
	SkPoint bcd  = interpolate(bc, cd, tt);
	SkPoint abcd = interpolate(abc, bcd, tt);

	dst[0] = src[0];
	dst[1] = ab;
	dst[2] = abc;
	dst[3] = abcd;
	dst[4] = bcd;
	dst[5] = cd;
	dst[6] = src[3];
}

int valid_unit_divide(SkScalar numer, SkScalar denom, SkScalar *ratio)
{
	assert(ratio);

	if (numer < 0)
	{
		numer = -numer;
		denom = -denom;
	}

	if (ror::equal_zero(denom) || ror::equal_zero(numer) || numer >= denom)
	{
		return 0;
	}

	SkScalar r = numer / denom;
	if (ror::is_nan(r))
	{
		return 0;
	}
	assert(r >= 0 && r < SK_Scalar1);        //, "numer %f, denom %f, r %f", numer, denom, r);
	if (ror::equal_zero(r))
	{        // catch underflow if numer <<<< denom
		return 0;
	}
	*ratio = r;
	return 1;
}

void sk_chop_cubic_at(const SkPoint src[4], SkPoint dst[],
					  const SkScalar tValues[], int roots)
{
#ifdef _DEBUG
	{
		for (int i = 0; i < roots - 1; i++)
		{
			assert(0 < tValues[i] && tValues[i] < 1);
			assert(0 < tValues[i + 1] && tValues[i + 1] < 1);
			assert(tValues[i] < tValues[i + 1]);
		}
	}
#endif

	if (dst)
	{
		if (roots == 0)
		{        // nothing to chop
			memcpy(dst, src, 4 * sizeof(SkPoint));
		}
		else
		{
			SkScalar t = tValues[0];
			SkPoint  tmp[4];

			for (int i = 0; i < roots; i++)
			{
				sk_chop_cubic_at(src, dst, t);
				if (i == roots - 1)
				{
					break;
				}

				dst += 3;
				// have src point to the remaining cubic (after the chop)
				memcpy(tmp, dst, 4 * sizeof(SkPoint));
				src = tmp;

				// watch out in case the renormalized t isn't in range
				if (!valid_unit_divide(tValues[i + 1] - tValues[i],
									   SK_Scalar1 - tValues[i], &t))
				{
					// if we can't, just create a degenerate cubic
					dst[4] = dst[5] = dst[6] = src[3];
					break;
				}
			}
		}
	}
}

void convert_noninflect_cubic_to_quads(const SkPoint         p[4],
									   SkScalar              tolerance_sqd,
									   std::vector<SkPoint> *quads,
									   int                   sublevel               = 0,
									   bool                  preserve_first_tangent = true,
									   bool                  preserve_last_tangent  = true)
{
	// Notation: Point a is always p[0]. Point b is p[1] unless p[1] == p[0], in which case it is
	// p[2]. Point d is always p[3]. Point c is p[2] unless p[2] == p[3], in which case it is p[1].
	SkPoint ab = p[1] - p[0];
	SkPoint dc = p[2] - p[3];

	if (length_sqd(ab) < ror::ror_epsilon)
	{
		if (length_sqd(dc) < ror::ror_epsilon)
		{
			quads->emplace_back(p[0]);
			quads->emplace_back(p[0]);
			quads->emplace_back(p[3]);

			return;
		}
		ab = p[2] - p[0];
	}
	if (length_sqd(dc) < ror::ror_epsilon)
	{
		dc = p[1] - p[3];
	}

	static const SkScalar k_length_scale = 3.0f * SK_Scalar1 / 2.0f;
	static const int      k_max_subdivs  = 10;

	ab.scale(k_length_scale);
	dc.scale(k_length_scale);

	// c0 and c1 are extrapolations along vectors ab and dc.
	SkPoint c0 = p[0] + ab;
	SkPoint c1 = p[3] + dc;

	SkScalar dSqd = sublevel > k_max_subdivs ? 0 : distance_to_sqd(c0, c1);
	if (dSqd < tolerance_sqd)
	{
		SkPoint newC;
		if (preserve_first_tangent == preserve_last_tangent)
		{
			// We used to force a split when both tangents need to be preserved and c0 != c1.
			// This introduced a large performance regression for tiny paths for no noticeable
			// quality improvement. However, we aren't quite fulfilling our contract of guaranteeing
			// the two tangent vectors and this could introduce a missed pixel in
			// GrAAHairlinePathRenderer.
			newC = (c0 + c1) * 0.5f;
		}
		else if (preserve_first_tangent)
		{
			newC = c0;
		}
		else
		{
			newC = c1;
		}

		quads->emplace_back(p[0]);
		quads->emplace_back(newC);
		quads->emplace_back(p[3]);

		return;
	}
	SkPoint choppedPts[7];
	sk_chop_cubic_at(p, choppedPts, 0.5f);
	convert_noninflect_cubic_to_quads(
		choppedPts + 0, tolerance_sqd, quads, sublevel + 1, preserve_first_tangent, false);
	convert_noninflect_cubic_to_quads(
		choppedPts + 3, tolerance_sqd, quads, sublevel + 1, false, preserve_last_tangent);
}

static int return_check_zero(int value)
{
	if (value == 0)
	{
		return 0;
	}
	return value;
}

void sk_chop_quad_at(const SkPoint src[3], SkPoint dst[5], SkScalar tt)
{
	assert(tt > 0 && tt < SK_Scalar1);

	SkPoint p01 = interpolate(src[0], src[1], tt);
	SkPoint p12 = interpolate(src[1], src[2], tt);

	dst[0] = src[0];
	dst[1] = p01;
	dst[2] = interpolate(p01, p12, tt);
	dst[3] = p12;
	dst[4] = src[2];
}

void sk_chop_quad_at_half(const SkPoint src[3], SkPoint dst[5])
{
	sk_chop_quad_at(src, dst, 0.5f);
}

constexpr int32_t g_float_bits_exponent_mask = 0x7F800000;
constexpr int32_t g_float_bits_matissa_mask  = 0x007FFFFF;

inline bool sk_float_bits_is_finite(int32_t bits)
{
	return (bits & g_float_bits_exponent_mask) != g_float_bits_exponent_mask;
}

union SkFloatIntUnion
{
	float   fFloat;
	int32_t fSignBitInt;
};

// Helper to see a float as its bit pattern (w/o aliasing warnings)
inline int32_t sk_float2_bits(float x)
{
	SkFloatIntUnion data;
	data.fFloat = x;
	return data.fSignBitInt;
}

inline bool sk_float_is_finite(float x)
{
	return sk_float_bits_is_finite(sk_float2_bits(x));
}

int sk_find_unit_quad_roots(SkScalar A, SkScalar B, SkScalar C, SkScalar roots[2])
{
	assert(roots);

	if (ror::equal_zero(A))
	{
		return return_check_zero(valid_unit_divide(-C, B, roots));
	}

	SkScalar *r = roots;

	// use doubles so we don't overflow temporarily trying to compute R
	double dr = static_cast<double>(B * B) - 4.0 * static_cast<double>(A * C);

	if (dr < 0)
	{
		return return_check_zero(0);
	}
	dr         = sqrt(dr);
	SkScalar R = static_cast<SkScalar>(dr);
	if (!sk_float_is_finite(R))
	{
		return return_check_zero(0);
	}

	SkScalar Q = (B < 0) ? -(B - R) / 2 : -(B + R) / 2;
	r += valid_unit_divide(Q, A, r);
	r += valid_unit_divide(C, Q, r);
	if (r - roots == 2)
	{
		if (roots[0] > roots[1])
		{
			using std::swap;
			swap(roots[0], roots[1]);
		}
		else if (ror::decimal_equal(roots[0], roots[1]))
		{                  // nearly-equal?
			r -= 1;        // skip the double root
		}
	}
	return return_check_zero(static_cast<int>(r - roots));
}

int sk_find_cubic_inflections(const SkPoint src[4], SkScalar tValues[])
{
	SkScalar Ax = src[1].fX - src[0].fX;
	SkScalar Ay = src[1].fY - src[0].fY;
	SkScalar Bx = src[2].fX - 2 * src[1].fX + src[0].fX;
	SkScalar By = src[2].fY - 2 * src[1].fY + src[0].fY;
	SkScalar Cx = src[3].fX + 3 * (src[1].fX - src[2].fX) - src[0].fX;
	SkScalar Cy = src[3].fY + 3 * (src[1].fY - src[2].fY) - src[0].fY;

	return sk_find_unit_quad_roots(Bx * Cy - By * Cx,
								   Ax * Cy - Ay * Cx,
								   Ax * By - Ay * Bx,
								   tValues);
}

int sk_chop_cubic_at_inflections(const SkPoint src[], SkPoint dst[10])
{
	SkScalar tValues[2];
	int      count = sk_find_cubic_inflections(src, tValues);

	if (dst)
	{
		if (count == 0)
		{
			memcpy(dst, src, 4 * sizeof(SkPoint));
		}
		else
		{
			sk_chop_cubic_at(src, dst, tValues, count);
		}
	}
	return count + 1;
}

// tolScale can be 1.0f
void sk_convert_cubic_to_quads(const SkPoint         p[4],
							   SkScalar              tolScale,
							   std::vector<SkPoint> *quads)
{
	SkPoint chopped[10];
	int     count = sk_chop_cubic_at_inflections(p, chopped);

	const SkScalar tolSqd = tolScale * tolScale;

	for (int i = 0; i < count; ++i)
	{
		SkPoint *cubic = chopped + 3 * i;
		convert_noninflect_cubic_to_quads(cubic, tolSqd, quads);
	}
}

// Ugly plumbing to Skia cubic convert function
template <class _type>
void convert_cubic_to_quadratic(const ror::Bezier<ror::Vector2<_type>, 3>         a_cubic_bezier,
								std::vector<ror::Bezier<ror::Vector2<_type>, 2>> &a_children)
{
	SkPoint p[4];
	for (size_t i = 0; i < 4; ++i)
	{
		p[i].fX = static_cast<SkScalar>(a_cubic_bezier.m_points[i].x);
		p[i].fY = static_cast<SkScalar>(a_cubic_bezier.m_points[i].y);
	}

	std::vector<SkPoint> bezies;
	sk_convert_cubic_to_quads(p, 1.0f, &bezies);

	size_t beziers_count = bezies.size();
	for (size_t i = 0; i < beziers_count; i += 3)
	{
		a_children.emplace_back(ror::Vector2<_type>(static_cast<_type>(bezies[i].fX), static_cast<_type>(bezies[i].fY)),
								ror::Vector2<_type>(static_cast<_type>(bezies[i + 1].fX), static_cast<_type>(bezies[i + 1].fY)),
								ror::Vector2<_type>(static_cast<_type>(bezies[i + 2].fX), static_cast<_type>(bezies[i + 2].fY)));
	}
}

}        // namespace ror
