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

#include "rhi/rorshader_buffer.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rortypes.hpp"
#include <cctype>
#include <string>

namespace rhi
{
constexpr uint32_t format_base_alignment_count(Format a_format)
{
	switch (a_format)
	{
		case Format::invalid:
			return 0;
		case Format::int8_1:
		case Format::int8_1_norm:
		case Format::uint8_1:
		case Format::uint8_1_norm:
		case Format::uint8_custom:
		case Format::int16_1:
		case Format::int16_1_norm:
		case Format::uint16_1:
		case Format::uint16_1_norm:
		case Format::half16_1:
		case Format::uint16_custom:
		case Format::bool32_1:
		case Format::int32_1:
		case Format::uint32_1:
		case Format::float32_1:
		case Format::int1010102_norm:
		case Format::uint1010102_norm:
		case Format::int101111_norm:
		case Format::uint101111_norm:
		case Format::uint32_custom:
		case Format::float32_custom:
			return 1;
		case Format::int8_2:
		case Format::int8_2_norm:
		case Format::uint8_2:
		case Format::uint8_2_norm:
		case Format::int16_2:
		case Format::int16_2_norm:
		case Format::uint16_2:
		case Format::uint16_2_norm:
		case Format::half16_2:
		case Format::bool32_2:
		case Format::int32_2:
		case Format::uint32_2:
		case Format::float32_2:
		case Format::uint64_1:
		case Format::float64_1:
		case Format::float32_2x2:
		case Format::float32_3x2:
		case Format::float32_4x2:
			return 2;
		case Format::struct_1:
		case Format::struct_0:
		case Format::int8_3:
		case Format::int8_3_norm:
		case Format::uint8_3:
		case Format::uint8_3_norm:
		case Format::int16_3:
		case Format::int16_3_norm:
		case Format::uint16_3:
		case Format::uint16_3_norm:
		case Format::half16_3:
		case Format::bool32_3:
		case Format::int32_3:
		case Format::uint32_3:
		case Format::float32_3:
		case Format::int8_4:
		case Format::int8_4_norm:
		case Format::uint8_4:
		case Format::uint8_4_norm:
		case Format::uint8_4_norm_bgra:
		case Format::int16_4:
		case Format::int16_4_norm:
		case Format::uint16_4:
		case Format::uint16_4_norm:
		case Format::half16_4:
		case Format::bool32_4:
		case Format::int32_4:
		case Format::uint32_4:
		case Format::float32_4:
		case Format::float32_2x3:
		case Format::float32_2x4:
		case Format::float32_3x3:
		case Format::float32_3x4:
		case Format::float32_4x3:
		case Format::float32_4x4:
		case Format::float64_2:
		case Format::uint64_2:
		case Format::float64_2x2:
		case Format::float64_3x2:
		case Format::float64_4x2:
			return 4;
		case Format::float64_2x3:
		case Format::float64_2x4:
		case Format::float64_3x3:
		case Format::float64_3x4:
		case Format::float64_4x3:
		case Format::float64_4x4:
		case Format::float64_3:
		case Format::uint64_3:
		case Format::float64_4:
		case Format::uint64_4:
			return 8;
	}

	return 0;
}

constexpr uint32_t format_base_alignment(Format a_format)        //, Layout a_layout = Layout::std140)
{
	auto alignment = format_base_alignment_count(a_format);

	alignment *= 4;

	return alignment;
}

constexpr bool is_matrix_type(Format a_type)
{
	if (a_type == Format::float32_2x2 ||
	    a_type == Format::float32_2x3 ||
	    a_type == Format::float32_2x4 ||
	    a_type == Format::float32_3x2 ||
	    a_type == Format::float32_3x3 ||
	    a_type == Format::float32_3x4 ||
	    a_type == Format::float32_4x2 ||
	    a_type == Format::float32_4x3 ||
	    a_type == Format::float32_4x4)
		return true;
	else
		return false;
}

// A bit of a hack but glsl aligns some matrix dimensions to vec4
// This fixes those from the C++ size of matrices
constexpr uint32_t glsl_size(Format a_type)
{
	auto size = rhi::vertex_format_to_bytes(a_type);

	if (a_type == Format::float32_2x3)        // 24 -> 32
		return size + 8;
	else if (a_type == Format::float32_3x3)        // 36 -> 48
		return size + 12;
	else if (a_type == Format::float32_4x3)        // 48 -> 64
		return size + 16;
	else if (a_type == Format::float64_2x3)        // 48 -> 64
		return size + 16;
	else if (a_type == Format::float64_3x3)        // 72 -> 96
		return size + 24;
	else if (a_type == Format::float64_4x3)        // 96 -> 128
		return size + 32;
	else
		return size;
}

// A bit of a hack but glsl aligns vec3 to vec4 in arrays
constexpr uint32_t glsl_size_vec3_correction(Format a_type, uint32_t a_size)
{
	if (a_type == Format::float32_3 ||
	    a_type == Format::int32_3 ||
	    a_type == Format::uint32_3 ||
	    a_type == Format::bool32_3)
		return a_size += 4;

	return a_size;
}

ShaderBufferTemplate::Struct::Struct(std::string a_name, uint32_t a_count) :
    Entry(a_name, Format::struct_1, a_count, 0, 0, 0)
{
	this->m_entries.reserve(10);
}

void ShaderBufferTemplate::Struct::add_entry(const std::string &a_name, Format a_type, Layout a_layout, uint32_t a_count)
{
	const uint32_t base_alignment = format_base_alignment(a_type);
	const uint32_t aligned_offset = ror::align(this->m_offset, (a_count != 1 && a_layout == Layout::std140 ? 16 : base_alignment));        // a_count != 1 means either 0, unbounded array or > 1 a sized array
	const uint32_t size           = glsl_size(a_type);
	const uint32_t size_in_array  = glsl_size_vec3_correction(a_type, size);
	const uint32_t stride         = (a_count != 1 ? (a_layout == Layout::std140 ? std::max(16u, size_in_array) : size_in_array) : size);

	this->m_entries.emplace_back(std::in_place_type<Entry>, a_name, a_type, a_count, stride, aligned_offset, size);

	// Now lets calculate next entry offset
	this->m_offset = aligned_offset + (stride * std::max(a_count, 1u));

	// Update struct alignment
	this->m_alignment = ror::align(std::max(this->m_alignment, size), base_alignment);

	// Next item after array alignment or matrix needs to be aligned to base_alignment, valid for std140 and std430
	if (a_count != 1 || (is_matrix_type(a_type) && a_layout == Layout::std140))
		this->m_offset = ror::align(this->m_offset, base_alignment);

	if (this->m_unit_size == 0)
		this->m_unit_size = stride;
	else
		this->m_unit_size = this->m_offset;

	// std::cout << "add_entry(): Unit size = " << m_unit_size << std::endl;
}

void ShaderBufferTemplate::Struct::add_struct(Struct a_struct)
{
	// Lets make sure our input index is aligned to the struct alignment
	const auto struct_offset = ror::align(this->m_offset, this->m_alignment);
	this->m_offset           = struct_offset;

	// Lets update the new struct offsets
	for (auto &e : a_struct.m_entries)
	{
		if (Entry *pentry = std::get_if<Entry>(&e))
			pentry->m_offset += this->m_offset;
	}

	const auto struct_stride = ror::align16(a_struct.m_offset);
	a_struct.m_stride        = struct_stride;

	this->m_offset    = ror::align16(this->m_offset + struct_stride * std::max(a_struct.m_count, 1u));
	a_struct.m_size   = this->m_offset;
	a_struct.m_offset = struct_offset;

	this->m_entries.emplace_back(a_struct);

	this->m_unit_size = ror::align16((struct_stride * std::max(a_struct.m_count, 1u)) + struct_offset);
	// std::cout << "add_struct(): Unit size = " << m_unit_size << std::endl;

	// this->m_unit_size =  a_struct.m_unit_size * std::max(a_struct.m_count, 1u);
	// this->m_unit_size = this->m_offset;
}

std::vector<const ShaderBufferTemplate::Entry *> ShaderBufferTemplate::entries()
{
	std::vector<const Entry *> es;
	es.reserve(this->m_toplevel.m_entries.size());

	for (auto &e : this->m_toplevel.m_entries)
	{
		if (const rhi::ShaderBufferTemplate::Entry *pentry = std::get_if<rhi::ShaderBufferTemplate::Entry>(&e))
			es.push_back(pentry);
		else
			assert(0 && "Requesting only entries from a non-simple shader buffer");
	}

	return es;
}

std::vector<const ShaderBufferTemplate::Entry *> ShaderBufferTemplate::entries_structs()
{
	std::vector<const Entry *> es;
	es.reserve(this->m_toplevel.m_entries.size());

	for (auto &e : this->m_toplevel.m_entries)
	{
		if (const rhi::ShaderBufferTemplate::Entry *pentry = std::get_if<rhi::ShaderBufferTemplate::Entry>(&e))
		{
			es.push_back(pentry);
		}
		else
		{
			auto &s = std::get<rhi::ShaderBufferTemplate::Struct>(e);

			for (auto &se : s.m_entries)
			{
				if (const rhi::ShaderBufferTemplate::Entry *spentry = std::get_if<rhi::ShaderBufferTemplate::Entry>(&se))
				{
					es.push_back(spentry);
				}
				else
				{
					assert(0 && "Don't support more than 1 level nested structs in shader buffer template");
				}
			}

			// Also return the struct itself as an entry, discards the struct bits but returns the entry bits
			es.push_back(reinterpret_cast<const Entry *>(&s));
		}
	}

	return es;
}

static void create_variable_entry(std::string &a_output, const std::string &a_type, const std::string &a_name, uint32_t a_count)
{
	a_output.append("\t");
	a_output.append(a_type);
	a_output.append(" ");
	a_output.append(a_name);
	if (a_count != 1)
	{
		a_output.append("[");
		a_output.append(a_count == 0 ? "" : std::to_string(a_count));
		a_output.append("]");
	}
	a_output.append(";\n");
}

std::string ShaderBufferTemplate::to_glsl_string() const
{
	std::string pre_output{};
	std::string output{"layout("};
	output.append(this->layout_string());
	output.append(", set = ");
	output.append(std::to_string(this->m_set));
	output.append(", binding = ");
	output.append(std::to_string(this->m_binding));
	output.append(") ");
	output.append(this->type_string());
	output.append(" ");
	output.append(this->m_toplevel.m_name);
	output.append("\n{\n");

	for (auto &e : this->m_toplevel.m_entries)
	{
		if (const rhi::ShaderBufferTemplate::Entry *pentry = std::get_if<rhi::ShaderBufferTemplate::Entry>(&e))
		{
			create_variable_entry(output, vertex_format_to_glsl_type(pentry->m_type), pentry->m_name, pentry->m_count);
		}
		else
		{
			auto &s           = std::get<rhi::ShaderBufferTemplate::Struct>(e);
			auto  struct_name = s.m_name;
			struct_name[0]    = static_cast<char>(std::toupper(static_cast<unsigned char>(struct_name[0])));

			create_variable_entry(output, struct_name, s.m_name, s.m_count);

			pre_output.append("struct ");
			pre_output.append(struct_name);
			pre_output.append("\n{\n");

			for (auto &se : s.m_entries)
			{
				if (const rhi::ShaderBufferTemplate::Entry *spentry = std::get_if<rhi::ShaderBufferTemplate::Entry>(&se))
				{
					create_variable_entry(pre_output, vertex_format_to_glsl_type(spentry->m_type), spentry->m_name, spentry->m_count);
				}
				else
				{
					assert(0 && "Don't support more than 1 level nested structs in shader buffer template");
				}
			}

			pre_output.append("};\n\n");
		}
	}

	output.append("} in_");
	output.append(this->m_toplevel.m_name);
	output.append(";\n");

	return pre_output == "" ? output : pre_output + output;
}

define_translation_unit_vtable(ShaderBufferTemplate::Entry)
{}

define_translation_unit_vtable(ShaderBufferTemplate::Struct)
{}

}        // namespace rhi
