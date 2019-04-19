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

namespace ror
{
FORCE_INLINE BoundingBox::BoundingBox(void) :
    m_minimum(g_zero_vector3f),
    m_maximum(g_zero_vector3f)
{
}

FORCE_INLINE Vector3f BoundingBox::get_minimum() const
{
	return this->m_minimum;
}

FORCE_INLINE void BoundingBox::set_minimum(Vector3f &a_minimum)
{
	this->m_minimum = a_minimum;
}

FORCE_INLINE Vector3f BoundingBox::get_maximum() const
{
	return this->m_maximum;
}

FORCE_INLINE void BoundingBox::set_maximum(Vector3f &a_maximum)
{
	this->m_maximum = a_maximum;
}

FORCE_INLINE void BoundingBox::add_bounding_box(BoundingBox *a_bounding_box)
{
	if (a_bounding_box->m_minimum.x < this->m_minimum.x)
		this->m_minimum.x = a_bounding_box->m_minimum.x;
	if (a_bounding_box->m_minimum.x > this->m_maximum.x)
		this->m_maximum.x = a_bounding_box->m_minimum.x;
	if (a_bounding_box->m_minimum.y < this->m_minimum.x)
		this->m_minimum.y = a_bounding_box->m_minimum.y;
	if (a_bounding_box->m_minimum.y > this->m_maximum.x)
		this->m_maximum.y = a_bounding_box->m_minimum.y;
	if (a_bounding_box->m_minimum.z < this->m_minimum.x)
		this->m_minimum.z = a_bounding_box->m_minimum.z;
	if (a_bounding_box->m_minimum.z > this->m_maximum.x)
		this->m_maximum.z = a_bounding_box->m_minimum.z;
}

FORCE_INLINE void BoundingBox::add_point_to_bound(const Vector3f &a_point)
{
	if (a_point.x < this->m_minimum.x)
		this->m_minimum.x = a_point.x;
	if (a_point.x > this->m_maximum.x)
		this->m_maximum.x = a_point.x;
	if (a_point.y < this->m_minimum.x)
		this->m_minimum.y = a_point.y;
	if (a_point.y > this->m_maximum.x)
		this->m_maximum.y = a_point.y;
	if (a_point.z < this->m_minimum.x)
		this->m_minimum.z = a_point.z;
	if (a_point.z > this->m_maximum.x)
		this->m_maximum.z = a_point.z;
}

FORCE_INLINE void BoundingBox::add_bound_to_bound(IBounds *a_bounding)
{
	BoundingBox *bounding_box = dynamic_cast<BoundingBox *>(a_bounding);
	if (bounding_box)
	{
		this->add_bounding_box(bounding_box);
	}
	//////////////////////////////////////////////////////////////////////////
	// Checks for Other types of Derived Bounds goes here
}

FORCE_INLINE bool BoundingBox::is_point_inside(const Vector3f &a_point) const
{
	return (a_point.x >= this->m_minimum.x) && (a_point.x <= this->m_maximum.x) &&
	       (a_point.y >= this->m_minimum.y) && (a_point.y <= this->m_maximum.y) &&
	       (a_point.z >= this->m_minimum.z) && (a_point.z <= this->m_maximum.z);
}

FORCE_INLINE bool BoundingBox::is_bounding_inside(IBounds *a_bounding)
{
	BoundingBox *bounding_box  = dynamic_cast<BoundingBox *>(a_bounding);
	bool         return_result = false;
	if (bounding_box)
	{
		return this->is_bounding_box_inside(bounding_box);
	}
	//////////////////////////////////////////////////////////////////////////
	// Checks for Other types of Derived Bounds goes here
	return false;
}

FORCE_INLINE bool BoundingBox::is_bounding_box_inside(BoundingBox *a_bounding_box)
{
	if (a_bounding_box)
	{
		return (
		    is_point_inside(a_bounding_box->m_minimum) &&
		    is_point_inside(a_bounding_box->m_maximum) &&

		    is_point_inside(Vector3f(a_bounding_box->m_minimum.x, a_bounding_box->m_minimum.y, a_bounding_box->m_maximum.z)) &&
		    is_point_inside(Vector3f(a_bounding_box->m_minimum.x, a_bounding_box->m_maximum.y, a_bounding_box->m_minimum.z)) &&
		    is_point_inside(Vector3f(a_bounding_box->m_maximum.x, a_bounding_box->m_minimum.y, a_bounding_box->m_minimum.z)) &&

		    is_point_inside(Vector3f(a_bounding_box->m_maximum.x, a_bounding_box->m_maximum.y, a_bounding_box->m_minimum.z)) &&
		    is_point_inside(Vector3f(a_bounding_box->m_maximum.x, a_bounding_box->m_minimum.y, a_bounding_box->m_maximum.z)) &&
		    is_point_inside(Vector3f(a_bounding_box->m_minimum.x, a_bounding_box->m_maximum.y, a_bounding_box->m_maximum.z)));
	}
	return false;
}

}        // namespace ror
