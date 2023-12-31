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
#include "rhi/rorbuffer.hpp"
#include "rhi/rorcommand_buffer.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/rortexture.hpp"

#include <Metal/MTLCommandBuffer.hpp>

namespace rhi
{
class RenderpassMetal;

declare_rhi_render_type(Renderpass);
declare_rhi_render_type(RenderCommandEncoder);
declare_rhi_render_type(ComputeCommandEncoder);

class CommandBufferMetal final
{
  public:
	FORCE_INLINE                     CommandBufferMetal(const CommandBufferMetal &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                     CommandBufferMetal(CommandBufferMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE CommandBufferMetal &operator=(const CommandBufferMetal &a_other)              = default;        //! Copy assignment operator
	FORCE_INLINE CommandBufferMetal &operator=(CommandBufferMetal &&a_other) noexcept          = default;        //! Move assignment operator
	FORCE_INLINE ~CommandBufferMetal() noexcept                                                = default;        //! Destructor

	FORCE_INLINE explicit CommandBufferMetal(rhi::Device &a_device)
	{
		this->m_buffer = a_device.platform_command_buffer();
	}

	FORCE_INLINE constexpr MTL::CommandBuffer *platform_command_buffer() const noexcept
	{
		return this->m_buffer;
	}

	FORCE_INLINE constexpr void present_drawable(rhi::Swapchain a_swapchain) noexcept
	{
		this->m_buffer->presentDrawable(a_swapchain);
	}

	FORCE_INLINE constexpr void commit() noexcept
	{
		this->m_buffer->commit();
	}

	FORCE_INLINE constexpr void wait_until_completed() noexcept
	{
		this->m_buffer->waitUntilCompleted();
	}

	FORCE_INLINE constexpr void release() const noexcept
	{
		this->m_buffer->release();
	}

	FORCE_INLINE void addCompletedHandler(const std::function<void()> &a_function)
	{
		this->m_buffer->addCompletedHandler([a_function](MTL::CommandBuffer *cmd_buffer) {
			a_function();
			(void) cmd_buffer;
			// cmd_buffer->release(); // Remember can't release here because multiple completed handlers could be defined which will result in multiple releases
		});
	}

	// These set of encoder getters might look redundant but they are not. Can be used without a renderpass
	rhi::RenderCommandEncoder  render_encoder(rhi::Renderpass &a_render_pass, uint32_t a_index);
	rhi::ComputeCommandEncoder compute_encoder(rhi::Renderpass &a_render_pass, uint32_t a_index);
	rhi::ComputeCommandEncoder compute_encoder();
	rhi::ComputeCommandEncoder compute_encoder_concurrent();

  protected:
  private:
	FORCE_INLINE CommandBufferMetal() = default;        //! Default constructor

	MTL::CommandBuffer *m_buffer{nullptr};
};

declare_rhi_render_type(CommandBuffer);

rhi::Buffer read_pixels(rhi::Device &a_device, rhi::TextureImage a_texture, uint32_t a_face, uint32_t a_level);
void        texture_to_texture(rhi::Device &a_device, rhi::TextureImage &a_source_texture, rhi::TextureImage &a_destination_texture, uint32_t a_face, uint32_t a_level);
void        texture_to_mipmapped_texture(const rhi::CommandBuffer &a_command_buffer, rhi::TextureImage &a_source_texture, ror::Vector2ui a_source_origin, rhi::TextureImage &a_destination_texture, uint32_t a_destination_face, uint32_t a_destination_level);
void        texture_patch_to_mipmapped_cubemap_texture(rhi::Device &a_device, rhi::TextureImage &a_source_texture, rhi::TextureImage &a_destination_texture);
}        // namespace rhi
