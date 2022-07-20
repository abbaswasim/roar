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

#include "command_line/rorcommand_line.hpp"
#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include <any>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <unordered_map>
#include <vector>

namespace ror
{

CommandLineParser::CommandLineParser(std::vector<std::string> a_arguments) :
    m_arguments(a_arguments)
{
	assert(this->m_arguments.size() > 0);

	add("--help", "-h", "Command line arguments help", ror::CommandLineParser::OptionType::none, false,
	    [this](std::any) { print_help("Help", false); });
}

void CommandLineParser::add(std::string a_long_option, std::string a_short_option, std::string a_documentation, OptionType a_option_type, bool a_required, std::function<void(std::any)> a_callback)
{
	Option option;

	option.m_required      = a_required;
	option.m_short_name    = a_short_option;
	option.m_documentation = a_documentation;
	option.m_type          = a_option_type;
	option.m_callback      = a_callback;

	// Long option addressable Option
	auto inserted = this->m_options.emplace(a_long_option, option);

	if (a_required)
		this->m_required_options.emplace(a_long_option, inserted.first->second);

	// Also second copy of short option addressable Option
	if (a_short_option != "")
		this->m_short_to_long.emplace(a_short_option, a_long_option);

	this->m_max_argument_size      = std::max(this->m_max_argument_size, (a_long_option.length() + a_short_option.length() + 4));
	this->m_max_documentation_size = std::max(this->m_max_documentation_size, a_documentation.length());
}

void CommandLineParser::add(std::string a_long_option, std::function<void(std::any)> a_callback)
{
	this->add(a_long_option, "", "", OptionType::none, false, a_callback);
}

void CommandLineParser::add(std::string a_long_option, std::string a_documentation, std::function<void(std::any)> a_callback)
{
	this->add(a_long_option, "", a_documentation, OptionType::none, false, a_callback);
}

bool CommandLineParser::execute()
{
	std::unordered_map<std::string, bool> required_options;
	for (auto &req_opt : this->m_required_options)
		required_options.emplace(req_opt.first, false);

	for (size_t i = 1; i < this->m_arguments.size(); ++i)
	{
		auto arg          = this->m_arguments[i];
		auto short_option = this->m_short_to_long.find(arg);

		if (short_option != this->m_short_to_long.end())
			arg = short_option->second;

		auto long_option = this->m_options.find(arg);

		if (long_option != this->m_options.end())
		{
			auto    &opt = long_option->second;
			std::any callback_args{};

			if (opt.m_type != OptionType::none)
			{
				if (i + 1 >= this->m_arguments.size())
				{
					this->print_help("No more arguments to read but option expects a value");
					return false;
				}

				auto &value       = this->m_arguments[i++ + 1];
				auto  next_option = this->m_options.find(value);        // This means that the no options can be values of arguments

				if (next_option != this->m_options.end())
				{
					this->print_help("Argument expects a value but following the argument is another option");
					return false;
				}

				callback_args = this->option_type_cast(opt.m_type, value);
			}

			if (opt.m_required)
				required_options[long_option->first] = true;

			opt.m_callback(callback_args);
		}
		else
		{
			this->print_help("Unknown option: " + arg + " provided");
			return false;
		}
	}

	std::string required_options_names{};
	for (auto &req_opt : required_options)
		if (!req_opt.second)
		{
			required_options_names.append(req_opt.first);
			required_options_names.append(", ");
		}

	if (required_options_names != "")
	{
		this->print_help("Required options: " + required_options_names + "not provided");
		return false;
	}

	return true;
}

void CommandLineParser::print_help(std::string a_error_string, bool a_error)
{
	if (a_error)
		log_error("Command line parsing returned with error \"{}\"\n", a_error_string.c_str());
	else
		log_info("Command line parsing \"{}\"\n", a_error_string.c_str());

	std::string arguments;
	auto        max_size    = this->m_max_argument_size + this->m_max_documentation_size + this->m_max_type_size + 10;
	auto        extra_space = 3u;
	arguments.append("\t");
	arguments.append(max_size + 10, '-');
	arguments.append("\n\t");
	arguments.append("Name / Short Name");
	arguments.append(this->m_max_argument_size - std::string("Name / Short Name").length() + extra_space, ' ');
	arguments.append("Argument");
	arguments.append(this->m_max_type_size - std::string("Argument").length() + extra_space, ' ');
	arguments.append("Documentation");
	arguments.append(this->m_max_documentation_size - std::string("Documentation").length() + extra_space, ' ');
	arguments.append("Required");
	arguments.append(extra_space, ' ');
	arguments.append("\n\t");
	arguments.append(max_size + 10, '-');

	for (auto &opt : this->m_options)
	{
		arguments.append("\n\t");
		arguments.append(opt.first);
		auto name_space = 3u;
		if (opt.second.m_short_name != "")
		{
			arguments.append(" / ");
			arguments.append(opt.second.m_short_name);
			name_space = 0;
		}
		auto tspace = this->m_max_argument_size - (opt.first.length() + opt.second.m_short_name.length()) + name_space;
		arguments.append(tspace, ' ');

		auto opt_string = this->option_to_string(opt.second.m_type);
		arguments.append(opt_string);
		arguments.append(this->m_max_type_size - opt_string.length() + extra_space, ' ');
		arguments.append(opt.second.m_documentation);
		arguments.append(this->m_max_documentation_size - opt.second.m_documentation.length() + extra_space, ' ');
		if (opt.second.m_required)
			arguments.append("yes");
		else
			arguments.append("no");
	}

	arguments.append("\n\t");
	arguments.append(max_size + 10, '-');
	arguments.append("\n");

	std::filesystem::path app_name{this->m_arguments[0]};

	log_info("\n\n{} expects the following command line arguments:\n{}", app_name.filename().c_str(), arguments.c_str());
}

std::any CommandLineParser::option_type_cast(OptionType a_type, const std::string &a_value)
{
	// clang-format off
		switch (a_type)
		{
			case OptionType::type_int: return std::stoi(a_value);
			case OptionType::type_float: return std::stof(a_value);
			case OptionType::type_string: return a_value;
			case OptionType::none: return {};
		}
	// clang-format on
	return {};
}

std::string CommandLineParser::option_to_string(OptionType a_type)
{
	// clang-format off
		switch (a_type)
		{
			case OptionType::type_int: return "integer";
			case OptionType::type_float: return "float";
			case OptionType::type_string: return "string";
			case OptionType::none: return "none";
		}
	// clang-format on
	return "none";
}

}        // namespace ror
