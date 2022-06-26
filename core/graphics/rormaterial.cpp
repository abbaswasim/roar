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

#include "graphics/rormaterial.hpp"
#include "rhi/rorshader_buffer.hpp"

namespace ror
{
void material_to_shader_buffer(const ror::Material &a_material, rhi::ShaderBuffer &shader_buffer)
{
	if (a_material.m_base_color.m_type == ror::Material::ComponentType::factor || a_material.m_base_color.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("base_color_factor", rhi::Format::float32_4, 1);
	if (a_material.m_diffuse_color.m_type == ror::Material::ComponentType::factor || a_material.m_diffuse_color.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("diffuse_color_factor", rhi::Format::float32_4, 1);
	if (a_material.m_specular_glossyness.m_type == ror::Material::ComponentType::factor || a_material.m_specular_glossyness.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("specular_glossyness_factor", rhi::Format::float32_4, 1);
	if (a_material.m_emissive.m_type == ror::Material::ComponentType::factor || a_material.m_emissive.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("emissive_factor", rhi::Format::float32_4, 1);
	if (a_material.m_anisotropy.m_type == ror::Material::ComponentType::factor || a_material.m_anisotropy.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("anisotrophy_factor", rhi::Format::float32_4, 1);
	if (a_material.m_transmission.m_type == ror::Material::ComponentType::factor || a_material.m_transmission.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("transmission_factor", rhi::Format::float32_2, 1);
	if (a_material.m_sheen_color.m_type == ror::Material::ComponentType::factor || a_material.m_sheen_color.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("sheen_color_factor", rhi::Format::float32_3, 1);
	if (a_material.m_sheen_roughness.m_type == ror::Material::ComponentType::factor || a_material.m_sheen_roughness.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("sheen_roughness_factor", rhi::Format::float32_1, 1);
	if (a_material.m_clearcoat_normal.m_type == ror::Material::ComponentType::factor || a_material.m_clearcoat_normal.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("clearcoat_normal_factor", rhi::Format::float32_2, 1);
	if (a_material.m_clearcoat.m_type == ror::Material::ComponentType::factor || a_material.m_clearcoat.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("clearcoat_factor", rhi::Format::float32_1, 1);
	if (a_material.m_clearcoat_roughness.m_type == ror::Material::ComponentType::factor || a_material.m_clearcoat_roughness.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("clearcoat_roughness_factor", rhi::Format::float32_1, 1);
	if (a_material.m_metallic.m_type == ror::Material::ComponentType::factor || a_material.m_metallic.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("metallic_factor", rhi::Format::float32_1, 1);
	if (a_material.m_roughness.m_type == ror::Material::ComponentType::factor || a_material.m_roughness.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("roughness_factor", rhi::Format::float32_1, 1);
	if (a_material.m_occlusion.m_type == ror::Material::ComponentType::factor || a_material.m_occlusion.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("occlusion_factor", rhi::Format::float32_1, 1);
	if (a_material.m_normal.m_type == ror::Material::ComponentType::factor || a_material.m_normal.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("normal_factor", rhi::Format::float32_1, 1);
	if (a_material.m_bent_normal.m_type == ror::Material::ComponentType::factor || a_material.m_bent_normal.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("bent_normal_factor", rhi::Format::float32_1, 1);
	if (a_material.m_height.m_type == ror::Material::ComponentType::factor || a_material.m_height.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("height_factor", rhi::Format::float32_1, 1);
	if (a_material.m_anisotropy.m_type == ror::Material::ComponentType::factor || a_material.m_anisotropy.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("anisotrophy_factor", rhi::Format::float32_1, 1);
	if (a_material.m_opacity.m_type == ror::Material::ComponentType::factor || a_material.m_opacity.m_type == ror::Material::ComponentType::factor_texture)
		shader_buffer.add_entry("opacity_factor", rhi::Format::float32_1, 1);

	// Unconditional factor of reflectance needs to be there
	shader_buffer.add_entry("reflectance_factor", rhi::Format::float32_1, 1);

	// TODO: The following needs some condition, add that later for subsurface scattering support
	// if (a_material.m_subsurface_color.m_type != ror::Material::MaterialComponentType::texture_only)
	//	output.append("\tvec4  subsurface_color_factor;\n\t");
	// if (a_material.m_subsurface_radius.m_type != ror::Material::MaterialComponentType::texture_only)
	//	output.append("vec4  subsurface_radius_factor;\n\t");
	// if (a_material.m_subsurface_scattering.m_type != ror::Material::MaterialComponentType::texture_only)
	//	output.append("float subsurface_scattering_factor;\n\t");
}
}        // namespace ror
