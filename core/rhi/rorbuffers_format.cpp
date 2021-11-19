// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

#include "rhi/rorbuffers_format.hpp"
#include <cstdio>
#include "foundation/rorutilities.hpp"
#include <functional>
#include <memory>
#include "rhi/rorbuffer.hpp"
#include "rhi/rortypes.hpp"
#include <unordered_map>

namespace ror
{

void BuffersFormatConfig::load_specific()
{
	assert(this->m_json_file.contains("buffer_packs") && "BufferFormatConfig file must contain a list of buffer_packs");

	if (this->m_json_file.contains("unit"))
		this->m_buffers_format.m_unit = this->m_json_file["unit"];

	std::string current_buffer_pack{};
	if (this->m_json_file.contains("buffer_pack_current"))
		current_buffer_pack = this->m_json_file["buffer_pack_current"];

	auto buffer_packs = this->m_json_file["buffer_packs"];

	this->m_buffers_format.m_buffer_packs.reserve(buffer_packs.size());
	for (auto &bp : buffer_packs)
	{
		BufferPack buffer_pack;
		buffer_pack.m_name = bp["name"];

		auto buffers = bp["buffers"];
		buffer_pack.m_buffers.reserve(buffers.size());
		for (auto &b : buffers)
		{
			rhi::Buffer buffer;
			assert(b.contains("size") && "Each buffer should specifiy a size");
			uint64_t size = b["size"];
			size *= this->m_buffers_format.m_unit;
			buffer.size(size);

			if (b.contains("interleaved"))
				buffer.interleaved(b["interleaved"] == "global" ? false : true);

			auto semantics = b["semantic"];
			for (auto &f : semantics)
			{
				for (auto it = f.begin(); it != f.end(); ++it)
					buffer.emplace_semantic(std::make_pair(rhi::get_format_shader_semantic(it.key()), it.value()));
			}

			buffer_pack.m_buffers.emplace_back(std::move(buffer));
		}

		this->m_buffers_format.m_buffer_packs.emplace_back(std::move(buffer_pack));
	}

	for (size_t k = 0; k < this->m_buffers_format.m_buffer_packs.size(); ++k)
	{
		if (this->m_buffers_format.m_buffer_packs[k].m_name == current_buffer_pack)
		{
			this->m_buffers_format.m_current_format = static_cast_safe<uint32_t>(k);
			break;
		}
	}

	// Add buffers that might be required but not specified in the config
	this->load_remaining_buffers();
}

void BuffersFormatConfig::load_remaining_buffers()
{
	std::vector<rhi::BufferSemantic> all_semantics{
#define item(_enum) rhi::ShaderSemantic::_enum
#define item_value(_enum)
		describe_shader_semantics(item)
#undef item
#undef item_value
	};

	std::unordered_map<rhi::BufferSemantic, bool> remaining;
	remaining.clear();
	remaining.reserve(all_semantics.size());

	for (auto &buffer_pack : this->m_buffers_format.m_buffer_packs)
	{
		rhi::Buffer *empty_buffer{nullptr};        // Non-owning ptr, so its ok to use raw pointer here

		for (auto &semantic : all_semantics)
			remaining[semantic] = false;        // Deliberately using the [] operator here, which inserts or updates

		for (auto &buffer : buffer_pack.m_buffers)
		{
			if (buffer.semantics().size() == 0)        // While we are at it find an empty buffer as well
				empty_buffer = &buffer;

			for (auto &semantic : buffer.semantics())
			{
				remaining[semantic.first] = true;
			}
		}

		// We have been through all buffers, there should be an empty buffer available now
		assert(empty_buffer != nullptr && "Buffer pack doesn't provide an empty buffer for remaining buffers");

		// In the empty buffer find place for the remaining semantics
		size_t result = std::accumulate(std::begin(remaining), std::end(remaining), 0ULL,
										[](const size_t previous, const std::pair<rhi::BufferSemantic, bool> &p) {
											return previous + (p.second ? 0 : 1);
										});

		uint64_t per_buffer_size = empty_buffer->size() / result;        // Roughly split the size of the empty buffer between the remaining semantics
		for (auto &semantic : remaining)
			if (semantic.second == false)
				empty_buffer->emplace_semantic({semantic.first, per_buffer_size});
	}
}

define_translation_unit_vtable(BuffersFormatConfig)
{}

}        // namespace ror
