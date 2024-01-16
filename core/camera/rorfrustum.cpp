// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2024
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

#include "math/rormatrix4.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include "math/rorvector_functions.hpp"
#include "rorfrustum.hpp"

namespace ror
{

void Frustum::setup(const ror::Matrix4f &a_view_projection)
{
	// Get the 8 points of the view frustum in world space

	ror::Vector4f frustumCorners[8] = {
	    {-1.0f,  1.0f, 0.0f, 1.0f},
	    { 1.0f,  1.0f, 0.0f, 1.0f},
	    { 1.0f, -1.0f, 0.0f, 1.0f},
	    {-1.0f, -1.0f, 0.0f, 1.0f},
	    {-1.0f,  1.0f, 1.0f, 1.0f},
	    { 1.0f,  1.0f, 1.0f, 1.0f},
	    { 1.0f, -1.0f, 1.0f, 1.0f},
	    {-1.0f, -1.0f, 1.0f, 1.0f},
	};

	this->m_view_projection = a_view_projection;

	auto result = this->m_view_projection.inverse(this->m_view_projection_inverse);
	assert(result && "Can't invert view projection");
	(void) result;

	for (int i = 0; i < 8; ++i)
	{
		auto cs = this->m_view_projection_inverse * frustumCorners[i];
		cs      = cs / cs.w;

		this->m_corners[i] = ror::Vector3f(cs);
		this->m_center     = this->m_center + this->m_corners[i];
	}

	this->m_center = this->m_center / 8.0f;
}

}        // namespace ror
