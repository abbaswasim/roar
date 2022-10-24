// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2021
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

#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"

namespace rhi
{
FORCE_INLINE constexpr uint32_t make_rate_multiplier(uint32_t a_rate, uint32_t a_multiplier)
{
	assert(a_rate <= 65535 && "Rate doesn't fit in 16bit!");
	assert(a_multiplier <= 65535 && "Format multiplier doesn't fit in 16bit!");

	a_rate = a_rate | (a_multiplier << 16);

	return a_rate;
}
void VertexDescriptor::add(VertexAttribute a_attribute, VertexLayout a_layout, const rhi::BuffersPack *a_buffers_pack)
{
	tuple_type_vector attributes_tuple_vector{};

	auto rate = make_rate_multiplier(a_layout.rate(), a_layout.format_multiplier());

	attributes_tuple_vector.emplace_back(a_attribute.semantics(), a_attribute.format(), a_layout.step_function(), rate);
	this->create_attributes_and_layouts(attributes_tuple_vector, a_buffers_pack);
	this->create_mapping();
}

void VertexDescriptor::add(std::vector<VertexAttribute> a_attribute, std::vector<VertexLayout> a_layout, const rhi::BuffersPack *a_buffers_pack)
{
	assert(a_attribute.size() == a_layout.size() && "Attributes and layouts should be equal to use this version of add!");
	tuple_type_vector attributes_tuple_vector{};
	attributes_tuple_vector.reserve(a_attribute.size());

	for (size_t i = 0; i < a_attribute.size(); ++i)
	{
		auto rate = make_rate_multiplier(a_layout[i].rate(), a_layout[i].format_multiplier());

		attributes_tuple_vector.emplace_back(a_attribute[i].semantics(), a_attribute[i].format(), a_layout[i].step_function(), rate);
	}

	this->create_attributes_and_layouts(attributes_tuple_vector, a_buffers_pack);
	this->create_mapping();
}

void VertexDescriptor::upload(const std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>> &a_attrib_data, rhi::BuffersPack *a_buffers_pack)
{
	assert(a_attrib_data.size() == this->m_attributes.size() && "Uploading partial attributes not supported");
	std::unordered_map<size_t, ptrdiff_t> buffer_to_size_map{};

	// First calculate the total allocation size required for each buffer from the different attributes it can have
	for (const auto &attrib_data : a_attrib_data)
	{
		auto buffer_index = a_buffers_pack->attribute_buffer_index(attrib_data.first);

		auto [buffer_pointer, buffer_size, buffer_stride] = attrib_data.second;
		buffer_to_size_map[buffer_index] += buffer_size;
	}

	// Allocate space for all of the attributes in its specific buffer
	for (auto &attr_data : buffer_to_size_map)
	{
		auto &buffer        = a_buffers_pack->buffer(attr_data.first);
		auto  buffer_offset = buffer.offset(attr_data.second);        // Do allocation by calling Buffer::offset()
		attr_data.second    = buffer_offset;                          // This is the offset not the size anymore
	}

	// Upload the data into the allocated space
	for (auto &attrib_data : a_attrib_data)
	{
		auto &attribute    = this->attribute(attrib_data.first);
		auto &layout       = this->layout(attrib_data.first);
		auto  stride       = layout.stride();
		auto  buffer_index = attribute.buffer_index();
		auto  format_bytes = rhi::format_to_bytes(attribute.format()) * layout.format_multiplier();
		auto &buffer       = a_buffers_pack->buffer(buffer_index);
		auto &real_offset  = buffer_to_size_map[buffer_index];
		auto  attrib_offset{0L};

		auto [buffer_pointer, buffer_size, buffer_stride] = attrib_data.second;

		attribute.buffer_offset(real_offset);
		attrib_offset = real_offset + attribute.offset();

		if (!buffer.interleaved())
			real_offset += buffer_size;

		auto element_count = buffer_size / format_bytes;
		if (stride == format_bytes && buffer_stride == format_bytes)        // Case 1: both source and destination strides are equal to format_bytes, everything is packed
		{
			buffer.copy(buffer_pointer, buffer_size, attrib_offset);        // Do a bulk upload
		}
		else
		{
			// Case 2: Destination stride is packed but source stride isn't, destination packed source interleaved
			// Case 3: Destination stride is interleaved but source stride is packed, destination interleaved source packed
			// Case 4: Both destination and source stride means they are interleaved
			// Thats why lets do element by element upload
			for (size_t i = 0; i < element_count; ++i)
				buffer.copy(buffer_pointer + i * buffer_stride, format_bytes, attrib_offset + stride * static_cast<ptrdiff_t>(i));
		}

		attribute.count(element_count);
	}
}

void VertexDescriptor::create_mapping()
{
	this->m_mapping.clear();        // Perhaps could be converted into re-entrant function without this, for now this will do
	this->m_mapping.reserve(this->m_attributes.size());

	for (auto &attribute : this->m_attributes)
	{
		auto found = std::find_if(std::begin(this->m_layouts), std::end(this->m_layouts), [&](VertexLayout &vl) { return attribute.binding() == vl.binding(); });

		assert(found != this->m_layouts.end() && "No vertex layout available for this attribute!, invalid layout attributes combination provided");

		this->m_mapping.emplace(attribute.semantics(),
		                        std::make_pair(
		                            std::ref(attribute),
		                            std::ref(*found)));
	}
}

void VertexDescriptor::create_attributes_and_layouts(const tuple_type_vector &a_attributes, const rhi::BuffersPack *a_buffers_pack)
{
	uint32_t binding{0};        // Start with a lower binding and keep going up

	if (this->m_layouts.size())
		binding = this->m_layouts.back().binding() + 1;

	// Lets prime attributes and layouts before we start
	this->m_attributes.reserve(this->m_attributes.size() + a_attributes.size());
	this->m_layouts.reserve(this->m_layouts.size() + a_attributes.size());

	// Iterate over attributes to allocate their layouts
	for (auto &attribute : a_attributes)
	{
		auto semantic      = std::get<rhi::BufferSemantic>(attribute);
		auto format        = std::get<rhi::VertexFormat>(attribute);
		auto step_function = std::get<rhi::StepFunction>(attribute);
		auto rate          = std::get<uint32_t>(attribute);
		auto multiplier    = std::max(1U, rate >> 16);
		auto buffer_index  = a_buffers_pack->attribute_buffer_index(semantic);

		uint32_t stride{0};

		if (a_buffers_pack->buffer(buffer_index).interleaved())
		{
			for (auto &attribute0 : a_attributes)
			{
				auto semantic0     = std::get<rhi::BufferSemantic>(attribute0);
				auto buffer_index0 = a_buffers_pack->attribute_buffer_index(semantic0);
				auto format0       = std::get<rhi::VertexFormat>(attribute0);
				auto rate0         = std::get<uint32_t>(attribute);
				auto multiplier0   = std::max(1U, rate0 >> 16);
				auto format_bytes  = vertex_format_to_bytes(format0) * multiplier0;

				if (buffer_index == buffer_index0)
					stride += format_bytes;
			}

			for (auto &attribute0 : this->m_attributes)
			{
				auto semantic0     = attribute0.semantics();
				auto buffer_index0 = a_buffers_pack->attribute_buffer_index(semantic0);
				auto format0       = attribute0.format();
				auto multiplier0   = this->m_layouts[attribute0.binding()].format_multiplier();
				auto format_bytes  = vertex_format_to_bytes(format0) * multiplier0;

				if (buffer_index == buffer_index0)
					stride += format_bytes;
			}

			// Lets update all the existing layouts with new strides
			// This assumes the property that there is 1:1 mapping of attributes to layouts
			for (auto &attrib : this->m_attributes)        // Can't use mapping here because it contains iterators that are invalid after input into attributes or layouts
			{
				if (attrib.buffer_index() == buffer_index)
					this->m_layouts[attrib.binding()].stride(stride);
			}
		}
		else
			stride = vertex_format_to_bytes(format) * multiplier;

		this->m_layouts.emplace_back(binding++, stride, rate & 65535, multiplier, step_function);
	}

	uint32_t location{0U};        // Assuming we start with 'an' order, zero is a good start for location

	// Lets get end attributes location to use in this iteration
	if (this->m_attributes.size())
		location = this->m_attributes.back().location() + vertex_format_to_location(this->m_attributes.back().format());        // Location for next attribute

	// Lets create attributes now
	auto layout_index{this->m_attributes.size()};        // This needs to be last layout
	for (auto &attribute : a_attributes)
	{
		auto  semantic     = std::get<rhi::BufferSemantic>(attribute);
		auto  format       = std::get<rhi::VertexFormat>(attribute);
		auto  buffer_index = a_buffers_pack->attribute_buffer_index(semantic);
		auto  offset       = 0U;        // Will be updated to the stride from layout for this buffer later
		auto &layout       = this->m_layouts[layout_index++];

		if (a_buffers_pack->buffer(buffer_index).interleaved())
		{
			for (auto &attribute0 : this->m_attributes)
			{
				auto semantic0 = attribute0.semantics();

				if (semantic0 == semantic)
					break;

				auto buffer_index0 = a_buffers_pack->attribute_buffer_index(semantic0);
				if (buffer_index == buffer_index0)
				{
					auto format0      = attribute0.format();
					auto multiplier0  = this->m_layouts[attribute0.binding()].format_multiplier();
					auto format_bytes = vertex_format_to_bytes(format0) * multiplier0;
					offset += format_bytes;
				}
			}
		}

		this->m_attributes.emplace_back(location, offset, /* count */ 0u, /* buffer_offset */ 0ULL, layout.binding(), buffer_index, semantic, format);        // count and buffer_offset are set later, check ::upload()

		location += vertex_format_to_location(format);
	}
}
}        // namespace rhi
