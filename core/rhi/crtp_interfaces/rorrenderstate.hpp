
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
#include "rhi/rordevice.hpp"
#include "rhi/rortypes.hpp"

namespace rhi
{
template <class _type>
class RenderstateCrtp : public ror::Crtp<_type, RenderstateCrtp>
{
  public:
	FORCE_INLINE                  RenderstateCrtp(const RenderstateCrtp &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                  RenderstateCrtp(RenderstateCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderstateCrtp &operator=(const RenderstateCrtp &a_other)           = default;        //! Copy assignment operator
	FORCE_INLINE RenderstateCrtp &operator=(RenderstateCrtp &&a_other) noexcept       = default;        //! Move assignment operator
	FORCE_INLINE virtual ~RenderstateCrtp() noexcept override                         = default;        //! Destructor

	FORCE_INLINE RenderstateCrtp(rhi::DepthCompareFunction a_compare_function, bool a_depth_write) :
	    m_compare_function(a_compare_function), m_depth_write(a_depth_write)
	{}

	// clang-format off
	FORCE_INLINE constexpr auto compare_function()        const noexcept { return this->m_compare_function;    }
	FORCE_INLINE constexpr auto depth_write()             const noexcept { return this->m_depth_write;         }

	void upload(rhi::Device& a_device)        { this->underlying().upload(a_device); }
	// clang-format on

  protected:
	FORCE_INLINE RenderstateCrtp() = default;        //! Default constructor
  private:
	// Depth render state
	rhi::DepthCompareFunction m_compare_function{};
	bool                      m_depth_write{true};
};

template <typename _RenderstateDepth> // will get other types as we add more state
class Renderstate final
{
  public:
	FORCE_INLINE              Renderstate()                               = default;        //! Default constructor
	FORCE_INLINE              Renderstate(const Renderstate &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE              Renderstate(Renderstate &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE Renderstate &operator=(const Renderstate &a_other)       = delete;         //! Copy assignment operator
	FORCE_INLINE Renderstate &operator=(Renderstate &&a_other) noexcept   = delete;         //! Move assignment operator
	FORCE_INLINE ~Renderstate() noexcept                                  = default;        //! Destructor

	void upload(rhi::Device &a_device);

	// clang-format off
	FORCE_INLINE constexpr auto &depth_state()                 const noexcept { return this->m_depth_state;                 }
	FORCE_INLINE constexpr auto &depth_state_less_equal()      const noexcept { return this->m_depth_state_less_equal;      }
	FORCE_INLINE constexpr auto &depth_state_equal_no_write()  const noexcept { return this->m_depth_state_equal_no_write;  }
	FORCE_INLINE constexpr auto &depth_state_always_no_write() const noexcept { return this->m_depth_state_always_no_write; }
	FORCE_INLINE constexpr auto &depth_state_less_no_write()   const noexcept { return this->m_depth_state_less_no_write;   }
	// clang-format on

  protected:
  private:
	_RenderstateDepth m_depth_state{rhi::DepthCompareFunction::less, true};
	_RenderstateDepth m_depth_state_less_equal{rhi::DepthCompareFunction::less_equal, true};
	_RenderstateDepth m_depth_state_equal_no_write{rhi::DepthCompareFunction::equal, false};
	_RenderstateDepth m_depth_state_always_no_write{rhi::DepthCompareFunction::always, false};
	_RenderstateDepth m_depth_state_less_no_write{rhi::DepthCompareFunction::less, false};
};

}        // namespace rhi

#include "rhi/crtp_interfaces/rorrenderstate.hh"
