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

namespace application
{
FORCE_INLINE Window32::Window32(void)
{
	// Fill Out Window
	this->m_class_name         = "RoarClass";
	this->m_title              = "Roar ...";
	this->m_window_width       = 800;
	this->m_window_height      = 600;
	this->m_zoom_factor        = 0;
	this->m_bits_per_pixel     = 32;
	this->m_is_full_screen     = true;
	this->m_mouse              = ror::Mouse::get_instance();
	this->m_keyboard           = ror::Keyboard::get_instance();
	this->m_event_listener     = nullptr;
	this->m_is_program_looping = true;
	//m_RenderingInstance = new Rendering::COpenGL;
}

FORCE_INLINE Window32::Window32(int32_t a_width, int32_t a_height, int32_t a_bpp)
{
	// Fill Out Window
	this->m_class_name         = "RoarClass";
	this->m_title              = "Roar ...";
	this->m_window_width       = a_width;
	this->m_window_height      = a_height;
	this->m_zoom_factor        = 0;
	this->m_bits_per_pixel     = a_bpp;
	this->m_is_full_screen     = true;
	this->m_mouse              = ror::Mouse::get_instance();
	this->m_keyboard           = ror::Keyboard::get_instance();
	this->m_event_listener     = nullptr;
	this->m_is_program_looping = true;
	//m_RenderingInstance = new Rendering::COpenGL;
}

FORCE_INLINE void Window32::size(WPARAM a_w_param, LPARAM a_l_param)
{
	switch (a_w_param)
	{
		case 0:        //SIZE_MINIMIZED:
			this->size_minimize();
			return;
		case 1:        //SIZE_MAXIMIZED:
			this->size_maximize(a_w_param, a_l_param);
			return;
		case 2:        //SIZE_RESTORED:
			this->size_restore(a_w_param, a_l_param);
			return;
	}
}

FORCE_INLINE void Window32::size_minimize()
{
	this->m_is_visible = false;
}

FORCE_INLINE void Window32::size_maximize(WPARAM a_w_param, LPARAM a_l_param)
{
	this->m_is_visible = true;
	this->reshape(LOWORD(a_l_param), HIWORD(a_l_param));
}

FORCE_INLINE void Window32::size_restore(WPARAM a_w_param, LPARAM a_l_param)
{
	this->m_is_visible = true;
	this->reshape(LOWORD(a_l_param), HIWORD(a_l_param));
}

FORCE_INLINE void Window32::close()
{
	return;
	//PostMessage (m_hWnd, WM_QUIT, 0, 0);		//this is bad practice
	this->m_is_program_looping = false;
	this->destroy_window32();
	//UnregisterClass (this->m_ClassName, this->m_hInstance);
	this->shutdown();
	//if (!m_IsFullScreen)
	//exit(0);
}

FORCE_INLINE void Window32::key_down(rUsChar a_key, bool a_value)
{
	this->update_window_key_handling(a_key, a_value);
	a_value ?
	    this->m_keyboard->setkey_down(a_key) :
	    this->m_keyboard->setkey_up(a_key);
}

FORCE_INLINE void Window32::key_up(rUsChar a_key, bool a_value)
{
	a_value ?
	    this->m_keyboard->setkey_up(a_key) :
	    this->m_keyboard->setkey_down(a_key);
}

FORCE_INLINE void Window32::left_button_down(int32_t a_x, int32_t a_y)
{
	//SetCapture(this->m_hWnd);
	this->m_mouse->set_left_button_down(true);
	this->m_mouse->set_old_xy(a_x, a_y);
}

FORCE_INLINE void Window32::middle_button_down(int32_t a_x, int32_t a_y)
{
	//SetCapture(this->m_hWnd);
	this->m_mouse->set_middle_button_down(true);
	this->m_mouse->set_old_xy(a_x, a_y);
}

FORCE_INLINE void Window32::right_button_down(int32_t a_x, int32_t a_y)
{
	//SetCapture(this->m_hWnd);
	this->m_mouse->set_right_button_down(true);
	this->m_mouse->set_old_xy(a_x, a_y);
}

FORCE_INLINE void Window32::left_button_up(int32_t a_x, int32_t a_y)
{
	//ReleaseCapture();
	this->m_mouse->set_left_button_down(false);
	this->m_mouse->set_old_xy(a_x, a_y);
}

FORCE_INLINE void Window32::middle_button_up(int32_t a_x, int32_t a_y)
{
	//ReleaseCapture();
	this->m_mouse->set_middle_button_down(false);
	this->m_mouse->set_old_xy(a_x, a_y);
}

FORCE_INLINE void Window32::right_button_up(int32_t a_x, int32_t a_y)
{
	//ReleaseCapture();
	this->m_mouse->set_right_button_down(false);
	this->m_mouse->set_old_xy(a_x, a_y);
}

FORCE_INLINE void Window32::mouse_leave()
{
	//ReleaseCapture();
	this->m_mouse->set_left_button_down(false);
	this->m_mouse->set_middle_button_down(false);
	this->m_mouse->set_right_button_down(false);
}
FORCE_INLINE void Window32::mouse_wheel(int32_t a_delta)
{
	if (a_delta < 0)
	{
		if (this->m_zoom_factor > -30)
		{
			this->m_zoom_factor -= 5;
		}
	}
	else
	{
		if (this->m_zoom_factor < 125)
		{
			this->m_zoom_factor += 5;
		}
	}

	this->reshape(this->m_window_width, this->m_window_height);
}

FORCE_INLINE void Window32::set_event_listener(ror::IEventListener *a_event_listener)
{
	this->m_event_listener = a_event_listener;
}

FORCE_INLINE ror::IEventListener *Window32::get_event_listener() const
{
	return this->m_event_listener;
}

FORCE_INLINE void Window32::set_zoom_factor(int32_t a_zoom_factor)
{
	this->m_zoom_factor = a_zoom_factor;
}

FORCE_INLINE int32_t Window32::get_zoom_factor() const
{
	return this->m_zoom_factor;
}

FORCE_INLINE void Window32::set_window_height(int32_t a_window_height)
{
	this->m_window_height = a_window_height;
}

FORCE_INLINE int32_t Window32::get_window_height() const
{
	return this->m_window_height;
}

FORCE_INLINE void Window32::set_window_width(int32_t a_window_width)
{
	this->m_window_width = a_window_width;
}

FORCE_INLINE int32_t Window32::get_window_width() const
{
	return this->m_window_width;
}

}        // namespace application
