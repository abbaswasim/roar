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
FORCE_INLINE BoundingSphere::BoundingSphere(void) :
    m_center(g_zero_vector3f),
    m_radius(0.0f)
{
}

FORCE_INLINE Vector3f BoundingSphere::get_center() const
{
	return m_center;
}

FORCE_INLINE void BoundingSphere::set_center(Vector3f &a_center)
{
	m_center             = a_center;
	m_center_transformed = m_center;
}

FORCE_INLINE Vector3f BoundingSphere::get_center_transformed() const
{
	return m_center_transformed;
}

FORCE_INLINE void BoundingSphere::set_center_transformed(Vector3f &a_center)
{
	m_center_transformed = a_center;
}

FORCE_INLINE float32_t BoundingSphere::get_radius() const
{
	return m_radius;
}

FORCE_INLINE void BoundingSphere::set_radius(float32_t a_radius)
{
	m_radius = a_radius;
}

FORCE_INLINE bool BoundingSphere::is_point_inside(const Vector3f &a_point) const
{
	float32_t rough_diameter = this->m_center.distance(a_point);
	if (rough_diameter > this->m_radius)
	{
		return false;
	}
	return true;
}

FORCE_INLINE void BoundingSphere::add_bound_to_bound(IBounds *a_bounding)
{
	BoundingSphere *bounding_sphere = dynamic_cast<BoundingSphere *>(a_bounding);
	if (bounding_sphere)
	{
		this->add_bounding_sphere(bounding_sphere);
	}
	//////////////////////////////////////////////////////////////////////////
	// Checks for Other types of Derived Bounds goes here
}

FORCE_INLINE void BoundingSphere::add_point_to_bound(const Vector3f &a_point)
{
	float32_t rough_diameter = this->m_center.distance(a_point);
	if (rough_diameter > this->m_radius)
	{
		// www.cs.sjsu.edu/~teoh/teaching/cs134/lectures/Lecture20_SceneGraph.ppt
		// The following algorithm is available on the above website

		rough_diameter += this->m_radius;
		Vector3f V = a_point - this->m_center;
		V.normalize();
		this->m_center = this->m_center + V * ((rough_diameter / 2.0f) - this->m_radius);
		this->m_radius = rough_diameter / 2.0f;
	}
}

FORCE_INLINE bool BoundingSphere::is_bounding_inside(IBounds *a_bounding)
{
	BoundingSphere *bounding_sphere = dynamic_cast<BoundingSphere *>(a_bounding);
	if (bounding_sphere)
	{
		return is_bounding_sphere_inside(bounding_sphere);
	}
	//////////////////////////////////////////////////////////////////////////
	// Checks for Other types of Derived Bounds goes here
	return false;
}

FORCE_INLINE void BoundingSphere::add_bounding_sphere(const BoundingSphere *a_bounding_sphere)
{
	float32_t rough_diameter = this->m_center.distance(a_bounding_sphere->m_center) + a_bounding_sphere->m_radius;
	if (rough_diameter > this->m_radius)
	{
		// www.cs.sjsu.edu/~teoh/teaching/cs134/lectures/Lecture20_SceneGraph.ppt
		// The following algorithm is available on the above website

		rough_diameter += this->m_radius;
		Vector3f V = a_bounding_sphere->m_center - this->m_center;
		V.normalize();
		this->m_center = this->m_center + V * ((rough_diameter / 2.0f) - this->m_radius);
		this->m_radius = rough_diameter / 2.0f;
	}
}

FORCE_INLINE bool BoundingSphere::is_bounding_sphere_inside(const BoundingSphere *a_bounding_sphere) const
{
	if (a_bounding_sphere)
	{
		float32_t rough_diameter = this->m_center.distance(a_bounding_sphere->m_center) + a_bounding_sphere->m_radius;
		if (rough_diameter > this->m_radius)
		{
			return false;
		}
	}
	return true;
	// Possible Improvements in Performance (Profile Later)
	/*
		// get the separating axis
		Vector3f vSepAxis = this->Center() - refSphere.Center();
		// get the sum of the radii
		float32_t fRadiiSum = this->Radius() + refSphere.Radius();
		// if the distance between the centers is less than the sum
		// of the radii, then we have an intersection
		// we calculate this using the squared lengths for speed
		if(vSepAxis.getSqLength() < (fRadiiSum * fRadiiSum))
			return(true);
		// otherwise they are separated
		return(false);
		*/
	////
}

FORCE_INLINE void BoundingSphere::draw_bound() const
{
	// Draw the bound at the current location
	//		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_COLOR_MATERIAL);
	//		glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0f, 1.0f, 0.0f);
	ror::Utility::draw_sphere(m_center.x, m_center.y, m_center.z, m_radius, 8, 8);
	//CoreEngine::CUtility::DrawSphere(m_CenterTransformed.x,m_CenterTransformed.y,m_CenterTransformed.z,m_Radius,8,8);
	//		glDisable(GL_COLOR_MATERIAL);
	//		glPopAttrib();
}

}        // namespace ror
