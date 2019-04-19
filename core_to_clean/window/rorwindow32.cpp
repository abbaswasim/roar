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

#include "rorwindow32.h"
#include "profiling/rorlog.h"

namespace application
{
#define WM_TOGGLEFULLSCREEN (WM_USER + 1)

#ifndef WM_MOUSEWHEEL
#	define WM_MOUSEWHEEL 0x020a
#endif

#define ENABLE_DISABLE_VSYNC true        // True Enables and False disables it

Window32::~Window32(void)
{
	// TODO : do the cleanup here
	/* Can't delete Static Pointers
		RDELETE (m_Mouse);
		RDELETE (m_Keyboard);
		*/
}

bool Window32::change_screen_resolution()
{
#ifdef _WIN32
	DEVMODE dm_screen_settings;
	memset(&dm_screen_settings, 0, sizeof(DEVMODE));
	dm_screen_settings.dm_size         = sizeof(DEVMODE);
	dm_screen_settings.dm_pels_width   = this->m_window_width;
	dm_screen_settings.dm_pels_height  = this->m_window_height;
	dm_screen_settings.dm_bits_per_pel = this->m_bits_per_pixel;
	dm_screen_settings.dm_fields       = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	if (change_display_settings(&dm_screen_settings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return false;
	}
#endif
	return true;
}

void Window32::abortion_clean_up()
{
#ifdef _WIN32
	release_dc(this->m_h_wnd, this->m_h_dc);
	this->m_h_dc = 0;
	destroy_window(this->m_h_wnd);
	this->m_h_wnd = 0;
#endif
}

bool Window32::setup_pixel_format()
{
#ifdef _WIN32
	PIXELFORMATDESCRIPTOR pixel_format_descriptor =
	    {
	        sizeof(PIXELFORMATDESCRIPTOR),        // Size Of This Pixel Format Descriptor
	        1,                                    // Version Number
	        PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
	        this->m_bits_per_pixel,        // Select Our Color Depth
	        0, 0, 0, 0, 0, 0,              // Color Bits Ignored
	        0,                             // No Alpha Buffer
	        0,                             // Shift Bit Ignored
	        0,                             // No Accumulation Buffer
	        0, 0, 0, 0,                    // Accumulation Bits Ignored
	        16,                            // 16Bit Z-Buffer (Depth Buffer)
	        0,                             // No Stencil Buffer
	        0,                             // No Auxiliary Buffer
	        PFD_MAIN_PLANE,                // Main Drawing Layer
	        0,                             // Reserved
	        0, 0, 0                        // Layer Masks Ignored
	    };

	int32_t pixel_format_id = choose_pixel_format(this->m_h_dc, &pixel_format_descriptor);

	/************************************************************************/
	// Check if you get Hardware Acceleration?
	PIXELFORMATDESCRIPTOR pfd_new;
	describe_pixel_format(this->m_h_dc, pixel_format_id, sizeof(PIXELFORMATDESCRIPTOR), &pfd_new);

	int32_t generic_format      = pfd_new.dw_flags & PFD_GENERIC_FORMAT;
	int32_t generic_accelerated = pfd_new.dw_flags & PFD_GENERIC_ACCELERATED;

	if (generic_format && !generic_accelerated)
	{
		// software
		message_box(this->m_h_wnd, L"Running in Software mode.", L"Software Rendering...", MB_OK | MB_ICONINFORMATION);
	}
	else if (generic_format && generic_accelerated)
	{
		// hardware - MCD
		ror::Log::get_instance()->message("Using Hardware Acceleration - MCD");
	}
	else if (!generic_format && !generic_accelerated)
	{
		// hardware - ICD
		ror::Log::get_instance()->message("Using Hardware Acceleration");
	}
	/************************************************************************/
	if (!pixel_format_id)
		return false;

	if (!set_pixel_format(this->m_h_dc, pixel_format_id, &pixel_format_descriptor))
		return false;
#endif
	return true;
}

bool Window32::setup_opengl()
{
#ifdef _WIN32
	this->m_h_dc = get_dc(this->m_h_wnd);
	if (this->m_h_dc == 0)
	{
		destroy_window(this->m_h_wnd);
		return false;
	}

	if (!this->setup_pixel_format())
		return false;

	this->m_h_rc = wgl_create_context(this->m_h_dc);
	if (this->m_h_rc == 0)
	{
		this->abortion_clean_up();
		return false;
	}

	if (wgl_make_current(this->m_h_dc, this->m_h_rc) == false)
	{
		wgl_delete_context(this->m_h_rc);
		this->m_h_rc = 0;
		this->abortion_clean_up();
		return false;
	}

	this->reshape(this->m_window_width, this->m_window_height);
#endif
	return true;
}

void Window32::set_vsync(int32_t interval /*=1*/)
{
#ifdef _WIN32
	typedef bool(APIENTRY * PFNWGLSWAPINTERVALFARPROC)(int32_t);
	PFNWGLSWAPINTERVALFARPROC wgl_swap_interval_ext = 0;

	const rUsChar *extensions = glGetString(GL_EXTENSIONS);

	if (strstr((char8_t *) extensions, "WGL_EXT_swap_control") == 0)
		return;
	else
	{
		wgl_swap_interval_ext = (PFNWGLSWAPINTERVALFARPROC) wgl_get_proc_address("wglSwapIntervalEXT");
		if (wgl_swap_interval_ext)
			wgl_swap_interval_ext(!interval);
	}
#endif
}

bool Window32::create_window32()
{
#ifdef _WIN32
	DWORD window_style          = WS_OVERLAPPEDWINDOW;
	DWORD window_extended_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	RECT window_rect = {0, 0, this->m_window_width, this->m_window_height};
	//MESSAGEBOXERROR("Before Creating Window");
	if (this->m_is_full_screen == true)
	{
		if (change_screen_resolution() == false)
		{
			message_box(HWND_DESKTOP, L"Mode Switch Failed.\nRunning In Windowed Mode.", L"Error...2", MB_OK | MB_ICONEXCLAMATION);
			this->m_is_full_screen = false;
		}
		else
		{
			show_cursor(false);
			window_style = WS_POPUP;
			window_extended_style |= WS_EX_TOPMOST;
		}
	}
	else
	{
		// Adjust Window, Account For Window Borders
		adjust_window_rect_ex(&window_rect, window_style, 0, window_extended_style);
	}

	// Create The OpenGL Window
	this->m_h_wnd = create_window_ex(window_extended_style, m_class_name, this->m_title, window_style,
	                                 0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
	                                 HWND_DESKTOP, 0, (HINSTANCE) get_module_handle(nullptr), (void *) this);

	if (this->m_h_wnd == 0)
		return false;

	//this->SetupOpenGL();
	//if (this->m_IsFullScreen != true) // Testig :
	this->m_is_visible = !show_window(this->m_h_wnd, /*SW_NORMAL*/ SW_MAXIMIZE);
	update_window(this->m_h_wnd);

	this->m_last_tick_count = get_tick_count();
#endif
	return true;
}

bool Window32::destroy_window32()
{
#ifdef _WIN32
	if (this->m_h_wnd != 0)
	{
		if (this->m_h_dc != 0)
		{
			wgl_make_current(this->m_h_dc, nullptr);
			if (this->m_h_rc != 0)
			{
				wgl_delete_context(this->m_h_rc);
				this->m_h_rc = 0;
			}
			release_dc(this->m_h_wnd, this->m_h_dc);
			this->m_h_dc = 0;
		}
		destroy_window(this->m_h_wnd);
		this->m_h_wnd = 0;
	}

	if (!this->m_is_full_screen)
	{
		change_display_settings(nullptr, 0);
		show_cursor(true);
	}
#endif
	return true;
}

bool Window32::register_window_class(HINSTANCE a_h_instance)
{
#ifdef _WIN32
	WNDCLASSEX window_class;
	this->m_h_instance = a_h_instance;

	zero_memory(&window_class, sizeof(WNDCLASSEX));

	window_class.cb_size         = sizeof(WNDCLASSEX);
	window_class.h_instance      = this->m_h_instance;
	window_class.lpsz_class_name = this->m_class_name;
	window_class.style           = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfn_wnd_proc   = window_procedure;
	window_class.cb_cls_extra    = 0;
	window_class.cb_wnd_extra    = 0;
	window_class.lpsz_menu_name  = nullptr;        //MAKEINTRESOURCE(IDR_MENU);
	window_class.hbr_background  = (HBRUSH) COLOR_WINDOW + 1;
	window_class.h_icon          = load_icon(a_h_instance, MAKEINTRESOURCE(IDI_ICON1));
	window_class.h_cursor        = load_cursor(nullptr, IDC_ARROW);
	window_class.h_icon_sm       = load_icon(a_h_instance, MAKEINTRESOURCE(IDI_ICON1));

	if (register_class_ex(&window_class) == 0)
	{
		message_box(HWND_DESKTOP, L"RegisterClassEx Failed!", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
#endif
	return true;
}

int32_t Window32::main_loop(HINSTANCE a_h_instance)
{
#ifdef _WIN32
	bool    message_pump_running;
	MSG     window_message;
	rUsLong tick_count;
	bool    updated = false;

	if (this->register_window_class(a_h_instance) == false)
	{
		MESSAGEBOXERROR("Error Registering Window Class!");
		return -1;
	}

	this->m_is_full_screen = false;

	while (this->m_is_program_looping)
	{
		if (this->create_window32() == true)
		{
			if (this->initilize() == false)        // FUll Screen Functionality is currently broken, if it needs update this must be taken away
				this->close();
			else
			{
				message_pump_running = true;
				set_vsync(ENABLE_DISABLE_VSYNC);        // Turn off Vsync

				while (message_pump_running == true)
				{
					if (peek_message(&window_message, this->m_h_wnd, 0, 0, PM_REMOVE) != 0)
					{
						if (window_message.message != WM_QUIT)
							dispatch_message(&window_message);
						else
							message_pump_running = false;
					}
					else
					{
						if (this->m_is_visible == false)
							wait_message();
						else
						{
							// Process Application Loop and Rendering etc
							tick_count = get_tick_count();                             // Get The Tick Count
							this->update(tick_count - this->m_last_tick_count);        // Update The Counter
							this->m_last_tick_count = tick_count;                      // Set Last Count To Current Count
							this->render();                                            // Draw Our Scene
							swap_buffers(this->m_h_dc);
						}
					}
				}

			}        // (Initilize() == false)
			this->destroy_window32();
		}
		else
			message_box(HWND_DESKTOP, L"Error Creating OpenGL Window", L"Error...5", MB_OK | MB_ICONEXCLAMATION);
	}
//		this->DestroyWindow32();
//		UnregisterClass (this->m_ClassName, this->m_hInstance);
//		this->Shutdown();
#endif
	return 0;
}

bool Window32::initilize()
{
	this->m_mouse->initialize();
	this->m_keyboard->initialize();
	return true;
}

void Window32::reshape(int32_t a_width, int32_t a_height)
{
#ifdef _WIN32
	get_client_rect(this->m_h_wnd, &m_window_rectangle);        // Obsolete Line of Code

	this->m_window_height = a_height;
	this->m_window_width  = a_width;

	if (this->m_window_height == 0)
		this->m_window_height = 1;

	this->set_projection();
#endif
}

void Window32::toggle_fullscreen()
{
#ifdef _WIN32
	MESSAGEBOXINFORMATION("Sorry for Inconvenience, fullscreen functionality is broken :)");
	return;
	this->m_is_full_screen = !this->m_is_full_screen;
	post_message(this->m_h_wnd, WM_QUIT, 0, 0);
	//PostMessage (m_hWnd, WM_TOGGLEFULLSCREEN, 0, 0);
#endif
}

void Window32::update_window_key_handling(rUsChar a_key, bool a_value)
{
#ifdef _WIN32
	if (a_key == ror::KeyboardEvent::rKEY_ESCAPE)
		this->close();
	if (a_key == ror::KeyboardEvent::rKEY_F1)
		this->toggle_fullscreen();
	if (a_key == ror::KeyboardEvent::rKEY_SPACE)
	{
		//Handle the Space Key here
		//MessageBox(nullptr,L" ",L"",MB_OK | MB_ICONEXCLAMATION);
	}
#endif
}

LRESULT window_procedure(HWND a_h_wnd, uint32_t a_message, WPARAM a_w_param, LPARAM a_l_param)
{
#ifdef _WIN32
	// Get The Window Context
	static Window32 *           window32       = nullptr;
	static ror::IEventListener *event_listener = nullptr;

	window32       = nullptr;
	event_listener = nullptr;
	window32       = (Window32 *) get_window_long(a_h_wnd, GWL_USERDATA);

	if ((nullptr == window32) && (WM_CREATE != a_message))
		return def_window_proc(a_h_wnd, a_message, a_w_param, a_l_param);

	if (nullptr != window32)
		event_listener = window32->get_event_listener();

	// Handle the rest of the messages
	switch (a_message)
	{
		case WM_SYSCOMMAND:
		{
			switch (a_w_param)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
					/* Other Possible Options
							SC_CLOSE
							SC_CONTEXTHELP
							SC_DEFAULT
							SC_HOTKEY
							SC_HSCROLL
							SC_KEYMENU
							SC_MAXIMIZE
							SC_MINIMIZE
							SC_MONITORPOWER
							SC_MOUSEMENU
							SC_MOVE
							SC_NEXTWINDOW
							SC_PREVWINDOW
							SC_RESTORE
							SC_SIZE
							SC_TASKLIST
							SC_VSCROLL;
							*/
			}
			break;
		}
			return 0;
		case WM_COMMAND:
		{
			switch (LOWORD(a_w_param))
			{
				case ID_MAIN_EXIT:
					exit(0);
					break;
				case ID_EDIT_COPY1:
					MESSAGEBOX("Not Implemented Yet...");
					break;
			}
		}
		break;
		case WM_CREATE:
		{
			HINSTANCE h_inst = (HINSTANCE) get_window_long(a_h_wnd, GWL_HINSTANCE);
			window32         = (Window32 *) (((LPCREATESTRUCT) a_l_param)->lp_create_params);
			set_window_long(a_h_wnd, GWL_USERDATA, (LONG) window32);
			window32->m_h_wnd = a_h_wnd;
			/*
					HMENU Menu;
					Menu = LoadMenu(window32->m_hInstance, MAKEINTRESOURCE(IDR_MENU));
					SetMenu(window32->m_hWnd,Menu);
					*/
			window32->setup_opengl();
			return 0;
		}
		break;
		//case WM_DESTROY:
		//case WM_QUIT:
		case WM_CLOSE:
			event_listener->window_closing(ror::WindowEvent());
			return 0;
			break;
		case WM_SIZE:
			event_listener->window_size_changed(ror::WindowEvent(a_w_param, a_l_param, window32->get_window_width(), window32->get_window_height()));
			break;
		case WM_KEYDOWN:
			if ((a_w_param >= 0) && (a_w_param <= 255))
			{
				window32->key_down((rUsChar) a_w_param, true);
				event_listener->key_pressed(ror::KeyboardEvent((ror::KeyboardEvent::EKeyboardKeys)(rUsChar) a_w_param));
				return 0;
			}
			break;
		case WM_KEYUP:
			if ((a_w_param >= 0) && (a_w_param <= 255))
			{
				window32->key_down((rUsChar) a_w_param, false);
				event_listener->key_released(ror::KeyboardEvent((ror::KeyboardEvent::EKeyboardKeys)(rUsChar) a_w_param));
				return 0;
			}
			break;
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			break;
		case WM_LBUTTONDOWN:
			//LOG->MessageDebugWindow("MOUSE CLICKED \n");
			window32->left_button_down(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param));
			event_listener->mouse_pressed(ror::MouseEvent(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param), window32->get_window_height() - GET_Y_LPARAM(a_l_param), ror::MouseEvent::rLEFT_BUTTON));
			break;
		case WM_MBUTTONDOWN:
			window32->middle_button_down(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param));
			event_listener->mouse_pressed(ror::MouseEvent(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param), window32->get_window_height() - GET_Y_LPARAM(a_l_param), ror::MouseEvent::rMIDDLE_BUTTON));
			break;
		case WM_RBUTTONDOWN:
			window32->right_button_down(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param));
			event_listener->mouse_pressed(ror::MouseEvent(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param), window32->get_window_height() - GET_Y_LPARAM(a_l_param), ror::MouseEvent::rRIGHT_BUTTON));
			break;
		case WM_LBUTTONUP:
			window32->left_button_up(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param));
			event_listener->mouse_released(ror::MouseEvent(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param), window32->get_window_height() - GET_Y_LPARAM(a_l_param), ror::MouseEvent::rLEFT_BUTTON));
			break;
		case WM_MBUTTONUP:
			window32->middle_button_up(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param));
			event_listener->mouse_released(ror::MouseEvent(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param), window32->get_window_height() - GET_Y_LPARAM(a_l_param), ror::MouseEvent::rMIDDLE_BUTTON));
			break;
		case WM_RBUTTONUP:
			window32->right_button_up(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param));
			event_listener->mouse_released(ror::MouseEvent(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param), window32->get_window_height() - GET_Y_LPARAM(a_l_param), ror::MouseEvent::rRIGHT_BUTTON));
			break;
		case WM_MOUSEMOVE:
			//LOG->MessageDebugWindow("MOUSE MOVE \n");
			event_listener->mouse_moved(ror::MouseEvent(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param), window32->get_window_height() - GET_Y_LPARAM(a_l_param)));
			break;
		case WM_MOUSELEAVE:        // Don't know why is this one never called
			window32->mouse_leave();
			event_listener->mouse_leaved(ror::MouseEvent(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param), window32->get_window_height() - GET_Y_LPARAM(a_l_param)));
			break;
		case WM_NCMOUSELEAVE:        // This one is not much reliable, since it only works if you move very slow
			event_listener->mouse_leaved(ror::MouseEvent(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param), window32->get_window_height() - GET_Y_LPARAM(a_l_param)));
			break;
		case WM_MOUSEWHEEL:
			event_listener->mouse_scrolled(ror::MouseEvent(GET_X_LPARAM(a_l_param), GET_Y_LPARAM(a_l_param), window32->get_window_height() - GET_Y_LPARAM(a_l_param),
			                                               ror::MouseEvent::rNO_BUTTON,
			                                               (((rShort) HIWORD(a_w_param)) < 0 ? ror::MouseEvent::rDOWN_MOVEMENT : ror::MouseEvent::rUP_MOVEMENT)));
			break;
		case WM_TOGGLEFULLSCREEN:
			//window32->ChangeScreenResolution();
			//window32->CreateWindow32();
			//window32->ToggleFullscreen();
			break;
		case WM_ERASEBKGND:
			//return 0;
			break;
	}
	return def_window_proc(a_h_wnd, a_message, a_w_param, a_l_param);
#else
	return nullptr;
#endif
}
}        // namespace application
