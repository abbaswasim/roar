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
FORCE_INLINE OGLShader::OGLShader(void)
{
	//m_ShaderID = 0;
}

FORCE_INLINE OGLShader::OGLShader(const std::string a_file_name, EShaderType a_type)
{
	m_shader_id = glCreateShader(a_type);

	const char8_t *source = read(a_file_name);
	glShaderSource(m_shader_id, 1, static_cast<const char8_t **>(&source), nullptr);

	if (!this->compile())
		MESSAGEBOX("Shader Compilation Failed..");

	delete[] source;
}

FORCE_INLINE bool OGLShader::compile()
{
	int32_t compiled;

	glCompileShader(m_shader_id);
	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &compiled);

	return (compiled ? true : false);
}

FORCE_INLINE void OGLShader::get_log(std::string &a_shader_log) const
{
	char8_t *log;
	int32_t  length;

	glGetShaderiv(m_shader_id, GL_INFO_LOG_LENGTH, &length);

	log = new char8_t[length];
	glGetShaderInfoLog(m_shader_id, length, &length, log);

	a_shader_log = std::string(log);
	delete[] log;
}

FORCE_INLINE void OGLShader::get_souce(std::string &a_shader_souce) const
{
	char8_t *source;
	int32_t  length;

	glGetShaderiv(m_shader_id, GL_SHADER_SOURCE_LENGTH, &length);

	source = new char8_t[length];

	glGetShaderSource(m_shader_id, length, &length, source);

	a_shader_souce.append(source, 0, length);

	delete[] source;
}

FORCE_INLINE void OGLShader::set_souce(std::string &a_shader_souce)
{
	const char8_t *source = a_shader_souce.c_str();
	glShaderSource(m_shader_id, 1, static_cast<const char8_t **>(&source), nullptr);

	if (!this->compile())
		MESSAGEBOX("Shader Compilation Failed..");
}

FORCE_INLINE void OGLShader::get_parameter(rGLEnum a_parameter, int32_t *a_data) const
{
	glGetShaderiv(m_shader_id, a_parameter, a_data);
}

}        // namespace ror
