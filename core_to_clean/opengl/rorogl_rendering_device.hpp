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

#include "foundation/rorfont_manager.h"
#include "game/rorboids.h"
#include "game/rorgame.h"
#include "interfaces/rorirendering_device.h"
#include "models/rorcba_manager.h"
#include "opengl/rorogl_frame_buffer.h"
#include "utility/rorutility.h"

namespace rendering
{
// TODO : try to clean this class, its full of shit :)
class ROAR_ENGINE_ITEM OGLRenderingDevice : public rendering::IRenderingDevice
{
  public:
	FORCE_INLINE OGLRenderingDevice(void);
	~OGLRenderingDevice(void);

	FORCE_INLINE void               pre_render();
	FORCE_INLINE void               render();
	FORCE_INLINE void               post_render();
	FORCE_INLINE bool               update(rUsLong a_delta_time);
	FORCE_INLINE static std::string parse_errors();
	FORCE_INLINE void               draw_view_port();
	FORCE_INLINE void               identity();
	FORCE_INLINE Vector3f camera_position();
	FORCE_INLINE Vector3f world_coords(const Vector3f &a_screen_coords);
	FORCE_INLINE Vector3f screen_coords(const Vector3f &a_world_position);
	FORCE_INLINE void     setup_wireframe_rendering();
	FORCE_INLINE void     setup_shaded_rendering();

	static FORCE_INLINE void push_matrix();
	static FORCE_INLINE void pop_matrix();
	static FORCE_INLINE void set_back_color(float32_t a_r, float32_t a_g, float32_t a_b, float32_t a_a);
	static FORCE_INLINE void set_color(ror::Vector3f &a_color);
	static FORCE_INLINE void set_color(ror::Vector4f &a_color);
	static FORCE_INLINE void set_color(ror::Vector3i &a_color);
	static FORCE_INLINE void set_color(float32_t a_r, float32_t a_g, float32_t a_b);
	static FORCE_INLINE void set_color(int32_t a_r, int32_t a_g, int32_t a_b);
	static FORCE_INLINE void save_current_color();
	static FORCE_INLINE void restore_current_color();
	static FORCE_INLINE void get_viewport(int32_t a_viewport[4]);
	static FORCE_INLINE void enable2dmode();
	static FORCE_INLINE void disable2dmode();
	static FORCE_INLINE void save_render_state();
	static FORCE_INLINE void restore_render_state();

	bool prepair();
	bool shutdown();
	bool reset();

	void draw_sphere(float32_t a_radius, int32_t a_slices, int32_t a_stacks);

	game::Game *get_game() const
	{
		return m_game;
	}
	void set_game(game::Game *a_val)
	{
		m_game = a_val;
	}

	FORCE_INLINE void sin_cos(float32_t *return_sin, float32_t *return_cos, float32_t theta);
	void             setup(float32_t heading, float32_t pitch, float32_t bank, float32_t xtrans, float32_t ytrans, float32_t ztrans, float32_t *matrix);

	float32_t multiplier;
	//		uint32_t m_ShadowMapTexture;	// Testing
	//		uint32_t m_ShadowMapSize;		// Testing
	//		CMatrix4f m_LightViewMatrix;				// Camera Matrix Used for the Camera Settings,
	//		int32_t	m_ViewPort[4];
  private:
	static int32_t m_current_color[4];

	game::Game * m_game;
	ror::Timer * m_timer;
	game::Boids *m_boids;

	float32_t m_maximum_delta_time;
	float32_t m_counter;
	rLong     m_fpscounter;
	GLuint    m_i_fbcol, m_i_fbdepth;        // Testing :: color and depth framebuffer objects
	GLuint    m_i_tex_id;                    // Testing :: ID of dynamic texture

	ror::CCBA **      my_cba;              // Testing
	ror::ccbamanager *m_cbamanager;        // Testing

	ror::OGLFrameBuffer *   m_fram_buffer;            // Testing
	ror::OGLTextureManager *m_texture_manager;        // Testing
	ror::TextureObject *    m_texture;                // Testing
	bool                    m_registered;
};
}        // namespace rendering

#include "rorogl_rendering_device.inl"
