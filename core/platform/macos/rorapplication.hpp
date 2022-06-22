// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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

#include "platform/rorapplication.hpp"

namespace ror
{

class ROAR_ENGINE_ITEM MacOSApp final : public Application<MacOSApp>
{
  public:
	FORCE_INLINE           MacOSApp()                            = default;         //! Default constructor
	FORCE_INLINE           MacOSApp(const MacOSApp &a_other)     = default;         //! Copy constructor
	FORCE_INLINE           MacOSApp(MacOSApp &&a_other) noexcept = default;         //! Move constructor
	FORCE_INLINE MacOSApp &operator=(const MacOSApp &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE MacOSApp &operator=(MacOSApp &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~MacOSApp() noexcept override                    = default;        //! Destructor

	declare_translation_unit_vtable();

	void init()
	{
		if (!glfwInit())
		{
			printf("GLFW Init failed, check if you have a working display set!\n");
			exit(EXIT_FAILURE);
		}

		glfwWindowHint(GLFW_DEPTH_BITS, 16);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		this->m_window = glfwCreateWindow(win_width, win_height, "Roar Editor...", nullptr, nullptr);

		if (!this->m_window)
		{
			glfwTerminate();
			printf("GLFW Windows can't be created\n");
			exit(EXIT_FAILURE);
		}

		glfwSetErrorCallback(glfw_error_callback);

		glfwSetKeyCallback(this->m_window, glfw_key);
		glfwSetWindowSizeCallback(this->m_window, glfw_resize);

		// Lets use this as a user pointer in glfw
		glfwSetWindowUserPointer(this->m_window, this);
	}

	void run()
	{
		this->init();
		this->loop();
		this->shutdown();
	}

	void loop()
	{
		while (!glfwWindowShouldClose(this->m_window))
		{
			glfwPollEvents();
			// ror::glfw_camera_update(vp, m, camera_position);
		}
	}

	void update()
	{}

	void animate()
	{}

	void resize(int a_width, int a_height)
	{
		(void) a_width;
		(void) a_height;
	}

	void shutdown()
	{
		glfwDestroyWindow(this->m_window);
		glfwTerminate();
	}

	std::pair<unsigned int, double> get_keyframe_time()
	{
		double new_time = 0.0;

		// if (do_animate)
		new_time = glfwGetTime();

		auto delta = new_time - old_time;

		// Note this is very specific to AstroBoy
		static double accumulate_time  = 0.0;
		static int    current_keyframe = 0;
		const double  pf               = 1.166670 / 36.0;

		accumulate_time += delta;
		// if (do_animate)
		current_keyframe = static_cast<int>(accumulate_time / pf);

		if (accumulate_time > 1.66670 || (current_keyframe > 5))        // TODO: Just strip off some frames
		{
			accumulate_time  = 0.0;
			current_keyframe = 0;
		}

		this->old_time = new_time;

		return std::make_pair(current_keyframe, delta);
	}

  protected:
  private:
	double      old_time{0.0};
	GLFWwindow *m_window{nullptr};

	int32_t win_width{1024};
	int32_t win_height{900};
};

define_translation_unit_vtable(MacOSApp)
{}

}        // namespace ror
