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

#pragma once

#include "foundation/rorcompiler_workarounds.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rorutilities.hpp"
#include "rhi/rorvertex_attribute.hpp"
#include "rhi/rorvertex_layout.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rortypes.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace rhi
{
struct Rate
{
	Rate()
	{}
	Rate(uint32_t a_value) :
		m_value(a_value)
	{}
	uint32_t m_value{};
};

using tuple_type     = std::tuple<rhi::BufferSemantic, rhi::VertexFormat, rhi::StepFunction, uint32_t>;
using attrib_variant = std::variant<const rhi::BuffersPack *, rhi::BufferSemantic, rhi::VertexFormat, rhi::StepFunction, rhi::Rate>;
using attrib_vector  = std::vector<attrib_variant>;

/**
 * Can be used to describe a vertex attributes and layouts for a mesh.
 * It uses get_buffers_pack() to access the default buffers allocated via "buffers_format.json"
 * Currently there is no way to provide a different set of buffers or use a different config file
 */

class ROAR_ENGINE_ITEM VertexDescriptor final
{
  public:
	FORCE_INLINE                   VertexDescriptor()                                    = default;        //! Default constructor
	FORCE_INLINE                   VertexDescriptor(const VertexDescriptor &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                   VertexDescriptor(VertexDescriptor &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE VertexDescriptor &operator=(const VertexDescriptor &a_other) = default;                   //! Copy assignment operator
	FORCE_INLINE VertexDescriptor &operator=(VertexDescriptor &&a_other) noexcept = default;               //! Move assignment operator
	FORCE_INLINE ~VertexDescriptor() noexcept                                     = default;               //! Destructor

	/**
	 * Use this constructor to create mapping of attributes to layouts that you think works.
	 * If you are not sure what layout do I need and what is the buffers format use the single arguemnt ctor instead
	 * Its usage might be risky and and slightly complicated, forcing you to work out layouts and alignments manually
	 */
	FORCE_INLINE VertexDescriptor(std::vector<VertexAttribute> a_attributes, std::vector<VertexLayout> a_layouts) :
		m_attributes(a_attributes),
		m_layouts(a_layouts)
	{
		this->create_mapping();
	}

	/**
	 * Use this constructor if you want the system to work out what layout and mapping you need
	 * Just provide the attribute semantics, the only compulsory argument and optional format, step function and rate and let VertexDescriptor take care of the rest for you
	 * Rate is special, if any of the formats are custom then rate is split between two uin16_t, lower bits has rate while upper bits has format multiplier
	 * If no formats are custom then its value is used as it is, in other words the top bits when 0 or 1 both means single format multiplier
	 * Usage could be something like
	 * VertexDescriptor vd{rhi::ShaderSemantic::vertex_position, &bp};
	 * or
	 * VertexDescriptor vd{rhi::ShaderSemantic::vertex_position, rhi::VertexFormat::float32_3, &bp};
	 * or
	 * VertexDescriptor vd{rhi::ShaderSemantic::vertex_position, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, &bp};
	 * or
	 * VertexDescriptor vd{rhi::ShaderSemantic::vertex_position, rhi::StepFunction::vertex, &bp}; etc etc
	 * Argument that are not provided for each attribute will be the default values
	 * NOTE: Make sure you provide a rhi::BuffersPack somewhere  in the list otherwise it can't work out which buffers to use
	 */
	template <class... _types>
	FORCE_INLINE VertexDescriptor(_types... a_attributes)
	{
		this->parse_attributes_and_layouts(a_attributes...);
		this->create_mapping();
	}

	/**
	 * Same as the parameter pack constructor, exactly the same behaviour
	 * This is unlike the constructor which takes a vector of attributes and a vector of layouts, in which case client takes resonsibility of correctness
	 * In this case add takes care of the correctness of the attributes and layouts created
	 * As you can guess add with no arguments doesn't do anything but is still a valid call
	 * NOTE: Make sure you provide a rhi::BuffersPack somewhere  in the list otherwise it can't work out which buffers to use
	 */
	template <class... _types>
	void add(_types... a_attributes)
	{
		this->parse_attributes_and_layouts(a_attributes...);
		this->create_mapping();
	}

	/**
	 * Similar as the parameter pack add but it only accepts one attribute and one layout at a time
	 * This is unlike the constructor which takes a vector of attributes and a vector of layouts, in which case client takes resonsibility of correctness
	 * In this case add takes care of the correctness of the attributes and layouts created
	 */
	void add(VertexAttribute a_attribute, VertexLayout a_Layout, const rhi::BuffersPack *a_buffers_pack)
	{
		std::vector<tuple_type> attributes_tuple_vector{};

		auto rate       = a_Layout.rate();
		auto multiplier = a_Layout.format_multiplier();

		assert(rate <= 65535 && "Rate doesn't fit in 16bit!");
		assert(multiplier <= 65535 && "Format multiplier doesn't fit in 16bit!");

		rate = rate | (multiplier << 16);

		attributes_tuple_vector.emplace_back(a_attribute.semantics(), a_attribute.format(), a_Layout.step_function(), rate);
		this->create_attributes_and_layouts(attributes_tuple_vector, a_buffers_pack);
		this->create_mapping();
	}

	/**
	 * Similar as the parameter pack add but it accepts a structured vector of each attribute list and layout list
	 * This is unlike the constructor which takes a vector of attributes and a vector of layouts, in which case client takes resonsibility of correctness
	 * In this case add takes care of the correctness of the attributes and layouts created, size of the attributes and layouts vector must be same
	 */
	void add(std::vector<VertexAttribute> a_attribute, std::vector<VertexLayout> a_Layout, const rhi::BuffersPack *a_buffers_pack)
	{
		assert(a_attribute.size() == a_Layout.size() && "Attributes and layouts should be the to use this version of add!");
		std::vector<tuple_type> attributes_tuple_vector{};

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

	/**
	 * Should return true if all the attributes and layouts are created
	 * These attributes and layouts might not be valid though
	 */
	FORCE_INLINE bool complete() const
	{
		for (auto &attribute : this->m_attributes)
		{
			if (!attribute.complete())
				return false;
		}

		for (auto &layout : this->m_layouts)
		{
			if (!layout.complete())
				return false;
		}

		// TODO: Make sure VertexDescriptor returns a message saying whether there were packing/alignment and cache line issues

		return true;
	}

	/**
	 * Returns a bitmask of all the attributes in the VertexDescriptor
	 */
	FORCE_INLINE uint64_t type() const
	{
		uint64_t result = 0;

		for (auto &attribute : this->m_attributes)
		{
			result |= ror::enum_to_type_cast(attribute.semantics());
		}

		return result;
	}

	/**
	 * Returns an attribute for specified semantic
	 * If the key doesn't exisit expect std::out_of_range exception, I can't really catch this exception meaningfully
	 */
	FORCE_INLINE const VertexAttribute &attribute(rhi::BufferSemantic a_semantic_key) const
	{
		return this->m_mapping.at(a_semantic_key).first;
	}

	/**
	 * Returns a layout for specified semantic
	 * If the key doesn't exisit expect std::out_of_range exception, I can't really catch this exception meaningfully
	 */
	FORCE_INLINE const VertexLayout &layout(rhi::BufferSemantic a_semantic_key) const
	{
		return this->m_mapping.at(a_semantic_key).second;
	}

  private:
	FORCE_INLINE void create_mapping()
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

	/**
	 * Used for vertex attributes and layouts for instance or non-instanced attribtues
	 * It parses the argument pack a_attributes and creates a vector of tupes from it
	 * That tuple vector is easy to parse further in create_attributes_and_layouts which it calls at the end
	 */
	template <class... _types>
	FORCE_INLINE void parse_attributes_and_layouts(_types &...a_attributes)
	{
		// Using Rate here just to make it typesafe
		attrib_vector attributes{a_attributes...};

		if (attributes.size() < 1)
			return;

		tuple_type              default_tuple{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, 1};
		tuple_type              temp_tuple{default_tuple};
		std::vector<tuple_type> attributes_tuple_vector{};
		attributes_tuple_vector.reserve(attributes.size());        // Worst case scenario

		const rhi::BuffersPack *any_buffers_pack{nullptr};

		bool started = false;

		assert(std::get_if<rhi::BufferSemantic>(&attributes[0]) != nullptr && "Attribute description should be created with ShaderSemantics!");

		for (auto &attb : attributes)
		{
			// Very gross hack to make visiting variants work on GCC
			if constexpr (ror::get_compiler() == ror::CompilerType::comp_gcc)
			{
				if (auto shader_semantic = std::get_if<rhi::BufferSemantic>(&attb))
				{
					if (started)        // If we have already started and we found another ShaderSemantic, we have completed one attribute at least
					{
						attributes_tuple_vector.push_back(temp_tuple);
						// Also lets reset the temp_tuple
						temp_tuple = default_tuple;
					}
					else
						started = true;

					std::get<rhi::BufferSemantic>(temp_tuple) = *shader_semantic;
				}
				else if (auto vertex_format = std::get_if<rhi::VertexFormat>(&attb))
				{
					std::get<rhi::VertexFormat>(temp_tuple) = *vertex_format;
				}
				else if (auto step_function = std::get_if<rhi::StepFunction>(&attb))
				{
					std::get<rhi::StepFunction>(temp_tuple) = *step_function;
				}
				else if (auto rate = std::get_if<rhi::Rate>(&attb))
				{
					std::get<uint32_t>(temp_tuple) = rate->m_value;
				}
				else if (auto buffers_pack = std::get_if<const rhi::BuffersPack *>(&attb))
				{
					any_buffers_pack = *buffers_pack;
				}
			}
			else
			{
				// Instead of this the alternative is to do something like  "if (auto shader_semantic = std::get_if<rhi::ShaderSemantic>(&attb))" and build a chain
				std::visit(
					ror::Overload{
						[&](rhi::BufferSemantic sm) {
							if (started)        // If we have already started and we found another ShaderSemantic, we have completed one attribute at least
							{
								attributes_tuple_vector.push_back(temp_tuple);
								// Also lets reset the temp_tuple
								temp_tuple = default_tuple;
							}
							else
								started = true;

							std::get<rhi::BufferSemantic>(temp_tuple) = sm;
						},
						[&](rhi::VertexFormat vf) {
							std::get<rhi::VertexFormat>(temp_tuple) = vf;
						},
						[&](rhi::StepFunction sf) {
							std::get<rhi::StepFunction>(temp_tuple) = sf;
						},
						[&](rhi::Rate rt) {
							std::get<uint32_t>(temp_tuple) = rt.m_value;
						},
						[&](const rhi::BuffersPack *bp) {
							any_buffers_pack = bp;
						}},
					attb);
			}
		}

		assert(any_buffers_pack != nullptr && "No buffers pack provided, can't continue!");

		// Last one that was incomplete in the loop
		attributes_tuple_vector.push_back(temp_tuple);

		this->create_attributes_and_layouts(attributes_tuple_vector, any_buffers_pack);
	}

	/**
	 * Used for vertex attributes and layouts for both vertex and instance types
	 * This function resets/recreates the m_attributes and m_layouts member variables
	 */
	FORCE_INLINE void create_attributes_and_layouts(const std::vector<tuple_type> &a_attributes, const rhi::BuffersPack *a_buffers_pack)
	{
		size_t                                 existing_layouts_size{this->m_layouts.size()};
		size_t                                 existing_attributes_size{this->m_attributes.size()};
		uint32_t                               location{0};        // Assuming we start with 'an' order, zero is a good start for location
		std::unordered_map<uint32_t, uint32_t> strides{};          // Accumulated stride per buffer index
		uint32_t                               binding{0};         // Start with a lower binding and keep going up

		if (!this->m_attributes.empty())        // If this is not the first time we are calling this function
		{
			location = this->m_attributes[existing_attributes_size - 1].location();           // Location as it is is not correct, needs to be updated later
			binding  = this->m_attributes[existing_attributes_size - 1].binding() + 1;        // Increment by 1 for next binding

			auto format = this->m_attributes[existing_attributes_size - 1].format();
			location += vertex_format_to_location(format);        // Increment by the format to bytes of the last attribute
		}

		this->m_attributes.reserve(this->m_attributes.size() + a_attributes.size());
		this->m_layouts.reserve(this->m_layouts.size() + a_attributes.size());

		// Iterate over the attributes, create internal attributes vector as well as strides for layouts
		for (auto &attribute : a_attributes)
		{
			auto semantic = std::get<rhi::BufferSemantic>(attribute);
			auto format   = std::get<rhi::VertexFormat>(attribute);
			auto rate     = std::get<uint32_t>(attribute);

			uint32_t buffer_index = a_buffers_pack->attribute_buffer_index(semantic);
			// uint64_t buffer_offset   = bp.attribute_buffer_offset(semantic, 0ULL);        // TODO: How many bytes do we need for this, this has to be done later in a second pass
			uint32_t vert_format_to_bytes = vertex_format_to_bytes(format);

			if (semantic == rhi::BufferSemantic::custom &&
				(format == rhi::VertexFormat::uint8_custom || format == rhi::VertexFormat::uint16_custom || format == rhi::VertexFormat::float32_custom) &&
				(rate >> 16))
				vert_format_to_bytes *= (rate >> 16);        // Use the format multiplier from upper 16bits of rate

			// Get the old stride from existing layouts before inserting a new one
			if (existing_layouts_size > 0)
			{
				// This could be optimised by either adding a buffer_index in layout or creating another mapping, but this shouldn't be on hot path
				auto old_attribute = std::find_if(std::begin(this->m_attributes), std::end(this->m_attributes), [&](VertexAttribute &va) { return va.buffer_index() == buffer_index; });
				if (old_attribute != this->m_attributes.end())        // If an attribute with this buffer index exists then find its layout so we can use its binding
				{
					auto old_layout = std::find_if(std::begin(this->m_layouts), std::end(this->m_layouts), [&](VertexLayout &vl) { return vl.binding() == old_attribute->binding(); });

					assert(old_layout != this->m_layouts.end() && "Can't find a layout for attribute that should exis, something went wrong!");

					vert_format_to_bytes += old_layout->stride();
					old_layout->stride(vert_format_to_bytes);        // Also update the stride of this layout which will now be shared with this new attribute
				}
			}

			auto [buffer, success] = strides.insert({buffer_index, vert_format_to_bytes});

			if (!success)
			{
				// Check if we are locally or globally interleaved, in the later case don't need to update stride
				if (a_buffers_pack->attribute_buffer_interleaved(semantic))
					buffer->second += vert_format_to_bytes;
			}

			this->m_attributes.emplace_back(location, /* offset */ 0, /* buffer_offset */ 0ULL, binding, buffer_index, semantic, format);        // Hopefully its moved into the vector

			location += vertex_format_to_location(format);
			binding += 1;
		}

		// Iterate over attributes to allocate their layouts
		// for (size_t i = 0; i < this->m_attributes.size(); ++i)
		for (size_t i = 0; i < a_attributes.size(); ++i)
		{
			auto &final_attribute = this->m_attributes[existing_attributes_size + i];
			auto &raw_attribute   = a_attributes[i];
			auto  format          = final_attribute.format();
			auto  step_function   = std::get<rhi::StepFunction>(raw_attribute);
			auto  rate            = std::get<uint32_t>(raw_attribute);
			auto  multiplier      = std::max(1U, rate >> 16);

			uint32_t stride = strides[final_attribute.buffer_index()];

			if (!a_buffers_pack->attribute_buffer_interleaved(final_attribute.semantics()))        // If global interleaved buffer then use the format bytes as stride, otherwise multiply it
				stride = vertex_format_to_bytes(format) * multiplier;

			this->m_layouts.emplace_back(final_attribute.binding(), stride, rate & 65535, multiplier, step_function);
		}
	}

	using SemanticToAttribLayoutMap = std::unordered_map<rhi::BufferSemantic, std::pair<VertexAttribute &, VertexLayout &>>;

	std::vector<VertexAttribute> m_attributes{};        //! All attributes, either provided or created from ShaderSemantic and rhi::VertexFormat
	std::vector<VertexLayout>    m_layouts{};           //! All layouts for each buffer providing data for m_attributes
	SemanticToAttribLayoutMap    m_mapping{};           //! Mapping of each Semantic to its pair of layout and attribute
};

}        // namespace rhi
