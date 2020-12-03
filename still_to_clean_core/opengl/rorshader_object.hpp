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

#include "memory_manager/rormemory_check.h"
#include "rorogl_shader.h"

namespace ror
{
class ROAR_ENGINE_ITEM ShaderObject
{
  public:
	FORCE_INLINE ShaderObject(void);
	FORCE_INLINE ShaderObject(uint32_t a_id, uint32_t a_shader_size, uint32_t a_reference_count, OGLShader *a_shader_pointer);
	FORCE_INLINE ShaderObject(ShaderObject &a_other);
	~ShaderObject(void);

	FORCE_INLINE uint32_t get_shader_id() const;
	FORCE_INLINE void     set_shader_id(uint32_t a_shader_id);

	FORCE_INLINE uint32_t get_shader_size() const;
	FORCE_INLINE void     set_shader_size(uint32_t a_shader_id);

	FORCE_INLINE uint32_t get_reference_count() const;
	FORCE_INLINE void     set_reference_count(uint32_t &a_shader_id);
	FORCE_INLINE uint32_t increment_reference_count();
	FORCE_INLINE uint32_t decrement_reference_count();

	FORCE_INLINE OGLShader *get_shader_pointer() const;
	FORCE_INLINE void       set_shader_pointer(OGLShader *a_shader_pointer);

  private:
	uint32_t        m_shader_id;              // Shader ID used by The Renderer
	uint32_t        m_shader_size;            // Shader Data size in Bytes
	uint32_t        m_reference_count;        // How many times this Shader is Used
	ror::OGLShader *m_shader_pointer;         // Pointer to a Shader
};
}        // namespace ror

#include "rorshader_object.hh"
