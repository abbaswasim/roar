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
FORCE_INLINE Material::Material(void)
{
	//{ 0.0f, 0.0f, 0.0f, 1.0f };
	this->m_emmision = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);

	this->m_shine        = 0.0f;
	this->m_transparency = 1.0f;

	//		// OpenGL Defaults
	//		m_Ambient[0] = 0.2f;
	//		m_Ambient[1] = 0.2f;
	//		m_Ambient[2] = 0.2f;
	//		m_Ambient[3] = 1.0f;
	//
	//		//{ 1.0f, 1.0f, 1.0f, 1.0f };
	//		m_Diffuse[0] = 0.8f;
	//		m_Diffuse[1] = 0.8f;
	//		m_Diffuse[2] = 0.8f;
	//		m_Diffuse[3] = 1.0f;
	//
	//		//{ 1.0f, 1.0f, 1.0f, 1.0f };
	//		m_Specular[0] = 0.0f;
	//		m_Specular[1] = 0.0f;
	//		m_Specular[2] = 0.0f;
	//		m_Specular[3] = 1.0f;
	//
	//		//{ 0.0f, 0.0f, 0.0f, 1.0f };
	//		m_Emmision[0] = 0.0f;
	//		m_Emmision[1] = 0.0f;
	//		m_Emmision[2] = 0.0f;
	//		m_Emmision[3] = 1.0f;
	//
	//		m_Shine = 0.0f;
	//		m_Transparency = 1.0f;
}

FORCE_INLINE void Material::bind_material()
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, this->m_specular.m_values);
	glMaterialfv(GL_FRONT, GL_AMBIENT, this->m_ambient.m_values);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, this->m_diffuse.m_values);
	glMaterialfv(GL_FRONT, GL_EMISSION, this->m_emmision.m_values);
	glMaterialf(GL_FRONT, GL_SHININESS, this->m_shine);
}

FORCE_INLINE float32_t Material::get_shine() const
{
	return this->m_shine;
}

FORCE_INLINE void Material::set_shine(float32_t a_shine)
{
	this->m_shine = a_shine;
}

FORCE_INLINE const Vector4f *Material::get_emmision() const
{
	return &this->m_emmision;
}

FORCE_INLINE void Material::set_emmision(Vector4f a_emmision)
{
	this->m_emmision = a_emmision;
}

FORCE_INLINE float32_t Material::get_transparency() const
{
	return this->m_transparency;
}

FORCE_INLINE void Material::set_transparency(float32_t a_transparence)
{
	this->m_transparency = a_transparence;
}

}        // namespace ror
