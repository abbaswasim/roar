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
FORCE_INLINE FirstPersonCamera::FirstPersonCamera(void)
{
	*this->m_camera_position    = Vector3f(0.0f, 0.0f, 0.0f);
	*this->m_camera_focus_point = Vector3f(0.0f, 0.0f, 0.0f);
	*this->m_camera_up_vector   = Vector3f(0.0f, 1.0f, 0.0f);
	ror::Camera::setup();
}

FORCE_INLINE void FirstPersonCamera::global_look_at(Vector3f &a_look_at)
{}

FORCE_INLINE void FirstPersonCamera::global_look_at()
{
	glLoadMatrixf(this->m_camera_matrix->m_values);
}

FORCE_INLINE void FirstPersonCamera::global_update(int32_t &a_x_delta, int32_t &a_y_delta)
{
	int32_t dif_x;
	int32_t dif_y;
	this->m_mouse->difference(a_x_delta, a_y_delta, dif_x, dif_y);
	this->update(dif_x, dif_y);
}

FORCE_INLINE void FirstPersonCamera::update(int32_t &a_x_delta, int32_t &a_y_delta)
{
	this->mouse_setup(a_x_delta, a_y_delta);
	ror::Camera::setup();
}

FORCE_INLINE void FirstPersonCamera::global_update_newtonian(rUsLong a_milli_seconds)
{
	if (this->get_in_motion())
	{
		ror::Moveable::update_moveable((float32_t) a_milli_seconds * 0.001f);
		*this->m_camera_position = *this->m_position;
		this->update_transformation_matrices();
		ror::Camera::setup();
	}
}

FORCE_INLINE void FirstPersonCamera::mouse_setup(int32_t &a_x_delta, int32_t &a_y_delta)
{
	if (this->m_mouse->is_left_button_down())
	{
		float32_t x_angle = (float32_t)(-0.05f * (float32_t) a_x_delta);
		float32_t y_angle = (float32_t)(-0.05f * (float32_t) a_y_delta);

		this->rotate(*this->m_camera_up_vector, x_angle);
		this->rotate(*this->m_camera_right_vector, y_angle);
	}
}

FORCE_INLINE void FirstPersonCamera::move(float32_t &a_distance)
{
	Vector3f temp = *this->m_camera_focus_point;

	temp -= *this->m_camera_position;
	temp.normalize();

	temp *= a_distance;

	*this->m_camera_position += temp;
	*this->m_camera_focus_point += temp;

	// Move as Entity as well
	*this->m_position = *this->m_camera_position;
	this->update_transformation_matrices();
}

FORCE_INLINE void FirstPersonCamera::strafe(float32_t &a_distance)
{
	this->m_camera_position->x += this->m_camera_right_vector->x * a_distance;
	this->m_camera_position->z += this->m_camera_right_vector->z * a_distance;

	this->m_camera_focus_point->x += this->m_camera_right_vector->x * a_distance;
	this->m_camera_focus_point->z += this->m_camera_right_vector->z * a_distance;

	// Move as Entity as well
	*this->m_position = *this->m_camera_position;
	this->update_transformation_matrices();
}

FORCE_INLINE void FirstPersonCamera::elevate(float32_t &a_distance)
{
	this->m_camera_position->y += a_distance;
	this->m_camera_focus_point->y += a_distance;

	// Move as Entity as well
	*this->m_position = *this->m_camera_position;
	this->update_transformation_matrices();
}

FORCE_INLINE void FirstPersonCamera::rotate(Vector3f &a_axis, float32_t &a_angle)
{
	// This part of the code is borrowed from NVDIA Grass Demo
	Vector3f new_view,
	    v_view = *this->m_camera_focus_point;

	float32_t cos_theta = cos(RADIANS(a_angle)),
	          sin_theta = sin(RADIANS(a_angle));

	v_view -= *this->m_camera_position;

	// Find the new x *this->m_CameraPosition for the new rotated point
	new_view.x = (cos_theta + (1 - cos_theta) * a_axis.x * a_axis.x) * v_view.x;
	new_view.x += ((1 - cos_theta) * a_axis.x * a_axis.y - a_axis.z * sin_theta) * v_view.y;
	new_view.x += ((1 - cos_theta) * a_axis.x * a_axis.z + a_axis.y * sin_theta) * v_view.z;

	// Find the new y *this->m_CameraPosition for the new rotated point
	new_view.y = ((1 - cos_theta) * a_axis.x * a_axis.y + a_axis.z * sin_theta) * v_view.x;
	new_view.y += (cos_theta + (1 - cos_theta) * a_axis.y * a_axis.y) * v_view.y;
	new_view.y += ((1 - cos_theta) * a_axis.y * a_axis.z - a_axis.x * sin_theta) * v_view.z;

	// Find the new z *this->m_CameraPosition for the new rotated point
	new_view.z = ((1 - cos_theta) * a_axis.x * a_axis.z - a_axis.y * sin_theta) * v_view.x;
	new_view.z += ((1 - cos_theta) * a_axis.y * a_axis.z + a_axis.x * sin_theta) * v_view.y;
	new_view.z += (cos_theta + (1 - cos_theta) * a_axis.z * a_axis.z) * v_view.z;

	// Now we just add the newly rotated vector to our *this->m_CameraPosition to set
	// our new rotated view of our camera.
	*this->m_camera_focus_point = *this->m_camera_position;
	*this->m_camera_focus_point += new_view;
}

//	FORCE_INLINE void CFirstPersonCamera::MoveLocal( float32_t &a_X, float32_t &a_Y, float32_t &a_Z, float32_t &a_Distance )
//	{
//		this->MoveLocal(CVector3f(a_X,a_Y,a_Z),a_Distance);
//	}
//
//	FORCE_INLINE void CFirstPersonCamera::MoveLocal( CVector3f &a_Direction, float32_t &a_Distance )
//	{
//		a_Direction *= a_Distance;
//		CVector3f temp = this->m_CameraMatrix->TranslationVector();
//		temp += a_Direction;
//		this->m_CameraMatrix->SetTranslationVector(temp);
//	}
//
//	FORCE_INLINE void CFirstPersonCamera::MoveGlobal( float32_t &a_X, float32_t &a_Y, float32_t &a_Z, float32_t &a_Distance )
//	{
//		this->MoveGlobal(CVector3f(a_X,a_Y,a_Z),a_Distance);
//	}
//
//	FORCE_INLINE void CFirstPersonCamera::MoveGlobal( CVector3f &a_Direction, float32_t &a_Distance )
//	{
//		this->m_CameraMatrix->InverseRotateVector(a_Direction.m_Values);
//
//		a_Direction.Normalize();
//		a_Direction *= a_Distance;
//
//		this->m_CameraMatrix->SetTranslationVector(this->m_CameraMatrix->TranslationVector() + a_Direction);
//	}
//
//	FORCE_INLINE void CFirstPersonCamera::RotateLocal( CVector3f &a_Axis, float32_t &a_Angle )
//	{
//		glPushMatrix();
//		glLoadMatrixf(this->m_CameraMatrix->m_Values);
//		glRotatef(a_Angle,a_Axis.a_Axis.x,a_Axis.a_Axis.y,a_Axis.a_Axis.z);
//		glGetFloatv(GL_MODELVIEW_MATRIX,this->m_CameraMatrix->m_Values);
//		glPopMatrix();
//	}
//
//	FORCE_INLINE void CFirstPersonCamera::RotateGlobal( CVector3f &a_Axis, float32_t &a_Angle )
//	{
//		this->m_CameraMatrix->InverseRotateVector(a_Axis.m_Values);
//		glMatrixMode(GL_MODELVIEW);
//		glPushMatrix();
//		glLoadMatrixf(this->m_CameraMatrix->m_Values);
//		glRotatef(a_Angle, a_Axis.a_Axis.x,a_Axis.a_Axis.y,a_Axis.a_Axis.z);
//		glGetFloatv(GL_MODELVIEW_MATRIX, this->m_CameraMatrix->m_Values);
//		glPopMatrix();
//	}
//
}        // namespace ror
