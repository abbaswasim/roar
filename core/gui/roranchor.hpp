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

#include "foundation/rormacros.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorvector4.hpp"
#include <cstdint>
#include <imgui/imgui.h>
#include <vector>

namespace ror
{
class ROAR_ENGINE_ITEM Anchors final
{
  public:
	FORCE_INLINE          Anchors()                             = default;        //! Default constructor
	FORCE_INLINE          Anchors(const Anchors &a_other)       = default;        //! Copy constructor
	FORCE_INLINE          Anchors(Anchors &&a_other) noexcept   = default;        //! Move constructor
	FORCE_INLINE Anchors &operator=(const Anchors &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Anchors &operator=(Anchors &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Anchors() noexcept                            = default;        //! Destructor

	enum class AnchorType
	{
		circle,
		triangle,
		rectangle,
		pyramid,        // top cutted pyramid like a perspective pyramid
		bezier
	};

	class ROAR_ENGINE_ITEM Anchor final
	{
	  public:
		FORCE_INLINE         Anchor()                             = default;        //! Default constructor
		FORCE_INLINE         Anchor(const Anchor &a_other)        = default;        //! Copy constructor
		FORCE_INLINE         Anchor(Anchor &&a_other) noexcept    = default;        //! Move constructor
		FORCE_INLINE Anchor &operator=(const Anchor &a_other)     = default;        //! Copy assignment operator
		FORCE_INLINE Anchor &operator=(Anchor &&a_other) noexcept = default;        //! Move assignment operator
		FORCE_INLINE ~Anchor() noexcept                           = default;        //! Destructor

		Anchor(ror::Vector4f a_center, float32_t a_radius);
		Anchor(ror::Vector4f a_p1, ror::Vector4f a_p2, ror::Vector4f a_p3, AnchorType a_type = AnchorType::triangle);
		Anchor(ror::Vector4f a_p1, ror::Vector4f a_p2, ror::Vector4f a_p3, ror::Vector4f a_p4, AnchorType a_type = AnchorType::rectangle);

		void draw(ImDrawList *a_drawlist, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport, uint32_t a_color, uint32_t a_click_color, uint32_t a_white, bool a_hovering);
		void recenter();
		void move(const ror::Vector2f &a_delta, const ror::Matrix4f &a_view_projection, const ror::Matrix4f &a_view_projection_inverse, const ror::Vector4f &a_viewport);
		void reset(const ror::Vector4f &a_origin);
		bool inside(const ror::Vector2f &a_mouse_position, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport);

		// clang-format off
		constexpr auto  center(const ror::Vector4f &a_center)                     noexcept { this->m_center = a_center;         } 
		constexpr auto  new_center(const ror::Vector4f &a_center)                 noexcept { this->m_new_center = a_center;     } 
		constexpr auto  point(uint32_t a_index, const ror::Vector4f a_point)      noexcept { this->m_points[a_index] = a_point; } 
		constexpr auto  ribbon_value(float32_t a_value)                           noexcept { this->m_ribbon_value = a_value;    } 
		constexpr auto  has_ribbon(bool a_value)                                  noexcept { this->m_has_ribbon = a_value;      }
		constexpr auto  clicked(bool a_clicked)                                   noexcept { this->m_clicked = a_clicked;       }
		constexpr auto  color(uint32_t a_color)                                   noexcept { this->m_color = a_color;           }
		constexpr auto  radius(float32_t a_radius)                                noexcept { this->m_radius = a_radius;         }
		constexpr auto  type(AnchorType a_type)                                   noexcept { this->m_type = a_type;             }
		constexpr auto &center()                                                  noexcept { return this->m_center;             } 
		constexpr auto &new_center()                                              noexcept { return this->m_new_center;         } 
		constexpr auto &point(uint32_t a_index)                                   noexcept { return this->m_points[a_index];    } 
		constexpr auto &radius()                                                  noexcept { return this->m_radius;             }
		constexpr auto &type()                                                    noexcept { return this->m_type;               }
		constexpr auto  ribbon_value()                                      const noexcept { return this->m_ribbon_value;       } 
		constexpr auto  clicked()                                           const noexcept { return this->m_clicked;            }
		constexpr auto &color()                                                   noexcept { return this->m_color;              }
		// clang-format on

	  protected:
	  private:
		void move(ror::Vector4f &a_destination, const ror::Vector2f &a_delta, const ror::Matrix4f &a_view_projection, const ror::Matrix4f &a_view_projection_inverse, const ror::Vector4f &a_viewport);
		void make_ribbon(ImDrawList *a_drawlist, ror::Vector4f new_center, ror::Vector4f center, float line_thickness, uint32_t orange_alpha, uint32_t a_white, bool straight = false);

		AnchorType    m_type{AnchorType::circle};
		ror::Vector4f m_center{ror::zero_vector4f};            //! The center of the anchor shape
		ror::Vector4f m_new_center{ror::zero_vector4f};        //! The updated center of the anchor shape while moving
		ror::Vector4f m_points[4]{ror::zero_vector4f};         //! The supported shapes with up to 4 points
		float32_t     m_radius{5.0f};                          //! Radius of the circle Anchor
		float32_t     m_ribbon_value{0.0f};                    //! Ribbon value that can be used for whatever this Anchor is suppose to do
		uint32_t      m_color{0};                              //! Color of the Anchor in ImGui format
		bool          m_clicked{false};                        //! Internal use only checks if Anchor is clicked
		bool          m_has_ribbon{false};                     //! Do I draw a Ribbon when dragged or not
	};

	void    new_frame(bool a_mouse_down_state, bool a_mouse_up_state, ror::Vector2f a_mouse_position, ror::Vector2f a_mouse_clicked_position);
	void    draw(ImDrawList *draw_list, const ror::Matrix4f &a_view_projection, ror::Vector4f a_viewport, bool a_mouse_down_state);
	void    push_anchor(Anchor a_anchor);
	bool    moving(size_t a_index);
	void    reset(const ror::Vector4f &a_origin);
	size_t  anchors_count();
	Anchor &anchor(size_t a_index);

  protected:
  private:
	bool                m_draging{false};
	ror::Vector2f       m_clicked_position{};
	ror::Vector2f       m_mouse_position{};
	std::vector<Anchor> m_anchors{};
};

}        // namespace ror
