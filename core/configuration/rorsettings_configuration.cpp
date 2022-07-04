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

#include "configuration/rorsettings_configuration.hpp"
#include <vector>

namespace ror
{
define_translation_unit_vtable(SettingsConfig)
{}

generic_config get_generic_config_value(json::iterator a_it)
{
	generic_config c;

	if (a_it->is_number())
		c = static_cast<uint32_t>(a_it.value());
	if (a_it->is_string())
		c = static_cast<std::string>(a_it.value());
	if (a_it->is_boolean())
		c = static_cast<bool>(a_it.value());
	if (a_it->is_number_float())
		c = static_cast<float32_t>(a_it.value());
	if (a_it->is_array())
	{
		if (a_it->begin()->is_number())
		{
			c = static_cast<std::vector<uint32_t>>(a_it.value());
		}
		if (a_it->begin()->is_string())
		{
			c = static_cast<std::vector<std::string>>(a_it.value());
		}
		if (a_it->begin()->is_number_float())
		{
			c = static_cast<std::vector<float32_t>>(a_it.value());
		}
	}

	return c;
}

void SettingsConfig::load_specific()
{
	std::stack<std::pair<json::iterator, std::string>> objects_stack;        // Iterators stack to not have to do recursive walk of the config tree

	for (json::iterator it = this->m_json_file.begin(); it != this->m_json_file.end(); ++it)
		objects_stack.push(std::make_pair(it, ""));

	while (!objects_stack.empty())
	{
		auto object_iter = objects_stack.top();
		objects_stack.pop();

		if (object_iter.first->is_object())
		{
			for (json::iterator iter = object_iter.first->begin(); iter != object_iter.first->end(); ++iter)
				objects_stack.push(std::make_pair(iter, object_iter.second + object_iter.first.key() + ":"));
		}
		else
		{
			generic_config c{get_generic_config_value(object_iter.first)};
			this->m_generic_configs[object_iter.second + object_iter.first.key()] = c;
		}
	}
}
}        // namespace ror
