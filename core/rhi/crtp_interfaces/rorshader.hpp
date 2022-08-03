
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
#include "foundation/rormacros.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rortypes.hpp"
#include <cassert>
#include <vector>

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

	/**
	 * Use this ctor to create and load an existing shader from the file system
	 * or to create a shader without underlying file on the file system
	 * if a_create == true, This will create a file in project_root/shaders/cache/a_shader_name in memory
	 * which could be saved to disk if needed by calling write on the resource
	 * if a_create == false, This will load a file from project_root/[*]/a_shader_name from disk
	 */
	FORCE_INLINE ShaderCrtp(std::string a_shader, rhi::ShaderType a_type, ror::ResourceAction a_action);

	// clang-format off
	FORCE_INLINE constexpr auto shader_path() const noexcept { return this->m_shader->absolute_path(); }
	FORCE_INLINE constexpr void upload()      const noexcept { this->underlying().upload(); }
	// clang-format on

	/**
	 * Updates the underlying Resource data with a_source and also converts it to SPIRV
	 */
	FORCE_INLINE void source(const std::string &a_source);

  protected:
	FORCE_INLINE ShaderCrtp() = default;        //! Default constructor
  private:
	rhi::ShaderType       m_type{rhi::ShaderType::vertex};        //! Shader type could be vertex, fragment, compute etc
	ror::Resource        *m_shader{nullptr};                      //! non-owning pointer to a resource that has the shader source
	std::vector<uint32_t> m_spirv{};                              //! SPIR-V compiled version of the source code for this shader
};

bool compile_to_spirv(const std::string          &a_glsl_source,
                      rhi::ShaderType             a_shader_type,
                      const std::string          &a_entry_point,
                      std::vector<std::uint32_t> &spirv,
                      std::string                &info_log);

void glslang_wrapper_initialize_process();
void glslang_wrapper_finalize_process();
}        // namespace rhi

#include "rhi/crtp_interfaces/rorshader.hh"
