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
#include "graphics/rorscene.hpp"
#include "math/rorvector2.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rordevice.hpp"
#include <any>

namespace rhi
{

template <class _type>
class ContextCrtp : public ror::Crtp<_type, ContextCrtp>
{
  public:
	FORCE_INLINE              ContextCrtp(const ContextCrtp &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              ContextCrtp(ContextCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ContextCrtp &operator=(const ContextCrtp &a_other)       = default;        //! Copy assignment operator
	FORCE_INLINE ContextCrtp &operator=(ContextCrtp &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE virtual ~ContextCrtp() noexcept override                 = default;        //! Destructor

	FORCE_INLINE void init(std::any a_window, ror::Vector2ui a_dimensions)
	{
		this->m_current_device->init(a_window);

		// Should only be called once per execution, TODO: check if this could be used in MT environment
		basist::basisu_transcoder_init();

		// Initialize glslang library via our wrapper, calling glslang directly means we need to link it with editor which creates all kinds of issues
		glslang_wrapper_initialize_process();

		this->m_renderer.dimensions(a_dimensions);
		this->m_renderer.upload(*this->m_current_device);

		// Load all the models now in a deferred way
		this->m_scene.load_models(*this->m_job_system, *this->m_current_device, this->m_renderer);
		this->m_scene.upload(this->m_renderer, *this->m_current_device, this->m_event_system);

		this->underlying().init_derived();
	}

	// clang-format off
	FORCE_INLINE void   post_init()        {    this->underlying().post_init_derived();    }
	FORCE_INLINE void   pre_tick()         {    this->underlying().pre_tick_derived();     }
	FORCE_INLINE void   tick()             {    this->underlying().tick_derived();         }
	FORCE_INLINE void   post_tick()        {    this->underlying().post_tick_derived();    }

	FORCE_INLINE auto  &job_system()       {    return this->m_job_system;                 }
	FORCE_INLINE auto  &event_system()     {    return this->m_event_system;               }
	FORCE_INLINE auto  &scene()            {    return this->m_scene;                      }
	FORCE_INLINE auto  &buffer_pack()      {    return *this->m_buffer_pack;               }
	FORCE_INLINE auto  &device()           {    return *this->m_current_device;            }
	FORCE_INLINE auto  &renderer()         {    return this->m_renderer;                   }
	// clang-format on

	FORCE_INLINE void shutdown()
	{
		// Shutdown glslang library via our wrapper, calling glslang directly means we need to link it with editor which creaates all kinds of issues
		glslang_wrapper_finalize_process();

		this->m_buffer_pack->free();
		this->m_job_system->stop();

		this->underlying().shutdown_derived();
	}

  protected:
	// FORCE_INLINE ContextCrtp() = default;
	FORCE_INLINE ContextCrtp() :
	    m_job_system(&ror::get_job_system()),
	    m_scene(ror::settings().m_default_scene),
	    m_buffer_pack(&rhi::get_buffers_pack())
	{
		this->m_current_device = std::make_shared<Device>();
		this->m_devices.emplace_back(this->m_current_device);
	}

  private:
	ror::JobSystem                      *m_job_system{nullptr};            //! Non-owning job system alias that will be used by the whole application
	ror::EventSystem                     m_event_system{};                 //! An event system that the application can use,
	ror::Scene                           m_scene{};                        //! A scene we want to render
	rhi::BuffersPack                    *m_buffer_pack{nullptr};           //! A non-owning alias to the global buffers pack
	std::vector<std::shared_ptr<Device>> m_devices{};                      //! List of devices in the system
	std::shared_ptr<Device>              m_current_device{nullptr};        //! Current device we are using, only one device possible at a time
	ror::Renderer                        m_renderer{};                     //! Renderer that will be used by the context to render stuff
};

}        // namespace rhi
