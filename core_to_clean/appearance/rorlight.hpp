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

#include "rorcolor_base.h"

namespace ror
{
class ROAR_ENGINE_ITEM Triangle;
class ROAR_ENGINE_ITEM TriangleGroup;
class ROAR_ENGINE_ITEM CCBA;
// If the Light Position's Fourth Values CVector4f(0,0,0,thisOne) is "Zero" then it means a directional light
// and The position means the direction vector
// If the Light Position's Fourth Values CVector4f(0,0,0,thisOne) is NOT "Zero" then it means a Positional light
// And the Position is the Position of the Light
class ROAR_ENGINE_ITEM Light : public ror::ColorBase
{
  public:
	enum ELightType
	{
		rPOSITIONAL,
		rDIRECTIONAL
	};

	FORCE_INLINE Light(void);
	~Light(void);

	FORCE_INLINE void initialize();
	FORCE_INLINE void bind();
	FORCE_INLINE void disable();

	FORCE_INLINE const Vector4f *get_attenuation() const;
	FORCE_INLINE const Vector4f *get_light_position() const;
	FORCE_INLINE const Vector4f *get_color() const;
	FORCE_INLINE int32_t get_light_id() const;
	FORCE_INLINE void    set_attenuation(const Vector4f &a_attenuation);
	FORCE_INLINE void    set_enabled(bool a_enabled);
	FORCE_INLINE void    set_light_position();
	FORCE_INLINE void    set_light_position(const Vector4f &a_position);
	FORCE_INLINE void    set_light_id(int32_t a_id);
	FORCE_INLINE bool    is_enabled();
	FORCE_INLINE Light::ELightType get_light_type() const;
	FORCE_INLINE void              set_light_type(Light::ELightType a_light_type);

  private:
	friend class ROAR_ENGINE_ITEM Triangle;
	friend class ROAR_ENGINE_ITEM TriangleGroup;
	friend class ROAR_ENGINE_ITEM CCBA;

	int32_t    m_id;
	ELightType m_light_type;
	Vector4f   m_attenuation;
	Vector4f   m_position;
	Vector4f   m_color;
	bool       m_enabled;

	static int32_t m_no_of_lights;
};
}        // namespace ror

#include "rorlight.hh"
