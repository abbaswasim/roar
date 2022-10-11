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

#include "rhi/rorvertex_description.hpp"

namespace rhi
{
void VertexDescriptor::add(VertexAttribute a_attribute, VertexLayout a_Layout, const rhi::BuffersPack *a_buffers_pack)
{
	tuple_type_vector attributes_tuple_vector{};

	auto rate       = a_Layout.rate();
	auto multiplier = a_Layout.format_multiplier();

	assert(rate <= 65535 && "Rate doesn't fit in 16bit!");
	assert(multiplier <= 65535 && "Format multiplier doesn't fit in 16bit!");

	rate = rate | (multiplier << 16);

	attributes_tuple_vector.emplace_back(a_attribute.semantics(), a_attribute.format(), a_Layout.step_function(), rate);
	this->create_attributes_and_layouts(attributes_tuple_vector, a_buffers_pack);
	this->create_mapping();
}

void VertexDescriptor::add(std::vector<VertexAttribute> a_attribute, std::vector<VertexLayout> a_Layout, const rhi::BuffersPack *a_buffers_pack)
{
	assert(a_attribute.size() == a_Layout.size() && "Attributes and layouts should be equal to use this version of add!");
	tuple_type_vector attributes_tuple_vector{};
	attributes_tuple_vector.reserve(a_attribute.size());

	for (size_t i = 0; i < a_attribute.size(); ++i)
	{
		auto rate       = a_Layout[i].rate();
		auto multiplier = a_Layout[i].format_multiplier();

		assert(rate <= 65535 && "Rate doesn't fit in 16bit!");
		assert(multiplier <= 65535 && "Format multiplier doesn't fit in 16bit!");

		rate = rate | (multiplier << 16);
		attributes_tuple_vector.emplace_back(a_attribute[i].semantics(), a_attribute[i].format(), a_Layout[i].step_function(), rate);
	}

	this->create_attributes_and_layouts(attributes_tuple_vector, a_buffers_pack);
	this->create_mapping();
}

void VertexDescriptor::upload(const std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>> &a_attrib_data, rhi::BuffersPack *a_buffers_pack)
{
	std::unordered_map<size_t, ptrdiff_t> buffer_to_size_map{};

	// First calculate the total allocation size required for each buffer from the different attributes it can have
	for (const auto &attrib_data : a_attrib_data)
	{
		auto buffer_index = a_buffers_pack->attribute_buffer_index(attrib_data.first);

		auto [buffer_pointer, buffer_size, buffer_stride] = attrib_data.second;
		buffer_to_size_map[buffer_index] += buffer_size;
	}

	// Allocate space for all of the attributes in its specific buffer
	for (const auto &attr_data : buffer_to_size_map)
	{
		auto &buffer        = a_buffers_pack->buffer(attr_data.first);
		auto  buffer_offset = buffer.offset(attr_data.second);        // Do allocation by calling Buffer::offset()

		// Now update every attribute's buffer_offset that is using this buffer and this allocation
		for (auto &attribute : this->m_attributes)
		{
			if (attribute.buffer_index() == attr_data.first)
				attribute.buffer_offset(buffer_offset);
		}
	}

	// Upload the data into the allocated space
	for (auto &attrib_data : a_attrib_data)
	{
		auto &attribute = this->attribute(attrib_data.first);
		auto &layout    = this->layout(attrib_data.first);

		auto  stride        = layout.stride();
		auto  buffer_index  = attribute.buffer_index();
		auto  format_bytes  = rhi::format_to_bytes(attribute.format());
		auto &buffer        = a_buffers_pack->buffer(buffer_index);
		auto  attrib_offset = attribute.buffer_offset() + attribute.offset();        // Will be 0 in case of single attribute or bulk upload;

		auto [buffer_pointer, buffer_size, buffer_stride] = attrib_data.second;

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
			auto element_count = buffer_size / format_bytes;
			attribute.count(element_count);
			for (size_t i = 0; i < element_count; ++i)
				buffer.copy(buffer_pointer + i * buffer_stride, format_bytes, attrib_offset + stride * static_cast<ptrdiff_t>(i));
		}
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
	size_t                                 existing_attributes_size{this->m_attributes.size()};
	uint32_t                               location{0};        // Assuming we start with 'an' order, zero is a good start for location
	std::unordered_map<uint32_t, uint32_t> strides{};          // Accumulated stride per buffer index
	uint32_t                               binding{0};         // Start with a lower binding and keep going up

	if (!this->m_attributes.empty())        // If this is not the first time we are calling this function
	{
		auto &last_attribute = this->m_attributes[existing_attributes_size - 1];
		auto  format         = last_attribute.format();        // Get the format for location increment

		// Lets get end attributes location and binding to use in this iteration
		location = last_attribute.location() + vertex_format_to_location(format);        // Location for next attribute
		binding  = last_attribute.binding() + 1;                                         // Increment by 1 for next binding

		// Lets warm up our strides map
		strides.reserve(this->m_mapping.size());
		for (auto &attrib_layout : this->m_mapping)
			strides.emplace(attrib_layout.second.first.buffer_index(), attrib_layout.second.second.stride());

		assert(this->m_layouts.size() == this->m_attributes.size() && "Layouts attributes should be equal");
	}

	this->m_attributes.reserve(this->m_attributes.size() + a_attributes.size());
	this->m_layouts.reserve(this->m_layouts.size() + a_attributes.size());

	// Iterate over the attributes, create internal attributes vector as well as strides for layouts
	for (auto &attribute : a_attributes)
	{
		auto semantic = std::get<rhi::BufferSemantic>(attribute);
		auto format   = std::get<rhi::VertexFormat>(attribute);
		auto rate     = std::get<uint32_t>(attribute);
		auto offset   = 0U;        // Could be updated to the stride from layout for this buffer later
		// auto location = rhi::semantic_to_index(semantic);

		uint32_t buffer_index = a_buffers_pack->attribute_buffer_index(semantic);
		// uint64_t buffer_offset   = bp.attribute_buffer_offset(semantic, 0ULL);        // How many bytes do we need for this, this has to be done later in a second pass, check ::upload()
		uint32_t vert_format_to_bytes = vertex_format_to_bytes(format);

		if (semantic == rhi::BufferSemantic::custom &&
		    (format == rhi::VertexFormat::uint8_custom || format == rhi::VertexFormat::uint16_custom || format == rhi::VertexFormat::float32_custom) &&
		    (rate >> 16))
			vert_format_to_bytes *= (rate >> 16);        // Use the format multiplier from upper 16bits of rate

		auto [buffer, success] = strides.insert({buffer_index, vert_format_to_bytes});

		if (!success)
		{
			// Check if we are locally or globally interleaved, in the later case don't need to update stride
			if (a_buffers_pack->buffer(buffer_index).interleaved())
			{
				offset = buffer->second;
				buffer->second += vert_format_to_bytes;
			}
		}

		this->m_attributes.emplace_back(location, offset, /* count */ 0u, /* buffer_offset */ 0ULL, binding, buffer_index, semantic, format);        // count and buffer_offset are set later, chech ::upload()

		location += vertex_format_to_location(format);
		binding += 1;
	}

	// Lets update all the existing layouts with new strides
	// This assumes the property that there is 1:1 mapping of attributes to layouts
	for (size_t i = 0; i < this->m_layouts.size(); ++i)
		this->m_layouts[i].stride(strides[this->m_attributes[i].buffer_index()]);

	// Iterate over attributes to allocate their layouts
	for (size_t i = 0; i < a_attributes.size(); ++i)
	{
		const auto &final_attribute = this->m_attributes[existing_attributes_size + i];
		const auto &raw_attribute   = a_attributes[i];
		const auto  format          = final_attribute.format();
		const auto  step_function   = std::get<rhi::StepFunction>(raw_attribute);
		const auto  rate            = std::get<uint32_t>(raw_attribute);
		const auto  multiplier      = std::max(1U, rate >> 16);
		const auto  buffer_index    = final_attribute.buffer_index();

		uint32_t stride = strides[buffer_index];

		if (!a_buffers_pack->buffer(buffer_index).interleaved())        // If global interleaved buffer then use the format bytes as stride, otherwise multiply it
			stride = vertex_format_to_bytes(format) * multiplier;

		this->m_layouts.emplace_back(final_attribute.binding(), stride, rate & 65535, multiplier, step_function);
	}
}

}        // namespace rhi
