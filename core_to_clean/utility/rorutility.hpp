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

#include "foundation/rormacros.h"
//#include INCLUDERENDERER

namespace ror
{
class ROAR_ENGINE_ITEM Utility
{
  public:
	FORCE_INLINE Utility(void);
	FORCE_INLINE ~Utility(void);

	FORCE_INLINE static float32_t       rand(float32_t n1, float32_t n2);
	FORCE_INLINE static int32_t         rand_int(int32_t n1, int32_t n2);
	FORCE_INLINE static int32_t         convert_string_to_int(char8_t *a_string);
	FORCE_INLINE static void            draw_sphere(const float32_t a_x, const float32_t a_y, const float32_t a_z, const float32_t a_radius, const int32_t a_slices, const int32_t a_stacks);
	FORCE_INLINE static EAnimationClips get_animation_clip(std::string a_clip_name);

  private:
	//static GLUquadric				*m_Quadric;		// No more Glaux Support
};
}        // namespace ror

#include "rorutility.hh"
