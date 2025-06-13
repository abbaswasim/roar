// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2025
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
#include "foundation/rormacros.hpp"
#include "rhi/rorframe_data.hpp"
#include <array>
#include <cstddef>

namespace ror
{

static constexpr uint32_t max_frames_in_flight = 3;

class ROAR_ENGINE_ITEM Frames final
{
  public:
	FORCE_INLINE         Frames()                             = default;        //! Default constructor
	FORCE_INLINE         Frames(const Frames &a_other)        = default;        //! Copy constructor
	FORCE_INLINE         Frames(Frames &&a_other) noexcept    = default;        //! Move constructor
	FORCE_INLINE Frames &operator=(const Frames &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Frames &operator=(Frames &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~Frames() noexcept                   = default;        //! Destructor

	size_t current_frame_index() const
	{
		return this->m_current_frame_index;
	}

	constexpr static size_t max_frames()
	{
		return max_frames_in_flight;
	}

	void begin_frame()
	{
		auto frame = this->next_frame();
		(void) frame;
	}

	void end_frame()
	{
		auto frame = this->current_frame();
		(void) frame;
	}

  protected:
  private:
	// Advance to the next frame in flight
	void advance_frame()
	{
		this->m_current_frame_index = (this->m_current_frame_index + 1) % max_frames_in_flight;
	}

	rhi::FrameData &current_frame()
	{
		// Use completeHandlers system to auto wait here
		// Also automatically wait for the fences
		return this->m_frames[this->m_current_frame_index];
	}

	rhi::FrameData &next_frame()
	{
		this->advance_frame();
		return this->current_frame();
	}

	std::array<rhi::FrameData, max_frames_in_flight> m_frames{};
	size_t                                           m_current_frame_index{0};        //! If we start from {max_frames_in_flight - 1} so begin_frame advances it to 0,
	                                                                                  //! But some shader_buffers are created before they are duplicated and it doesn't work
};

}        // namespace ror

#include "rorframes.hh"
