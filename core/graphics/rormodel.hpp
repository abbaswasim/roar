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

#include "graphics/roranimation.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"
#include "graphics/rornode.hpp"
#include "graphics/rorskin.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rortexture.hpp"
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

class ROAR_ENGINE_ITEM Model
{
  public:
	FORCE_INLINE        Model()                             = default;        //! Default constructor
	FORCE_INLINE        Model(const Model &a_other)         = default;        //! Copy constructor
	FORCE_INLINE        Model(Model &&a_other) noexcept     = default;        //! Move constructor
	FORCE_INLINE Model &operator=(const Model &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Model &operator=(Model &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Model() noexcept                          = default;        //! Destructor

	// Populates data from a gltf file loaded as a resource.
	// If this or any other load functions are called before, data needs to be appended
	void load_from_gltf_file(std::filesystem::path a_filename);
	void upload();

	// clang-format off
	FORCE_INLINE const auto &images()           const    {  return this->m_images;          }
	FORCE_INLINE const auto &samplers()         const    {  return this->m_samplers;        }
	FORCE_INLINE const auto &textures()         const    {  return this->m_textures;        }
	FORCE_INLINE const auto &materials()        const    {  return this->m_materials;       }
	FORCE_INLINE const auto &meshes()           const    {  return this->m_meshes;          }
	FORCE_INLINE const auto &skins()            const    {  return this->m_skins;           }
	FORCE_INLINE const auto &nodes()            const    {  return this->m_nodes;           }
	FORCE_INLINE const auto &nodes_side_data()  const    {  return this->m_nodes_side_data; }
	FORCE_INLINE const auto &animations()       const    {  return this->m_animations;      }
	// clang-format on

  private:
	// This works for 1 model at the moment. Next append to these when a second asset is loaded
	// This would require going through all the models that needs loading and getting max bounds of each and then adjusting indices

	std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>>     m_images{};                 //! All images, by handles
	std::vector<rhi::TextureSampler, rhi::BufferAllocator<rhi::TextureSampler>> m_samplers{};               //! All samplers, by handles
	std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>               m_textures{};               //! All textures by handles
	std::vector<ror::Material, rhi::BufferAllocator<ror::Material>>             m_materials{};              //! All the materials in this asset
	std::vector<ror::Mesh, rhi::BufferAllocator<ror::Mesh>>                     m_meshes{};                 //! All the meshes in this asset
	std::vector<ror::Skin, rhi::BufferAllocator<ror::Skin>>                     m_skins{};                  //! All the skins we have
	std::vector<ror::Node, rhi::BufferAllocator<ror::Node>>                     m_nodes{};                  //! All the nodes in this asset
	std::vector<ror::NodeData, rhi::BufferAllocator<ror::NodeData>>             m_nodes_side_data{};        //! All the nodes parallel data that needs to be maintained
	std::vector<ror::Animation, rhi::BufferAllocator<ror::Animation>>           m_animations{};             //! All the animations in this asset
	BinBuffer_Vector_Vector                                                     m_buffers{};                //! All the buffers provided in gltf, only required temporarily
};

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
