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

#include "rhi/rorrhi_buffers_pack.hpp"
#include "rhi/rorrhi_vertex_attribute.hpp"
#include "rhi/rorrhi_vertex_layout.hpp"
#include <cassert>
#include <cstddef>
#include "foundation/rorcompiler_workarounds.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorutilities.hpp"
#include <functional>
#include <memory>
#include "profiling/rorlog.hpp"
#include "rhi/rorrhi_types.hpp"
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
	uint32_t m_value;
};

// TODO: Make sure VertexDescriptor returns a message saying whether there were packing/alignment and cache line issues
class ROAR_ENGINE_ITEM VertexDescriptor final
{
  public:
	FORCE_INLINE VertexDescriptor()                                    = default;                   //! Default constructor
	FORCE_INLINE VertexDescriptor(const VertexDescriptor &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE VertexDescriptor(VertexDescriptor &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE VertexDescriptor &operator=(const VertexDescriptor &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE VertexDescriptor &operator=(VertexDescriptor &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~VertexDescriptor() noexcept                                     = default;        //! Destructor

	// Probably the r-value refs should be removed here
	/**
	 * Use this constructor to create mapping of attributes to layouts that you think works.
	 * If you are not sure what layout do I need and what is the buffers format use the single arguemnt ctor instead
	 */
	// TODO: Perhaps think about removing this ctor, its usage is risky and complicated
	FORCE_INLINE VertexDescriptor(std::vector<VertexAttribute> &&a_attributes, std::vector<VertexLayout> &&a_layouts) :
		m_attributes(std::move(a_attributes)),
		m_layouts(std::move(a_layouts))
	{
		this->create_mapping();
	}

	/**
	 * Use this constructor if you want the system to work out what layout and mapping you need
	 * Just provide the attribute semantics, the only compulsory argument and optional format, step function and rate and let VertexDescriptor take care of the rest for you
	 * Rate is special, any of the formats are custom then rate is split between two uin16_t, lower bits has rate while upper bits has format multiplier
	 * If no formats are custom then its value is used as it is, in other words the top bits when 0 or 1 both means single format multiplier
	 * Usage could be something like
	 * VertexDescriptor vd{ShaderSemantic::vertex_position};
	 * or
	 * VertexDescriptor vd{ShaderSemantic::vertex_position, VertexFormat::float32_3};
	 * or
	 * VertexDescriptor vd{ShaderSemantic::vertex_position, VertexFormat::float32_3, StepFunction::vertex};
	 * or
	 * VertexDescriptor vd{ShaderSemantic::vertex_position, StepFunction::vertex}; etc etc
	 * Argument that are not provided for each attribute will be the default values
	 */
	template <class... _types>
	FORCE_INLINE VertexDescriptor(_types... a_attributes)
	{
		this->parse_attributes_and_layouts(a_attributes...);
		this->create_mapping();
	}

	FORCE_INLINE void add(int32_t a_attribute);
	void              add_position(VertexAttribute a_attribute);
	void              add_non_position(VertexAttribute a_attribute);        // Maybe requires a method per semantic

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

		return true;
	}

	/**
	 * Returns a bitmask of all the attributes in the VertexDescriptor
	 */
	FORCE_INLINE uint32_t type() const
	{
		uint32_t result = 0;

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
	FORCE_INLINE const VertexAttribute &attribute(ShaderSemantic a_semantic_key) const
	{
		return this->m_mapping.at(a_semantic_key).first;
	}

	/**
	 * Returns a layout for specified semantic
	 * If the key doesn't exisit expect std::out_of_range exception, I can't really catch this exception meaningfully
	 */
	FORCE_INLINE const VertexLayout &layout(ShaderSemantic a_semantic_key) const
	{
		return this->m_mapping.at(a_semantic_key).second;
	}

  private:
	FORCE_INLINE void create_mapping()
	{
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
	 */
	template <class... _types>
	FORCE_INLINE void parse_attributes_and_layouts(_types &...a_attributes)
	{
		// Using Rate here just to make it typesafe
		std::vector<std::variant<ShaderSemantic, VertexFormat, StepFunction, Rate>> attributes{a_attributes...};

		using tuple_type = std::tuple<ShaderSemantic, VertexFormat, StepFunction, uint32_t>;

		tuple_type              default_tuple{ShaderSemantic::vertex_position, VertexFormat::float32_3, StepFunction::vertex, 1};
		tuple_type              temp_tuple{default_tuple};
		std::vector<tuple_type> attributes_tuple_vector{};
		attributes_tuple_vector.reserve(attributes.size());        // Worst case scenario

		bool started = false;

		if (attributes.size() < 1)
			return;

		assert(std::get_if<ShaderSemantic>(&attributes[0]) != nullptr && "Attribute description should be created with ShaderSemantics!");

		for (auto &attb : attributes)
		{
			// Instead of this the alternative is to do somethign like  "if (auto shader_semantic = std::get_if<ShaderSemantic>(&attb))" and build a chain
			std::visit(
				ror::Overload{
					[&](ShaderSemantic sm) {
						if (started)        // If we have alraedy started and we found another ShaderSemantic, we have completed one attribute at least
						{
							attributes_tuple_vector.push_back(temp_tuple);
							// Also lets reset the temp_tuple
							temp_tuple = default_tuple;
						}
						else
							started = true;

						std::get<ShaderSemantic>(temp_tuple) = sm;
					},
					[&](VertexFormat vf) {
						std::get<VertexFormat>(temp_tuple) = vf;
					},
					[&](StepFunction sf) {
						std::get<StepFunction>(temp_tuple) = sf;
					},
					[&](Rate rt) {
						std::get<uint32_t>(temp_tuple) = rt.m_value;
					},
				},
				attb);
		}

		// Last one that was incomplete in the loop
		attributes_tuple_vector.push_back(temp_tuple);

		this->create_attributes_and_layouts(attributes_tuple_vector);
	}

	/**
	 * Only used for vertex attributes and layouts not for instance attribtues
	 */
	FORCE_INLINE void create_attributes_and_layouts(const std::vector<std::tuple<ShaderSemantic, VertexFormat, StepFunction, uint32_t>> &a_attributes)
	{
		uint32_t                               location = 0;        // Assuming we start with 'an' order, zero is a good start for location
		std::unordered_map<uint32_t, uint32_t> strides{};           // Accumulated stride per buffer index
		uint32_t                               binding = 0;         // Start with a lower binding and keep going up

		this->m_attributes.reserve(a_attributes.size());
		this->m_layouts.reserve(a_attributes.size());

		const auto &bp = get_buffers_pack();        // TODO: Should I have my own static BuffersPack in here? That way I can change it around without consequences?

		// Iterate over the attributes, create internal attributes vector as well as strides for layouts
		for (auto &attribute : a_attributes)
		{
			auto semantic = std::get<ShaderSemantic>(attribute);
			auto format   = std::get<VertexFormat>(attribute);
			auto rate     = std::get<uint32_t>(attribute);

			uint32_t buffer_index = bp.attribute_buffer_index(semantic);
			// uint64_t buffer_offset   = bp.attribute_buffer_offset(semantic, 0ULL);        // FIXME: How many bytes do we need for this, this has to be done later in a second pass
			uint32_t format_to_bytes = vertex_format_to_bytes(format);

			if (semantic == rhi::ShaderSemantic::custom &&
				(format == rhi::VertexFormat::uint8_custom || format == rhi::VertexFormat::uint16_custom || format == rhi::VertexFormat::float32_custom) &&
				(rate >> 16))
				format_to_bytes *= (rate >> 16);        // Use the format multiplier from upper 16bits of rate

			auto [buffer, success] = strides.insert({buffer_index, format_to_bytes});

			if (!success)
			{
				// Check if we are locally or globally interleaved, in the later case don't need to update stride
				if (bp.attribute_buffer_interleaved(semantic))
					buffer->second += format_to_bytes;
			}

			this->m_attributes.emplace_back(location, /* offset */ 0, /* buffer_offset */ 0ULL, binding, buffer_index, semantic, format);        // Hopefully its moved into the vector

			location += vertex_format_to_location(format);
			binding += 1;
		}

		// Iterate over attributes to allocate their layouts
		for (size_t i = 0; i < this->m_attributes.size(); ++i)
		{
			auto &final_attribute = this->m_attributes[i];
			auto &raw_attribute   = a_attributes[i];
			auto  format          = final_attribute.format();
			auto  step_function   = std::get<StepFunction>(raw_attribute);
			auto  rate            = std::get<uint32_t>(raw_attribute);
			auto  multiplier      = std::max(1U, rate >> 16);

			uint32_t stride = strides[final_attribute.buffer_index()];

			if (!bp.attribute_buffer_interleaved(final_attribute.semantics()))        // If global interleaved buffer then use the format bytes as stride
				stride = vertex_format_to_bytes(format) * multiplier;

			this->m_layouts.emplace_back(final_attribute.binding(), stride, rate & 65535, multiplier, step_function);
		}
	}

	using SemanticToAttribLayoutMap = std::unordered_map<ShaderSemantic, std::pair<VertexAttribute &, VertexLayout &>>;

	std::vector<VertexAttribute> m_attributes{};        //! All attributes, either provided or created from ShaderSemantic and VertexFormat
	std::vector<VertexLayout>    m_layouts{};           //! All layouts for each buffer providing data for m_attributes
	SemanticToAttribLayoutMap    m_mapping{};           //! Mapping of each Semantic to its pair of layout and attribute
};

// static_assert(std::is_trivially_copyable<VertexDescriptor>::value, "VertexDescriptor is not trivially copyable");

}        // namespace rhi
