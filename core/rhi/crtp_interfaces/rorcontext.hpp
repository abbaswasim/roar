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

#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rorcrtp.hpp"
#include "foundation/rorjobsystem.hpp"
#include "graphics/rorscene.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector4.hpp"
#include "profiling/rortimer.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rordevice.hpp"
#include "settings/rorsettings.hpp"
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

	FORCE_INLINE void init(std::any a_platform_window, void *a_window, ror::Vector4f a_dimensions)
	{
		this->m_current_device->init(a_platform_window, a_window, this->m_event_system, ror::Vector2ui{static_cast<uint32_t>(a_dimensions.x), static_cast<uint32_t>(a_dimensions.y)});
		ror::settings().setup_generic_numbers(this->m_event_system);

		// Should only be called once per execution, TODO: check if this could be used in MT environment
		basist::basisu_transcoder_init();

		// Initialize glslang library via our wrapper, calling glslang directly means we need to link it with editor which creates all kinds of issues
		glslang_wrapper_initialize_process();

		this->m_renderer.dimensions(a_dimensions, *this->m_current_device);
		this->m_renderer.upload(*this->m_current_device, *this->m_buffer_pack);
		this->m_renderer.set_modifier_events(this->m_event_system);

		// Load all the models now in a deferred way
		this->m_scene.load_models(*this->m_job_system, *this->m_current_device, this->m_renderer, this->m_event_system, *this->m_buffer_pack);
		this->m_scene.upload(*this->m_job_system, this->m_renderer, *this->m_current_device, this->m_event_system);

		this->m_renderer.deferred_buffer_upload(*this->m_current_device, this->m_scene);

		this->underlying().init_derived();
	}

	FORCE_INLINE void tick()
	{
		auto &job_system   = this->job_system();
		auto &event_system = this->event_system();
		auto &scene        = this->scene();
		auto &cameras      = scene.cameras();
		auto &buffer_pack  = this->buffer_pack();
		auto &device       = this->device();
		auto &renderer     = this->renderer();
		auto &timer        = this->timer();

		ror::EventCallback camera_cycle_callback = [this, &cameras](ror::Event &) {
			size_t cameras_size{cameras.size()};

			cameras[this->m_current_camera_index].disable();

			this->m_current_camera_index++;
			this->m_current_camera_index = this->m_current_camera_index % ror::static_cast_safe<uint32_t>(cameras_size);

			cameras[this->m_current_camera_index].enable();
		};

		auto &camera = cameras[this->m_current_camera_index];

		static bool enable_camera = true;

		if (enable_camera)
		{
			auto dims = renderer.dimensions();
			auto bbox = scene.bounding_box();

			for (auto &cam : cameras)
			{
				cam.bounds(dims.x, dims.y);
				cam.volume(bbox.minimum(), bbox.maximum());
			}

			camera.enable();
			enable_camera = false;

			event_system.subscribe(ror::keyboard_c_click, camera_cycle_callback);
		}

		camera.update(renderer);

		renderer.render(scene, job_system, event_system, buffer_pack, device, timer);

		this->underlying().tick_derived();
	}

	// clang-format off
	FORCE_INLINE void   post_init()        {    this->underlying().post_init_derived();    }
	FORCE_INLINE void   pre_tick()         {    this->underlying().pre_tick_derived();     }
	FORCE_INLINE void   post_tick()        {    this->underlying().post_tick_derived();    }

	FORCE_INLINE constexpr auto  &job_system()     noexcept  {    return *this->m_job_system;                }
	FORCE_INLINE constexpr auto  &event_system()   noexcept  {    return this->m_event_system;               }
	FORCE_INLINE constexpr auto  &scene()          noexcept  {    return this->m_scene;                      }
	FORCE_INLINE constexpr auto  &buffer_pack()    noexcept  {    return *this->m_buffer_pack;               }
	FORCE_INLINE constexpr auto  &device()         noexcept  {    return *this->m_current_device;            }
	FORCE_INLINE constexpr auto  &renderer()       noexcept  {    return this->m_renderer;                   }
	FORCE_INLINE constexpr auto  &timer()          noexcept  {    return this->m_timer;                      }
	// clang-format on

	FORCE_INLINE void shutdown()
	{
		// Shutdown glslang library via our wrapper, calling glslang directly means we need to link it with editor which creaates all kinds of issues
		glslang_wrapper_finalize_process();

		this->m_scene.shutdown(this->m_event_system);

		this->m_buffer_pack->free();
		this->m_job_system->stop();

		this->underlying().shutdown_derived();
	}

  protected:
	// FORCE_INLINE ContextCrtp() = default;
	FORCE_INLINE ContextCrtp() :
	    m_job_system(&ror::get_job_system()),
	    m_scene(ror::settings().m_default_scene, m_event_system),
	    m_buffer_pack(&rhi::get_buffers_pack())
	{
		this->m_current_device = std::make_shared<Device>();
		this->m_devices.emplace_back(this->m_current_device);
	}

  private:
	ror::Timer                           m_timer{};                        //! Main timer for the system
	ror::JobSystem                      *m_job_system{nullptr};            //! Non-owning job system alias that will be used by the whole application
	ror::EventSystem                     m_event_system{};                 //! An event system that the application can use,
	ror::Scene                           m_scene{};                        //! A scene we want to render
	rhi::BuffersPack                    *m_buffer_pack{nullptr};           //! A non-owning alias to the global buffers pack
	std::vector<std::shared_ptr<Device>> m_devices{};                      //! List of devices in the system
	std::shared_ptr<Device>              m_current_device{nullptr};        //! Current device we are using, only one device possible at a time
	ror::Renderer                        m_renderer{};                     //! Renderer that will be used by the context to render stuff
	ror::Vector4ui                       m_frame_dimensions{};             //! current frame dimensions updated by looking up current window and framebuffer sizes
	uint32_t                             m_current_camera_index{0};        //! Camera to use to render the scene
};

}        // namespace rhi
