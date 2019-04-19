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
FORCE_INLINE ShaderObject::ShaderObject(void) :
    m_shader_id(0),
    m_reference_count(1),
    m_shader_size(0),
    m_shader_pointer(nullptr)
{}

FORCE_INLINE ShaderObject::ShaderObject(uint32_t a_id, uint32_t a_shader_size, uint32_t a_reference_count, OGLShader *a_shader_pointer) :
    m_shader_id(a_id),
    m_reference_count(a_reference_count),
    m_shader_size(a_shader_size),
    m_shader_pointer(a_shader_pointer)
{}

FORCE_INLINE ShaderObject::ShaderObject(ShaderObject &a_other) :
    m_shader_id(a_other.m_shader_id),
    m_reference_count(a_other.m_reference_count),
    m_shader_size(a_other.m_shader_size),
    m_shader_pointer(a_other.m_shader_pointer)
{}

FORCE_INLINE uint32_t ShaderObject::get_shader_id() const
{
	return m_shader_id;
}

FORCE_INLINE void ShaderObject::set_shader_id(uint32_t a_shader_id)
{
	m_shader_id = a_shader_id;
}

FORCE_INLINE uint32_t ShaderObject::get_shader_size() const
{
	return m_shader_size;
}

FORCE_INLINE void ShaderObject::set_shader_size(uint32_t a_shader_id)
{
	m_shader_size = a_shader_id;
}

FORCE_INLINE uint32_t ShaderObject::increment_reference_count()
{
	return ++m_reference_count;
}

FORCE_INLINE uint32_t ShaderObject::decrement_reference_count()
{
	return --m_reference_count;
}

FORCE_INLINE uint32_t ShaderObject::get_reference_count() const
{
	return m_reference_count;
}

FORCE_INLINE void ShaderObject::set_reference_count(uint32_t &a_reference_count)
{
	m_reference_count = a_reference_count;
}

FORCE_INLINE OGLShader *ShaderObject::get_shader_pointer() const
{
	return m_shader_pointer;
}

FORCE_INLINE void ShaderObject::set_shader_pointer(OGLShader *a_shader_pointer)
{
	m_shader_pointer = a_shader_pointer;
}

}        // namespace ror
