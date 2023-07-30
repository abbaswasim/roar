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
#include "rhi/rortypes.hpp"

#include <Metal/MTLDepthStencil.hpp>

namespace rhi
{
class RenderstateDepthMetal : public RenderstateCrtp<RenderstateDepthMetal>
{
  public:
	FORCE_INLINE                        RenderstateDepthMetal()                                         = default;        //! Default constructor
	FORCE_INLINE                        RenderstateDepthMetal(const RenderstateDepthMetal &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                        RenderstateDepthMetal(RenderstateDepthMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderstateDepthMetal &operator=(const RenderstateDepthMetal &a_other)                 = default;        //! Copy assignment operator
	FORCE_INLINE RenderstateDepthMetal &operator=(RenderstateDepthMetal &&a_other) noexcept             = default;        //! Move assignment operator
	FORCE_INLINE virtual ~RenderstateDepthMetal() noexcept override;                                                      //! Destructor

	FORCE_INLINE RenderstateDepthMetal(rhi::DepthCompareFunction a_compare_func, bool a_depth_write) :
	    RenderstateCrtp<RenderstateDepthMetal>(a_compare_func, a_depth_write)
	{}

	void upload(rhi::Device &a_device);

	// clang-format off
	FORCE_INLINE constexpr auto depth_state()             const noexcept { return this->m_depth_state;         }
	// clang-format on

  protected:
  private:
	declare_translation_unit_vtable();
	void release();

	MTL::DepthStencilState *m_depth_state{nullptr};
};

declare_rhi_render_type(RenderstateDepth);

}        // namespace rhi

#include "rhi/metal/rorrenderstate.hh"
