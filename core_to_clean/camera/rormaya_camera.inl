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
FORCE_INLINE MayaCamera::MayaCamera(void)
{
	this->m_z_cam   = 25;
	this->m_x_rot   = 15.0f;
	this->m_y_rot   = 25.0f;
	this->m_x_trans = 0;
	this->m_y_trans = 0;

	//		glMatrixMode(GL_MODELVIEW);
	//		glLoadIdentity();
	//		float32_t viewmatrix[16]={
	//
	//			Transform[0], Transform[4], -Transform[8], 0,
	//			Transform[1], Transform[5], -Transform[9], 0,
	//			Transform[2], Transform[6], -Transform[10], 0,
	//
	//			-(Transform[0]*Transform[12] + Transform[1]*Transform[13] + Transform[2]*Transform[14]),
	//
	//			-(Transform[4]*Transform[12] + Transform[5]*Transform[13] + Transform[6]*Transform[14]),
	//
	//			(Transform[8]*Transform[12] + Transform[9]*Transform[13] + Transform[10]*Transform[14]), 1};
	//		glLoadMatrixf(viewmatrix);
}

FORCE_INLINE void MayaCamera::global_look_at(Vector3f &a_look_at)
{}

FORCE_INLINE void MayaCamera::global_look_at()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(this->m_x_trans, this->m_y_trans, -this->m_z_cam);        // X and Y translation of the camera
	glRotatef(this->m_x_rot, 1.0, 0.0, 0.0);                               // Rotation along the X-Axis
	glRotatef(this->m_y_rot, 0.0, 1.0, 0.0);                               // Rotation along the Y-Axis
}

FORCE_INLINE void MayaCamera::global_update(int32_t &a_x_delta, int32_t &a_y_delta)
{
	int32_t dif_x;
	int32_t dif_y;
	this->m_mouse->difference(a_x_delta, a_y_delta, dif_x, dif_y);
	this->update(dif_x, dif_y);
}

FORCE_INLINE void MayaCamera::update(int32_t &a_x_delta, int32_t &a_y_delta)
{
	if (this->m_mouse->is_left_button_down())
	{
		this->m_x_rot = this->m_x_rot + (float32_t)(0.05f * (float32_t) a_y_delta);
		this->m_y_rot = this->m_y_rot + (float32_t)(0.05f * (float32_t) a_x_delta);
	}
	if (this->m_mouse->is_middle_button_down())
	{
		this->m_x_trans = this->m_x_trans + (float32_t)(0.05f * (float32_t) a_x_delta);
		this->m_y_trans = this->m_y_trans - (float32_t)(0.05f * (float32_t) a_y_delta);
	}
	if (this->m_mouse->is_right_button_down())
	{
		this->m_z_cam = this->m_z_cam + (float32_t)(0.05f * (float32_t) a_x_delta);
	}
}

}        // namespace ror
