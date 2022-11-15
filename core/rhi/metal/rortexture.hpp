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

#include "foundation/rormacros.hpp"
#include "rhi/crtp_interfaces/rortexture.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/rortypes.hpp"

#include <Metal/MTLSampler.hpp>
#include <Metal/MTLTexture.hpp>

namespace rhi
{
declare_rhi_render_type(RenderCommandEncoder);
declare_rhi_render_type(ComputeCommandEncoder);

class ROAR_ENGINE_ITEM TextureImageMetal : public TextureImageCrtp<TextureImageMetal>
{
  public:
	void upload(rhi::Device &a_device);

	FORCE_INLINE constexpr auto platform_handle() const noexcept;

	void bind(rhi::RenderCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) noexcept;
	void bind(rhi::ComputeCommandEncoder &a_cmd_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) noexcept;

  protected:
  private:
	declare_translation_unit_vtable();

	MTL::Texture *m_texture{nullptr};
};

class ROAR_ENGINE_ITEM TextureSamplerMetal : public TextureSamplerCrtp<TextureSamplerMetal>
{
  public:
	void upload(rhi::Device &a_device);

	FORCE_INLINE constexpr auto platform_handle() const noexcept;

	void bind(rhi::RenderCommandEncoder &a_cmd_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) noexcept;
	void bind(rhi::ComputeCommandEncoder &a_cmd_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) noexcept;

  protected:
  private:
	declare_translation_unit_vtable();

	MTL::SamplerState *m_sampler{nullptr};
};

class ROAR_ENGINE_ITEM TextureMetal final : public TextureCrtp<TextureMetal>
{
  public:
  protected:
  private:
	declare_translation_unit_vtable();
};

// The following will create something like
// using Texture = TextureMetal
// So the rest of the system can use the CRTP implementation directly as "Texture"

declare_rhi_render_type(TextureImage);
declare_rhi_render_type(TextureSampler);
declare_rhi_render_type(Texture);

// Define type to semantic for buffers allocator
define_type_to_shader_semantics(TextureImageMetal)
{
	return rhi::BufferSemantic::texture_image_data;
}

define_type_to_shader_semantics(TextureSamplerMetal)
{
	return rhi::BufferSemantic::texture_sampler_data;
}

define_type_to_shader_semantics(TextureMetal)
{
	return rhi::BufferSemantic::texture_data;
}

}        // namespace rhi

#include "rhi/metal/rortexture.hh"
