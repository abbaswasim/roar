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
FORCE_INLINE BillboardNode::BillboardNode(void)
{
	this->m_texture_object = nullptr;
}

FORCE_INLINE void BillboardNode::global_draw()
{
	//glBindTexture(GL_TEXTURE_2D,m_TextureID);
	this->m_texture_object->bind_texture();
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);

	glTexCoord2f(1.0f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	glTexCoord2f(0.0f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.0f);

	glEnd();
	//glBindTexture(GL_TEXTURE_2D,0);
	this->m_texture_object->bind_texture(false);
	glPopMatrix();
}

FORCE_INLINE void BillboardNode::set_orientation()
{
	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, m_model_view_matrix.m_values);

	for (uint32_t i = 0; i < 3; i += 2)
	{
		for (uint32_t j = 0; j < 3; j++)
		{
			if (i == j)
				m_model_view_matrix.m_values[i * 4 + j] = 1.0;
			else
				m_model_view_matrix.m_values[i * 4 + j] = 0.0;
		}
	}
	glLoadMatrixf(m_model_view_matrix.m_values);
}

FORCE_INLINE void BillboardNode::load_geometry(std::string a_file_name)
{
	static ror::OGLTextureManager *texture_manager = ror::OGLTextureManager::get_instance();
	a_file_name                                    = WORKINGDIRECTORY + a_file_name;
	this->m_texture_object                         = texture_manager->create_texture(a_file_name.c_str());
}

FORCE_INLINE Matrix4f *BillboardNode::get_model_view_matrix() const
{
	return &m_model_view_matrix;
}

FORCE_INLINE void BillboardNode::set_model_view_matrix(Matrix4f *a_modelview_matrix)
{
	m_model_view_matrix = *a_modelview_matrix;
	rDELETE(a_modelview_matrix);
	/*
		for(uint32_t i=0; i<3; i++ )
		{
		for(uint32_t j=0; j<3; j++ )
		{
		if ( i==j )
		m_ModelViewMatrix->m_Entry[i*4+j] = 1.0;
		else
		m_ModelViewMatrix->m_Entry[i*4+j] = 0.0;
		}
		}*/

	// Cheap billboarding just using the Camera Matrix
	for (uint32_t i = 0; i < 3; i += 2)
	{
		for (uint32_t j = 0; j < 3; j++)
		{
			if (i == j)
				m_model_view_matrix.m_values[i * 4 + j] = 1.0;
			else
				m_model_view_matrix.m_values[i * 4 + j] = 0.0;
		}
	}
}

}        // namespace ror
