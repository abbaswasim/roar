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

#include "foundation/rorsingleton.h"
#include "interfaces/rorishader_manager.h"
#include "rorshader_object.h"

namespace ror
{
class ROAR_ENGINE_ITEM OGLShaderManager : public Singleton<OGLShaderManager>, public ror::IShaderManager
{
  public:
	FORCE_INLINE bool update_shader(uint32_t a_id);
	FORCE_INLINE uint32_t get_shader_size(uint32_t a_id);
	FORCE_INLINE uint32_t get_all_shaders_size();

	OGLShader *create_shader(std::string a_file_name, EShaderType a_type);
	void       release_shader(OGLShader *a_shader_pointer);
	void       release_all_shaders();

  private:
	FORCE_INLINE OGLShaderManager(void);
	~OGLShaderManager(void);

	typedef std::unordered_map<std::string, ShaderObject *> shader_objects_map;

	shader_objects_map *m_shaders;        // Shaders Store

	// Make CSingleton<COGLShaderManager> as friend
	friend class Singleton<OGLShaderManager>;
};
}        // namespace ror

#include "rorogl_shader_manager.hh"
