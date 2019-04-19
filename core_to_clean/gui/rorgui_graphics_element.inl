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
FORCE_INLINE cguigraphics_element::cguigraphics_element(void)
{
	this->m_vertex_buffer         = nullptr;
	this->m_texture_object        = nullptr;
	this->m_texture_offset        = ror::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);
	this->m_scalable              = false;
	this->m_draw_background       = true;
	this->m_draw_caption          = false;
	this->m_no_of_vertex_buffers  = 1;
	this->m_current_vertex_buffer = 0;
}

FORCE_INLINE bool cguigraphics_element::initialize(uint32_t a_turn)
{
	float32_t *points             = nullptr;
	float32_t *tex_coords         = nullptr;
	uint32_t   number_of_vertices = 0;

	if (0 == a_turn)        // Just Update Position and such in the First turn
	{
		// If we have any Text then Lest Draw it, this is to save the Costly String Comparison
		if ("" != this->m_text)
		{
			this->m_draw_caption = true;
		}

		int32_t viewport[4];
		RENDERER::get_viewport(viewport);

		if (nullptr != this->m_parent)
		{
			// Invert the y-axis for the GUI Elements
			if (0 <= this->m_position.y)        // If the position is specified in Minus then it means offset from bottom, and we have to invert it
			{
				this->m_position.y = this->m_parent->get_bounds().y - (this->m_bounds.y + this->m_position.y);
			}
			else
			{
				this->m_position.y *= -1;        // If the position is specified in Minus then it means offset from bottom, and we don't need to invert it
				// Invert the Alignment and Offset of Parent if its negative
				if (nullptr != this->m_parent)
				{
					this->m_position.y = (this->m_parent->get_bounds().y) - abs(this->m_position.y) - this->m_bounds.y;
				}
			}
			if (this->m_position.x < 0)
				this->m_position.x *= -1;
			if (this->m_position.y < 0)
				this->m_position.y *= -1;
		}
		else
		{
			if (0 > this->m_position.x)        // If the position is specified in Minus then it means offset from right side
			{
				this->m_position.x = viewport[2] - (this->m_bounds.x - this->m_position.x);
				// Invert the Alignment and Offset of Parent if its negative
				if (nullptr != this->m_parent)
				{
					this->m_position.x = (this->m_parent->get_position().x + this->m_parent->get_bounds().x) - abs(this->m_parent->get_position().x - this->m_position.x) - this->m_bounds.x;
				}
			}

			// Invert the y-axis for the GUI Elements
			if (0 <= this->m_position.y)        // If the position is specified in Minus then it means offset from bottom, and we have to invert it
			{
				this->m_position.y = viewport[3] - (this->m_bounds.y + this->m_position.y);
			}
			else
			{
				this->m_position.y *= -1;        // If the position is specified in Minus then it means offset from bottom, and we don't need to invert it
				// Invert the Alignment and Offset of Parent if its negative
				if (nullptr != this->m_parent)
				{
					this->m_position.y = (this->m_parent->get_position().y + this->m_parent->get_bounds().y) - abs(this->m_parent->get_position().y - this->m_position.y) - this->m_bounds.y;
				}
			}
		}
	}

	if (nullptr != this->m_texture_object && true == this->m_scalable)
	{
		if (this->m_texture_offset == Vector4f(0.0f, 0.0f, 0.0f, 0.0f))
		{
			this->m_texture_offset.x = 0.0f;
			this->m_texture_offset.z = (float32_t) this->m_texture_object->get_texture_width();
			this->m_texture_offset.y = 0.0f;
			this->m_texture_offset.w = (float32_t) this->m_texture_object->get_texture_height();
		}

		Vector4f texture_offset;
		// Normalize the TextureOffset
		try
		{
			texture_offset.x = (float32_t) this->m_texture_offset.x / (float32_t) this->m_texture_object->get_texture_width();
			texture_offset.z = (float32_t) this->m_texture_offset.z / (float32_t) this->m_texture_object->get_texture_width();
			texture_offset.y = (float32_t) this->m_texture_offset.y / (float32_t) this->m_texture_object->get_texture_height();
			texture_offset.w = (float32_t) this->m_texture_offset.w / (float32_t) this->m_texture_object->get_texture_height();
		}
		catch (int32_t)
		{
			// Division by zero
			MESSAGEBOXERROR("DIVEION BY ZERO OCCURED");
		}

		number_of_vertices = 54 * 2;
		points             = new float32_t[number_of_vertices];
		tex_coords         = new float32_t[number_of_vertices];
		// Lets Calculate the Size of texture and its midpoints
		ror::Vector2i texture_size;
		// TextureSize is caluculated to maintain 1:1 ration of GUI Element and texture
		texture_size.x = int32_t(this->m_texture_offset.z - this->m_texture_offset.x);        //TextureSize.x = this->m_TextureObject->GetTextureWidth();
		texture_size.y = int32_t(this->m_texture_offset.w - this->m_texture_offset.y);        //TextureSize.y = this->m_TextureObject->GetTextureHeight();

		ror::Vector2i texture_mid;
		texture_mid = texture_size / 2;

		// If the Size of the element is less then the Texture size
		if (this->m_bounds.x < texture_size.x)
		{
			texture_mid.x = this->m_bounds.x / 2;
		}
		if (this->m_bounds.y < texture_size.y)
		{
			texture_mid.y = this->m_bounds.y / 2;
		}

		// Lets calculate the corners of the Triangle in Screen Coordinates
		ror::Vector2f bottom_left, top_right;
		ror::Vector2f tex_coord_bottom_left(texture_offset.x, texture_offset.y);
		ror::Vector2f tex_coord_mid(texture_offset.x + ((texture_offset.z - texture_offset.x) / 2), texture_offset.y + ((texture_offset.w - texture_offset.y) / 2));
		ror::Vector2f tex_coord_top_right(texture_offset.z, texture_offset.w);

		//			if (nullptr != this->m_Parent)
		//			{
		//				BottomLeft.x = (float32_t)this->m_Position.x;
		//				BottomLeft.y = (float32_t)this->m_Position.y;
		//
		//				TopRight.x = float32_t(this->m_Position.x + this->m_Bounds.x);
		//				TopRight.y = float32_t(this->m_Position.y + this->m_Bounds.y);
		//			}
		//			else
		{
			top_right.x = float32_t(/*this->m_Position.x + */ this->m_bounds.x);
			top_right.y = float32_t(/*this->m_Position.y + */ this->m_bounds.y);
		}

		// Create Triangle 1
		points[0]     = bottom_left.x;
		points[1]     = bottom_left.y;
		tex_coords[0] = tex_coord_bottom_left.x;
		tex_coords[1] = tex_coord_bottom_left.y;

		points[2]     = bottom_left.x + texture_mid.x;
		points[3]     = bottom_left.y;
		tex_coords[2] = tex_coord_mid.x;
		tex_coords[3] = tex_coord_bottom_left.y;

		points[4]     = bottom_left.x + texture_mid.x;
		points[5]     = bottom_left.y + texture_mid.y;
		tex_coords[4] = tex_coord_mid.x;
		tex_coords[5] = tex_coord_mid.y;

		// Create Triangle 2
		points[6]     = bottom_left.x;
		points[7]     = bottom_left.y;
		tex_coords[6] = tex_coord_bottom_left.x;
		tex_coords[7] = tex_coord_bottom_left.y;

		points[8]     = bottom_left.x + texture_mid.x;
		points[9]     = bottom_left.y + texture_mid.y;
		tex_coords[8] = tex_coord_mid.x;
		tex_coords[9] = tex_coord_mid.y;

		points[10]     = bottom_left.x;
		points[11]     = bottom_left.y + texture_mid.y;
		tex_coords[10] = tex_coord_bottom_left.x;
		tex_coords[11] = tex_coord_mid.y;

		// Create Triangle 3
		points[12]     = bottom_left.x;
		points[13]     = bottom_left.y + texture_mid.y;
		tex_coords[12] = tex_coord_bottom_left.x;
		tex_coords[13] = tex_coord_mid.y;

		points[14]     = bottom_left.x + texture_mid.x;
		points[15]     = bottom_left.y + texture_mid.y;
		tex_coords[14] = tex_coord_mid.x;
		tex_coords[15] = tex_coord_mid.y;

		points[16]     = bottom_left.x + texture_mid.x;
		points[17]     = top_right.y - texture_mid.y;
		tex_coords[16] = tex_coord_mid.x;
		tex_coords[17] = tex_coord_mid.y;

		// Create Triangle 4
		points[18]     = bottom_left.x;
		points[19]     = bottom_left.y + texture_mid.y;
		tex_coords[18] = tex_coord_bottom_left.x;
		tex_coords[19] = tex_coord_mid.y;

		points[20]     = bottom_left.x + texture_mid.x;
		points[21]     = top_right.y - texture_mid.y;
		tex_coords[20] = tex_coord_mid.x;
		tex_coords[21] = tex_coord_mid.y;

		points[22]     = bottom_left.x;
		points[23]     = top_right.y - texture_mid.y;
		tex_coords[22] = tex_coord_bottom_left.x;
		tex_coords[23] = tex_coord_mid.y;

		// Create Triangle 5
		points[24]     = bottom_left.x;
		points[25]     = top_right.y - texture_mid.y;
		tex_coords[24] = tex_coord_bottom_left.x;
		tex_coords[25] = tex_coord_mid.y;

		points[26]     = bottom_left.x + texture_mid.x;
		points[27]     = top_right.y - texture_mid.y;
		tex_coords[26] = tex_coord_mid.x;
		tex_coords[27] = tex_coord_mid.y;

		points[28]     = bottom_left.x + texture_mid.x;
		points[29]     = top_right.y;
		tex_coords[28] = tex_coord_mid.x;
		tex_coords[29] = tex_coord_top_right.y;

		// Create Triangle 6
		points[30]     = bottom_left.x;
		points[31]     = top_right.y - texture_mid.y;
		tex_coords[30] = tex_coord_bottom_left.x;
		tex_coords[31] = tex_coord_mid.y;

		points[32]     = bottom_left.x + texture_mid.x;
		points[33]     = top_right.y;
		tex_coords[32] = tex_coord_mid.x;
		tex_coords[33] = tex_coord_top_right.y;

		points[34]     = bottom_left.x;
		points[35]     = top_right.y;
		tex_coords[34] = tex_coord_bottom_left.x;
		tex_coords[35] = tex_coord_top_right.y;

		// Third Row
		bottom_left.x = top_right.x - texture_mid.x;
		// Create Triangle 1
		points[36]     = bottom_left.x;
		points[37]     = bottom_left.y;
		tex_coords[36] = tex_coord_mid.x;
		tex_coords[37] = tex_coord_bottom_left.y;

		points[38]     = bottom_left.x + texture_mid.x;
		points[39]     = bottom_left.y;
		tex_coords[38] = tex_coord_top_right.x;
		tex_coords[39] = tex_coord_bottom_left.y;

		points[40]     = bottom_left.x + texture_mid.x;
		points[41]     = bottom_left.y + texture_mid.y;
		tex_coords[40] = tex_coord_top_right.x;
		tex_coords[41] = tex_coord_mid.y;

		// Create Triangle 2
		points[42]     = bottom_left.x;
		points[43]     = bottom_left.y;
		tex_coords[42] = tex_coord_mid.x;
		tex_coords[43] = tex_coord_bottom_left.y;

		points[44]     = bottom_left.x + texture_mid.x;
		points[45]     = bottom_left.y + texture_mid.y;
		tex_coords[44] = tex_coord_top_right.x;
		tex_coords[45] = tex_coord_mid.y;

		points[46]     = bottom_left.x;
		points[47]     = bottom_left.y + texture_mid.y;
		tex_coords[46] = tex_coord_mid.x;
		tex_coords[47] = tex_coord_mid.y;

		// Create Triangle 3
		points[48]     = bottom_left.x;
		points[49]     = bottom_left.y + texture_mid.y;
		tex_coords[48] = tex_coord_mid.x;
		tex_coords[49] = tex_coord_mid.y;

		points[50]     = bottom_left.x + texture_mid.x;
		points[51]     = bottom_left.y + texture_mid.y;
		tex_coords[50] = tex_coord_top_right.x;
		tex_coords[51] = tex_coord_mid.y;

		points[52]     = bottom_left.x + texture_mid.x;
		points[53]     = top_right.y - texture_mid.y;
		tex_coords[52] = tex_coord_top_right.x;
		tex_coords[53] = tex_coord_mid.y;

		// Create Triangle 4
		points[54]     = bottom_left.x;
		points[55]     = bottom_left.y + texture_mid.y;
		tex_coords[54] = tex_coord_mid.x;
		tex_coords[55] = tex_coord_mid.y;

		points[56]     = bottom_left.x + texture_mid.x;
		points[57]     = top_right.y - texture_mid.y;
		tex_coords[56] = tex_coord_top_right.x;
		tex_coords[57] = tex_coord_mid.y;

		points[58]     = bottom_left.x;
		points[59]     = top_right.y - texture_mid.y;
		tex_coords[58] = tex_coord_mid.x;
		tex_coords[59] = tex_coord_mid.y;

		// Create Triangle 5
		points[60]     = bottom_left.x;
		points[61]     = top_right.y - texture_mid.y;
		tex_coords[60] = tex_coord_mid.x;
		tex_coords[61] = tex_coord_mid.y;

		points[62]     = bottom_left.x + texture_mid.x;
		points[63]     = top_right.y - texture_mid.y;
		tex_coords[62] = tex_coord_top_right.x;
		tex_coords[63] = tex_coord_mid.y;

		points[64]     = bottom_left.x + texture_mid.x;
		points[65]     = top_right.y;
		tex_coords[64] = tex_coord_top_right.x;
		tex_coords[65] = tex_coord_top_right.y;

		// Create Triangle 6
		points[66]     = bottom_left.x;
		points[67]     = top_right.y - texture_mid.y;
		tex_coords[66] = tex_coord_mid.x;
		tex_coords[67] = tex_coord_mid.y;

		points[68]     = bottom_left.x + texture_mid.x;
		points[69]     = top_right.y;
		tex_coords[68] = tex_coord_top_right.x;
		tex_coords[69] = tex_coord_top_right.y;

		points[70]     = bottom_left.x;
		points[71]     = top_right.y;
		tex_coords[70] = tex_coord_mid.x;
		tex_coords[71] = tex_coord_top_right.y;

		// Second Row
		//			if (nullptr != this->m_Parent)
		//				BottomLeft.x = float32_t(this->m_Position.x + TextureMid.x);
		//			else
		bottom_left.x = float32_t(/*this->m_Position.x + */ texture_mid.x);

		texture_mid.x = ((int32_t) top_right.x - texture_mid.x) - (int32_t) bottom_left.x;
		// Create Triangle 1
		points[72]     = bottom_left.x;
		points[73]     = bottom_left.y;
		tex_coords[72] = tex_coord_mid.x;
		tex_coords[73] = tex_coord_bottom_left.y;

		points[74]     = bottom_left.x + texture_mid.x;
		points[75]     = bottom_left.y;
		tex_coords[74] = tex_coord_mid.x;
		tex_coords[75] = tex_coord_bottom_left.y;

		points[76]     = bottom_left.x + texture_mid.x;
		points[77]     = bottom_left.y + texture_mid.y;
		tex_coords[76] = tex_coord_mid.x;
		tex_coords[77] = tex_coord_mid.y;

		// Create Triangle 2
		points[78]     = bottom_left.x;
		points[79]     = bottom_left.y;
		tex_coords[78] = tex_coord_mid.x;
		tex_coords[79] = tex_coord_bottom_left.y;

		points[80]     = bottom_left.x + texture_mid.x;
		points[81]     = bottom_left.y + texture_mid.y;
		tex_coords[80] = tex_coord_mid.x;
		tex_coords[81] = tex_coord_mid.y;

		points[82]     = bottom_left.x;
		points[83]     = bottom_left.y + texture_mid.y;
		tex_coords[82] = tex_coord_mid.x;
		tex_coords[83] = tex_coord_mid.y;

		// Create Triangle 3
		points[84]     = bottom_left.x;
		points[85]     = bottom_left.y + texture_mid.y;
		tex_coords[84] = tex_coord_mid.x;
		tex_coords[85] = tex_coord_mid.y;

		points[86]     = bottom_left.x + texture_mid.x;
		points[87]     = bottom_left.y + texture_mid.y;
		tex_coords[86] = tex_coord_mid.x;
		tex_coords[87] = tex_coord_mid.y;

		points[88]     = bottom_left.x + texture_mid.x;
		points[89]     = top_right.y - texture_mid.y;
		tex_coords[88] = tex_coord_mid.x;
		tex_coords[89] = tex_coord_mid.y;

		// Create Triangle 4
		points[90]     = bottom_left.x;
		points[91]     = bottom_left.y + texture_mid.y;
		tex_coords[90] = tex_coord_mid.x;
		tex_coords[91] = tex_coord_mid.y;

		points[92]     = bottom_left.x + texture_mid.x;
		points[93]     = top_right.y - texture_mid.y;
		tex_coords[92] = tex_coord_mid.x;
		tex_coords[93] = tex_coord_mid.y;

		points[94]     = bottom_left.x;
		points[95]     = top_right.y - texture_mid.y;
		tex_coords[94] = tex_coord_mid.x;
		tex_coords[95] = tex_coord_mid.y;

		// Create Triangle 5
		points[96]     = bottom_left.x;
		points[97]     = top_right.y - texture_mid.y;
		tex_coords[96] = tex_coord_mid.x;
		tex_coords[97] = tex_coord_mid.y;

		points[98]     = bottom_left.x + texture_mid.x;
		points[99]     = top_right.y - texture_mid.y;
		tex_coords[98] = tex_coord_mid.x;
		tex_coords[99] = tex_coord_mid.y;

		points[100]     = bottom_left.x + texture_mid.x;
		points[101]     = top_right.y;
		tex_coords[100] = tex_coord_mid.x;
		tex_coords[101] = tex_coord_top_right.y;

		// Create Triangle 6
		points[102]     = bottom_left.x;
		points[103]     = top_right.y - texture_mid.y;
		tex_coords[102] = tex_coord_mid.x;
		tex_coords[103] = tex_coord_mid.y;

		points[104]     = bottom_left.x + texture_mid.x;
		points[105]     = top_right.y;
		tex_coords[104] = tex_coord_mid.x;
		tex_coords[105] = tex_coord_top_right.y;

		points[106]     = bottom_left.x;
		points[107]     = top_right.y;
		tex_coords[106] = tex_coord_mid.x;
		tex_coords[107] = tex_coord_top_right.y;
		this->fill_vertex_buffer(a_turn, number_of_vertices, 2, points, tex_coords);
	}
	else if (nullptr != this->m_texture_object && false == this->m_scalable)
	{
		if (this->m_texture_offset == Vector4f(0.0f, 0.0f, 0.0f, 0.0f))
		{
			this->m_texture_offset.x = 0.0f;
			this->m_texture_offset.z = (float32_t) this->m_texture_object->get_texture_width();
			this->m_texture_offset.y = 0.0f;
			this->m_texture_offset.w = (float32_t) this->m_texture_object->get_texture_height();
		}

		number_of_vertices = 6 * 2;
		points             = new float32_t[number_of_vertices];
		tex_coords         = new float32_t[number_of_vertices];

		Vector4f texture_offset;
		// Normalize the TextureOffset
		try
		{
			texture_offset.x = (float32_t) this->m_texture_offset.x / (float32_t) this->m_texture_object->get_texture_width();
			texture_offset.z = (float32_t) this->m_texture_offset.z / (float32_t) this->m_texture_object->get_texture_width();
			texture_offset.y = (float32_t) this->m_texture_offset.y / (float32_t) this->m_texture_object->get_texture_height();
			texture_offset.w = (float32_t) this->m_texture_offset.w / (float32_t) this->m_texture_object->get_texture_height();
		}
		catch (int32_t)
		{
			// Division by zero
			MESSAGEBOXERROR("DIVEION BY ZERO OCCURED");
		}
		// Lets calculate the corners of the Triangle in Screen Coordinates
		ror::Vector2f bottom_left, top_right;
		ror::Vector2f tex_coord_bottom_left(texture_offset.x, texture_offset.y);
		ror::Vector2f tex_coord_top_right(texture_offset.z, texture_offset.w);

		//			if (nullptr != this->m_Parent)
		//			{
		//				BottomLeft.x = (float32_t)this->m_Position.x;
		//				BottomLeft.y = (float32_t)this->m_Position.y;
		//
		//				TopRight.x = float32_t(this->m_Position.x + this->m_Bounds.x);
		//				TopRight.y = float32_t(this->m_Position.y + this->m_Bounds.y);
		//			}
		//			else
		{
			top_right.x = float32_t(/*this->m_Position.x + */ this->m_bounds.x);
			top_right.y = float32_t(/*this->m_Position.y + */ this->m_bounds.y);
		}

		// Create Triangle 1
		points[0]     = bottom_left.x;
		points[1]     = bottom_left.y;
		tex_coords[0] = tex_coord_bottom_left.x;
		tex_coords[1] = tex_coord_bottom_left.y;

		points[2]     = top_right.x;
		points[3]     = bottom_left.y;
		tex_coords[2] = tex_coord_top_right.x;
		tex_coords[3] = tex_coord_bottom_left.y;

		points[4]     = top_right.x;
		points[5]     = top_right.y;
		tex_coords[4] = tex_coord_top_right.x;
		tex_coords[5] = tex_coord_top_right.y;

		// Create Triangle 2
		points[6]     = top_right.x;
		points[7]     = top_right.y;
		tex_coords[6] = tex_coord_top_right.x;
		tex_coords[7] = tex_coord_top_right.y;

		points[8]     = bottom_left.x;
		points[9]     = top_right.y;
		tex_coords[8] = tex_coord_bottom_left.x;
		tex_coords[9] = tex_coord_top_right.y;

		points[10]     = bottom_left.x;
		points[11]     = bottom_left.y;
		tex_coords[10] = tex_coord_bottom_left.x;
		tex_coords[11] = tex_coord_bottom_left.y;

		this->fill_vertex_buffer(a_turn, number_of_vertices, 2, points, tex_coords);
	}
	else
	{
		number_of_vertices = 6 * 2;
		points             = new float32_t[number_of_vertices];

		// Lets calculate the corners of the Triangle in Screen Coordinates
		ror::Vector2f bottom_left, top_right;

		//			if (nullptr != this->m_Parent)
		//			{
		//				BottomLeft.x = (float32_t)this->m_Position.x;
		//				BottomLeft.y = (float32_t)this->m_Position.y;
		//
		//				TopRight.x = float32_t(this->m_Position.x + this->m_Bounds.x);
		//				TopRight.y = float32_t(this->m_Position.y + this->m_Bounds.y);
		//			}
		//			else
		{
			top_right.x = float32_t(/*this->m_Position.x + */ this->m_bounds.x);
			top_right.y = float32_t(/*this->m_Position.y + */ this->m_bounds.y);
		}

		// Create Triangle 1
		points[0] = bottom_left.x;
		points[1] = bottom_left.y;

		points[2] = top_right.x;
		points[3] = bottom_left.y;

		points[4] = top_right.x;
		points[5] = top_right.y;

		// Create Triangle 2
		points[6] = top_right.x;
		points[7] = top_right.y;

		points[8] = bottom_left.x;
		points[9] = top_right.y;

		points[10] = bottom_left.x;
		points[11] = bottom_left.y;

		this->fill_vertex_buffer(a_turn, number_of_vertices, 1, points, tex_coords);
	}

	rDELETEARRAY(points);
	rDELETEARRAY(tex_coords);

	if (rPANEL == this->m_element_type)
	{
		// Here i can set the Font Position after the Positions are finalized
		ror::Font *font         = FONT->get_font(this->get_font_index());
		this->m_text_position.x = this->m_non_clien_area.x;
		this->m_text_position.y = this->m_bounds.y - this->m_non_clien_area.z + ((this->m_non_clien_area.z - int32_t(font->get_height())) / 2);        // Some Magic Number to define the alignment
	}

	if (0 == a_turn)
	{
		this->m_absolute_position = this->m_position;
		if (nullptr != this->m_parent)
		{
			this->m_absolute_position += this->m_parent->get_position();
		}
		this->global_initialize();        // Let the Derived Classes do their initialization
	}

	return true;
}

FORCE_INLINE void cguigraphics_element::fill_vertex_buffer(uint32_t a_index, uint32_t a_number_of_vertices, uint32_t a_number_of_attributes, float32_t *a_points, float32_t *a_tex_coords)
{
	// And put them directly in VBOs
	if (nullptr == this->m_vertex_buffer)
	{
		this->m_vertex_buffer = new ror::OGLVertexBuffer *[this->m_no_of_vertex_buffers];
		for (uint32_t i = 0; i < this->m_no_of_vertex_buffers; i++)
		{
			this->m_vertex_buffer[i] = rNew ror::OGLVertexBuffer();
			assert(nullptr != this->m_vertex_buffer[i] && "Not Enough Memory : Filename : RORGuiGraphicsElement.inl");
		}
	}
	//this->m_VertexBuffer[a_Index] = rNew CoreEngine::COGLVertexBuffer();
	//assert(nullptr != this->m_VertexBuffer[] && "Not Enough Memory : Filename : RORGuiGraphicsElement.inl");

	// Fill the Data in VBO
	this->m_vertex_buffer[a_index]->set_no_of_attributes(a_number_of_attributes);
	this->m_vertex_buffer[a_index]->set_total_no_of_attributes(a_number_of_vertices / 2);
	ror::VertexAttributes *temp_attribs = rNew ror::VertexAttributes[a_number_of_attributes];

	temp_attribs[0].set_vertex_attribute(ror::VertexAttributes::rVERTICES);
	temp_attribs[0].set_offset(0);
	temp_attribs[0].set_no_of_components(2);        // For Positions
	temp_attribs[0].set_vbodata(a_points);

	if (a_number_of_attributes > 1)
	{
		temp_attribs[1].set_vertex_attribute(ror::VertexAttributes::rTEXCOORDS);
		temp_attribs[1].set_offset(a_number_of_vertices * sizeof(float32_t));
		temp_attribs[1].set_no_of_components(2);        // For TextureCoordinates
		temp_attribs[1].set_vbodata(a_tex_coords);
	}

	this->m_vertex_buffer[a_index]->set_attributes(temp_attribs);
	this->m_vertex_buffer[a_index]->set_size_of_vbo(a_number_of_attributes * (a_number_of_vertices * sizeof(float32_t)));
	this->m_vertex_buffer[a_index]->load_vertex_buffer();
	// TODO : Verify the Outcomes
}

FORCE_INLINE bool cguigraphics_element::update(float32_t a_delta_time)
{}

FORCE_INLINE void cguigraphics_element::render()
{
	if (this->m_draw_background)
	{
		static Vector3f white_color(1.0f, 1.0f, 1.0f);
		if (nullptr != this->m_texture_object)
		{
			glActiveTexture(GL_TEXTURE0);
			this->m_texture_object->bind_texture();
		}
		else
		{
			RENDERER::save_current_color();
			RENDERER::set_color(*this->get_background());
		}

		//		glVertexPointer(2, GL_FLOAT, 0, Points);
		//		glEnableClientState(GL_VERTEX_ARRAY);
		//
		//		if (nullptr != this->m_TextureObject)
		//		{
		//			glTexCoordPointer(2, GL_FLOAT, 0, TexCoords) ;
		//			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//		}
		//
		//		glDrawArrays(GL_TRIANGLES, 0, CurrentVertices);
		//
		//		if (nullptr != this->m_TextureObject)
		//		{
		//			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//		}
		//		glDisableClientState(GL_VERTEX_ARRAY);

		glPushMatrix();
		this->m_vertex_buffer[this->m_current_vertex_buffer]->bind_buffer(true);
		glTranslatef((float32_t) this->m_absolute_position.x, (float32_t) this->m_absolute_position.y, 0.0f);

		this->m_vertex_buffer[this->m_current_vertex_buffer]->draw_arrays(GL_TRIANGLES);

		if (nullptr == this->m_texture_object)        // If he don't have a texture then lets draw its border
		{
			RENDERER::set_color(white_color);
			this->m_vertex_buffer[this->m_current_vertex_buffer]->draw_arrays(GL_LINE_STRIP);
		}

		//			glBegin(GL_LINES);
		//				glVertex2f(this->m_AbsolutePosition.x,this->m_AbsolutePosition.y);
		//				glVertex2f(this->m_AbsolutePosition.x + this->m_Bounds.x,this->m_AbsolutePosition.y + this->m_Bounds.y);
		//			glEnd();

		this->m_vertex_buffer[this->m_current_vertex_buffer]->bind_buffer(false);

		if (nullptr != this->m_texture_object)
		{
			glActiveTexture(GL_TEXTURE0);
			this->m_texture_object->bind_texture(false);
		}
		else
		{
			RENDERER::restore_current_color();
		}

		// Draw the Caption
		if (rPANEL == this->m_element_type && this->m_draw_caption /*&& "" != this->m_Text*/)
		{
			ror::Font *font = FONT->get_font(this->get_font_index());
			if (nullptr != font)
			{
				font->print(this->m_text_position.x, this->m_text_position.y, this->m_text.c_str());
			}
		}

		glPopMatrix();
	}
}

FORCE_INLINE OGLVertexBuffer **cguigraphics_element::get_vertex_buffer() const
{
	return this->m_vertex_buffer;
}
//
//	FORCE_INLINE void CGUIGraphicsElement::SetVertexBuffer( COGLVertexBuffer **a_VBO )
//	{
//		this->m_VertexBuffer = a_VBO;
//	}

FORCE_INLINE TextureObject *cguigraphics_element::get_texture_object() const
{
	return this->m_texture_object;
}

FORCE_INLINE void cguigraphics_element::set_texture_object(TextureObject *a_texture_object)
{
	this->m_texture_object = a_texture_object;
}

FORCE_INLINE Vector4f cguigraphics_element::get_texture_offset() const
{
	return this->m_texture_offset;
}

FORCE_INLINE void cguigraphics_element::set_texture_offset(Vector4f &a_texture_offset)
{
	this->m_texture_offset = a_texture_offset;
}

FORCE_INLINE bool cguigraphics_element::get_scalable_texture() const
{
	return this->m_scalable;
}

FORCE_INLINE void cguigraphics_element::set_scalable_texture(bool a_scalable_texture)
{
	this->m_scalable = a_scalable_texture;
}

FORCE_INLINE void cguigraphics_element::set(cguielement *a_parent, std::string a_text, Vector3i &a_position, Vector4i &a_non_client_area, bool a_visible, bool a_ground, Vector4f *a_background, Vector3f *a_foreground, Vector2i &a_bounds, IFunctor *a_call_back, EElementType a_element_type, int32_t a_font_index, EAlignment a_alighment, TextureObject *a_texture_object, Vector4f a_texture_offset, bool a_scaleable)
{
	this->m_text               = a_text;
	this->m_parent             = a_parent;
	this->m_position           = a_position;
	this->m_background         = a_background;
	this->m_foreground         = a_foreground;
	this->m_bounds             = a_bounds;
	this->m_call_back          = a_call_back;
	this->m_element_type       = a_element_type;
	this->m_font_index         = a_font_index;
	this->m_texture_object     = a_texture_object;
	this->m_texture_offset     = a_texture_offset;
	this->m_scalable           = a_scaleable;
	this->m_non_clien_area     = a_non_client_area;
	this->m_sequence_in_parent = a_non_client_area.z;
	this->m_visible            = a_visible;
	this->m_draw_background    = a_ground;

	// Define Alignment as well
	if (nullptr == this->m_parent)        // Define Alignments for Parents
	{
		if (this->m_position.x < 0 && this->m_position.y < 0)
			this->m_alignment = rBOTTOM_RIGHT;
		if (this->m_position.x > 0 && this->m_position.y > 0)
			this->m_alignment = rTOP_LEFT;
		if (this->m_position.x < 0 && this->m_position.y > 0)
			this->m_alignment = rTOP_RIGHT;
		if (this->m_position.x > 0 && this->m_position.y < 0)
			this->m_alignment = rBOTTOM_LEFT;
	}
}

FORCE_INLINE void cguigraphics_element::set_draw_background(bool a_draw_background)
{
	this->m_draw_background = a_draw_background;
}

FORCE_INLINE void cguigraphics_element::toggle_background()
{
	this->m_draw_background = (this->m_draw_background ? false : true);
}

FORCE_INLINE bool cguigraphics_element::get_draw_background() const
{
	return this->m_draw_background;
}

FORCE_INLINE void cguigraphics_element::set_no_of_vertex_buffers(uint32_t a_no_of_vbos)
{
	this->m_no_of_vertex_buffers = a_no_of_vbos;
}

FORCE_INLINE uint32_t cguigraphics_element::get_no_of_vertex_buffers() const
{
	return this->m_no_of_vertex_buffers;
}

FORCE_INLINE uint32_t cguigraphics_element::get_current_vertex_buffer() const
{
	return this->m_current_vertex_buffer;
}

FORCE_INLINE void cguigraphics_element::set_current_vertex_buffer(uint32_t a_active_vbo)
{
	this->m_current_vertex_buffer = a_active_vbo;
}

FORCE_INLINE void cguigraphics_element::increment_current_vertex_buffer()
{
	this->m_current_vertex_buffer++;
	if (this->m_no_of_vertex_buffers == this->m_current_vertex_buffer)
	{
		this->m_current_vertex_buffer = 0;
	}
}

}        // namespace ror
