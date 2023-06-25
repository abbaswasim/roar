
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

#include "rhi/vulkan/rorshader.hpp"
#include "rhi/vulkan/rordevice.hpp"

namespace rhi
{

FORCE_INLINE ShaderVulkan::~ShaderVulkan() noexcept
{
	// if (this->m_main_function)
	// 	this->m_main_function->release();

	// if (this->m_msl_Library)
	// 	this->m_msl_Library->release();
}

// NOTE: When using this function, you still need to compile() and upload()
FORCE_INLINE auto load_shader(const std::filesystem::path &a_shader_path)
{
	auto type = rhi::string_to_shader_type(a_shader_path.extension());
	auto hash = ror::hash_64(a_shader_path.c_str(), a_shader_path.string().length());
	return ShaderVulkan{a_shader_path.string(), hash, type, ror::ResourceAction::load};
}

FORCE_INLINE auto build_shader(rhi::Device &a_device, const std::filesystem::path &a_shader_path)
{
	auto shader = load_shader(a_shader_path);

	shader.compile();
	shader.upload(a_device);

	return shader;
}
}        // namespace rhi
