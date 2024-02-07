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

#include "rhi/crtp_interfaces/rortexture.hpp"
#include "rhi/metal/rorcompute_command_encoder.hpp"
#include "rhi/metal/rordevice.hpp"
#include "rhi/metal/rormetal_common.hpp"
#include "rhi/metal/rorrender_command_encoder.hpp"
#include "rhi/metal/rortexture.hpp"
#include "rhi/rortypes.hpp"

#include <Metal/MTLBlitCommandEncoder.hpp>
#include <Metal/MTLBuffer.hpp>
#include <Metal/MTLCommandBuffer.hpp>
#include <Metal/MTLCommandEncoder.hpp>
#include <Metal/MTLCommandQueue.hpp>
#include <Metal/MTLDevice.hpp>
#include <Metal/MTLResource.hpp>
#include <Metal/MTLSampler.hpp>
#include <Metal/MTLTexture.hpp>
#include <Metal/MTLTypes.hpp>
#include <cassert>

namespace rhi
{

FORCE_INLINE constexpr MTL::TextureType to_metal_texture_target(rhi::TextureTarget a_target)
{
	switch (a_target)
	{
		case rhi::TextureTarget::texture_1D:
			return MTL::TextureType::TextureType1D;
		case rhi::TextureTarget::texture_1D_array:
			return MTL::TextureType::TextureType1DArray;
		case rhi::TextureTarget::texture_2D:
			return MTL::TextureType::TextureType2D;
		case rhi::TextureTarget::texture_2D_array:
			return MTL::TextureType::TextureType2DArray;
		case rhi::TextureTarget::texture_2D_MS:
			return MTL::TextureType::TextureType2DMultisample;
		case rhi::TextureTarget::texture_cube:
			return MTL::TextureType::TextureTypeCube;
		case rhi::TextureTarget::texture_cube_array:
			return MTL::TextureType::TextureTypeCubeArray;
		case rhi::TextureTarget::texture_3D:
			return MTL::TextureType::TextureType3D;
		case rhi::TextureTarget::texture_2D_MS_array:
			return MTL::TextureType::TextureType2DMultisampleArray;
	}
}

FORCE_INLINE constexpr MTL::SamplerMinMagFilter to_metal_texture_filter(rhi::TextureFilter a_filter)
{
	return static_cast<MTL::SamplerMinMagFilter>(a_filter);
}

FORCE_INLINE constexpr MTL::SamplerMipFilter to_metal_texture_mip_filter(rhi::TextureMipFilter a_filter)
{
	return static_cast<MTL::SamplerMipFilter>(a_filter);
}

FORCE_INLINE constexpr MTL::SamplerBorderColor to_metal_texture_border(rhi::TextureBorder a_color)
{
	return static_cast<MTL::SamplerBorderColor>(a_color);
}

FORCE_INLINE constexpr MTL::SamplerAddressMode to_metal_texture_address_mode(rhi::TextureAddressMode a_mode)
{
	return static_cast<MTL::SamplerAddressMode>(a_mode);
}

void TextureImageMetal::bind(rhi::RenderCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) const noexcept
{
	if (a_shader_stage == rhi::ShaderStage::fragment || a_shader_stage == rhi::ShaderStage::vertex_fragment || a_shader_stage == rhi::ShaderStage::compute_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.fragment_texture(*this, a_index);

	if (a_shader_stage == rhi::ShaderStage::vertex || a_shader_stage == rhi::ShaderStage::vertex_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.vertex_texture(*this, a_index);

	if (a_shader_stage == rhi::ShaderStage::tile)
		a_command_encoder.tile_texture(*this, a_index);
}

void TextureImageMetal::bind(rhi::ComputeCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) const noexcept
{
	if (a_shader_stage == rhi::ShaderStage::compute || a_shader_stage == rhi::ShaderStage::compute_vertex || a_shader_stage == rhi::ShaderStage::compute_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.texture(*this, a_index);
	else
	{
		assert(0 && "Can't bind texture image to this shader stage");
	}
}

void TextureSamplerMetal::bind(rhi::RenderCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) const noexcept
{
	if (a_shader_stage == rhi::ShaderStage::fragment || a_shader_stage == rhi::ShaderStage::vertex_fragment || a_shader_stage == rhi::ShaderStage::compute_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.fragment_sampler(*this, a_index);

	if (a_shader_stage == rhi::ShaderStage::vertex || a_shader_stage == rhi::ShaderStage::vertex_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.vertex_sampler(*this, a_index);

	if (a_shader_stage == rhi::ShaderStage::tile)
		a_command_encoder.tile_sampler(*this, a_index);
}

void TextureSamplerMetal::bind(rhi::ComputeCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) const noexcept
{
	if (a_shader_stage == rhi::ShaderStage::compute || a_shader_stage == rhi::ShaderStage::compute_vertex || a_shader_stage == rhi::ShaderStage::compute_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.sampler(*this, a_index);
	else
	{
		assert(0 && "Can't bind texture image to this shader stage");
	}
}

void TextureImageMetal::upload(const rhi::Device &a_device)
{
	if (this->width() == 0 || this->height() == 0)
	{
		ror::log_critical("Uploading a texture of zero width or height texture name = {}, width x height=({}, {})",
		                  this->name().c_str(), this->width(), this->height());
		return;
	}

	MTL::Device            *device             = a_device.platform_device();
	MTL::TextureDescriptor *texture_descriptor = MTL::TextureDescriptor::alloc()->init();
	MTL::Origin             texture_origin{0, 0, 0};
	MTL::Size               size{this->width(), this->height(), this->depth()};
	bool                    is_array = is_texture_array(this->target());
	bool                    is_cube  = is_texture_cubemap(this->target());
	// bool                    is_3d    = this->target() == rhi::TextureTarget::texture_3D; // TODO: Workout what you need to do about this

	assert(device);

	// For mipmapping to work, you need 3 things to be setup correctly
	// 1. All mip-levels properly setup either manually or via blit encoder
	// 2. Have mip levels setup correctly in the texture descriptor
	// 3. Have a mipmapped sampler setup and attached to the shader

	texture_descriptor->setWidth(this->width());
	texture_descriptor->setHeight(this->height());
	texture_descriptor->setPixelFormat(to_metal_pixelformat(this->format()));
	texture_descriptor->setTextureType(to_metal_texture_target(this->target()));
	texture_descriptor->setMipmapLevelCount(this->levels());
	texture_descriptor->setUsage(MTL::TextureUsageUnknown);

	bool needs_upload = true;

	if (this->usage() == rhi::TextureUsage::shader_read || this->usage() == rhi::TextureUsage::render_target_read)
		texture_descriptor->setUsage(MTL::TextureUsageShaderRead);

	if (this->usage() == rhi::TextureUsage::render_target || this->usage() == rhi::TextureUsage::render_target_read)
	{
		texture_descriptor->setUsage(texture_descriptor->usage() | MTL::TextureUsageRenderTarget);
		needs_upload = false;
	}

	if (this->usage() == rhi::TextureUsage::shader_write)
	{
		texture_descriptor->setUsage(texture_descriptor->usage() | MTL::TextureUsageShaderWrite | MTL::TextureUsageShaderRead);        // Implied read like the definition of shader_write
		needs_upload = false;
	}

	texture_descriptor->setStorageMode(MTL::StorageModePrivate);

	if (this->m_texture)
		this->m_texture->release();

	this->m_texture = device->newTexture(texture_descriptor);

	if (needs_upload || this->data())
	{
		auto &last_mip  = this->mips().back();
		auto &first_mip = this->mips()[0];
		(void) last_mip;
		(void) first_mip;

		assert(this->data());
		if (this->mip_gen_mode() == rhi::TextureMipGenMode::manual && this->mipmapped())
		{
			assert(this->size() == ((last_mip.m_width * last_mip.m_height * last_mip.m_depth * this->bytes_per_pixel()) + last_mip.m_offset) && "Image size doesn't match the expected texture size");
		}
		else
		{
			if (is_cube)
			{
				assert(this->size() == ((last_mip.m_width * last_mip.m_height * last_mip.m_depth * this->bytes_per_pixel()) + last_mip.m_offset) && "Image size doesn't match the expected texture size");
			}
			else
			{
				assert(this->size() == (first_mip.m_width * first_mip.m_height * first_mip.m_depth * this->bytes_per_pixel()) && "Image size doesn't match the expected texture size");
			}
		}

		MTL::CommandQueue       *queue                = a_device.platform_queue();
		MTL::Buffer             *source_buffer        = device->newBuffer(this->data(), this->size(), MTL::ResourceStorageModeShared);
		MTL::CommandBuffer      *command_buffer       = queue->commandBuffer();
		MTL::BlitCommandEncoder *blit_command_encoder = command_buffer->blitCommandEncoder();

		assert(queue);
		assert(this->mip_gen_mode() == rhi::TextureMipGenMode::automatic || this->mip_gen_mode() == rhi::TextureMipGenMode::manual && "Not supported mip gen mode");

		size_t layers{is_array ? this->depth() : 1};
		size_t faces{is_cube ? 6ul : 1ul};
		size_t levels{this->mip_gen_mode() == rhi::TextureMipGenMode::manual ? this->levels() : 1u};        // If Gen mode is manual, expect data to be ready
		size_t level_index{0};

		assert(layers == 1 && "Don't support array textured yet, implement me");

		for (size_t layer = 0; layer < layers; ++layer)
		{
			for (size_t face = 0; face < faces; ++face)
			{
				for (size_t level = 0; level < levels; ++level)
				{
					auto &miplevel = this->mips()[level_index];

					size = MTL::Size{miplevel.m_width, miplevel.m_height, miplevel.m_depth};
					blit_command_encoder->copyFromBuffer(source_buffer, miplevel.m_offset, miplevel.m_width * this->bytes_per_pixel(), miplevel.m_width * miplevel.m_height * this->bytes_per_pixel(), size,
					                                     this->m_texture, face, level, texture_origin);

					++level_index;
				}
			}
		}

		if (this->mipmapped() && this->mip_gen_mode() == rhi::TextureMipGenMode::automatic && this->levels() > 1)
			blit_command_encoder->generateMipmaps(this->m_texture);

		blit_command_encoder->endEncoding();

		command_buffer->addCompletedHandler([this, source_buffer](MTL::CommandBuffer *) {this->ready(true); source_buffer->release(); });
		command_buffer->commit();
	}
	else
		this->ready(true);

	texture_descriptor->release();
}

void TextureSamplerMetal::upload(const rhi::Device &a_device)
{
	MTL::Device            *device             = a_device.platform_device();
	MTL::SamplerDescriptor *sampler_descriptor = MTL::SamplerDescriptor::alloc()->init();

	sampler_descriptor->setSAddressMode(to_metal_texture_address_mode(this->wrap_s()));
	sampler_descriptor->setTAddressMode(to_metal_texture_address_mode(this->wrap_t()));
	sampler_descriptor->setRAddressMode(to_metal_texture_address_mode(this->wrap_u()));
	sampler_descriptor->setMinFilter(to_metal_texture_filter(this->min_filter()));
	sampler_descriptor->setMagFilter(to_metal_texture_filter(this->mag_filter()));
	sampler_descriptor->setMipFilter(to_metal_texture_mip_filter(this->mip_mode()));
	sampler_descriptor->setBorderColor(to_metal_texture_border(this->border_color()));

	this->m_sampler = device->newSamplerState(sampler_descriptor);

	sampler_descriptor->release();
}

define_translation_unit_vtable(TextureImageMetal)
{}

define_translation_unit_vtable(TextureSamplerMetal)
{}

define_translation_unit_vtable(TextureMetal)
{}

}        // namespace rhi
