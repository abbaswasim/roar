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

	template<typename _type>
	void update_data(std::string a_variable, _type a_value)
	{
		this->update_data(std::move(a_variable), reinterpret_cast<const uint8_t *>(&a_value));
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

	auto &shader_buffer() { return this->m_shader_buffer;}

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
