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
FORCE_INLINE World::World(void)
{
	this->m_rendering_device = rNew RENDERER();
	//this->m_ActiveCamera		= rNew CoreEngine::CMayaCamera();
	//this->m_ActiveCamera		= rNew CoreEngine::CFirstPersonCamera();
	this->m_active_camera  = nullptr;
	this->m_levels         = new std::vector<Level *>();
	this->m_player         = nullptr;
	this->m_event_listener = this;
	this->m_guimanager     = nullptr;
	this->m_draw_shadows   = false;
	this->m_shadow_mapping = nullptr;
	this->m_shadow_light   = nullptr;
}

FORCE_INLINE World::World(int32_t a_width, int32_t a_height, int32_t a_bpp) :
    Window32(a_width, a_height, a_bpp)
{
	this->m_rendering_device = rNew RENDERER();
	//this->m_ActiveCamera		= rNew CoreEngine::CMayaCamera();
	//this->m_ActiveCamera		= rNew CoreEngine::CFirstPersonCamera();
	this->m_active_camera       = nullptr;
	this->m_levels              = new std::vector<Level *>();
	this->m_player              = nullptr;
	this->m_grid                = true;
	this->m_wireframe_rendering = false;
	this->m_menu                = false;
	this->m_pause               = false;
	this->m_render_gui          = false;
	this->m_event_listener      = this;
	this->m_guimanager          = nullptr;
	this->m_draw_shadows        = true;
	this->m_shadow_mapping      = nullptr;
	this->m_shadow_light        = nullptr;
}

//	FORCE_INLINE void CWorld::MouseMove( int32_t a_X,int32_t a_Y )
//	{
//		if (m_Mouse->IsAnyButtonDown())
//		{
//			this->m_ActiveCamera->Update(a_X, a_Y);
//		}
//
// //		// Testing ::
// //		if (!this->m_Menu)
// //		{
// //			if (a_Y < 25)
// //			{
// //				SetMenu(this->m_hWnd,LoadMenu(this->m_hInstance, MAKEINTRESOURCE(IDR_MENU)));
// //				this->m_Menu = true;
// //			}
// //		}
// //		else if (a_Y > 50)
// //		{
// //			SetMenu(this->m_hWnd,nullptr);
// //			this->m_Menu = false;
// //		}
//	}

//	FORCE_INLINE void CWorld::CheckMouseInput()
//	{
//		//float32_t Angle = 15.0f;
//		//CVector3f yAxis(1.0f,0.0f,0.0f);
//	}

//	FORCE_INLINE void CWorld::CheckKeyboardInput()
//	{
//		//if (this->m_Keyboard->IsKeyDown('G') || this->m_Keyboard->IsKeyDown('g') )
//		if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_G))
//		{
//			this->m_Grid = !this->m_Grid;
//		}
//		if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_T))
//		{
//			this->m_RenderGUI = !this->m_RenderGUI;
//		}
//		if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_W))
//		{
//			this->m_WireframeRendering = !this->m_WireframeRendering;
//
//			if (this->m_WireframeRendering)
//				this->m_RenderingDevice->SetupWireframeRendering();
//			else
//				this->m_RenderingDevice->SetupShadedRendering();
//		}
//		if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_P))
//		{
//			this->m_Pause = !this->m_Pause;
//		}
//		if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_E))
//		{
//			if (CoreEngine::CUtility::RandInt(0,5))
//				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//			else
//				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//		}
//
//		if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_M))
//		{
//			this->m_Menu = !this->m_Menu;
//			if (!this->m_Menu)
//				SetMenu(this->m_hWnd,nullptr);
//			else
//			{
//				HMENU Menu;
//				Menu = LoadMenu(this->m_hInstance, MAKEINTRESOURCE(IDR_MENU));
//				SetMenu(this->m_hWnd,Menu);
//			}
//		}
//
//		if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_R))
//		{
//			//this->m_Player = (*m_Levels)[0]->GetSceneRoot();
//			(*this->m_Levels)[0]->IterateCamera(&this->m_ActiveCamera);
//		}
//
//		if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_C))
//		{
//			this->m_Player = (*m_Levels)[0]->GetEntity();
//			this->m_Player->SetInMotion(true);
//			this->m_Player->SetUseGravity(true);
//		}
//
// //		if (nullptr != this->m_Player)
// //		{
// //			if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboard::rKEY_UP))
// //				this->m_Player->GetLocalTransformation()->m_Values[14] -= 0.5;
// //			if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboard::rKEY_DOWN))
// //				this->m_Player->GetLocalTransformation()->m_Values[14] += 0.5;
// //			if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboard::rKEY_LEFT))
// //				this->m_Player->GetLocalTransformation()->m_Values[12] -= 0.5;
// //			if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboard::rKEY_RIGHT))
// //				this->m_Player->GetLocalTransformation()->m_Values[12] += 0.5;
// //		}
//
//		float32_t Delta = 2.35f;
//		int32_t a;
//
//		static CoreEngine::CFirstPersonCamera *ActiveCamera;
//		ActiveCamera = dynamic_cast<CoreEngine::CFirstPersonCamera*> (this->m_ActiveCamera); // Hack :
//
//		if (nullptr != ActiveCamera)
//		{
//			if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_UP))
//			{
//				ActiveCamera->Move(Delta);
//				ActiveCamera->Update(a,a);
//			}
//			if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_DOWN))
//			{
//				ActiveCamera->Move(Delta*=-1);
//				ActiveCamera->Update(a,a);
//			}
//			if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_RIGHT))
//			{
//				ActiveCamera->Strafe(Delta);
//				ActiveCamera->Update(a,a);
//			}
//			if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_LEFT))
//			{
//				ActiveCamera->Strafe(Delta*=-1);
//				ActiveCamera->Update(a,a);
//			}
//			if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_HOME))
//			{
//				ActiveCamera->Elevate(Delta);
//				ActiveCamera->Update(a,a);
//			}
//			if (this->m_Keyboard->IsKeyDown(CoreEngine::CKeyboardEvent::rKEY_END))
//			{
//				ActiveCamera->Elevate(Delta*=-1);
//				ActiveCamera->Update(a,a);
//			}
//		}
//	}

FORCE_INLINE void World::set_projection()
{
	glViewport(0, 0, (GLsizei) this->m_window_width, (GLsizei) this->m_window_height);

	// Reset Projection matrix stack
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (nullptr != this->m_active_camera)
	{
		/*gluPerspective(this->m_ActiveCamera->GetFOV() + this->m_ZoomFactor,
				float32_t(this->m_WindowWidth)/float32_t(this->m_WindowHeight),
				this->m_ActiveCamera->GetNear(), this->m_ActiveCamera->GetFar());*/
		// No more Glaux supported

		//static CoreEngine::CMatrix4f m_ProjectionMatrix;
		//m_ProjectionMatrix.Identity();
		this->m_projection_matrix.set_perspective(this->m_active_camera->get_fov() + this->m_zoom_factor,
		                                          float32_t(this->m_window_width) / float32_t(this->m_window_height),
		                                          this->m_active_camera->get_near(), this->m_active_camera->get_far());
		glLoadMatrixf(this->m_projection_matrix);
	}

	//Set the Model View Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

FORCE_INLINE void World::render()
{
	// Hack  ::
	//this->m_ShaderProgram->Enable();
	//this->m_ShaderProgram->SendUniform("shadowMap",(int32_t)this->m_ShadowMapping->GetShadowMapID());

	this->m_rendering_device->identity();
	this->m_active_camera->look_at();
	this->m_active_camera->update_viewport();
	this->m_active_camera->update_frustrum(this->m_rendering_device->get_combined_matrix());
	static const uint32_t current_level = 0;
	// Hack
	/*
			m_ActiveCamera->DrawLocalAxis();
			m_ActiveCamera->DrawBoundingSphere();
		*/

	if (this->m_draw_shadows)
	{
		// Pass 1
		this->m_shadow_mapping->setup_pass1();
		ror::Entities::set_current_render_pass(ror::Entities::rSHADOWING_PASS);
		//glPolygonOffset(1.1f, 4.0f);

		//glEnable(GL_LIGHTING);
		//		static uint32_t NoOfLevels;
		//		NoOfLevels = (uint32_t)this->m_Levels->size();
		//		for (uint32_t LevelIndex = 0; LevelIndex < NoOfLevels; LevelIndex++)
		{
			RENDERER::push_matrix();
			(*this->m_levels)[current_level]->render(this->m_rendering_device, this->m_active_camera, false, Level::rRENDER_SHADOW);        // Testing :: Render the first and Only Level
			RENDERER::pop_matrix();
		}

		// Pass 2
		this->m_shadow_mapping->setup_pass2();
		//CoreEngine::CEntities::SetCurrentRenderPass(CoreEngine::CEntities::rSHADOWING_PASS_TWO);
		//glPolygonOffset(1.1f, -4.0f);
		this->m_rendering_device->identity();
		this->m_active_camera->look_at();
		this->m_active_camera->update_viewport();
		this->m_active_camera->update_frustrum(this->m_rendering_device->get_combined_matrix());

		glLightfv(GL_LIGHT0, GL_POSITION, ror::Vector4f(-50.0f, 50.0f, 50.0f, 1.0f));        //-10,10,10 Hack :: to be fixed soon
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(this->m_projection_matrix);
		glMatrixMode(GL_MODELVIEW);

		{
			RENDERER::push_matrix();
			(*this->m_levels)[current_level]->render(this->m_rendering_device, this->m_active_camera, true, Level::rRENDER_SHADOW);        // Testing :: Render the first and Only Level
			RENDERER::pop_matrix();
		}
		// Pass 3
		this->m_shadow_mapping->setup_pass3(this->m_projection_matrix);
		{
			RENDERER::push_matrix();
			(*this->m_levels)[current_level]->render(this->m_rendering_device, this->m_active_camera, false, Level::rRENDER_SHADOW);        // Testing :: Render the first and Only Level
			RENDERER::pop_matrix();
		}
		this->m_shadow_mapping->end();

		// Draw the Clouds without any shadows or lights
		ror::Entities::set_current_render_pass(ror::Entities::rNON_SHADOWING_PASS);
		{
			RENDERER::push_matrix();
			(*this->m_levels)[current_level]->render(this->m_rendering_device, this->m_active_camera, false, Level::rRENDER_FULL);        // Testing :: Render the first and Only Level
			RENDERER::pop_matrix();
		}
	}
	else
	{
		ror::Entities::set_current_render_pass(ror::Entities::rRENDERING_PASS);
		//glEnable(GL_LIGHTING);
		//			static uint32_t NoOfLevels;
		//			NoOfLevels = (uint32_t)this->m_Levels->size();
		//			for (uint32_t LevelIndex = 0; LevelIndex < NoOfLevels; LevelIndex++)
		{
			RENDERER::push_matrix();
			(*this->m_levels)[current_level]->render(this->m_rendering_device, this->m_active_camera, true, Level::rRENDER_FULL);        // Testing :: Render the first and Only Level
			RENDERER::pop_matrix();
		}
	}

	//glDisable(GL_LIGHTING);

	// Hack  ::
	//this->m_ShaderProgram->Disable();

	this->m_rendering_device->render();

	//glEnable(GL_TEXTURE_2D);
	if (this->m_render_gui)
	{
		glDisable(GL_LIGHTING);
		RENDERER::enable2dmode();
		this->m_guimanager->render();
		RENDERER::disable2dmode();
		glEnable(GL_LIGHTING);
	}

	if (this->m_grid)
		this->draw_grid();

	glColor3f(1.0f, 1.0f, 1.0f);

	GLenum t3 = glGetError();
	if (GL_NO_ERROR != t3)
	{
		float32_t a = 3;
	}
}

FORCE_INLINE void World::update(rUsLong a_delta_time)
{
	if (!this->m_pause)
	{
		this->m_rendering_device->update(a_delta_time);
		(*this->m_levels)[0]->update(a_delta_time);        // Hack :
	}
}

FORCE_INLINE void World::draw_grid()
{
	glBegin(GL_LINES);
	float32_t size = 10.0f;
	for (float32_t i = -size; i <= size; i++)
	{
		if (!i)
			glColor3f(0.2f, 0.2f, 0.2f);
		else
			glColor3f(0.9f, 0.9f, 0.9f);

		glVertex3f(i, 0.0f, size);
		glVertex3f(i, 0.0f, -size);

		glVertex3f(size, 0.0f, i);
		glVertex3f(-size, 0.0f, i);
	}
	glEnd();
}

FORCE_INLINE void World::mouse_moved(ror::MouseEvent &a_event)
{
	if (this->m_mouse->is_any_button_down())
	{
		this->m_active_camera->update(a_event.m_x, a_event.m_y);
	}
	if (nullptr != this->m_guimanager)
		this->m_guimanager->check_for_mouse_events(a_event);
}

FORCE_INLINE void World::mouse_leaved(ror::MouseEvent &a_event)
{
	if (nullptr != this->m_guimanager)
		this->m_guimanager->check_for_mouse_events(a_event);
}

FORCE_INLINE void World::mouse_pressed(ror::MouseEvent &a_event)
{
	if (nullptr != this->m_guimanager)
		this->m_guimanager->check_for_mouse_events(a_event);
}

FORCE_INLINE void World::mouse_released(ror::MouseEvent &a_event)
{
	if (nullptr != this->m_guimanager)
		this->m_guimanager->check_for_mouse_events(a_event);
}

FORCE_INLINE void World::mouse_scrolled(ror::MouseEvent &a_event)
{
	this->mouse_wheel(a_event.m_scroll);
	//		if (CMouseEvent::rUP_MOVEMENT == a_Event.m_Scroll)
	//		{
	//				if (this->m_ZoomFactor > -30)
	//				{
	//					this->m_ZoomFactor -= 5;
	//				}
	//		}
	//		if (CMouseEvent::rDOWN_MOVEMENT == a_Event.m_Scroll)
	//		{
	//			if (this->m_ZoomFactor < 125)
	//			{
	//				this->m_ZoomFactor += 5;
	//			}
	//		}
	//		this->Reshape(this->m_WindowWidth,this->m_WindowHeight); // In this case this is safe to just use the Window dimentions
	if (nullptr != this->m_guimanager)
		this->m_guimanager->check_for_mouse_events(a_event);
}

FORCE_INLINE void World::mouse_double_clicked(ror::MouseEvent &a_event)
{
	if (nullptr != this->m_guimanager)
		this->m_guimanager->check_for_mouse_events(a_event);
}

FORCE_INLINE void World::key_pressed(ror::KeyboardEvent &a_event)
{
	if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_G))
	{
		this->m_grid = !this->m_grid;
	}
	if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_T))
	{
		this->m_render_gui = !this->m_render_gui;
	}
	if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_W))
	{
		this->m_wireframe_rendering = !this->m_wireframe_rendering;

		if (this->m_wireframe_rendering)
			this->m_rendering_device->setup_wireframe_rendering();
		else
			this->m_rendering_device->setup_shaded_rendering();
	}
	if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_P))
	{
		this->m_pause = !this->m_pause;
	}
	if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_E))
	{
		if (ror::Utility::rand_int(0, 5))
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		else
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_M))
	{
		this->m_menu = !this->m_menu;
		//			if (!this->m_Menu)
		//				SetMenu(this->m_hWnd,nullptr);
		//			else
		//			{
		//				HMENU Menu;
		//				Menu = LoadMenu(this->m_hInstance, MAKEINTRESOURCE(IDR_MENU));
		//				SetMenu(this->m_hWnd,Menu);
		//			}
	}

	if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_R))
	{
		//this->m_Player = (*m_Levels)[0]->GetSceneRoot();
		(*this->m_levels)[0]->iterate_camera(&this->m_active_camera);
	}

	if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_C))
	{
		this->m_player = (*m_levels)[0]->get_entity();
		this->m_player->set_in_motion(true);
		this->m_player->set_use_gravity(true);
	}

	if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_S))
	{
		this->m_draw_shadows = !this->m_draw_shadows;
	}

	float32_t delta = 2.35f;
	int32_t   a;

	static ror::FirstPersonCamera *active_camera;
	active_camera = dynamic_cast<ror::FirstPersonCamera *>(this->m_active_camera);        // Hack :

	if (nullptr != active_camera)
	{
		if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_UP))
		{
			active_camera->move(delta);
			active_camera->update(a, a);
		}
		if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_DOWN))
		{
			active_camera->move(delta *= -1);
			active_camera->update(a, a);
		}
		if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_RIGHT))
		{
			active_camera->strafe(delta);
			active_camera->update(a, a);
		}
		if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_LEFT))
		{
			active_camera->strafe(delta *= -1);
			active_camera->update(a, a);
		}
		if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_HOME))
		{
			active_camera->elevate(delta);
			active_camera->update(a, a);
		}
		if (this->m_keyboard->is_key_down(ror::KeyboardEvent::rKEY_END))
		{
			active_camera->elevate(delta *= -1);
			active_camera->update(a, a);
		}
	}
}

FORCE_INLINE void World::key_released(ror::KeyboardEvent &a_event)
{}

FORCE_INLINE void World::window_closing(ror::WindowEvent &a_event)
{
	this->close();
}

FORCE_INLINE void World::window_size_changed(ror::WindowEvent &a_event)
{
	this->size(a_event.get_wparam(), a_event.get_lparam());
	if (nullptr != this->m_guimanager)
		this->m_guimanager->check_for_window_events(a_event);
}

FORCE_INLINE void World::guievent_triggered(ror::cguievent &a_event)
{
}

FORCE_INLINE bool World::draw_shadows() const
{
	return this->m_draw_shadows;
}

FORCE_INLINE void World::draw_shadows(bool a_draw_shadows)
{
	this->m_draw_shadows = a_draw_shadows;
}

FORCE_INLINE void World::set_shadow_light(ror::Light *a_shadow_light)
{
	this->m_shadow_light = a_shadow_light;
}

FORCE_INLINE Light *World::get_shadow_light() const
{
	return this->m_shadow_light;
}

}        // namespace ror
