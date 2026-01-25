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
#include "profiling/rorlog.hpp"
#include "settings/rorsettings.hpp"
#include <fstream>
#include <memory>
#include <regex>
#include <source_location>
#include <string>

namespace ror
{
constexpr uint32_t column_size = 40;

static std::string trim_token(std::string a_text)
{
	auto not_space = [](unsigned char c) { return !std::isspace(c); };

	a_text.erase(a_text.begin(), std::find_if(a_text.begin(), a_text.end(), not_space));
	a_text.erase(std::find_if(a_text.rbegin(), a_text.rend(), not_space).base(), a_text.end());

	if (!a_text.empty() && a_text.back() == ';')
		a_text.pop_back();

	return a_text;
}

static bool read_specific_line(const std::source_location &a_loc, std::string &a_out_line)
{
	auto filename    = a_loc.file_name();
	auto target_line = a_loc.line();

	std::ifstream in(filename);
	if (!in.is_open())
		return false;

	std::string line;
	std::size_t current_line = 1;

	while (std::getline(in, line))
	{
		if (current_line == target_line)
		{
			a_out_line = line;
			return true;
		}
		++current_line;
	}

	return false;        // line not found
}

static std::string read_function_name(const std::source_location &a_loc)
{
	std::string function_name{a_loc.function_name()};

	std::regex  nsp_col_col_regex(R"((?:[A-Za-z_]\w*(?:<[^<>]*>)?::)+[A-Za-z_]\w*(?:<[^<>]*>)?(?=\())");
	std::smatch identifier;

	auto match_found = std::regex_search(function_name, identifier, nsp_col_col_regex);
	assert(match_found && "Couldn't extract function name from the function name for lambda");
	(void) match_found;

	return identifier.str();
}

static std::string read_file_name(const std::source_location &a_loc)
{
	uint32_t    file_line{a_loc.line()};
	std::string file_name{a_loc.file_name()};
	std::string roar_name{"roar_engine"};

	file_name += ":";
	file_name += std::to_string(file_line);
	auto pos = file_name.find(roar_name);
	assert(pos != std::string::npos && "Can't have file outside roar_engine");

	return "./" + file_name.substr(pos);
}

static std::string fit_string_to_size(std::string a_string, uint32_t a_size)
{
	(void) a_size;
	auto str_size = a_string.size();
	if (str_size > a_size)        // Mean string already too big we need to trim it
	{
		assert(a_size > 3 && "String column size is too small for this string");
		auto str = a_string.substr(str_size - a_size + 3);
		str      = "..." + str;

		return str;
	}

	auto diff = a_size - str_size;
	for (uint32_t i = 0; i < diff; i++)
		a_string += " ";

	return a_string;
}

static std::string extract_callback_identifier(const std::source_location &a_loc)
{
	std::string function = fit_string_to_size(read_function_name(a_loc), column_size);
	std::string filename = fit_string_to_size(read_file_name(a_loc), column_size);

	std::string line;
	if (read_specific_line(a_loc, line))
	{
		auto subscribe_pos = line.find("subscribe");
		if (subscribe_pos == std::string::npos)
			return "{anonymous_lambda}";

		auto comma = line.find(',', subscribe_pos);
		if (comma == std::string::npos)
			return "{anonymous_lambda}";

		auto is_unnamed = false;
		auto tharrow    = line.find("this->", comma);
		if (tharrow != std::string::npos)
			comma = tharrow + 6;
		else
		{
			auto spc = line.find(" ", comma);
			if (spc != std::string::npos)
				comma = spc;

			auto anonym = line.find("[", comma);
			if (anonym != std::string::npos)
				is_unnamed = true;
		}

		auto close = line.find(')', comma);
		if (close == std::string::npos)
			close = line.size();

		auto token = line.substr(comma, close - comma);

		token = trim_token(token);
		assert(token.size() < column_size && "Lambda identifier name is too big");

		if (is_unnamed)
			token = "{anonymous_lambda}";

		token = fit_string_to_size(token, column_size);

		token += " | ";

		return token + function + " | " + filename + " |";
	}

	ror::log_critical("Source Location file name probably can't be opened which should be a valid file on the system");
	return {};
}

static std::string subscriber_name(const EventSystem::EventSubscriber &a_subscriber)
{
	assert(!a_subscriber.m_name.empty() && "Can't find subcriber name");
	return a_subscriber.m_name;
}

// clang-format off
std::string event_type(EventType a_type)
{
	switch(a_type)
	{
	case EventType::mouse:             return "mouse:";
	case EventType::application:       return "application:";
	case EventType::keyboard:          return "keyboard:";
	case EventType::renderer:          return "renderer:";
	case EventType::gamepad:           return "gamepad:";
	case EventType::window:            return "window:";
	case EventType::buffer:            return "buffer:";
	case EventType::error:             return "error:";
	case EventType::file:              return "file:";
	case EventType::max:               return "max:";
    }
};

std::string event_code(EventCode a_code)
{
	switch(a_code)
	{
	case EventCode::none:              return "none:";
	case EventCode::left_mouse:        return "left_mouse:";
	case EventCode::middle_mouse:      return "middle_mouse:";
	case EventCode::right_mouse:       return "right_mouse:";
	case EventCode::n0:                return "n0:";
	case EventCode::n1:                return "n1:";
	case EventCode::n2:                return "n2:";
	case EventCode::n3:                return "n3:";
	case EventCode::n4:                return "n4:";
	case EventCode::n5:                return "n5:";
	case EventCode::n6:                return "n6:";
	case EventCode::n7:                return "n7:";
	case EventCode::n8:                return "n8:";
	case EventCode::n9:                return "n9:";
	case EventCode::num0:              return "num0:";
	case EventCode::num1:              return "num1:";
	case EventCode::num2:              return "num2:";
	case EventCode::num3:              return "num3:";
	case EventCode::num4:              return "num4:";
	case EventCode::num5:              return "num5:";
	case EventCode::num6:              return "num6:";
	case EventCode::num7:              return "num7:";
	case EventCode::num8:              return "num8:";
	case EventCode::num9:              return "num9:";
	case EventCode::a:                 return "a:";
	case EventCode::b:                 return "b:";
	case EventCode::c:                 return "c:";
	case EventCode::d:                 return "d:";
	case EventCode::e:                 return "e:";
	case EventCode::f:                 return "f:";
	case EventCode::g:                 return "g:";
	case EventCode::h:                 return "h:";
	case EventCode::i:                 return "i:";
	case EventCode::j:                 return "j:";
	case EventCode::k:                 return "k:";
	case EventCode::l:                 return "l:";
	case EventCode::m:                 return "m:";
	case EventCode::n:                 return "n:";
	case EventCode::o:                 return "o:";
	case EventCode::p:                 return "p:";
	case EventCode::q:                 return "q:";
	case EventCode::r:                 return "r:";
	case EventCode::s:                 return "s:";
	case EventCode::t:                 return "t:";
	case EventCode::u:                 return "u:";
	case EventCode::v:                 return "v:";
	case EventCode::w:                 return "w:";
	case EventCode::x:                 return "x:";
	case EventCode::y:                 return "y:";
	case EventCode::z:                 return "z:";
	case EventCode::f1:                return "f1:";
	case EventCode::f2:                return "f2:";
	case EventCode::f3:                return "f3:";
	case EventCode::f4:                return "f4:";
	case EventCode::f5:                return "f5:";
	case EventCode::f6:                return "f6:";
	case EventCode::f7:                return "f7:";
	case EventCode::f8:                return "f8:";
	case EventCode::f9:                return "f9:";
	case EventCode::f10:               return "f10:";
	case EventCode::f11:               return "f11:";
	case EventCode::f12:               return "f12:";
	case EventCode::f13:               return "f13:";
	case EventCode::f14:               return "f14:";
	case EventCode::f15:               return "f15:";
	case EventCode::f16:               return "f16:";
	case EventCode::f17:               return "f17:";
	case EventCode::f18:               return "f18:";
	case EventCode::f19:               return "f19:";
	case EventCode::f20:               return "f20:";
	case EventCode::f21:               return "f21:";
	case EventCode::f22:               return "f22:";
	case EventCode::f23:               return "f23:";
	case EventCode::f24:               return "f24:";
	case EventCode::f25:               return "f25:";
	case EventCode::tab:               return "tab:";
	case EventCode::space:             return "space:";
	case EventCode::back_space:        return "back_space:";
	case EventCode::caps_lock:         return "caps_lock:";
	case EventCode::page:              return "page:";
	case EventCode::enter:             return "enter:";
	case EventCode::del:               return "del:";
	case EventCode::home:              return "home:";
	case EventCode::end:               return "end:";
	case EventCode::insert:            return "insert:";
	case EventCode::esc:               return "esc:";
	case EventCode::fn:                return "fn:";
	case EventCode::decimal:           return "decimal:";
	case EventCode::divide:            return "divide:";
	case EventCode::multiply:          return "multiply:";
	case EventCode::subtract:          return "subtract:";
	case EventCode::add:               return "add:";
	case EventCode::equal:             return "equal:";
	case EventCode::apostrophe:        return "apostrophe:";
	case EventCode::comma:             return "comma:";
	case EventCode::minus:             return "minus:";
	case EventCode::period:            return "period:";
	case EventCode::slash:             return "slash:";
	case EventCode::semicolon:         return "semicolon:";
	case EventCode::left_bracket:      return "left_bracket:";
	case EventCode::backslash:         return "backslash:";
	case EventCode::right_bracket:     return "right_bracket:";
	case EventCode::grave_accent:      return "grave_accent:";
	case EventCode::right:             return "right:";
	case EventCode::left:              return "left:";
	case EventCode::down:              return "down:";
	case EventCode::up:                return "up:";
	case EventCode::page_up:           return "page_up:";
	case EventCode::page_down:         return "page_down:";
	case EventCode::scroll_lock:       return "scroll_lock:";
	case EventCode::num_lock:          return "num_lock:";
	case EventCode::print_screen:      return "print_screen:";
	case EventCode::pause:             return "pause:";
	case EventCode::left_shift:        return "left_shift:";
	case EventCode::left_control:      return "left_control:";
	case EventCode::left_alt:          return "left_alt:";
	case EventCode::left_super:        return "left_super:";
	case EventCode::right_shift:       return "right_shift:";
	case EventCode::right_control:     return "right_control:";
	case EventCode::right_alt:         return "right_alt:";
	case EventCode::right_super:       return "right_super:";
	case EventCode::menu:              return "menu:";
	case EventCode::max:               return "max:";
	}
};


std::string event_modifier(EventModifier a_modifier)
{
	switch(a_modifier)
	{
	case EventModifier::none:          return "none:";
	case EventModifier::shift:         return "shift:";
	case EventModifier::control:       return "control:";
	case EventModifier::command:       return "command:";
	case EventModifier::option:        return "option:";
	case EventModifier::caps_lock:     return "caps_lock:";
	case EventModifier::num_lock:      return "num_lock:";
	case EventModifier::max:           return "max:";
	}
};

std::string event_state(EventState a_state)
{
	switch(a_state)
	{
	case EventState::click:            return "click";
	case EventState::repeat:           return "repeat";
	case EventState::down:             return "down";
	case EventState::move:             return "move";
	case EventState::scroll:           return "scroll";
	case EventState::drag:             return "drag";
	case EventState::drop:             return "drop";
	case EventState::resize:           return "resize";
	case EventState::focus:            return "focus";
	case EventState::max:              return "max";
	}
};

// clang-format on
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

std::string create_event_handle(EventHandle a_handle)
{
	auto type     = ror::event_type(a_handle);
	auto code     = ror::event_code(a_handle);
	auto modifier = ror::event_modifier(a_handle);
	auto state    = ror::event_state(a_handle);

	return ror::event_type(type) + ror::event_code(code) + ror::event_modifier(modifier) + ror::event_state(state);
}

EventSystem::EventSystem()
{
	this->init();
}

void EventSystem::subscribe(EventHandle a_event_handle, std::function<void(Event &)> a_function, const std::source_location &a_loc)
{
	std::lock_guard<std::mutex> lock{this->m_mutex};

	this->m_subscribers[a_event_handle].push_back({std::move(a_function), extract_callback_identifier(a_loc)});
}

void EventSystem::subscribe_early(EventHandle a_event_handle, std::function<void(Event &)> a_function, const std::source_location &a_loc)
{
	std::lock_guard<std::mutex> lock{this->m_mutex};

	auto &subs = this->m_subscribers[a_event_handle];
	subs.insert(subs.begin(), {std::move(a_function), extract_callback_identifier(a_loc)});
}

void EventSystem::unsubscribe(EventHandle a_event_handle, std::function<void(Event &)> a_function)
{
	std::lock_guard<std::mutex> lock{this->m_mutex};

	auto  function_find_predicate = [&a_function](EventSubscriber &subscriber) { return subscriber.m_callback.target_type().hash_code() == a_function.target_type().hash_code(); };
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

	std::lock_guard<std::mutex> lock{this->m_mutex};
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
		auto &subs = this->m_subscribers.at(a_event.m_handle);
		for (auto &sub : subs)
		{
			sub.m_callback(a_event);
			if (!a_event.m_live)        // Some subscribers might consume the event, in which case the order of subscription matters
				break;
		}

		if constexpr (ror::get_build() == ror::BuildType::build_debug)
		{
			if (subs.empty() && ror::settings().m_warn_event_system)
				ror::log_warn("Event not registered {}", create_event_handle(a_event.m_handle));
			else if (ror::settings().m_log_event_system)
				ror::log_info("Event triggered {}", create_event_handle(a_event.m_handle));
		}
	}
}

void EventSystem::notify(std::vector<Event> a_events) const
{
	for (auto event : a_events)
		this->notify(event);
}

void EventSystem::print_keybindings() const
{
	std::string result;
	result.reserve(this->m_subscribers.size() * 150);

	for (const auto &entry : this->m_subscribers)
	{
		if (entry.second.empty())
			continue;

		if (event_type(entry.first) != EventType::keyboard)
			continue;

		auto short_event = ror::event_code(ror::event_code(entry.first));
		auto handle_str = create_event_handle(entry.first);
		assert(handle_str.size() < column_size && "Keyboard handle is bigger than column_size characters");
		assert(short_event.size() < (column_size / 3) && "Keyboard handle short event is bigger than half column_size characters");

		result += "\n| ";
		result += short_event.substr(0, short_event.size() - 1);

		for (uint32_t i = 0; i < (column_size / 3) - short_event.size(); i++)
			result += " ";

		result += "| ";
		result += handle_str;

		for (uint32_t i = 0; i < column_size - handle_str.size(); i++)
			result += " ";

		result += "|";

		uint32_t sub_it = 1;
		for (const auto &subscriber : entry.second)
		{
			result += " ";
			if (sub_it > 1)
			{
				for (uint32_t i = 0; i < column_size + (column_size / 3) + 1; i++)
					result += " ";

				result += " | ";
			}

			result += subscriber_name(subscriber);
			if (sub_it < entry.second.size())
			{
				result += "\n";
				sub_it++;
			}
		}

		// result += " | ";
	}

	ror::log_info("Here all the keybinding for {} subscribers\n{}", this->m_subscribers.size(), result);
}

}        // namespace ror
