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
FORCE_INLINE ShadowMap::ShadowMap(void)
{}

FORCE_INLINE uint32_t ShadowMap::get_shadow_map_id() const
{
	return this->m_shadow_map_id;
}

FORCE_INLINE void ShadowMap::set_shadow_map_id(uint32_t a_shadow_map_id)
{
	this->m_shadow_map_id = a_shadow_map_id;
}

FORCE_INLINE uint32_t ShadowMap::get_shadow_map_size() const
{
	return this->m_shadow_map_size;
}

FORCE_INLINE void ShadowMap::set_shadow_map_size(uint32_t a_shadow_map_size)
{
	this->m_shadow_map_size = a_shadow_map_size;
}

FORCE_INLINE void ShadowMap::start()
{
	// TODO : this should be configurable
	this->m_shadow_map_size = 1024;

	ror::Vector3f from = ror::Vector3f(-50, 50, 50);
	ror::Vector3f to   = ror::Vector3f(0.0f, 0.0f, 0.0f);
	ror::Vector3f up   = ror::Vector3f(0.0f, 1.0f, 0.0f);

	this->m_light_view_matrix.look_at(from, to, up);
	this->m_light_projection_matrix.set_perspective(35.0f, 1.0f, 1.0f, 100.0f);
	int32_t size = 50;

	//		glMatrixMode(GL_MODELVIEW);
	//		glPushMatrix();
	//		glLoadIdentity();
	//		glOrtho(-Size,Size,-Size,Size,-Size,Size);
	//		glGetFloatv(GL_MODELVIEW_MATRIX,this->m_LightProjectionMatrix.m_Values);
	//		glPopMatrix();

	//		glMatrixMode(GL_MODELVIEW);
	//		glPushMatrix();
	//		glLoadMatrixf(this->m_LightProjectionMatrix);
	//		glTranslatef(0.0f,0.0f,-10.0f);
	//		glGetFloatv(GL_MODELVIEW_MATRIX,this->m_LightProjectionMatrix.m_Values);
	//		glPopMatrix();

	glGenTextures(1, &this->m_shadow_map_id);
	glBindTexture(GL_TEXTURE_2D, this->m_shadow_map_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->m_shadow_map_size, this->m_shadow_map_size, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}

FORCE_INLINE void ShadowMap::setup_pass1()
{
	//Use view port the same size as the shadow map
	glGetIntegerv(GL_VIEWPORT, this->m_view_port);
	glViewport(0, 0, this->m_shadow_map_size, this->m_shadow_map_size);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(this->m_light_projection_matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(this->m_light_view_matrix);

	//Draw back faces into the shadow map
	glCullFace(GL_FRONT);

	//Disable color writes, and use flat shading for speed
	glShadeModel(GL_FLAT);
	glColorMask(0, 0, 0, 0);
	glDisable(GL_LIGHTING);
	glEnable(GL_POLYGON_OFFSET_FILL);
}

FORCE_INLINE void ShadowMap::setup_pass2()
{
	//Read the depth buffer into the shadow map texture
	glActiveTexture(GL_TEXTURE1);
	//glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, this->m_shadow_map_id);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, this->m_shadow_map_size, this->m_shadow_map_size);

	//Restore states
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);

	glViewport(this->m_view_port[0], this->m_view_port[1], this->m_view_port[2], this->m_view_port[3]);

	//Use dim light to represent shadowed areas
	glEnable(GL_LIGHT0);
	//glLightfv(GL_LIGHT0, GL_POSITION, CoreEngine::CVector4f(-15.0f,15.0f,15.0f,1.0f));//-10,10,10
	glLightfv(GL_LIGHT0, GL_AMBIENT, g_black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_black);
	glLightfv(GL_LIGHT0, GL_SPECULAR, g_black);
	glEnable(GL_LIGHTING);
	glDisable(GL_POLYGON_OFFSET_FILL);
}

FORCE_INLINE void ShadowMap::setup_pass3(Matrix4f &a_projection)
{
	//Draw with bright light
	glLightfv(GL_LIGHT0, GL_AMBIENT, g_black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, g_white);

	//Calculate texture matrix for projection
	//This matrix takes us from eye space to the light's clip space
	static Matrix4f bias_matrix(0.5f, 0.0f, 0.0f, 0.0f,
	                            0.0f, 0.5f, 0.0f, 0.0f,
	                            0.0f, 0.0f, 0.5f, 0.0f,
	                            0.5f, 0.5f, 0.5f, 1.0f);        //bias from [-1, 1] to [0, 1]

	Matrix4f texture_matrix = this->m_light_view_matrix * /*a_Projection*/ this->m_light_projection_matrix * bias_matrix;

	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL);
	//		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE,GL_LUMINANCE);
	//		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
	//		glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, texture_matrix.get_row(0));
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, texture_matrix.get_row(1));
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, texture_matrix.get_row(2));
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, texture_matrix.get_row(3));
	glEnable(GL_TEXTURE_GEN_Q);

	//Bind & enable shadow map texture
	glBindTexture(GL_TEXTURE_2D, this->m_shadow_map_id);
	//glEnable(GL_TEXTURE_2D);

	//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);

	//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

	//Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	//Set alpha test to discard false comparisons
	//glAlphaFunc(GL_GEQUAL, 0.99f);
	//glEnable(GL_ALPHA_TEST);
	//glActiveTexture(GL_TEXTURE0);
}

FORCE_INLINE void ShadowMap::end() const
{
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	//glDisable(GL_ALPHA_TEST);
}

}        // namespace ror
