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
FORCE_INLINE TriangleGroup::TriangleGroup(void)
{
	//m_Triangles = new std::vector<CTriangle>();
	m_material = rNew Material();
}

FORCE_INLINE Triangle *TriangleGroup::get_triangles() const
{
	return m_triangles;
}

FORCE_INLINE void TriangleGroup::set_triangles(Triangle *a_triangles)
{
	rDELETEARRAY(m_triangles);
	m_triangles = a_triangles;
}

FORCE_INLINE Material *TriangleGroup::get_material() const
{
	return m_material;
}

FORCE_INLINE void TriangleGroup::set_material(Material *a_material)
{
	rDELETE(m_material);
	m_material = a_material;
}

FORCE_INLINE uint32_t TriangleGroup::get_triangles_size() const
{
	return m_no_of_triangles;
}

FORCE_INLINE void TriangleGroup::set_triangles_size(uint32_t a_no_of_triangles)
{
	this->m_no_of_triangles = a_no_of_triangles;
}

}        // namespace ror
