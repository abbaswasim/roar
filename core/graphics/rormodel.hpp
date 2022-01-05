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
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rortexture.hpp"

namespace ror
{
class ROAR_ENGINE_ITEM Model
{
  public:
	FORCE_INLINE        Model()                         = default;            //! Default constructor
	FORCE_INLINE        Model(const Model &a_other)     = default;            //! Copy constructor
	FORCE_INLINE        Model(Model &&a_other) noexcept = default;            //! Move constructor
	FORCE_INLINE Model &operator=(const Model &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Model &operator=(Model &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Model() noexcept                          = default;        //! Destructor

  private:
	// TODO: This works for 1 model at the moment. Next append to these when a second asset is loaded
	// This would require going through all the models that needs loading and getting max bounds of each
	std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>>     m_images{};
	std::vector<rhi::TextureSampler, rhi::BufferAllocator<rhi::TextureSampler>> m_samplers{};
	std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>               m_textures{};
	std::vector<ror::Material, rhi::BufferAllocator<Material>>                  m_materials{};
	std::vector<Mesh, rhi::BufferAllocator<Mesh>>                               m_meshes{};
	std::vector<Skin, rhi::BufferAllocator<Skin>>                               m_skins{};
	std::vector<Node, rhi::BufferAllocator<Node>>                               m_nodes{};
	std::vector<Animation, rhi::BufferAllocator<Animation>>                     m_animations{};
};

}        // namespace ror

namespace rhi
{
// Define type to semantic for buffers allocator
define_type_to_shader_semantics(ror::Model)
{
	return rhi::BufferSemantic::mesh_data;
}

}        // namespace rhi
