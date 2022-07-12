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

#include "foundation/rormacros.hpp"
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

class ROAR_ENGINE_ITEM CommandLineParser final
{
  public:
	FORCE_INLINE                    CommandLineParser(const CommandLineParser &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                    CommandLineParser(CommandLineParser &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE CommandLineParser &operator=(const CommandLineParser &a_other)             = default;        //! Copy assignment operator
	FORCE_INLINE CommandLineParser &operator=(CommandLineParser &&a_other) noexcept         = default;        //! Move assignment operator
	FORCE_INLINE ~CommandLineParser() noexcept                                              = default;        //! Destructor

	enum class OptionType
	{
		none,
		type_int,
		type_float,
		type_string
	};

	struct Option
	{
		bool                          m_required{false};
		std::string                   m_short_name{};
		std::string                   m_documentation{};
		OptionType                    m_type{OptionType::none};
		std::function<void(std::any)> m_callback{};
	};

	CommandLineParser(std::vector<std::string> a_arguments);
	void add(std::string a_long_option, std::string a_short_option, std::string a_documentation, OptionType a_option_type, bool a_required, std::function<void(std::any)> a_callback);
	void add(std::string a_long_option, std::function<void(std::any)> a_callback);
	void add(std::string a_long_option, std::string a_documentation, std::function<void(std::any)> a_callback);
	bool execute();
	void print_help(std::string a_error_message, bool a_error = true);

  protected:
  private:
	std::any    option_type_cast(OptionType a_type, const std::string &a_value);
	std::string option_to_string(OptionType a_type);

	FORCE_INLINE CommandLineParser() = default;        //! Default constructor

	size_t                                          m_max_argument_size{30};             //! For alignment purposes what is the argument size
	size_t                                          m_max_documentation_size{50};        //! For alignment purposes what is the argument documentation size
	size_t                                          m_max_type_size{15};                 //! Needs to be changed of OptionType changes
	std::vector<std::string>                        m_arguments{};                       //! All the arguements that are provided by the caller
	std::unordered_map<std::string, Option>         m_options{};                         //! All the options that this parser knows about
	std::unordered_map<std::string, std::string>    m_short_to_long{};                   //! All the short options to big options mapping
	std::unordered_map<std::string, const Option &> m_required_options{};                //! All the options that this parser knows about and are required, non-owning aliases
};

}        // namespace ror
