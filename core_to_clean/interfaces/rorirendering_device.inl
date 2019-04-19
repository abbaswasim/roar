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

namespace rendering
{
FORCE_INLINE IRenderingDevice::IRenderingDevice(void)
{
	this->m_active_camera     = nullptr;
	this->m_model_view_matrix = rNew ror::Matrix4f();
	this->m_projection_matrix = rNew ror::Matrix4f();
	this->m_combined_matrix   = rNew   ror::Matrix4f();
}

// TODO : Take all this to OGLRenderingDevice class
FORCE_INLINE ror::Matrix4f *IRenderingDevice::update_projection_matrix()
{
	glGetFloatv(GL_PROJECTION_MATRIX, this->m_projection_matrix->m_values);
	return this->m_projection_matrix;
}

FORCE_INLINE ror::Matrix4f *IRenderingDevice::update_model_view_matrix()
{
	glGetFloatv(GL_MODELVIEW_MATRIX, this->m_model_view_matrix->m_values);
	return this->m_model_view_matrix;
}

FORCE_INLINE ror::Matrix4f *IRenderingDevice::update_combined_matrix()
{
	glPushMatrix();
	glGetFloatv(GL_PROJECTION_MATRIX, this->m_projection_matrix->m_values);
	glGetFloatv(GL_MODELVIEW_MATRIX, this->m_model_view_matrix->m_values);
	glLoadMatrixf(this->m_projection_matrix->m_values);
	glMultMatrixf(this->m_model_view_matrix->m_values);
	glGetFloatv(GL_MODELVIEW_MATRIX, this->m_combined_matrix->m_values);
	glPopMatrix();
	return this->m_combined_matrix;
}

FORCE_INLINE ror::Matrix4f *IRenderingDevice::get_projection_matrix()
{
	return this->m_projection_matrix;
}

FORCE_INLINE ror::Matrix4f *IRenderingDevice::get_model_view_matrix()
{
	return this->m_model_view_matrix;
}

FORCE_INLINE ror::Matrix4f *IRenderingDevice::get_combined_matrix()
{
	return this->m_combined_matrix;
}

}        // namespace rendering
