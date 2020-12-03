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
FORCE_INLINE TerrainNode::TerrainNode(void)
{
	//m_GeometryData = rNew CoreEngine::CCBA();
}

FORCE_INLINE void TerrainNode::global_draw()
{
	GeometryNode::global_draw();
	// Further Lets do the Billboard Drawings too
	this->draw_billboards();
}

FORCE_INLINE void TerrainNode::load_geometry(std::string a_file_name)
{
	ror::GeometryNode::load_geometry(a_file_name);
	//Further Lets do the Billboard Loading too
	this->load_billboards();
}

FORCE_INLINE void TerrainNode::set_geometry(ror::CCBA *a_cba)
{
	GeometryNode::set_geometry(a_cba);
	// Further Lets do the Billboard Loading too
	this->load_billboards();
}

FORCE_INLINE void TerrainNode::load_billboards()
{
	//std::string HeightMapFileName = "Terrain.TGA";
	std::string grass_map_file_name = "Assets/Textures/Terrain2.TGA";
	std::string billboard_file_name = "Assets/Textures/Grass.TGA";

	//HeightMapFileName	= WORKINGDIRECTORY + HeightMapFileName;
	grass_map_file_name = WORKINGDIRECTORY + grass_map_file_name;

	this->m_billboard = rNew ror::BillboardNode();
	this->m_billboard->load_geometry(billboard_file_name);

	ror::TextureImage *grass_map = rNew ror::TextureImage();
	grass_map->load_image_file(grass_map_file_name);
	rUsChar **grass_map_data = grass_map->get_pixel_data();

	// Now we will use the CBA to update the Height information for Grass
	//		FILE *p;
	//		fopen_s(&p,"d:/temp.raw","wb");

	assert(m_geometry_data->get_header()->get_geometry_chunk().m_no_of_triangle_groups == 1 && "Terrain Node has Multiple Meshes");
	ror::TriangleGroup *TG            = (*m_geometry_data->get_triangles_groups())[0];
	ror::GeometryData * geometic_data = m_geometry_data->get_geometry_data();

	for (uint32_t w = 0; w < rWIDTH; w++)
		for (uint32_t b = 0; b < rBREADTH; b++)
			m_has_grass[w][b].x = -258.0f;

	for (uint32_t tr = 0; tr < TG->get_triangles_size(); tr++)
	{
		ror::Triangle current_triangle = TG->get_triangles()[tr];
		float32_t     x                = geometic_data->get_vertices_array()[(current_triangle.get_vertices_index()[0] * 3)];
		float32_t     y                = geometic_data->get_vertices_array()[(current_triangle.get_vertices_index()[0] * 3) + 1];
		float32_t     z                = geometic_data->get_vertices_array()[(current_triangle.get_vertices_index()[0] * 3) + 2];

		float32_t s = geometic_data->get_texture_coords_array()[(current_triangle.get_textures_index()[0] * 2)];
		float32_t t = geometic_data->get_texture_coords_array()[(current_triangle.get_textures_index()[0] * 2) + 1];

		uint32_t s_in_grass = (uint32_t)(s * (float32_t) rWIDTH);
		uint32_t t_in_grass = (uint32_t)(t * (float32_t) rBREADTH);

		assert(s_in_grass <= rWIDTH && "Texture Coordinate sInGrass Out of Bound");
		assert(t_in_grass <= rBREADTH && "Texture Coordinate tInGrass Out of Bound");

		if (0 == (*grass_map_data)[(s_in_grass + (t_in_grass * rBREADTH)) * 4])
		{
			if (s_in_grass < rWIDTH && t_in_grass < rBREADTH)
			{
				m_has_grass[s_in_grass][t_in_grass].x = x;
				m_has_grass[s_in_grass][t_in_grass].y = y;
				m_has_grass[s_in_grass][t_in_grass].z = z;
			}
		}

		/*
			rUsChar GValue[1];
			//GValue[0] = (*GrassMapData)[(b + (w * rBREADTH)) * 4];			// Check for RED component
			//fwrite(GValue,1,1,p);
			if (0 == GValue[0])
			{
			//m_HasGrass[rWIDTH - w][b] = abs((float32_t)(*HeightMapData)[((w * rBREADTH) + b) * 4] / 255 * Scale);
			}
			*/
	}

	/*
		GrassMap =  rNew CoreEngine::CTextureImage();
		GrassMap->LoadImageFile(HeightMapFileName);
		rUsChar **HeightMapData = GrassMap->GetPixelData();

		float32_t Scale = 20.0f;
		//		FILE *p;
		//		fopen_s(&p,"d:/temp.raw","wb");
		for( uint32_t w = rWIDTH; w > 0; w--)
		{
		for(uint32_t b = 0; b < rBREADTH; b++)
		{
		m_HasGrass[rWIDTH - w][b] = 0.0f;
		rUsChar GValue[1];
		GValue[0] = (*GrassMapData)[(b + (w * rBREADTH)) * 4];			// Check for RED component
		//fwrite(GValue,1,1,p);
		if (0 == GValue[0])
		{
		m_HasGrass[rWIDTH - w][b] = abs((float32_t)(*HeightMapData)[((w * rBREADTH) + b) * 4] / 255 * Scale);
		}
		}
		}*/
}

FORCE_INLINE void TerrainNode::draw_billboards()
{
	uint32_t  x = 50, y = 50;
	float32_t interpolation = 5.0f;
	glPushMatrix();
	for (uint32_t w = 0; w < rWIDTH; w++)
	{
		for (uint32_t b = 0; b < rBREADTH; b++)
		{
			if (-258.0f != m_has_grass[w][b].x)
			{
				glPushMatrix();
				//glTranslatef((100 * w / rWIDTH),m_HasGrass[w][b],(100 * b / rBREADTH));
				glTranslatef(m_has_grass[w][b].x, m_has_grass[w][b].y + 0.5f, m_has_grass[w][b].z);
				m_billboard->set_orientation();
				m_billboard->global_draw();
				//DrawBilly();
				glPopMatrix();
			}
		}
	}
	glPopMatrix();
}

FORCE_INLINE void TerrainNode::draw_billy()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, 0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, 0.1f);

	glVertex3f(-0.1f, 0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, -0.1f);

	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, 0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, 0.1f);

	glVertex3f(0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, -0.1f);

	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, 0.1f, 0.1f);

	glVertex3f(0.1f, 0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, -0.1f);
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, -0.1f);
	glEnd();
}
}        // namespace ror
