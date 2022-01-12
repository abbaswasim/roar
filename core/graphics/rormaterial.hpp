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

#include "foundation/rortypes.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorhandles.hpp"
#include "rhi/rortypes.hpp"
#include <bitset>
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
	FORCE_INLINE           Material()                            = default;         //! Default constructor
	FORCE_INLINE           Material(const Material &a_other)     = default;         //! Copy constructor
	FORCE_INLINE           Material(Material &&a_other) noexcept = default;         //! Move constructor
	FORCE_INLINE Material &operator=(const Material &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Material &operator=(Material &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Material() noexcept                             = default;        //! Destructor

	enum class MaterialComponentType
	{
		factor_only,
		texture_only,
		both
	};

	template <typename _factor_type>
	struct MaterialComponent
	{
		_factor_type          m_factor{1.0f};                             //! TODO: Should be set differently for different textures
		rhi::TextureHandle    m_texture{-1};                              //! Texture ID of a Texture within a textures array (contains texture image id and texture sampler id)
		uint32_t              m_uv_map{0};                                //! Which UV map set should be used, 0 is default but some things like light maps usually use uv set 1
		MaterialComponentType m_type{MaterialComponentType::both};        //! Whether it has factor or texture only or both
		std::bitset<4>        m_channels{0};                              //! RGBA and all combinations of it, where R=bit1, B=bit2, G=bit3, A=bit4
	};

	enum class MaterialType
	{
		metalic_roughness,
		specular_glossyness,
		both
	};

	// TODO: Check and verify how this can be packed better
	MaterialComponent<ror::Color4f> m_base_color{};                                   //! Premultiplied linear RGB like filament, loader takes care of this
	MaterialComponent<ror::Color4f> m_diffuse_color{};                                //! Specular Glossyness specific diffuse color, either will have a factor or texture not both default of (1.0, 1.0, 1.0, 1.0)
	MaterialComponent<ror::Color4f> m_specular_glossyness{};                          //! Specular Glossyness specific vec3 Specular and float Glossyness factors default (1.0, 1.0, 1.0) and 1.0 or provided via a texture
	MaterialComponent<ror::Color4f> m_emissive{};                                     //! Used for lit objects, a texture and emissive RGB color with emissive strength in A component of the factor
	MaterialComponent<ror::Color3f> m_anisotrophy_normal{};                           //! To give different directions to anisotophy, factor.xyz is direction mutually exclusive with texture map
	MaterialComponent<ror::Color2f> m_transmission{};                                 //! 1.0 means stuff is fully transparent, Also called Opacity in UE4, factor is transmission factor and internal roughness, m_roughness is external surface roughness
	MaterialComponent<ror::Color3f> m_sheen_color{};                                  //! What should be the color of the sheen and its roughness. Roughness means either object color or light color
	MaterialComponent<float32_t>    m_sheen_roughness{};                              //! What should be the color of the sheen and its roughness. Roughness means either object color or light color
	MaterialComponent<ror::Color2f> m_clearcoat_normal{};                             //! For orange peel onto clearcoat, object normal usually has metallic flacks in it and its own normals, factor contains clear coat value and clear coat roughness
	MaterialComponent<float32_t>    m_clearcoat{};                                    //! Similar texture as clearcoat_normal provided by gltf // TODO: Confirm these are used
	MaterialComponent<float32_t>    m_clearcoat_roughness{};                          //! Similar texture as clearcoat_normal provided by gltf // TODO: Confirm these are used
	MaterialComponent<float32_t>    m_metallic{};                                     //! Dielectric or conductor(metallic)
	MaterialComponent<float32_t>    m_roughness{};                                    //! Surface roughness, as compared to internal roughness unlike m_transmission_roughness
	MaterialComponent<float32_t>    m_occlusion{};                                    //! Occlusion map, not very common and usually not provided
	MaterialComponent<float32_t>    m_normal{};                                       //! Normal map, Used for normal mapping and bump mapping
	MaterialComponent<float32_t>    m_bent_normal{};                                  //! Map of median rays of un-occluded normal of a hemisphere around a texel, can be used for AO etc
	MaterialComponent<float32_t>    m_height{};                                       //! Used for bump mapping
	MaterialComponent<float32_t>    m_anisotrophy{};                                  //! From -1 to 1 uniform value defines wether anisotrophy is along or perpendicular to direction, uniform single value or texture map
	MaterialComponent<float32_t>    m_opacity{};                                      //! Map to create cutouts, or a constant factor as alpha_cutoff_threshold of {0.5} default, also called Mask texture
	rhi::MaterialModel              m_material_model{rhi::MaterialModel::lit};        //! Is it lit, hair or eyes material etc
	rhi::BlendMode                  m_blend_mode{rhi::BlendMode::opaque};             //! Blend mode of the material
	MaterialType                    m_type{MaterialType::metalic_roughness};          //! Material could either be metallic rougness or specular glossy or both in which case one will be chosen
	bool                            m_double_sided{false};                            //! Should this be rendered with double sided triangle state
	bool                            m_layered{false};                                 //! Is this material part of a layerd material chain
	ror::Color4f                    m_subsurface_color{0.0f};                         //! Whats the color of scattering
	ror::Color4f                    m_subsurface_radius{0.0f};                        //! How far does it go, in each component, red is usually higher compared to blue and then green (1.0, 0.2, 0.1) for human skin
	uint8_t                         m_material_name[30];                              //! Can't use string here otherwise not trivially_copyable
	float32_t                       m_subsurface_scattering{0.0f};                    //! How much SSS do we want
	float32_t                       m_reflectance_factor{0.0f};                       //! Fresnel reflectance at normal incidence, used for reflections and calculating F0, note we don't need ior here because F0= ((ior − 1 ) / ( ior + 1 ))²
																					  //! Note we don't need F90 because Schlick equation only use F0 which can be derived like vec3 f0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + base_color * metallic
																					  //! TODO: Find out who provides m_reflectance_factor, gltf doesn't seem to have it in their material
};

static_assert(std::is_trivially_copyable_v<Material>, "Material is not trivially copyable");
static_assert(std::is_standard_layout_v<Material>, "Material is not standard layout");

// #define STBI_NO_FAILURE_STRINGS
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

}        // namespace ror

namespace rhi
{

define_type_to_shader_semantics(ror::Material)
{
	return rhi::BufferSemantic::material_data;
}

}        // namespace rhi
