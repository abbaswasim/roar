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
FORCE_INLINE Light::Light(void)
{
	this->m_attenuation = Vector4f(1.0f, 0.0f, 0.0f, 0.0f);
	this->m_position    = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
	this->m_color       = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	this->m_light_type  = rDIRECTIONAL;
	initialize();
}

FORCE_INLINE void Light::initialize()
{
	this->m_id      = 0;
	this->m_enabled = true;

	if (this->m_no_of_lights < 8)
	{
		switch (this->m_no_of_lights)
		{
			case 0:
				this->m_id = GL_LIGHT0;
				break;
			case 1:
				this->m_id = GL_LIGHT1;
				break;
			case 2:
				this->m_id = GL_LIGHT2;
				break;
			case 3:
				this->m_id = GL_LIGHT3;
				break;
			case 4:
				this->m_id = GL_LIGHT4;
				break;
			case 5:
				this->m_id = GL_LIGHT5;
				break;
			case 6:
				this->m_id = GL_LIGHT6;
				break;
			case 7:
				this->m_id = GL_LIGHT7;
				break;
		}

		if (this->m_id)
			this->m_no_of_lights++;
	}
}

FORCE_INLINE void Light::bind()
{
	if (!this->m_enabled && this->m_id)
		return;

	glEnable(this->m_id);
	this->m_enabled = true;

	//		glLightf(this->m_ID, GL_CONSTANT_ATTENUATION , this->m_Attenuation.x);
	//		glLightf(this->m_ID, GL_LINEAR_ATTENUATION   , this->m_Attenuation.y);
	//		glLightf(this->m_ID, GL_QUADRATIC_ATTENUATION, this->m_Attenuation.z);

	//		this->m_Position.m_Values;
	//
	//		this->m_Ambient.m_Values[0] = 0.0f;
	//		this->m_Ambient.m_Values[1] = 0.0f;
	//		this->m_Ambient.m_Values[2] = 0.0f;
	//		this->m_Ambient.m_Values[3] = 0.0f;
	//
	//		this->m_Diffuse.m_Values[0] = 0.0f;
	//		this->m_Diffuse.m_Values[1] = 0.0f;
	//		this->m_Diffuse.m_Values[2] = 0.0f;
	//		this->m_Diffuse.m_Values[3] = 0.0f;
	//
	//		this->m_Specular.m_Values[0] = 0.0f;
	//		this->m_Specular.m_Values[1] = 0.0f;
	//		this->m_Specular.m_Values[2] = 0.0f;
	//		this->m_Specular.m_Values[3] = 0.0f;

	glLightfv(this->m_id, GL_POSITION, this->m_position.m_values);
	glLightfv(this->m_id, GL_AMBIENT, this->m_ambient.m_values);
	glLightfv(this->m_id, GL_DIFFUSE, this->m_diffuse.m_values);
	glLightfv(this->m_id, GL_SPECULAR, this->m_specular.m_values);
}

FORCE_INLINE void Light::disable()
{
	if (!this->m_enabled && this->m_id)
	{
		return;
	}
	glDisable(this->m_id);
	this->m_enabled = false;
}

FORCE_INLINE const Vector4f *Light::get_attenuation() const
{
	return &this->m_attenuation;
}

FORCE_INLINE const Vector4f *Light::get_light_position() const
{
	return &this->m_position;
}

FORCE_INLINE const Vector4f *Light::get_color() const
{
	return &this->m_color;
}

FORCE_INLINE int32_t Light::get_light_id() const
{
	return this->m_id;
}

FORCE_INLINE void Light::set_attenuation(const Vector4f &a_attenuation)
{
	this->m_attenuation = a_attenuation;
}

FORCE_INLINE void Light::set_enabled(bool a_enabled)
{
	this->m_enabled = a_enabled;
}

FORCE_INLINE void Light::set_light_position()
{
	glLightfv(this->m_id, GL_POSITION, this->m_position.m_values);
}

FORCE_INLINE void Light::set_light_position(const Vector4f &a_position)
{
	this->m_position = a_position;

	if (0.0f != this->m_position.w)
	{
		this->m_light_type = rPOSITIONAL;
	}

	glLightfv(this->m_id, GL_POSITION, this->m_position.m_values);
}

FORCE_INLINE void Light::set_light_id(int32_t a_id)
{
	this->m_id = a_id;
}

FORCE_INLINE bool Light::is_enabled()
{
	return m_enabled;
}

FORCE_INLINE void Light::set_light_type(Light::ELightType a_light_type)
{
	this->m_light_type = a_light_type;
}

FORCE_INLINE Light::ELightType Light::get_light_type() const
{
	return this->m_light_type;
}

}        // namespace ror
