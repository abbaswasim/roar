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
#include "profiling/rorlog.hpp"
#include "rhi/vulkan/rorcommand_buffer.hpp"
#include "rhi/rorrender_command_encoder.hpp"
#include "rhi/rorcompute_command_encoder.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorrhi_macros.hpp"

namespace rhi
{

FORCE_INLINE rhi::RenderCommandEncoder CommandBuffer::render_encoder(rhi::Renderpass &a_render_pass, uint32_t a_index)
{
	(void) a_render_pass;
	(void) a_index;

	ror::log_critical("Fix me, I shouldnt be using default ctor here {}", __FUNCTION__);

	return rhi::RenderCommandEncoder{};
}

FORCE_INLINE rhi::ComputeCommandEncoder CommandBuffer::compute_encoder(rhi::Renderpass &a_render_pass, uint32_t a_index)
{
	(void) a_render_pass;
	(void) a_index;

	ror::log_critical("Fix me, I shouldnt be using default ctor here {}", __FUNCTION__);
	return rhi::ComputeCommandEncoder{};
}

rhi::ComputeCommandEncoder CommandBuffer::compute_encoder()
{
	// return rhi::ComputeCommandEncoder{this->m_buffer->computeCommandEncoder()};
	return rhi::ComputeCommandEncoder{};
}

// This means all commands in the command buffer can be executed in parallel
rhi::ComputeCommandEncoder CommandBuffer::compute_encoder_concurrent()
{
	// return rhi::ComputeCommandEncoder{this->m_buffer->computeCommandEncoder(MTL::DispatchType::DispatchTypeConcurrent)};
	return rhi::ComputeCommandEncoder{};
}

// Very heavy waight glReadPixel style read pixels, shouldn't be used at run time
rhi::Buffer read_pixels(rhi::Device &a_device, const rhi::TextureImage &a_texture, uint32_t a_face, uint32_t a_level)
{
	(void) a_face;

	rhi::CommandBuffer  command_buffer{a_device};
	// MTL::CommandBuffer *cmd_buffer = command_buffer.platform_command_buffer();

	auto mip = a_texture.mip(a_level);

	auto width  = mip.m_width;
	auto height = mip.m_height;

	auto bytes_per_pixel = a_texture.bytes_per_pixel();
	auto bytes_per_row   = width * bytes_per_pixel;
	auto bytes_per_image = height * bytes_per_row;

	rhi::Buffer read_buffer;
	read_buffer.init(a_device, bytes_per_image);

	// auto blit_encoder = cmd_buffer->blitCommandEncoder();

	// blit_encoder->copyFromTexture(a_texture.platform_handle(), a_face, a_level, MTL::Origin(0, 0, 0), MTL::Size(width, height, 1), read_buffer.platform_buffer(), 0, bytes_per_row, bytes_per_image);
	// blit_encoder->endEncoding();

	// // TODO: process the pixels in a completion handler instead of blocking here
	// cmd_buffer->addCompletedHandler([](MTL::CommandBuffer *) {});

	// cmd_buffer->commit();
	// cmd_buffer->waitUntilCompleted();        // Heavy wait wait before we can read data from read_buffer.

	return read_buffer;
}

// Very heavy waight texture to texture copy, shouldn't be used at run time
void texture_to_texture(rhi::Device &a_device, const rhi::TextureImage &a_source_texture, const rhi::TextureImage &a_destination_texture, uint32_t a_face, uint32_t a_level)
{
	(void) a_source_texture;
	(void) a_destination_texture;
	(void) a_face;
	(void) a_level;

	rhi::CommandBuffer  command_buffer{a_device};
	// MTL::CommandBuffer *cmd_buffer = command_buffer.platform_command_buffer();

	// auto        mip    = a_source_texture.mip(a_level);
	// auto        width  = mip.m_width;
	// auto        height = mip.m_height;
	// MTL::Origin origin{0, 0, 0};
	// MTL::Size   size{width, height, 1};

	// auto blit_encoder = cmd_buffer->blitCommandEncoder();

	// blit_encoder->copyFromTexture(a_source_texture.platform_handle(), a_face, a_level, origin, size, a_destination_texture.platform_handle(), a_face, a_level, origin);
	// blit_encoder->endEncoding();

	// cmd_buffer->addCompletedHandler([](MTL::CommandBuffer *) {});

	// cmd_buffer->commit();
	// cmd_buffer->waitUntilCompleted();        // Heavy wait wait before we can read data from read_buffer.
}

void texture_to_mipmapped_texture(const rhi::CommandBuffer &a_command_buffer, const rhi::TextureImage &a_source_texture, ror::Vector2ui a_source_origin, const rhi::TextureImage &a_destination_texture, uint32_t a_destination_face, uint32_t a_destination_level)
{
	(void) a_command_buffer;
	(void) a_source_texture;
	(void) a_source_origin;
	(void) a_destination_texture;
	(void) a_destination_face;
	(void) a_destination_level;

	// MTL::CommandBuffer *cmd_buffer = a_command_buffer.platform_command_buffer();

	// auto        mip = a_destination_texture.mip(a_destination_level);
	// MTL::Size   size{mip.m_width, mip.m_height, 1};
	// MTL::Origin source_origin{a_source_origin.x, a_source_origin.y, 0};
	// MTL::Origin destination_origin{0, 0, 0};

	// auto blit_encoder = cmd_buffer->blitCommandEncoder();

	// blit_encoder->copyFromTexture(a_source_texture.platform_handle(), 0, 0, source_origin, size, a_destination_texture.platform_handle(), a_destination_face, a_destination_level, destination_origin);
	// blit_encoder->endEncoding();
}

void texture_to_mipmapped_texture(void *a_blit_encoder, const rhi::TextureImage &a_source_texture, ror::Vector2ui a_source_origin, const rhi::TextureImage &a_destination_texture, uint32_t a_destination_face, uint32_t a_destination_level)
{
	(void) a_blit_encoder;
	(void) a_source_texture;
	(void) a_source_origin;
	(void) a_destination_texture;
	(void) a_destination_face;
	(void) a_destination_level;

	// auto        mip = a_destination_texture.mip(a_destination_level);
	// MTL::Size   size{mip.m_width, mip.m_height, 1};
	// MTL::Origin source_origin{a_source_origin.x, a_source_origin.y, 0};
	// MTL::Origin destination_origin{0, 0, 0};

	// a_blit_encoder->copyFromTexture(a_source_texture.platform_handle(), 0, 0, source_origin, size, a_destination_texture.platform_handle(), a_destination_face, a_destination_level, destination_origin);
}

void texture_patch_to_mipmapped_cubemap_texture(rhi::Device &a_device, const rhi::TextureImage &a_source_texture, const rhi::TextureImage &a_destination_texture)
{
	(void) a_source_texture;
	(void) a_destination_texture;

	// auto w = a_destination_texture.width();
	// auto h = a_destination_texture.height();

	assert(rhi::is_texture_cubemap(a_destination_texture.target()) && "Can't copy from patch to non-cubemap texture");

	rhi::CommandBuffer  command_buffer{a_device};
	// MTL::CommandBuffer *platform_command_buffer = command_buffer.platform_command_buffer();
	// auto                blit_encoder            = platform_command_buffer->blitCommandEncoder();

	// auto mip_levels_count = rhi::calculate_texture_mip_levels(w, h, 1);

	// ror::Vector2ui offset{0, 0};

	// for (uint32_t level = 0; level < mip_levels_count; ++level)
	// {
	// 	auto mip_w = std::max(1u, w >> level);
	// 	auto mip_h = std::max(1u, h >> level);

	// 	rhi::texture_to_mipmapped_texture(blit_encoder, a_source_texture, {mip_w + offset.x, 0         + offset.y}, a_destination_texture, 0, level);
	// 	rhi::texture_to_mipmapped_texture(blit_encoder, a_source_texture, {0     + offset.x, 0         + offset.y}, a_destination_texture, 1, level);
	// 	rhi::texture_to_mipmapped_texture(blit_encoder, a_source_texture, {0     + offset.x, mip_h     + offset.y}, a_destination_texture, 2, level);
	// 	rhi::texture_to_mipmapped_texture(blit_encoder, a_source_texture, {0     + offset.x, 2 * mip_h + offset.y}, a_destination_texture, 3, level);
	// 	rhi::texture_to_mipmapped_texture(blit_encoder, a_source_texture, {0     + offset.x, 3 * mip_h + offset.y}, a_destination_texture, 4, level);
	// 	rhi::texture_to_mipmapped_texture(blit_encoder, a_source_texture, {mip_w + offset.x, 3 * mip_h + offset.y}, a_destination_texture, 5, level);

	// 	offset.x += mip_w;
	// 	offset.y += mip_h;
	// }

	// blit_encoder->endEncoding();

	// platform_command_buffer->commit();
	// platform_command_buffer->waitUntilCompleted();

	// blit_encoder->release();
	// command_buffer->release();
}
}        // namespace rhi
