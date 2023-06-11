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

#include "foundation/rorhash.hpp"
#include "foundation/rortypes.hpp"
#include "graphics/rormaterial.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rortypes.hpp"

namespace ror
{

void Material::generate_hash()
{
	this->m_hash = this->m_base_color.hash();

	hash_combine_64(this->m_hash, this->m_diffuse_color.hash());
	hash_combine_64(this->m_hash, this->m_specular_glossyness.hash());
	hash_combine_64(this->m_hash, this->m_emissive.hash());
	hash_combine_64(this->m_hash, this->m_anisotropy.hash());
	hash_combine_64(this->m_hash, this->m_transmission.hash());
	hash_combine_64(this->m_hash, this->m_sheen_color.hash());
	hash_combine_64(this->m_hash, this->m_sheen_roughness.hash());
	hash_combine_64(this->m_hash, this->m_clearcoat_normal.hash());
	hash_combine_64(this->m_hash, this->m_clearcoat.hash());
	hash_combine_64(this->m_hash, this->m_clearcoat_roughness.hash());
	hash_combine_64(this->m_hash, this->m_metallic.hash());
	hash_combine_64(this->m_hash, this->m_roughness.hash());
	hash_combine_64(this->m_hash, this->m_occlusion.hash());
	hash_combine_64(this->m_hash, this->m_normal.hash());
	hash_combine_64(this->m_hash, this->m_bent_normal.hash());
	hash_combine_64(this->m_hash, this->m_height.hash());
	hash_combine_64(this->m_hash, this->m_subsurface_color.hash());

	// TODO: The following needs some condition, add that later for subsurface scattering support
	// hash_combine_64(this->m_hash, hash_64(&this->m_subsurface, sizeof(this->m_subsurface)));

	hash_combine_64(this->m_hash, hash_64(&this->m_material_model, sizeof(this->m_material_model)));
	hash_combine_64(this->m_hash, hash_64(&this->m_blend_mode, sizeof(this->m_blend_mode)));
	hash_combine_64(this->m_hash, hash_64(&this->m_type, sizeof(this->m_type)));
	hash_combine_64(this->m_hash, hash_64(&this->m_double_sided, sizeof(this->m_double_sided)));
	hash_combine_64(this->m_hash, hash_64(&this->m_layered, sizeof(this->m_layered)));

	if (this->m_blend_mode == rhi::BlendMode::mask)
		hash_combine_64(this->m_hash, hash_64(&this->m_opacity, sizeof(this->m_opacity)));

	// Not using material_name and f0 because there are not part of shader generated for this material
}

void Material::fill_shader_buffer()
{
	if (this->m_base_color.m_type == ror::Material::ComponentType::factor || this->m_base_color.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("base_color_factor", rhi::Format::float32_4, 1);
	if (this->m_diffuse_color.m_type == ror::Material::ComponentType::factor || this->m_diffuse_color.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("diffuse_color_factor", rhi::Format::float32_4, 1);
	if (this->m_specular_glossyness.m_type == ror::Material::ComponentType::factor || this->m_specular_glossyness.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("specular_glossyness_factor", rhi::Format::float32_4, 1);
	if (this->m_emissive.m_type == ror::Material::ComponentType::factor || this->m_emissive.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("emissive_factor", rhi::Format::float32_4, 1);
	if (this->m_anisotropy.m_type == ror::Material::ComponentType::factor || this->m_anisotropy.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("anisotropy_factor", rhi::Format::float32_4, 1);
	if (this->m_sheen_color.m_type == ror::Material::ComponentType::factor || this->m_sheen_color.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("sheen_color_factor", rhi::Format::float32_3, 1);
	if (this->m_clearcoat_normal.m_type == ror::Material::ComponentType::factor || this->m_clearcoat_normal.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("clearcoat_normal_factor", rhi::Format::float32_3, 1);
	if (this->m_transmission.m_type == ror::Material::ComponentType::factor || this->m_transmission.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("transmission_factor", rhi::Format::float32_2, 1);
	if (this->m_sheen_roughness.m_type == ror::Material::ComponentType::factor || this->m_sheen_roughness.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("sheen_roughness_factor", rhi::Format::float32_1, 1);
	if (this->m_clearcoat.m_type == ror::Material::ComponentType::factor || this->m_clearcoat.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("clearcoat_factor", rhi::Format::float32_1, 1);
	if (this->m_clearcoat_roughness.m_type == ror::Material::ComponentType::factor || this->m_clearcoat_roughness.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("clearcoat_roughness_factor", rhi::Format::float32_1, 1);
	if (this->m_metallic.m_type == ror::Material::ComponentType::factor || this->m_metallic.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("metallic_factor", rhi::Format::float32_1, 1);
	if (this->m_roughness.m_type == ror::Material::ComponentType::factor || this->m_roughness.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("roughness_factor", rhi::Format::float32_1, 1);
	if (this->m_occlusion.m_type == ror::Material::ComponentType::factor || this->m_occlusion.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("occlusion_factor", rhi::Format::float32_1, 1);
	if (this->m_normal.m_type == ror::Material::ComponentType::factor || this->m_normal.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("normal_factor", rhi::Format::float32_1, 1);
	if (this->m_bent_normal.m_type == ror::Material::ComponentType::factor || this->m_bent_normal.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("bent_normal_factor", rhi::Format::float32_1, 1);
	if (this->m_height.m_type == ror::Material::ComponentType::factor || this->m_height.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.add_entry("height_factor", rhi::Format::float32_1, 1);

	if (this->m_blend_mode == rhi::BlendMode::mask)
		this->m_shader_buffer.add_entry("opacity_factor", rhi::Format::float32_1, 1);

	// Unconditional factor of f0 needs to be there
	this->m_shader_buffer.add_entry("f0_factor", rhi::Format::float32_1, 1);

	// TODO: The following needs some condition, add that later for subsurface scattering support
	// if (this->m_subsurface_color.m_type != ror::Material::MaterialComponentType::texture_only)
	//	output.append("\tvec4  subsurface_color_factor;\n\t");
	// if (this->m_subsurface_radius.m_type != ror::Material::MaterialComponentType::texture_only)
	//	output.append("vec4  subsurface_radius_factor;\n\t");
	// if (this->m_subsurface_scattering.m_type != ror::Material::MaterialComponentType::texture_only)
	//	output.append("float subsurface_scattering_factor;\n\t");
}

void Material::update()
{
	this->m_shader_buffer.buffer_map();

	if (this->m_base_color.m_type == ror::Material::ComponentType::factor || this->m_base_color.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("base_color_factor", &this->m_base_color.m_factor);
	if (this->m_diffuse_color.m_type == ror::Material::ComponentType::factor || this->m_diffuse_color.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("diffuse_color_factor", &this->m_diffuse_color.m_factor);
	if (this->m_specular_glossyness.m_type == ror::Material::ComponentType::factor || this->m_specular_glossyness.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("specular_glossyness_factor", &this->m_specular_glossyness.m_factor);
	if (this->m_emissive.m_type == ror::Material::ComponentType::factor || this->m_emissive.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("emissive_factor", &this->m_emissive.m_factor);
	if (this->m_anisotropy.m_type == ror::Material::ComponentType::factor || this->m_anisotropy.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("anisotropy_factor", &this->m_anisotropy.m_factor);
	if (this->m_sheen_color.m_type == ror::Material::ComponentType::factor || this->m_sheen_color.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("sheen_color_factor", &this->m_sheen_color.m_factor);
	if (this->m_clearcoat_normal.m_type == ror::Material::ComponentType::factor || this->m_clearcoat_normal.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("clearcoat_normal_factor", &this->m_clearcoat_normal.m_factor);
	if (this->m_transmission.m_type == ror::Material::ComponentType::factor || this->m_transmission.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("transmission_factor", &this->m_transmission.m_factor);
	if (this->m_sheen_roughness.m_type == ror::Material::ComponentType::factor || this->m_sheen_roughness.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("sheen_roughness_factor", &this->m_sheen_roughness.m_factor);
	if (this->m_clearcoat.m_type == ror::Material::ComponentType::factor || this->m_clearcoat.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("clearcoat_factor", &this->m_clearcoat.m_factor);
	if (this->m_clearcoat_roughness.m_type == ror::Material::ComponentType::factor || this->m_clearcoat_roughness.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("clearcoat_roughness_factor", &this->m_clearcoat_roughness.m_factor);
	if (this->m_metallic.m_type == ror::Material::ComponentType::factor || this->m_metallic.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("metallic_factor", &this->m_metallic.m_factor);
	if (this->m_roughness.m_type == ror::Material::ComponentType::factor || this->m_roughness.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("roughness_factor", &this->m_roughness.m_factor);
	if (this->m_occlusion.m_type == ror::Material::ComponentType::factor || this->m_occlusion.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("occlusion_factor", &this->m_occlusion.m_factor);
	if (this->m_normal.m_type == ror::Material::ComponentType::factor || this->m_normal.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("normal_factor", &this->m_normal.m_factor);
	if (this->m_bent_normal.m_type == ror::Material::ComponentType::factor || this->m_bent_normal.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("bent_normal_factor", &this->m_bent_normal.m_factor);
	if (this->m_height.m_type == ror::Material::ComponentType::factor || this->m_height.m_type == ror::Material::ComponentType::factor_texture)
		this->m_shader_buffer.update("height_factor", &this->m_height.m_factor);

	if (this->m_blend_mode == rhi::BlendMode::mask)
		this->m_shader_buffer.update("opacity_factor", &this->m_opacity);

	// Unconditional factor of f0 needs to be there
	this->m_shader_buffer.update("f0_factor", &this->m_f0);

	this->m_shader_buffer.buffer_unmap();
}

void Material::upload(rhi::Device &a_device)
{
	// Will create UBO for factors like the following
	/*
	  layout(std140, set = 1, binding = 0) uniform factors
	  {
	      vec4  base_color_factor;
	      float metallic_factor;
	      float roughness_factor;
	      float opacity_factor;
	      float f0_factor;
	  } in_material_factors;
	*/

	this->fill_shader_buffer();
	this->m_shader_buffer.upload(a_device, rhi::ResourceStorageOption::managed);

	this->update();
}
}        // namespace ror
