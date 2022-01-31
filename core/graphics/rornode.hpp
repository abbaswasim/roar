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
#include "math/rorquaternion.hpp"
#include "math/rortransform.hpp"
#include "rhi/rorbuffer_allocator.hpp"

namespace ror
{

class ROAR_ENGINE_ITEM Node
{
  public:
	FORCE_INLINE       Node()                        = default;             //! Default constructors
	FORCE_INLINE       Node(const Node &a_other)     = default;             //! Copy constructor
	FORCE_INLINE       Node(Node &&a_other) noexcept = default;             //! Move constructor
	FORCE_INLINE Node &operator=(const Node &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Node &operator=(Node &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Node() noexcept                         = default;        //! Destructor

	int32_t    m_parent{-1};             //! Parent of the nodes, should be init with -1
	int32_t    m_mesh_index{-1};         //! Mesh index of the nodes, should be init with -1
	int32_t    m_skin_index{-1};         //! Skin index, not node index, should be init with -1
	Transformf m_trs_transform{};        //! Contains translation vec3, Rotation quaternion and scale vec3
};

/**
 * This is just some side data that is maintained in parallel for editor and debugging
 * Since this data shouldn't be send over to the GPU directly via buffers its separate from Node
 * This needs to be maintained in some highlevel construct in parallel, not within another type along size Node
 * That will defeat the purpose of keeping it separate
 */
class ROAR_ENGINE_ITEM NodeData
{
  public:
	FORCE_INLINE           NodeData()                            = default;         //! Default constructors
	FORCE_INLINE           NodeData(const NodeData &a_other)     = default;         //! Copy constructor
	FORCE_INLINE           NodeData(NodeData &&a_other) noexcept = default;         //! Move constructor
	FORCE_INLINE NodeData &operator=(const NodeData &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE NodeData &operator=(NodeData &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~NodeData() noexcept                             = default;        //! Destructor

	// Editor only and for debugging purposes
	// NOTE: Using std::vectors
	std::string           m_name{};
	std::vector<uint32_t> m_children{};        //! All the list of childrens for each node
};

}        // namespace ror

namespace rhi
{
// Define type to semantic for buffers allocator
define_type_to_shader_semantics(ror::Node)
{
	return rhi::BufferSemantic::node_data;
}

define_type_to_shader_semantics(ror::NodeData)
{
	return rhi::BufferSemantic::node_suplementory_data;
}

define_type_to_shader_semantics(ror::Matrix4f)
{
	return rhi::BufferSemantic::instance_transform;
}

define_type_to_shader_semantics(ror::Transformf)
{
	return rhi::BufferSemantic::instance_trs;
}

}        // namespace rhi
