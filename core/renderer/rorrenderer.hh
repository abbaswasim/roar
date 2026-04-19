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

#include "foundation/rormacros.hpp"
#include "renderer/rorrenderer.hpp"
#include "settings/rorsettings.hpp"

namespace ror
{

FORCE_INLINE Renderer::Renderer()
{
	this->load(ror::settings().m_renderer_config, ResourceSemantic::configs);        // By default its renderer.json
}

FORCE_INLINE constexpr auto Renderer::picking_available() noexcept
{
	if (this->m_pick_queue >= 0)
		return this->m_pick_queue-- == 0;

	return false;
}

void toggle_selection(ror::NodeData *a_node);

FORCE_INLINE auto Renderer::calculate_shader_buffer_index(rhi::ShaderBufferFrequency a_frequency) const
{
	size_t frequency{0};

	if (a_frequency == rhi::ShaderBufferFrequency::constant)
		frequency = 0;
	else if (a_frequency == rhi::ShaderBufferFrequency::per_view)
		frequency = this->m_renderpass_index + this->m_frames.current_frame_index() * this->m_renderpasses_count;
	else if (a_frequency == rhi::ShaderBufferFrequency::per_frame)
		frequency = this->m_frames.current_frame_index();

	return frequency;
}

FORCE_INLINE auto Renderer::calculate_shader_buffer_index(const rhi::ShaderBuffer &a_shader_buffer) const
{
	auto frequency = a_shader_buffer.frequency();

	return calculate_shader_buffer_index(frequency);
}

FORCE_INLINE auto Renderer::shader_buffer(const std::string &a_name) const
{
	auto   shader_buffers = this->m_buffers_mapping.at(a_name);
	auto   shader_buffer  = &(*shader_buffers)[0];                                // Get shader buffer at first index, there must be one there
	size_t frequency      = calculate_shader_buffer_index(*shader_buffer);        // Now get the real frequency/index depending on which frame and which renderpass

	return &(*shader_buffers)[frequency];
}

}        // namespace ror
