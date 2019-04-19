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
FORCE_INLINE OGLShaderProgram::OGLShaderProgram() :
    m_vertex_shader(nullptr),
    m_fragment_shader(nullptr)
{
	m_program_id = glCreateProgram();
}

FORCE_INLINE OGLShaderProgram::OGLShaderProgram(const std::string &a_shader_file_name, EShaderType a_type) :
    m_vertex_shader(nullptr),
    m_fragment_shader(nullptr)
{
	m_program_id    = glCreateProgram();
	m_vertex_shader = ror::OGLShaderManager::get_instance()->create_shader(a_shader_file_name, a_type);

	attach_shader(m_vertex_shader);

	if (rGL_VERTEX_SHADER != a_type)
	{
		m_fragment_shader = m_vertex_shader;
		m_vertex_shader   = nullptr;
	}

	compile();
}

/*
	FORCE_INLINE COGLShaderProgram::COGLShaderProgram( CoreEngine::COGLShader *a_VertexShader, CoreEngine::COGLShader *a_FragmentShader )
		: m_VertexShader(nullptr), m_FragmentShader(nullptr)
	{
		m_ProgramID = glCreateProgram();

		m_VertexShader		= a_VertexShader;
		m_FragmentShader	= a_FragmentShader;

		if (nullptr != m_VertexShader)
			AttachShader(m_VertexShader);

		if (nullptr != m_FragmentShader)
			AttachShader(m_FragmentShader);

		if (nullptr != m_FragmentShader || nullptr != m_VertexShader)
			Compile();
	}
*/

FORCE_INLINE OGLShaderProgram::OGLShaderProgram(const std::string &a_vertex_shader_file_name, const std::string &a_fragment_shader_file_name) :
    m_vertex_shader(nullptr),
    m_fragment_shader(nullptr)
{
	m_program_id = glCreateProgram();

	m_vertex_shader   = ror::OGLShaderManager::get_instance()->create_shader(a_vertex_shader_file_name, rGL_VERTEX_SHADER);
	m_fragment_shader = ror::OGLShaderManager::get_instance()->create_shader(a_fragment_shader_file_name, rGL_FRAGMENT_SHADER);
	//		m_VertexShader		= rNew CoreEngine::COGLShader(a_VertexShaderFileName, GL_VERTEX_SHADER);
	//		m_FragmentShader	= rNew CoreEngine::COGLShader(a_FragmentShaderFileName, GL_FRAGMENT_SHADER);

	attach_shader(m_vertex_shader);
	attach_shader(m_fragment_shader);

	compile();
}

FORCE_INLINE uint32_t OGLShaderProgram::get_id() const
{
	return m_program_id;
}

FORCE_INLINE void OGLShaderProgram::get_program_log(std::string &log) const
{
	GLchar *debug;
	int32_t debug_length;
	//GetParameter(GL_INFO_LOG_LENGTH, &debugLength);
	glGetProgramiv(this->m_program_id, GL_INFO_LOG_LENGTH, &debug_length);

	debug = new char8_t[debug_length];
	glGetProgramInfoLog(m_program_id, debug_length, &debug_length, debug);

	log.append(debug, 0, debug_length);
	delete[] debug;
}

FORCE_INLINE void OGLShaderProgram::validate() const
{
	glValidateProgram(m_program_id);
}

FORCE_INLINE bool OGLShaderProgram::is_valid_program() const
{
	int32_t status;
	get_parameter(GL_VALIDATE_STATUS, &status);
	if (!status)
		return false;
	else
		return true;
}

FORCE_INLINE void OGLShaderProgram::get_parameter(rGLEnum param, int32_t *data) const
{
	glGetProgramiv(m_program_id, param, data);
}

FORCE_INLINE void OGLShaderProgram::attach_shader(ror::OGLShader *a_shader)
{
	glAttachShader(m_program_id, a_shader->m_shader_id);
}

FORCE_INLINE void OGLShaderProgram::attach_shader(uint32_t a_shader_id)
{
	glAttachShader(m_program_id, a_shader_id);
}

FORCE_INLINE void OGLShaderProgram::detach_shader(ror::OGLShader *a_shader)
{
	glDetachShader(m_program_id, a_shader->m_shader_id);
}

FORCE_INLINE void OGLShaderProgram::compile()
{
	glLinkProgram(m_program_id);
	m_uniforms.clear();
}

FORCE_INLINE void OGLShaderProgram::enable() const
{
	//m_Enables++;
	glUseProgram(m_program_id);
}

FORCE_INLINE void OGLShaderProgram::disable()
{
	//m_Disables++;
	glUseProgram(0);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(const std::string &name, float32_t x)
{
	glUniform1f(get_location(name), x);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(const std::string &name, float32_t x, float32_t y)
{
	glUniform2f(get_location(name), x, y);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(const std::string &name, float32_t x, float32_t y, float32_t z)
{
	glUniform3f(get_location(name), x, y, z);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(const std::string &name, float32_t x, float32_t y, float32_t z, float32_t w)
{
	glUniform4f(get_location(name), x, y, z, w);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(const std::string &name, int32_t x)
{
	glUniform1i(get_location(name), x);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(const std::string &name, uint32_t x)
{
	glUniform1i(get_location(name), x);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(const std::string &name, int32_t x, int32_t y)
{
	glUniform2i(get_location(name), x, y);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(const std::string &name, int32_t x, int32_t y, int32_t z)
{
	glUniform3i(get_location(name), x, y, z);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(const std::string &name, int32_t x, int32_t y, int32_t z, int32_t w)
{
	glUniform4i(get_location(name), x, y, z, w);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(const std::string &name, float32_t *matrix, bool transpose, int32_t size)
{
	int32_t loc = get_location(name);

	switch (size)
	{
		case '2':
			glUniformMatrix2fv(loc, 0, transpose, matrix);
			break;
		case '3':
			glUniformMatrix3fv(loc, 0, transpose, matrix);
			break;
		case '4':
			glUniformMatrix4fv(loc, 0, transpose, matrix);
			break;
	}
}

FORCE_INLINE void OGLShaderProgram::send_uniform(uint32_t location, float32_t x)
{
	glUniform1f(location, x);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(uint32_t location, float32_t x, float32_t y)
{
	glUniform2f(location, x, y);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(uint32_t location, float32_t x, float32_t y, float32_t z)
{
	glUniform3f(location, x, y, z);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(uint32_t location, float32_t x, float32_t y, float32_t z, float32_t w)
{
	glUniform4f(location, x, y, z, w);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(uint32_t location, int32_t x)
{
	glUniform1i(location, x);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(uint32_t location, int32_t x, int32_t y)
{
	glUniform2i(location, x, y);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(uint32_t location, int32_t x, int32_t y, int32_t z)
{
	glUniform3i(location, x, y, z);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(uint32_t location, int32_t x, int32_t y, int32_t z, int32_t w)
{
	glUniform4i(location, x, y, z, w);
}

FORCE_INLINE void OGLShaderProgram::send_uniform(uint32_t location, float32_t *matrix, bool transpose, int32_t size)
{
	switch (size)
	{
		case '2':
			glUniformMatrix2fv(location, 0, transpose, matrix);
			break;
		case '3':
			glUniformMatrix3fv(location, 0, transpose, matrix);
			break;
		case '4':
			glUniformMatrix4fv(location, 0, transpose, matrix);
			break;
	}
}

FORCE_INLINE uint32_t OGLShaderProgram::get_uniform_location(const std::string &name) const
{
	return glGetUniformLocation(m_program_id, name.c_str());
}

FORCE_INLINE void OGLShaderProgram::set_attribute_location(const std::string &name, uint32_t location)
{
	glBindAttribLocation(m_program_id, location, name.c_str());
}

FORCE_INLINE uint32_t OGLShaderProgram::get_attribute_location(const std::string &name) const
{
	return glGetAttribLocation(m_program_id, name.c_str());
}

FORCE_INLINE void OGLShaderProgram::get_attribute_info(uint32_t location, std::string &name, rGLEnum &datatype, int32_t &size) const
{
	// first we need to query the length of the longest name
	GLsizei length;
	get_parameter(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, (int32_t *) &length);

	std::vector<char8_t> nametmp(length);
	GLsizei              copiedsize;
	glGetActiveAttrib(m_program_id, location, length, &copiedsize, &size, &datatype, &nametmp[0]);

	if (0 == copiedsize)
		return;

	name.assign(&nametmp[0]);
}

FORCE_INLINE void OGLShaderProgram::get_uniform_info(uint32_t location, std::string &name, rGLEnum &datatype, int32_t &size) const
{
	// first we need to query the length of the longest name
	int32_t length;
	get_parameter(GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);

	std::vector<char8_t> nametmp(length);
	GLsizei              copiedsize;
	glGetActiveUniform(m_program_id, location, length, &copiedsize, &size, &datatype, &nametmp[0]);

	if (0 == copiedsize)
		return;

	name.assign(&nametmp[0]);
}

FORCE_INLINE void OGLShaderProgram::get_attached_shaders(std::vector<uint32_t> &shaderhandles)
{
	int32_t number;
	get_parameter(GL_ATTACHED_SHADERS, &number);

	shaderhandles.resize(number);
	glGetAttachedShaders(m_program_id, number, nullptr, &shaderhandles[0]);
}
/*
	namespace {

	struct shaderconstuctor
	{
	std::vector<CoreEngine::COGLShader> &shaders_;
	shaderconstuctor(std::vector<CoreEngine::COGLShader> &shaders) : shaders_(shaders)
	{
	}
	void operator()(std::vector<uint32_t>::value_type data)
	{
	shaders_.push_back(CoreEngine::COGLShader(data));
	}
	};
	}
	*/

FORCE_INLINE void OGLShaderProgram::get_attached_shaders(std::vector<ror::OGLShader> &shaders)
{
	/*
		std::vector<uint32_t> shaderhandles;
		GetAttachedShaders(shaderhandles);

		std::for_each(shaderhandles.begin(),shaderhandles.end(),shaderconstuctor(shaders));
		*/
}

FORCE_INLINE int32_t OGLShaderProgram::get_location(const std::string &name)
{
	int32_t                              loc = -1;
	program_uniforms_map::const_iterator it  = m_uniforms.find(name);
	if (it == m_uniforms.end())
	{
		loc = glGetUniformLocation(m_program_id, name.c_str());
		if (loc == -1)
		{
			std::string s;
			s.append(name);
			s.append(" - is not a valid uniform variable name");
			throw 1;
		}
		m_uniforms.insert(program_uniforms_map::value_type(name, loc));
	}
	else
		loc = it->second;

	return loc;
}

FORCE_INLINE OGLShader *OGLShaderProgram::get_vertex_shader() const
{
	return m_vertex_shader;
}

FORCE_INLINE void OGLShaderProgram::set_vertex_shader(ror::OGLShader *a_vertex_shader)
{
	m_vertex_shader = a_vertex_shader;
}

FORCE_INLINE OGLShader *OGLShaderProgram::get_fragment_shader() const
{
	return m_fragment_shader;
}

FORCE_INLINE void OGLShaderProgram::set_fragment_shader(ror::OGLShader *a_fragment_shader)
{
	m_fragment_shader = a_fragment_shader;
}

FORCE_INLINE std::string OGLShaderProgram::parse_errors()
{
	rGLEnum     error = glGetError();
	std::string return_value;
	switch (error)
	{
		case GL_INVALID_ENUM:
			return_value = "INVALID ENUM";
			break;
		case GL_INVALID_VALUE:
			return_value = "INVALID VALUE";
			break;
		case GL_INVALID_OPERATION:
			return_value = "INVALID OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			return_value = "STACK OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			return_value = "STACK UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			return_value = "OUT OF MEMORY";
			break;
		case GL_NO_ERROR:
			return_value = "NO ERROR";
			break;
	}
	return return_value;
}

}        // namespace ror
