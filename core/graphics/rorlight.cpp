// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021-2022
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

#include "graphics/rorlight.hpp"

namespace ror
{

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
	// std::cout << "Her is the glsl string for light type = " << this->m_light_struct_name << "\n"
	//           << this->m_shader_buffer.to_glsl_string();

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

}        // namespace ror
