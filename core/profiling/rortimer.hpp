// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2009-2010
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Version: 1.0.0 (2008-2018)

#pragma once

#include "foundation/rortypes.hpp"
#include "roar.hpp"

#include <chrono>

namespace ror
{
// TODO: Fix this mess some day, dynamically decide which clock to use

// Make sure the high res clock is steady
// static_assert(std::chrono::high_resolution_clock::is_steady);
static_assert(std::chrono::steady_clock::is_steady, "chrono::steady_clock is not steady.");

// Make sure the high res clock has enough resolution
// static_assert(std::chrono::high_resolution_clock::period::den == 1000000000);
static_assert(std::chrono::steady_clock::period::den == 1000000000, "chrono::steady_clock doesn't have enough resolution.");        // Steady clock is high resolution

class ROAR_ENGINE_ITEM Timer final
{
  public:
	FORCE_INLINE        Timer();                                            //! Default constructor
	FORCE_INLINE        Timer(const Timer &other)         = delete;         //! Copy constructor
	FORCE_INLINE        Timer(Timer &&other) noexcept     = delete;         //! Move constructor
	FORCE_INLINE Timer &operator=(const Timer &other)     = delete;         //! Copy assignment operator
	FORCE_INLINE Timer &operator=(Timer &&other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~Timer() noexcept                        = default;        //! Destructor

	FORCE_INLINE int64_t tick();        // Tick will return the time difference in useconds between calls
	FORCE_INLINE auto    elapsed();

	FORCE_INLINE double64_t tick_seconds();             // Tick will return the time difference in seconds between calls
	FORCE_INLINE double64_t tick_milliseconds();        // Tick will return the time difference in milliseconds between calls
	FORCE_INLINE double64_t tick_microseconds();        // Tick will return the time difference in microseconds between calls
	FORCE_INLINE double64_t tick_nanoseconds();         // Tick will return the time difference in nanoseconds between calls

  private:
	// using TimePoint    = std::chrono::high_resolution_clock::time_point;
	using TimePoint    = std::chrono::steady_clock::time_point;
	using Nanoseconds  = std::chrono::nanoseconds;
	using Microseconds = std::chrono::microseconds;

	TimePoint m_previous_time{};
};

}        // namespace ror

#include "rortimer.hh"
