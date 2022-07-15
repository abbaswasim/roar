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
#include "foundation/rorcrtp.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "graphics/rorscene.hpp"
#include "rhi/rorbuffers_format.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rorcontext.hpp"
#include "settings/rorsettings.hpp"
#include <memory>

class GLFWwindow;

namespace ror
{
template <class _type>
class ROAR_ENGINE_ITEM Application : public Crtp<_type, Application>
{
  public:
	FORCE_INLINE              Application(const Application &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              Application(Application &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE Application &operator=(const Application &a_other)       = default;        //! Copy assignment operator
	FORCE_INLINE Application &operator=(Application &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE virtual ~Application() noexcept override                 = default;        //! Destructor

	// clang-format off
	FORCE_INLINE void         loop()                                     {     this->underlying().loop();                        }
	FORCE_INLINE void         resize(int a_width, int a_height)          {     this->underlying().resize(a_width, a_height);     }
	FORCE_INLINE std::any     window()                                   {     return this->underlying().window();               }
	FORCE_INLINE void*        platform_window()                          {     return this->underlying().platform_window();      }
	FORCE_INLINE EventSystem &event_system()                             {     return this->m_context.event_system();            }
	// clang-format on

	// This is called from underlying loop
	FORCE_INLINE void update()
	{
		this->m_context.pre_tick();
		this->m_context.tick();
		this->m_context.post_tick();
	}

	FORCE_INLINE void run()
	{
		this->init();
		this->loop();
		this->shutdown();
	}

  protected:
	// This is protected so that no one can create pointers to base Application
	FORCE_INLINE Application() = default;        //! Default constructor

  private:
	FORCE_INLINE void init()
	{
		this->m_context.init(this->platform_window());
		this->m_context.post_init();
	}

	FORCE_INLINE void shutdown()
	{
		this->m_context.shutdown();
	}

	rhi::Context m_context;
};

}        // namespace ror
