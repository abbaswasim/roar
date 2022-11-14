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

#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"

namespace ror
{

size_t EventHandleHash::operator()(const EventHandle &a_event_handle) const
{
	return std::hash<uint32_t>()(a_event_handle);
}

EventHandle create_event_handle(EventType     a_event_type,
                                EventCode     a_event_code,
                                EventModifier a_event_modifier,
                                EventState    event_state)
{
	EventHandle event_handle;

	event_handle = EventHandle(enum_to_type_cast(a_event_type) |
	                           enum_to_type_cast(a_event_code) << 8 |
	                           enum_to_type_cast(a_event_modifier) << 16 |
	                           enum_to_type_cast(event_state) << 24);

	return event_handle;
}

EventSystem::EventSystem()
{
	this->init();
}

void EventSystem::subscribe(EventHandle a_event_handle, std::function<void(Event &)> a_function)
{
	this->m_subscribers[a_event_handle].push_back(a_function);
}

void EventSystem::unsubscribe(EventHandle a_event_handle, std::function<void(Event &)> a_function)
{
	auto  function_find_predicate = [&a_function](std::function<void(Event &)> &function) { return function.target_type().hash_code() == a_function.target_type().hash_code(); };
	auto &subscribers             = this->m_subscribers[a_event_handle];
	auto  iter                    = std::find_if(subscribers.begin(), subscribers.end(), function_find_predicate);

	if (iter != subscribers.end())
		subscribers.erase(iter);
}

void EventSystem::init()
{
	auto event_type_max     = enum_to_type_cast(EventType::max);
	auto event_code_max     = enum_to_type_cast(EventCode::max);
	auto event_modifier_max = enum_to_type_cast(EventModifier::max);
	auto event_state_max    = enum_to_type_cast(EventState::max);

	auto subscribers_count = event_type_max * event_modifier_max * event_state_max;
	this->m_subscribers.reserve(subscribers_count);

	// This will create some invalid handle combinations like mouse + a + right + move but thats ok
	for (size_t i = 0; i < event_type_max; ++i)
		for (size_t j = 0; j < event_code_max; ++j)
			for (size_t k = 0; k < event_modifier_max; ++k)
				for (size_t l = 0; l < event_state_max; ++l)
					this->m_subscribers[create_event_handle(static_cast<EventType>(i),
					                                        static_cast<EventCode>(j),
					                                        static_cast<EventModifier>(k),
					                                        static_cast<EventState>(l))] = {};
}

void EventSystem::notify(Event a_event) const
{
	if (a_event.m_live)
	{
		for (auto &sub : this->m_subscribers.at(a_event.m_handle))
		{
			sub(a_event);
			if (!a_event.m_live)        // Some subscribers might consume the event
				break;
		}
	}
}

void EventSystem::notify(std::vector<Event> a_events) const
{
	for (auto event : a_events)
	{
		this->notify(event);
	}
}

}        // namespace ror
