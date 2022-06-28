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

#include "configuration/rorconfiguration.hpp"
#include "foundation/rormacros.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rornode.hpp"
#include "math/rormatrix4.hpp"
#include "platform/rorapplication.hpp"
#include <sys/_types/_int32_t.h>

namespace ror
{
// TODO: Define properly somewhere
class ROAR_ENGINE_ITEM RenderDevice
{};
class ROAR_ENGINE_ITEM Camera
{
  public:
	FORCE_INLINE         Camera()                             = default;        //! Default constructors
	FORCE_INLINE         Camera(const Camera &a_other)        = default;        //! Copy constructor
	FORCE_INLINE         Camera(Camera &&a_other) noexcept    = default;        //! Move constructor
	FORCE_INLINE Camera &operator=(const Camera &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Camera &operator=(Camera &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Camera() noexcept                           = default;        //! Destructor

	CameraType m_type;
};
class ROAR_ENGINE_ITEM ParticleSystem
{};
class ROAR_ENGINE_ITEM Shader
{};
class ROAR_ENGINE_ITEM Light
{};

class ROAR_ENGINE_ITEM SceneNode
{
  public:
	FORCE_INLINE            SceneNode()                             = default;        //! Default constructors
	FORCE_INLINE            SceneNode(const SceneNode &a_other)     = default;        //! Copy constructor
	FORCE_INLINE            SceneNode(SceneNode &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE SceneNode &operator=(const SceneNode &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE SceneNode &operator=(SceneNode &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~SceneNode() noexcept                              = default;        //! Destructor

	int32_t    m_parent{-1};             //! Parent of the nodes, should be init with -1
	Transformf m_trs_transform{};        //! Contains translation vec3, Rotation quaternion and scale vec3
};

/**
 * A little bit more data on top of NodeData that my scene requires
 */
class ROAR_ENGINE_ITEM SceneNodeData : public NodeData
{
  public:
	FORCE_INLINE                SceneNodeData()                                 = default;        //! Default constructors
	FORCE_INLINE                SceneNodeData(const SceneNodeData &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                SceneNodeData(SceneNodeData &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE SceneNodeData &operator=(const SceneNodeData &a_other)         = default;        //! Copy assignment operator
	FORCE_INLINE SceneNodeData &operator=(SceneNodeData &&a_other) noexcept     = default;        //! Move assignment operator
	FORCE_INLINE ~SceneNodeData() noexcept                                      = default;        //! Destructor

	int32_t     m_program_id{-1};         //! Which program does this node require to render
	int32_t     m_light_id{-1};           //! Which light is linked to this node
	int32_t     m_camera_id{-1};          //! Which camera is used to render this scene from this node
	int32_t     m_particle_id{-1};        //! Which particle emitter is attached to the node
	bool        m_has_bvh{false};         //! Does the node has model that has a BVH
	std::string m_model_path{};           //! Which model was loaded for this node
};

class ROAR_ENGINE_ITEM Scene : public Configuration<Scene>
{
  public:
	FORCE_INLINE        Scene()                             = default;        //! Default constructor
	FORCE_INLINE        Scene(const Scene &a_other)         = default;        //! Copy constructor
	FORCE_INLINE        Scene(Scene &&a_other) noexcept     = default;        //! Move constructor
	FORCE_INLINE Scene &operator=(const Scene &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Scene &operator=(Scene &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Scene() noexcept override                 = default;        //! Destructor

	explicit Scene(std::filesystem::path a_level);

	FORCE_INLINE Node *get_entity() const;
	FORCE_INLINE Node *get_root() const;
	FORCE_INLINE void  set_root(ror::Node *a_root);
	FORCE_INLINE void  next_camera(ror::Camera **a_active_camera);
	FORCE_INLINE void  save_render_state();
	FORCE_INLINE void  restore_render_state();

	declare_translation_unit_vtable();

	void render(const RenderDevice *a_rendering_device);
	void update(double64_t a_milli_seconds);
	void unload();
	void load_specific();

	// clang-format off
	FORCE_INLINE const auto &models()           const    {  return this->m_models;          }
	FORCE_INLINE const auto &textures()         const    {  return this->m_textures;        }
	FORCE_INLINE const auto &nodes()            const    {  return this->m_nodes;           }
	FORCE_INLINE const auto &nodes_side_data()  const    {  return this->m_nodes_data; }
	// clang-format on

  private:
	void read_nodes();
	void read_lights();
	void read_cameras();
	void read_programs();

	// All of these can be buffer allocated but for now leave them as is
	std::vector<ror::Model>          m_models{};            //! All the assets loaded as 3D models
	std::vector<rhi::Texture>        m_textures{};          //! All textures by handles
	std::vector<ror::SceneNode>      m_nodes{};             //! All the nodes in this scene
	std::vector<ror::SceneNodeData>  m_nodes_data{};        //! All the nodes parallel data that needs to be maintained
	std::vector<ror::ParticleSystem> m_particles{};         //! All the particle emittors
	std::vector<ror::Shader>         m_shaders{};           //! All the global shaders
	std::vector<ror::Camera>         m_cameras{};           //! All the cameras in the scene
	std::vector<ror::Light>          m_lights{};            //! All the lights in the scene
};

}        // namespace ror
