
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
#include <filesystem>

namespace rhi
{
template <class _type>
class ShaderCrtp : public ror::Crtp<_type, ShaderCrtp>
{
  public:
	FORCE_INLINE             ShaderCrtp(const ShaderCrtp &a_other)     = default;        //! Copy constructor
	FORCE_INLINE             ShaderCrtp(ShaderCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShaderCrtp &operator=(const ShaderCrtp &a_other)      = default;        //! Copy assignment operator
	FORCE_INLINE ShaderCrtp &operator=(ShaderCrtp &&a_other) noexcept  = default;        //! Move assignment operator
	FORCE_INLINE virtual ~ShaderCrtp() noexcept override               = default;        //! Destructor

	FORCE_INLINE explicit ShaderCrtp(std::filesystem::path a_shader) :
	    m_shader_path(a_shader)
	{}

  protected:
	FORCE_INLINE ShaderCrtp() = default;        //! Default constructor
  private:
	std::filesystem::path m_shader_path{};
};

}        // namespace rhi

#include "rhi/crtp_interfaces/rorshader.hh"
