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

#include "configuration/rorsettings_configuration.hpp"
#include "foundation/rorsystem.hpp"
#include "rorsettings.hpp"

namespace ror
{

Settings::Settings()
{
	auto setting = ror::get_settings();        // This is config settings that will load settings.json, then we cache it in Settings object

	this->m_roar_title         = setting.get<std::string>("title");
	this->m_roar_cache         = setting.get<std::string>("roar_cache");
	this->m_default_scene      = setting.get<std::string>("default_scene");
	this->m_buffers_format     = setting.get<std::string>("buffers_format");
	this->m_working_dir        = setting.get<std::string>("working_dir");
	this->m_renderer_config    = setting.get<std::string>("renderer_config");
	this->m_application_name   = setting.get<std::string>("application_name");
	this->m_engine_name        = setting.get<std::string>("engine_name");
	this->m_shaders_watch_path = setting.get<std::string>("shaders_watch_path");

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
	this->m_resolve_includes_depth    = setting.get<uint32_t>("resolve_includes_depth");

	auto av = setting.get<std::vector<uint32_t>>("application_version");
	if (av.size() >= 3)
		this->m_application_version = ROAR_MAKE_VERSION(av[0], av[1], av[2]);

	auto ev = setting.get<std::vector<uint32_t>>("engine_version");
	if (ev.size() >= 3)
		this->m_engine_version = ROAR_MAKE_VERSION(ev[0], ev[1], ev[2]);

	this->m_debug_mesh_type = static_cast<DebugMeshType>(setting.get<uint32_t>("debug_mesh_type"));

	for (size_t i = 0; i < generic_numbers_count; ++i)
		this->m_generic_numbers[i] = 0;

	this->m_save_scene_state          = setting.get<bool>("save_scene_state");
	this->m_load_scene_state          = setting.get<bool>("load_scene_state");
	this->m_clean_on_boot             = setting.get<bool>("clean_on_boot");
	this->m_visualise_mipmaps         = setting.get<bool>("visualise_mipmaps");
	this->m_vertical_sync             = setting.get<bool>("vsync");
	this->m_window.m_transparent      = setting.get<bool>("window:transparent");
	this->m_window.m_premultiplied    = setting.get<bool>("window:premultiplied");
	this->m_window.m_prerotated       = setting.get<bool>("window:prerotated");
	this->m_fullscreen                = setting.get<bool>("fullscreen");
	this->m_resizable                 = setting.get<bool>("resizable");
	this->m_always_on_top             = setting.get<bool>("always_on_top");
	this->m_force_rgba_textures       = setting.get<bool>("force_rgba_textures");
	this->m_force_ldr_textures        = setting.get<bool>("force_ldr_textures");
	this->m_background_srgb_to_linear = setting.get<bool>("background_to_srgb");
	this->m_force_linear_textures     = setting.get<bool>("force_linear_textures");
	this->m_force_mipmapped_textures  = setting.get<bool>("force_mipmapped_textures");
	this->m_animate_cpu               = setting.get<bool>("animate_cpu");
	this->m_clamp_material_roughness  = setting.get<bool>("clamp_material_roughness");
	this->m_clamp_material_metallic   = setting.get<bool>("clamp_material_metallic");
	this->m_show_axis                 = setting.get<bool>("show_axis");

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

		if (w > 0 && h > 0)
			this->m_window.m_dimensions = ror::Vector4i(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(w), static_cast<int32_t>(h));
	}

	this->m_viewport = this->m_window.m_dimensions;

	auto winding = setting.get<std::string>("winding");

	if (winding != "")
		this->m_primitive_winding = rhi::string_to_primitive_winding(winding);

	auto color_format         = setting.get<std::string>("window:color_format");
	auto depth_stencil_format = setting.get<std::string>("window:depth_stencil_format");

	if (color_format != "")
		this->m_window.m_pixel_format = rhi::string_to_pixel_format(color_format);

	if (depth_stencil_format != "")
		this->m_window.m_depth_stencil_format = rhi::string_to_pixel_format(depth_stencil_format);

	this->m_window.m_buffer_count = setting.get<uint32_t>("window:buffer_count");

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
		this->m_gui.m_show_anchors       = setting.get<bool>("gui:show_anchors");
		this->m_gui.m_show_gizmo         = setting.get<bool>("gui:show_gizmo");
		this->m_gui.m_show_overlays      = setting.get<bool>("gui:show_overlays");
		this->m_gui.m_show_settings      = setting.get<bool>("gui:show_settings");
		this->m_gui.m_show_debug         = setting.get<bool>("gui:show_debug");
		this->m_gui.m_dump_texture       = setting.get<bool>("gui:dump_texture");
		this->m_gui.m_default_font       = setting.get<uint32_t>("gui:default_font");
		this->m_gui.m_font_size          = setting.get<uint32_t>("gui:font_size");
		this->m_gui.m_vertex_buffer_size = setting.get<uint32_t>("gui:vertex_buffer_size");
		this->m_gui.m_index_buffer_size  = setting.get<uint32_t>("gui:index_buffer_size");
		this->m_gui.m_anchor_size        = setting.get<float32_t>("gui:anchor_size");
		this->m_gui.m_overlay_scale      = setting.get<float32_t>("gui:overlay_scale");
		this->m_gui.m_gizmo_size         = setting.get<float32_t>("gui:gizmo_size");
		this->m_gui.m_fonts              = setting.get<std::vector<std::string>>("gui:fonts");
		this->m_gui.m_render_modes       = setting.get<std::vector<std::string>>("gui:render_modes");
		this->m_gui.m_font_scale         = setting.get<float32_t>("gui:font_scale");

		auto gco = setting.get<std::vector<float32_t>>("gui:anchor_color");
		if (gco.size() >= 4)
			this->m_gui.m_anchor_color = ror::Vector4f(gco[0], gco[1], gco[2], gco[3]);

		gco = setting.get<std::vector<float32_t>>("gui:anchor_click_color");
		if (gco.size() >= 4)
			this->m_gui.m_anchor_click_color = ror::Vector4f(gco[0], gco[1], gco[2], gco[3]);

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
		}

		assert(this->m_gui.m_default_font < this->m_gui.m_fonts.size() && "Default font index is out of range");
	}

	// Needs special handling now, for Metal and Vulkan
	std::string bindings_location = "";

	if (ror::get_render_api() == ror::RenderType::vulkan)
		bindings_location = "vulkan:";

	auto bs_shadowmap_sampler  = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:shadow_map_sampler");
	auto bs_brdf_sampler       = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:brdf_integration_sampler");
	auto bs_skybox_sampler     = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:skybox_sampler");
	auto bs_irradiance_sampler = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:irradiance_sampler");
	auto bs_radiance_sampler   = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:radiance_sampler");
	auto bs_material_samplers  = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:material_samplers");
	auto bs_material_factors   = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:material_factors");
	auto bs_per_frame_uniform  = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:per_frame_uniform");
	auto bs_per_view_uniform   = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:per_view_uniform");
	auto bs_nodes_models       = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:nodes_models");
	auto bs_nodes_offsets      = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:nodes_offsets");
	auto bs_directional_light  = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:directional_light");
	auto bs_point_light        = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:point_light");
	auto bs_spot_light         = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:spot_light");
	auto bs_area_light         = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:area_light");
	auto bs_skin_joints        = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:skin_joints");
	auto bs_morphs_weights     = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:morphs_weights");
	auto bs_joint_inverse_bind = setting.get<std::vector<uint32_t>>(bindings_location + "sets_bindings:joint_inverse_bind");

	auto max_attributes               = setting.get<uint32_t>(bindings_location + "sets_bindings:max_attributes");
	this->m_bindings.m_max_attributes = std::max(max_attributes, this->m_bindings.m_max_attributes);

	// clang-format off
	if (bs_shadowmap_sampler.size() > 0)       this->m_bindings.m_shadow_map_sampler          =   bs_shadowmap_sampler[0] << bits_shift;
	if (bs_shadowmap_sampler.size() > 1)       this->m_bindings.m_shadow_map_sampler          |=  bs_shadowmap_sampler[1];
	if (bs_brdf_sampler.size() > 0)            this->m_bindings.m_brdf_integration_sampler    =   bs_brdf_sampler[0] << bits_shift;
	if (bs_brdf_sampler.size() > 1)            this->m_bindings.m_brdf_integration_sampler    |=  bs_brdf_sampler[1];
	if (bs_skybox_sampler.size() > 0)          this->m_bindings.m_skybox_sampler              =   bs_skybox_sampler[0] << bits_shift;
	if (bs_skybox_sampler.size() > 1)          this->m_bindings.m_skybox_sampler              |=  bs_skybox_sampler[1];
	if (bs_irradiance_sampler.size() > 0)      this->m_bindings.m_irradiance_sampler          =   bs_irradiance_sampler[0] << bits_shift;
	if (bs_irradiance_sampler.size() > 1)      this->m_bindings.m_irradiance_sampler          |=  bs_irradiance_sampler[1];
	if (bs_radiance_sampler.size() > 0)        this->m_bindings.m_radiance_sampler            =   bs_radiance_sampler[0] << bits_shift;
	if (bs_radiance_sampler.size() > 1)        this->m_bindings.m_radiance_sampler            |=  bs_radiance_sampler[1];
	if (bs_material_samplers.size() > 0)       this->m_bindings.m_material_samplers           =   bs_material_samplers[0] << bits_shift;
	if (bs_material_samplers.size() > 1)       this->m_bindings.m_material_samplers           |=  bs_material_samplers[1];
	if (bs_material_factors.size() > 0)        this->m_bindings.m_material_factors            =   bs_material_factors[0] << bits_shift;
	if (bs_material_factors.size() > 1)        this->m_bindings.m_material_factors            |=  bs_material_factors[1];
	if (bs_per_frame_uniform.size() > 0)       this->m_bindings.m_per_frame_uniform           =   bs_per_frame_uniform[0] << bits_shift;
	if (bs_per_frame_uniform.size() > 1)       this->m_bindings.m_per_frame_uniform           |=  bs_per_frame_uniform[1];
	if (bs_per_view_uniform.size() > 0)        this->m_bindings.m_per_view_uniform            =   bs_per_view_uniform[0] << bits_shift;
	if (bs_per_view_uniform.size() > 1)        this->m_bindings.m_per_view_uniform            |=  bs_per_view_uniform[1];
	if (bs_nodes_models.size() > 0)            this->m_bindings.m_nodes_models                =   bs_nodes_models[0] << bits_shift;
	if (bs_nodes_models.size() > 1)            this->m_bindings.m_nodes_models                |=  bs_nodes_models[1];
	if (bs_nodes_offsets.size() > 0)           this->m_bindings.m_nodes_offsets               =   bs_nodes_offsets[0] << bits_shift;
	if (bs_nodes_offsets.size() > 1)           this->m_bindings.m_nodes_offsets               |=  bs_nodes_offsets[1];
	if (bs_directional_light.size() > 0)       this->m_bindings.m_directional_light           =   bs_directional_light[0] << bits_shift;
	if (bs_directional_light.size() > 1)       this->m_bindings.m_directional_light           |=  bs_directional_light[1];
	if (bs_point_light.size() > 0)             this->m_bindings.m_point_light                 =   bs_point_light[0] << bits_shift;
	if (bs_point_light.size() > 1)             this->m_bindings.m_point_light                 |=  bs_point_light[1];
	if (bs_spot_light.size() > 0)              this->m_bindings.m_spot_light                  =   bs_spot_light[0] << bits_shift;
	if (bs_spot_light.size() > 1)              this->m_bindings.m_spot_light                  |=  bs_spot_light[1];
	if (bs_area_light.size() > 0)              this->m_bindings.m_area_light                  =   bs_area_light[0] << bits_shift;
	if (bs_area_light.size() > 1)              this->m_bindings.m_area_light                  |=  bs_area_light[1];
	if (bs_skin_joints.size() > 0)             this->m_bindings.m_skin_joints                 =   bs_skin_joints[0] << bits_shift;
	if (bs_skin_joints.size() > 1)             this->m_bindings.m_skin_joints                 |=  bs_skin_joints[1];
	if (bs_morphs_weights.size() > 0)          this->m_bindings.m_morphs_weights              =   bs_morphs_weights[0] << bits_shift;
	if (bs_morphs_weights.size() > 1)          this->m_bindings.m_morphs_weights              |=  bs_morphs_weights[1];
	if (bs_joint_inverse_bind.size() > 0)      this->m_bindings.m_joint_inverse_bind          =   bs_joint_inverse_bind[0] << bits_shift;
	if (bs_joint_inverse_bind.size() > 1)      this->m_bindings.m_joint_inverse_bind          |=  bs_joint_inverse_bind[1];
	// clang-format on

	// assert(this->m_bindings.m_material_factors != 0 && "m_material_factors wasn't read properly");
	// assert(this->m_bindings.m_per_frame_uniform != 0 && "m_per_frame_uniform wasn't read properly");
	assert(this->m_bindings.m_per_view_uniform != 0 && "m_per_view_uniform wasn't read properly");
	assert(this->m_bindings.m_nodes_models != 0 && "m_nodes_models wasn't read properly");
	assert(this->m_bindings.m_nodes_offsets != 0 && "m_nodes_offsets wasn't read properly");
	assert(this->m_bindings.m_directional_light != 0 && "m_directional_light wasn't read properly");
	assert(this->m_bindings.m_point_light != 0 && "m_point_light wasn't read properly");
	assert(this->m_bindings.m_spot_light != 0 && "m_spot_light wasn't read properly");
	assert(this->m_bindings.m_area_light != 0 && "m_area_light wasn't read properly");
	assert(this->m_bindings.m_skin_joints != 0 && "m_skin_joints wasn't read properly");
	assert(this->m_bindings.m_morphs_weights != 0 && "m_morphs_weights wasn't read properly");
	assert(this->m_bindings.m_joint_inverse_bind != 0 && "m_joint_inverse_bind wasn't read properly");
	assert(this->m_bindings.m_max_attributes != 0 && "m_max_attributes wasn't read properly");

	this->m_metal.m_version_major          = setting.get<uint32_t>("metal:version_major");
	this->m_metal.m_version_minor          = setting.get<uint32_t>("metal:version_minor");
	this->m_metal.argument_buffers         = setting.get<bool>("metal:argument_buffers");
	this->m_metal.decoration_bindings      = setting.get<bool>("metal:force_spirv_bindings");
	this->m_metal.indirect_command_buffers = setting.get<bool>("metal:indirect_command_buffers");

	this->m_vulkan.m_descriptor_sets_size = setting.get<uint32_t>("vulkan:descriptor_sets_size");
	this->m_vulkan.m_descriptor_pool_size = setting.get<uint32_t>("vulkan:descriptor_pool_size");
	this->m_vulkan.m_version_major        = setting.get<uint32_t>("vulkan:version_major");
	this->m_vulkan.m_version_minor        = setting.get<uint32_t>("vulkan:version_minor");
	this->m_vulkan.m_validation           = setting.get<bool>("vulkan:validation");
	this->m_vulkan.m_api_dump             = setting.get<bool>("vulkan:api_dump");
	this->m_vulkan.m_verbose_log          = setting.get<bool>("vulkan:verbose_log");

	auto mode = setting.get<std::string>("vulkan:polygon_mode");

	if (mode == "fill")
		this->m_vulkan.m_polygon_mode = rhi::TriangleFillMode::fill;
	else if (mode == "lines")
		this->m_vulkan.m_polygon_mode = rhi::TriangleFillMode::lines;
	else
		assert(0 && "No polygon fill mode specified");

	this->m_print_generated_shaders          = setting.get<bool>("print_generated_shaders");
	this->m_print_generated_msl_shaders      = setting.get<bool>("print_generated_msl_shaders");
	this->m_print_generated_glsl_shaders     = setting.get<bool>("print_generated_glsl_shaders");
	this->m_print_generated_vertex_shaders   = setting.get<bool>("print_generated_vertex_shaders");
	this->m_print_generated_fragment_shaders = setting.get<bool>("print_generated_fragment_shaders");
	this->m_print_generated_compute_shaders  = setting.get<bool>("print_generated_compute_shaders");
	this->m_log_event_system                 = setting.get<bool>("log_event_system");
	this->m_warn_event_system                = setting.get<bool>("warn_event_system");
	this->m_write_generated_shaders          = setting.get<bool>("write_generated_shaders");
	this->m_watch_shaders                    = setting.get<bool>("watch_shaders");
	this->m_generate_debug_mesh              = setting.get<bool>("generate_debug_mesh");
	this->m_generate_gui_mesh                = setting.get<bool>("generate_gui_mesh");
	this->m_show_grid                        = setting.get<bool>("show_grid");
	this->m_generate_cube_mesh               = setting.get<bool>("generate_cube_mesh");
	this->m_generate_cube_map                = setting.get<bool>("generate_cube_map");
	this->m_generate_canonical_cube_map      = setting.get<bool>("generate_canonical_cube_map");
	this->m_invert_canonical_cube_map        = setting.get<bool>("invert_canonical_cube_map");
	this->m_generate_quad_mesh               = setting.get<bool>("generate_quad_mesh");
	this->m_generate_fullscreen_quad_mesh    = setting.get<bool>("generate_fullscreen_quad_mesh");
	this->m_generate_pink_textures           = setting.get<bool>("generate_pink_textures");

	this->m_clean_dirs = setting.get<std::vector<std::string>>("clean_dirs");

	auto attribs = setting.find("default_vertex_descriptor");

	this->m_default_vertex_descriptor.attributes.reserve(attribs.size());

	for (auto &att : attribs)
	{
		rhi::BufferSemantic semantic = rhi::get_format_semantic(att.begin().key());
		rhi::VertexFormat   format   = rhi::string_to_vertex_format(att.begin().value());

		this->m_default_vertex_descriptor.attributes.push_back({semantic, format});
	}

	this->m_environment.m_visible = setting.get<bool>("environment:visible");
	this->m_environment.m_mode    = get_environment_mode(setting.get<std::string>("environment:mode"));
	this->m_environment.m_mipmap  = setting.get<uint32_t>("environment:mipmap");
}

void Settings::setup_generic_numbers(EventSystem &a_event_system)
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

Settings::Environment::VisualizeMode Settings::get_environment_mode(std::string a_mode)
{
	if (a_mode == "skybox")
		return Settings::Environment::VisualizeMode::skybox;
	else if (a_mode == "irradiance")
		return Settings::Environment::VisualizeMode::irradiance;
	else if (a_mode == "radiance")
		return Settings::Environment::VisualizeMode::radiance;
	else if (a_mode == "brdf_lut")
		return Settings::Environment::VisualizeMode::brdf_lut;

	return Settings::Environment::VisualizeMode::skybox;
}

}        // namespace ror
