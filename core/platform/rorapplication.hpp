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
#include "foundation/rormacros.hpp"
#include "graphics/rorscene.hpp"
#include "rhi/rorbuffers_format.hpp"
#include "rhi/rorbuffers_pack.hpp"
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
	FORCE_INLINE EventSystem &event_system()                             {     return this->m_event_system;                      }
	// clang-format on

	explicit FORCE_INLINE Application(EventSystem &a_event_system, Scene &a_scene) :
		m_event_system(a_event_system), m_scene(a_scene), m_buffer_pack(&rhi::get_buffers_pack())
	{}

	// This is called from underlying loop
	FORCE_INLINE void update()
	{
		(void) this->m_scene;
	}

	FORCE_INLINE void run()
	{
		this->init();
		this->loop();
		this->shutdown();
	}

  protected:
  private:
	FORCE_INLINE Application() = default;        //! Default constructor

	FORCE_INLINE void init()
	{
		// Should only be called once per execution, TODO: check if this could be used in MT environment
		basist::basisu_transcoder_init();
	}

	FORCE_INLINE void animate()
	{}

	FORCE_INLINE void render()
	{}

	FORCE_INLINE void shutdown()
	{
		this->m_buffer_pack->free();
	}

	EventSystem      &m_event_system;        //! Non-owning pointer to an event system that the application can use,
	Scene            &m_scene;               //! A reference to the scene we want to render
	rhi::BuffersPack *m_buffer_pack;         //! A non-owning alias reference to the global buffers pack
};

}        // namespace ror
