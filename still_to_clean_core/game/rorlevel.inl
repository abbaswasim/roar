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
FORCE_INLINE Level::Level(void)
{
	this->m_cbamanager               = ror::ccbamanager::get_instance();
	this->m_shader_manager           = ror::OGLShaderManager::get_instance();
	this->m_texture_manager          = ror::OGLTextureManager::get_instance();
	this->m_particles_system_manager = ror::ParticleSystemManager::get_instance();
	this->m_font_manager             = ror::FontManager::get_instance();
	this->m_guimanager               = ror::cguimanager::get_instance();
	this->m_cameras                  = new std::vector<ror::Camera *>();
	this->m_lights                   = new std::vector<ror::Light *>();
	this->m_dynamic_textures         = new std::vector<ror::OGLFrameBuffer *>();
	this->m_scene_root               = nullptr;
	this->m_functors                 = new functor_vector();
}

FORCE_INLINE Level::Level(std::string a_file_name)
{
	this->m_cbamanager = ror::ccbamanager::get_instance();
	this->m_scene_root = nullptr;
}

FORCE_INLINE functor_vector *Level::initilize(ror::Camera **a_active_camera, ror::Light **a_shadow_light)
{
	std::string file_name = "Assets/Scene.xml";
	file_name             = WORKINGDIRECTORY + file_name;
	this->load_level(file_name, a_active_camera, a_shadow_light);

	return this->m_functors;
}

FORCE_INLINE ror::Entities *Level::get_scene_root() const
{
	return this->m_scene_root;
}

FORCE_INLINE ror::Entities *Level::get_entity() const
{
	return (Entities *) this->m_scene_root->get_child_at(0);        //->GetChildAt(0); // Testing :
}

FORCE_INLINE void Level::iterate_camera(ror::Camera **a_active_camera)
{
	if (!this->m_cameras->empty())
	{
		std::vector<ror::Camera *>::iterator camera_iterator = find(this->m_cameras->begin(), this->m_cameras->end(), *a_active_camera);
		if (this->m_cameras->end() != camera_iterator)
		{
			camera_iterator++;
			if (this->m_cameras->end() != camera_iterator)
			{
				*a_active_camera = *camera_iterator;
			}
			else
			{
				*a_active_camera = *this->m_cameras->begin();
			}
		}
	}
}

FORCE_INLINE void Level::set_scene_root(ror::Entities *a_root)
{
	this->m_scene_root = a_root;
}

FORCE_INLINE void Level::render(const rendering::IRenderingDevice *a_rendering_device, const ror::Camera *a_active_camera, bool a_camera_moved, ERenderMethod a_render_method)
{
	static uint32_t dynamic_texture_size = 0;
	dynamic_texture_size                 = (uint32_t) this->m_dynamic_textures->size();

	if (rRENDER_FULL == a_render_method)
	{
		for (uint32_t i = 0; i < dynamic_texture_size; i++)
		{
			//this->SaveRenderState();
			(*this->m_dynamic_textures)[i]->start_render();
			this->m_scene_root->draw();
			(*this->m_dynamic_textures)[i]->end_render();
			//this->RestoreRenderState();
		}
	}

	static Vector2i visible;

	if (a_camera_moved)
	{
		visible = this->m_scene_root->generate_visibility_set(a_active_camera);
	}

	glPushMatrix();
	//glRotatef(-90,1,0,0);
	this->m_scene_root->draw();
	//m_SceneRoot->DrawAxis();
	//m_SceneRoot->DrawInWorldSpace();
	//m_SceneRoot->DrawParentLines();
	glPopMatrix();

	//		glDisable(GL_LIGHTING);
	//		Rendering::COGLRenderer::Enable2DMode();
	//		this->m_GUIManager->Render();
	//		Rendering::COGLRenderer::Disable2DMode();
	//		glEnable(GL_LIGHTING);

	if (rRENDER_FULL == a_render_method)
	{
		visible.y                      = visible.y - visible.x;
		static ror::Font *default_font = FONTDEFAULT;
		default_font->safe_print(0, 2, "Culled %d", visible.x);
		default_font->safe_print(0, 4, "Visible %d", visible.y);
	}
	//rDELETE (Printer);
}

FORCE_INLINE void Level::update(rUsLong a_milli_seconds)
{
	this->m_scene_root->update(a_milli_seconds);
}

FORCE_INLINE void Level::save_render_state()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

FORCE_INLINE void Level::restore_render_state()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

}        // namespace ror
