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

#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "resources/rorresource.hpp"
#include "rhi/metal/rordevice.hpp"
#include "rhi/metal/rorshader_buffer.hpp"
#include "rhi/rortypes.hpp"
#include <string>
#include <unordered_map>
#include <utility>

namespace rhi
{

static std::string format_to_glsl_type(rhi::VertexFormat a_format)
{
	// bool
	// int
	// uint
	// float
	// double

	// bvecn:
	// ivecn:
	// uvecn:
	// vecn:
	// dvecn:

	// mat3x3
	// mat3x4
	// mat4x4

	// clang-format off
	switch (a_format)
	{
		case rhi::VertexFormat::invalid:			return " ";
		case rhi::VertexFormat::struct_1:			return " ";
		case rhi::VertexFormat::bool32_1:			return "bool";
		case rhi::VertexFormat::bool32_2:			return "bvec2";
		case rhi::VertexFormat::bool32_3:			return "bvec3";
		case rhi::VertexFormat::bool32_4:			return "bvec4";
		case rhi::VertexFormat::int8_1:				return "int";
		case rhi::VertexFormat::int8_2:				return "ivec2";
		case rhi::VertexFormat::int8_3:				return "ivec3";
		case rhi::VertexFormat::int8_4:				return "ivec4";
		case rhi::VertexFormat::int16_1:			return "int";
		case rhi::VertexFormat::int16_2:			return "ivec2";
		case rhi::VertexFormat::int16_3:			return "ivec3";
		case rhi::VertexFormat::int16_4:			return "ivec4";
		case rhi::VertexFormat::int32_1:			return "int";
		case rhi::VertexFormat::int32_2:			return "ivec2";
		case rhi::VertexFormat::int32_3:			return "ivec3";
		case rhi::VertexFormat::int32_4:			return "ivec4";
		case rhi::VertexFormat::half16_1:			return "int";
		case rhi::VertexFormat::half16_2:			return "ivec2";
		case rhi::VertexFormat::half16_3:			return "ivec3";
		case rhi::VertexFormat::half16_4:			return "ivec4";

		case rhi::VertexFormat::float32_1:			return "float";
		case rhi::VertexFormat::float32_2:			return "vec2";
		case rhi::VertexFormat::float32_3:			return "vec3";
		case rhi::VertexFormat::float32_4:			return "vec4";
		case rhi::VertexFormat::float32_2x2:		return "mat2x2";
		case rhi::VertexFormat::float32_2x3:		return "mat2x3";
		case rhi::VertexFormat::float32_2x4:		return "mat2x4";
		case rhi::VertexFormat::float32_3x2:		return "mat3x2";
		case rhi::VertexFormat::float32_3x3:		return "mat3x3";
		case rhi::VertexFormat::float32_3x4:		return "mat3x4";
		case rhi::VertexFormat::float32_4x2:		return "mat4x2";
		case rhi::VertexFormat::float32_4x3:		return "mat4x3";
		case rhi::VertexFormat::float32_4x4:		return "mat4x4";
		case rhi::VertexFormat::float64_2x2:		return "dmat2x2";
		case rhi::VertexFormat::float64_2x3:		return "dmat2x3";
		case rhi::VertexFormat::float64_2x4:		return "dmat2x4";
		case rhi::VertexFormat::float64_3x2:		return "dmat3x2";
		case rhi::VertexFormat::float64_3x3:		return "dmat3x3";
		case rhi::VertexFormat::float64_3x4:		return "dmat3x4";
		case rhi::VertexFormat::float64_4x2:		return "dmat4x2";
		case rhi::VertexFormat::float64_4x3:		return "dmat4x3";
		case rhi::VertexFormat::float64_4x4:		return "dmat4x4";
		case rhi::VertexFormat::float64_1:			return "double";
		case rhi::VertexFormat::float64_2:			return "dvec2";
		case rhi::VertexFormat::float64_3:			return "dvec3";
		case rhi::VertexFormat::float64_4:			return "dvec4";

		case rhi::VertexFormat::uint8_1:			return "uint";
		case rhi::VertexFormat::uint8_2:			return "uvec2";
		case rhi::VertexFormat::uint8_3:			return "uvec3";
		case rhi::VertexFormat::uint8_4:			return "uvec4";
		case rhi::VertexFormat::uint16_1:			return "uint";
		case rhi::VertexFormat::uint16_2:			return "uvec2";
		case rhi::VertexFormat::uint16_3:			return "uvec3";
		case rhi::VertexFormat::uint16_4:			return "uvec4";
		case rhi::VertexFormat::uint32_1:			return "uint";
		case rhi::VertexFormat::uint32_2:			return "uvec2";
		case rhi::VertexFormat::uint32_3:			return "uvec3";
		case rhi::VertexFormat::uint32_4:			return "uvec4";
		case rhi::VertexFormat::uint64_1:			return "double";
		case rhi::VertexFormat::uint64_2:			return "dvec2";
		case rhi::VertexFormat::uint64_3:			return "dvec3";
		case rhi::VertexFormat::uint64_4:			return "dvec4";

		case rhi::VertexFormat::int8_1_norm:		return "int";
		case rhi::VertexFormat::int8_2_norm:		return "ivec2";
		case rhi::VertexFormat::int8_3_norm:		return "ivec3";
		case rhi::VertexFormat::int8_4_norm:		return "ivec4";
		case rhi::VertexFormat::int16_1_norm:		return "int";
		case rhi::VertexFormat::int16_2_norm:		return "ivec2";
		case rhi::VertexFormat::int16_3_norm:		return "ivec3";
		case rhi::VertexFormat::int16_4_norm:		return "ivec4";
		case rhi::VertexFormat::uint8_1_norm:		return "uint";
		case rhi::VertexFormat::uint8_2_norm:		return "uvec2";
		case rhi::VertexFormat::uint8_3_norm:		return "uvec3";
		case rhi::VertexFormat::uint8_4_norm:		return "uvec4";
		case rhi::VertexFormat::uint16_1_norm:		return "uint";
		case rhi::VertexFormat::uint16_2_norm:		return "uvec2";
		case rhi::VertexFormat::uint16_3_norm:		return "uvec3";
		case rhi::VertexFormat::uint16_4_norm:		return "uvec4";
		case rhi::VertexFormat::int1010102_norm:	return "int";
		case rhi::VertexFormat::uint1010102_norm:	return "uint";
		case rhi::VertexFormat::int101111_norm:		return "int";
		case rhi::VertexFormat::uint101111_norm:	return "uint";
		case rhi::VertexFormat::uint8_4_norm_bgra:	return "uint";
		case rhi::VertexFormat::uint8_custom:		return "uint";
		case rhi::VertexFormat::uint16_custom:		return "uint";
		case rhi::VertexFormat::uint32_custom:		return "uint";
		case rhi::VertexFormat::float32_custom:		return "float";
	}
	// clang-format on

	return "";
}
class ROAR_ENGINE_ITEM ShaderInput final
{
  public:
	FORCE_INLINE              ShaderInput()                               = delete;         //! Default constructor
	FORCE_INLINE              ShaderInput(const ShaderInput &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE              ShaderInput(ShaderInput &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShaderInput &operator=(const ShaderInput &a_other)       = delete;         //! Copy assignment operator
	FORCE_INLINE ShaderInput &operator=(ShaderInput &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE ~ShaderInput() noexcept                                  = default;        //! Destructor

	FORCE_INLINE ShaderInput(std::unordered_map<std::string, rhi::Format> &&a_variables)
	{
		for (auto &[variable_name, variable_format] : a_variables)
		{
			this->m_variables.emplace(std::make_pair<const std::string, VariableEntry>(std::move(variable_name), {variable_format, 0, 0}));
		}
	}

	FORCE_INLINE ShaderInput(std::pair<std::string, rhi::Format> &&a_variable)
	{
		this->push(std::move(a_variable));
	}

	void push(std::pair<std::string, rhi::Format> &&a_variable)
	{
		this->m_variables.emplace(std::make_pair<std::string, VariableEntry>(std::move(a_variable.first), {a_variable.second, 0, 0}));
	}

	void update_data(std::string a_variable, const uint8_t *a_value)
	{
		auto &variable = this->m_variables[a_variable];
		auto *data_ptr = this->m_data.data();
		auto  bytes    = format_to_bytes(variable.m_format);

		std::memcpy(data_ptr + variable.m_data_offset, a_value, bytes);
	}

	void update(std::string a_variable, const uint8_t *a_value)
	{
		auto &variable = this->m_variables[a_variable];
		auto *data_ptr = this->m_data.data();
		auto  bytes    = format_to_bytes(variable.m_format);

		auto mapping = this->m_shader_buffer.map();

		std::memcpy(data_ptr + variable.m_data_offset, a_value, bytes);
		std::memcpy(mapping + variable.m_buffer_offset, data_ptr + variable.m_data_offset, bytes);

		this->m_shader_buffer.unmap();
	}

	void update()
	{
		auto mapping = this->m_shader_buffer.map();

		for (auto &[variable_name, variable_data] : this->m_variables)
			std::memcpy(mapping + variable_data.m_buffer_offset, this->m_data.data() + variable_data.m_data_offset, format_to_bytes(variable_data.m_format));

		this->m_shader_buffer.unmap();
	}

	void upload(rhi::Device &a_device)
	{
		this->fill();
		this->offsets();
		this->m_shader_buffer.init(a_device, ror::static_cast_safe<uint32_t>(this->m_data.size()));
		this->update();
	}

	std::string to_string(std::string a_name) const
	{
		std::string output{"layout(std140, set = 0, binding = 2) uniform "};        // TODO: Abstract out the set and binding
		output.append(a_name);
		output.append("\n{");

		for (auto &[variable_name, variable_data] : this->m_variables)
		{
			output.append("\t");
			output.append(format_to_glsl_type(variable_data.m_format));
			output.append(" ");
			output.append(variable_name);
			output.append(";\n");
		}

		output.append("\n} in_");
		output.append(a_name);
		output.append(";\n");
		return output;
	}

	struct VariableEntry
	{
		rhi::Format m_format;                  //! Format of the variable type
		size_t      m_data_offset{0};          //! Offset of the data in the m_data variable
		size_t      m_buffer_offset{0};        //! Offset of the variable inside the buffer
	};

	using VariableType = std::unordered_map<std::string, VariableEntry>;

  protected:
  private:
	void fill()
	{
		auto &shader_buffer = this->m_shader_buffer.shader_buffer();

		auto size = 0u;
		for (auto &variable : this->m_variables)
		{
			shader_buffer.add_entry(variable.first, variable.second.m_format, 1);        // TODO: Add arrays support
			size += format_to_bytes(variable.second.m_format);

			variable.second.m_data_offset = size;
		}

		this->m_data.resize(size);
	}

	void offsets()
	{
		auto &shader_buffer = this->m_shader_buffer.shader_buffer();
		auto  entries       = shader_buffer.entries_structs();

		for (auto entry : entries)
			this->m_variables[entry->m_name].m_buffer_offset = entry->m_offset;
	}

	rhi::ShaderBuffer m_shader_buffer{};        //! The shader buffer this Shader Input uses to communicate with the server
	ror::bytes_vector m_data{};                 //! This is where all the data for the input buffer is
	VariableType      m_variables{};            //! All the variables in this input buffer
};

}        // namespace rhi
