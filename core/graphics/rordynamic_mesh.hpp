// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
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
#include "event_system/rorevent_system.hpp"
#include "foundation/rormacros.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rorvertex_description.hpp"

namespace ror
{
class Renderer;

class ROAR_ENGINE_ITEM DynamicMesh final
{
  public:
	FORCE_INLINE              DynamicMesh()                               = default;        //! Default constructor
	FORCE_INLINE              DynamicMesh(const DynamicMesh &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE              DynamicMesh(DynamicMesh &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE DynamicMesh &operator=(const DynamicMesh &a_other)       = delete;         //! Copy assignment operator
	FORCE_INLINE DynamicMesh &operator=(DynamicMesh &&a_other) noexcept   = delete;         //! Move assignment operator
	FORCE_INLINE ~DynamicMesh() noexcept                                  = default;        //! Destructor

	void init(const rhi::Device &a_device, rhi::PrimitiveTopology a_topology);
	void init_upload(const rhi::Device &a_device, const ror::Renderer &a_renderer, rhi::BlendMode a_blend_mode, rhi::PrimitiveTopology a_topology);
	void render(const ror::Renderer &a_renderer, rhi::RenderCommandEncoder &a_encoder);
	void load_texture(const rhi::Device &a_device, std::filesystem::path a_texure_path = "checker.png");
	void set_texture(rhi::TextureImage *a_texture = nullptr, rhi::TextureSampler *a_sampler = nullptr);
	void setup_vertex_descriptor(rhi::VertexDescriptor *a_descriptor = nullptr);
	void setup_shaders(const ror::Renderer &a_renderer, rhi::BlendMode a_blend_mode = rhi::BlendMode::blend, std::filesystem::path a_vertex_shader = "triangle.glsl.vert", std::filesystem::path a_fragment_shader = "triangle.glsl.frag");

	// clang-format off
	FORCE_INLINE void device(const rhi::Device *a_device)                        { this->m_device                       = a_device;         }
	FORCE_INLINE void texture_sampler(rhi::TextureSampler a_sampler)             { this->m_texture_sampler              = a_sampler;         }
	FORCE_INLINE void shader_program(rhi::Program a_program)                     { this->m_shader_program               = a_program;         }
	FORCE_INLINE void shader_program_external(rhi::Program *a_program)           { this->m_shader_program_external      = a_program;         }
	FORCE_INLINE void vertex_buffer(rhi::Buffer a_vertex_buffer)                 { this->m_vertex_buffer                = a_vertex_buffer;   }
	FORCE_INLINE void index_buffer(rhi::Buffer a_index_buffer)                   { this->m_index_buffer                 = a_index_buffer;    }
	FORCE_INLINE void topology(rhi::PrimitiveTopology a_topology)                { this->m_topology                     = a_topology;        }
	FORCE_INLINE void has_indices(bool a_has_indices)                            { this->m_has_indices                  = a_has_indices;     }

	FORCE_INLINE       rhi::TextureImage     &texture_image() noexcept                { return this->m_texture_image;           }
	FORCE_INLINE       rhi::TextureSampler   &texture_sampler() noexcept              { return this->m_texture_sampler;         }
	FORCE_INLINE const rhi::Program          &shader_program() const noexcept         { return this->m_shader_program;          }
	FORCE_INLINE const rhi::Program          *shader_program_external() const noexcept{ return this->m_shader_program_external; }
	FORCE_INLINE       rhi::Program          *shader_program_external() noexcept      { return this->m_shader_program_external; }
	FORCE_INLINE const rhi::VertexDescriptor &vertex_descriptor() const noexcept      { return this->m_vertex_descriptor;       }
	FORCE_INLINE const rhi::Buffer           &vertex_buffer() const noexcept          { return this->m_vertex_buffer;           }
	FORCE_INLINE const rhi::Buffer           &index_buffer() const noexcept           { return this->m_index_buffer;            }
	// clang-format on

	void upload_data(const uint8_t *a_vertex_data_pointer, size_t a_vertex_size_in_bytes, uint32_t a_vertex_attributes_count,
	                 const uint8_t *a_indices_data_pointer = nullptr, size_t a_indices_size_in_bytes = 0, uint32_t a_indices_count = 0);

  protected:
  private:
	// Render data
	const rhi::Device     *m_device{nullptr};                                    //! Non-Owning pointer to a device that is used to initiliazed this gui
	rhi::TextureImage      m_texture_image{};                                    //! Texture image, should probably be a list of these at some point
	rhi::TextureSampler    m_texture_sampler{};                                  //! Texture sampler, the default sampler used to render
	rhi::TextureImage     *m_texture_image_external{nullptr};                    //! Non-Owning pointer of a an external Texture image
	rhi::TextureSampler   *m_texture_sampler_external{nullptr};                  //! Non-Owning pointer of a an external Texture sampler
	rhi::Program           m_shader_program{-1, -1};                             //! Since we have one of this or next external, its only guaranteed to render in main pass
	rhi::Program          *m_shader_program_external{nullptr};                   //! Non-Owning point to a an exteranl program that overrides its own program if valid
	rhi::VertexDescriptor  m_vertex_descriptor{};                                //! The vertex descriptor of the UI, that is defined by xy, uv, c of ImGUI vertex buffer
	rhi::Buffer            m_vertex_buffer{};                                    //! Vertex buffer with interleaved data of of any kind
	rhi::Buffer            m_index_buffer{};                                     //! Index buffer with uint16_t type
	bool                   m_has_texture{false};                                 //! Whether has textures or not
	bool                   m_has_positions{false};                               //! Whether has positions or using no attributes
	bool                   m_has_indices{false};                                 //! Whether has index buffer or not
	bool                   m_has_vertex_shader_buffer{false};                    //! Whether has shader buffer or just using the one from renderer
	bool                   m_has_fragment_shader_buffer{false};                  //! Whether has shader buffer or just using the one from renderer
	rhi::PrimitiveTopology m_topology{rhi::PrimitiveTopology::triangles};        //! What are we rendering, default triangles
	uint32_t               m_vertices_count{0};                                  //! How many vertices are there, this is also saved for each attribute
	rhi::ShaderBuffer      m_vertex_shader_buffer{"dynamic_mesh_uniform",
                                             rhi::ShaderBufferType::ubo,
                                             rhi::Layout::std140, 0, 0};        //! Mesh specific shader buffer for vertex shader that can be used along side other from the renderer
	rhi::ShaderBuffer      m_fragment_shader_buffer{"dynamic_mesh_uniform",
                                               rhi::ShaderBufferType::ubo,
                                               rhi::Layout::std140, 0, 0};        //! Mesh specific shader buffer for fragment shader that can be used along side other from the renderer
};

// Creates a default descriptor with nothing in it. This can be used to render no attributes geometry like the fullscreen quad
rhi::VertexDescriptor create_default_descriptor();

// Creates a float3 positions descriptor
rhi::VertexDescriptor create_p_float3_descriptor();

// Creates a float3 positions and uint16 index descriptor
rhi::VertexDescriptor create_p_float3_i_uint16_descriptor();

// Creates a float3 positions, float2 uv descriptor
rhi::VertexDescriptor create_p_float3_t_float2_descriptor();

// Creates a float3 positions, float2 uv and uint16 index descriptor
rhi::VertexDescriptor create_p_float3_t_float2_i_uint16_descriptor();

}        // namespace ror
