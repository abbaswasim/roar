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

#include "math/rorvector3.h"
#include "memory_manager/rormemory_check.h"

namespace ror
{
// Class Interface for all types of Bounds in Roar
class ROAR_ENGINE_ITEM IBounds
{
  public:
	IBounds(void)
	{}
	~IBounds(void)
	{}

	// Common Methods to all types of possible bounds
	// Can Add a Point to Bound
	virtual void add_point_to_bound(const Vector3f &a_point) = 0;
	// Can Add Any other type of Bound to Bound for future enhancements
	virtual void add_bound_to_bound(IBounds *a_bounding) = 0;
	// Is a Vector inside the bound
	virtual bool is_point_inside(const Vector3f &a_point) const = 0;
	// Is a Bounding Box inside the Bound
	virtual bool is_bounding_inside(IBounds *a_bounding) = 0;

	/*
//		// Can Add Bounding Box to Bound
//		virtual	void	AddBoundingBoxToBound	(IBounds *a_BoundingBox) = 0;
//		// Can Add Bounding Sphere to Bound
//		virtual	void	AddBoundingSphereToBound	(IBounds *a_BoundingSphere) = 0;
		*/
};
}        // namespace ror
