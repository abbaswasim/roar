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

#pragma once

#include "memory_manager/rormemory_check.h"
#include "profiling/rorlog.h"
#include "profiling/rortimer.h"
#include "rorball.h"
#include "rormesh.h"

namespace game
{
class ROAR_ENGINE_ITEM Game
{
  public:
	Game(void);
	~Game(void);
	void      render_game();
	float32_t update_game();

  private:
	// Total time elapsed since the game started without reset or since reset
	float32_t m_time;
	// Timer member variable gives time in Processor ticks
	ror::Timer *m_timer;
	// Number of frames counter
	rLong m_frame_rate;
	// the only One ball which is rendered and simulated in the scene
	game::Ball *m_ball;
	// The only scene mesh which is rendered in the scene
	ror::Mesh *m_plane_mesh;
};
}        // namespace game
