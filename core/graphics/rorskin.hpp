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
	FORCE_INLINE constexpr auto  joints_count() const noexcept   { return this->m_joints.size();                 }
	FORCE_INLINE constexpr auto &shader_buffer() const noexcept  { return this->m_joint_transform_shader_buffer; }
	// clang-format on

	void update()
	{
		this->m_joint_transform_shader_buffer.buffer_map();

		auto stride = this->m_joint_transform_shader_buffer.stride("joint_transforms");

		for (size_t joint_id = 0; joint_id < this->m_joint_transforms.size(); ++joint_id)
		{
			auto &xform = this->m_joint_transforms[joint_id];
			this->m_joint_transform_shader_buffer.update("rotation", &xform.m_rotation.x, static_cast<uint32_t>(joint_id), stride);
			this->m_joint_transform_shader_buffer.update("translation", &xform.m_translation.x, static_cast<uint32_t>(joint_id), stride);
			this->m_joint_transform_shader_buffer.update("scale", &xform.m_scale.x, static_cast<uint32_t>(joint_id), stride);
		}

		this->m_joint_transform_shader_buffer.buffer_unmap();
	}

	void upload(rhi::Device &a_device)
	{
		// Needs to create something like the following
		/*
		  const uint joints_count = 24;
		  struct trs_transform
		  {
		      vec4 rotation;
		      vec3 translation;
		      vec3 scale;
		  };
		  layout(std140, set = 2, binding = 0) uniform joint_transform
		  {
		      trs_transform joint_transforms[joints_count];
		  } in_joint_transforms;
		*/
		rhi::ShaderBufferTemplate::Struct trs_transform("joint_transforms", static_cast_safe<uint32_t>(this->joints_count()));
		trs_transform.add_entry("rotation", rhi::Format::float32_4, rhi::Layout::std140, 1);
		trs_transform.add_entry("translation", rhi::Format::float32_3, rhi::Layout::std140, 1);
		trs_transform.add_entry("scale", rhi::Format::float32_3, rhi::Layout::std140, 1);

		this->m_joint_transform_shader_buffer.add_struct(trs_transform);

		this->m_joint_transform_shader_buffer.upload(a_device);

		this->update();
	}

	std::vector<uint32_t, rhi::BufferAllocator<uint32_t>>     m_joints{};                               //! All the joints in this skeleton
	std::vector<Matrix4f, rhi::BufferAllocator<Matrix4f>>     m_inverse_bind_matrices{};                //! Inverse bind matrices for each joint in an array
	std::vector<Transformf, rhi::BufferAllocator<Transformf>> m_joint_transforms{};                     //! Scratch space for array of calculated transforms every frame
	int32_t                                                   m_root{-1};                               //! Node index of each skin, should be init with -1
	int32_t                                                   m_node_index{-1};                         //! Node index as well where the each skin is attached, should be init with -1
	rhi::ShaderBuffer                                         m_joint_transform_shader_buffer{"joint_transforms_uniform", rhi::ShaderBufferType::ubo, rhi::Layout::std140, 0, 1};        //! ShaderBuffers for joint_transforms within the skinning shader
};

}        // namespace ror

namespace rhi
{
// Define type to semantic for buffers allocator
define_type_to_shader_semantics(ror::Skin)
{
	return rhi::BufferSemantic::skin_data;
}

define_type_to_shader_semantics(uint32_t)
{
	return rhi::BufferSemantic::joint_index_data;
}

}        // namespace rhi
