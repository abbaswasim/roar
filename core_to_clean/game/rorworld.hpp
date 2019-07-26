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

#include "appearance/rorshadow_map.h"
#include "camera/rorfirst_person_camera.h"
#include "camera/rormaya_camera.h"
#include "opengl/rorogl_rendering_device.h"
#include "opengl/rorogl_shader_program.h"
#include "rorlevel.h"
#include "window/rorwindow32.h"

namespace ror
{
class ROAR_ENGINE_ITEM World : public application::Window32, public ror::IEventListener, public ror::iguievent_listener
{
  public:
	FORCE_INLINE World(void);
	FORCE_INLINE World(int32_t a_width, int32_t a_height, int32_t a_bpp);
	World(std::string a_file_name);
	virtual ~World(void);

	//FORCE_INLINE void		MouseMove(int32_t a_X,int32_t a_Y);
	FORCE_INLINE void pre_render(){};
	FORCE_INLINE void render();
	FORCE_INLINE void post_render(){};
	FORCE_INLINE void update(rUsLong a_delta_time);
	FORCE_INLINE void set_projection();

	//		FORCE_INLINE void		CheckMouseInput();
	//		FORCE_INLINE void		CheckKeyboardInput();
	FORCE_INLINE void draw_grid();

	bool initilize();
	void shutdown();
	bool reset()
	{
		return true;
	};
	bool load_world(std::string a_file_name);
	void un_load_world();

	// Input Event Listener Functions
	FORCE_INLINE void mouse_moved(ror::MouseEvent &a_event);
	FORCE_INLINE void mouse_leaved(ror::MouseEvent &a_event);
	FORCE_INLINE void mouse_pressed(ror::MouseEvent &a_event);
	FORCE_INLINE void mouse_released(ror::MouseEvent &a_event);
	FORCE_INLINE void mouse_scrolled(ror::MouseEvent &a_event);
	FORCE_INLINE void mouse_double_clicked(ror::MouseEvent &a_event);

	FORCE_INLINE void key_pressed(ror::KeyboardEvent &a_event);
	FORCE_INLINE void key_released(ror::KeyboardEvent &a_event);

	FORCE_INLINE void window_closing(ror::WindowEvent &a_event);
	FORCE_INLINE void window_size_changed(ror::WindowEvent &a_event);
	FORCE_INLINE bool draw_shadows() const;
	FORCE_INLINE void draw_shadows(bool a_draw_shadows);
	FORCE_INLINE Light *get_shadow_light() const;
	FORCE_INLINE void   set_shadow_light(ror::Light *a_shadow_light);

	// GUI Event Listener Functions
	FORCE_INLINE void guievent_triggered(ror::cguievent &a_event);

  protected:
	std::vector<Level *> *m_levels;                   // Different Levels in the World
	ror::Camera *         m_active_camera;            // Active Camera in the World (Maya Camera)
	RENDERER *            m_rendering_device;         // Rendering Device to use for rendering
	ror::Entities *       m_player;                   // A pointer to the main Player, the CWorld Does not take Ownership of this
	ror::cguimanager *    m_guimanager;               // Utility for Managing all the GUI Elements
	ror::ShadowMap *      m_shadow_mapping;           // Shadow Mapping
	ror::Light *          m_shadow_light;             // This is the light which will be used to draw the shadows in the scenes, world don't take ownership
	ror::Matrix4f         m_projection_matrix;        // Testing

	bool m_grid;
	bool m_pause;
	bool m_menu;
	bool m_wireframe_rendering;
	bool m_render_gui;
	bool m_draw_shadows;        // Shadows Toggle

	ror::OGLShaderProgram *m_shader_program;        // Testing ::
};
}        // namespace ror

#include "rorworld.hh"
