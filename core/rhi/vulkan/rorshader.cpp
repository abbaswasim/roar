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

#include "foundation/rormacros.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/vulkan/rorshader.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"

namespace rhi
{
define_translation_unit_vtable(ShaderVulkan)
{}

void ShaderVulkan::platform_source()
{
	if constexpr (ror::get_build() == ror::BuildType::build_debug)
	{
		this->print_source();        // Prints main shader source which is GLSL in this case
		this->write_source();        // Writes main shader source which is GLSL in this case
	}
}

void ShaderVulkan::upload(const rhi::Device &a_device)
{
	VkDevice device = a_device.platform_device();

	if (this->m_module)
		vk_destroy_shader_module(device, this->m_module);        // TODO: Don't destroy if in use

	this->m_module = rhi::vk_create_shader_module(device, this->spirv());
}

}        // namespace rhi
