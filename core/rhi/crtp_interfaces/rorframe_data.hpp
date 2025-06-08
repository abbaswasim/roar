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
#include "camera/rorcamera.hpp"
#include "core/foundation/rorcrtp.hpp"
#include "rhi/rordevice.hpp"

namespace rhi
{
template <class _type>
class FrameDataCrtp : public ror::Crtp<_type, FrameDataCrtp>
{
  public:
	FORCE_INLINE                FrameDataCrtp(const FrameDataCrtp &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                FrameDataCrtp(FrameDataCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE FrameDataCrtp &operator=(const FrameDataCrtp &a_other)         = default;        //! Copy assignment operator
	FORCE_INLINE FrameDataCrtp &operator=(FrameDataCrtp &&a_other) noexcept     = default;        //! Move assignment operator
	FORCE_INLINE virtual ~FrameDataCrtp() noexcept override                     = default;        //! Destructor

  protected:
	FORCE_INLINE FrameDataCrtp() = default;        //! Default constructor
  private:
	std::vector<ror::OrbitCamera *> m_num_views{};

	Swapchain m_swapchain{};        //! We have one in context, that should move here
};

}        // namespace rhi

#include "rhi/crtp_interfaces/rorframe_data.hh"
