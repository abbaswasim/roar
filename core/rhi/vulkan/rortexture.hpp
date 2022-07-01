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

#pragma once

#include "rhi/crtp_interfaces/rortexture.hpp"
#include "rhi/rorrender_types.hpp"

namespace rhi
{

class ROAR_ENGINE_ITEM TextureImageVulkan : public TextureImageCrtp<TextureImageVulkan>
{
	declare_translation_unit_vtable();
};

class ROAR_ENGINE_ITEM TextureSamplerVulkan : public TextureSamplerCrtp<TextureSamplerVulkan>
{
	declare_translation_unit_vtable();
};

class ROAR_ENGINE_ITEM TextureVulkan final : public TextureCrtp<TextureVulkan>
{
	declare_translation_unit_vtable();
};

declare_rhi_render_type(TextureImage);
declare_rhi_render_type(TextureSampler);
declare_rhi_render_type(Texture);
// declare_rhi_render_type(Buffer);
// declare_rhi_render_type(GraphicsPipeline);
// declare_rhi_render_type(ComputePipeline);
// declare_rhi_render_type(MeshPipeline);

// Define type to semantic for buffers allocator
define_type_to_shader_semantics(TextureImageVulkan)
{
	return rhi::BufferSemantic::texture_image_data;
}

define_type_to_shader_semantics(TextureSamplerVulkan)
{
	return rhi::BufferSemantic::texture_sampler_data;
}

define_type_to_shader_semantics(TextureVulkan)
{
	return rhi::BufferSemantic::texture_data;
}

}        // namespace rhi