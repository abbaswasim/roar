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
#include "profiling/rorlog.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_attribute.hpp"
#include "rhi/rorvertex_layout.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
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

using tuple_type            = std::tuple<rhi::BufferSemantic, rhi::VertexFormat, rhi::StepFunction, uint32_t>;
using tuple_type_vector     = std::vector<tuple_type>;
using attrib_variant        = std::variant<const rhi::BuffersPack *, rhi::BufferSemantic, rhi::VertexFormat, rhi::StepFunction, rhi::Rate>;
using attrib_variant_vector = std::vector<attrib_variant>;

/**
 * Can be used to describe vertex attributes and layouts for a single mesh drawable.
 * It will use the provided buffers pack to access the default buffers allocated via "buffers_format.json"
 */

class ROAR_ENGINE_ITEM VertexDescriptor final
{
  public:
	FORCE_INLINE                   VertexDescriptor()                                    = default;        //! Default constructor
	FORCE_INLINE                   VertexDescriptor(const VertexDescriptor &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                   VertexDescriptor(VertexDescriptor &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE VertexDescriptor &operator=(const VertexDescriptor &a_other)            = default;        //! Copy assignment operator
	FORCE_INLINE VertexDescriptor &operator=(VertexDescriptor &&a_other) noexcept        = default;        //! Move assignment operator
	FORCE_INLINE ~VertexDescriptor() noexcept                                            = default;        //! Destructor

	/**
	 * Use this constructor to create mapping of attributes to layouts that you think works.
	 * If you are not sure what layout do I need and what is the buffers format use the single argument ctor instead
	 * Its usage might be risky and slightly complicated, forcing you to work out layouts and alignments manually
	 */
	FORCE_INLINE VertexDescriptor(std::vector<VertexAttribute> a_attributes, std::vector<VertexLayout> a_layouts);

	/**
	 * Use this constructor if you want the system to work out what layout and mapping you need
	 * Just provide the attribute semantics, the only compulsory argument and optional format, step function and rate and let VertexDescriptor take care of the rest for you
	 * Rate is special, if any of the formats are custom then rate is split between two uint16_t, lower bits has rate while upper bits has format multiplier
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
	FORCE_INLINE VertexDescriptor(_types... a_attributes);

	/**
	 * Same as the parameter pack constructor, exactly the same behaviour
	 * This is unlike the constructor which takes a vector of attributes and a vector of layouts, in which case client takes resonsibility of correctness
	 * In this case add takes care of the correctness of the attributes and layouts created
	 * As you can guess add with no arguments doesn't do anything but is still a valid call
	 * NOTE: Make sure you provide a rhi::BuffersPack somewhere  in the list otherwise it can't work out which buffers to use
	 */
	template <class... _types>
	void add(_types... a_attributes);

	/**
	 * Similar as the parameter pack add but it only accepts one attribute and one layout at a time
	 * This is unlike the constructor which takes a vector of attributes and a vector of layouts, in which case client takes resonsibility of correctness
	 * In this case add takes care of the correctness of the attributes and layouts created
	 */
	void add(VertexAttribute a_attribute, VertexLayout a_Layout, const rhi::BuffersPack *a_buffers_pack);

	/**
	 * Similar as the parameter pack add but it accepts a structured vector of each attribute list and layout list
	 * This is unlike the constructor which takes a vector of attributes and a vector of layouts, in which case client takes resonsibility of correctness
	 * In this case add takes care of the correctness of the attributes and layouts created, size of the attributes and layouts vector must be same
	 */
	void add(std::vector<VertexAttribute> a_attribute, std::vector<VertexLayout> a_Layout, const rhi::BuffersPack *a_buffers_pack);

	/**
	 * Returns a bitmask of all the attributes in the VertexDescriptor
	 */
	FORCE_INLINE uint64_t type() const;

	/**
	 * Returns an attribute for specified semantic
	 * If the key doesn't exisit expect std::out_of_range exception, I can't really catch this exception meaningfully
	 */
	FORCE_INLINE VertexAttribute &attribute(rhi::BufferSemantic a_semantic_key) const;

	/**
	 * Returns a layout for specified semantic
	 * If the key doesn't exisit expect std::out_of_range exception, I can't really catch this exception meaningfully
	 */
	FORCE_INLINE VertexLayout &layout(rhi::BufferSemantic a_semantic_key) const;

	/**
	 * Does an upload/update into the buffer for these attributes, it doesn't have to be all the attributes
	 * It can do element wise or bulk upload/update depending on the source and destination strides
	 * It requires a tuple of buffer_pointer, buffer_size and buffer_stride for each attribute keyed by semantic.
	 * The buffer_size is the total bytes required for this attribute, it doesn't necessarily means the whole buffer available.
	 * This is because the data might be interleaved. In which case the buffer_size for both attributes will be added up using stride.
	 */
	void upload(const std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>> &a_attrib_data, rhi::BuffersPack *a_buffers_pack);
	void update(const std::unordered_map<rhi::BufferSemantic, std::tuple<uint8_t *, uint32_t, uint32_t>> &a_attrib_data, rhi::BuffersPack *a_buffers_pack);

	FORCE_INLINE const auto &attributes() const;
	FORCE_INLINE const auto &layouts() const;
	FORCE_INLINE auto        hash_64() const;
	FORCE_INLINE auto        hash_64_pass_aware(rhi::RenderpassType a_passtype) const;

	FORCE_INLINE auto &attributes();

  private:
	void create_mapping();

	/**
	 * Used for vertex attributes and layouts for instance or non-instanced attribtues
	 * It parses the argument pack a_attributes and creates a vector of tupes from it
	 * That tuple vector is easy to parse further in create_attributes_and_layouts which it calls at the end
	 */
	template <class... _types>
	void parse_attributes_and_layouts(_types &...a_attributes);

	/**
	 * Used for vertex attributes and layouts for both vertex and instance types
	 * This function updates/resets/recreates the m_attributes and m_layouts member variables
	 */
	void create_attributes_and_layouts(const tuple_type_vector &a_attributes, const rhi::BuffersPack *a_buffers_pack);

	using SemanticToAttribLayoutMap = std::unordered_map<rhi::BufferSemantic, std::pair<VertexAttribute &, VertexLayout &>>;

	std::vector<VertexAttribute> m_attributes{};        //! All attributes, either provided or created from ShaderSemantic and rhi::VertexFormat
	std::vector<VertexLayout>    m_layouts{};           //! All layouts for each buffer providing data for m_attributes
	SemanticToAttribLayoutMap    m_mapping{};           //! Mapping of each Semantic to its pair of layout and attribute
};

}        // namespace rhi

#include "rhi/rorvertex_description.hh"
