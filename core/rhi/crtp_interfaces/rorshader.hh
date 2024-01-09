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

#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rorshader.hpp"
#include "foundation/rorresolve_includes.hpp"

namespace rhi
{

template <class _type>
FORCE_INLINE ShaderCrtp<_type>::ShaderCrtp(const std::string &a_shader, hash_64_t a_hash, rhi::ShaderType a_type, ror::ResourceAction a_action) :
    m_hash(a_hash), m_type(a_type)
{
	if (a_action == ror::ResourceAction::create)
		this->m_shader = &ror::resource(a_shader, ror::ResourceSemantic::caches, ror::ResourceAction::create, "shaders");
	else
		this->m_shader = &ror::resource(a_shader, ror::ResourceSemantic::shaders, a_action);
}

template <class _type>
FORCE_INLINE void ShaderCrtp<_type>::source(const std::string &a_source)
{
	assert(this->m_shader);
	// This update is thread safe 1. because Resource is thread safe unless data() is called on it, and 2. because no data() can be called on shaders, only data_copy() is allowed and force is false as well
	this->m_shader->update({a_source.begin(), a_source.end()}, false, false, false);
}

template <class _type>
FORCE_INLINE void ShaderCrtp<_type>::compile()
{
	std::string info_log;

	// Doing a resolve_includes() very late here so I don't have to do it for small snipets multiple times
	std::string shader_source{this->source()};
	this->m_includes.clear();
	ror::resolve_includes(shader_source, ror::ResourceSemantic::shaders, false, this->m_includes);

	if (!compile_to_spirv(shader_source, this->m_type, "main", this->m_spirv, info_log))
	{
		ror::log_critical("Shader to SPIR-V conversion failed shader :{}\n \n{}", this->shader_path().c_str(), info_log.c_str());
	}

	this->platform_source();
}

template <class _type>
FORCE_INLINE void ShaderCrtp<_type>::reload()
{
	this->m_shader->load();
}

template <class _type>
FORCE_INLINE constexpr auto ShaderCrtp<_type>::source() const noexcept
{
	assert(this->m_shader);
	// Calling data_copy here, although its slow and creates a copy its thread safe compared to data(), which is not allowed on shaders
	return this->m_shader->data_copy();
}

template <class _type>
void ShaderCrtp<_type>::print_source() const noexcept
{
	auto &setting = ror::settings();

	bool vertex_glsl_print_code   = (setting.m_print_generated_vertex_shaders && setting.m_print_generated_glsl_shaders) || setting.m_print_generated_shaders;
	bool fragment_glsl_print_code = (setting.m_print_generated_fragment_shaders && setting.m_print_generated_glsl_shaders) || setting.m_print_generated_shaders;
	bool compute_glsl_print_code  = (setting.m_print_generated_compute_shaders && setting.m_print_generated_glsl_shaders) || setting.m_print_generated_shaders;

	if ((vertex_glsl_print_code && this->type() == rhi::ShaderType::vertex) ||
	    (fragment_glsl_print_code && this->type() == rhi::ShaderType::fragment) ||
	    (compute_glsl_print_code && this->type() == rhi::ShaderType::compute))
	{
		auto resource = this->source();
		ror::log_info("Generated GLSL, {} shader code.\n{}", shader_type_to_string(this->type()), this->source().c_str());
	}
}

template <class _type>
auto ShaderCrtp<_type>::generated_name() const noexcept
{
	static std::atomic<uint32_t> index = 0;

	std::string name{};
	name += std::to_string(this->hash());
	name += "_";
	name += std::to_string(index++);
	name += this->type() == rhi::ShaderType::vertex ? ".vert" : ".frag";

	return name;
}

template <class _type>
auto ShaderCrtp<_type>::write_source() const noexcept
{
	auto &setting = ror::settings();
	auto  name    = this->generated_name();
	if (setting.m_write_generated_shaders)
	{
		auto source = this->source();
		{
			auto &resource = ror::resource(name + ".glsl", ror::ResourceSemantic::caches, ror::ResourceAction::create, "generated_shaders");
			resource.update({source.begin(), source.end()}, false, false, true);
		}
	}

	return name;
}

// NOTE: When using this function, you still need to compile() and upload()
template <typename _type>
FORCE_INLINE _type load_shader(const std::filesystem::path &a_shader_path)
{
	auto type = rhi::string_to_shader_type(a_shader_path.extension());
	auto hash = ror::hash_64(a_shader_path.c_str(), a_shader_path.string().length());
	return _type{a_shader_path.string(), hash, type, ror::ResourceAction::load};
}

template <typename _type>
FORCE_INLINE _type build_shader(const rhi::Device &a_device, const std::filesystem::path &a_shader_path)
{
	auto shader = load_shader<_type>(a_shader_path);

	shader.compile();
	shader.upload(a_device);

	return shader;
}

}        // namespace rhi
