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

#include "rorogl_shader_manager.h"

namespace ror
{
OGLShaderManager::~OGLShaderManager(void)
{
	rDELETE(m_shaders);
}

OGLShader *OGLShaderManager::create_shader(std::string a_file_name, EShaderType a_type)
{
	// Before trying to create a new Shader check if its already in the Shaders Loaded
	shader_objects_map::iterator iter = m_shaders->find(a_file_name);
	if (iter != m_shaders->end())
	{
		iter->second->increment_reference_count();
		return iter->second->get_shader_pointer();
	}
	// Otherwise we load it from disk
	ror::OGLShader *temp_shader = rNew ror::OGLShader(a_file_name, a_type);

	ror::ShaderObject *new_shader_object = rNew ror::ShaderObject();
	new_shader_object->set_shader_pointer(temp_shader);
	new_shader_object->set_shader_id(temp_shader->get_id());

	m_shaders->insert(std::make_pair(std::string(a_file_name), new_shader_object));

	return new_shader_object->get_shader_pointer();
}

void OGLShaderManager::release_shader(OGLShader *a_shader_pointer)
{
	if (nullptr != a_shader_pointer)
	{
		shader_objects_map::iterator iter;
		for (iter = m_shaders->begin(); iter != m_shaders->end(); ++iter)
		{
			if (a_shader_pointer == iter->second->get_shader_pointer())
			{
				if (iter->second->decrement_reference_count() == 0)
				{
					rDELETE(a_shader_pointer);
					rDELETE(iter->second);
					m_shaders->erase(iter);
				}
				return;
			}
		}
	}
}

void OGLShaderManager::release_all_shaders()
{
	shader_objects_map::iterator iter;
	for (iter = m_shaders->begin(); iter != m_shaders->end(); ++iter)
	{
		ror::OGLShader *temp_pointer = iter->second->get_shader_pointer();
		rDELETE(iter->second);
		rDELETE(temp_pointer);
	}
	m_shaders->clear();
}

}        // namespace ror
