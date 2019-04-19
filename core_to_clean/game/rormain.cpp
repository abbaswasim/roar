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

#include "IO/rorkeyboard.h"
#include "IO/rormouse.h"
#include "profiling/rorlog.h"
#include "rorworld.h"
//#include "Scenegraph/RORSGNode.h"
//#include "RORQueue.h"
//#include "ROROGLShaderProgram.h"

/*/-----------------------------------------------------------------------//
// Profiling includes
#include "Math/RORMatrix4.h"
#include "RORTimer.h"
//-----------------------------------------------------------------------/*/

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
//#pragma comment( lib, "glaux.lib" )	// No more Glaux supported , Glaux Sux

#ifndef CDS_FULLSCREEN
#	define CDS_FULLSCREEN 4
#endif

// Program Entry (WinMain)
#ifdef _WIN32
int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
#else
int32_t main(int argc, char **argv, char **tmp)
#endif
{
	// Just Enable this line to enable _Crt Memory Manager
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	ror::World *ApplicationInstance = rNew ror::World(800, 600, 32);

	/*/-----------------------------------------------------------------------//
	// Profiling OpenGL LoadIdentity and creating identity by hand
	ror::CTimer IdentityCheck;
	IdentityCheck.Start();

	float32_t temT = 10.0f;
	for (long i=0;i < 200000;i++)
	{
		Matrix4x4* mNew = new Matrix4x4();
		//mNew->MakeIdentity();
		temT += sin(mNew->_13 * mNew->_14);
	}
	float32_t tempT = 0.0f;
	tempT += temT + 20.0f;

	float32_t Seconds = IdentityCheck.Tick();

	tempT += Seconds;
	//-----------------------------------------------------------------------/*/
	int hInstance;
	ApplicationInstance->main_loop(hInstance);
	rDELETE(ApplicationInstance);

	//MESSAGEBOX("TEST");
#ifdef USEROARMEMORYMANAGER
#	ifdef _DEBUG
	ror::CMemoryManager::DumpPotentialLeaks();
#	endif
#endif
	return 0;
}

// #ifdef _DEBUG
//	_CrtMemState initMemoryState;
//	_CrtMemCheckpoint(&initMemoryState);
//	//_CrtSetBreakAlloc(143);
// #endif

// #ifdef _DEBUG
//	_CrtMemState finalMemoryState;
//	_CrtMemCheckpoint(&finalMemoryState);
//
//	_CrtMemState diffMemoryState;
//	if(_CrtMemDifference(&diffMemoryState, &initMemoryState, &finalMemoryState))
//	{
//		_CrtMemDumpStatistics(&diffMemoryState);
//		_CrtDumpMemoryLeaks();
//		//DebugBreak();
//	}
// #endif
