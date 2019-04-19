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
FORCE_INLINE Utility::Utility(void)
{
}

FORCE_INLINE Utility::~Utility(void)
{
}

FORCE_INLINE int32_t Utility::convert_string_to_int(char8_t *a_string)
{
	int32_t return_value_sign = 1;
	int32_t return_value      = 0;
	int32_t lenght_of_string  = sizeof(a_string);
	if (lenght_of_string > 0)        // which makes sure its not null
	{
		int32_t starting_index = 0;
		if (a_string[starting_index] == '-')
		{
			return_value_sign = -1;
			starting_index    = 1;
		}

		int32_t i;
		for (i = starting_index; i < lenght_of_string - 1; i++)
		{
			int32_t power_value = 1;
			int32_t j;
			for (j = 0; j < i; j++)
				power_value *= 10;

			return_value += power_value * (a_string[i] - 48);
		}
	}
	return return_value * return_value_sign;
}

FORCE_INLINE float32_t Utility::rand(float32_t n1, float32_t n2)
{
	return n1 + (::rand() * (n2 - n1)) / RAND_MAX;
}

FORCE_INLINE int32_t Utility::rand_int(int32_t n1, int32_t n2)
{
	return n1 + (::rand() * (n2 - n1)) / RAND_MAX;
}

FORCE_INLINE void Utility::draw_sphere(const float32_t a_x, const float32_t a_y, const float32_t a_z, const float32_t a_radius, const int32_t a_slices, const int32_t a_stacks)
{
	float32_t X, Y, Z;
	float32_t X1, Y1, Z1;

	float32_t M = (float32_t) a_slices;
	float32_t N = (float32_t) a_stacks;
	glPushMatrix();
	glTranslatef(a_x, a_y, a_z);
	for (int32_t i = 0; i <= a_slices + 1; i++)
	{
		glBegin(GL_LINE_STRIP);
		for (int32_t j = 1; j <= a_stacks + 1; j++)
		{
			X = a_radius * sin((i * PI) / (M + 1)) * cos((j * PI_TIMES2) / N);
			Y = a_radius * sin((i * PI) / (M + 1)) * sin((j * PI_TIMES2) / N);
			Z = -a_radius * cos((i * PI) / (M + 1));

			glVertex3f(X, Y, Z);
		}
		glEnd();
		glBegin(GL_LINES);
		for (int32_t j = 1; j <= a_stacks + 1; j++)
		{
			X = a_radius * sin((i * PI) / (M + 1)) * cos((j * PI_TIMES2) / N);
			Y = a_radius * sin((i * PI) / (M + 1)) * sin((j * PI_TIMES2) / N);
			Z = -a_radius * cos((i * PI) / (M + 1));

			X1 = a_radius * sin(((i + 1) * PI) / (M + 1)) * cos(((j) *PI_TIMES2) / N);
			Y1 = a_radius * sin(((i + 1) * PI) / (M + 1)) * sin(((j) *PI_TIMES2) / N);
			Z1 = -a_radius * cos(((i + 1) * PI) / (M + 1));

			glVertex3f(X, Y, Z);
			glVertex3f(X1, Y1, Z1);
		}
		glEnd();
	}
	glPopMatrix();

	//		glPushMatrix();
	//		glTranslatef(a_X,a_Y,a_Z);
	//		gluSphere(m_Quadric,a_Radius,a_Slices,a_Stacks);
	//		glPopMatrix();
}

FORCE_INLINE EAnimationClips Utility::get_animation_clip(std::string a_clip_name)
{
	if ("idle" == a_clip_name)
	{
		return rIDLE;
	}
	else if ("run" == a_clip_name)
	{
		return rRUN;
	}
	else if ("walk" == a_clip_name)
	{
		return rWALK;
	}
	else if ("jump" == a_clip_name)
	{
		return rJUMP;
	}
	else if ("hurt" == a_clip_name)
	{
		return rHURT;
	}
	else if ("dead" == a_clip_name)
	{
		return rDEAD;
	}
	else if ("crawl" == a_clip_name)
	{
		return rCRAWL;
	}
	else if ("dying" == a_clip_name)
	{
		return rDYING;
	}
	else if ("crouch" == a_clip_name)
	{
		return rCROUCH;
	}
	else if ("defense" == a_clip_name)
	{
		return rDEFENSE;
	}

	return rNONE;
	//		rIDLE,
	//		rRUN,
	//		rWALK,
	//		rJUMP,
	//		rHURT,
	//		rDEAD,
	//		rCRAWL,
	//		rDYING,
	//		rCROUCH,
	//		rDEFENSE,
	//		rANIMATION_CLIPS_MAX
	//
}

}        // namespace ror
