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

#include "rhi/metal/rorbuffer.hpp"
#include "rhi/metal/rorcompute_command_encoder.hpp"
#include "rhi/metal/rorrender_command_encoder.hpp"

namespace rhi
{

define_translation_unit_vtable(BufferMetal)
{}

void BufferMetal::bind(rhi::RenderCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uintptr_t a_offset, uint32_t a_index) noexcept
{
	if (a_shader_stage == rhi::ShaderStage::fragment || a_shader_stage == rhi::ShaderStage::vertex_fragment || a_shader_stage == rhi::ShaderStage::compute_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.fragment_buffer(*this, a_offset, a_index);

	if (a_shader_stage == rhi::ShaderStage::vertex || a_shader_stage == rhi::ShaderStage::vertex_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.vertex_buffer(*this, a_offset, a_index);

	if (a_shader_stage == rhi::ShaderStage::tile)
		a_command_encoder.tile_buffer(*this, a_offset, a_index);
}

void BufferMetal::bind(rhi::ComputeCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uintptr_t a_offset, uint32_t a_index) noexcept
{
	if (a_shader_stage == rhi::ShaderStage::compute || a_shader_stage == rhi::ShaderStage::compute_vertex || a_shader_stage == rhi::ShaderStage::compute_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.buffer(*this, a_offset, a_index);
	else
	{
		assert(0 && "Can't bind texture image to this shader stage");
	}
}
}        // namespace rhi
