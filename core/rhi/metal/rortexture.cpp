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

#include "profiling/rorlog.hpp"
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
#include <Metal/MTLTexture.hpp>
#include <Metal/MTLTypes.hpp>

namespace rhi
{

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
	texture_descriptor->setPixelFormat(mtl::to_metal_pixelformat(this->format()));
	texture_descriptor->setTextureType(mtl::to_metal_texture_target(this->target()));
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

#if 1
	// Enable the managed route for now because of bug in BigSur
	if (is_pixel_format_depth_format(this->format()))
		texture_descriptor->setStorageMode(MTL::StorageModePrivate);
	else
		texture_descriptor->setStorageMode(MTL::StorageModeManaged);

	this->m_texture = device->newTexture(texture_descriptor);

	if (needs_upload)
	{
		MTL::Region region{0, 0, 0, this->width(), this->height(), 1};
		this->m_texture->replaceRegion(region, 0, this->data(), bytes_per_row);
		this->ready(true);
	}
#else

	texture_descriptor->setStorageMode(MTL::StorageModePrivate);

	this->m_texture = device->newTexture(texture_descriptor);

	if (needs_upload)
	{
		MTL::CommandQueue *queue = rhi_device->platform_queue();
		assert(queue);

		assert(this->data());
		assert(this->size() == this->width() * this->height() * this->bytes_per_pixel());

		MTL::Buffer *source_buffer = device->newBuffer(this->data(), this->size(), MTL::ResourceStorageModeShared);
		(void) source_buffer;

		MTL::CommandBuffer *command_buffer = queue->commandBuffer();

		MTL::BlitCommandEncoder *blit_command_encoder = command_buffer->blitCommandEncoder();

		// copyFromBuffer is hanging the whole System and rebooting the machine
		// This is a bug in BigSur it works fine on Monterey M1
		blit_command_encoder->copyFromBuffer(source_buffer, 0, bytes_per_row, this->width() * this->height() * this->bytes_per_pixel(), size, this->m_texture, 0, 0, texture_origin);
		blit_command_encoder->endEncoding();

		command_buffer->addCompletedHandler([this](MTL::CommandBuffer *) { this->ready(true); });
		command_buffer->commit();
	}

#endif

	texture_descriptor->release();
}

define_translation_unit_vtable(TextureImageMetal)
{}

define_translation_unit_vtable(TextureSamplerMetal)
{}

define_translation_unit_vtable(TextureMetal)
{}

}        // namespace rhi
