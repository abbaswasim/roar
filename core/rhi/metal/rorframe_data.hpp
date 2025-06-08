// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2025
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

#include "rhi/crtp_interfaces/rorframe_data.hpp"
#include "rhi/rorrhi_macros.hpp"

namespace rhi
{
class FrameDataMetal : public FrameDataCrtp<FrameDataMetal>
{
  public:
	FORCE_INLINE                 FrameDataMetal()                                  = default;        //! Default constructor
	FORCE_INLINE                 FrameDataMetal(const FrameDataMetal &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                 FrameDataMetal(FrameDataMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE FrameDataMetal &operator=(const FrameDataMetal &a_other)          = default;        //! Copy assignment operator
	FORCE_INLINE FrameDataMetal &operator=(FrameDataMetal &&a_other) noexcept      = default;        //! Move assignment operator
	FORCE_INLINE virtual ~FrameDataMetal() noexcept override                       = default;        //! Destructor

  protected:
  private:
	dispatch_semaphore_t m_acquire_semaphore{};
	dispatch_semaphore_t m_present_semaphore{};
};

declare_rhi_render_type(FrameData);

}        // namespace rhi

#include "rhi/metal/rorframe_data.hh"
