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
#include "event_system/rorevent_system.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "graphics/rordynamic_mesh.hpp"
#include "graphics/rorlight.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rornode.hpp"
#include "math/rormatrix4.hpp"
#include "math/rortransform.hpp"
#include "profiling/rortimer.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include <any>
#include <filesystem>
#include <limits>
#include <unordered_map>

namespace ror
{
// TODO: Define properly somewhere
class ROAR_ENGINE_ITEM ParticleSystem
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
	FORCE_INLINE                SceneNodeData(const SceneNodeData &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                SceneNodeData(SceneNodeData &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE SceneNodeData &operator=(const SceneNodeData &a_other)         = delete;         //! Copy assignment operator
	FORCE_INLINE SceneNodeData &operator=(SceneNodeData &&a_other) noexcept     = delete;         //! Move assignment operator
	FORCE_INLINE ~SceneNodeData() noexcept                                      = default;        //! Destructor

	int32_t     m_model{-1};              //! Model index should be init with -1 if no models in this node
	int32_t     m_program_id{-1};         //! Which program does this node require to render
	uint32_t    m_animation{0};           //! Which animation clip should be used if any
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
	FORCE_INLINE        Scene(const Scene &a_other)         = delete;         //! Copy constructor
	FORCE_INLINE        Scene(Scene &&a_other) noexcept     = delete;         //! Move constructor
	FORCE_INLINE Scene &operator=(const Scene &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE Scene &operator=(Scene &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~Scene() noexcept override                 = default;        //! Destructor

	explicit Scene(const std::filesystem::path &a_level, ror::EventSystem &a_event_system);

	FORCE_INLINE Node *get_entity() const;
	FORCE_INLINE Node *get_root() const;
	FORCE_INLINE void  set_root(ror::Node *a_root);
	FORCE_INLINE void  next_camera(ror::OrbitCamera **a_active_camera);
	FORCE_INLINE void  save_render_state();
	FORCE_INLINE void  restore_render_state();

	declare_translation_unit_vtable();

	void render(rhi::RenderCommandEncoder &a_encoder, rhi::BuffersPack &a_buffers_pack, ror::Renderer &a_renderer, const rhi::Rendersubpass &a_subpass, ror::EventSystem &a_event_system);
	void pre_render(rhi::RenderCommandEncoder &a_encoder, rhi::BuffersPack &a_buffers_pack, ror::Renderer &a_renderer, const rhi::Rendersubpass &a_subpass);
	void compute_pass_walk_scene(rhi::ComputeCommandEncoder &a_command_encoder, rhi::Device &a_device, rhi::BuffersPack &a_buffers_pack, ror::Renderer &a_renderer, const rhi::Rendersubpass &a_subpass, Timer &a_timer, ror::EventSystem &a_event_system);
	// void cpu_walk_scene(rhi::ComputeCommandEncoder &a_command_encoder, rhi::Device &a_device, rhi::BuffersPack &a_buffers_pack, ror::Renderer &a_renderer, const rhi::Rendersubpass &a_subpass, Timer &a_timer, ror::EventSystem &a_event_system);

	void update(ror::Renderer &a_renderer, ror::Timer &a_timer);
	void update_from_data();
	void setup_cameras(ror::Renderer &a_renderer, ror::EventSystem &a_event_system);
	void load_models(ror::JobSystem &a_job_system, rhi::Device &a_device, const ror::Renderer &a_renderer, ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffers_packs);
	void shutdown(std::filesystem::path a_level, ror::EventSystem &a_event_system);
	void unload();
	void load_specific();
	void reset_to_default_state(ror::Renderer &a_renderer, rhi::RenderCommandEncoder &a_encoder);
	void fill_scene_data();

	// clang-format off
	FORCE_INLINE constexpr       auto &models()                 noexcept   {  return this->m_models;          }
	FORCE_INLINE constexpr       auto &nodes()                  noexcept   {  return this->m_nodes;           }
	FORCE_INLINE constexpr       auto &models()           const noexcept   {  return this->m_models;          }
	FORCE_INLINE constexpr const auto &nodes()            const noexcept   {  return this->m_nodes;           }
	FORCE_INLINE constexpr const auto &nodes_side_data()  const noexcept   {  return this->m_nodes_data;      }
	FORCE_INLINE constexpr const auto &particles()        const noexcept   {  return this->m_particles;       }
	FORCE_INLINE constexpr const auto &programs()         const noexcept   {  return this->m_programs;        }
    FORCE_INLINE constexpr const auto &shaders()          const noexcept   {  return this->m_shaders;         }
	FORCE_INLINE constexpr const auto &global_programs()  const noexcept   {  return this->m_global_programs; }
	FORCE_INLINE                 auto &cameras()                noexcept   {  return this->m_cameras;         }
	FORCE_INLINE constexpr const auto &lights()           const noexcept   {  return this->m_lights;          }
	FORCE_INLINE constexpr const auto &bounding_box()     const noexcept   {  return this->m_bounding_box;    }
	FORCE_INLINE constexpr       auto &dymanic_meshes()   const noexcept   {  return this->m_dynamic_meshes;  }
	FORCE_INLINE constexpr       auto  has_shadows()      const noexcept   {  return this->m_has_shadows;     }
	// clang-format on

	void upload(ror::JobSystem &a_job_system, const ror::Renderer &a_renderer, rhi::Device &a_device);

	// Some stuff that we want to save and restore for a scene after its changed, like camera position etc
	class ROAR_ENGINE_ITEM SceneState : public Configuration<SceneState>
	{
	  public:
		FORCE_INLINE             SceneState()                              = default;        //! Default constructor
		FORCE_INLINE             SceneState(const SceneState &a_other)     = delete;         //! Copy constructor
		FORCE_INLINE             SceneState(SceneState &&a_other) noexcept = delete;         //! Move constructor
		FORCE_INLINE SceneState &operator=(const SceneState &a_other)      = delete;         //! Copy assignment operator
		FORCE_INLINE SceneState &operator=(SceneState &&a_other) noexcept  = delete;         //! Move assignment operator
		FORCE_INLINE ~SceneState() noexcept override                       = default;        //! Destructor

		void init(std::filesystem::path a_data_path);

		declare_translation_unit_vtable();

		void load_specific();
		void write_specific();

		Vector3f   m_camera_center{0.0f, 0.0f, 0.0f};               //! Target position in worldspace
		Vector3f   m_camera_eye{0.0f, 0.0f, 1.0f};                  //! Eye position in worldspace
		Vector3f   m_camera_right{1.0f, 0.0f, 0.0f};                //! Right vector in camera's frame of reference
		Vector3f   m_camera_up{0.0f, 1.0f, 0.0f};                   //! Up vector in camera's frame of reference
		Vector3f   m_camera_forward{0.0f, 0.0f, -1.0f};             //! Forward vector in camera's frame of reference
		Vector3f   m_camera_minimum{-50.0f, -50.0f, -50.0f};        //! Minimum bound of the bounding volume that the camera will always make sure to see fully
		Vector3f   m_camera_maximum{50.0f, 50.0f, 50.0f};           //! Maximum bound of the bounding volume that the camera will always make sure to see fully
		float32_t  m_camera_y_fov{60.0f};                           //! Y-FOV of the camera
		float32_t  m_camera_z_near{0.1f};                           //! z-near of the camera
		float32_t  m_camera_z_far{1000.0f};                         //! z-far of the camera
		float32_t  m_camera_width{1024.0f};                         //! Width of the rectangle it needs to fill
		float32_t  m_camera_height{768.0f};                         //! Height of the rectangle it needs to fill
		float32_t  m_camera_x_mag{1.0f};                            //! Width of the orthographics camera
		float32_t  m_camera_y_mag{1.0f};                            //! Height of the orthographics camera
		CameraMode m_camera_mode{CameraMode::orbit};                //! Default orbit camera
		CameraType m_camera_type{CameraType::perspective};          //! Default perspective camera
		bool       m_is_valid{false};                               //! Is the data valid to be used by the system
	};

  private:
	struct GlobalProgram
	{
		int32_t      node_id{-1};
		int32_t      node_program_id{-1};
		int32_t      model_id{-1};
		int32_t      mesh_id{-1};
		int32_t      primitive_id{-1};
		rhi::Program program{-1, -1};
	};

	void init(const std::filesystem::path &a_level, ror::EventSystem &a_event_system);
	void install_input_handlers(ror::EventSystem &a_event_system);
	void uninstall_input_handlers(ror::EventSystem &a_event_system);
	void make_overlays();
	void read_nodes();
	void read_lights();
	void read_cameras();
	void read_programs();
	void init_global_programs();
	void read_probes();
	void generate_shaders(const ror::Renderer &a_renderer, ror::JobSystem &a_job_system);
	void update_bounding_box();
	void generate_grid_model(ror::JobSystem &a_job_system, const std::function<bool(size_t)> &a_upload_job, std::vector<ror::JobHandle<bool>> &a_job_handles, size_t a_model_index, rhi::BuffersPack &a_buffer_pack);
	void generate_debug_model(const std::function<bool(size_t)> &a_upload_lambda, size_t a_model_index, rhi::BuffersPack &a_buffer_pack);
	void add_model_node(int32_t a_model_index);
	void add_node();
	void push_shader_updates(const ror::Renderer &a_renderer);

	void create_global_program(const char *a_vertex_shader, const char *a_fragment_shader, size_t a_node_id, size_t a_model_id);
	auto find_global_program(rhi::RenderpassType a_passtype, uint32_t a_model_id, uint32_t a_mesh_id, size_t a_prim_id, Scene::GlobalProgram **a_global_program);

	using RenderpassPrograms = std::unordered_map<rhi::RenderpassType, std::vector<rhi::Program>>;
	using GlobalPrograms     = std::unordered_map<rhi::RenderpassType, std::vector<GlobalProgram>>;

	// All of these can be buffer allocated but for now leave them as is
	std::vector<ror::Model>          m_models{};                                               //! All the assets loaded as 3D models
	std::vector<ror::SceneNode>      m_nodes{};                                                //! All the nodes in this scene, must be in the parent order, parent first then children
	std::vector<ror::SceneNodeData>  m_nodes_data{};                                           //! All the nodes parallel data that needs to be maintained
	std::vector<ror::ParticleSystem> m_particles{};                                            //! All the particle emittors
	RenderpassPrograms               m_programs{};                                             //! All the shader programs per render pass for all the models
	std::vector<rhi::Shader>         m_shaders{};                                              //! All the shaders for all meshes in each model
	GlobalPrograms                   m_global_programs{};                                      //! All the global shader programs that overrides per mesh/model programs
	std::vector<ror::OrbitCamera>    m_cameras{};                                              //! All the cameras in the scene
	std::vector<ror::Light>          m_lights{};                                               //! All the lights in the scene
	std::vector<EnvironmentProbe>    m_probes{};                                               //! All the environment probes
	ror::BoundingBoxf                m_bounding_box{};                                         //! Scene bounding box, a combination of its models in object space
	bool                             m_indices_dirty{true};                                    //! If the scene graph indicies are direty and not uploaded yet
	bool                             m_pause_animation{false};                                 //! Should the animation be running or not
	bool                             m_has_shadows{false};                                     //! To tell any fragment shaders for any pass generated to use shadow mapping, this is NOT about shadow pass itself
	rhi::TriangleFillMode            m_triangle_fill_mode{rhi::TriangleFillMode::fill};        //! Triangle fill mode, initially filled but could be lines too
	std::vector<ror::DynamicMesh *>  m_dynamic_meshes{};                                       //! Non-Owning pointers to all the dynamic meshes created in the scene rendererd at once in the end
	int32_t                          m_grid_model_id{-1};                                      //! Reference to the grid for easy access
	EventCallback                    m_semi_colon_key_callback{};                              //! Semi colon key call back to enable disable the grid
	SceneState                       m_scene_state;                                            //! All the scene data that can be saved and restored to and from disk
	uint32_t                         m_current_camera_index{0};                                //! Camera to use to render the scene
};

void get_animation_sizes(ror::Scene &a_scene, uint32_t &a_animation_size, uint32_t &a_animation_count, uint32_t &a_sampler_input_size, uint32_t &a_sampler_output_size, uint32_t &a_weights_output_size);
void fill_morph_weights(ror::Scene &a_scene, rhi::ShaderBuffer &a_shader_buffer, uint32_t a_weights_size);

}        // namespace ror
