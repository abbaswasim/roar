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

#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rorcontext.hpp"
#include "rhi/rorrhi_macros.hpp"

namespace rhi
{
class ContextMetal : public ContextCrtp<ContextMetal>
{
  public:
	FORCE_INLINE               ContextMetal()                                = default;        //! Default constructor
	FORCE_INLINE               ContextMetal(const ContextMetal &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE               ContextMetal(ContextMetal &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE ContextMetal &operator=(const ContextMetal &a_other)        = delete;         //! Copy assignment operator
	FORCE_INLINE ContextMetal &operator=(ContextMetal &&a_other) noexcept    = delete;         //! Move assignment operator
	FORCE_INLINE virtual ~ContextMetal() noexcept override                   = default;        //! Destructor

	declare_translation_unit_vtable();

	// clang-format off
	FORCE_INLINE void     init_derived()            {}
	FORCE_INLINE void     post_init_derived()       {}
	FORCE_INLINE void     pre_tick_derived()        {}
	FORCE_INLINE void     post_tick_derived()       {}
	FORCE_INLINE void     shutdown_derived()        {}
	// clang-format on

	FORCE_INLINE void tick_derived()
	{
		auto &job_system   = this->job_system();
		auto &event_system = this->event_system();
		auto &scene        = this->scene();
		auto &buffer_pack  = this->buffer_pack();
		auto &device       = this->device();
		auto &renderer     = this->renderer();

		(void) job_system;
		(void) event_system;
		(void) scene;
		(void) buffer_pack;
		(void) device;
		(void) renderer;

		// Setup uniforms
		/*
		Uniforms *uniform = reinterpret_cast<Uniforms *>(mvp->contents());

		// Update uniforms here

		memcpy(uniform->joints_matrices[0].m_values, skinning_matrices[0].m_values, 44 * sizeof(float) * 16);

		// Only for ResourceStorageModeManaged resources
		mvp->didModifyRange(NS::Range::Make(0, mvp->length()));
		*/
	}

  protected:
  private:
};

define_translation_unit_vtable(ContextMetal)
{}

declare_rhi_render_type(Context);

}        // namespace rhi
