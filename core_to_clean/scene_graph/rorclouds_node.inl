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
FORCE_INLINE CloudsNode::CloudsNode(void) :
    m_scrolling_speed(0.0f)
{}

FORCE_INLINE void CloudsNode::global_draw()
{
	//glDisable(GL_LIGHTING);
	glActiveTexture(GL_TEXTURE0);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	this->m_scrolling_position.call_gl_translate();
	//glActiveTexture(GL_TEXTURE1);
	glMatrixMode(GL_MODELVIEW);
	GeometryNode::global_draw();
	glActiveTexture(GL_TEXTURE0);
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	//glActiveTexture(GL_TEXTURE1);
	glMatrixMode(GL_MODELVIEW);
	//glEnable(GL_LIGHTING);
}

FORCE_INLINE void CloudsNode::global_update(rUsLong a_milli_seconds)
{
	GeometryNode::global_update(a_milli_seconds);
	this->m_scrolling_position.x += m_scrolling_speed;
}

FORCE_INLINE void CloudsNode::set_scrolling_speed(float32_t a_scrolling_speed)
{
	m_scrolling_speed = a_scrolling_speed;
}

FORCE_INLINE float32_t CloudsNode::get_scrolling_speed() const
{
	return m_scrolling_speed;
}

}        // namespace ror
