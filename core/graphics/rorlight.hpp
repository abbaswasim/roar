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

#pragma once

#include "foundation/rormacros.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "math/rortransform.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "roar_export_import.hpp"

namespace ror
{
class ROAR_ENGINE_ITEM Light
{
  public:
	FORCE_INLINE        Light()                             = default;        //! Default constructors
	FORCE_INLINE        Light(const Light &a_other)         = delete;         //! Copy constructor
	FORCE_INLINE        Light(Light &&a_other) noexcept     = default;        //! Move constructor
	FORCE_INLINE Light &operator=(const Light &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE Light &operator=(Light &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~Light() noexcept                          = default;        //! Destructor

	enum class LightType
	{
		directional,
		spot,
		point,
		area
	};

	void update();
	void upload(rhi::Device &a_device);
	void fill_shader_buffer();
	void setup_transformations();
	void get_transformations(ror::Matrix4f **a_view_projection, ror::Matrix4f **a_projection, ror::Matrix4f **a_view, ror::Vector3f **a_position, ror::Vector4ui **a_viewport);

	// clang-format off
	FORCE_INLINE constexpr auto& shader_buffer() const noexcept  { return this->m_shader_buffer;      }
	FORCE_INLINE constexpr auto& shader_buffer()       noexcept  { return this->m_shader_buffer;      }
	FORCE_INLINE constexpr auto& view_projection()     noexcept  { return this->m_view_projection;    }
	// clang-format on

	bool              m_dirty{true};                                         //! If dirty will update/upload into the GPU otherwise not
	LightType         m_type{LightType::directional};                        //! Light type
	Matrix4f          m_view{};                                              //! View of the light, used in shadow mapping
	Matrix4f          m_projection{};                                        //! Projection of the light, used in shadow mapping
	Matrix4f          m_view_projection{};                                   //! View projection of the light, used in shadow mapping
	Vector4ui         m_shadow_viewport{};                                   //! Size and position of rendering into the shadowmap
	Vector3f          m_color{};                                             //! Light color
	Vector3f          m_position{};                                          //! Position of point and spot lights
	Vector3f          m_direction{};                                         //! Direction of directional and spot lights
	float32_t         m_intensity{1.0f};                                     //! Light intensity
	float32_t         m_range{std::numeric_limits<float32_t>::max()};        //! Light range after which light attenuates
	float32_t         m_inner_angle{0.0f};                                   //! Spot light inner angle, in radians, inner and outer might flip because outer > inner always
	float32_t         m_outer_angle{ror::ror_pi / 4.0f};                     //! Spot light outter angle, in radians, inner and outer might flip because outer > inner always
	std::string       m_light_struct_name{};                                 //! Light struct name cache
	rhi::ShaderBuffer m_shader_buffer{"Light",
	                                  rhi::ShaderBufferType::ubo,
	                                  rhi::Layout::std140,
	                                  settings().directional_light_set(),
	                                  settings().directional_light_binding()};        //! Shader buffer for a specific type of light UBO
};

class ROAR_ENGINE_ITEM EnvironmentProbe final
{
  public:
	FORCE_INLINE                   EnvironmentProbe()                                    = default;        //! Default constructor
	FORCE_INLINE                   EnvironmentProbe(const EnvironmentProbe &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                   EnvironmentProbe(EnvironmentProbe &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE EnvironmentProbe &operator=(const EnvironmentProbe &a_other)            = default;        //! Copy assignment operator
	FORCE_INLINE EnvironmentProbe &operator=(EnvironmentProbe &&a_other) noexcept        = default;        //! Move assignment operator
	FORCE_INLINE ~EnvironmentProbe() noexcept                                            = default;        //! Destructor

	// clang-format off
	FORCE_INLINE void transform(ror::Transformf a_transform) { this->m_transform = a_transform; }
	FORCE_INLINE void path(std::filesystem::path a_path)     { this->m_path = a_path;           }

	FORCE_INLINE auto transform()                            { return this->m_transform;        }
	FORCE_INLINE auto path()                                 { return this->m_path;             }
	// clang-format on

  protected:
  private:
	ror::Transformf       m_transform{};
	std::filesystem::path m_path{};
};

}        // namespace ror
