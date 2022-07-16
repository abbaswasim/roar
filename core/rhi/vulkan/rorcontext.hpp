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
#include "rhi/crtp_interfaces/rorcontext.hpp"
#include "rhi/rorrhi_macros.hpp"

namespace rhi
{

class ContextVulkan : public ContextCrtp<ContextVulkan>
{
  public:
	FORCE_INLINE                ContextVulkan()                                 = default;         //! Default constructor
	FORCE_INLINE                ContextVulkan(const ContextVulkan &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                ContextVulkan(ContextVulkan &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ContextVulkan &operator=(const ContextVulkan &a_other)         = default;        //! Copy assignment operator
	FORCE_INLINE ContextVulkan &operator=(ContextVulkan &&a_other) noexcept     = default;        //! Move assignment operator
	FORCE_INLINE virtual ~ContextVulkan() noexcept override                     = default;        //! Destructor

	declare_translation_unit_vtable();

	// clang-format off
	FORCE_INLINE void         init_derived()                                     {}
	FORCE_INLINE void         post_init_derived()                                {}
	FORCE_INLINE void         pre_tick_derived()                                 {}
	FORCE_INLINE void         tick_derived()                                     {}
	FORCE_INLINE void         post_tick_derived()                                {}
	FORCE_INLINE void         shutdown_derived()                                 {}
	// clang-format on
  protected:
	// FORCE_INLINE ContextVulkan(std::any *a_window)
	// {
	// this->m_current_instance = std::make_shared<Instance>();
	// this->m_current_gpu      = std::make_shared<Device>(this->m_current_instance->get_handle(), a_window);

	// this->m_instances.emplace_back(this->m_current_instance);
	// this->m_gpus.emplace_back(this->m_current_gpu);
	// }

  private:
	// std::vector<std::shared_ptr<Instance>> m_instances;
	// std::vector<std::shared_ptr<Device>>   m_gpus;
	// std::shared_ptr<Device>                m_current_gpu{nullptr};
	// std::shared_ptr<Instance>              m_current_instance{nullptr};
};

define_translation_unit_vtable(ContextVulkan)
{}

declare_rhi_render_type(Context);

}        // namespace rhi
