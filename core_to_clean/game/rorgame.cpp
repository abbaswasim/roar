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

#include "rorgame.h"

namespace game
{
Game::Game(void)
{
	m_ball       = rNew       game::Ball;
	m_plane_mesh = rNew ror::Mesh;
	m_time       = 0;        // the time is in seconds
	m_timer      = rNew ror::Timer();
	m_timer->start();
	m_frame_rate = 0;
}

Game::~Game(void)
{
	rDELETE(m_ball);
	rDELETE(m_plane_mesh);
	rDELETE(m_timer);
}

void Game::render_game()
{
	// TODO: Ultimately all the assest and game logic will be rendered here
	m_ball->draw_ball();
}

float32_t Game::update_game()
{
	// TODO: Ultimately all the assets and game logic will be rendered here
	float32_t dt = m_timer->tick();
	m_ball->update_ball(dt);
	return dt;
}

}        // namespace game
