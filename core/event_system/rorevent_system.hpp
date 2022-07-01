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

#include "foundation/rorcrtp.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorutilities.hpp"
#include "rhi/rorhandles.hpp"
#include <algorithm>
#include <any>
#include <cstdio>
#include <functional>
#include <list>
#include <queue>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace ror
{
create_handle(EventHandle, uint32_t);

class EventHandleHash
{
  public:
	size_t operator()(const EventHandle &a_event_handle) const;
};

enum class EventType : uint32_t
{
	mouse = 0,
	keyboard,
	gamepad,
	window,
	buffer,
	error,
	file,
	max
};

// clang-format off
// TODO: Clean the left_control etc
enum class EventCode : uint32_t
{
	none = 0,
	n0, n1, n2, n3, n4, n5, n6, n7, n8, n9,
	num0, num1, num2, num3, num4, num5, num6, num7, num8, num9,
	a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
	f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25,
	tab, space, back_space, caps_lock, page, enter, del, home, end, insert, esc, fn,
	decimal, divide, multiply, subtract, add, equal, apostrophe, comma, minus, period, slash, semicolon, left_bracket, backslash, right_bracket, grave_accent,
	right, left, down, up, page_up, page_down, scroll_lock, num_lock, print_screen, pause, left_shift, left_control, left_alt, left_super, right_shift, right_control, right_alt, right_super, menu,
	max
};
// clang-format on

enum class EventModifier : uint32_t
{
	none = 0,
	shift,
	control,
	command,
	option,
	caps_lock,
	num_lock,
	left_mouse,
	middle_mouse,
	right_mouse,
	max
};

enum class EventState : uint32_t
{
	click = 0,
	repeat,
	down,
	move,
	scroll,
	drag,
	drop,
	resize,
	focus,
	max
};

EventHandle             create_event_handle(EventType     a_event_type,
											EventCode     a_event_code     = EventCode::none,
											EventModifier a_event_modifier = EventModifier::none,
											EventState    event_state      = EventState::click);
constexpr EventType     event_type(EventHandle a_handle);
constexpr EventCode     event_code(EventHandle a_handle);
constexpr EventModifier event_modifier(EventHandle a_handle);
constexpr EventState    event_state(EventHandle a_handle);

struct Event
{
	EventHandle m_handle{0};         //! Handle to the event containing, type, code, modifier and state
	bool        m_live{true};        //! Is the event still live or consumed by some subscriber
	std::any    m_payload{0};        //! Payload of the event containing a user data to something the user knows

	FORCE_INLINE Event()
	{}
	FORCE_INLINE Event(EventHandle a_event_handle, bool a_live, std::any a_payload = 0) :
		m_handle(a_event_handle), m_live(a_live), m_payload(a_payload)
	{}

	/**
	 * Checks if the payload type is what it should be
	 * TODO: Check the performance of this, it might be slow due to typeid
	 */
	template <class _type>
	bool is_compatible()
	{
		return this->m_payload.has_value() && this->m_payload.type() == typeid(_type);
	}

	/**
	 * Returns the payload contained within the event safely.
	 * If it doesn't exist or its the wrong type default value
	 * of the requested type is requred
	 */
	template <class _type>
	auto get_payload()
	{
		if (is_compatible<_type>())
			return std::any_cast<_type>(this->m_payload);

		return _type();
	}
};

static_assert(sizeof(Event) == 40, "Size of Event is too big");

class ROAR_ENGINE_ITEM EventSystem final
{
  public:
	EventSystem();                                                                          //! Default constructor
	FORCE_INLINE              EventSystem(const EventSystem &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              EventSystem(EventSystem &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE EventSystem &operator=(const EventSystem &a_other)       = default;        //! Copy assignment operator
	FORCE_INLINE EventSystem &operator=(EventSystem &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE ~EventSystem() noexcept                                  = default;        //! Destructor

	/**
	 * When using this function make sure you use the same lambda that was used to subscribe
	 * The following will work fine:
	 *
	 * auto func = [](){};
	 * subsribe(..., func);
	 * unsubsribe(..., func);
	 *
	 * The following will not work fine
	 * subsribe(..., [](){});
	 * unsubsribe(..., [](){});
	 * will not remove the previously added function
	 */
	void unsubscribe(EventHandle a_event_handle, std::function<void(Event &)> a_function);
	void subscribe(EventHandle a_event_handle, std::function<void(Event &)> a_function);
	void notify(Event a_event) const;
	void notify(std::vector<Event> a_events) const;

  protected:
  private:
	void init();

	// TODO: Add some buffering system at some point
	// TODO: Make this thread safe

	std::unordered_map<EventHandle, std::vector<std::function<void(Event &)>>, EventHandleHash> m_subscribers;        //! All the functions that needs to be called for this Event Handle
};

constexpr EventType event_type(EventHandle a_handle)
{
	uint32_t mask = 0x000000FF;
	return static_cast<EventType>(a_handle & mask);
}

constexpr EventCode event_code(EventHandle a_handle)
{
	uint32_t mask = 0x0000FF00;
	return static_cast<EventCode>(a_handle & mask);
}

constexpr EventModifier event_modifier(EventHandle a_handle)
{
	uint32_t mask = 0x00FF0000;
	return static_cast<EventModifier>(a_handle & mask);
}

constexpr EventState event_state(EventHandle a_handle)
{
	uint32_t mask = 0xFF000000;
	return static_cast<EventState>(a_handle & mask);
}

}        // namespace ror
