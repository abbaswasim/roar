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

#include "interfaces/rorishader.h"

namespace ror
{
class ROAR_ENGINE_ITEM OGLShaderProgram;
class ROAR_ENGINE_ITEM OGLShader : public ror::IShader
{
  public:
	FORCE_INLINE OGLShader(void);
	FORCE_INLINE OGLShader(const std::string a_file_name, EShaderType a_type);
	virtual ~OGLShader(void);

	FORCE_INLINE bool compile();
	FORCE_INLINE void get_log(std::string &a_shader_log) const;
	FORCE_INLINE void get_souce(std::string &a_shader_souce) const;
	FORCE_INLINE void set_souce(std::string &a_shader_souce);
	FORCE_INLINE void get_parameter(rGLEnum a_parameter, int32_t *a_data) const;

  protected:
	friend class ROAR_ENGINE_ITEM OGLShaderProgram;
};
}        // namespace ror

#include "rorogl_shader.hh"
