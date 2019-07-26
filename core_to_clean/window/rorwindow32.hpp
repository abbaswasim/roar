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

#ifdef _WIN32
#	include <windowsx.h>
#else
#	define HINSTANCE int32_t
#	define HWND uint32_t
#	define LPCWSTR std::string
#	define HDC uint32_t
#	define HPALETTE uint32_t
#	define HGLRC uint32_t
#	define RECT uint32_t
#	define DWORD uint64_t
#	define LRESULT uint32_t
#endif

#include "IO/rorkeyboard.h"
#include "IO/rormouse.h"
#include "camera/rorviewport.h"
#include "events/rorkeyboard_event.h"
#include "interfaces/rorievent_listener.h"
#include "memory_manager/rormemory_check.h"
#include "rorresources.h"

namespace application
{
class ROAR_ENGINE_ITEM Window32
{
  public:
	FORCE_INLINE Window32(void);
	FORCE_INLINE Window32(int32_t a_width, int32_t a_height, int32_t a_bpp);
	virtual ~Window32(void);

	// Must Be overridden by Client, Functions Mostly needed in OpenGL
	virtual bool initilize();
	virtual void render(){};
	virtual void shutdown(){};
	virtual void update(rUsLong a_milli_seconds){};
	virtual void set_projection(){};
	virtual bool register_window_class(HINSTANCE a_h_instance);

	void    reshape(int32_t a_width, int32_t a_height);
	bool    change_screen_resolution();
	void    set_vsync(int32_t interval = 1);
	bool    create_window32();
	bool    destroy_window32();
	bool    setup_opengl();
	void    abortion_clean_up();
	bool    setup_pixel_format();
	void    setup_palette();
	int32_t main_loop(HINSTANCE a_h_instance);
	void    toggle_fullscreen();        // WM_TOGGLEFULLSCREEN
	void    update_window_key_handling(rUsChar a_key, bool a_value);

	FORCE_INLINE ror::IEventListener *get_event_listener() const;
	FORCE_INLINE void                 set_event_listener(ror::IEventListener *a_event_listener);
	FORCE_INLINE int32_t get_window_width() const;
	FORCE_INLINE void    set_window_width(int32_t a_window_width);
	FORCE_INLINE int32_t get_window_height() const;
	FORCE_INLINE void    set_window_height(int32_t a_window_height);
	FORCE_INLINE int32_t get_zoom_factor() const;
	FORCE_INLINE void    set_zoom_factor(int32_t a_zoom_factor);

  protected:
	friend LRESULT   window_procedure(HWND a_h_wnd, uint32_t a_message, WPARAM a_w_param, LPARAM a_l_param);
	FORCE_INLINE void size(WPARAM a_w_param, LPARAM a_l_param);        // WM_SIZE
	FORCE_INLINE void close();                                         // WM_CLOSE
	FORCE_INLINE void mouse_wheel(int32_t a_delta);                    // WM_MOUSEWHEEL

	// Important stuff for win32 Platform
	HINSTANCE m_h_instance;
	LPCWSTR   m_class_name;
	int32_t   m_bits_per_pixel;
	LPCWSTR   m_title;
	HWND      m_h_wnd;
	HDC       m_h_dc;
	HPALETTE  m_h_palette;
	HGLRC     m_h_rc;
	bool      m_is_visible;
	RECT      m_window_rectangle;

	// Window size in pixels and other parameters
	int32_t m_window_width;
	int32_t m_window_height;
	int32_t m_zoom_factor;
	bool    m_is_full_screen;
	bool    m_is_program_looping;
	DWORD   m_last_tick_count;

	ror::Keyboard *      m_keyboard;
	ror::Mouse *         m_mouse;
	ror::IEventListener *m_event_listener;

  private:
	// Windows Message Handling Functions
	FORCE_INLINE void palette_changed(WPARAM w_param);                          // WM_PALETTECHANGED
	FORCE_INLINE bool query_new_palette();                                      // WM_QUERYNEWPALETTE
	FORCE_INLINE void paint();                                                  // WM_PAINT
	FORCE_INLINE void size_minimize();                                          // SIZE_MINIMIZED
	FORCE_INLINE void size_maximize(WPARAM a_w_param, LPARAM a_l_param);        // SIZE_MAXIMIZED
	FORCE_INLINE void size_restore(WPARAM a_w_param, LPARAM a_l_param);         // SIZE_RESTORED
	FORCE_INLINE void mouse_leave();                                            // WM_MOUSELAEVE
	FORCE_INLINE void left_button_down(int32_t a_x, int32_t a_y);               // WM_LBUTTONDOWN
	FORCE_INLINE void middle_button_down(int32_t a_x, int32_t a_y);             // WM_MBUTTONDOWN
	FORCE_INLINE void right_button_down(int32_t a_x, int32_t a_y);              // WM_RBUTTONDOWN
	FORCE_INLINE void left_button_up(int32_t a_x, int32_t a_y);                 // WM_LBUTTONUP
	FORCE_INLINE void middle_button_up(int32_t a_x, int32_t a_y);               // WM_MBUTTONUP
	FORCE_INLINE void right_button_up(int32_t a_x, int32_t a_y);                // WM_RBUTTONUP
	FORCE_INLINE void key_down(rUsChar a_key, bool a_value);                    // WM_KEYDOWN
	FORCE_INLINE void key_up(rUsChar a_key, bool a_value);                      // WM_KEYUP
};
}        // namespace application

#include "rorwindow32.hh"
