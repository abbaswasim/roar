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

#include "graphics/rormodel.hpp"
#include "graphics/rornode.hpp"
#include "math/rormatrix4.hpp"
#include "platform/rorapplication.hpp"

namespace ror
{
// TODO: Define properly somewhere
class ROAR_ENGINE_ITEM RenderDevice
{};
class ROAR_ENGINE_ITEM Camera
{};
class ROAR_ENGINE_ITEM ParticleSystem
{};
class ROAR_ENGINE_ITEM Shader
{};
class ROAR_ENGINE_ITEM Light
{};

class ROAR_ENGINE_ITEM Scene
{
  public:
	FORCE_INLINE        Scene()                         = default;            //! Default constructor
	FORCE_INLINE        Scene(const Scene &a_other)     = default;            //! Copy constructor
	FORCE_INLINE        Scene(Scene &&a_other) noexcept = default;            //! Move constructor
	FORCE_INLINE Scene &operator=(const Scene &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Scene &operator=(Scene &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Scene() noexcept                          = default;        //! Destructor

	FORCE_INLINE Node *get_entity() const;
	FORCE_INLINE Node *get_root() const;
	FORCE_INLINE void  set_root(ror::Node *a_root);
	FORCE_INLINE void  next_camera(ror::Camera **a_active_camera);
	FORCE_INLINE void  save_render_state();
	FORCE_INLINE void  restore_render_state();

	void render(const RenderDevice *a_rendering_device);
	void update(double64_t a_milli_seconds);
	void load(std::filesystem::path a_level = "");
	void unload();

	// clang-format off
	FORCE_INLINE const auto &models()           const    {  return this->m_models;          }
	FORCE_INLINE const auto &textures()         const    {  return this->m_textures;        }
	FORCE_INLINE const auto &nodes()            const    {  return this->m_nodes;           }
	FORCE_INLINE const auto &nodes_side_data()  const    {  return this->m_nodes_side_data; }
	// clang-format on

  private:
	// All of these can be buffer allocated but for now leave them as is
	std::vector<ror::Model>          m_models{};                 //! All the assets loaded as 3D models
	std::vector<rhi::Texture>        m_textures{};               //! All textures by handles
	std::vector<ror::Node>           m_nodes{};                  //! All the nodes in this assets
	std::vector<ror::NodeData>       m_nodes_side_data{};        //! All the nodes parallel data that needs to be maintained
	std::vector<ror::ParticleSystem> m_particles;                //! All the particle emittors
	std::vector<ror::Shader>         m_shaders;                  //! All the global shaders
	std::vector<ror::Camera>         m_cameras;                  //! All the cameras in the scene
	std::vector<ror::Light>          m_lights;                   //! All the lights in the scene
};

}        // namespace ror
