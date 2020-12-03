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

#include "rormesh.h"

namespace ror
{
Mesh::Mesh(void)
{
}

Mesh::Mesh(int32_t a_width, int32_t a_height) :
    m_width(MESH_WIDTH + 1),
    m_height(MESH_HEIGHT + 1)
{
	m_vertices = rNew Vector3f[a_width * a_height];        // the memory should be de-allocated later
	assert(m_vertices != nullptr);
	m_height_feild = glGenLists(1);        // Generate a valid List ID for mesh
	create_mesh();
	//m_Triangles = rNew CTriangle[(a_Width - 1) * (a_Height - 1) * 2];
	//assert(m_Triangles != nullptr);
}

Mesh::~Mesh(void)
{
	delete[] m_vertices;
}

// The function which makes mesh out of the Plane equation,
// this mesh will be used for collision detection later
// the method is private so that only once it is created from the constructor
void Mesh::create_mesh(void)
{
	//glNewList(m_HeightFeild,GL_COMPILE);
	float32_t y;
	//glPointSize(4);
	// Height = 0.3 * SIN( 3 * SQRT( (X-5)2+(Y-5)2 ) + 0.5*COS(X+Y) ) required
	float32_t size        = 10;
	float32_t increment_x = (2 * size) / m_width;
	float32_t increment_y = (2 * size) / m_height;
	int32_t   index       = 0;
	//glBegin(GL_POINTS);
	for (float32_t x = -size; x < size; x += increment_x)
	{
		for (float32_t z = -size; z < size; z += increment_y)
		{
			// this is the height function given for the plane
			y = 0.3f * sin(3.0f * sqrt(pow((x - 5.0f), 2.0f) + pow((z - 5.0f), 2.0f)) + 0.5f * cos(x + z));
			//glColor3f(y+0.5f,y+0.5f,y+0.5f);
			//glVertex3f(x,y,z);
			// also save the vertex in the Mesh
			m_vertices[index].x = x;
			m_vertices[index].y = y;
			m_vertices[index].z = z;
			index++;
		}
	}
	//glEnd();
	//glEndList();

	/// Now Create all the Triangles
	size  = 10;
	index = 0;
	for (int32_t x = 0; x < m_width - 1; x++)
	{
		for (int32_t y = 0; y < m_height - 1; y++)
		{
			//				m_Triangles[Index].m_P1 = m_Vertices[x + y * m_Width];
			//				m_Triangles[Index].m_P2 = m_Vertices[(x + 1) + (y + 1) * m_Width];
			//				m_Triangles[Index].m_P3 = m_Vertices[(x + 1) + y * m_Width];
			//
			//				CVector3f v1 = m_Triangles[Index].m_P3 - m_Triangles[Index].m_P1;
			//				CVector3f v2 = m_Triangles[Index].m_P2 - m_Triangles[Index].m_P1;
			//
			//				m_Triangles[Index].m_Normal = CrossProduct(v1 , v2);
			//				m_Triangles[Index].m_Normal.Normalize();
			//				CVector3f centeroid = m_Triangles[Index].m_P3 + m_Triangles[Index].m_P1 + m_Triangles[Index].m_P2 / 3;
			//				m_Triangles[Index].m_Centroid = (centeroid);
			//
			//				Index++;
			//
			//				m_Triangles[Index].m_P1 = m_Vertices[x + y * m_Width];
			//				m_Triangles[Index].m_P2 = m_Vertices[x + (y + 1) * m_Width];
			//				m_Triangles[Index].m_P3 = m_Vertices[(x + 1) + (y + 1) * m_Width];
			//
			//				v1 = m_Triangles[Index].m_P3 - m_Triangles[Index].m_P1;
			//				v2 = m_Triangles[Index].m_P2 - m_Triangles[Index].m_P1;
			//
			//				m_Triangles[Index].m_Normal = CrossProduct(v1, v2);
			//				m_Triangles[Index].m_Normal.Normalize();
			//
			//				centeroid = m_Triangles[Index].m_P3 + m_Triangles[Index].m_P1 + m_Triangles[Index].m_P2 / 3;
			//
			//				m_Triangles[Index].m_Centroid = (centeroid);
			//
			//
			//				Index++;
			//				m_Vertices[Index].y;
			//				m_Vertices[Index].z;
			//				Index++;
		}
	}
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	uint32_t no_triangles = MESH_HEIGHT * MESH_WIDTH * 2;
	//glNewList(m_HeightFeild,GL_COMPILE);
	glBegin(GL_TRIANGLES);
	for (uint32_t i = 0; i < no_triangles; i++)
	{
		if (1 == m_triangles[i].m_color)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else
			glColor3f(m_triangles[i].m_p1.y + 0.5f, m_triangles[i].m_p1.y + 0.5f, m_triangles[i].m_p1.y + 0.5f);

		glVertex3f(m_triangles[i].m_p1.x, m_triangles[i].m_p1.y, m_triangles[i].m_p1.z);
		glVertex3f(m_triangles[i].m_p2.x, m_triangles[i].m_p2.y, m_triangles[i].m_p2.z);
		glVertex3f(m_triangles[i].m_p3.x, m_triangles[i].m_p3.y, m_triangles[i].m_p3.z);
	}
	glEnd();
	//glEndList();
}

// This function is called when the mesh is to be drawn in the scene
void Mesh::draw_mesh(void)
{
	//glCallList(m_HeightFeild); // why is the list slow then the Immediate mode drawing (check Later)
	//MessageBox(nullptr, "Debug Message S","Debug",MB_OK);

	/*
		glBegin(GL_POINTS);
		for (rLong i=0;i< m_Width * m_Height; i++)
		{
			glColor3f(m_Vertices[i].y + 0.5,m_Vertices[i].y + 0.5,m_Vertices[i].y + 0.5);
			glVertex3f(m_Vertices[i].x,m_Vertices[i].y,m_Vertices[i].z);
		}
		glEnd();

		*/

	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_TRIANGLES);
	uint32_t no_triangles = MESH_HEIGHT * MESH_WIDTH * 2;
	for (uint32_t i = 0; i < no_triangles; i++)
	{
		//glColor3f(m_Triangles[i].m_P1.y + 0.5,m_Triangles[i].m_P1.y + 0.5,m_Triangles[i].m_P1.y + 0.5);
		if (1 == m_triangles[i].m_color)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else
			glColor3f(m_triangles[i].m_p1.y + 0.5f, m_triangles[i].m_p1.y + 0.5f, m_triangles[i].m_p1.y + 0.5f);

		glVertex3f(m_triangles[i].m_p1.x, m_triangles[i].m_p1.y, m_triangles[i].m_p1.z);
		glVertex3f(m_triangles[i].m_p2.x, m_triangles[i].m_p2.y, m_triangles[i].m_p2.z);
		glVertex3f(m_triangles[i].m_p3.x, m_triangles[i].m_p3.y, m_triangles[i].m_p3.z);
	}
	glEnd();

	// Draw the normals of the triangles
	/*

		glBegin(GL_LINES);
		for (rLong i=0;i < NoTriangles; i++)
		{
			glColor3f(1.0f,0.0f,0.0f);
			CVector3f temp = m_Triangles[i].m_Centroid + (m_Triangles[i].m_Normal * 0.01);
			glVertex3f(temp.x,temp.y,temp.z);
			glVertex3f(m_Triangles[i].m_Centroid.x,m_Triangles[i].m_Centroid.y,m_Triangles[i].m_Centroid.z);
		}
		glEnd();

*/
}

Mesh::Triangle Mesh::check_triangle(game::Ball &a_ball)
{
	//http://www.blackpawn.com/texts/pointinpoly/default.html
	//Barycentric Technique
	// Compute vectors
	/*
		v0 = C - A
		v1 = B - A
		v2 = P - A

		// Compute dot products
		dot00 = dot(v0, v0)
		dot01 = dot(v0, v1)
		dot02 = dot(v0, v2)
		dot11 = dot(v1, v1)
		dot12 = dot(v1, v2)

		// Compute barycentric coordinates
		invDenom = 1 / (dot00 * dot11 - dot01 * dot01)
		u = (dot11 * dot02 - dot01 * dot12) * invDenom
		v = (dot00 * dot12 - dot01 * dot02) * invDenom

		// Check if point is in triangle
		return (u > 0) && (v > 0) && (u + v < 1)
		*/

	Mesh::Triangle temp;
	int32_t        no_triangles = MESH_HEIGHT * MESH_WIDTH * 2;
	Vector3f       point        = a_ball.get_current_position();
	//		Point.y = Point.z;
	//		Point.z = 0.0f;
	//		for (rLong Index = 0;Index < NoTriangles; Index++)
	//		{
	//			// Compute vectors
	//			CVector3f v0 = m_Triangles[Index].m_P3 - m_Triangles[Index].m_P1;
	//			CVector3f v1 = m_Triangles[Index].m_P2 - m_Triangles[Index].m_P1;
	//			CVector3f v2 = Point - m_Triangles[Index].m_P1;
	//
	// //			v2.y = v2.z;
	// //			v2.z = 0.0f;
	// //			v1.y = v1.z;
	// //			v1.z = 0.0f;
	// //			v0.y = v0.z;
	// //			v0.z = 0.0f;
	//
	//			// Compute dot products
	//			float32_t dot00 = v0 * v0;
	//			float32_t dot01 = v0 * v1;
	//			float32_t dot02 = v0 * v2;
	//			float32_t dot11 = v1 * v1;
	//			float32_t dot12 = v1 * v2;
	//
	//			// Compute barycentric coordinates
	//			float32_t invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	//			float32_t u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	//			float32_t v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	//
	//			// Check if point is in triangle
	//			if ( (u > 0) && (v > 0) && (u + v < 1))
	//			{
	//				m_Triangles[Index].SetColor(1);
	//				return  m_Triangles[Index];
	//			}
	//		}
	return temp;
}
}        // namespace ror
