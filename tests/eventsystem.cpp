// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

#include "common.hpp"
#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include <gtest/gtest.h>
#include <typeindex>
#include <vector>

namespace ror_test
{

class Subscriber1
{
  public:
	void sub1_func1(ror::Event e)
	{
		(void) e;
	}
	void sub1_func2(ror::Event e)
	{
		(void) e;
	}
};

class Subscriber2
{
  public:
	void sub2_func1(ror::Event e)
	{
		(void) e;
	}
	void sub2_func2(ror::Event e)
	{
		(void) e;
	}
};

class Camera
{
  public:
	void mouse_move(ror::Event e)
	{
		(void) e;
	}
	void mouse_clicked(ror::Event e)
	{
		(void) e;
	}
};

using namespace ror;

TEST(EventSystemTest, subscribe_many_subscribers)
{
	Subscriber1 sub1;
	Subscriber2 sub2;
	Camera      cam1;

	ror::EventSystem event_state;
	event_state.subscribe(keyboard_a_click, [&sub1](ror::Event &e) { sub1.sub1_func1(e); });
	event_state.subscribe(keyboard_a_click, [&sub2](ror::Event &e) { sub2.sub2_func1(e); });
	event_state.subscribe(keyboard_a_click, [&cam1](ror::Event &e) { cam1.mouse_move(e); });
	event_state.subscribe(create_event_handle(ror::EventType::mouse, ror::EventCode::none, ror::EventModifier::left_mouse, ror::EventState::down), [&cam1](ror::Event &e) { cam1.mouse_move(e); });

	auto camera_move = [&cam1](ror::Event &e) { cam1.mouse_move(e); };

	event_state.subscribe(create_event_handle(ror::EventType::mouse, ror::EventCode::none, ror::EventModifier::left_mouse, ror::EventState::down), camera_move);
	event_state.unsubscribe(create_event_handle(ror::EventType::mouse, ror::EventCode::none, ror::EventModifier::left_mouse, ror::EventState::down), camera_move);
}

class Counter
{
  public:
	void inc(ror::Event e)
	{
		this->m_value++;

		if (e.m_payload.has_value())
			this->m_value += std::any_cast<int32_t>(e.m_payload);
	}

	void add_vector2(ror::Event e)
	{
		if (e.is_compatible<ror::Vector2f>())
		{
			auto vec2 = std::any_cast<ror::Vector2f>(e.m_payload);
			this->m_value += static_cast<uint32_t>(vec2.x + vec2.y);
		}
	}

	template <class _type>
	void add_vectors(ror::Event e)
	{
		if (e.is_compatible<ror::Vector4f>())
		{
			auto vec = std::any_cast<_type>(e.m_payload);
			this->m_value += static_cast<uint32_t>(vec.x + vec.y);
		}
	}

	template <class _type>
	void add_anything(ror::Event e)
	{
		auto vec3 = e.get_payload<ror::Vector3f>();
		this->m_value += static_cast<uint32_t>(vec3.x + vec3.y + vec3.z);
	}

	int32_t val()
	{
		return m_value;
	}

	int32_t m_value{0};
};

TEST(EventSystemTest, create_many_events)
{
	Counter count;

	ror::EventSystem event_system;

	event_system.subscribe(keyboard_a_click, [&count](ror::Event &e) { count.inc(e); });

	EXPECT_NE(count.m_value, 1);

	event_system.notify({keyboard_a_click, true, 0});
	EXPECT_EQ(count.m_value, 1);

	event_system.notify({keyboard_a_click, true, 0});
	EXPECT_EQ(count.m_value, 2);

	event_system.subscribe(keyboard_a_click, [&count](ror::Event &e) { count.inc(e); });

	event_system.notify({keyboard_a_click, true, 0});
	event_system.notify({keyboard_a_click, true, 0});
	EXPECT_EQ(count.m_value, 6);

	event_system.unsubscribe(keyboard_a_click, [&count](ror::Event &e) { count.inc(e); });        // This one shouldn't work. We need to use a lambda we know
	event_system.notify({keyboard_a_click, true, 0});
	EXPECT_EQ(count.m_value, 8);

	auto count_inc = [&count](ror::Event &e) { count.inc(e); };

	event_system.subscribe(keyboard_a_click, count_inc);
	event_system.notify({keyboard_a_click, true, 0});
	EXPECT_EQ(count.m_value, 11);

	event_system.notify({keyboard_a_click, true, 0});
	EXPECT_EQ(count.m_value, 14);

	event_system.notify({keyboard_a_click, true, 0});
	EXPECT_EQ(count.m_value, 17);

	event_system.unsubscribe(keyboard_a_click, count_inc);

	event_system.notify({keyboard_a_click, true, 0});
	EXPECT_EQ(count.m_value, 19);

	event_system.notify({keyboard_a_click, true, 0});
	EXPECT_EQ(count.m_value, 21);

	event_system.notify({keyboard_a_click, true, 0});
	EXPECT_EQ(count.m_value, 23);

	event_system.notify({keyboard_a_repeat, true, 0});
	EXPECT_EQ(count.m_value, 23);

	event_system.notify({mouse_left_mouse_click, true, 0});
	EXPECT_EQ(count.m_value, 23);

	{
		std::vector<ror::Event> events{{mouse_left_mouse_click, true, 0}, {mouse_left_mouse_click, true, 0}, {mouse_left_mouse_click, true, 0}, {keyboard_a_click, true, 0}, {keyboard_a_repeat, true, 0}};
		event_system.notify(events);
	}

	EXPECT_EQ(count.m_value, 25);

	{
		std::vector<ror::Event> events{{mouse_left_mouse_click, true, 0}, {mouse_left_mouse_click, true, 0}, {mouse_left_mouse_click, true, 0}, {keyboard_a_click, false, 0}, {keyboard_a_repeat, true, 0}};
		event_system.notify(events);
	}

	EXPECT_EQ(count.m_value, 25);

	int32_t payload = 24;

	// Now we have two subscribers looking for keyboard_a_click, that does an increment and +24 each
	event_system.notify({keyboard_a_click, true, payload});
	EXPECT_EQ(count.m_value, 75);

	event_system.subscribe(keyboard_a_click, count_inc);
	event_system.notify(ror::Event{keyboard_a_click, true});
	EXPECT_EQ(count.m_value, 78);

	{
		std::vector<ror::Event> events{{mouse_left_mouse_click, true}, {mouse_left_mouse_click, true}, {mouse_left_mouse_click, true}, {keyboard_a_click, true}, {keyboard_a_repeat, true}};
		event_system.notify(events);
	}

	EXPECT_EQ(count.m_value, 81);
}

TEST(EventSystemTest, different_payload)
{
	Counter count;

	ror::EventSystem event_system;

	auto vec2_inc = [&count](ror::Event &e) { count.add_vector2(e); };

	event_system.subscribe(keyboard_b_click, vec2_inc);

	EXPECT_NE(count.m_value, 1);

	event_system.notify({keyboard_b_click, true, ror::Vector2f(1.0, 2.0)});
	EXPECT_EQ(count.m_value, 3);

	event_system.notify({keyboard_b_click, true, ror::Vector2f(10.0, 20.0)});
	EXPECT_EQ(count.m_value, 33);

	event_system.notify({keyboard_b_click, true, ror::Vector2f(100.0, 200.0)});
	EXPECT_EQ(count.m_value, 333);

	event_system.subscribe(keyboard_b_click, [&count](ror::Event &e) { count.add_vectors<ror::Vector4f>(e); });

	event_system.notify({keyboard_b_click, true, ror::Vector4f(100.0, 200.0, 0.0, 0.0)});
	EXPECT_EQ(count.m_value, 633);

	event_system.subscribe(keyboard_b_click, [&count](ror::Event &e) { count.add_anything<ror::Vector3f>(e); });
	event_system.notify({keyboard_b_click, true, ror::Vector3f(1.0, 2.0, 3.0)});
	EXPECT_EQ(count.m_value, 639);
}
}        // namespace ror_test
