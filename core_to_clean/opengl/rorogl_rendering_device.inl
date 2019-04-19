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
FORCE_INLINE OGLRenderingDevice::OGLRenderingDevice(void)
{
	// Member data initialization
	this->m_maximum_delta_time = 0.0f;
	this->m_counter            = 0.0f;
	this->m_timer              = rNew ror::Timer;
	this->m_timer->start();
	this->m_maximum_delta_time = this->m_timer->tick();
	this->m_boids              = nullptr;        //rNew Game::CBoids;
	this->m_game               = nullptr;        //rNew Game::CGame;
	this->m_cbamanager         = ror::ccbamanager::get_instance();
	this->m_texture_manager    = ror::OGLTextureManager::get_instance();
	this->multiplier           = 20;
	this->m_fram_buffer        = nullptr;        //rNew CoreEngine::COGLFrameBuffer();
	this->m_registered         = false;
	//		m_TextureManager = CoreEngine::COGLTextureManager::GetInstance();
}

FORCE_INLINE void OGLRenderingDevice::pre_render()
{
}

FORCE_INLINE void OGLRenderingDevice::render()
{
	this->draw_view_port();
}

FORCE_INLINE void OGLRenderingDevice::post_render()
{
}

FORCE_INLINE bool OGLRenderingDevice::update(rUsLong a_delta_time)
{
	//m_Game->UpdateGame(m_DeltaTime);
	this->update_combined_matrix();
	return true;
}

FORCE_INLINE void OGLRenderingDevice::draw_view_port()
{
	this->m_counter++;
	glPushMatrix();
	float32_t temp_dt = 0.0f;

	if (this->m_game)
	{
		temp_dt = this->m_game->update_game();
		this->m_game->render_game();
	}

	if (this->m_boids)
	{
		this->m_boids->update_boids();
		this->m_boids->draw_boids();
	}

	glPopMatrix();

	this->m_maximum_delta_time += this->m_timer->tick();
	//tempDT = 1.0f / tempDT; // 0.051102364
	if (this->m_maximum_delta_time > 1.0f)        //(tempDT > m_MaximumDeltaTime)
	{
		this->m_maximum_delta_time = 0.0f;        //tempDT;
		this->m_fpscounter         = (long) this->m_counter;
		if (this->m_boids)
		{
			if (!ror::Utility::rand_int(-1, 1))
			{
				this->m_boids->m_target_point = Vector3f(
				    ror::Utility::rand(-10.0f, 10.0f),
				    ror::Utility::rand(0.0f, 10.0f),
				    ror::Utility::rand(-10.0f, 10.0f));
			}
		}
		this->m_counter = 0.0f;
	}
	static ror::Font *printer = FONTDEFAULT;
	printer->safe_print(0, 0, "FPS: %i", this->m_fpscounter);
	/*
		int view[4];
		glGetIntegerv(GL_VIEWPORT,view);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0,view[2],0,view[3],-100,100);
		//glViewport(0,0,500,500);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		//glDisable(GL_DEPTH_TEST);


		// Testing
		float32_t *Points = new float32_t[54 * 2];
		float32_t *TexCoords = new float32_t[54 * 2];
		// Lets Calculate the Size of texture and its midpoints
		CoreEngine::CVector2i TextureSize;
		TextureSize.x = 128;
		TextureSize.y = 128;

		CoreEngine::CVector2i TextureMid;
		TextureMid = TextureSize / 2;

		// Lets calculate the corners of the Triangle in Screen Coordinates
		CoreEngine::CVector2i BottomLeft,TopRight;
		CoreEngine::CVector2f TexCoordBottomLeft(0.0f,0.0f),TexCoordMid(0.5f,0.5f),TexCoordTopRight(1.0f,1.0f);

		CoreEngine::CVector2i m_Position(100,100);
		CoreEngine::CVector2i m_Bounds(200,300);


		BottomLeft.x = m_Position.x;
		BottomLeft.y = m_Position.y;

		TopRight.x = m_Position.x + m_Bounds.x;
		TopRight.y = m_Position.y + m_Bounds.y;


		// Create Triangle 1
		Points[0] = BottomLeft.x;
		Points[1] = BottomLeft.y;
		TexCoords[0] = TexCoordBottomLeft.x;
		TexCoords[1] = TexCoordBottomLeft.y;

		Points[2] = BottomLeft.x + TextureMid.x;
		Points[3] = BottomLeft.y;
		TexCoords[2] = TexCoordMid.x;
		TexCoords[3] = TexCoordBottomLeft.y;

		Points[4] = BottomLeft.x + TextureMid.x;
		Points[5] = BottomLeft.y + TextureMid.y;
		TexCoords[4] = TexCoordMid.x;
		TexCoords[5] = TexCoordMid.y;

		// Create Triangle 2
		Points[6] = BottomLeft.x;
		Points[7] = BottomLeft.y;
		TexCoords[6] = TexCoordBottomLeft.x;
		TexCoords[7] = TexCoordBottomLeft.y;

		Points[8] = BottomLeft.x + TextureMid.x;
		Points[9] = BottomLeft.y + TextureMid.y;
		TexCoords[8] = TexCoordMid.x;
		TexCoords[9] = TexCoordMid.y;

		Points[10] = BottomLeft.x;
		Points[11] = BottomLeft.y + TextureMid.y;
		TexCoords[10] = TexCoordBottomLeft.x;
		TexCoords[11] = TexCoordMid.y;

		// Create Triangle 3
		Points[12] = BottomLeft.x;
		Points[13] = BottomLeft.y + TextureMid.y;
		TexCoords[12] = TexCoordBottomLeft.x;
		TexCoords[13] = TexCoordMid.y;

		Points[14] = BottomLeft.x + TextureMid.x;
		Points[15] = BottomLeft.y + TextureMid.y;
		TexCoords[14] = TexCoordMid.x;
		TexCoords[15] = TexCoordMid.y;

		Points[16] = BottomLeft.x + TextureMid.x;
		Points[17] = TopRight.y   - TextureMid.y;
		TexCoords[16] = TexCoordMid.x;
		TexCoords[17] = TexCoordMid.y;

		// Create Triangle 4
		Points[18] = BottomLeft.x;
		Points[19] = BottomLeft.y + TextureMid.y;
		TexCoords[18] = TexCoordBottomLeft.x;
		TexCoords[19] = TexCoordMid.y;

		Points[20] = BottomLeft.x + TextureMid.x;
		Points[21] = TopRight.y   - TextureMid.y;
		TexCoords[20] = TexCoordMid.x;
		TexCoords[21] = TexCoordMid.y;

		Points[22] = BottomLeft.x;
		Points[23] = TopRight.y   - TextureMid.y;
		TexCoords[22] = TexCoordBottomLeft.x;
		TexCoords[23] = TexCoordMid.y;

		// Create Triangle 5
		Points[24] = BottomLeft.x;
		Points[25] = TopRight.y   - TextureMid.y;
		TexCoords[24] = TexCoordBottomLeft.x;
		TexCoords[25] = TexCoordMid.y;

		Points[26] = BottomLeft.x + TextureMid.x;
		Points[27] = TopRight.y   - TextureMid.y;
		TexCoords[26] = TexCoordMid.x;
		TexCoords[27] = TexCoordMid.y;

		Points[28] = BottomLeft.x + TextureMid.x;
		Points[29] = TopRight.y;
		TexCoords[28] = TexCoordMid.x;
		TexCoords[29] = TexCoordTopRight.y;

		// Create Triangle 6
		Points[30] = BottomLeft.x;
		Points[31] = TopRight.y   - TextureMid.y;
		TexCoords[30] = TexCoordBottomLeft.x;
		TexCoords[31] = TexCoordMid.y;

		Points[32] = BottomLeft.x + TextureMid.x;
		Points[33] = TopRight.y;
		TexCoords[32] = TexCoordMid.x;
		TexCoords[33] = TexCoordTopRight.y;

		Points[34] = BottomLeft.x;
		Points[35] = TopRight.y;
		TexCoords[34] = TexCoordBottomLeft.x;
		TexCoords[35] = TexCoordTopRight.y;


		// Third Row
		BottomLeft.x = TopRight.x - TextureMid.x;
		// Create Triangle 1
		Points[36] = BottomLeft.x;
		Points[37] = BottomLeft.y;
		TexCoords[36] = TexCoordMid.x;
		TexCoords[37] = TexCoordBottomLeft.y;

		Points[38] = BottomLeft.x + TextureMid.x;
		Points[39] = BottomLeft.y;
		TexCoords[38] = TexCoordTopRight.x;
		TexCoords[39] = TexCoordBottomLeft.y;

		Points[40] = BottomLeft.x + TextureMid.x;
		Points[41] = BottomLeft.y + TextureMid.y;
		TexCoords[40] = TexCoordTopRight.x;
		TexCoords[41] = TexCoordMid.y;

		// Create Triangle 2
		Points[42] = BottomLeft.x;
		Points[43] = BottomLeft.y;
		TexCoords[42] = TexCoordMid.x;
		TexCoords[43] = TexCoordBottomLeft.y;

		Points[44] = BottomLeft.x + TextureMid.x;
		Points[45] = BottomLeft.y + TextureMid.y;
		TexCoords[44] = TexCoordTopRight.x;
		TexCoords[45] = TexCoordMid.y;

		Points[46] = BottomLeft.x;
		Points[47] = BottomLeft.y + TextureMid.y;
		TexCoords[46] = TexCoordMid.x;
		TexCoords[47] = TexCoordMid.y;

		// Create Triangle 3
		Points[48] = BottomLeft.x;
		Points[49] = BottomLeft.y + TextureMid.y;
		TexCoords[48] = TexCoordMid.x;
		TexCoords[49] = TexCoordMid.y;

		Points[50] = BottomLeft.x + TextureMid.x;
		Points[51] = BottomLeft.y + TextureMid.y;
		TexCoords[50] = TexCoordTopRight.x;
		TexCoords[51] = TexCoordMid.y;

		Points[52] = BottomLeft.x + TextureMid.x;
		Points[53] = TopRight.y   - TextureMid.y;
		TexCoords[52] = TexCoordTopRight.x;
		TexCoords[53] = TexCoordMid.y;

		// Create Triangle 4
		Points[54] = BottomLeft.x;
		Points[55] = BottomLeft.y + TextureMid.y;
		TexCoords[54] = TexCoordMid.x;
		TexCoords[55] = TexCoordMid.y;

		Points[56] = BottomLeft.x + TextureMid.x;
		Points[57] = TopRight.y   - TextureMid.y;
		TexCoords[56] = TexCoordTopRight.x;
		TexCoords[57] = TexCoordMid.y;

		Points[58] = BottomLeft.x;
		Points[59] = TopRight.y   - TextureMid.y;
		TexCoords[58] = TexCoordMid.x;
		TexCoords[59] = TexCoordMid.y;

		// Create Triangle 5
		Points[60] = BottomLeft.x;
		Points[61] = TopRight.y   - TextureMid.y;
		TexCoords[60] = TexCoordMid.x;
		TexCoords[61] = TexCoordMid.y;

		Points[62] = BottomLeft.x + TextureMid.x;
		Points[63] = TopRight.y   - TextureMid.y;
		TexCoords[62] = TexCoordTopRight.x;
		TexCoords[63] = TexCoordMid.y;

		Points[64] = BottomLeft.x + TextureMid.x;
		Points[65] = TopRight.y;
		TexCoords[64] = TexCoordTopRight.x;
		TexCoords[65] = TexCoordTopRight.y;

		// Create Triangle 6
		Points[66] = BottomLeft.x;
		Points[67] = TopRight.y - TextureMid.y;
		TexCoords[66] = TexCoordMid.x;
		TexCoords[67] = TexCoordMid.y;

		Points[68] = BottomLeft.x + TextureMid.x;
		Points[69] = TopRight.y;
		TexCoords[68] = TexCoordTopRight.x;
		TexCoords[69] = TexCoordTopRight.y;

		Points[70] = BottomLeft.x;
		Points[71] = TopRight.y;
		TexCoords[70] = TexCoordMid.x;
		TexCoords[71] = TexCoordTopRight.y;


		// Second Row
		BottomLeft.x = float32_t(m_Position.x + TextureMid.x);
		TextureMid.x = ((int32_t)TopRight.x - TextureMid.x) - (int32_t)BottomLeft.x;
		// Create Triangle 1
		Points[72] = BottomLeft.x;
		Points[73] = BottomLeft.y;
		TexCoords[72] = TexCoordMid.x;
		TexCoords[73] = TexCoordBottomLeft.y;

		Points[74] = BottomLeft.x + TextureMid.x;
		Points[75] = BottomLeft.y;
		TexCoords[74] = TexCoordMid.x;
		TexCoords[75] = TexCoordBottomLeft.y;

		Points[76] = BottomLeft.x + TextureMid.x;
		Points[77] = BottomLeft.y + TextureMid.y;
		TexCoords[76] = TexCoordMid.x;
		TexCoords[77] = TexCoordMid.y;

		// Create Triangle 2
		Points[78] = BottomLeft.x;
		Points[79] = BottomLeft.y;
		TexCoords[78] = TexCoordMid.x;
		TexCoords[79] = TexCoordBottomLeft.y;

		Points[80] = BottomLeft.x + TextureMid.x;
		Points[81] = BottomLeft.y + TextureMid.y;
		TexCoords[80] = TexCoordMid.x;
		TexCoords[81] = TexCoordMid.y;

		Points[82] = BottomLeft.x;
		Points[83] = BottomLeft.y + TextureMid.y;
		TexCoords[82] = TexCoordMid.x;
		TexCoords[83] = TexCoordMid.y;

		// Create Triangle 3
		Points[84] = BottomLeft.x;
		Points[85] = BottomLeft.y + TextureMid.y;
		TexCoords[84] = TexCoordMid.x;
		TexCoords[85] = TexCoordMid.y;

		Points[86] = BottomLeft.x + TextureMid.x;
		Points[87] = BottomLeft.y + TextureMid.y;
		TexCoords[86] = TexCoordMid.x;
		TexCoords[87] = TexCoordMid.y;

		Points[88] = BottomLeft.x + TextureMid.x;
		Points[89] = TopRight.y   - TextureMid.y;
		TexCoords[88] = TexCoordMid.x;
		TexCoords[89] = TexCoordMid.y;

		// Create Triangle 4
		Points[90] = BottomLeft.x;
		Points[91] = BottomLeft.y + TextureMid.y;
		TexCoords[90] = TexCoordMid.x;
		TexCoords[91] = TexCoordMid.y;

		Points[92] = BottomLeft.x + TextureMid.x;
		Points[93] = TopRight.y   - TextureMid.y;
		TexCoords[92] = TexCoordMid.x;
		TexCoords[93] = TexCoordMid.y;

		Points[94] = BottomLeft.x;
		Points[95] = TopRight.y   - TextureMid.y;
		TexCoords[94] = TexCoordMid.x;
		TexCoords[95] = TexCoordMid.y;

		// Create Triangle 5
		Points[96] = BottomLeft.x;
		Points[97] = TopRight.y   - TextureMid.y;
		TexCoords[96] = TexCoordMid.x;
		TexCoords[97] = TexCoordMid.y;

		Points[98] = BottomLeft.x + TextureMid.x;
		Points[99] = TopRight.y   - TextureMid.y;
		TexCoords[98] = TexCoordMid.x;
		TexCoords[99] = TexCoordMid.y;

		Points[100] = BottomLeft.x + TextureMid.x;
		Points[101] = TopRight.y;
		TexCoords[100] = TexCoordMid.x;
		TexCoords[101] = TexCoordTopRight.y;

		// Create Triangle 6
		Points[102] = BottomLeft.x;
		Points[103] = TopRight.y - TextureMid.y;
		TexCoords[102] = TexCoordMid.x;
		TexCoords[103] = TexCoordMid.y;

		Points[104] = BottomLeft.x + TextureMid.x;
		Points[105] = TopRight.y;
		TexCoords[104] = TexCoordMid.x;
		TexCoords[105] = TexCoordTopRight.y;

		Points[106] = BottomLeft.x;
		Points[107] = TopRight.y;
		TexCoords[106] = TexCoordMid.x;
		TexCoords[107] = TexCoordTopRight.y;

		if (!this->m_Registered)
		{
			std::string FileName = ((std::string)WORKINGDIRECTORY) + "Assets/GUI/FrameTest.tga";
			//this->SetBoundTextureID(this->m_TextureManager->CreateTexture(FileName) );
			this->m_Texture = this->m_TextureManager->CreateTexture(FileName);
			this->m_Registered = true;
		}

		glEnable(GL_TEXTURE_2D);
		this->m_Texture->BindTexture();
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glBegin(GL_TRIANGLES);
		for (int i =0;i < (54 * 2) - 1;i+=2)
		{
			glTexCoord2f(TexCoords[i],TexCoords[i+1]);
			glVertex2f(Points[i],Points[i+1]);
		}
		glEnd();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		this->m_Texture->UnBindTexture();
		*/
}

FORCE_INLINE void OGLRenderingDevice::identity()
{
	//m_ModelViewMatrix->Identity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

FORCE_INLINE Vector3f OGLRenderingDevice::camera_position()
{
	Vector3f camera_position;

	this->m_buffer_matrix = this->m_model_view_matrix;
	this->m_buffer_matrix->inverse();

	camera_position.set(this->m_buffer_matrix->m_values[12], this->m_buffer_matrix->m_values[13], this->m_buffer_matrix->m_values[14]);
	return camera_position;
}

FORCE_INLINE Vector3f OGLRenderingDevice::world_coords(const Vector3f &a_screen_coords)
{
	Vector4f temp(a_screen_coords, 1.0f);
	int32_t  viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	*this->m_buffer_matrix = *this->m_projection_matrix;
	*this->m_buffer_matrix *= *this->m_model_view_matrix;
	this->m_buffer_matrix->inverse();

	temp.x = (a_screen_coords.x - viewport[0]) * 2.0f / viewport[2] - 1.0f;
	temp.y = (a_screen_coords.y - viewport[1]) * 2.0f / viewport[3] - 1.0f;
	temp.z = a_screen_coords.z * 2.0f - 1.0f;

	temp *= *this->m_buffer_matrix;
	//Check for division by zero and return an all zero tuple or the correct world
	//coordinates accordingly
	return !temp.w ? Vector3f() : Vector3f(temp.x / temp.w, temp.y / temp.w, temp.z / temp.w);
}

FORCE_INLINE Vector3f OGLRenderingDevice::screen_coords(const Vector3f &a_world_position)
{
	Vector4f temp;
	int32_t  viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);

	temp.set(a_world_position, 1.0f);
	temp *= *this->m_model_view_matrix;
	temp *= *this->m_projection_matrix;

	temp /= temp.w;

	temp.x = viewport[0] + float32_t(viewport[2]) * (temp.x + 1.0f) / 2.0f;
	temp.y = viewport[1] + float32_t(viewport[3]) * (temp.y + 1.0f) / 2.0f;
	temp.z = 0.5f * temp.z + 0.5f;

	return Vector3f(temp.x, temp.y, temp.z);
}

FORCE_INLINE std::string OGLRenderingDevice::parse_errors()
{
	rGLEnum     error = glGetError();
	std::string return_value;
	switch (error)
	{
		case GL_NO_ERROR:
			return_value = "NO ERROR";
			break;
		case GL_INVALID_ENUM:
			return_value = "INVALID ENUM";
			break;
		case GL_INVALID_VALUE:
			return_value = "INVALID VALUE";
			break;
		case GL_INVALID_OPERATION:
			return_value = "INVALID OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			return_value = "STACK OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			return_value = "STACK UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			return_value = "OUT OF MEMORY";
			break;
	}
	return return_value;
}

FORCE_INLINE void OGLRenderingDevice::setup_wireframe_rendering()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

FORCE_INLINE void OGLRenderingDevice::setup_shaded_rendering()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

FORCE_INLINE void OGLRenderingDevice::set_back_color(float32_t a_r, float32_t a_g, float32_t a_b, float32_t a_a)
{
	glClearColor(a_r, a_g, a_b, a_a);
}

FORCE_INLINE void OGLRenderingDevice::set_color(ror::Vector3f &a_color)
{
	glColor3fv(a_color.m_values);
}

FORCE_INLINE void OGLRenderingDevice::set_color(ror::Vector4f &a_color)
{
	glColor4fv(a_color.m_values);
}

FORCE_INLINE void OGLRenderingDevice::set_color(ror::Vector3i &a_color)
{
	glColor3iv(a_color.m_values);
	//glColor3ub(a_Color.r,a_Color.g,a_Color.b);
}

FORCE_INLINE void OGLRenderingDevice::set_color(float32_t a_r, float32_t a_g, float32_t a_b)
{
	glColor3f(a_r, a_g, a_b);
}

FORCE_INLINE void OGLRenderingDevice::set_color(int32_t a_r, int32_t a_g, int32_t a_b)
{
	glColor3i(a_r, a_g, a_b);
}

FORCE_INLINE void OGLRenderingDevice::restore_current_color()
{
	glColor4iv(m_current_color);
}

FORCE_INLINE void OGLRenderingDevice::save_current_color()
{
	glGetIntegerv(GL_CURRENT_COLOR, m_current_color);
}

FORCE_INLINE void OGLRenderingDevice::get_viewport(int32_t a_viewport[4])
{
	glGetIntegerv(GL_VIEWPORT, a_viewport);
}

FORCE_INLINE void OGLRenderingDevice::enable2dmode()
{
	static int32_t viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(viewport[0], viewport[2], viewport[1], viewport[3], -1, 1);
	glDisable(GL_DEPTH_TEST);
}

FORCE_INLINE void OGLRenderingDevice::disable2dmode()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

FORCE_INLINE void OGLRenderingDevice::save_render_state()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

FORCE_INLINE void OGLRenderingDevice::restore_render_state()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

FORCE_INLINE void OGLRenderingDevice::push_matrix()
{
	glPushMatrix();
}

FORCE_INLINE void OGLRenderingDevice::pop_matrix()
{
	glPopMatrix();
}

}        // namespace rendering
