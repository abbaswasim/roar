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

#include "event_system/rorevent_system.hpp"
#include "foundation/rorcrtp.hpp"
#include "foundation/rormacros.hpp"
#include <memory>

class GLFWwindow;

namespace ror
{
template <class _type>
class ROAR_ENGINE_ITEM Application : public Crtp<_type, Application>
{
  public:
	FORCE_INLINE              Application(const Application &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              Application(Application &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE Application &operator=(const Application &a_other) = default;              //! Copy assignment operator
	FORCE_INLINE Application &operator=(Application &&a_other) noexcept = default;          //! Move assignment operator
	FORCE_INLINE virtual ~Application() noexcept override               = default;          //! Destructor

	using value_type = _type;

	// clang-format off
	FORCE_INLINE void         run()                                      {     this->underlying().run();                         }
	FORCE_INLINE void         resize(int a_width, int a_height)          {     this->underlying().resize(a_width, a_height);     }
	FORCE_INLINE EventSystem &event_system()                             {     return this->m_event_system;                      }
	// clang-format on

	explicit FORCE_INLINE Application(EventSystem &a_event_system) :
		m_event_system(a_event_system)
	{}

  protected:
	FORCE_INLINE Application() = delete;        //! Default constructor
  private:
	EventSystem &m_event_system;        //! Non-owning pointer to an event system that the application can use,
};

}        // namespace ror
