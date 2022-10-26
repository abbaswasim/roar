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

#include "foundation/rortypes.hpp"
#include "rortimer.hpp"
#include <chrono>
#include <ratio>

namespace ror
{
FORCE_INLINE Timer::Timer()
{
	// this->m_previous_time = std::chrono::high_resolution_clock::now();
	this->m_previous_time = std::chrono::steady_clock::now();
}

FORCE_INLINE auto Timer::elapsed()
{
	auto now              = std::chrono::steady_clock::now();
	auto elapsed          = now - this->m_previous_time;
	this->m_previous_time = now;

	return elapsed;
}

FORCE_INLINE int64_t Timer::tick()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(this->elapsed()).count();
}

FORCE_INLINE double64_t Timer::tick_seconds()
{
	return std::chrono::duration<double>(this->elapsed()).count();
}

FORCE_INLINE double64_t Timer::tick_milliseconds()
{
	return std::chrono::duration<double, std::milli>(this->elapsed()).count();
}

FORCE_INLINE double64_t Timer::tick_microseconds()
{
	return std::chrono::duration<double, std::micro>(this->elapsed()).count();
}

FORCE_INLINE double64_t Timer::tick_nanoseconds()
{
	return std::chrono::duration<double, std::nano>(this->elapsed()).count();
}
}        // namespace ror
