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

#include "math/rormatrix4.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorvertex_description.hpp"

namespace ror
{
class ROAR_ENGINE_ITEM Skin
{
  public:
	FORCE_INLINE       Skin()                        = default;             //! Default constructor
	FORCE_INLINE       Skin(const Skin &a_other)     = default;             //! Copy constructor
	FORCE_INLINE       Skin(Skin &&a_other) noexcept = default;             //! Move constructor
	FORCE_INLINE Skin &operator=(const Skin &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Skin &operator=(Skin &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Skin() noexcept                         = default;        //! Destructor

	std::vector<uint32_t, rhi::BufferAllocator<uint32_t>> m_joints{};                       //! All the joints in this skeleton
	std::vector<Matrix4f, rhi::BufferAllocator<Matrix4f>> m_inverse_bind_matrices{};        //! Inverse bind matrices for each joint in an array
	std::vector<Matrix4f, rhi::BufferAllocator<Matrix4f>> m_joint_matrices{};               //! Scratch space for array of calculated matrices every frame
	int32_t                                               m_root{-1};                       //! Node index of each skin, should be init with -1
	int32_t                                               m_node_index{-1};                 //! Node index as well where the each skin is attached, should be init with -1
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
