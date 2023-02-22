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

#include "configuration/rorconfiguration.hpp"
#include "configuration/rorsettings_configuration.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "math/rorvector4.hpp"
#include "rhi/rortypes.hpp"
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace ror
{

static const auto bits_shift            = 16u;
static const auto bits_mask             = 0x0000FFFF;
static const auto generic_numbers_count = 10;

class ROAR_ENGINE_ITEM Settings final
{
  public:
	FORCE_INLINE           Settings(const Settings &a_other)      = delete;         //! Copy constructor
	FORCE_INLINE           Settings(Settings &&a_other) noexcept  = delete;         //! Move constructor
	FORCE_INLINE Settings &operator=(const Settings &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE Settings &operator=(Settings &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~Settings() noexcept                             = default;        //! Destructor

	FORCE_INLINE Settings()
	{
		auto setting = ror::get_settings();        // This is config settings that will load settings.json, then we cache it in Settings object

		this->m_roar_title      = setting.get<std::string>("title");
		this->m_roar_cache      = setting.get<std::string>("roar_cache");
		this->m_default_scene   = setting.get<std::string>("default_scene");
		this->m_buffers_format  = setting.get<std::string>("buffers_format");
		this->m_working_dir     = setting.get<std::string>("working_dir");
		this->m_renderer_config = setting.get<std::string>("renderer_config");

		this->m_zoom_speed  = setting.get<float32_t>("zoom_speed");
		this->m_depth_clear = setting.get<float32_t>("depth");

		this->m_unit                      = setting.get<uint32_t>("unit");
		this->m_threads_multiplier        = setting.get<uint32_t>("threads_multiplier");
		this->m_buffer_increment          = setting.get<uint32_t>("buffer_increment");
		this->m_multisample_count         = setting.get<uint32_t>("multisample_count");
		this->m_debug_mesh_count          = setting.get<uint32_t>("debug_mesh_count");
		this->m_gui_primitives_count      = setting.get<uint32_t>("gui_primitives_count");
		this->m_gui_primitives_index_size = setting.get<uint32_t>("gui_primitives_index_size");
		this->m_gui_primitives_size       = setting.get<uint32_t>("gui_primitives_size");
		this->m_debug_mesh_type           = static_cast<DebugMeshType>(setting.get<uint32_t>("debug_mesh_type"));

		for (size_t i = 0; i < generic_numbers_count; ++i)
			this->m_generic_numbers[i] = 0;

		this->m_clean_on_boot             = setting.get<bool>("clean_on_boot");
		this->m_visualise_mipmaps         = setting.get<bool>("visualise_mipmaps");
		this->m_vertical_sync             = setting.get<bool>("vsync");
		this->m_window_transparent        = setting.get<bool>("window_transparent");
		this->m_window_premultiplied      = setting.get<bool>("window_premultiplied");
		this->m_window_prerotated         = setting.get<bool>("window_prerotated");
		this->m_fullscreen                = setting.get<bool>("fullscreen");
		this->m_resizable                 = setting.get<bool>("resizable");
		this->m_force_rgba_textures       = setting.get<bool>("force_rgba_textures");
		this->m_background_srgb_to_linear = setting.get<bool>("background_to_srgb");
		this->m_force_linear_textures     = setting.get<bool>("force_linear_textures");
		this->m_animate_cpu               = setting.get<bool>("animate_cpu");
		this->m_clamp_material_roughness  = setting.get<bool>("clamp_material_roughness");
		this->m_clamp_material_metallic   = setting.get<bool>("clamp_material_metallic");
		this->m_show_axis                 = setting.get<bool>("show_axis");
		this->m_show_lights               = setting.get<bool>("show_lights");
		this->m_show_cameras              = setting.get<bool>("show_cameras");

		auto amc = setting.get<std::vector<float32_t>>("debug_mesh_color");
		if (amc.size() >= 4)
			this->m_debug_mesh_color = ror::Vector4f(amc[0], amc[1], amc[2], amc[3]);

		auto alc = setting.get<std::vector<float32_t>>("ambient_light_color");
		if (alc.size() >= 4)
			this->m_ambient_light_color = ror::Vector4f(alc[0], alc[1], alc[2], alc[3]);

		this->m_fog_enabled = setting.get<bool>("fog:enabled");

		auto foc = setting.get<std::vector<float32_t>>("fog:color");
		if (foc.size() >= 4)
			this->m_fog_color = ror::Vector4f(foc[0], foc[1], foc[2], foc[3]);

		this->m_fog_start = setting.get<float32_t>("fog:start");
		this->m_fog_end   = setting.get<float32_t>("fog:end");

		{
			auto x = setting.get<uint32_t>("window:x");
			auto y = setting.get<uint32_t>("window:y");
			auto w = setting.get<uint32_t>("window:width");
			auto h = setting.get<uint32_t>("window:height");

			if (x > 0 && y > 0 && w > 0 && h > 0)
				this->m_window_dimensions = ror::Vector4i(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(w), static_cast<int32_t>(h));
		}

		this->m_viewport = this->m_window_dimensions;

		auto winding = setting.get<std::string>("winding");

		if (winding != "")
			this->m_primitive_winding = rhi::string_to_primitive_winding(winding);

		auto color_format         = setting.get<std::string>("window:color_format");
		auto depth_stencil_format = setting.get<std::string>("window:depth_stencil_format");

		if (color_format != "")
			this->m_pixel_format = rhi::string_to_pixel_format(color_format);

		if (depth_stencil_format != "")
			this->m_depth_stencil_format = rhi::string_to_pixel_format(depth_stencil_format);

		this->m_window_buffer_count = setting.get<uint32_t>("window:buffer_count");

		{
			auto x = setting.get<uint32_t>("viewport:x");
			auto y = setting.get<uint32_t>("viewport:y");
			auto w = setting.get<uint32_t>("viewport:width");
			auto h = setting.get<uint32_t>("viewport:height");        // Height can be negative

			if (x > 0 && w > 0 && h > 0)
				this->m_viewport = ror::Vector4i(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(w), static_cast<int32_t>(h));
		}

		{
			auto x = setting.get<uint32_t>("grid:size");
			auto y = setting.get<uint32_t>("grid:spread");

			this->m_grid.m_sizes.x = x;
			this->m_grid.m_sizes.y = y;

			auto gco = setting.get<std::vector<float32_t>>("grid:color");
			if (gco.size() >= 4)
				this->m_grid.m_color = ror::Vector4f(gco[0], gco[1], gco[2], gco[3]);

			this->m_grid.m_show_y_axis = setting.get<bool>("grid:y_axis");
		}

		{
			this->m_gui.m_scale              = setting.get<uint32_t>("gui:scale");
			this->m_gui.m_visible            = setting.get<bool>("gui:visible");
			this->m_gui.m_default_font       = setting.get<uint32_t>("gui:default_font");
			this->m_gui.m_font_size          = setting.get<uint32_t>("gui:font_size");
			this->m_gui.m_vertex_buffer_size = setting.get<uint32_t>("gui:vertex_buffer_size");
			this->m_gui.m_index_buffer_size  = setting.get<uint32_t>("gui:index_buffer_size");
			this->m_gui.m_fonts              = setting.get<std::vector<std::string>>("gui:fonts");

			auto theme = setting.get<std::string>("gui:theme");
			if (theme == "classic")
				this->m_gui.m_theme = GuiTheme::classic;
			else if (theme == "dark")
				this->m_gui.m_theme = GuiTheme::dark;
			else if (theme == "light")
				this->m_gui.m_theme = GuiTheme::light;
			else
			{
				assert(0 && "Unknown theme in settings.json at gui:theme");
			};

			assert(this->m_gui.m_default_font < this->m_gui.m_fonts.size() && "Default font index is out of range");
		}

		auto bs_material_factors      = setting.get<std::vector<uint32_t>>("sets_bindings:material_factors");
		auto bs_per_frame_uniform     = setting.get<std::vector<uint32_t>>("sets_bindings:per_frame_uniform");
		auto bs_per_view_uniform      = setting.get<std::vector<uint32_t>>("sets_bindings:per_view_uniform");
		auto bs_directional_light     = setting.get<std::vector<uint32_t>>("sets_bindings:directional_light");
		auto bs_point_light           = setting.get<std::vector<uint32_t>>("sets_bindings:point_light");
		auto bs_spot_light            = setting.get<std::vector<uint32_t>>("sets_bindings:spot_light");
		auto bs_area_light            = setting.get<std::vector<uint32_t>>("sets_bindings:area_light");
		auto bs_skin_joints           = setting.get<std::vector<uint32_t>>("sets_bindings:skin_joints");
		auto bs_morph_weights         = setting.get<std::vector<uint32_t>>("sets_bindings:morph_weights");
		auto bs_node_transform_output = setting.get<std::vector<uint32_t>>("sets_bindings:node_transform_output");
		auto bs_node_transform_input  = setting.get<std::vector<uint32_t>>("sets_bindings:node_transform_input");
		auto bs_nodes_model           = setting.get<std::vector<uint32_t>>("sets_bindings:nodes_model");
		auto bs_nodes_index           = setting.get<std::vector<uint32_t>>("sets_bindings:nodes_index");
		auto bs_joint_inverse_bind    = setting.get<std::vector<uint32_t>>("sets_bindings:joint_inverse_bind");

		auto max_attributes               = setting.get<uint32_t>("sets_bindings:max_attributes");
		this->m_bindings.m_max_attributes = std::max(max_attributes, this->m_bindings.m_max_attributes);

		// clang-format off
		if (bs_material_factors.size() > 0)        this->m_bindings.m_material_factors      =   bs_material_factors[0] << bits_shift;
		if (bs_material_factors.size() > 1)        this->m_bindings.m_material_factors      |=  bs_material_factors[1];
		if (bs_per_frame_uniform.size() > 0)       this->m_bindings.m_per_frame_uniform     =   bs_per_frame_uniform[0] << bits_shift;
		if (bs_per_frame_uniform.size() > 1)       this->m_bindings.m_per_frame_uniform     |=  bs_per_frame_uniform[1];
		if (bs_per_view_uniform.size() > 0)        this->m_bindings.m_per_view_uniform      =   bs_per_view_uniform[0] << bits_shift;
		if (bs_per_view_uniform.size() > 1)        this->m_bindings.m_per_view_uniform      |=  bs_per_view_uniform[1];
		if (bs_directional_light.size() > 0)       this->m_bindings.m_directional_light     =   bs_directional_light[0] << bits_shift;
		if (bs_directional_light.size() > 1)       this->m_bindings.m_directional_light     |=  bs_directional_light[1];
		if (bs_point_light.size() > 0)             this->m_bindings.m_point_light           =   bs_point_light[0] << bits_shift;
		if (bs_point_light.size() > 1)             this->m_bindings.m_point_light           |=  bs_point_light[1];
		if (bs_spot_light.size() > 0)              this->m_bindings.m_spot_light            =   bs_spot_light[0] << bits_shift;
		if (bs_spot_light.size() > 1)              this->m_bindings.m_spot_light            |=  bs_spot_light[1];
		if (bs_area_light.size() > 0)              this->m_bindings.m_area_light            =   bs_area_light[0] << bits_shift;
		if (bs_area_light.size() > 1)              this->m_bindings.m_area_light            |=  bs_area_light[1];
		if (bs_skin_joints.size() > 0)             this->m_bindings.m_skin_joints           =   bs_skin_joints[0] << bits_shift;
		if (bs_skin_joints.size() > 1)             this->m_bindings.m_skin_joints           |=  bs_skin_joints[1];
		if (bs_morph_weights.size() > 0)           this->m_bindings.m_morph_weights         =   bs_morph_weights[0] << bits_shift;
		if (bs_morph_weights.size() > 1)           this->m_bindings.m_morph_weights         |=  bs_morph_weights[1];
		if (bs_node_transform_output.size() > 0)   this->m_bindings.m_node_transform_output =   bs_node_transform_output[0] << bits_shift;
		if (bs_node_transform_output.size() > 1)   this->m_bindings.m_node_transform_output |=  bs_node_transform_output[1];
		if (bs_node_transform_input.size() > 0)    this->m_bindings.m_node_transform_input  =   bs_node_transform_input[0] << bits_shift;
		if (bs_node_transform_input.size() > 1)    this->m_bindings.m_node_transform_input  |=  bs_node_transform_input[1];
		if (bs_nodes_model.size() > 0)             this->m_bindings.m_nodes_model           =   bs_nodes_model[0] << bits_shift;
		if (bs_nodes_model.size() > 1)             this->m_bindings.m_nodes_model           |=  bs_nodes_model[1];
		if (bs_nodes_index.size() > 0)             this->m_bindings.m_nodes_index           =   bs_nodes_index[0] << bits_shift;
		if (bs_nodes_index.size() > 1)             this->m_bindings.m_nodes_index           |=  bs_nodes_index[1];
		if (bs_joint_inverse_bind.size() > 0)      this->m_bindings.m_joint_inverse_bind    =   bs_joint_inverse_bind[0] << bits_shift;
		if (bs_joint_inverse_bind.size() > 1)      this->m_bindings.m_joint_inverse_bind    |=  bs_joint_inverse_bind[1];
		// clang-format on

		// assert(this->m_bindings.m_material_factors != 0 && "m_material_factors wasn't read properly");
		assert(this->m_bindings.m_per_frame_uniform != 0 && "m_per_frame_uniform wasn't read properly");
		assert(this->m_bindings.m_per_view_uniform != 0 && "m_per_view_uniform wasn't read properly");
		assert(this->m_bindings.m_directional_light != 0 && "m_directional_light wasn't read properly");
		assert(this->m_bindings.m_point_light != 0 && "m_point_light wasn't read properly");
		assert(this->m_bindings.m_spot_light != 0 && "m_spot_light wasn't read properly");
		assert(this->m_bindings.m_area_light != 0 && "m_area_light wasn't read properly");
		assert(this->m_bindings.m_skin_joints != 0 && "m_skin_joints wasn't read properly");
		assert(this->m_bindings.m_morph_weights != 0 && "m_morph_weights wasn't read properly");
		assert(this->m_bindings.m_node_transform_output != 0 && "m_node_transform_output wasn't read properly");
		assert(this->m_bindings.m_node_transform_input != 0 && "m_node_transform_input wasn't read properly");
		assert(this->m_bindings.m_nodes_model != 0 && "m_nodes_model wasn't read properly");
		assert(this->m_bindings.m_nodes_index != 0 && "m_nodes_index wasn't read properly");
		assert(this->m_bindings.m_joint_inverse_bind != 0 && "m_joint_inverse_bind wasn't read properly");
		assert(this->m_bindings.m_max_attributes != 0 && "m_max_attributes wasn't read properly");

		this->m_metal.version_major            = setting.get<uint32_t>("metal:version_major");
		this->m_metal.version_minor            = setting.get<uint32_t>("metal:version_minor");
		this->m_metal.argument_buffers         = setting.get<bool>("metal:argument_buffers");
		this->m_metal.decoration_bindings      = setting.get<bool>("metal:force_spirv_bindings");
		this->m_metal.indirect_command_buffers = setting.get<bool>("metal:indirect_command_buffers");

		this->m_vulkan.version_major = setting.get<uint32_t>("vulkan:version_major");
		this->m_vulkan.version_minor = setting.get<uint32_t>("vulkan:version_minor");

		this->m_print_generated_shaders = setting.get<bool>("print_generated_shaders");
		this->m_write_generated_shaders = setting.get<bool>("write_generated_shaders");
		this->m_generate_debug_mesh     = setting.get<bool>("generate_debug_mesh");
		this->m_generate_gui_mesh       = setting.get<bool>("generate_gui_mesh");
		this->m_generate_grid_mesh      = setting.get<bool>("generate_grid_mesh");

		this->m_clean_dirs = setting.get<std::vector<std::string>>("clean_dirs");

		auto attribs = setting.find("default_vertex_descriptor");

		this->m_default_vertex_descriptor.attributes.reserve(attribs.size());

		for (auto &att : attribs)
		{
			rhi::BufferSemantic semantic = rhi::get_format_semantic(att.begin().key());
			rhi::VertexFormat   format   = rhi::string_to_vertex_format(att.begin().value());

			this->m_default_vertex_descriptor.attributes.push_back({semantic, format});
		}
	}

	// clang-format off
	FORCE_INLINE constexpr auto max_attributes()                  const noexcept { return this->m_bindings.m_max_attributes + 1;                                             }

	FORCE_INLINE constexpr auto material_factors_binding()        const noexcept { return (this->m_bindings.m_material_factors & bits_mask) + this->max_attributes();        }
	FORCE_INLINE constexpr auto per_frame_uniform_binding()       const noexcept { return (this->m_bindings.m_per_frame_uniform & bits_mask) + this->max_attributes();       }
	FORCE_INLINE constexpr auto per_view_uniform_binding()        const noexcept { return (this->m_bindings.m_per_view_uniform & bits_mask) + this->max_attributes();        }
	FORCE_INLINE constexpr auto directional_light_binding()       const noexcept { return (this->m_bindings.m_directional_light & bits_mask) + this->max_attributes();       }
	FORCE_INLINE constexpr auto point_light_binding()             const noexcept { return (this->m_bindings.m_point_light & bits_mask) + this->max_attributes();             }
	FORCE_INLINE constexpr auto spot_light_binding()              const noexcept { return (this->m_bindings.m_spot_light & bits_mask) + this->max_attributes();              }
	FORCE_INLINE constexpr auto area_light_binding()              const noexcept { return (this->m_bindings.m_area_light & bits_mask) + this->max_attributes();              }
	FORCE_INLINE constexpr auto skin_joints_binding()             const noexcept { return (this->m_bindings.m_skin_joints & bits_mask) + this->max_attributes();             }
	FORCE_INLINE constexpr auto morph_weights_binding()           const noexcept { return (this->m_bindings.m_morph_weights & bits_mask) + this->max_attributes();           }
	FORCE_INLINE constexpr auto node_transform_output_binding()   const noexcept { return (this->m_bindings.m_node_transform_output & bits_mask) + this->max_attributes();   }
	FORCE_INLINE constexpr auto node_transform_input_binding()    const noexcept { return (this->m_bindings.m_node_transform_input & bits_mask) + this->max_attributes();    }
	FORCE_INLINE constexpr auto nodes_model_binding()             const noexcept { return (this->m_bindings.m_nodes_model & bits_mask) + this->max_attributes();             }
	FORCE_INLINE constexpr auto nodes_index_binding()             const noexcept { return (this->m_bindings.m_nodes_index & bits_mask) + this->max_attributes();             }
	FORCE_INLINE constexpr auto joint_inverse_bind_binding()      const noexcept { return (this->m_bindings.m_joint_inverse_bind & bits_mask) + this->max_attributes();      }

	FORCE_INLINE constexpr auto material_factors_set()            const noexcept { return (this->m_bindings.m_material_factors >> bits_shift);                               }
	FORCE_INLINE constexpr auto per_frame_uniform_set()           const noexcept { return (this->m_bindings.m_per_frame_uniform >> bits_shift);                              }
	FORCE_INLINE constexpr auto per_view_uniform_set()            const noexcept { return (this->m_bindings.m_per_view_uniform >> bits_shift);                               }
	FORCE_INLINE constexpr auto directional_light_set()           const noexcept { return (this->m_bindings.m_directional_light >> bits_shift);                              }
	FORCE_INLINE constexpr auto point_light_set()                 const noexcept { return (this->m_bindings.m_point_light >> bits_shift);                                    }
	FORCE_INLINE constexpr auto spot_light_set()                  const noexcept { return (this->m_bindings.m_spot_light >> bits_shift);                                     }
	FORCE_INLINE constexpr auto area_light_set()                  const noexcept { return (this->m_bindings.m_area_light >> bits_shift);                                     }
	FORCE_INLINE constexpr auto skin_joints_set()                 const noexcept { return (this->m_bindings.m_skin_joints >> bits_shift);                                    }
	FORCE_INLINE constexpr auto morph_weights_set()               const noexcept { return (this->m_bindings.m_morph_weights >> bits_shift);                                  }
	FORCE_INLINE constexpr auto node_transform_output_set()       const noexcept { return (this->m_bindings.m_node_transform_output >> bits_shift);                          }
	FORCE_INLINE constexpr auto node_transform_input_set()        const noexcept { return (this->m_bindings.m_node_transform_input >> bits_shift);                           }
	FORCE_INLINE constexpr auto nodes_model_set()                 const noexcept { return (this->m_bindings.m_nodes_model >> bits_shift);                                    }
	FORCE_INLINE constexpr auto nodes_index_set()                 const noexcept { return (this->m_bindings.m_nodes_index >> bits_shift);                                    }
	FORCE_INLINE constexpr auto joint_inverse_bind_set()          const noexcept { return (this->m_bindings.m_joint_inverse_bind >> bits_shift);                             }
	// clang-format on

	void setup_generic_numbers(EventSystem &a_event_system)
	{
		for (size_t i = 0; i < generic_numbers_count; ++i)
		{
			auto generic_increment = [this, i](Event) {
				this->m_generic_numbers[i]++;
			};
			auto generic_decrement = [this, i](Event) {
				this->m_generic_numbers[i]--;
			};

			const auto key_ni_cmd_clk = create_event_handle(EventType::keyboard, static_cast<EventCode>(ror::enum_to_type_cast(EventCode::n0) + i), EventModifier::command, EventState::click);
			const auto key_ni_ctr_clk = create_event_handle(EventType::keyboard, static_cast<EventCode>(ror::enum_to_type_cast(EventCode::n0) + i), EventModifier::control, EventState::click);
			a_event_system.subscribe(key_ni_cmd_clk, generic_increment);
			a_event_system.subscribe(key_ni_ctr_clk, generic_decrement);
		}
	}

	std::string m_roar_title{};
	std::string m_roar_cache{};
	std::string m_default_scene{};
	std::string m_buffers_format{};
	std::string m_working_dir{};
	std::string m_renderer_config{"renderer.json"};

	std::vector<std::string> m_clean_dirs{};        //! Directories to clean on boot

	float32_t m_zoom_speed{20.0f};
	float32_t m_fog_start{0.0f};
	float32_t m_fog_end{500.0f};
	float32_t m_depth_clear{1.0f};

	uint32_t m_unit{1};                      //! 1 == meter, 1000 == km etc, to use the unit multiply it with your quantities
	uint32_t m_threads_multiplier{2};        //! How many more threads should the job system create on top of available cores. Remember this is a multiplier
	uint32_t m_buffer_increment{1};
	uint32_t m_multisample_count{8};
	uint32_t m_debug_mesh_count{1000};
	uint32_t m_gui_primitives_count{10};                      // 10 times
	uint32_t m_gui_primitives_size{1000000};                  // About 1.0 MB size of the vertex buffer
	uint32_t m_gui_primitives_index_size{10000};              // Size of index buffer
	int32_t  m_generic_numbers[generic_numbers_count];        //! This is used to limit things or render a specific node etc, each number is decremented by Ctr + N and increment by CMD + N

	bool m_clean_on_boot{false};
	bool m_visualise_mipmaps{false};
	bool m_vertical_sync{false};
	bool m_window_transparent{false};
	bool m_window_premultiplied{false};
	bool m_window_prerotated{false};
	bool m_fullscreen{false};
	bool m_resizable{false};
	bool m_fog_enabled{false};
	bool m_force_rgba_textures{false};
	bool m_print_generated_shaders{false};
	bool m_write_generated_shaders{false};
	bool m_generate_debug_mesh{false};
	bool m_generate_gui_mesh{false};
	bool m_generate_grid_mesh{false};
	bool m_background_srgb_to_linear{false};
	bool m_force_linear_textures{false};
	bool m_animate_cpu{false};
	bool m_clamp_material_roughness{false};
	bool m_clamp_material_metallic{false};
	bool m_show_axis{false};
	bool m_show_lights{false};
	bool m_show_cameras{false};

	ror::Vector4f m_debug_mesh_color{1.0f, 0.2f, 0.2f, 0.5f};
	ror::Vector4f m_ambient_light_color{0.2f, 0.2f, 0.2f, 1.0f};
	ror::Vector4f m_fog_color{0.5f, 0.5f, 0.5f, 1.0f};

	ror::Vector4i m_window_dimensions{0, 0, 1024, 768};
	ror::Vector4i m_viewport{0, 0, 1024, 768};
	uint32_t      m_window_buffer_count{3};

	rhi::PixelFormat m_pixel_format{rhi::PixelFormat::r8g8b8a8_uint32_norm};
	rhi::PixelFormat m_depth_stencil_format{rhi::PixelFormat::depth24_norm_stencil8_uint32};

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
		// assumes set is 0 for all of the following
		uint32_t m_material_factors{0};
		uint32_t m_per_frame_uniform{1};
		uint32_t m_per_view_uniform{2};
		uint32_t m_directional_light{3};
		uint32_t m_point_light{4};
		uint32_t m_spot_light{5};
		uint32_t m_area_light{6};
		uint32_t m_skin_joints{7};
		uint32_t m_morph_weights{8};
		uint32_t m_node_transform_output{9};
		uint32_t m_node_transform_input{10};
		uint32_t m_nodes_model{11};
		uint32_t m_nodes_index{12};
		uint32_t m_joint_inverse_bind{13};
		uint32_t m_max_attributes{17};
	};

	struct Options
	{
		uint32_t version_major{3};
		uint32_t version_minor{0};
	};

	struct MetalOptions : public Options
	{
		bool argument_buffers{true};
		bool decoration_bindings{true};
		bool indirect_command_buffers{false};
	};

	struct VulkanOptions : public Options
	{};

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
		GuiTheme                 m_theme{GuiTheme::dark};
		uint32_t                 m_default_font{0};
		uint32_t                 m_vertex_buffer_size{2000000};        // Initial Size of vertex buffer
		uint32_t                 m_index_buffer_size{500000};          // Initial Size of index buffer
		std::vector<std::string> m_fonts{};
	};

	Gui m_gui{};

  protected:
  private:
	BindingsSets m_bindings{};
};

FORCE_INLINE Settings &settings() noexcept
{
	static Settings sc{};        // Global settings container
	return sc;
}
}        // namespace ror
