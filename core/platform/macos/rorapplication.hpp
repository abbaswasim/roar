// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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

#include "event_system/rorevent_system.hpp"
#include "platform/rorapplication.hpp"
#include "platform/rorglfw_wrapper.hpp"

namespace ror
{

#define USE_GLFW

class ROAR_ENGINE_ITEM MacOSApp final : public Application<MacOSApp>
{
  public:
	FORCE_INLINE           MacOSApp()                            = delete;         //! Default constructor
	FORCE_INLINE           MacOSApp(const MacOSApp &a_other)     = default;         //! Copy constructor
	FORCE_INLINE           MacOSApp(MacOSApp &&a_other) noexcept = default;         //! Move constructor
	FORCE_INLINE MacOSApp &operator=(const MacOSApp &a_other) = delete;            //! Copy assignment operator
	FORCE_INLINE MacOSApp &operator=(MacOSApp &&a_other) noexcept = delete;        //! Move assignment operator
	FORCE_INLINE ~MacOSApp() noexcept override                    = default;        //! Destructor

	declare_translation_unit_vtable();

	FORCE_INLINE MacOSApp(EventSystem & a_event_system) :
		Application(a_event_system), m_glfw_window(this)
	{}

	FORCE_INLINE void init()
	{}

	FORCE_INLINE void loop()
	{
		this->m_glfw_window.loop();
	}

	FORCE_INLINE void update()
	{}

	FORCE_INLINE void animate()
	{}

	FORCE_INLINE void resize(int a_width, int a_height)
	{
		(void) a_width;
		(void) a_height;
	}

	FORCE_INLINE void shutdown()
	{}

	FORCE_INLINE void run()
	{
		this->init();
		this->loop();
		this->shutdown();
	}

  protected:
  private:
#if defined USE_GLFW
	GLFWwindowWrapper<MacOSApp> m_glfw_window;
#endif
};

define_translation_unit_vtable(MacOSApp)
{}

}        // namespace ror
