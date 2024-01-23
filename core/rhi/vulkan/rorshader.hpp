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
#include "rhi/crtp_interfaces/rorshader.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"

namespace rhi
{
class ShaderVulkan : public ShaderCrtp<ShaderVulkan>
{
  public:
	FORCE_INLINE               ShaderVulkan(const ShaderVulkan &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               ShaderVulkan(ShaderVulkan &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShaderVulkan &operator=(const ShaderVulkan &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE ShaderVulkan &operator=(ShaderVulkan &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE virtual ~ShaderVulkan() noexcept override                   = default;        //! Destructor

	FORCE_INLINE ShaderVulkan(const std::string &a_shader, hash_64_t a_hash, rhi::ShaderType a_type, ror::ResourceAction a_action) :
	    ShaderCrtp(a_shader, a_hash, a_type, a_action)
	{}

	// clang-format off
	FORCE_INLINE constexpr VkShaderModule module()   const noexcept   { return this->m_module;     }
	// clang-format on

	void platform_source();
	void upload(const rhi::Device &a_device);

  protected:
	FORCE_INLINE ShaderVulkan() = default;        //! Default constructor
  private:
	declare_translation_unit_vtable();

	VkShaderModule m_module{nullptr};        //! Vulkan Shader module for this shader
};

declare_rhi_render_type(Shader);

}        // namespace rhi

#include "rhi/vulkan/rorshader.hh"
