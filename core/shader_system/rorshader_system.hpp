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

#include "graphics/rormaterial.hpp"
#include "graphics/rormodel.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"
#include <string>

namespace ror
{

/**
 * A simple shader system responsible for generating all the shader combinations
 */
class ROAR_ENGINE_ITEM ShaderSystem final
{
  public:
	FORCE_INLINE               ShaderSystem()                                = default;        //! Default constructor
	FORCE_INLINE               ShaderSystem(const ShaderSystem &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               ShaderSystem(ShaderSystem &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShaderSystem &operator=(const ShaderSystem &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE ShaderSystem &operator=(ShaderSystem &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE ~ShaderSystem() noexcept                                    = default;        //! Destructor

  private:
};

class Renderer;

using materials_vector = std::vector<ror::Material, rhi::BufferAllocator<ror::Material>>;

std::string vertex_shader_input_output(const rhi::VertexDescriptor &a_vertex_descriptor, uint32_t a_location_offset = 0, uint32_t a_target_offset = 0, const std::string &a_prefix = "", bool a_output = false, bool a_depth_shadow = false);
std::string fragment_shader_input_output(const rhi::VertexDescriptor &a_vertex_descriptor);

std::string generate_primitive_vertex_shader(const ror::Model &a_model, uint32_t a_mesh_index, uint32_t a_primitive_index, rhi::RenderpassType a_passtype, const ror::Renderer &a_renderer);
// std::string generate_primitive_vertex_shader(const ror::Mesh &a_mesh, uint32_t a_index);
std::string generate_primitive_fragment_shader(const ror::Mesh &a_mesh, const materials_vector &a_materials, uint32_t a_primitive_index, rhi::RenderpassType a_passtype, const ror::Renderer &a_renderer, bool a_has_shadow = false);

std::vector<std::string>& mesh_shaders_dependencies();

void node_transform_glsl_comp(std::string &a_input, const ror::Renderer &a_renderer);
}        // namespace ror
