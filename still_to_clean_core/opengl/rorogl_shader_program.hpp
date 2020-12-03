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

#include "rorogl_shader_manager.h"

namespace ror
{
class ROAR_ENGINE_ITEM OGLShaderProgram
{
  public:
	FORCE_INLINE OGLShaderProgram(void);
	FORCE_INLINE OGLShaderProgram(const std::string &a_shader_file_name, EShaderType a_type);
	FORCE_INLINE OGLShaderProgram(const std::string &a_vertex_shader_file_name, const std::string &a_fragment_shader_file_name);
	//FORCE_INLINE COGLShaderProgram(CoreEngine::COGLShader *a_VertexShader, CoreEngine::COGLShader *a_FragmentShader);
	virtual ~OGLShaderProgram(void);

	FORCE_INLINE void attach_shader(ror::OGLShader *a_shader);
	FORCE_INLINE void attach_shader(uint32_t a_shader_id);
	FORCE_INLINE void detach_shader(ror::OGLShader *a_shader);

	FORCE_INLINE void        compile();
	FORCE_INLINE void        enable() const;
	FORCE_INLINE static void disable();

	FORCE_INLINE void send_uniform(const std::string &a_uniform_name, float32_t x);
	FORCE_INLINE void send_uniform(const std::string &a_uniform_name, float32_t x, float32_t y);
	FORCE_INLINE void send_uniform(const std::string &a_uniform_name, float32_t x, float32_t y, float32_t z);
	FORCE_INLINE void send_uniform(const std::string &a_uniform_name, float32_t x, float32_t y, float32_t z, float32_t w);
	FORCE_INLINE void send_uniform(const std::string &a_uniform_name, int32_t x);
	FORCE_INLINE void send_uniform(const std::string &a_uniform_name, uint32_t x);
	FORCE_INLINE void send_uniform(const std::string &a_uniform_name, int32_t x, int32_t y);
	FORCE_INLINE void send_uniform(const std::string &a_uniform_name, int32_t x, int32_t y, int32_t z);
	FORCE_INLINE void send_uniform(const std::string &a_uniform_name, int32_t x, int32_t y, int32_t z, int32_t w);
	FORCE_INLINE void send_uniform(const std::string &a_uniform_name, float32_t *m, bool transp = false, int32_t size = 4);

	FORCE_INLINE void send_uniform(uint32_t a_uniform_location, float32_t x);
	FORCE_INLINE void send_uniform(uint32_t a_uniform_location, float32_t x, float32_t y);
	FORCE_INLINE void send_uniform(uint32_t a_uniform_location, float32_t x, float32_t y, float32_t z);
	FORCE_INLINE void send_uniform(uint32_t a_uniform_location, float32_t x, float32_t y, float32_t z, float32_t w);
	FORCE_INLINE void send_uniform(uint32_t a_uniform_location, int32_t x);
	FORCE_INLINE void send_uniform(uint32_t a_uniform_location, int32_t x, int32_t y);
	FORCE_INLINE void send_uniform(uint32_t a_uniform_location, int32_t x, int32_t y, int32_t z);
	FORCE_INLINE void send_uniform(uint32_t a_uniform_location, int32_t x, int32_t y, int32_t z, int32_t w);
	FORCE_INLINE void send_uniform(uint32_t a_uniform_location, float32_t *m, bool transp = false, int32_t size = 4);

	FORCE_INLINE uint32_t get_uniform_location(const std::string &a_uniform_name) const;
	FORCE_INLINE void     set_attribute_location(const std::string &a_attribute_name, uint32_t a_location);
	FORCE_INLINE uint32_t get_attribute_location(const std::string &a_attribute_name) const;

	FORCE_INLINE void get_program_log(std::string &a_log) const;
	FORCE_INLINE uint32_t get_id() const;
	FORCE_INLINE void     get_parameter(rGLEnum a_parameter, int32_t *a_data) const;
	FORCE_INLINE void     validate() const;
	FORCE_INLINE bool     is_valid_program() const;
	FORCE_INLINE void     get_attribute_info(uint32_t a_location, std::string &a_name, rGLEnum &type, int32_t &size) const;
	FORCE_INLINE void     get_uniform_info(uint32_t a_location, std::string &a_name, rGLEnum &datatype, int32_t &size) const;
	FORCE_INLINE void     get_attached_shaders(std::vector<uint32_t> &shaderhandles);
	FORCE_INLINE void     get_attached_shaders(std::vector<ror::OGLShader> &shaders);

	FORCE_INLINE OGLShader *get_vertex_shader() const;
	FORCE_INLINE void       set_vertex_shader(ror::OGLShader *a_vertex_shader);
	FORCE_INLINE OGLShader *get_fragment_shader() const;
	FORCE_INLINE void       set_fragment_shader(ror::OGLShader *a_fragment_shader);

	FORCE_INLINE static std::string parse_errors();

	//		static uint32_t		m_Enables;
	//		static uint32_t		m_Disables;

  private:
	FORCE_INLINE int32_t get_location(const std::string &name);

	typedef std::map<std::string, int32_t> program_uniforms_map;

	ror::OGLShader *     m_vertex_shader;          // Program Don't take ownership of m_VertexShader (don't need to delete)
	ror::OGLShader *     m_fragment_shader;        // Program Don't take ownership of m_FragmentShader (don't need to delete)
	uint32_t             m_program_id;
	program_uniforms_map m_uniforms;
};
}        // namespace ror

#include "rorogl_shader_program.hh"
