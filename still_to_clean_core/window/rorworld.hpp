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
#include "opengl/roroglrendering_device.h"
#include "opengl/roroglshader_program.h"
#include "rorlevel.h"
#include "scene_graph/rormaya_camera.h"
#include "window/rorwindow32.h"

namespace ror
{
class ROAR_ENGINE_ITEM World : public application::Window32
{
  public:
	FORCE_INLINE World(void);
	FORCE_INLINE World(int32_t a_width, int32_t a_height, int32_t a_bpp);
	World(std::string a_file_name);
	virtual ~World(void);

	FORCE_INLINE void mouse_move(int32_t a_x, int32_t a_y);
	FORCE_INLINE void pre_render(){};
	FORCE_INLINE void render();
	FORCE_INLINE void post_render(){};
	FORCE_INLINE void update(rUsLong a_delta_time);
	FORCE_INLINE void set_projection();

	FORCE_INLINE void check_mouse_input();
	FORCE_INLINE void check_keyboard_input();
	FORCE_INLINE void draw_grid();

	bool initilize();
	void shutdown();
	bool reset()
	{
		return true;
	};
	bool load_world(std::string a_file_name);
	void un_load_world();

  protected:
	std::vector<Level *> *         m_levels;                  // Different Levels in the World
	ror::Camera *                  m_active_camera;           // Active Camera in the World (Maya Camera)
	rendering::OGLRenderingDevice *m_rendering_device;        // Rendering Device to use for rendering

	ror::Entities *m_player;        // Don't need to delete later

	bool m_grid;
	bool m_pause;
	bool m_menu;
	bool m_wireframe_rendering;
};
}        // namespace ror

#include "rorworld.hh"
