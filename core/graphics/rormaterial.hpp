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

#include "foundation/rorhash.hpp"
#include "foundation/rortypes.hpp"
#include "math/rormatrix3.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorhandles.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rortypes.hpp"
#include <bitset>
#include <cstddef>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>

namespace ror
{
// To do material layering, one has to interpolate these parameters first before calculating the BRDF
class ROAR_ENGINE_ITEM Material final
{
  public:
	FORCE_INLINE           Material()                             = default;        //! Default constructor
	FORCE_INLINE           Material(const Material &a_other)      = default;        //! Copy constructor
	FORCE_INLINE           Material(Material &&a_other) noexcept  = default;        //! Move constructor
	FORCE_INLINE Material &operator=(const Material &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Material &operator=(Material &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Material() noexcept                             = default;        //! Destructor

	enum class MaterialType
	{
		metalic_roughness,
		specular_glossyness,
		both
	};

	enum class ComponentType
	{
		none,
		factor,
		texture,
		factor_texture
	};

	template <typename _factor_type>
	struct Component
	{
		_factor_type       m_factor{1.0f};                     //! TODO: Should be set differently for different textures
		rhi::TextureHandle m_texture{-1};                      //! Texture ID of a Texture within a textures array (contains texture image id and texture sampler id)
		uint32_t           m_uv_map{0};                        //! Which UV map set should be used, 0 is default but some things like light maps usually use uv set 1
		ComponentType      m_type{ComponentType::none};        //! Whether it has factor or texture only or both or none
		bool               m_has_transform{false};             //! Whether the UVs has a texture transform or not
		ror::Matrix3f      m_transform{};                      //! The UV transform available for this map, at the moment its constant for this material and in its shader
		// std::bitset<4>     m_channels{0};                      //! RGBA and all combinations of it, where R=bit1, B=bit2, G=bit3, A=bit4, TODO: Fix and use this, make sure to use bit-field not bitset, uint32_t m_channels{0} : 4;

		hash_64_t hash()
		{
			// Note m_factor and m_texture are not used in hash calculation because factor is constant as far as Material is concerned and texture will always be different
			hash_64_t hash_value = hash_64(&this->m_uv_map, sizeof(this->m_uv_map));
			hash_combine_64(hash_value, hash_64(&this->m_type, sizeof(this->m_type)));
			hash_combine_64(hash_value, hash_64(&this->m_has_transform, sizeof(this->m_has_transform)));
			hash_combine_64(hash_value, hash_64(&this->m_transform, sizeof(this->m_transform)));

			return hash_value;
		}
	};

	// TODO: Check and verify how this can be packed better
	Component<ror::Color4f> m_base_color{};                                   //! Premultiplied linear RGB like filament, loader takes care of this // TODO: Fix the pre-multiplied bit
	Component<ror::Color4f> m_diffuse_color{};                                //! Specular Glossyness specific diffuse color, either will have a factor or texture not both default of (1.0, 1.0, 1.0, 1.0)
	Component<ror::Color4f> m_specular_glossyness{};                          //! Specular Glossyness specific vec3 Specular and float Glossyness factors default (1.0, 1.0, 1.0) and 1.0 or provided via a texture
	Component<ror::Color4f> m_emissive{};                                     //! Used for lit objects, a texture and emissive RGB color with emissive strength in A component of the factor
	Component<ror::Color4f> m_anisotropy{};                                   //! xyz is directions of anisotophy, mutually exclusive with texture, w is factor [-1 1] defines if anisotrophy is along or perpendicular to direction
	Component<ror::Color2f> m_transmission{};                                 //! 1.0 means stuff is fully transparent, Also called Opacity in UE4, factor is transmission factor and internal roughness, m_roughness is external surface roughness
	Component<ror::Color3f> m_sheen_color{};                                  //! What should be the color of the sheen and its roughness. Roughness means either object color or light color
	Component<float32_t>    m_sheen_roughness{};                              //! What should be the color of the sheen and its roughness. Roughness means either object color or light color
	Component<ror::Color2f> m_clearcoat_normal{};                             //! For orange peel onto clearcoat, object normal usually has metallic flacks in it and its own normals, factor contains clear coat value and clear coat roughness
	Component<float32_t>    m_clearcoat{};                                    //! Similar texture as clearcoat_normal provided by gltf
	Component<float32_t>    m_clearcoat_roughness{};                          //! Similar texture as clearcoat_normal provided by gltf
	Component<float32_t>    m_metallic{};                                     //! Dielectric or conductor(metallic)
	Component<float32_t>    m_roughness{};                                    //! Surface roughness, as compared to internal roughness unlike m_transmission_roughness
	Component<float32_t>    m_occlusion{};                                    //! Occlusion map factor is the strength
	Component<float32_t>    m_normal{};                                       //! Normal map, Used for normal mapping and bump mapping, factor is the normal scale
	Component<float32_t>    m_bent_normal{};                                  //! Map of median rays of un-occluded normal of a hemisphere around a texel, can be used for AO etc
	Component<float32_t>    m_height{};                                       //! Height map that ca be used for bump mapping, factor is the scale
	Component<float32_t>    m_opacity{};                                      //! Map to create cutouts, or a constant factor as alpha_cutoff_threshold of {0.5} default, also called Mask texture
	Component<ror::Color4f> m_subsurface_color{};                             //! Whats the color of scattering, could also be a texture map
	ror::Color4f            m_subsurface{0.0f};                               //! Reach in each component, red is higher compared to blue and green (1.0, 0.2, 0.1) for human skin, w is scattering like how much SSS do we want
	rhi::MaterialModel      m_material_model{rhi::MaterialModel::lit};        //! Is it lit, hair or eyes material etc
	rhi::BlendMode          m_blend_mode{rhi::BlendMode::opaque};             //! Blend mode of the material
	MaterialType            m_type{MaterialType::metalic_roughness};          //! Material could either be metallic roughness or specular glossy or both in which case one will be chosen
	bool                    m_double_sided{false};                            //! Should this be rendered with double sided triangle state
	bool                    m_layered{false};                                 //! Is this material part of a layerd material chain
	uint8_t                 m_material_name[30];                              //! Can't use string here otherwise not trivially_copyable
	float32_t               m_reflectance{0.0f};                              //! Fresnel reflectance at normal incidence, used for reflections and calculating F0, we use only F0 from F0=((ior − 1) / (ior + 1))²
	                                                                          //! Note we don't need F90 coz Schlick equation only use F0 which can be derived like vec3 f0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + base_color * metallic
	rhi::ShaderBuffer *m_shader_buffer{nullptr};                              //! Non-owning pointer to ShaderBuffer which describes its shader's UBO/SSBO view
	hash_64_t          m_hash;                                                //! Material hash to make sure we don't create duplicate shaders

	void generate_hash();
};

void material_to_shader_buffer(const ror::Material &a_material, rhi::ShaderBuffer &a_shader_buffer);

static_assert(std::is_trivially_copyable_v<Material>, "Material is not trivially copyable");
static_assert(std::is_standard_layout_v<Material>, "Material is not standard layout");

}        // namespace ror

namespace rhi
{

define_type_to_shader_semantics(ror::Material)
{
	return rhi::BufferSemantic::material_data;
}

}        // namespace rhi

// Credit glTF 2.0 Specification https://www.khronos.org/registry/glTF/specs/2.0/glTF-2.0.html
// gltf function for calcuating f0 etc
/*
function fresnel_mix(ior, base, layer) {
  f0 = ((1-ior)/(1+ior))^2
  fr = f0 + (1 - f0)*(1 - abs(VdotH))^5
  return mix(base, layer, fr)
}
*/
