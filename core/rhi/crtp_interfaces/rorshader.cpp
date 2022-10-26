
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

#include "rhi/crtp_interfaces/rorshader.hpp"
#include "rhi/rortypes.hpp"
#include <SPIRV/GlslangToSpv.h>
#include <SPIRV/Logger.h>
#include <StandAlone/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>

namespace rhi
{

void glslang_wrapper_initialize_process()
{
	glslang::InitializeProcess();
}

void glslang_wrapper_finalize_process()
{
	glslang::FinalizeProcess();
}

EShLanguage shader_type_to_language(rhi::ShaderType a_type)
{
	switch (a_type)
	{
		// clang-format off
        case rhi::ShaderType::mesh:             return EShLangMeshNV;
    	case rhi::ShaderType::task:             return EShLangTaskNV;
	    case rhi::ShaderType::vertex:           return EShLangVertex;
	    case rhi::ShaderType::compute:          return EShLangCompute;
	    case rhi::ShaderType::fragment:         return EShLangFragment;
	    case rhi::ShaderType::ray_miss:         return EShLangMiss;
	    case rhi::ShaderType::ray_any_hit:      return EShLangAnyHit;
	    case rhi::ShaderType::ray_closest_hit:  return EShLangClosestHit;
	    case rhi::ShaderType::ray_intersection: return EShLangIntersect;
	    case rhi::ShaderType::ray_generation:   return EShLangRayGen;
	    case rhi::ShaderType::none:
	    case rhi::ShaderType::tile: assert(0);  return EShLangCallable;
			// clang-format on
	}

	assert(0);
	return EShLangCallable;
}

// Based on https://github.com/KhronosGroup/Vulkan-Samples/blob/master/framework/glsl_compiler.cpp#L93
bool compile_to_spirv(const std::string          &a_glsl_source,
                      rhi::ShaderType             a_shader_type,
                      const std::string          &a_entry_point,
                      std::vector<std::uint32_t> &spirv,
                      std::string                &info_log)
{
	EShMessages messages = static_cast<EShMessages>(EShMsgDefault | EShMsgVulkanRules | EShMsgSpvRules);        // | EShMsgDebugInfo);

	EShLanguage                       language        = shader_type_to_language(a_shader_type);
	glslang::EShTargetLanguage        target_language = glslang::EShTargetLanguage::EShTargetSpv;
	glslang::EShTargetLanguageVersion version         = glslang::EShTargetSpv_1_5;        // Could be 1_6

	const char *shader_source = reinterpret_cast<const char *>(a_glsl_source.data());

	glslang::TShader shader(language);
	shader.setStrings(&shader_source, 1);
	shader.setEntryPoint(a_entry_point.c_str());
	shader.setSourceEntryPoint(a_entry_point.c_str());
	shader.setEnvTarget(target_language, version);

	// NOTE: If required in the future for creating shader variants
	// shader.setPreamble();         // Should be something like #define skinning
	// shader.addProcesses();        // Should be something like -DSkinning=ON or -USkinning=Off, not sure

	// TODO: Find out what does this 100 or 110 mean here some people use it like ((EOptionNone & EOptionDefaultDesktop) ? 110 : 100, false)
	if (!shader.parse(&glslang::DefaultTBuiltInResource, 110, false, messages))
	{
		info_log += a_glsl_source;
		info_log += std::string(shader.getInfoLog()) + "\n" + std::string(shader.getInfoDebugLog());
		return false;
	}

	// // Add shader to new program object.
	glslang::TProgram program;
	program.addShader(&shader);

	// // Link program.
	if (!program.link(messages))
	{
		info_log = std::string(program.getInfoLog()) + "\n" + std::string(program.getInfoDebugLog());
		return false;
	}

	// // Save any info log that was generated.
	if (shader.getInfoLog())
	{
		info_log += std::string(shader.getInfoLog()) + "\n" + std::string(shader.getInfoDebugLog()) + "\n";
	}

	if (program.getInfoLog())
	{
		info_log += std::string(program.getInfoLog()) + "\n" + std::string(program.getInfoDebugLog());
	}

	glslang::TIntermediate *intermediate = program.getIntermediate(language);

	// // Translate to SPIRV.
	if (!intermediate)
	{
		info_log += "Creating glslang::TIntermediate failed.\n";
		return false;
	}

	spv::SpvBuildLogger logger;

	glslang::GlslangToSpv(*intermediate, spirv, &logger);

	info_log += logger.getAllMessages() + "\n";

	return true;
}
}        // namespace rhi
