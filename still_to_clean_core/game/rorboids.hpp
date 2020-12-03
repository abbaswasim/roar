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

#include "math/rormatrix4.h"
#include "math/rorvector3.h"
#include "memory_manager/rormemory_check.h"
#include "models/rorcba_manager.h"

using namespace ror;
namespace game
{
#define NUMBEROFBOIDS 200

class ROAR_ENGINE_ITEM Boid
{
  public:
	Vector3f m_position;
	Vector3f m_velocity;
	Vector3f m_color;
	bool     m_perching;
	int32_t  m_perching_timer;

	CCBA *m_model;

	Boid();
	Boid(float32_t a_x, float32_t a_y, float32_t a_z);

	void draw();
	void update();
	void draw_boid();

  protected:
  private:
};

class ROAR_ENGINE_ITEM Boids
{
  public:
	Boid *   m_boids;
	Vector3f m_target_point;        //(-5.0f,4.0f,0.0f)
	bool     m_first_update;

	Boids();
	~Boids();

	void     draw_boids();
	void     update_boids();
	Vector3f boids_tendency_to_center_of_mass(int32_t a_index);
	Vector3f boids_keep_small_distance(int32_t a_index);
	Vector3f boids_tendency_to_match_velocity(int32_t a_index);
	Vector3f boids_tendency_to_particular_point(int32_t a_index);
	Vector3f boids_limit_position(int32_t a_index);        // Limit the position to not go outside of screen
	void     boids_limit_velocity(int32_t a_index);        // Limit the Velocity to not go very fast
  protected:
  private:
};
}        // namespace game
