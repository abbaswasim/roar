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

#include "interfaces/roribounds.h"

namespace ror
{
class ROAR_ENGINE_ITEM BoundingBox : public IBounds
{
  public:
	FORCE_INLINE BoundingBox(void);
	~BoundingBox(void);

	// Methods from the base must be override
	// Can Add a Point to Bound
	FORCE_INLINE void add_point_to_bound(const Vector3f &a_point);
	// Can Add Any other type of Bound to Bound for future enhancements
	FORCE_INLINE void add_bound_to_bound(IBounds *a_bounding);
	// Is a Vector inside the bound
	FORCE_INLINE bool is_point_inside(const Vector3f &a_point) const;
	// Is a Bounding Box inside the Bound
	FORCE_INLINE bool is_bounding_inside(IBounds *a_bounding);

	// Can Add a Bounding Box to Me
	// Safe Version of AddBoundToBound in Derived Class
	FORCE_INLINE void add_bounding_box(BoundingBox *a_bounding_box);
	FORCE_INLINE bool is_bounding_box_inside(BoundingBox *a_bounding_box);

	// Accessors and Mutator functions
	FORCE_INLINE Vector3f get_minimum() const;
	FORCE_INLINE void     set_minimum(Vector3f &a_minimum);
	FORCE_INLINE Vector3f get_maximum() const;
	FORCE_INLINE void     set_maximum(Vector3f &a_maximum);

  private:
	// Data Needed for the BoundingSphere
	Vector3f m_minimum;        // Minimum Bound of the Box
	Vector3f m_maximum;        // Maximum Bound of the Box
};

}        // namespace ror
#include "rorbounding_box.inl"
