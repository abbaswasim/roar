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
#include "rhi/metal/rordevice.hpp"
#include "rhi/metal/rormetal_common.hpp"
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

namespace rhi
{
constexpr FORCE_INLINE MTL::TextureType to_metal_texture_target(rhi::TextureTarget a_target)
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

constexpr FORCE_INLINE MTL::SamplerMinMagFilter to_metal_texture_filter(rhi::TextureFilter a_filter)
{
	return static_cast<MTL::SamplerMinMagFilter>(a_filter);
}

constexpr FORCE_INLINE MTL::SamplerMipFilter to_metal_texture_mip_filter(rhi::TextureMipFilter a_filter)
{
	return static_cast<MTL::SamplerMipFilter>(a_filter);
}

constexpr FORCE_INLINE MTL::SamplerBorderColor to_metal_texture_border(rhi::TextureBorder a_color)
{
	return static_cast<MTL::SamplerBorderColor>(a_color);
}

constexpr FORCE_INLINE MTL::SamplerAddressMode to_metal_texture_address_mode(rhi::TextureAddressMode a_mode)
{
	return static_cast<MTL::SamplerAddressMode>(a_mode);
}

void TextureImageMetal::upload(rhi::Device &a_device)
{
	MTL::Device            *device             = a_device.platform_device();
	MTL::TextureDescriptor *texture_descriptor = MTL::TextureDescriptor::alloc()->init();
	auto                    tex_bpp            = this->bytes_per_pixel();
	uint32_t                bytes_per_row      = tex_bpp * this->width();
	MTL::Origin             texture_origin{0, 0, 0};
	MTL::Size               size{this->width(), this->height(), this->depth()};

	assert(device);

	if (this->width() == 0 || this->height() == 0)
	{
		ror::log_critical("Uploading a texture of zero width or height texture name = {}, width x height=({}, {})",
		                  this->name().c_str(), this->width(), this->height());
		return;
	}

	texture_descriptor->setWidth(this->width());
	texture_descriptor->setHeight(this->height());
	texture_descriptor->setPixelFormat(to_metal_pixelformat(this->format()));
	texture_descriptor->setTextureType(to_metal_texture_target(this->target()));
	texture_descriptor->setMipmapLevelCount(this->mips().size());
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
		texture_descriptor->setUsage(texture_descriptor->usage() | MTL::TextureUsageShaderWrite);
		needs_upload = false;
	}

	texture_descriptor->setStorageMode(MTL::StorageModePrivate);

	this->m_texture = device->newTexture(texture_descriptor);

	if (needs_upload)
	{
		assert(this->data());
		assert(this->size() == this->width() * this->height() * this->bytes_per_pixel());

		MTL::CommandQueue       *queue                = a_device.platform_queue();
		MTL::Buffer             *source_buffer        = device->newBuffer(this->data(), this->size(), MTL::ResourceStorageModeShared);
		MTL::CommandBuffer      *command_buffer       = queue->commandBuffer();
		MTL::BlitCommandEncoder *blit_command_encoder = command_buffer->blitCommandEncoder();

		assert(queue);

		blit_command_encoder->copyFromBuffer(source_buffer, 0, bytes_per_row, this->width() * this->height() * this->bytes_per_pixel(), size, this->m_texture, 0, 0, texture_origin);
		blit_command_encoder->endEncoding();

		command_buffer->addCompletedHandler([this](MTL::CommandBuffer *) { this->ready(true); });
		command_buffer->commit();
	}
	else
		this->ready(true);

	texture_descriptor->release();
}

void TextureSamplerMetal::upload(rhi::Device &a_device)
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
