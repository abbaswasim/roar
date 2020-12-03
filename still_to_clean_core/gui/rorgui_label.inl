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
FORCE_INLINE cguilabel::cguilabel(void)
{}

FORCE_INLINE void cguilabel::set(cguielement *a_parent, std::string a_text, bool a_visible, bool a_ground, IFunctor *a_call_back, int32_t a_font_index, cguielement::EAlignment a_alignment, Vector4i &a_non_client_area)
{
	// I have to Calculate Position and Bound here
	Vector3i position;
	Vector4f texture_offset;
	Vector2i bounds;
	//CVector4i NonClientArea(1,1,1,1);

	int32_t height;
	int32_t width;
	if (0 > a_font_index)
	{
		a_font_index         = this->get_font_index();
		ror::Font *temp_font = FONT->get_font(a_font_index);
		assert(nullptr != temp_font && "Font Is Not Loaded :: RORGUILabel.inl");
		height = temp_font->get_height();
		width  = temp_font->get_width();
	}
	else
	{
		ror::Font *temp_font = FONT->get_font(a_font_index);
		assert(nullptr != temp_font && "Font Is Not Loaded :: RORGUILabel.inl");
		height = temp_font->get_height();
		width  = temp_font->get_width();
	}

	// Lets calculate the Bound
	bounds.y = height + a_non_client_area.z + a_non_client_area.w;                                     // Height plus Top and Bottom Margins
	bounds.x = (width * (int32_t) a_text.length()) + a_non_client_area.x + a_non_client_area.y;        // Width plus Left and Right Margins

	// Now we calculate the Position
	assert(nullptr != a_parent && "Label without Parent :: RORGUILabel.inl");
	if (nullptr != a_parent)
	{
		// Take Parent Position and Add the Offset to it
		position.y = 0;        //abs(a_Parent->GetPosition().y);
		position.y += a_parent->get_sequence_in_parent();
		this->m_sequence_in_parent = a_parent->get_sequence_in_parent();
		if (0 > a_parent->get_position().y)
		{
			position.y *= -1;
		}
	}        // Else leave it zero

	switch (a_alignment)
	{
		case cguielement::rLEFT_CENTER:
		case cguielement::rTOP_LEFT:
		case cguielement::rBOTTOM_LEFT:
		case cguielement::rLEFT:
			position.x = 0;        //abs(a_Parent->GetPosition().x);
			if (0 > a_parent->get_position().x)
			{
				position.x *= -1;
				position.x += a_parent->get_non_clien_area().x;
			}
			else
				position.x -= a_parent->get_non_clien_area().x;
			break;
		case cguielement::rRIGHT_CENTER:
		case cguielement::rTOP_RIGHT:
		case cguielement::rBOTTOM_RIGHT:
		case cguielement::rRIGHT:
			position.x = /*abs(a_Parent->GetPosition().x) + */ a_parent->get_bounds().x - bounds.x;
			if (0 > a_parent->get_position().x)
			{
				position.x *= -1;
				position.x += a_parent->get_non_clien_area().y;
			}
			else
				position.x -= a_parent->get_non_clien_area().y;
			break;
		case cguielement::rCENTER:
		case cguielement::rTOP_CENTER:
		case cguielement::rBOTTOM_CENTER:
			position.x = /*abs(a_Parent->GetPosition().x) + */ (a_parent->get_bounds().x / 2) - (bounds.x / 2);
			if (0 > a_parent->get_position().x)
			{
				position.x *= -1;
			}
			break;
	}
	// Set The Z-Order of the Element
	position.z = a_parent->get_position().z + 1;
	cguigraphics_element::set(a_parent, a_text, position, a_non_client_area, a_visible, a_ground, nullptr, nullptr, bounds, a_call_back, cguielement::rLABEL, a_font_index, a_alignment, nullptr, texture_offset, false);
}

FORCE_INLINE void cguilabel::global_initialize()
{
	// Here i can set the Font Position after the Positions are finalized
	ror::Font *font         = FONT->get_font(this->m_font_index);
	this->m_text_position.x = /*this->m_Position.x + */ this->m_non_clien_area.x;
	this->m_text_position.y = /*this->m_Position.y + */ this->m_non_clien_area.w;        //int32_t((Font->GetHeight()* 3) / 2.5f); // Some Magic Number to define the alignment
}

FORCE_INLINE void cguilabel::render()
{
	cguigraphics_element::render();
	// Render the Text on top of it
	RENDERER::save_current_color();
	RENDERER::set_color(*this->get_foreground());
	RENDERER::push_matrix();
	glTranslatef((float32_t) this->m_absolute_position.x, (float32_t) this->m_absolute_position.y, 0);
	ror::Font *font = FONT->get_font(this->get_font_index());
	assert(font != nullptr && "Font is Null");
	font->print(this->m_text_position.x, this->m_text_position.y, this->m_text.c_str());
	RENDERER::pop_matrix();
	RENDERER::restore_current_color();
}

}        // namespace ror
