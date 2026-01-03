// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021-2022
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

#include "camera/rorcamera.hpp"
#include "graphics/roranimation.hpp"
#include "graphics/rorlight.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"
#include "graphics/rornode.hpp"
#include "graphics/rorskin.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include <vector>

namespace ror
{
struct BinBuffer
{
	uint8_t m_byte;
};

using BinBuffer_Allocator     = rhi::BufferAllocator<BinBuffer>;
using BinBuffer_Vector        = std::vector<BinBuffer, BinBuffer_Allocator>;
using BinBuffer_Vector_Vector = std::vector<BinBuffer_Vector, rhi::BufferAllocator<BinBuffer_Vector>>;
using DefaultMeshData         = std::vector<std::vector<std::tuple<rhi::PrimitiveTopology, bool, int32_t>>>;        // Mesh data is per mesh for each primitive primitive topology, has_indices, material indices

class ROAR_ENGINE_ITEM Model final
{
  public:
	FORCE_INLINE        Model()                             = default;        //! Default constructor
	FORCE_INLINE        Model(const Model &a_other)         = default;        //! Copy constructor
	FORCE_INLINE        Model(Model &&a_other) noexcept     = default;        //! Move constructor
	FORCE_INLINE Model &operator=(const Model &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Model &operator=(Model &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Model() noexcept                          = default;        //! Destructor

	// Populates data from a gltf file loaded as a resource.
	// Takes scene cameras and lights as parameter and fills it up with this model cameras and lights
	// If this or any other load functions are called before, data needs to be appended
	void              load_from_gltf_file(std::filesystem::path a_filename, std::vector<ror::OrbitCamera> &a_cameras, std::vector<ror::Light> &a_lights, bool a_generate_shaders, rhi::BuffersPack &a_buffers_pack);
	void              create_default_mesh(const char *a_name, bool a_generate_shaders, DefaultMeshData a_mesh_data, std::vector<ror::Material> &&a_mesh_materials);
	void              create_grid(bool a_generate_shaders, rhi::BuffersPack &a_buffers_pack);
	void              create_debug(bool a_generate_shaders, std::vector<std::vector<float32_t>> &attributes_data, std::vector<rhi::PrimitiveTopology> a_topology, rhi::BuffersPack &a_buffers_pack);
	void              reset();
	void              upload(rhi::Device &a_device);
	ror::BoundingBoxf bounding_box_scaled(const ror::Matrix4f a_scene_xform);

	// clang-format off
	FORCE_INLINE constexpr auto &images()                 noexcept  {  return this->m_images;             }
	FORCE_INLINE constexpr auto &samplers()               noexcept  {  return this->m_samplers;           }
	FORCE_INLINE constexpr auto &textures()               noexcept  {  return this->m_textures;           }
	FORCE_INLINE constexpr auto &materials()              noexcept  {  return this->m_materials;          }
	FORCE_INLINE constexpr auto &meshes()                 noexcept  {  return this->m_meshes;             }
	FORCE_INLINE constexpr auto &images()           const noexcept  {  return this->m_images;             }
	FORCE_INLINE constexpr auto &samplers()         const noexcept  {  return this->m_samplers;           }
	FORCE_INLINE constexpr auto &textures()         const noexcept  {  return this->m_textures;           }
	FORCE_INLINE constexpr auto &materials()        const noexcept  {  return this->m_materials;          }
	FORCE_INLINE constexpr auto &meshes()           const noexcept  {  return this->m_meshes;             }
	FORCE_INLINE constexpr auto &skins()            const noexcept  {  return this->m_skins;              }
	FORCE_INLINE constexpr auto &skins()                  noexcept  {  return this->m_skins;              }
	FORCE_INLINE constexpr auto &nodes()            const noexcept  {  return this->m_nodes;              }
	FORCE_INLINE constexpr auto &nodes()                  noexcept  {  return this->m_nodes;              }
	FORCE_INLINE constexpr auto &nodes_side_data()  const noexcept  {  return this->m_nodes_side_data;    }
	FORCE_INLINE constexpr auto &nodes_side_data()        noexcept  {  return this->m_nodes_side_data;    }
	FORCE_INLINE constexpr auto &animations()       const noexcept  {  return this->m_animations;         }
	FORCE_INLINE constexpr auto &bounding_box()     const noexcept  {  return this->m_bounding_box;       }
	FORCE_INLINE constexpr auto &generate_shaders() const noexcept  {  return this->m_generate_shaders;   }
	// clang-format on

	void update_mesh_program(uint32_t a_mesh_index, uint32_t a_primitive_index, int32_t a_program_index);

  private:
	// This works for 1 model at the moment. Next append to these when a second asset is loaded
	// This would require going through all the models that needs loading and getting max bounds of each and then adjusting indices

	void update_hashes();

	std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>>     m_images{};                      //! All images, by handles
	std::vector<rhi::TextureSampler, rhi::BufferAllocator<rhi::TextureSampler>> m_samplers{};                    //! All samplers, by handles
	std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>               m_textures{};                    //! All textures by handles
	std::vector<ror::Material, rhi::BufferAllocator<ror::Material>>             m_materials{};                   //! All the materials in this asset
	std::vector<ror::Mesh, rhi::BufferAllocator<ror::Mesh>>                     m_meshes{};                      //! All the meshes in this asset
	std::vector<ror::Skin, rhi::BufferAllocator<ror::Skin>>                     m_skins{};                       //! All the skins we have
	std::vector<ror::Node, rhi::BufferAllocator<ror::Node>>                     m_nodes{};                       //! All the nodes in this asset
	std::vector<ror::NodeData, rhi::BufferAllocator<ror::NodeData>>             m_nodes_side_data{};             //! All the nodes parallel data that needs to be maintained
	std::vector<ror::Animation, rhi::BufferAllocator<ror::Animation>>           m_animations{};                  //! All the animations in this asset
	BinBuffer_Vector_Vector                                                     m_buffers{};                     //! All the buffers provided in gltf, only required temporarily
	ror::BoundingBoxf                                                           m_bounding_box{};                //! Model bounding box, a combination of its mesh in object space
	bool                                                                        m_generate_shaders{true};        //! Flag to determine if the model requires its shaders to be generated or these are provided,
	                                                                                                             //! For instanced models to many nodes this will generate shaders for all nodes
};

template <typename _node_container, typename _node_type>
auto get_node_global_transform(_node_container &a_nodes_container, _node_type &a_node)
{
	auto         &nodes = a_nodes_container.nodes();
	ror::Matrix4f node_matrix{a_node.m_trs_transform.matrix4f()};
	_node_type   *node = &a_node;

	while (node->m_parent != -1)
	{
		auto &parent = nodes[static_cast<size_t>(node->m_parent)];
		node_matrix  = parent.m_trs_transform.matrix4f() * node_matrix;

		node = &parent;
	}

	return node_matrix;
}

}        // namespace ror

namespace rhi
{
// Define type to semantic for buffers allocator
define_type_to_shader_semantics(ror::Model)
{
	return rhi::BufferSemantic::mesh_data;
}

define_type_to_shader_semantics(ror::BinBuffer)
{
	return rhi::BufferSemantic::gltf_bin_buffer;
}

define_type_to_shader_semantics(ror::BinBuffer_Vector)
{
	return rhi::BufferSemantic::gltf_bin_buffer;
}

}        // namespace rhi
