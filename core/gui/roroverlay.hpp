// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
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

#include "camera/rorcamera.hpp"
#include "graphics/rorlight.hpp"
#include "roranchor.hpp"
#include <cstdint>
#include <functional>
#include <variant>
#include <vector>

namespace ror
{
enum class OverlayType
{
	light,
	camera
};

using OverlaySource = std::variant<std::reference_wrapper<std::vector<ror::Light>>, std::reference_wrapper<std::vector<ror::OrbitCamera>>>;

class ROAR_ENGINE_ITEM Overlay final
{
  public:
	FORCE_INLINE          Overlay()                             = delete;         //! Default constructor
	FORCE_INLINE          Overlay(const Overlay &a_other)       = default;        //! Copy constructor
	FORCE_INLINE          Overlay(Overlay &&a_other) noexcept   = default;        //! Move constructor
	FORCE_INLINE Overlay &operator=(const Overlay &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Overlay &operator=(Overlay &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Overlay() noexcept                            = default;        //! Destructor

	Overlay(const ror::Light &a_light, OverlaySource a_source, uint32_t a_source_index);
	Overlay(const ror::OrbitCamera &, OverlaySource a_source, uint32_t a_source_index);

	// clang-format off
	constexpr auto &type()            const noexcept { return this->m_type;           }
	constexpr auto &source()          const noexcept { return this->m_source;         }
	constexpr auto &source_index()    const noexcept { return this->m_source_index;   }
	constexpr auto &anchors()         const noexcept { return this->m_anchors;        }

	constexpr auto type(OverlayType a_type)                noexcept { this->m_type = a_type;                  }
	constexpr auto source_index(uint32_t a_source_index)   noexcept { this->m_source_index = a_source_index;  }
	// constexpr auto source(OverlaySource a_source)          noexcept { this->m_source = a_source;              }
	// constexpr auto anchors(std::vector<Anchors> a_anchors) noexcept { this->m_anchors = a_anchors;             }
	// clang-format on

	void draw(ImDrawList *a_drawlist, ImFont *a_icon_font, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport);

  private:
	constexpr ror::Light &light()
	{
		assert(std::holds_alternative<std::reference_wrapper<std::vector<ror::Light>>>(this->m_source) && "Asking for Light in non-light source");
		return std::get<std::reference_wrapper<std::vector<ror::Light>>>(this->m_source).get()[this->m_source_index];
	}

	constexpr ror::OrbitCamera &camera()
	{
		assert(std::holds_alternative<std::reference_wrapper<std::vector<ror::OrbitCamera>>>(this->m_source) && "Asking for Camera in non-camera source");
		return std::get<std::reference_wrapper<std::vector<ror::OrbitCamera>>>(this->m_source).get()[this->m_source_index];
	}

	void add_light_anchors(const ror::Light &a_light);
	void add_camera_anchors(const ror::OrbitCamera &a_camera);

	void create_light(ImDrawList *a_drawlist, ImFont *a_icon_font, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport);
	void create_camera(ImDrawList *a_drawlist, ImFont *a_icon_font, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport);
	void update_other_anchors(const ror::Light &a_light);
	void update_light(bool a_left_released);
	void update_camera(bool a_left_released);
	void update(bool a_left_released);

	OverlayType   m_type{OverlayType::light};        //! Type of overlay, initially a point light type
	OverlaySource m_source;                        //! Source vector of this type of Overlays, like lights and cameras
	uint32_t      m_source_index{};                  //! Index inside of the source vector this type of Overlays
	Anchors       m_anchors{};                       //! Multi-purpose anchors used for different types of overlay
};

class ROAR_ENGINE_ITEM Overlays final
{
  public:
	FORCE_INLINE           Overlays()                             = default;        //! Default constructor
	FORCE_INLINE           Overlays(const Overlays &a_other)      = default;        //! Copy constructor
	FORCE_INLINE           Overlays(Overlays &&a_other) noexcept  = default;        //! Move constructor
	FORCE_INLINE Overlays &operator=(const Overlays &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Overlays &operator=(Overlays &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Overlays() noexcept                             = default;        //! Destructor

	void init(ImFont *a_icon_font);
	void draw(const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport);

	template <typename _type>
	void add(std::vector<_type> &a_overlay_target);

  protected:
  private:
	std::vector<Overlay> m_overlays{};                     //! All overlays in the scene
	ImDrawList          *m_draw_list{nullptr};             //! Drawlist into ImGui data
	ImFont              *m_roar_icon_font{nullptr};        //! Roar icon font with all the fancy icons
};

template <typename _type>
void Overlays::add(std::vector<_type> &a_targets)
{
	uint32_t index{0};
	for (auto &target : a_targets)
		this->m_overlays.emplace_back(target, a_targets, index++);
}

}        // namespace ror
