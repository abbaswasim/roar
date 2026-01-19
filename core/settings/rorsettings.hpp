// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

// #include "configuration/rorconfiguration.hpp"
// #include "configuration/rorsettings_configuration.hpp"
#include "event_system/rorevent_system.hpp"
// #include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
// #include "foundation/rorutilities.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector4.hpp"
// #include "math/rorvector.hpp"
#include "rhi/rortypes.hpp"
#include <cstdint>
#include <string>
// #include <utility>
#include <vector>

namespace ror
{

static const auto bits_shift            = 16u;
static const auto bits_mask             = 0x0000FFFF;
static const auto generic_numbers_count = 10;

#define ROAR_MAKE_VERSION(major, minor, patch) \
	(((static_cast<uint32_t>(major)) << 22u) | ((static_cast<uint32_t>(minor)) << 12u) | (static_cast<uint32_t>(patch)))

class ROAR_ENGINE_ITEM Settings final
{
  public:
	FORCE_INLINE           Settings(const Settings &a_other)      = delete;         //! Copy constructor
	FORCE_INLINE           Settings(Settings &&a_other) noexcept  = delete;         //! Move constructor
	FORCE_INLINE Settings &operator=(const Settings &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE Settings &operator=(Settings &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~Settings() noexcept                             = default;        //! Destructor

	Settings();

	// clang-format off
	FORCE_INLINE constexpr auto max_attributes()                  const noexcept { return this->m_bindings.m_max_attributes + 1;                                             }

	// Note: Samplers bindings are not advances by max_attributes, this is because is Metal they are in different namespace and in vulkan in different set
	FORCE_INLINE constexpr auto shadow_map_sampler_binding()      const noexcept { return (this->m_bindings.m_shadow_map_sampler & bits_mask);                               }
	FORCE_INLINE constexpr auto brdf_integration_sampler_binding()const noexcept { return (this->m_bindings.m_brdf_integration_sampler & bits_mask);                         }
	FORCE_INLINE constexpr auto skybox_sampler_binding()          const noexcept { return (this->m_bindings.m_skybox_sampler & bits_mask);                                   }
	FORCE_INLINE constexpr auto irradiance_sampler_binding()      const noexcept { return (this->m_bindings.m_irradiance_sampler & bits_mask);                               }
	FORCE_INLINE constexpr auto radiance_sampler_binding()        const noexcept { return (this->m_bindings.m_radiance_sampler & bits_mask);                                 }
	FORCE_INLINE constexpr auto material_samplers_binding()       const noexcept { return (this->m_bindings.m_material_samplers & bits_mask);                                }
	FORCE_INLINE constexpr auto material_factors_binding()        const noexcept { return (this->m_bindings.m_material_factors & bits_mask) + this->max_attributes();        }
	FORCE_INLINE constexpr auto per_frame_uniform_binding()       const noexcept { return (this->m_bindings.m_per_frame_uniform & bits_mask) + this->max_attributes();       }
	FORCE_INLINE constexpr auto per_view_uniform_binding()        const noexcept { return (this->m_bindings.m_per_view_uniform & bits_mask) + this->max_attributes();        }
	FORCE_INLINE constexpr auto nodes_model_uniform_binding()     const noexcept { return (this->m_bindings.m_nodes_models & bits_mask) + this->max_attributes();            }
	FORCE_INLINE constexpr auto nodes_offset_uniform_binding()    const noexcept { return (this->m_bindings.m_nodes_offsets & bits_mask) + this->max_attributes();           }
	FORCE_INLINE constexpr auto directional_light_binding()       const noexcept { return (this->m_bindings.m_directional_light & bits_mask) + this->max_attributes();       }
	FORCE_INLINE constexpr auto point_light_binding()             const noexcept { return (this->m_bindings.m_point_light & bits_mask) + this->max_attributes();             }
	FORCE_INLINE constexpr auto spot_light_binding()              const noexcept { return (this->m_bindings.m_spot_light & bits_mask) + this->max_attributes();              }
	FORCE_INLINE constexpr auto area_light_binding()              const noexcept { return (this->m_bindings.m_area_light & bits_mask) + this->max_attributes();              }
	FORCE_INLINE constexpr auto skin_joints_binding()             const noexcept { return (this->m_bindings.m_skin_joints & bits_mask) + this->max_attributes();             }
	FORCE_INLINE constexpr auto morph_weights_uniform_binding()   const noexcept { return (this->m_bindings.m_morphs_weights & bits_mask) + this->max_attributes();          }
	FORCE_INLINE constexpr auto joint_inverse_bind_binding()      const noexcept { return (this->m_bindings.m_joint_inverse_bind & bits_mask) + this->max_attributes();      }

	FORCE_INLINE constexpr auto shadow_map_sampler_set()          const noexcept { return (this->m_bindings.m_shadow_map_sampler >> bits_shift);                             }
	FORCE_INLINE constexpr auto brdf_integration_sampler_set()    const noexcept { return (this->m_bindings.m_brdf_integration_sampler >> bits_shift);                       }
	FORCE_INLINE constexpr auto skybox_sampler_set()              const noexcept { return (this->m_bindings.m_skybox_sampler >> bits_shift);                                 }
	FORCE_INLINE constexpr auto irradiance_sampler_set()          const noexcept { return (this->m_bindings.m_irradiance_sampler >> bits_shift);                             }
	FORCE_INLINE constexpr auto radiance_sampler_set()            const noexcept { return (this->m_bindings.m_radiance_sampler >> bits_shift);                               }
	FORCE_INLINE constexpr auto material_samplers_set()           const noexcept { return (this->m_bindings.m_material_samplers >> bits_shift);                              }
	FORCE_INLINE constexpr auto material_factors_set()            const noexcept { return (this->m_bindings.m_material_factors >> bits_shift);                               }
	FORCE_INLINE constexpr auto per_frame_uniform_set()           const noexcept { return (this->m_bindings.m_per_frame_uniform >> bits_shift);                              }
	FORCE_INLINE constexpr auto per_view_uniform_set()            const noexcept { return (this->m_bindings.m_per_view_uniform >> bits_shift);                               }
	FORCE_INLINE constexpr auto nodes_model_uniform_set()         const noexcept { return (this->m_bindings.m_nodes_models >> bits_shift);                                   }
	FORCE_INLINE constexpr auto nodes_offset_uniform_set()        const noexcept { return (this->m_bindings.m_nodes_offsets >> bits_shift);                                  }
	FORCE_INLINE constexpr auto directional_light_set()           const noexcept { return (this->m_bindings.m_directional_light >> bits_shift);                              }
	FORCE_INLINE constexpr auto point_light_set()                 const noexcept { return (this->m_bindings.m_point_light >> bits_shift);                                    }
	FORCE_INLINE constexpr auto spot_light_set()                  const noexcept { return (this->m_bindings.m_spot_light >> bits_shift);                                     }
	FORCE_INLINE constexpr auto area_light_set()                  const noexcept { return (this->m_bindings.m_area_light >> bits_shift);                                     }
	FORCE_INLINE constexpr auto skin_joints_set()                 const noexcept { return (this->m_bindings.m_skin_joints >> bits_shift);                                    }
	FORCE_INLINE constexpr auto morph_weights_uniform_set()       const noexcept { return (this->m_bindings.m_morphs_weights >> bits_shift);                                 }
	FORCE_INLINE constexpr auto joint_inverse_bind_set()          const noexcept { return (this->m_bindings.m_joint_inverse_bind >> bits_shift);                             }
	// clang-format on

	void setup_generic_numbers(EventSystem &a_event_system);

	struct Environment
	{
		enum class VisualizeMode
		{
			skybox,
			irradiance,
			radiance,
			brdf_lut
		};

		bool          m_visible{true};
		VisualizeMode m_mode{VisualizeMode::skybox};
		uint32_t      m_mipmap{0};
	};

	Environment::VisualizeMode get_environment_mode(std::string a_mode);

	std::string m_roar_title{};
	std::string m_roar_cache{};
	std::string m_default_scene{};
	std::string m_buffers_format{};
	std::string m_working_dir{};
	std::string m_renderer_config{"renderer.json"};
	std::string m_application_name{"Roar Editor"};
	std::string m_engine_name{"Roar"};
	std::string m_shaders_watch_path{};

	std::vector<std::string> m_clean_dirs{};        //! Directories to clean on boot

	float32_t m_zoom_speed{20.0f};
	float32_t m_fog_start{0.0f};
	float32_t m_fog_end{500.0f};
	float32_t m_depth_clear{1.0f};

	uint32_t m_unit{1};                      //! 1 == meter, 1000 == km etc, to use the unit multiply it with your quantities
	uint32_t m_threads_multiplier{2};        //! How many more threads should the job system create on top of available cores. Remember this is a multiplier
	uint32_t m_buffer_increment{1};
	uint32_t m_multisample_count{4};
	uint32_t m_depth_buffer_bits{16};
	uint32_t m_debug_mesh_count{1000};
	uint32_t m_gui_primitives_count{10};                      // 10 times
	uint32_t m_gui_primitives_size{1000000};                  // About 1.0 MB size of the vertex buffer
	uint32_t m_gui_primitives_index_size{10000};              // Size of index buffer
	int32_t  m_generic_numbers[generic_numbers_count];        //! This is used to limit things or render a specific node etc, each number is decremented by Ctr + N and increment by CMD + N
	uint32_t m_application_version{0};
	uint32_t m_engine_version{0};
	uint32_t m_resolve_includes_depth{10};        //! How many levels deep should the includes searching go

	bool m_save_scene_state{true};
	bool m_load_scene_state{true};
	bool m_clean_on_boot{false};
	bool m_visualise_mipmaps{false};
	bool m_vertical_sync{false};
	bool m_fullscreen{false};
	bool m_resizable{false};
	bool m_always_on_top{false};
	bool m_fog_enabled{false};
	bool m_force_rgba_textures{false};
	bool m_force_ldr_textures{false};
	bool m_print_generated_shaders{false};
	bool m_print_generated_msl_shaders{false};
	bool m_print_generated_glsl_shaders{false};
	bool m_print_generated_vertex_shaders{false};
	bool m_print_generated_fragment_shaders{false};
	bool m_print_generated_compute_shaders{false};
	bool m_log_event_system{false};
	bool m_warn_event_system{false};
	bool m_write_generated_shaders{false};
	bool m_watch_shaders{false};
	bool m_generate_debug_mesh{false};
	bool m_generate_gui_mesh{false};
	bool m_show_grid{false};
	bool m_generate_cube_mesh{false};
	bool m_generate_quad_mesh{false};
	bool m_generate_fullscreen_quad_mesh{false};
	bool m_generate_cube_map{false};
	bool m_generate_canonical_cube_map{false};
	bool m_invert_canonical_cube_map{false};
	bool m_generate_pink_textures{false};
	bool m_fit_far_to_scene{false};
	bool m_background_srgb_to_linear{false};
	bool m_force_linear_textures{false};
	bool m_force_mipmapped_textures{false};
	bool m_animate_cpu{false};
	bool m_clamp_material_roughness{false};
	bool m_clamp_material_metallic{false};
	bool m_show_axis{false};

	ror::Vector4f m_debug_mesh_color{1.0f, 0.2f, 0.2f, 0.5f};
	ror::Vector4f m_ambient_light_color{0.2f, 0.2f, 0.2f, 1.0f};
	ror::Vector4f m_fog_color{0.5f, 0.5f, 0.5f, 1.0f};

	struct Window
	{
		bool             m_transparent{false};
		bool             m_premultiplied{false};
		bool             m_prerotated{false};
		uint32_t         m_buffer_count{3};
		rhi::PixelFormat m_pixel_format{rhi::PixelFormat::r8g8b8a8_uint32_norm};
		rhi::PixelFormat m_depth_stencil_format{rhi::PixelFormat::depth24_norm_stencil8_uint32};
		ror::Vector4i    m_dimensions{0, 0, 1024, 768};
	};

	Window m_window{};

	ror::Vector4i m_viewport{0, 0, 1024, 768};

	rhi::PrimitiveWinding m_primitive_winding{rhi::PrimitiveWinding::counter_clockwise};

	enum class DebugMeshType
	{
		none,
		line_box,
		line_sphere,
		triangle_box,
		triangle_sphere
	};

	DebugMeshType m_debug_mesh_type{DebugMeshType::none};

	struct BindingsSets
	{
		// Initially assumes set is 0 for all of the following, will be loaded from setting later
		uint32_t m_shadow_map_sampler{0};
		uint32_t m_brdf_integration_sampler{0};
		uint32_t m_skybox_sampler{0};
		uint32_t m_irradiance_sampler{0};
		uint32_t m_radiance_sampler{0};
		uint32_t m_material_samplers{0};
		uint32_t m_material_factors{0};
		uint32_t m_per_frame_uniform{1};
		uint32_t m_per_view_uniform{2};
		uint32_t m_nodes_models{3};
		uint32_t m_nodes_offsets{4};
		uint32_t m_directional_light{5};
		uint32_t m_point_light{6};
		uint32_t m_spot_light{7};
		uint32_t m_area_light{8};
		uint32_t m_skin_joints{9};
		uint32_t m_morphs_weights{10};
		uint32_t m_joint_inverse_bind{11};
		uint32_t m_max_attributes{17};
	};

	struct Options
	{
		uint32_t m_version_major{3};
		uint32_t m_version_minor{0};
	};

	struct MetalOptions : public Options
	{
		bool argument_buffers{true};
		bool decoration_bindings{true};
		bool indirect_command_buffers{false};
	};

	struct VulkanOptions : public Options
	{
		uint32_t              m_descriptor_sets_size{4};
		uint32_t              m_descriptor_pool_size{1024};
		bool                  m_validation{true};
		bool                  m_api_dump{false};
		bool                  m_verbose_log{true};
		bool                  m_extended_dynamic_state{false};
		bool                  m_extended_dynamic_state3{false};
		rhi::TriangleFillMode m_polygon_mode{rhi::TriangleFillMode::fill};
	};

	VulkanOptions m_vulkan{};
	MetalOptions  m_metal{};

	struct DefaultVertexDescriptor
	{
		std::vector<std::pair<rhi::BufferSemantic, rhi::VertexFormat>> attributes{};
	};

	DefaultVertexDescriptor m_default_vertex_descriptor{};

	struct Grid
	{
		ror::Vector2ui m_sizes{10, 2048};
		ror::Vector4f  m_color{0.5f, 0.5f, 0.5f, 1.0f};
		bool           m_show_y_axis{false};
	};

	enum class GuiTheme
	{
		dark,
		light,
		classic
	};

	Grid m_grid{};

	struct Gui
	{
		uint32_t                 m_font_size{13};
		uint32_t                 m_scale{1};
		bool                     m_visible{true};
		bool                     m_show_anchors{true};
		bool                     m_show_gizmo{true};
		bool                     m_show_overlays{true};
		bool                     m_show_settings{true};
		bool                     m_show_debug{true};
		bool                     m_dump_texture{true};
		GuiTheme                 m_theme{GuiTheme::dark};
		uint32_t                 m_default_font{0};
		uint32_t                 m_vertex_buffer_size{2000000};        // Initial Size of vertex buffer
		uint32_t                 m_index_buffer_size{500000};          // Initial Size of index buffer
		float32_t                m_overlay_scale{1.5f};
		float32_t                m_gizmo_size{100.0f};
		float32_t                m_anchor_size{3.0f};
		float32_t                m_font_scale{0.45f};
		ror::Vector4f            m_anchor_color{0.1f, 0.7f, 0.1f, 1.0f};
		ror::Vector4f            m_anchor_click_color{0.7f, 0.1f, 0.1f, 1.0f};
		std::vector<std::string> m_fonts{};
		std::vector<std::string> m_render_modes{};
	};

	Gui m_gui{};

	Environment m_environment{};

  protected:
  private:
	BindingsSets m_bindings{};
};

FORCE_INLINE Settings &settings() noexcept
{
	static Settings sc{};        // Global settings container
	return sc;
}

FORCE_INLINE const std::string &render_modes() noexcept
{
	static std::string options{};

	if (options.empty())
		for (auto &render_mode : ror::settings().m_gui.m_render_modes)
			options += render_mode + '\0';

	return options;
}

FORCE_INLINE const std::string &render_mode(size_t a_index) noexcept
{
	auto &setting = ror::settings();

	assert(a_index < setting.m_gui.m_render_modes.size() && "Render mode index out of range");

	return setting.m_gui.m_render_modes[a_index];
}

FORCE_INLINE uint32_t metal_api_version() noexcept
{
	auto &setting = ror::settings();

	return ROAR_MAKE_VERSION(setting.m_metal.m_version_major, setting.m_metal.m_version_minor, 0);
}

FORCE_INLINE uint32_t vulkan_api_version() noexcept
{
	auto &setting = ror::settings();

	return ROAR_MAKE_VERSION(setting.m_vulkan.m_version_major, setting.m_vulkan.m_version_minor, 0);
}

// TODO: Add setting values
FORCE_INLINE constexpr uint32_t number_of_buffers()
{
	return 3;        // Tripple buffering
}

FORCE_INLINE constexpr bool visualise_mipmaps()
{
	return false;
}

FORCE_INLINE uint32_t multisample_count()
{
	return ror::settings().m_multisample_count;
}

FORCE_INLINE constexpr bool sample_rate_shading_enabled()
{
	return false;
}

FORCE_INLINE constexpr float sample_rate_shading()
{
	return 0.5f;
}

FORCE_INLINE auto vsync()
{
	// TODO: No vsync available on Android so when enabling make sure android is guarded
	return false;
}

FORCE_INLINE auto window_transparent()
{
	auto &setting = ror::settings();
	return setting.m_window.m_transparent;
}

FORCE_INLINE auto window_premultiplied()
{
	auto &setting = ror::settings();
	return setting.m_window.m_premultiplied;
}

FORCE_INLINE auto window_prerotated()
{
	auto &setting = ror::settings();
	return setting.m_window.m_prerotated;
}

}        // namespace ror
