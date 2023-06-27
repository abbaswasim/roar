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
#include "core/foundation/rorcrtp.hpp"
#include "event_system/rorevent_system.hpp"
#include "rhi/rortypes.hpp"
#include <any>

namespace rhi
{
template <class _type>
class DeviceCrtp : public ror::Crtp<_type, DeviceCrtp>
{
  public:
	FORCE_INLINE             DeviceCrtp(const DeviceCrtp &a_other)     = default;        //! Copy constructor
	FORCE_INLINE             DeviceCrtp(DeviceCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE DeviceCrtp &operator=(const DeviceCrtp &a_other)      = default;        //! Copy assignment operator
	FORCE_INLINE DeviceCrtp &operator=(DeviceCrtp &&a_other) noexcept  = default;        //! Move assignment operator
	FORCE_INLINE virtual ~DeviceCrtp() noexcept override               = default;        //! Destructor

	// clang-format off
	FORCE_INLINE void  init(std::any a_platform_window, void* a_window, ror::EventSystem &a_event_system)  { this->underlying().init(a_platform_window, a_window, a_event_system); }
	// clang-format on

  protected:
	FORCE_INLINE DeviceCrtp() = default;        //! Default constructor
  private:
};

}        // namespace rhi

#include "rhi/crtp_interfaces/rordevice.hh"
