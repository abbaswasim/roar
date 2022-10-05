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

#include "foundation/rorhash.hpp"
#include "foundation/rormacros.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"

namespace rhi
{

FORCE_INLINE VertexDescriptor::VertexDescriptor(std::vector<VertexAttribute> a_attributes, std::vector<VertexLayout> a_layouts) :
    m_attributes(a_attributes),
    m_layouts(a_layouts)
{
	this->create_mapping();
}

template <class... _types>
FORCE_INLINE VertexDescriptor::VertexDescriptor(_types... a_attributes)
{
	this->parse_attributes_and_layouts(a_attributes...);
	this->create_mapping();
}

template <class... _types>
void VertexDescriptor::add(_types... a_attributes)
{
	this->parse_attributes_and_layouts(a_attributes...);
	this->create_mapping();
}

FORCE_INLINE uint64_t VertexDescriptor::type() const
{
	uint64_t result = 0;

	for (auto &attribute : this->m_attributes)
	{
		result |= ror::enum_to_type_cast(attribute.semantics());
	}

	return result;
}

FORCE_INLINE VertexAttribute &VertexDescriptor::attribute(rhi::BufferSemantic a_semantic_key) const
{
	if constexpr (ror::get_build() == ror::BuildType::build_debug)
	{
		auto key = this->m_mapping.find(a_semantic_key);
		assert(key != this->m_mapping.end() && "A semantic key requested but it doesn't exist in the map");
		(void) key;
	}
	return this->m_mapping.at(a_semantic_key).first;
}

FORCE_INLINE VertexLayout &VertexDescriptor::layout(rhi::BufferSemantic a_semantic_key) const
{
	if constexpr (ror::get_build() == ror::BuildType::build_debug)
	{
		auto key = this->m_mapping.find(a_semantic_key);
		assert(key != this->m_mapping.end() && "A semantic key requested but it doesn't exist in the map");
		(void) key;
	}
	return this->m_mapping.at(a_semantic_key).second;
}

FORCE_INLINE const auto &VertexDescriptor::attributes() const
{
	return this->m_attributes;
}

FORCE_INLINE auto &VertexDescriptor::attributes()
{
	return this->m_attributes;
}

FORCE_INLINE const auto &VertexDescriptor::layouts() const
{
	return this->m_layouts;
}

template <class... _types>
void VertexDescriptor::parse_attributes_and_layouts(_types &...a_attributes)
{
	// Using Rate here just to make it typesafe
	attrib_variant_vector attributes{a_attributes...};

	if (attributes.size() < 1)
		return;

	tuple_type        default_tuple{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, 1};
	tuple_type        temp_tuple{default_tuple};
	tuple_type_vector attributes_tuple_vector{};
	attributes_tuple_vector.reserve(attributes.size());        // Worst case scenario

	const rhi::BuffersPack *any_buffers_pack{nullptr};

	bool started = false;

	assert(std::get_if<rhi::BufferSemantic>(&attributes[0]) != nullptr && "Attribute description should be started with ShaderSemantics!");

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

// TODO: cache the result if this turns out to be expensive
FORCE_INLINE auto VertexDescriptor::hash_64() const
{
	const auto type = this->type();

	auto hash = ror::hash_64(&type, sizeof(type));

	for (auto &attrib : this->m_attributes)
	{
		const auto location  = attrib.location();
		const auto offset    = attrib.offset();
		const auto semantics = attrib.semantics();
		const auto format    = attrib.format();

		ror::hash_combine_64(hash, ror::hash_64(&location, sizeof(location)));          // Now lets add all the locations
		ror::hash_combine_64(hash, ror::hash_64(&offset, sizeof(offset)));              // Now lets add all the offests
		ror::hash_combine_64(hash, ror::hash_64(&semantics, sizeof(semantics)));        // Now lets add all the semantics
		ror::hash_combine_64(hash, ror::hash_64(&format, sizeof(format)));              // Now lets add all the formats
	}

	return hash;
}

FORCE_INLINE auto VertexDescriptor::hash_64_pass_aware(rhi::RenderpassType a_passtype) const
{
	auto is_depth_shadow = (a_passtype == rhi::RenderpassType::depth || a_passtype == rhi::RenderpassType::shadow);

	const auto type = this->type();

	auto hash = ror::hash_64(&type, sizeof(type));

	for (auto &attrib : this->m_attributes)
	{
		const auto semantics = attrib.semantics();
		if (is_attribute_required_in_pass(semantics, is_depth_shadow))
		{
			const auto location = attrib.location();
			const auto offset   = attrib.offset();
			const auto format   = attrib.format();

			ror::hash_combine_64(hash, ror::hash_64(&location, sizeof(location)));          // Now lets add all the locations
			ror::hash_combine_64(hash, ror::hash_64(&offset, sizeof(offset)));              // Now lets add all the offests
			ror::hash_combine_64(hash, ror::hash_64(&semantics, sizeof(semantics)));        // Now lets add all the semantics
			ror::hash_combine_64(hash, ror::hash_64(&format, sizeof(format)));              // Now lets add all the formats
		}
	}

	return hash;
}
}        // namespace rhi
