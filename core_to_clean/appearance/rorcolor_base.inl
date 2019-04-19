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
FORCE_INLINE ColorBase::ColorBase(void)
{
	this->m_ambient  = Vector4f(0.2f, 0.2f, 0.2f, 1.0f);
	this->m_diffuse  = Vector4f(0.8f, 0.8f, 0.8f, 1.0f);
	this->m_specular = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);

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

FORCE_INLINE const Vector4f *ColorBase::get_ambient() const
{
	return &this->m_ambient;
}

FORCE_INLINE void ColorBase::set_ambient(Vector4f &a_ambient)
{
	this->m_ambient = a_ambient;
}

FORCE_INLINE const Vector4f *ColorBase::get_diffuse() const
{
	return &this->m_diffuse;
}

FORCE_INLINE void ColorBase::set_diffuse(Vector4f &a_diffuse)
{
	this->m_diffuse = a_diffuse;
}

FORCE_INLINE const Vector4f *ColorBase::get_specular() const
{
	return &this->m_specular;
}

FORCE_INLINE void ColorBase::set_specular(Vector4f &a_specular)
{
	this->m_specular = a_specular;
}

}        // namespace ror
