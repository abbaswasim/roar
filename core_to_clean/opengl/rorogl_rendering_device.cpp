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

#include "rorogl_rendering_device.h"
#include "profiling/rorlog.h"

namespace rendering
{
Vector2i g_visible;
int32_t  OGLRenderingDevice::m_current_color[4];

OGLRenderingDevice::~OGLRenderingDevice(void)
{
	rDELETE(this->m_game);
	rDELETE(this->m_timer);
	rDELETE(this->m_boids);
	rDELETE(this->m_fram_buffer);
}

bool OGLRenderingDevice::prepair()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);        // set the drawing color
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//		glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	//		glEnable(GL_LIGHT0);
	//		glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHTING);

	//glEnable( GL_CULL_FACE );
	//glCullFace( GL_BACK );

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);        // Enable Alpha Blending (disable alpha testing)
	glEnable(GL_BLEND);
	//		glAlphaFunc(GL_GREATER,0.1f);								// Set Alpha Testing     (disable blending)
	//		glEnable(GL_ALPHA_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonOffset(1.1f, 4.0f);

	// Use this to enable specular highlights after texturing
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	//		glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	//		glEnable(GL_COLOR_MATERIAL);

	//		float32_t noAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	//		float32_t whiteDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	//		float32_t redSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	//		float32_t position1[] = {0.0f, 0.0f, 0.0f, 1.0f};
	//		float32_t position2[] = {0.0f, 5.0f, 20.0f, 0.0f};

	//		glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuse);
	//		glLightfv(GL_LIGHT0, GL_AMBIENT, noAmbient);
	//		glLightfv(GL_LIGHT0, GL_SPECULAR, redSpecular);
	//
	//		glLightfv(GL_LIGHT1, GL_DIFFUSE, whiteDiffuse);
	//		glLightfv(GL_LIGHT1, GL_AMBIENT, noAmbient);
	//		glLightfv(GL_LIGHT1, GL_SPECULAR, redSpecular);
	//
	//
	//		glLightfv(GL_LIGHT0, GL_POSITION, position1);
	//		glLightfv(GL_LIGHT0, GL_POSITION, position2);

	//		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteDiffuse);
	//		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, noAmbient);
	//		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redSpecular);
	//		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, whiteDiffuse);
	//		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1.0f);

	//////////////////////////////////////////////////////////////////////////
	//		float32_t lposition[] = { 1.0, 1.0, 1.0, 1.0 };
	//		float32_t lambient[] = {1.0f,1.0f,1.0f};
	//		float32_t ldiffuse[] = {1.0f,1.0f,1.0f};
	//		float32_t lspecular[] = {1.0f,1.0f,1.0f};
	//		float32_t lshine[] = {25.0f};
	//
	//		glLightfv(GL_LIGHT0,GL_SPECULAR,lspecular);
	//		glLightfv(GL_LIGHT0,GL_AMBIENT,lambient);
	//		glLightfv(GL_LIGHT0,GL_DIFFUSE,ldiffuse);
	//		glLightfv(GL_LIGHT0,GL_SHININESS,lshine);
	//		glLightfv(GL_LIGHT0, GL_POSITION, lposition);

	//		glGenTextures( 1, &m_iTexID );
	//
	//		glBindTexture( GL_TEXTURE_2D, m_iTexID );
	//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	//		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	//		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//
	//		CoreEngine::COGLFrameBuffer *NewFrameBuffer = rNew CoreEngine::COGLFrameBuffer();
	//		CoreEngine::COGLRenderBuffer *NewRenderBuffer = rNew CoreEngine::COGLRenderBuffer();
	//
	//		NewRenderBuffer->LoadRenderBuffer(CoreEngine::COGLRenderBuffer::rGL_DEPTH_COMPONENT,512,512);
	//
	//		GLenum t1 = glGetError();
	//
	//		if (GL_NO_ERROR != t1)
	//		{
	//			float32_t a = 3;
	//			uint32_t aa = a * 23 ;
	//		}
	//
	//		NewFrameBuffer->LoadFrameBuffer();
	//		t1 = glGetError();
	//
	//		if (GL_NO_ERROR != t1)
	//		{
	//			float32_t a = 3;
	//			uint32_t aa = a * 23 ;
	//		}
	//		CoreEngine::CTextureObject temp;
	//		temp.SetTextureID(m_iTexID);
	//		temp.SetTextureTarget(CoreEngine::CTextureObject::rGL_TEXTURE_2D);
	//		NewFrameBuffer->AttachTextureImage(&temp);
	//		t1 = glGetError();
	//
	//		if (GL_NO_ERROR != t1)
	//		{
	//			float32_t a = 3;
	//			uint32_t aa = a * 23 ;
	//		}
	//		NewFrameBuffer->AttachRenderBuffer(NewRenderBuffer, CoreEngine::COGLFrameBuffer::rGL_ATTACH_DEPTH);
	//
	//		t1 = glGetError();
	//
	//		if (GL_NO_ERROR != t1)
	//		{
	//			float32_t a = 3;
	//			uint32_t aa = a * 23 ;
	//		}
	//
	//		rDELETE (NewRenderBuffer);
	//		//this->m_DynamicTextures->push_back(NewFrameBuffer);
	//		m_FramBuffer = NewFrameBuffer;
	//
	//		GLenum t3 = glGetError();
	//
	//		if (GL_NO_ERROR != t3)
	//		{
	//			float32_t a = 3;
	//			uint32_t aa = a * 23 ;
	//		}

	//		//glEnable(GL_NORMALIZE);
	//
	//		//glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	//		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//
	//		glGenFramebuffersEXT( 1, &m_iFBCol );
	//		glGenRenderbuffersEXT( 1, &m_iFBDepth );
	//		glGenTextures( 1, &m_iTexID );
	//
	//		glBindTexture( GL_TEXTURE_2D, m_iTexID );
	//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	//		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	//		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//
	//		// bind the FB to a texture
	//		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_iFBCol );
	//		glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_iTexID, 0 );
	//
	//		// attach a depth buffer to the FB
	//		glBindRenderbuffer( GL_RENDERBUFFER_EXT, m_iFBDepth );
	//		glRenderbufferStorage( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, 512, 512);
	//		glFramebufferRenderbuffer( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER_EXT, m_iFBDepth );
	//		glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);
	//		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//		t3 = glGetError();
	//
	//		if (GL_NO_ERROR != t3)
	//		{
	//			float32_t a = 3;
	//			uint32_t aa = a * 23 ;
	//		}

	LOG->message("\nInitialized OpenGL Renderer");
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	return true;        // Return TRUE (Initialization Successful)
}

bool OGLRenderingDevice::shutdown()
{
	return true;
}

bool OGLRenderingDevice::reset()
{
	return true;
}

}        // namespace rendering
