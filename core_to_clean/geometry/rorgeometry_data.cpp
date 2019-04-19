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

#include "rorgeometry_data.h"

namespace ror
{
GeometryData::~GeometryData(void)
{
	rDELETEARRAY(m_vertices_array);
	rDELETEARRAY(m_skinned_vertices_array);
	rDELETEARRAY(m_normals_array);
	rDELETEARRAY(m_skinned_normals_array);
	rDELETEARRAY(m_texture_coords_array);
	rDELETEARRAY(m_vertex_weights_array);

	uint32_t                                 size = (uint32_t) this->m_vertex_influences->size();
	std::vector<VertexInfluence *>           temp;
	std::vector<VertexInfluence *>::iterator deleted;
	for (uint32_t i = 0; i < size; i++)
	{
		deleted = find(temp.begin(), temp.end(), (*m_vertex_influences)[i]);
		if (deleted == temp.end())
		{
			temp.push_back((*m_vertex_influences)[i]);
			rDELETE((*m_vertex_influences)[i]);
		}
	}
	rDELETE(m_vertex_influences);
}

}        // namespace ror
