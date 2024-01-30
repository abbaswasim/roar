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
#include "profiling/rorlog.hpp"
#include "rorfrustum.hpp"

namespace ror
{

/**
 * @brief      Calculates frustum corners in world space
 * @details    This method calculates frustum corners in world space using view transform only.
               Usually one would transform NDC space frustum corners into worldspace by perspective transforming with view_projection.
               I can't use this because I am using infinite projection matrix which brings NDC points behind camera.
               Instead I am using a method described in Chapter 6 of Fundamentals of Game Engine Volume 2 by Eric Lengyel
 * @param      a_view   The viewing matrix
 */
void Frustum::setup(const ror::Matrix4f &a_view)
{
	float32_t s = this->m_aspect;
	float32_t g = 1.0f / std::tan(ror::to_radians(this->m_fov / 2.0f));
	// float32_t g = s / std::tan(ror::to_radians(this->m_fov / 2.0f)); // This one is required if m_fov is horizonal fov
	float32_t n = this->m_near;
	float32_t f = this->m_far;

	ror::Vector4f frustum_corners[8] = {
	    {-(n * s) / g, -n / g, -n, 1.0f},
	    {(n * s) / g, -n / g, -n, 1.0f},
	    {(n * s) / g, n / g, -n, 1.0f},
	    {-(n * s) / g, n / g, -n, 1.0f},
	    {-(f * s) / g, -f / g, -f, 1.0f},
	    {(f * s) / g, -f / g, -f, 1.0f},
	    {(f * s) / g, f / g, -f, 1.0f},
	    {-(f * s) / g, f / g, -f, 1.0f},
	};

	this->m_view = a_view;

	auto result = this->m_view.inverse(this->m_view_inverse);
	assert(result && "Can't invert view matrix");
	(void) result;

	for (int i = 0; i < 8; ++i)
	{
		auto cs = this->m_view_inverse * frustum_corners[i];

		this->m_corners[i] = ror::Vector3f(cs);
		this->m_center     = this->m_center + this->m_corners[i];
	}

	this->m_center = this->m_center / 8.0f;
}

}        // namespace ror
