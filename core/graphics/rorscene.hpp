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

#include "camera/rorcamera.hpp"
#include "configuration/rorconfiguration.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rornode.hpp"
#include "math/rormatrix4.hpp"
#include "math/rortransform.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rortypes.hpp"
#include <any>
#include <filesystem>
#include <limits>
#include <unordered_map>

// TODO: Remove me
#include <Metal/MTLRenderCommandEncoder.hpp>

namespace ror
{
// TODO: Define properly somewhere
class ROAR_ENGINE_ITEM ParticleSystem
{};

class ROAR_ENGINE_ITEM Light
{
  public:
	FORCE_INLINE        Light()                             = default;        //! Default constructors
	FORCE_INLINE        Light(const Light &a_other)         = default;        //! Copy constructor
	FORCE_INLINE        Light(Light &&a_other) noexcept     = default;        //! Move constructor
	FORCE_INLINE Light &operator=(const Light &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Light &operator=(Light &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Light() noexcept                          = default;        //! Destructor

	enum class LightType
	{
		directional,
		spot,
		point,
		area
	};

	void update();
	void upload(rhi::Device &a_device);
	void fill_shader_buffer();

	// clang-format off
	FORCE_INLINE constexpr auto& shader_buffer() const noexcept  { return this->m_shader_buffer; }
	// clang-format on

	LightType m_type{LightType::directional};                        //! Light type
	Matrix4f  m_mvp{};                                               //! Model view projection of the light, used in shadow mapping
	Vector3f  m_color{};                                             //! Light color
	Vector3f  m_position{};                                          //! Position of point and spot lights
	Vector3f  m_direction{};                                         //! Direction of directional and spot lights
	float32_t m_intensity{1.0f};                                     //! Light intensity
	float32_t m_range{std::numeric_limits<float32_t>::max()};        //! Light range after which light attenuates
	float32_t m_inner_angle{0.0f};                                   //! Spot light inner angle
	float32_t m_outer_angle{ror::ror_pi / 4.0f};                     //! Spot light outter angle

	uint32_t m_mvp_offset{0};                //! Offset in UBO of Model view projection of the light, used in shadow mapping
	uint32_t m_color_offset{0};              //! Offset in UBO of Light color
	uint32_t m_position_offset{0};           //! Offset in UBO of Position of point and spot lights
	uint32_t m_direction_offset{0};          //! Offset in UBO of Direction of directional and spot lights
	uint32_t m_intensity_offset{0};          //! Offset in UBO of Light intensity
	uint32_t m_range_offset{0};              //! Offset in UBO of Light range after which light attenuates
	uint32_t m_inner_angle_offset{0};        //! Offset in UBO of Spot light inner angle
	uint32_t m_outer_angle_offset{0};        //! Offset in UBO of Spot light outter angle

	rhi::ShaderBuffer m_shader_buffer{};        //! Shader buffer for a specific type of light UBO
};

class ROAR_ENGINE_ITEM EnvironmentProbe final
{
  public:
	FORCE_INLINE                   EnvironmentProbe()                                    = default;        //! Default constructor
	FORCE_INLINE                   EnvironmentProbe(const EnvironmentProbe &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                   EnvironmentProbe(EnvironmentProbe &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE EnvironmentProbe &operator=(const EnvironmentProbe &a_other)            = default;        //! Copy assignment operator
	FORCE_INLINE EnvironmentProbe &operator=(EnvironmentProbe &&a_other) noexcept        = default;        //! Move assignment operator
	FORCE_INLINE ~EnvironmentProbe() noexcept                                            = default;        //! Destructor

	// clang-format off
	FORCE_INLINE void transform(Transformf a_transform)      { this->m_transform = a_transform; }
	FORCE_INLINE void path(std::filesystem::path a_path)     { this->m_path = a_path;           }

	FORCE_INLINE auto transform()                            { return this->m_transform;        }
	FORCE_INLINE auto path()                                 { return this->m_path;             }
	// clang-format on

  protected:
  private:
	ror::Transformf       m_transform{};
	std::filesystem::path m_path{};
};

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
	FORCE_INLINE void  next_camera(ror::OrbitCamera **a_active_camera);
	FORCE_INLINE void  save_render_state();
	FORCE_INLINE void  restore_render_state();

	declare_translation_unit_vtable();

	void render(const RenderDevice *a_rendering_device);
	void update(double64_t a_milli_seconds);
	void load_models(ror::JobSystem &a_job_system, rhi::Device &a_device,  const ror::Renderer& a_renderer, ror::EventSystem &a_event_system);
	void unload();
	void load_specific();

	// clang-format off
	FORCE_INLINE const auto &models()           const    {  return this->m_models;          }
	FORCE_INLINE const auto &nodes()            const    {  return this->m_nodes;           }
	FORCE_INLINE const auto &nodes_side_data()  const    {  return this->m_nodes_data;      }
	FORCE_INLINE const auto &particles()        const    {  return this->m_particles;       }
	FORCE_INLINE const auto &programs()         const    {  return this->m_programs;        }
    FORCE_INLINE const auto &global_shaders()   const    {  return this->m_shaders;         }
	FORCE_INLINE const auto &global_programs()  const    {  return this->m_global_programs; }
	FORCE_INLINE       auto &cameras()                   {  return this->m_cameras;         }
	FORCE_INLINE const auto &lights()           const    {  return this->m_lights;          }
	// clang-format on

  private:
	void read_nodes();
	void read_lights();
	void read_cameras();
	void read_programs();
	void read_probes();
	void generate_shaders(const ror::Renderer &a_renderer, rhi::Device &a_device, ror::JobSystem &a_job_system, ror::EventSystem& a_event_system);
	void upload(const ror::Renderer &a_renderer, rhi::Device &a_device, ror::EventSystem& a_event_system);

	using RenderpassPrograms = std::unordered_map<rhi::RenderpassType, std::vector<rhi::Program>>;

	// All of these can be buffer allocated but for now leave them as is
	std::vector<ror::Model>          m_models{};                 //! All the assets loaded as 3D models
	std::vector<ror::SceneNode>      m_nodes{};                  //! All the nodes in this scene
	std::vector<ror::SceneNodeData>  m_nodes_data{};             //! All the nodes parallel data that needs to be maintained
	std::vector<ror::ParticleSystem> m_particles{};              //! All the particle emittors
	RenderpassPrograms               m_programs{};               //! All the shader programs per render pass for all the models
	std::vector<rhi::Shader>         m_shaders{};                //! All the shaders for all meshes in each model
	std::vector<rhi::Program>        m_global_programs{};        //! All the global shader programs that overrides per mesh/model programs
	std::vector<ror::OrbitCamera>    m_cameras{};                //! All the cameras in the scene
	std::vector<ror::Light>          m_lights{};                 //! All the lights in the scene
	std::vector<EnvironmentProbe>    m_probes{};                 //! All the environment probes
};

}        // namespace ror
