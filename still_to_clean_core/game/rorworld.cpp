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

#include "rorworld.h"

namespace ror
{
World::World(std::string a_file_name)
{}

World::~World(void)
{
	rDELETE(this->m_rendering_device);
	//rDELETE (m_ActiveCamera);

	if (!this->m_levels->empty())
	{
		std::vector<Level *>::iterator index;
		for (index = this->m_levels->begin(); index != this->m_levels->end(); ++index)
		{
			rDELETE(*index);
		}
	}
	m_levels->clear();
	//assert(m_Levels->size() == 0 && "m_Levels didn't Freed Properly.");
	this->m_levels->clear();
	rDELETE(this->m_shader_program);
	rDELETE(this->m_levels);
}

bool World::load_world(std::string a_file_name)
{
	ror::Level *temp_level                 = rNew ror::Level();
	functor_vector *              functors = temp_level->initilize(&this->m_active_camera, &this->m_shadow_light);
	if (nullptr == this->m_active_camera)
	{
		MESSAGEBOXERROR("Can't Find Any Camera in the Scene");
		exit(0);
	}
	if (nullptr == this->m_shadow_light)
	{
		MESSAGEBOXERROR("Can't Find Any Shadow Light in the Scene");
	}
	this->set_projection();
	this->m_levels->push_back(temp_level);

	// Here we Assign the Callbacks now
	functor_vector::iterator end = functors->end();

	for (functor_vector::iterator index = functors->begin(); index < end; ++index)
	{
		IFunctor *functor = nullptr;
		if ("CGUIElement" == (*index).m_class_id)
		{
			if ("HideMe" == (*index).m_function)        // Hiding the Panel : Testing :
			{
				functor = rNew ror::Functor<cguielement>((*index).m_pointer, &cguielement::hide_me);
			}
		}
		if ("CGUIGraphicsElement" == (*index).m_class_id)
		{
			if ("TurnONOffGround" == (*index).m_function)        // Hiding the Ground : Testing :
			{
				functor = rNew ror::Functor<cguigraphics_element>(dynamic_cast<cguigraphics_element *>((*index).m_pointer), &cguigraphics_element::toggle_background);
			}
		}

		if (nullptr != functor)
		{
			(*index).m_pointer->set_call_back(functor);
		}
	}
	functors->clear();        // Delete the Temporary data
	rDELETE(functors);        // This will delete the Functors Vector from Level

	// Enable Shadow Mapping
	this->m_shadow_mapping = ror::ShadowMap::get_instance();
	this->m_shadow_mapping->start();
	return true;
}

bool World::initilize()
{
	Window32::initilize();
	this->m_rendering_device->prepair();

	if (this->m_wireframe_rendering)
		this->m_rendering_device->setup_wireframe_rendering();
	else
		this->m_rendering_device->setup_shaded_rendering();

	this->load_world("");
	this->m_guimanager = GUI;

	// Hack ::
	std::string vert = WORKINGDIRECTORY, frag = WORKINGDIRECTORY;
	vert                   = vert + "Assets/Shaders/Vert.rvs";
	frag                   = frag + "Assets/Shaders/Frag.rfs";
	this->m_shader_program = rNew ror::OGLShaderProgram(vert.c_str(), frag.c_str());
	this->m_shader_program->enable();
	this->m_shader_program->send_uniform("shadowMap", this->m_shadow_mapping->get_shadow_map_id());
	this->m_shader_program->disable();

	GLenum t3 = glGetError();
	if (GL_NO_ERROR != t3)
	{
		float32_t a = 3;
	}

	return true;
}

void World::shutdown()
{
	//UnLoadWorld();
	// #ifdef USEROARMEMORYMANAGER
	// #ifdef _DEBUG
	//		DumpPotentialLeaks();
	// #endif
	// #endif
}

void World::un_load_world()
{
	rDELETE(this->m_rendering_device);
	rDELETE(this->m_active_camera);
	rDELETE((*this->m_levels)[0]);        // Hack
	rDELETE(this->m_levels);
}

}        // namespace ror
