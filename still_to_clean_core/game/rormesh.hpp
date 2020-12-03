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

#include "math/rormath_utility.h"
#include "memory_manager/rormemory_check.h"
#include "rorball.h"

namespace ror
{
#define MESH_HEIGHT 199
#define MESH_WIDTH 199

class ROAR_ENGINE_ITEM Mesh
{
  public:
	class ROAR_ENGINE_ITEM Triangle
	{
	  public:
		Triangle()
		{
			m_p1.zero();
			m_p2.zero();
			m_p3.zero();

			m_normal.zero();

			m_centroid.zero();

			m_color = 0;
		}
		~Triangle()
		{}
		Vector3f  m_p1, m_p2, m_p3;
		Vector3f  m_normal;
		Vector3f  m_centroid;
		float32_t m_color;

		void set_color(float32_t a_color)
		{
			m_color = a_color;
		}
	};
	Mesh(void);
	Mesh(int32_t a_width, int32_t a_height);        // the memory should be de-allocated later
	~Mesh(void);                                    // dtor must free the dynamic memory allocated
  private:
	// The list of all the vertices in the mesh
	Vector3f *m_vertices;
	int32_t   m_width, m_height;
	uint32_t  m_height_feild;        // Mesh ID used for GenLists
	Triangle  m_triangles[MESH_HEIGHT * MESH_WIDTH * 2];

	// The function which makes mesh out of the Plane equation,
	// This mesh will be used for collision detection later
	// The method is private so that only once it is created from the constructor
	void create_mesh(void);

  public:
	// This function is called when the mesh is to be drawn in the scene
	void    draw_mesh(void);
	int32_t get_height()
	{
		return m_height;
	}
	int32_t get_width()
	{
		return m_width;
	}
	Vector3f get_vertices(int32_t a_index) const
	{
		return m_vertices[a_index];
	}
	Triangle check_triangle(game::Ball &a_ball);
};

}        // namespace ror
