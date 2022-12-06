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

#include "camera/rorcamera.hpp"
#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/roranimation.hpp"
#include "graphics/rormesh.hpp"
#include "graphics/rormodel.hpp"
#include "graphics/rornode.hpp"
#include "graphics/rorscene.hpp"
#include "math/rorquaternion.hpp"
#include "math/rortransform.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include "profiling/rorlog.hpp"
#include "profiling/rortimer.hpp"
#include "renderer/rorrenderer.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rorhandles.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include "shader_system/rorshader_system.hpp"
#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ror
{

define_translation_unit_vtable(Scene)
{}

void Light::fill_shader_buffer()
{
	const uint32_t fixed_light_count = 2;        // TODO: needs to be moved out of here, at least 2 so we make an array
	assert(this->m_type != ror::Light::LightType::area && "Area lights not supported yet");

	switch (this->m_type)
	{
		case ror::Light::LightType::directional:
			this->m_shader_buffer.top_level().m_name = "directional_light_uniform";
			this->m_shader_buffer.binding(settings().directional_light_binding());
			this->m_shader_buffer.set(settings().directional_light_set());
			this->m_light_struct_name = "directional_lights";
			break;
		case ror::Light::LightType::point:
			this->m_shader_buffer.top_level().m_name = "point_light_uniform";
			this->m_shader_buffer.binding(settings().point_light_binding());
			this->m_shader_buffer.set(settings().point_light_set());
			this->m_light_struct_name = "point_lights";
			break;
		case ror::Light::LightType::spot:
			this->m_shader_buffer.top_level().m_name = "spot_light_uniform";
			this->m_shader_buffer.binding(settings().spot_light_binding());
			this->m_shader_buffer.set(settings().spot_light_set());
			this->m_light_struct_name = "spot_lights";
			break;
		case ror::Light::LightType::area:
			this->m_shader_buffer.top_level().m_name = "area_light_uniform";
			this->m_shader_buffer.binding(settings().area_light_binding());
			this->m_shader_buffer.set(settings().area_light_set());
			this->m_light_struct_name = "area_lights";
			break;
	}

	rhi::ShaderBufferTemplate::Struct light_type(this->m_light_struct_name, static_cast_safe<uint32_t>(fixed_light_count));

	light_type.add_entry("mvp", rhi::Format::float32_4x4, rhi::Layout::std140, 1);
	light_type.add_entry("color", rhi::Format::float32_3, rhi::Layout::std140, 1);

	if (this->m_type != ror::Light::LightType::directional)
		light_type.add_entry("position", rhi::Format::float32_3, rhi::Layout::std140, 1);

	if (this->m_type != ror::Light::LightType::point)
		light_type.add_entry("direction", rhi::Format::float32_3, rhi::Layout::std140, 1);

	light_type.add_entry("intensity", rhi::Format::float32_1, rhi::Layout::std140, 1);
	light_type.add_entry("range", rhi::Format::float32_1, rhi::Layout::std140, 1);

	if (this->m_type == ror::Light::LightType::spot)
	{
		light_type.add_entry("inner_angle", rhi::Format::float32_1, rhi::Layout::std140, 1);
		light_type.add_entry("outer_angle", rhi::Format::float32_1, rhi::Layout::std140, 1);
	}

	this->m_shader_buffer.add_struct(light_type);
}

void Light::update()
{
	this->m_shader_buffer.buffer_map();

	auto     stride      = this->m_shader_buffer.stride(this->m_light_struct_name);
	uint32_t light_index = 0;

	this->m_shader_buffer.update("mvp", &this->m_mvp.m_values, light_index, stride);
	this->m_shader_buffer.update("color", &this->m_color, light_index, stride);

	if (this->m_type != ror::Light::LightType::directional)
		this->m_shader_buffer.update("position", &this->m_position, light_index, stride);

	if (this->m_type != ror::Light::LightType::point)
		this->m_shader_buffer.update("direction", &this->m_direction, light_index, stride);

	this->m_shader_buffer.update("intensity", &this->m_intensity, light_index, stride);
	this->m_shader_buffer.update("range", &this->m_range, light_index, stride);

	if (this->m_type == ror::Light::LightType::spot)
	{
		this->m_shader_buffer.update("inner_angle", &this->m_inner_angle, light_index, stride);
		this->m_shader_buffer.update("outer_angle", &this->m_outer_angle, light_index, stride);
	}
	std::cout << "Her is the glsl string for light type = " << this->m_light_struct_name << "\n"
	          << this->m_shader_buffer.to_glsl_string();

	this->m_shader_buffer.buffer_unmap();
}

void Light::upload(rhi::Device &a_device)
{
	// Looking to create a UBO for directional light like below
	/*
	  const uint directional_lights_count = @;
	  struct light_type
	  {
	      vec3  color;
	      vec3  direction;
	      float intensity;
	      mat4  mvp;
	  };

	  layout(std140, set = @, binding = @) uniform directional_light_uniform
	  {
	      light_type lights[directional_lights_count];
	  } in_directional_light_uniforms;
	*/
	this->fill_shader_buffer();
	this->m_shader_buffer.upload(a_device);

	this->update();
}

Scene::Scene(std::filesystem::path a_level)
{
	this->load(a_level, ResourceSemantic::scenes);
}

template <typename _node_type>
FORCE_INLINE constexpr void copy_node(_node_type a_node, rhi::ShaderBuffer &a_shader_buffer, uint32_t a_node_index, uint32_t a_stride, size_t a_parent_offset, int32_t a_parent)
{
	a_shader_buffer.update("rotation", &a_node.m_trs_transform.m_rotation.x, a_node_index, a_stride);
	a_shader_buffer.update("translation", &a_node.m_trs_transform.m_translation.x, a_node_index, a_stride);
	a_shader_buffer.update("scale", &a_node.m_trs_transform.m_scale.x, a_node_index, a_stride);
	a_shader_buffer.update("scale_inverse", &a_node.m_trs_transform.m_scale.x, a_node_index, a_stride);

	ror::Vector4i parent_index{a_node.m_parent, 0, 0, 0};

	if (parent_index.x != -1)
		parent_index.x += static_cast_safe<int32_t>(a_parent_offset);
	else if (a_parent_offset != 0)
		parent_index.x = a_parent;

	a_shader_buffer.update("parent_index", &parent_index.x, a_node_index, a_stride);
}

auto copy_node_transforms(ror::Scene &a_scene, rhi::ShaderBuffer &a_input_buffer)
{
	auto &nodes      = a_scene.nodes();
	auto &nodes_data = a_scene.nodes_side_data();

	a_input_buffer.buffer_map();

	auto stride = a_input_buffer.stride("node_transform_in");

	uint32_t node_index    = 0;
	uint32_t parent_offset = 0;
	for (auto &node : nodes)
	{
		copy_node(node, a_input_buffer, node_index, stride, parent_offset, -1);
		node_index++;
	}

	int32_t parent_index = 0;
	for (auto &node : nodes_data)
	{
		if (node.m_model != -1)
		{
			parent_offset = node_index;

			auto &model_nodes = a_scene.models()[static_cast_safe<size_t>(node.m_model)].nodes();

			for (uint32_t model_node_index = 0; model_node_index < model_nodes.size(); ++model_node_index)
			{
				auto &n = model_nodes[model_node_index];
				copy_node(n, a_input_buffer, node_index, stride, parent_offset, parent_index);
				node_index++;
			}
		}

		parent_index++;
	}

	a_input_buffer.buffer_unmap();
	return node_index;
}

void Scene::compute_pass_walk_scene(rhi::ComputeCommandEncoder &a_command_encoder,
                                    rhi::Device                &a_device,
                                    rhi::BuffersPack           &a_buffers_pack,
                                    ror::Renderer              &a_renderer,
                                    const rhi::Rendersubpass   &a_subpass,
                                    Timer                      &a_timer,
                                    ror::EventSystem           &a_event_system)
{
	(void) a_device;
	(void) a_buffers_pack;
	(void) a_command_encoder;
	(void) a_subpass;
	(void) a_timer;
	(void) a_event_system;

	auto &compute_pso       = a_renderer.programs()[0];
	auto &input_buffers     = a_subpass.buffer_inputs();
	auto &trs_buffer        = input_buffers[0].m_render_output->m_target_reference.get();
	auto &per_frame_uniform = a_renderer.shader_buffer("per_frame_uniform");

	uint32_t ncount = static_cast_safe<uint32_t>(copy_node_transforms(*this, trs_buffer));

	uint32_t animation_size{0u};
	uint32_t animation_count{0u};
	uint32_t sampler_input_size{0u};
	uint32_t sampler_output_size{0u};
	uint32_t weights_output_size{0u};

	get_animation_sizes(*this, animation_size, animation_count, sampler_input_size, sampler_output_size, weights_output_size);

	static float32_t seconds{0.0f};
	seconds += static_cast<float32_t>(a_timer.tick_seconds());

	// TODO: After some time recent the seconds
	if (seconds > 1000.0f)        // TODO: Move the 1000 to settings
		seconds = static_cast<float32_t>(static_cast<int32_t>(seconds));

	per_frame_uniform->buffer_map();
	per_frame_uniform->update("delta_time", &seconds);
	per_frame_uniform->update("nodes_count", &ncount);
	per_frame_uniform->update("animations_count", &animation_count);
	per_frame_uniform->buffer_unmap();

	uint32_t node_matrices_size = static_cast_safe<uint32_t>(this->m_nodes.size());

	// Encode the pipeline state object and its parameters.
	a_command_encoder.compute_pipeline_state(compute_pso);
	trs_buffer.buffer_bind(a_command_encoder, rhi::ShaderStage::compute);
	per_frame_uniform->buffer_bind(a_command_encoder, rhi::ShaderStage::compute);

	// Calculate a threadgroup size.
	NS::UInteger max_thread_group_size = 1024;        // compute_pso->maxTotalThreadsPerThreadgroup();

	if (max_thread_group_size > node_matrices_size)
		max_thread_group_size = node_matrices_size;

	// Encode the compute command.
	a_command_encoder.dispatch_threads({node_matrices_size, 1, 1}, {static_cast<uint32_t>(max_thread_group_size), 1, 1});
}

struct DrawData
{
	rhi::BufferHybrid<rhi::Static> *positions{nullptr};
	rhi::BufferHybrid<rhi::Static> *texture_coords0{nullptr};
	rhi::BufferHybrid<rhi::Static> *texture_coords1{nullptr};
	rhi::BufferHybrid<rhi::Static> *texture_coords2{nullptr};
	rhi::BufferHybrid<rhi::Static> *normals{nullptr};
	rhi::BufferHybrid<rhi::Static> *bent_normals{nullptr};
	rhi::BufferHybrid<rhi::Static> *weights0{nullptr};
	rhi::BufferHybrid<rhi::Static> *weights1{nullptr};
	rhi::BufferHybrid<rhi::Static> *joint_ids0{nullptr};
	rhi::BufferHybrid<rhi::Static> *joint_ids1{nullptr};
	rhi::BufferHybrid<rhi::Static> *tangents{nullptr};
	rhi::BufferHybrid<rhi::Static> *colors0{nullptr};
	rhi::BufferHybrid<rhi::Static> *colors1{nullptr};
	rhi::BufferHybrid<rhi::Static> *morph_target{nullptr};
	rhi::BufferHybrid<rhi::Static> *morph_weights{nullptr};
	rhi::BufferHybrid<rhi::Static> *indices{nullptr};
	rhi::RenderCommandEncoder      *encoder{nullptr};
};

template <typename _type>
FORCE_INLINE void enable_material_component(const Material::Component<_type>                                            &a_component,
                                            std::vector<rhi::Texture, rhi::BufferAllocator<rhi::Texture>>               &a_textures,
                                            std::vector<rhi::TextureImage, rhi::BufferAllocator<rhi::TextureImage>>     &a_images,
                                            std::vector<rhi::TextureSampler, rhi::BufferAllocator<rhi::TextureSampler>> &a_samplers,
                                            uint32_t                                                                    &a_index,
                                            DrawData                                                                    &a_dd)
{
	if (a_component.m_texture != -1)
	{
		auto &texture = a_textures[static_cast_safe<size_t>(a_component.m_texture)];
		assert(texture.texture_image() != -1);
		auto &image   = a_images[static_cast_safe<size_t>(texture.texture_image())];
		auto &sampler = a_samplers[texture.texture_sampler()];

		a_dd.encoder->fragment_texture(image, a_index);
		a_dd.encoder->fragment_sampler(sampler, a_index++);        // This behaviour matches what's in the shader_system.cpp
	}
};

void render_mesh(ror::Model &a_model, ror::Mesh &a_mesh, DrawData &a_dd, const ror::Renderer &a_renderer, ror::Scene &a_scene, const rhi::Rendersubpass &subpass)
{
	(void) a_renderer;
	(void) subpass;
	(void) a_scene;

	auto &programs      = a_scene.programs();
	auto &pass_programs = programs.at(subpass.type());

	for (size_t prim_id = 0; prim_id < a_mesh.primitives_count(); ++prim_id)
	{
		auto &program = pass_programs[static_cast<size_t>(a_mesh.program(prim_id))];
		assert(a_mesh.material(prim_id) != -1);
		auto &material         = a_model.materials()[static_cast<uint32_t>(a_mesh.material(prim_id))];
		auto &material_factors = material.shader_buffer();
		// material_factors.bind(a_encoder, rhi::ShaderType::fragment, buffer_index_offset);
		material_factors.buffer_bind(*a_dd.encoder, rhi::ShaderStage::fragment);

		a_dd.encoder->render_pipeline_state(program);
		// TODO: Add joint_transforms trs_transforms UBO for skinned characters

		// Bind standard vertex attributes
		auto &vertex_attributes = a_mesh.vertex_descriptor(prim_id);
		for (auto &va : vertex_attributes.attributes())
		{
			rhi::Buffer *va_buffer{nullptr};
			if (va.semantics() == rhi::BufferSemantic::vertex_position)
				va_buffer = a_dd.positions;
			else if (va.semantics() == rhi::BufferSemantic::vertex_texture_coord_0)
				va_buffer = a_dd.texture_coords0;
			else if (va.semantics() == rhi::BufferSemantic::vertex_texture_coord_1)
				va_buffer = a_dd.texture_coords1;
			else if (va.semantics() == rhi::BufferSemantic::vertex_texture_coord_2)
				va_buffer = a_dd.texture_coords2;
			else if (va.semantics() == rhi::BufferSemantic::vertex_normal)
				va_buffer = a_dd.normals;
			else if (va.semantics() == rhi::BufferSemantic::vertex_bent_normal)
				va_buffer = a_dd.bent_normals;
			else if (va.semantics() == rhi::BufferSemantic::vertex_tangent)
				va_buffer = a_dd.tangents;
			else if (va.semantics() == rhi::BufferSemantic::vertex_color_0)
				va_buffer = a_dd.colors0;
			else if (va.semantics() == rhi::BufferSemantic::vertex_color_1)
				va_buffer = a_dd.colors1;
			else if (va.semantics() == rhi::BufferSemantic::vertex_bone_id_0)
				va_buffer = a_dd.joint_ids0;
			else if (va.semantics() == rhi::BufferSemantic::vertex_bone_id_1)
				va_buffer = a_dd.joint_ids1;
			else if (va.semantics() == rhi::BufferSemantic::vertex_weight_0)
				va_buffer = a_dd.weights0;
			else if (va.semantics() == rhi::BufferSemantic::vertex_weight_1)
				va_buffer = a_dd.weights1;
			else if (va.semantics() == rhi::BufferSemantic::vertex_index)
			{}
			else
				assert(0 && "Shouldn't reach here");

			if (va_buffer)
			{
				// Offset is 0 here because we have provided attribute offset in attribute descriptor
				a_dd.encoder->vertex_buffer(*va_buffer, 0, va.location());
			}
			else
			{
				assert(va.semantics() == rhi::BufferSemantic::vertex_index && "Attribute buffer is null for non-index buffer");
			}
		}

		// Bind morph targets vertex attributes
		auto &morph_target_vertex_descriptors = a_mesh.target_descriptor(prim_id);
		for (auto &morph_descriptor : morph_target_vertex_descriptors)
		{
			for (auto &va : morph_descriptor.attributes())
			{
				rhi::Buffer *va_buffer{nullptr};
				if (va.semantics() == rhi::BufferSemantic::vertex_position)
					va_buffer = a_dd.positions;
				else if (va.semantics() == rhi::BufferSemantic::vertex_normal)
					va_buffer = a_dd.normals;
				else if (va.semantics() == rhi::BufferSemantic::vertex_tangent)
					va_buffer = a_dd.tangents;
				else
				{
					assert(0 && "Shouldn't reach here. don't support any other morph target");
				}

				if (va_buffer)
				{
					a_dd.encoder->vertex_buffer(*va_buffer, 0, va.location());
				}
				else
				{
					assert(va.semantics() == rhi::BufferSemantic::vertex_index && "Attribute buffer is null for non-index buffer");
				}
			}
		}

		auto &textures = a_model.textures();
		auto &images   = a_model.images();
		auto &samplers = a_model.samplers();
		auto  binding_index{0u};

		enable_material_component(material.m_base_color, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_diffuse_color, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_specular_glossyness, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_emissive, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_anisotropy, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_transmission, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_sheen_color, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_sheen_roughness, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_clearcoat_normal, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_clearcoat, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_clearcoat_roughness, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_metallic, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_roughness, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_occlusion, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_normal, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_bent_normal, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_height, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_opacity, textures, images, samplers, binding_index, a_dd);
		enable_material_component(material.m_subsurface_color, textures, images, samplers, binding_index, a_dd);

		if (a_mesh.has_indices(prim_id))
		{
			auto &index_buffer_attribute = vertex_attributes.attribute(rhi::BufferSemantic::vertex_index);

			a_dd.encoder->draw_indexed_primitives(rhi::PrimitiveTopology::triangles,
			                                      index_buffer_attribute.count(),
			                                      index_buffer_attribute.format(),
			                                      *a_dd.indices,
			                                      static_cast_safe<uint32_t>(index_buffer_attribute.buffer_offset() + index_buffer_attribute.offset()));
		}
		else
		{
			auto &vertex_buffer_attribute = vertex_attributes.attribute(rhi::BufferSemantic::vertex_position);
			a_dd.encoder->draw_primitives(rhi::PrimitiveTopology::triangles, 0, vertex_buffer_attribute.count());
		}
	}
}

uint32_t animation_sampler_type(rhi::VertexFormat a_format)
{
	assert(a_format == rhi::VertexFormat::float32_4 ||
	       a_format == rhi::VertexFormat::float32_3 ||
	       a_format == rhi::VertexFormat::float32_1 ||
	       a_format == rhi::VertexFormat::int32_1 ||
	       a_format == rhi::VertexFormat::uint32_1 ||
	       a_format == rhi::VertexFormat::int16_1 ||
	       a_format == rhi::VertexFormat::uint16_1 ||
	       a_format == rhi::VertexFormat::int8_1 ||
	       a_format == rhi::VertexFormat::uint8_1 && "sampler format is not in the right format");

	if (a_format == rhi::VertexFormat::float32_4)
		return 0;
	else if (a_format == rhi::VertexFormat::float32_3)
		return 1;
	else if (a_format == rhi::VertexFormat::float32_1)
		return 2;
	else if (a_format == rhi::VertexFormat::int32_1)
		return 3;
	else if (a_format == rhi::VertexFormat::uint32_1)
		return 4;
	else if (a_format == rhi::VertexFormat::int16_1)
		return 5;
	else if (a_format == rhi::VertexFormat::uint16_1)
		return 6;
	else if (a_format == rhi::VertexFormat::int8_1)
		return 7;
	else if (a_format == rhi::VertexFormat::uint8_1)
		return 8;

	assert(0 && "Shouldn't reach here");

	return 0;
}

void get_animation_sizes(ror::Scene &a_scene,
                         uint32_t   &a_animation_size,
                         uint32_t   &a_animation_count,
                         uint32_t   &a_sampler_input_size,
                         uint32_t   &a_sampler_output_size,
                         uint32_t   &a_weights_output_size)
{
	// Lets upload animations data
	for (auto &node : a_scene.nodes_side_data())
	{
		if (node.m_model != -1)
		{
			auto &model = a_scene.models()[static_cast_safe<size_t>(node.m_model)];
			for (auto &model_node : model.nodes())
			{
				if (model_node.m_mesh_index != -1)
				{
					auto &mesh = model.meshes()[static_cast<size_t>(model_node.m_mesh_index)];
					if (mesh.has_morphs())
						a_weights_output_size += mesh.weights_count();
				}
			}

			auto current_anim{0u};
			for (auto &anim : model.animations())
			{
				a_animation_size += anim.m_channels.size();

				if (current_anim == node.m_animation)
					a_animation_count++;

				current_anim++;

				for (auto &sampler : anim.m_samplers)
				{
					a_sampler_input_size += sampler.m_input.size();
					a_sampler_output_size += sampler.m_output.size() / sizeof(float32_t);
					// a_sampler_output_size = ror::align16(a_sampler_output_size); // TODO: Fix me when other types are supported their start should be aligned
				}
			}
		}
	}
}

void fill_morph_weights(ror::Scene &a_scene, rhi::ShaderBuffer &a_shader_buffer, uint32_t a_weights_size)
{
	a_shader_buffer.buffer_map();

	auto stride = a_shader_buffer.stride("morph_weights");
	assert(stride == sizeof(float32_t) && "morph_weights are not packed");
	(void) stride;

	auto morph_offset{0u};
	for (auto &node : a_scene.nodes_side_data())
	{
		if (node.m_model != -1)
		{
			auto &model = a_scene.models()[static_cast_safe<size_t>(node.m_model)];
			for (auto &model_node : model.nodes())
			{
				if (model_node.m_mesh_index != -1)
				{
					auto &mesh = model.meshes()[static_cast<size_t>(model_node.m_mesh_index)];
					if (mesh.has_morphs())
					{
						a_shader_buffer.update("morph_weights", morph_offset * sizeof(float32_t), mesh.weights().data(), static_cast_safe<uint32_t>(mesh.weights().size() * sizeof(float32_t)));
						morph_offset += mesh.weights_count();
					}
				}
			}
		}
	}

	a_shader_buffer.buffer_unmap();

	(void) a_weights_size;
	assert(a_weights_size == morph_offset && "Not all of the weights are copied");
}

std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> morph_weights_offsets_count(ror::Scene &a_scene)
{
	std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> output;

	uint32_t node_offset{static_cast_safe<uint32_t>(a_scene.nodes().size())};

	auto morph_offset{0u};
	for (auto &node : a_scene.nodes_side_data())
	{
		if (node.m_model != -1)
		{
			auto &model = a_scene.models()[static_cast_safe<size_t>(node.m_model)];
			for (auto &model_node : model.nodes())
			{
				if (model_node.m_mesh_index != -1)
				{
					auto &mesh = model.meshes()[static_cast<size_t>(model_node.m_mesh_index)];
					if (mesh.has_morphs())
					{
						output[node_offset] = std::make_pair(morph_offset, mesh.weights_count());
						morph_offset += mesh.weights_count();
					}
				}
				node_offset++;
			}
		}
	}

	return output;
}

void fill_animation_buffers(ror::Scene &a_scene, ror::Renderer &a_renderer)
{
	auto animation_buffer         = a_renderer.shader_buffer("animations");
	auto sampler_input_buffer     = a_renderer.shader_buffer("animations_sampler_input");
	auto sampler_output_buffer    = a_renderer.shader_buffer("animations_sampler_output");
	auto current_animation_buffer = a_renderer.shader_buffer("current_animations");

	auto anim_size = sizeof(ror::Vector4ui) * 3;
	auto animation_size{0u};
	auto animation_count{0u};
	auto sampler_input_size{0u};
	auto sampler_output_size{0u};
	auto weights_output_size{0u};

	// This returns amounts of floats in input and output, output might still be float[1-3]
	get_animation_sizes(a_scene, animation_size, animation_count, sampler_input_size, sampler_output_size, weights_output_size);

	std::vector<uint8_t>        anim_data;
	std::vector<ror::Vector2ui> current_anim_data;
	std::vector<float32_t>      sampler_input_data;
	std::vector<float32_t>      sampler_output_data;
	std::vector<float32_t>      weights_output_data;

	anim_data.resize(animation_size * anim_size);        // For all animations, the animation struct has 2 uvec4s
	current_anim_data.reserve(animation_size);           // Very conservative, reserve for all animatoins
	sampler_input_data.resize(sampler_input_size);
	sampler_output_data.resize(sampler_output_size);
	weights_output_data.reserve(weights_output_size);

	auto anim_data_ptr           = anim_data.data();
	auto sampler_input_data_ptr  = reinterpret_cast<uint8_t *>(sampler_input_data.data());
	auto sampler_output_data_ptr = reinterpret_cast<uint8_t *>(sampler_output_data.data());

	auto     sampler_input_offset{0u};
	auto     sampler_output_offset{0u};
	uint32_t model_anim_index{0u};

	uint32_t node_offset{static_cast_safe<uint32_t>(a_scene.nodes().size())};

	auto node_to_offset{morph_weights_offsets_count(a_scene)};

	auto weights_offset{0u};
	auto weights_count{0u};
	for (auto &node : a_scene.nodes_side_data())
	{
		if (node.m_model != -1)
		{
			auto &model = a_scene.models()[static_cast_safe<size_t>(node.m_model)];
			auto  current_anim{0u};
			for (auto &anim : model.animations())
			{
				if (current_anim == node.m_animation)
					current_anim_data.emplace_back(model_anim_index, static_cast<uint32_t>(anim.m_channels.size()));

				model_anim_index += anim.m_channels.size();
				for (auto &chanl : anim.m_channels)
				{
					auto &anim_sampler = anim.m_samplers[chanl.m_sampler_index];

					// Encode channel for the compute shader
					ror::Vector4ui channel{chanl.m_sampler_index,
					                       static_cast_safe<uint32_t>(anim_sampler.m_input.size()),
					                       chanl.m_target_node_index + node_offset,
					                       ror::enum_to_type_cast(chanl.m_target_node_path)};

					// Encode sampler for the compute shader
					ror::Vector4ui sampler{*reinterpret_cast<const uint32_t *>(&anim_sampler.m_minimum),
					                       *reinterpret_cast<const uint32_t *>(&anim_sampler.m_maximum),
					                       animation_sampler_type(anim_sampler.m_output_format),
					                       ror::enum_to_type_cast(anim_sampler.m_interpolation)};

					auto i = node_to_offset.find(chanl.m_target_node_index + node_offset);
					if (i != node_to_offset.end())
					{
						weights_offset = i->second.first;
						weights_count  = i->second.second;
					}
					else
					{
						weights_offset = 0u;
						weights_count  = 0u;
					}

					// Encode offets
					ror::Vector4ui offsets{sampler_input_offset,
					                       sampler_output_offset,
					                       weights_offset,
					                       weights_count};

					// Lets advance the offset
					sampler_input_offset += anim_sampler.m_input.size();
					sampler_output_offset += anim_sampler.m_output.size() / sizeof(float32_t);

					std::memcpy(anim_data_ptr, &channel.x, sizeof(ror::Vector4ui));
					anim_data_ptr += sizeof(ror::Vector4ui);

					std::memcpy(anim_data_ptr, &sampler.x, sizeof(ror::Vector4ui));
					anim_data_ptr += sizeof(ror::Vector4ui);

					std::memcpy(anim_data_ptr, &offsets.x, sizeof(ror::Vector4ui));
					anim_data_ptr += sizeof(ror::Vector4ui);

					// Lets copy its sampler as well
					auto input_size  = anim_sampler.m_input.size() * sizeof(float32_t);
					auto output_size = anim_sampler.m_output.size();

					std::memcpy(sampler_input_data_ptr, anim_sampler.m_input.data(), input_size);
					std::memcpy(sampler_output_data_ptr, anim_sampler.m_output.data(), output_size);

					sampler_input_data_ptr += input_size;
					sampler_output_data_ptr += output_size;
				}
				current_anim++;
			}
			node_offset += static_cast_safe<uint32_t>(model.nodes().size());
		}
	}

	{
		auto stride = animation_buffer->stride("animation");

		anim_data_ptr = anim_data.data();

		animation_buffer->buffer_map();
		for (uint32_t i = 0; i < animation_size; ++i)
		{
			auto anim_data_offset = i * anim_size;
			auto anim_ptr         = anim_data_ptr + anim_data_offset;

			animation_buffer->update("animation_channel", anim_ptr, i, stride);

			anim_ptr += sizeof(ror::Vector4ui);
			animation_buffer->update("animation_sampler", anim_ptr, i, stride);

			anim_ptr += sizeof(ror::Vector4ui);
			animation_buffer->update("node_offsets", anim_ptr, i, stride);
		}
		animation_buffer->buffer_unmap();
	}

	{
		auto stride = current_animation_buffer->stride("animation");
		assert(animation_count == current_anim_data.size() && "Animations not fully collected");
		assert(stride == sizeof(ror::Vector2ui) && "Stride is wrong");
		(void) stride;
		current_animation_buffer->buffer_map();

		auto i{0u};
		for (auto &canim : current_anim_data)
			current_animation_buffer->update("animation", &canim.x, i++, stride);

		current_animation_buffer->buffer_unmap();
	}

	if (animation_size)
	{
		sampler_input_data_ptr = reinterpret_cast<uint8_t *>(sampler_input_data.data());

		sampler_input_buffer->buffer_map();
		sampler_input_buffer->update("inputs", sampler_input_data_ptr, static_cast_safe<uint32_t>(sampler_input_data.size() * sizeof(float32_t)));
		sampler_input_buffer->buffer_unmap();
	}

	if (animation_size)
	{
		sampler_output_data_ptr = reinterpret_cast<uint8_t *>(sampler_output_data.data());

		sampler_output_buffer->buffer_map();
		sampler_output_buffer->update("outputs", sampler_output_data_ptr, static_cast_safe<uint32_t>(sampler_output_data.size() * sizeof(float32_t)));
		sampler_output_buffer->buffer_unmap();
	}
}

void Scene::pre_render(rhi::RenderCommandEncoder &a_encoder, rhi::BuffersPack &a_buffers_pack, ror::Renderer &a_renderer, const rhi::Rendersubpass &a_subpass)
{
	(void) a_buffers_pack;
	(void) a_subpass;
	(void) a_renderer;
	(void) a_encoder;

	if (this->m_indices_dirty)
	{
		ror::Vector4ui node_index{0};        // Index contains index of the node in the resolved matrix array, and index offset for other things like joints
		for (auto &node : this->m_nodes_data)
		{
			node.update_offsets(node_index);
			node_index.x++;
		}

		auto morph_offset{0u};
		for (auto &node : this->m_nodes_data)
		{
			if (node.m_model != -1)
			{
				node_index.y = node_index.x;

				auto &model       = this->m_models[static_cast_safe<size_t>(node.m_model)];
				auto &model_nodes = model.nodes_side_data();

				for (uint32_t model_node_index = 0; model_node_index < model_nodes.size(); ++model_node_index)
				{
					auto &model_node = model.nodes()[model_node_index];
					if (model_node.m_mesh_index != -1)
					{
						auto &mesh = model.meshes()[static_cast<size_t>(model_node.m_mesh_index)];
						if (mesh.has_morphs())
						{
							node_index.z = morph_offset;
							morph_offset += mesh.weights_count();
						}
						else
						{
							node_index.z = 0;
						}
					}
					model_nodes[model_node_index].update_offsets(node_index);
					node_index.x++;
				}
			}
		}
		this->m_indices_dirty = false;
	}

	static bool first_time = true;        // TODO: Remove and add somewhere else
	if (first_time)
	{
		fill_animation_buffers(*this, a_renderer);
		first_time = false;
	}
}

void Scene::render(rhi::RenderCommandEncoder &a_encoder, rhi::BuffersPack &a_buffers_pack, ror::Renderer &a_renderer, const rhi::Rendersubpass &subpass)
{
	DrawData dd;

	dd.positions       = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_position);
	dd.texture_coords0 = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_texture_coord_0);
	dd.texture_coords1 = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_texture_coord_1);
	dd.texture_coords2 = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_texture_coord_2);
	dd.normals         = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_normal);
	dd.bent_normals    = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_bent_normal);
	dd.weights0        = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_weight_0);
	dd.weights1        = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_weight_1);
	dd.joint_ids0      = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_bone_id_0);
	dd.joint_ids1      = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_bone_id_1);
	dd.tangents        = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_tangent);
	dd.colors0         = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_color_0);
	dd.colors1         = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_color_1);
	dd.morph_target    = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_morph_target);
	dd.morph_weights   = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_morph_weight);
	dd.indices         = &a_buffers_pack.buffer(rhi::BufferSemantic::vertex_index);

	assert(dd.positions);
	assert(dd.texture_coords0);
	assert(dd.texture_coords1);
	assert(dd.texture_coords2);
	assert(dd.normals);
	assert(dd.bent_normals);
	assert(dd.weights0);
	assert(dd.weights1);
	assert(dd.joint_ids0);
	assert(dd.joint_ids1);
	assert(dd.tangents);
	assert(dd.colors0);
	assert(dd.colors1);
	assert(dd.morph_target);
	assert(dd.morph_weights);
	assert(dd.indices);

	dd.encoder = &a_encoder;

	// TODO: Abstract out these buffers and magic numbers
	auto &per_view_uniforms          = this->m_cameras[0].shader_buffer();
	auto &directional_light_uniforms = this->m_lights[0].shader_buffer();
	auto &spot_light_uniforms        = this->m_lights[1].shader_buffer();
	auto &point_light_uniforms       = this->m_lights[2].shader_buffer();

	auto &weights_shader_buffer = a_renderer.shader_buffer("morphs_weights");

	// Vertex shader bindings
	per_view_uniforms.buffer_bind(a_encoder, rhi::ShaderStage::vertex);

	// Fragment shader bindings
	per_view_uniforms.buffer_bind(a_encoder, rhi::ShaderStage::fragment);
	directional_light_uniforms.buffer_bind(a_encoder, rhi::ShaderStage::fragment);
	point_light_uniforms.buffer_bind(a_encoder, rhi::ShaderStage::fragment);
	spot_light_uniforms.buffer_bind(a_encoder, rhi::ShaderStage::fragment);

	auto &index_buffer_out = a_renderer.buffers()[3];
	index_buffer_out.buffer_bind(a_encoder, rhi::ShaderStage::vertex);

	this->pre_render(a_encoder, a_buffers_pack, a_renderer, subpass);

	// Render the scene graph
	for (auto &node : this->m_nodes_data)
	{
		if (node.m_model != -1)
		{
			auto &model            = this->m_models[static_cast_safe<size_t>(node.m_model)];
			auto &meshes           = model.meshes();
			auto &model_nodes_data = model.nodes_side_data();

			size_t node_data_index = 0;
			for (auto &model_node : model.nodes())
			{
				if (model_node.m_mesh_index != -1)
				{
					auto &mesh = meshes[static_cast<size_t>(model_node.m_mesh_index)];
					model_nodes_data[node_data_index].bind(a_encoder, rhi::ShaderStage::vertex);
					if (mesh.skin_index() != -1 && model_node.m_skin_index != -1)
					{
						auto &skin = model.skins()[static_cast<size_t>(model_node.m_skin_index)];
						skin.bind_joint_offset_buffer(a_encoder, rhi::ShaderStage::vertex);
						skin.bind_inverse_bind_buffer(a_encoder, rhi::ShaderStage::vertex);
					}

					if (mesh.has_morphs())
					{
						weights_shader_buffer->buffer_bind(a_encoder, rhi::ShaderStage::vertex);
					}

					render_mesh(model, mesh, dd, a_renderer, *this, subpass);
				}
				node_data_index++;
			}
		}
	}
}

void Scene::update(double64_t a_milli_seconds)
{
	(void) a_milli_seconds;
}

void Scene::load_models(ror::JobSystem &a_job_system, rhi::Device &a_device, const ror::Renderer &a_renderer)
{
	auto model_nodes{0u};
	for (auto &node : this->m_nodes_data)
		if (node.m_model_path != "")
			model_nodes++;

	if (model_nodes > 0)
	{
		this->m_models.resize(model_nodes);        // NOTE: I am resizing the models vector because I don't want many threads to emplace to it at the same time
		std::vector<ror::JobHandle<bool>> job_handles;
		job_handles.reserve(model_nodes * 2);        // Multiplied by 2 because I am creating two jobs, load and upload per model

		auto model_load_job = [this](const std::string &node_model_path, size_t a_model_index) -> auto
		{
			log_info("Loading model {}", node_model_path.c_str());
			Model &model = this->m_models[a_model_index];
			model.load_from_gltf_file(node_model_path, this->m_cameras);
			this->m_bounding_box.add_bounding(model.bounding_box());

			return true;
		};

		auto model_upload_job = [this, &a_device](size_t a_index) -> auto
		{
			Model &model = this->m_models[a_index];
			model.upload(a_device);
			return true;
		};

		auto model_index{0u};
		for (auto &node : this->m_nodes_data)
		{
			if (node.m_model_path != "")
			{
				auto load_job_handle   = a_job_system.push_job(model_load_job, node.m_model_path, model_index);
				auto upload_job_handle = a_job_system.push_job(model_upload_job, load_job_handle.job(), model_index);
				job_handles.emplace_back(std::move(load_job_handle));
				job_handles.emplace_back(std::move(upload_job_handle));
				model_index++;
			}
		}

		assert(model_index == model_nodes && "Models count vs how many are loaded doesn't match");

		// Wait for all jobs to finish
		for (auto &jh : job_handles)
			if (!jh.data())
				ror::log_critical("Can't load models specified in the scene.");
	}

	// Lets kick off shader generation while we upload the buffers
	auto shader_gen_job_handle = a_job_system.push_job([this, &a_renderer, &a_job_system]() -> auto{ this->generate_shaders(a_renderer, a_job_system); return true; });

	// By this time the buffer pack should be primed and filled with all kinds of geometry and animatiom data, lets upload it, all in one go
	// TODO: find out this might need to be done differently for Vulkan, also should be moved to upload()
	auto &bpack = rhi::get_buffers_pack();
	bpack.upload(a_device);

	if (!shader_gen_job_handle.data())
		ror::log_critical("Can't generate model shaders.");
}

hash_64_t pass_aware_vertex_hash(rhi::RenderpassType a_passtype, const ror::Mesh &a_mesh, size_t a_prim_index, const std::vector<ror::Skin, rhi::BufferAllocator<ror::Skin>> &a_skins)
{
	// NOTE: If every need to change how this hashing works, make sure to change in rormodel.cpp hash gen as well.
	// Ideally it should be abstracted out
	hash_64_t vertex_hash{};

	auto &vertex_attribute_descriptor              = a_mesh.vertex_descriptor(a_prim_index);
	auto &morph_target_vertex_attribute_descriptor = a_mesh.target_descriptor(a_prim_index);

	// Setup vertex hash
	vertex_hash = vertex_attribute_descriptor.hash_64_pass_aware(a_passtype);

	for (auto &attrib : morph_target_vertex_attribute_descriptor)
		hash_combine_64(vertex_hash, attrib.hash_64_pass_aware(a_passtype));

	// Only check if we have weights
	auto weights_count = a_mesh.weights_count();

	if (weights_count > 0)
		hash_combine_64(vertex_hash, hash_64(&weights_count, sizeof(weights_count)));

	auto skin_index = a_mesh.skin_index();
	if (skin_index != -1)
	{
		const auto &skin = a_skins[ror::static_cast_safe<size_t>(skin_index)];
		hash_combine_64(vertex_hash, skin.joints_count());
	}

	return vertex_hash;
}

hash_64_t pass_aware_fragment_hash(hash_64_t a_fragment_hash, hash_64_t a_vertex_hash)
{
	// Since a_vertex_hash is already pass aware, nothing more to do here but to combine existing fragment has with vertex hash
	auto fragment_hash = a_vertex_hash;
	hash_combine_64(fragment_hash, a_fragment_hash);

	return fragment_hash;
}

void Scene::generate_shaders(const ror::Renderer &a_renderer, ror::JobSystem &a_job_system)
{
	const std::vector<rhi::RenderpassType> render_pass_types = a_renderer.render_pass_types();

	size_t shaders_count = 0;

	for (auto &model : this->m_models)
		for (auto &mesh : model.meshes())
			shaders_count += mesh.primitives_count();

	// For each render pass in the framegraph create programs the model meshes can use
	bool has_shadows = false;

	for (auto &passtype : render_pass_types)
		if (passtype == rhi::RenderpassType::shadow)
			has_shadows = true;

	// This should be shaders_count * 2 * render_passes.size();
	// Then fill me up in a loop before entering the next loop with jobs in it
	this->m_shaders.reserve(shaders_count * 2 * render_pass_types.size());

	log_warn("About to create {} shaders ", shaders_count * 2 * render_pass_types.size());

	// Two pass approach, first create all the shaders into m_shaders, then allocate each to a job to fill it in with data
	std::unordered_map<hash_64_t, std::atomic_flag> shader_hash_to_flag{};
	std::unordered_map<hash_64_t, size_t>           shader_hash_to_index{};
	std::unordered_set<hash_64_t>                   unique_shaders{};

#define create_hash_to_index(shader_hash, shader_type, extension)                                           \
	{                                                                                                       \
		auto res = unique_shaders.emplace(shader_hash);                                                     \
		if (res.second)                                                                                     \
		{                                                                                                   \
			shader_hash_to_index[shader_hash] = this->m_shaders.size();                                     \
			shader_hash_to_flag.emplace(shader_hash, false);                                                \
			const auto shader_name = std::string(std::to_string(shader_hash) + pass_string + extension);    \
			this->m_shaders.emplace_back(shader_name, shader_hash, shader_type, ror::ResourceAction::make); \
		}                                                                                                   \
	}                                                                                                       \
	(void) 0

	for (auto &passtype : render_pass_types)
	{
		this->m_programs[passtype] = {};
		auto &pass_programs        = this->m_programs[passtype];
		auto  pass_string          = std::string("_") + rhi::renderpass_type_to_string(passtype);
		pass_programs.reserve(shaders_count);        // Pre-reserve so no reallocation happen under the hood while jobs are using it, times 2 for both vertex and fragment

		// Create vertex and fragment shaders but don't generate sources yet
		for (auto &model : this->m_models)
		{
			uint32_t mesh_index = 0;
			for (auto &mesh : model.meshes())
			{
				for (size_t prim_index = 0; prim_index < mesh.primitives_count(); ++prim_index)
				{
					// auto vs_hash = mesh.vertex_hash(prim_index);
					auto vs_hash     = pass_aware_vertex_hash(passtype, mesh, prim_index, model.skins());
					auto old_fs_hash = mesh.fragment_hash(prim_index);
					auto fs_hash     = pass_aware_fragment_hash(old_fs_hash, vs_hash);

					create_hash_to_index(vs_hash, rhi::ShaderType::vertex, ".vert");
					create_hash_to_index(fs_hash, rhi::ShaderType::fragment, ".frag");

					pass_programs.emplace_back(shader_hash_to_index[vs_hash], shader_hash_to_index[fs_hash]);

					// This is set once for the first render pass but it must stay the same for all of the rest because every pass must contain the same amount of shaders
					if (mesh.program(prim_index) == -1)
						model.update_mesh_program(mesh_index, static_cast_safe<uint32_t>(prim_index), static_cast_safe<int32_t>(pass_programs.size()) - 1);
				}
				++mesh_index;
			}
		}
	}

#undef create_hash_to_index

	std::vector<ror::JobHandle<bool>> job_handles;
	job_handles.reserve(shaders_count * 2);        // Multiplied by 2 because I am creating two jobs for each vertex and fragment shaders

	for (auto &passtype : render_pass_types)
	{
		// Generate vertex and fragment shaders for pre allocated shaders
		for (auto &model : this->m_models)
		{
			uint32_t mesh_index = 0;
			for (auto &mesh : model.meshes())
			{
				for (size_t prim_index = 0; prim_index < mesh.primitives_count(); ++prim_index)
				{
					// auto  vs_hash         = mesh.vertex_hash(prim_index);
					auto old_fs_hash = mesh.fragment_hash(prim_index);
					auto vs_hash     = pass_aware_vertex_hash(passtype, mesh, prim_index, model.skins());
					auto fs_hash     = pass_aware_fragment_hash(old_fs_hash, vs_hash);

					if (!shader_hash_to_flag[vs_hash].test_and_set())        // Not generated yet
					{
						auto &vs_shader_index = shader_hash_to_index[vs_hash];
						auto &vs_shader       = this->m_shaders[vs_shader_index];
						auto  vs_job_handle   = a_job_system.push_job([mesh_index, prim_index, passtype, &vs_shader, &model]() -> auto{
                            auto vs = ror::generate_primitive_vertex_shader(model, mesh_index, static_cast_safe<uint32_t>(prim_index), passtype);
                            vs_shader.source(vs);
                            return true;
						   });

						job_handles.emplace_back(std::move(vs_job_handle));
					}

					if (!shader_hash_to_flag[fs_hash].test_and_set())        // Not generated yet
					{
						auto &fs_shader_index = shader_hash_to_index[fs_hash];
						auto &fs_shader       = this->m_shaders[fs_shader_index];
						auto  fs_job_handle   = a_job_system.push_job([prim_index, passtype, has_shadows, &fs_shader, &mesh, &model]() -> auto{
                            auto fs = ror::generate_primitive_fragment_shader(mesh, model.materials(), static_cast_safe<uint32_t>(prim_index), passtype, has_shadows);
                            fs_shader.source(fs);
                            return true;
						   });

						job_handles.emplace_back(std::move(fs_job_handle));
					}
				}
				++mesh_index;
			}
		}
	}

	for (auto &jh : job_handles)
		if (!jh.data())
			ror::log_critical("Can't load shaders specified required for the scene.");

	if constexpr (get_build() == BuildType::build_debug)
	{
		auto &shaders = shader_hash_to_flag;
		for (auto &s : shaders)
		{
			(void) s;
			assert(s.second.test() == true && "Not all unique shaders are generated");
		}
		for (auto &passtype1 : render_pass_types)
		{
			auto &pass_programs1 = this->m_programs[passtype1];
			auto  size           = pass_programs1.size();
			(void) size;
			for (auto &passtype2 : render_pass_types)
			{
				auto &pass_programs2 = this->m_programs[passtype2];
				assert(pass_programs2.size() == size && "Something went wrong, sizes don't match");
				(void) pass_programs2;
			}
		}
	}

	log_warn("Actual number of shaders created {} ", this->m_shaders.size());
}

void Scene::upload(const ror::Renderer &a_renderer, rhi::Device &a_device, ror::EventSystem &a_event_system)
{
	auto render_passes = a_renderer.current_frame_graph();

	// Now lets upload them
	for (auto &shader : this->m_shaders)
	{
		shader.upload(a_device);
	}

	for (auto &shader : this->m_global_shaders)
	{
		shader.compile();
		shader.upload(a_device);
	}

	for (auto &program : this->m_global_programs)
	{
		program.upload(a_device, this->m_shaders);
	}

	// Upload all the shader programs creates pipelines in metal and vulkan cases
	for (auto &pass : render_passes)
	{
		for (auto &subpass : pass.subpasses())
		{
			auto    &pass_programs = this->m_programs[subpass.type()];
			uint32_t program       = 0;
			for (auto &model : this->m_models)
			{
				uint32_t mesh_index = 0;
				for (auto &mesh : model.meshes())
				{
					for (size_t prim_index = 0; prim_index < mesh.primitives_count(); ++prim_index)
					{
						pass_programs[program].upload(a_device, this->m_shaders, model, mesh_index, static_cast_safe<uint32_t>(prim_index), subpass);
						program++;
					}
					mesh_index++;
				}
			}
		}
	}

	// Upload lights
	for (auto &light : this->m_lights)
	{
		light.upload(a_device);
	}

	// Upload cameras
	for (auto &camera : this->m_cameras)
	{
		camera.init(a_event_system);
		camera.upload(a_device);
	}

	// Upload all nodes shader buffers
	for (auto &node : this->m_nodes_data)
		node.upload(a_device);

	for (auto &model : this->m_models)
		for (auto &node : model.nodes_side_data())
			node.upload(a_device);
}

void Scene::read_nodes()
{
	assert(this->m_json_file.contains("nodes") && "Provided scene file is not a roar scene.");

	// Read all the nodes
	auto    nodes = this->m_json_file["nodes"];
	int32_t model_index{0u};
	for (auto &node : nodes)
	{
		SceneNode     nod;
		SceneNodeData nod_data;

		nod_data.m_name = node["name"];

		if (node.contains("children"))
			nod_data.m_children = node["children"].get<std::vector<uint32_t>>();

		if (node.contains("translation"))
		{
			std::array<float32_t, 3> t = node["translation"];
			nod.m_trs_transform.translation({t[0], t[1], t[2]});
		}

		if (node.contains("rotation"))
		{
			std::array<float32_t, 4> r = node["rotation"];
			nod.m_trs_transform.rotation({r[0], r[1], r[2], r[3]});
		}

		if (node.contains("scale"))
		{
			std::array<float32_t, 3> s = node["scale"];
			nod.m_trs_transform.scale({s[0], s[1], s[2]});
		}

		if (node.contains("path"))
		{
			nod_data.m_model_path = node["path"];
			nod_data.m_model      = model_index;
			model_index++;
		}

		if (node.contains("program"))
			nod_data.m_program_id = node["program"];

		if (node.contains("animation"))
			nod_data.m_animation = node["animation"];

		if (node.contains("bvh"))
			nod_data.m_has_bvh = node["bvh"];

		if (node.contains("light"))
			nod_data.m_light_id = node["light"];

		if (node.contains("camera"))
			nod_data.m_light_id = node["camera"];

		if (node.contains("particle_emitter"))
			nod_data.m_particle_id = node["particle_emitter"];

		this->m_nodes.emplace_back(std::move(nod));
		this->m_nodes_data.emplace_back(std::move(nod_data));
	}

	// Since all nodes are loaded lets set the parent ids
	for (size_t i = 0; i < this->m_nodes_data.size(); ++i)
	{
		auto &node_data = this->m_nodes_data[i];
		for (auto &child : node_data.m_children)
		{
			auto &node    = this->m_nodes[child];
			node.m_parent = static_cast<int32_t>(i);
		}
	}
}

void Scene::read_cameras()
{
	if (this->m_json_file.contains("cameras"))
	{
		// Read all the cameras
		auto cameras = this->m_json_file["cameras"];
		for (auto &camera : cameras)
		{
			OrbitCamera cam;
			auto        cam_type = camera["type"];

			if (cam_type == "perspective")
			{
				cam.type(CameraType::perspective);
				if (camera.contains("perspective"))
				{
					auto cam_args = camera["perspective"];

					for (auto &[key, value] : cam_args.items())
					{
						if (key == "aspectRatio")
						{
							cam.aspect_ratio(value);
						}
						if (key == "yfov")
						{
							cam.y_fov(value);
						}
						if (key == "znear")
						{
							cam.z_near(value);
						}
						if (key == "zfar")
						{
							cam.z_far(value);
						}
					}
				}
				if (camera.contains("orthographic"))
				{
					auto cam_args = camera["orthographic"];

					for (auto &[key, value] : cam_args.items())
					{
						if (key == "xmag")
						{
							cam.width(value);
						}
						if (key == "ymag")
						{
							cam.height(value);
						}
						if (key == "znear")
						{
							cam.z_near(value);
						}
						if (key == "zfar")
						{
							cam.z_far(value);
						}
					}
				}
			}
			else
			{
				cam.type(CameraType::orthographic);
			}

			this->m_cameras.emplace_back(std::move(cam));
		}
	}
	else
	{
		log_info("No cameras founds in the scene, will assign default one");
		OrbitCamera cam;
		this->m_cameras.emplace_back(std::move(cam));
	}
}

void Scene::read_lights()
{
	if (this->m_json_file.contains("lights"))
	{
		// Read all the lights
		auto lights = this->m_json_file["lights"];
		for (auto &light : lights)
		{
			Light lit;
			auto  type = light["type"];

			if (type == "directional")
				lit.m_type = Light::LightType::directional;
			else if (type == "point")
				lit.m_type = Light::LightType::point;
			else if (type == "spot")
				lit.m_type = Light::LightType::spot;
			else if (type == "area")
				lit.m_type = Light::LightType::area;

			if (light.contains("color"))
			{
				std::array<float32_t, 3> c = light["color"];

				lit.m_color.x = c[0];
				lit.m_color.y = c[1];
				lit.m_color.z = c[2];
			}

			if (light.contains("position"))
			{
				std::array<float32_t, 3> p = light["position"];

				lit.m_position.x = p[0];
				lit.m_position.y = p[1];
				lit.m_position.z = p[2];
			}

			if (light.contains("direction"))
			{
				std::array<float32_t, 3> d = light["direction"];

				lit.m_direction.x = d[0];
				lit.m_direction.y = d[1];
				lit.m_direction.z = d[2];
			}

			if (light.contains("intensity"))
				lit.m_intensity = light["intensity"];

			if (light.contains("innerAngle"))
				lit.m_inner_angle = light["innerAngle"];

			if (light.contains("outerAngle"))
				lit.m_outer_angle = light["outerAngle"];

			if (light.contains("range"))
				lit.m_range = light["range"];

			this->m_lights.emplace_back(std::move(lit));
		}
	}
}

void Scene::read_programs()
{
	if (this->m_json_file.contains("shaders"))
	{
		// Read all the shaders
		auto shaders = this->m_json_file["shaders"];
		for (auto &shader : shaders)
		{
			auto s_path = std::filesystem::path(shader);
			auto type   = rhi::string_to_shader_type(s_path.extension());
			auto hash   = hash_64(s_path.c_str(), s_path.string().length());
			this->m_global_shaders.emplace_back(shader, hash, type, ror::ResourceAction::load);
		}
	}

	if (this->m_json_file.contains("programs"))
	{
		auto programs = this->m_json_file["programs"];
		for (auto &program : programs)
		{
			(void) program;

			if (program.contains("vertex"))
			{
				assert(program.contains("fragment") && "Program must contain both vertex and fragment ids");

				int32_t vid = program["vertex"];
				int32_t fid = program["fragment"];

				this->m_global_programs.emplace_back(vid, fid);
			}
			else if (program.contains("compute"))
			{
				assert(!program.contains("fragment") && "Program can't have compute and fragment ids");

				int32_t cid = program["compute"];

				this->m_global_programs.emplace_back(cid);
			}

			// TODO: Need to find a way to upload these programs (Challenge is for which model/mesh/primitive?)
		}
	}
}

void Scene::read_probes()
{
	if (this->m_json_file.contains("environment_probes"))
	{
		auto probes = this->m_json_file["environment_probes"];
		for (auto &probe : probes)
		{
			EnvironmentProbe prob;
			ror::Transformf  xform;
			if (probe.contains("translation"))
			{
				std::array<float32_t, 3> t = probe["translation"];
				xform.translation({t[0], t[1], t[2]});
			}
			if (probe.contains("rotation"))
			{
				std::array<float32_t, 4> r = probe["rotation"];
				xform.rotation({r[0], r[1], r[2], r[3]});
			}
			if (probe.contains("scale"))
			{
				std::array<float32_t, 3> s = probe["scale"];
				xform.scale({s[0], s[1], s[2]});
			}

			prob.transform(xform);

			if (probe.contains("path"))
				prob.path(probe["path"]);

			this->m_probes.emplace_back(std::move(prob));
		}
	}
}

void Scene::load_specific()
{
	this->read_nodes();
	this->read_cameras();
	this->read_lights();
	this->read_programs();
	this->read_probes();
}

void Scene::unload()
{}

}        // namespace ror
