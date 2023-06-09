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
#include "foundation/rorsystem.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "rhi/metal/rorshader.hpp"

#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include "spirv-cross/spirv_glsl.hpp"
#include <atomic>
#include <spirv-cross/spirv_msl.hpp>
#include <spirv.hpp>
#include <string>
#include <utility>
#include <vector>

#include <Metal/MTLLibrary.hpp>

namespace rhi
{
define_translation_unit_vtable(ShaderMetal)
{}

spv::ExecutionModel shader_type_to_execution_model(const rhi::ShaderType a_type)
{
	// clang-format off
	switch (a_type)
	{
		case rhi::ShaderType::mesh:               return spv::ExecutionModelMeshNV;
		case rhi::ShaderType::task:               return spv::ExecutionModelTaskNV;
		case rhi::ShaderType::vertex:             return spv::ExecutionModelVertex;
	    case rhi::ShaderType::compute:            return spv::ExecutionModelGLCompute;
		case rhi::ShaderType::fragment:           return spv::ExecutionModelFragment;
	    case rhi::ShaderType::ray_miss:           return spv::ExecutionModelMissKHR;
	    case rhi::ShaderType::ray_any_hit:        return spv::ExecutionModelAnyHitKHR;
	    case rhi::ShaderType::ray_closest_hit:    return spv::ExecutionModelClosestHitKHR;
	    case rhi::ShaderType::ray_intersection:   return spv::ExecutionModelIntersectionKHR;
	    case rhi::ShaderType::ray_generation:     return spv::ExecutionModelRayGenerationKHR;
		case rhi::ShaderType::none:
		case rhi::ShaderType::tile:
			assert(0 && "Can't convert this shader type to execution model");

	}
	// clang-format on
	assert(0 && "Implement more types");

	return spv::ExecutionModelVertex;
}

std::string shader_type_to_msl_entry(const rhi::ShaderType a_type)
{
	return shader_type_to_string(a_type) + "_main";
}

void ShaderMetal::platform_source()
{
	auto  spirv_binary  = this->spirv();
	auto &setting       = ror::settings();
	this->m_entry_point = shader_type_to_msl_entry(this->type());

	spirv_cross::CompilerMSL msl(std::move(spirv_binary));
	msl.rename_entry_point("main", this->m_entry_point, shader_type_to_execution_model(this->type()));

	spirv_cross::CompilerMSL::Options options;
	options.set_msl_version(setting.m_metal.version_major, setting.m_metal.version_minor);
	options.argument_buffers          = setting.m_metal.argument_buffers;
	options.enable_decoration_binding = setting.m_metal.decoration_bindings;
	msl.set_msl_options(options);

	this->m_msl_source = msl.compile();

	if constexpr (ror::get_build() == ror::BuildType::build_debug)
	{
#define print_command(message, source) ror::log_info("Generated {}, {} shader code.\n{}", message, shader_type_to_string(this->type()), source);

		bool vertex_glsl_print_code   = (setting.m_print_generated_vertex_shaders && setting.m_print_generated_glsl_shaders) || setting.m_print_generated_shaders;
		bool fragment_glsl_print_code = (setting.m_print_generated_fragment_shaders && setting.m_print_generated_glsl_shaders) || setting.m_print_generated_shaders;

		if ((vertex_glsl_print_code && this->type() == rhi::ShaderType::vertex) ||
		    (fragment_glsl_print_code && this->type() == rhi::ShaderType::fragment))
		{
			auto resource = this->source();
			print_command("GLSL", resource);
		}

		bool vertex_msl_print_code   = (setting.m_print_generated_vertex_shaders && setting.m_print_generated_msl_shaders) || setting.m_print_generated_shaders;
		bool fragment_msl_print_code = (setting.m_print_generated_fragment_shaders && setting.m_print_generated_msl_shaders) || setting.m_print_generated_shaders;

		if ((vertex_msl_print_code && this->type() == rhi::ShaderType::vertex) ||
		    (fragment_msl_print_code && this->type() == rhi::ShaderType::fragment))
		{
			print_command("Spirv-corss MSL", this->m_msl_source);
		}

#undef print_command

		if (setting.m_write_generated_shaders)
		{
			static std::atomic<uint32_t> index = 0;

			std::string     name{};
			name += std::to_string(this->hash());
			name += "_";
			name += std::to_string(index++);
			name += this->type() == rhi::ShaderType::vertex ? ".vert" : ".frag";

			auto source = this->source();
			{
				auto &resource = ror::resource(name + ".glsl", ror::ResourceSemantic::caches, ror::ResourceAction::create, "generated_shaders");
				resource.update({source.begin(), source.end()}, false, true);
			}
			{
				auto &resource = ror::resource(name + ".msl", ror::ResourceSemantic::caches, ror::ResourceAction::create, "generated_shaders");
				resource.update({this->m_msl_source.begin(), this->m_msl_source.end()}, false, true);
			}
		}
	}
}

void ShaderMetal::upload(rhi::Device &a_device)
{
	MTL::Device *device = a_device.platform_device();
	NS::Error   *pError = nullptr;

	this->m_msl_Library = device->newLibrary(NS::String::string(this->m_msl_source.c_str(), NS::UTF8StringEncoding), nullptr, &pError);

	if (!this->m_msl_Library)
	{
		ror::log_critical("Metal shader compile failed.\n {}", pError->localizedDescription()->utf8String());
		return;
	}

	if (this->m_main_function)
		this->m_main_function->release();

	this->m_main_function = this->m_msl_Library->newFunction(NS::String::string(this->m_entry_point.c_str(), NS::UTF8StringEncoding));

	if (!this->m_main_function)
	{
		ror::log_critical("Metal shader entry point \"{}\" query failed for shader \"{}\"", this->m_entry_point.c_str(), this->m_msl_source.c_str());
		this->m_main_function = nullptr;
		return;
	}
}

}        // namespace rhi
