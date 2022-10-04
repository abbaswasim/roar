
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
#include "rhi/crtp_interfaces/rorrenderstate.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"

#include <Metal/MTLDepthStencil.hpp>

namespace rhi
{
class RenderstateMetal : public RenderstateCrtp<RenderstateMetal>
{
  public:
	FORCE_INLINE                   RenderstateMetal()                                    = default;        //! Default constructor
	FORCE_INLINE                   RenderstateMetal(const RenderstateMetal &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                   RenderstateMetal(RenderstateMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderstateMetal &operator=(const RenderstateMetal &a_other)            = default;        //! Copy assignment operator
	FORCE_INLINE RenderstateMetal &operator=(RenderstateMetal &&a_other) noexcept        = default;        //! Move assignment operator
	FORCE_INLINE virtual ~RenderstateMetal() noexcept override                           = default;        //! Destructor

	void upload(rhi::Device& a_device);

  protected:
  private:
	declare_translation_unit_vtable();

	MTL::DepthStencilState *m_depth_state{nullptr};
	MTL::DepthStencilState *m_depth_state_less_equal{nullptr};
	MTL::DepthStencilState *m_depth_state_equal_no_write{nullptr};
	MTL::DepthStencilState *m_depth_state_always_no_write{nullptr};
	MTL::DepthStencilState *m_depth_state_less_no_write{nullptr};
};

declare_rhi_render_type(Renderstate);

}        // namespace rhi

#include "rhi/metal/rorrenderstate.hh"
