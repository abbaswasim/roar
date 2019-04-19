// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2019
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

namespace ror
{
FORCE_INLINE cguielement::cguielement(void)
{
	this->m_parent             = nullptr;
	this->m_position           = ror::Vector3i(0, 0, 1);
	this->m_absolute_position  = ror::Vector3i(0, 0, 1);
	this->m_non_clien_area     = ror::Vector4i(2, 2, 4, 2);
	this->m_background         = nullptr;
	this->m_foreground         = nullptr;
	this->m_bounds             = ror::Vector2i(1, 1);
	this->m_call_back          = nullptr;
	this->m_dirty              = true;
	this->m_pressed            = false;
	this->m_font_index         = -1;
	this->m_sequence_in_parent = 2;
	//this->m_CurrentSequenceOffset	= 0;
	this->m_alignment = rLEFT_CENTER;
}

FORCE_INLINE cguielement::cguielement(cguielement *a_parent, Vector3i &a_position, Vector4f *a_background, Vector3f *a_foreground, Vector2i &a_bounds, IFunctor *a_call_back, EElementType a_element_type, int32_t a_font_index, EAlignment a_alighment)
{
	this->m_dirty              = true;
	this->m_parent             = a_parent;
	this->m_position           = a_position;
	this->m_absolute_position  = ror::Vector3i(0, 0, 1);
	this->m_non_clien_area     = ror::Vector4i(2, 2, 4, 2);
	this->m_background         = a_background;
	this->m_foreground         = a_foreground;
	this->m_bounds             = a_bounds;
	this->m_call_back          = a_call_back;
	this->m_element_type       = a_element_type;
	this->m_font_index         = a_font_index;
	this->m_alignment          = a_alighment;
	this->m_sequence_in_parent = 2;
	this->m_pressed            = false;
	//this->m_CurrentSequenceOffset	= 0;
}

FORCE_INLINE void cguielement::set_id(int32_t a_id)
{
	this->m_id = a_id;
}

FORCE_INLINE int32_t cguielement::get_id() const
{
	return this->m_id;
}

FORCE_INLINE cguielement *cguielement::get_parent() const
{
	return this->m_parent;
}

FORCE_INLINE void cguielement::set_parent(cguielement *a_parent)
{
	this->m_parent = a_parent;
}

FORCE_INLINE Vector3i cguielement::get_position() const
{
	if (nullptr != this->m_parent)
	{
		return this->m_parent->m_position;
	}
	return this->m_position;
}

FORCE_INLINE Vector3i cguielement::get_own_position() const
{
	return this->m_position;
}

FORCE_INLINE void cguielement::set_position(Vector3i &a_position)
{
	this->m_position = a_position;
}

FORCE_INLINE void cguielement::set_absolute_position(ror::Vector3i &a_absolute_position)
{
	this->m_absolute_position = a_absolute_position;
}

FORCE_INLINE Vector3i cguielement::get_absolute_position() const
{
	return this->m_absolute_position;
}

FORCE_INLINE void cguielement::set_non_clien_area(Vector4i &a_ncareas)
{
	this->m_non_clien_area = a_ncareas;
}

FORCE_INLINE Vector4i cguielement::get_non_clien_area() const
{
	return this->m_non_clien_area;
}

FORCE_INLINE int32_t cguielement::get_zorder() const
{
	return this->m_position.z;
}

FORCE_INLINE void cguielement::set_zorder(int32_t a_zorder)
{
	this->m_position.z = a_zorder;
}

FORCE_INLINE Vector2i cguielement::get_bounds() const
{
	return this->m_bounds;
}

FORCE_INLINE void cguielement::set_bounds(Vector2i &a_bounds)
{
	this->m_bounds = a_bounds;
}

FORCE_INLINE Vector2i cguielement::get_text_position() const
{
	return this->m_text_position;
}

FORCE_INLINE void cguielement::set_text_position(Vector2i &a_position)
{
	this->m_text_position = a_position;
}

FORCE_INLINE std::string cguielement::get_text() const
{
	return this->m_text;
}

FORCE_INLINE void cguielement::set_text(std::string &a_text)
{
	this->m_text = a_text;
}

FORCE_INLINE IFunctor *cguielement::get_call_back() const
{
	return this->m_call_back;
}

FORCE_INLINE void cguielement::set_call_back(ror::IFunctor *a_callback)
{
	this->m_call_back = a_callback;
}

FORCE_INLINE bool cguielement::get_dirty() const
{
	return this->m_dirty;
}

FORCE_INLINE void cguielement::set_dirty(bool a_dirty)
{
	this->m_dirty = a_dirty;
}

FORCE_INLINE void cguielement::set_draw_caption(bool a_draw_caption)
{
	this->m_draw_caption = a_draw_caption;
}

FORCE_INLINE bool cguielement::get_draw_caption() const
{
	return this->m_draw_caption;
}

FORCE_INLINE cguielement::EElementType cguielement::get_element_type() const
{
	return m_element_type;
}

FORCE_INLINE void cguielement::set_element_type(cguielement::EElementType a_element_type)
{
	m_element_type = a_element_type;
}

FORCE_INLINE void cguielement::set_foreground(Vector3f *a_foreground)
{
	this->m_foreground = a_foreground;
}

FORCE_INLINE Vector3f *cguielement::get_foreground() const
{
	if (nullptr != this->m_foreground)
	{
		return this->m_foreground;
	}
	else if (nullptr != this->m_parent)
	{
		return this->m_parent->get_foreground();
	}
	else
	{
		return nullptr;
	}
}

FORCE_INLINE void cguielement::set_background(Vector4f *a_background)
{
	this->m_background = a_background;
}

FORCE_INLINE Vector4f *cguielement::get_background() const
{
	if (nullptr != this->m_background)
	{
		return this->m_background;
	}
	else if (nullptr != this->m_parent)
	{
		return this->m_parent->get_background();
	}
	else
	{
		return nullptr;
	}
}

FORCE_INLINE int32_t cguielement::get_sequence_in_parent() const
{
	return this->m_sequence_in_parent;
}

FORCE_INLINE void cguielement::set_sequence_in_parent(int32_t a_sequence_in_parent)
{
	this->m_sequence_in_parent = a_sequence_in_parent;
}

FORCE_INLINE void cguielement::increment_sequence_in_parent(cguielement *a_child)
{
	this->m_sequence_in_parent += a_child->m_bounds.y;
}

//	FORCE_INLINE void CGUIElement::SetCurrentSequenceOffset( int32_t a_CurrentSequenceOffset )
//	{
//		this->m_CurrentSequenceOffset = a_CurrentSequenceOffset;
//	}
//
//	FORCE_INLINE int32_t CGUIElement::GetCurrentSequenceOffset() const
//	{
//		return this->m_CurrentSequenceOffset;
//	}

FORCE_INLINE int32_t cguielement::get_font_index() const
{
	if (-1 != this->m_font_index)
	{
		return this->m_font_index;
	}
	else if (nullptr != this->m_parent)
	{
		return this->m_parent->get_font_index();
	}
	else
	{
		return FONT->get_font(FONT->get_default_font());
	}
}

FORCE_INLINE void cguielement::set_font_index(int32_t a_font_index)
{
	this->m_font_index = a_font_index;
}

FORCE_INLINE void cguielement::set_visible(bool a_visible)
{
	this->m_visible = a_visible;
}

FORCE_INLINE void cguielement::hide_me()
{
	this->m_visible = false;
}

FORCE_INLINE bool cguielement::get_visible() const
{
	return this->m_visible;
}

FORCE_INLINE bool cguielement::is_visible() const
{
	if (this->m_visible)
	{
		if (nullptr != this->m_parent)
		{
			return this->m_parent->m_visible;
		}
	}
	return this->m_visible;
}

FORCE_INLINE bool cguielement::operator()(cguielement *a_one, cguielement *a_two)
{
	return (a_one->m_position.z < a_two->m_position.z);
}

FORCE_INLINE void cguielement::set_pressed(bool a_pressed)
{
	this->m_pressed = a_pressed;
}

FORCE_INLINE bool cguielement::get_pressed() const
{
	return this->m_pressed;
}

}        // namespace ror
