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

#include "roranchor.hpp"

namespace ror
{

class ROAR_ENGINE_ITEM Gizmo final
{
  public:
	FORCE_INLINE        Gizmo()                             = default;        //! Default constructor
	FORCE_INLINE        Gizmo(const Gizmo &a_other)         = default;        //! Copy constructor
	FORCE_INLINE        Gizmo(Gizmo &&a_other) noexcept     = default;        //! Move constructor
	FORCE_INLINE Gizmo &operator=(const Gizmo &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Gizmo &operator=(Gizmo &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Gizmo() noexcept                          = default;        //! Destructor

	void init(ImFont *a_icon_font, const ror::Vector4f &a_origin);
	void draw(const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport);

  protected:
  private:
	static constexpr uint32_t total_anchors{16};

	void reset(const ror::Vector4f &a_origin);
	void rescale(float32_t a_world_scale);
	void shape();
	void update(float32_t a_world_scale);

	uint32_t  m_move[6];                                 //! Move anchors index within Gui for x, y, and z axis and yz, xz, xy
	uint32_t  m_scale[6];                                //! Scale anchors index within Gui for x, y, and z axis and yz, xz, xy
	uint32_t  m_rotate[3];                               //! Rotation anchors index within Gui for x, y, and z axis
	uint32_t  m_center{0};                               //! Center anchor for moving and scaling or rotation in screenspcae (left-drag move, right-drag rotate, both-drag scale)
	float32_t m_size{1.0f};                              //! Size of the gizmo in world units
	float32_t m_scale_scalar{1.0f};                      //! Scale for the scale anchor
	float32_t m_rotate_scalar{1.0f};                     //! Scale for the rotate anchor
	float32_t m_move_scale_scalar{1.0f};                 //! Scale for the move_scale anchor
	float32_t m_bezier_ease{1.0f};                       //! Scale for the bezier easing
	float32_t m_scale_scale[3]{1.0f, 1.0f, 1.0f};        //! Scale on each axis after manipulation

	Anchors     m_anchors_behind{};               //! All the anchors but split into 2 groups because of rendering order
	Anchors     m_anchors_front{};                //! Some lines needs to be rendererd in the middle of these
	ImDrawList *m_draw_list{nullptr};             //! Drawlist into ImGui data
	ImFont     *m_roar_icon_font{nullptr};        //! Roar icon font with all the fancy icons
};

}        // namespace ror
