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

#include "camera/rorfirst_person_camera.h"
#include "camera/rormaya_camera.h"
#include "foundation/rorfont_manager.h"
#include "foundation/rorfunctor.h"
#include "gui/rorgui_button.h"
#include "gui/rorgui_label.h"
#include "gui/rorgui_manager.h"
#include "interfaces/rorirendering_device.h"
#include "models/rorcba_manager.h"
#include "opengl/rorogl_frame_buffer.h"
#include "opengl/rorogl_shader_manager.h"
#include "opengl/rorogl_texture_manager.h"
#include "particles/rorparticle_system_manager.h"
#include "rorfunctor_data.h"
#include "scene_graph/rorbillboard_node.h"
#include "scene_graph/rorclouds_node.h"
#include "scene_graph/rordummy_node.h"
#include "scene_graph/rorlight_node.h"
#include "scene_graph/rorparticles_node.h"
#include "scene_graph/rorterrain_node.h"
#include "thirdparty/tinyxml/tinyxml.h"

namespace ror
{
class ROAR_ENGINE_ITEM World;
class ROAR_ENGINE_ITEM Level
{
  public:
	enum ERenderMethod
	{
		rRENDER_FULL,
		rRENDER_SHADOW
	};
	typedef std::unordered_map<std::string, std::string> entities_map;

	FORCE_INLINE Level(void);
	FORCE_INLINE Level(std::string a_file_name);
	virtual ~Level(void);

	virtual functor_vector *initilize(ror::Camera **a_active_camera, ror::Light **a_shadow_light);
	virtual void            render(const rendering::IRenderingDevice *a_rendering_device, const ror::Camera *a_active_camera, bool a_camera_moved, ERenderMethod a_render_method);
	virtual void            update(rUsLong a_milli_seconds);

	FORCE_INLINE Entities *get_entity() const;
	FORCE_INLINE Entities *get_scene_root() const;
	FORCE_INLINE void      set_scene_root(ror::Entities *a_root);
	FORCE_INLINE void      iterate_camera(ror::Camera **a_active_camera);
	FORCE_INLINE void      save_render_state();
	FORCE_INLINE void      restore_render_state();

	void                    load_level(std::string a_file_name, ror::Camera **a_active_camera, ror::Light **a_shadow_light);
	void                    load_guiscene(std::string a_file_name);
	void                    un_load_level();
	void                    parse_scene(TiXmlNode *a_entities, entities_map &a_entities_map, ror::Camera **a_active_camera, ror::Light **a_shadow_light);
	void                    parse_node(TiXmlNode *a_entities, entities_map &a_entities_map, ror::Entities *a_parent, ror::Camera **a_active_camera, ror::Light **a_shadow_light);
	cguielement *           parse_guielement(TiXmlNode *a_guielement);
	entities_map            parse_entities(TiXmlNode *a_entities);
	int32_t                 read_font(TiXmlNode *a_font_node);
	void                    setup_dynamic_texture(ror::Entities *a_entity);
	TextureObject *         read_texture(TiXmlNode *a_texture_node, Vector4f &a_texture_offset, bool &a_scalable);
	void                    read_call_back(TiXmlNode *a_functor_node, cguielement *a_element);
	cguielement *           read_label(TiXmlNode *a_label_node, cguielement *a_parent);
	cguielement *           read_button(TiXmlNode *a_button_node, cguielement *a_parent);
	void                    read_button_state(TiXmlNode *a_button_node, ror::cguibutton *a_button, cguibutton::EButtonState a_state);
	void                    string_to_integers(std::string a_string, int32_t a_size, int32_t *a_ints);
	void                    string_to_floats(std::string a_string, int32_t a_size, float32_t *a_floats);
	cguielement::EAlignment enumerate_alignment(std::string a_alignment);

  protected:
	// World should be able to access it
	friend class ROAR_ENGINE_ITEM World;

	ror::Entities *                     m_scene_root;                      // Root Node of the Scene Graph
	ror::ccbamanager *                  m_cbamanager;                      // Utility for Model Loading
	ror::OGLShaderManager *             m_shader_manager;                  // Utility for Shader Loading
	ror::OGLTextureManager *            m_texture_manager;                 // Utility for Texture Loading
	ror::ParticleSystemManager *        m_particles_system_manager;        // Utility for Particles Loading
	ror::FontManager *                  m_font_manager;                    // Utility for Fonts Loading and deleting
	ror::cguimanager *                  m_guimanager;                      // Utility for Managing all the GUI Elements
	std::vector<ror::Camera *> *        m_cameras;                         // All the cameras in the Level
	std::vector<ror::Light *> *         m_lights;                          // All the Lights in the Level
	std::vector<ror::OGLFrameBuffer *> *m_dynamic_textures;                // All the dynamic Textures needs to be rendered
	functor_vector *                    m_functors;                        // All the functors in the Current Scene
};
}        // namespace ror

#include "rorlevel.hh"
