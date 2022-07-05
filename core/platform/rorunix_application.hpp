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

/**
 * An implementation of Application
 * By default it uses GLFW on MacOs, Linux and Windows
 */
class ROAR_ENGINE_ITEM UnixApp final : public Application<UnixApp>
{
  public:
	// FORCE_INLINE          UnixApp()                             = delete;         //! Default constructor
	FORCE_INLINE          UnixApp(const UnixApp &a_other)       = default;        //! Copy constructor
	FORCE_INLINE          UnixApp(UnixApp &&a_other) noexcept   = default;        //! Move constructor
	FORCE_INLINE UnixApp &operator=(const UnixApp &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE UnixApp &operator=(UnixApp &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~UnixApp() noexcept override                   = default;        //! Destructor

	declare_translation_unit_vtable();

	FORCE_INLINE UnixApp() :
	    m_glfw_window(this)
	{}

	FORCE_INLINE void loop()
	{
#if defined USE_GLFW
		this->m_glfw_window.loop();
#endif
	}

#if defined USE_GLFW
	FORCE_INLINE GLFWwindow *window()
	{
		return this->m_glfw_window.window();
	}
#endif

  protected:
  private:
#if defined USE_GLFW
	GLFWwindowWrapper<UnixApp> m_glfw_window;        //! A GLFW wrapper that takes care of GLFW platform specific stuff
#endif
};

define_translation_unit_vtable(UnixApp)
{}

}        // namespace ror
