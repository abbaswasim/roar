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
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "rhi/crtp_interfaces/rorshader.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"
#include <Metal/MTLLibrary.hpp>
#include <string_view>

namespace rhi
{
class ShaderMetal : public ShaderCrtp<ShaderMetal>
{
  public:
	FORCE_INLINE              ShaderMetal(const ShaderMetal &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              ShaderMetal(ShaderMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShaderMetal &operator=(const ShaderMetal &a_other)       = default;        //! Copy assignment operator
	FORCE_INLINE ShaderMetal &operator=(ShaderMetal &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE virtual ~ShaderMetal() noexcept override;                                  //! Destructor

	FORCE_INLINE ShaderMetal(const std::string &a_shader, hash_64_t a_hash, rhi::ShaderType a_type, ror::ResourceAction a_action) :
	    ShaderCrtp(a_shader, a_hash, a_type, a_action)
	{}

	void platform_source();
	void upload(const rhi::Device &a_device);

	// clang-format off
	FORCE_INLINE constexpr auto      *function()   const  noexcept   {      return this->m_main_function;     }
	FORCE_INLINE std::string_view     msl_source() const             {      return this->m_msl_source;        }
	// clang-format on

  protected:
	FORCE_INLINE ShaderMetal() = default;        //! Default constructor
  private:
	declare_translation_unit_vtable();

	std::string    m_msl_source{};                      //! Copy of the msl source code
	MTL::Library  *m_msl_Library{nullptr};              //! Point to the metal shader library
	MTL::Function *m_main_function{nullptr};            //! Pointer to the entry point, it should be vertex_main for vertex and fragment_main for fragment shaders
	std::string    m_entry_point{"vertex_main"};        //! Shader main entry point, will be different based on the type of shader
};

declare_rhi_render_type(Shader);

}        // namespace rhi

#include "rhi/metal/rorshader.hh"
