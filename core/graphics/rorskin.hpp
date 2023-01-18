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

#include "foundation/rorutilities.hpp"
#include "math/rormatrix4.hpp"
#include "math/rorquaternion.hpp"
#include "math/rortransform.hpp"
#include "math/rorvector4.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "settings/rorsettings.hpp"
#include <cstddef>

namespace ror
{
class ROAR_ENGINE_ITEM Skin
{
  public:
	FORCE_INLINE       Skin()                             = default;        //! Default constructor
	FORCE_INLINE       Skin(const Skin &a_other)          = delete;         //! Copy constructor
	FORCE_INLINE       Skin(Skin &&a_other) noexcept      = default;        //! Move constructor
	FORCE_INLINE Skin &operator=(const Skin &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE Skin &operator=(Skin &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~Skin() noexcept                         = default;        //! Destructor

	// clang-format off
	FORCE_INLINE constexpr auto joints_count()               const noexcept { return this->m_joints.size();         }
	FORCE_INLINE constexpr auto root()                       const noexcept { return this->m_root;                  }
	FORCE_INLINE constexpr auto node_index()                 const noexcept { return this->m_node_index;            }
	FORCE_INLINE constexpr auto root(int32_t a_root)               noexcept { this->m_root = a_root;                }
	FORCE_INLINE constexpr auto node_index(int32_t a_index)        noexcept { this->m_node_index = a_index;         }
	FORCE_INLINE constexpr auto &inverse_bind_matrices()           noexcept { return this->m_inverse_bind_matrices; }
	FORCE_INLINE constexpr auto &joints()                    const noexcept { return this->m_joints;                }
	FORCE_INLINE constexpr auto &joints()                          noexcept { return this->m_joints;                }

	template<typename _encoder_type>
	FORCE_INLINE constexpr void bind_joint_offset_buffer( _encoder_type& a_encoder, rhi::ShaderStage a_stage)
		                                                { this->m_joint_offset_shader_buffer.buffer_bind(a_encoder, a_stage); }
	template<typename _encoder_type>
	FORCE_INLINE constexpr void bind_inverse_bind_buffer( _encoder_type& a_encoder, rhi::ShaderStage a_stage)
		                                                { this->m_inverse_bind_shader_buffer.buffer_bind(a_encoder, a_stage); }
	// clang-format on

	void update()
	{
		auto stride = this->m_joint_offset_shader_buffer.stride("joint_redirect");
		this->m_joint_offset_shader_buffer.buffer_map();
		for (size_t joint_id = 0; joint_id < this->m_joints.size(); ++joint_id)
			this->m_joint_offset_shader_buffer.update("joint_redirect", &this->m_joints[joint_id], static_cast<uint32_t>(joint_id), stride);
		this->m_joint_offset_shader_buffer.buffer_unmap();

		this->m_inverse_bind_shader_buffer.buffer_map();
		this->m_inverse_bind_shader_buffer.update("joint_inverse_matrix", &this->m_inverse_bind_matrices[0].m_values, static_cast_safe<uint32_t>(this->m_joints.size() * sizeof(ror::Matrix4f)));
		this->m_inverse_bind_shader_buffer.buffer_unmap();
	}

	void upload(rhi::Device &a_device)
	{
		this->m_joint_offset_shader_buffer.add_entry("joint_redirect", rhi::Format::uint16_1, static_cast_safe<uint32_t>(this->m_joints.size()));
		this->m_joint_offset_shader_buffer.upload(a_device);

		this->m_inverse_bind_shader_buffer.add_entry("joint_inverse_matrix", rhi::Format::float32_4x4, static_cast_safe<uint32_t>(this->m_joints.size()));
		this->m_inverse_bind_shader_buffer.upload(a_device);

		this->update();
	}

  private:
	std::vector<uint16_t, rhi::BufferAllocator<uint16_t>> m_joints{};                       //! All the joints in this skeleton
	std::vector<Matrix4f, rhi::BufferAllocator<Matrix4f>> m_inverse_bind_matrices{};        //! Inverse bind matrices for each joint in an array
	int32_t                                               m_root{-1};                       //! Node index of each skin, should be init with -1
	int32_t                                               m_node_index{-1};                 //! Node index as well where the each skin is attached, should be init with -1

	// TODO: Remove the use of matrices and bring back TRS instead
	// TRS is currently unused, only matrices are used

	rhi::ShaderBuffer m_joint_offset_shader_buffer{"joint_offset_uniform",
	                                               rhi::ShaderBufferType::ssbo,
	                                               rhi::Layout::std430,
	                                               settings().skin_joints_set(),
	                                               settings().skin_joints_binding()};        //! ShaderBuffers for joint_offsets within the skinning shader

	rhi::ShaderBuffer m_inverse_bind_shader_buffer{"joint_inverse_bind_matrices",
	                                               rhi::ShaderBufferType::ubo,
	                                               rhi::Layout::std140,
	                                               settings().joint_inverse_bind_set(),
	                                               settings().joint_inverse_bind_binding()};        //! ShaderBuffers for inverse_bind_transforms within the skinning shader
};

}        // namespace ror

namespace rhi
{
// Define type to semantic for buffers allocator
define_type_to_shader_semantics(ror::Skin)
{
	return rhi::BufferSemantic::skin_data;
}

define_type_to_shader_semantics(uint16_t)
{
	return rhi::BufferSemantic::joint_index_data;
}

}        // namespace rhi
