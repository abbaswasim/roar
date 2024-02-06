// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2024
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
#include "foundation/rortypes.hpp"
#include "rhi/vulkan/rordescriptor_cache.hpp"
#include "rhi/vulkan/rordescriptor_pool.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include <cstddef>
#include <vector>

/*
// Roar uses the following sets and bindings in rough order, this is from low frequency to high frequency for a frequencey based descriptor set design

layout(set = 0, binding = 0) uniform per_frame_uniform;
layout(set = 0, binding = 1) buffer  nodes_models;                                   // Nodes model matrix updated once per frame
layout(set = 0, binding = 2) uniform nodes_offsets;                                  // Nodes offset goes hand in hand with nodes_models, this is the index of the matrix in the array
layout(set = 0, binding = 3) uniform sampler2D shadow_map_sampler;
layout(set = 0, binding = 4) uniform sampler2D brdf_integration_sampler;
layout(set = 0, binding = 5) uniform samplerCube skybox_sampler;
layout(set = 0, binding = 6) uniform samplerCube irradiance_sampler;
layout(set = 0, binding = 7) uniform samplerCube radiance_sampler;
layout(set = 0, binding = 8) uniform directional_light_uniform;
layout(set = 0, binding = 9) uniform point_light_uniform;
layout(set = 0, binding = 10) uniform spot_light_uniform;
layout(set = 0, binding = 11) buffer morphs_weights;                                  // All the morph weights in the scene are uploaded at once, hence per frame

layout(set = 1, binding = 0) uniform per_view_uniform;

layout(set = 2, binding = 0) buffer joint_offset_uniform
layout(set = 2, binding = 1) buffer joint_inverse_bind_matrices

layout(set = 3, binding = 0) uniform material_factors;
layout(set = 3, binding = 1) uniform highp sampler2D base_color_sampler;
layout(set = 3, binding = 2) uniform highp sampler2D emissive_sampler;
layout(set = 3, binding = 3) uniform highp sampler2D metallic_sampler;
layout(set = 3, binding = 4) uniform highp sampler2D roughness_sampler;
layout(set = 3, binding = 5) uniform highp sampler2D occlusion_sampler;
layout(set = 3, binding = 6) uniform highp sampler2D normal_sampler;
*/

namespace rhi
{
class DescriptorSet final
{
  public:
	FORCE_INLINE                DescriptorSet()                                 = default;        //! Copy constructor
	FORCE_INLINE                DescriptorSet(const DescriptorSet &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                DescriptorSet(DescriptorSet &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE DescriptorSet &operator=(const DescriptorSet &a_other)         = delete;         //! Copy assignment operator
	FORCE_INLINE DescriptorSet &operator=(DescriptorSet &&a_other) noexcept     = delete;         //! Move assignment operator
	FORCE_INLINE ~DescriptorSet() noexcept                                      = default;

	void push_image(uint32_t a_binding, VkDescriptorImageInfo *a_image_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags = 0);
	void push_buffer(uint32_t a_binding, VkDescriptorBufferInfo *a_buffer_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags = 0);
	void allocate(const VkDevice a_device, DescriptorSetLayoutCache &a_factory, DescriptorPool &a_pool);
	void update(const VkDevice a_device);
	// void free(const VkDevice a_device); // Not provided because bad practice, would rather free the pool directly

  protected:
  private:
	void push_binding(uint32_t a_binding, VkDescriptorImageInfo *a_image_info, VkDescriptorBufferInfo *a_buffer_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags);

	std::vector<VkDescriptorSetLayoutBinding> m_bindings{};             //! All the bindings that makes up this descriptor
	std::vector<VkWriteDescriptorSet>         m_writes{};               //! All the writes that would need to performed when updating
	VkDescriptorSet                           m_handle{nullptr};        //! Handle to the descriptor set

	// VkDescriptorPool                          m_pool{nullptr};          //! Handle to the pool from where we allocated this descriptor, required if free is implemented
};

}        // namespace rhi

#include "rhi/vulkan/rordescriptor_set.hh"
