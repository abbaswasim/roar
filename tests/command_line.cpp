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
#include "foundation/rortypes.hpp"
#include <any>
#include <gtest/gtest.h>
#include <string>

namespace ror_test
{
TEST(CommandLine, trigger_test)
{
	uint32_t argc = 12;
	const char* argv[] = {"/somewhere/path/roar_test", "--first", "--second", "second", "--third", "3", "-t", "3", "--float", "3.14", "--path", "/something/somewhere/file.txt"};

	std::string output{};
	std::string expected{"--first--secondsecond--third3--third3--float3.140000--path/something/somewhere/file.txt"};

	ror::CommandLineParser cli{std::vector<std::string>{argv, argv + argc}};

	cli.add("--first", [&output](std::any){ output.append("--first");});
	cli.add("--second", "", "Someting second to none", ror::CommandLineParser::OptionType::type_string, false, [&output](std::any a_second){ output.append("--second"); output.append(std::any_cast<std::string>(a_second));});
	cli.add("--third", "-t", "Thirds short argument",  ror::CommandLineParser::OptionType::type_int,  false,  [&output](std::any a_integer){ output.append("--third"); output.append(std::to_string(std::any_cast<int32_t>(a_integer)));});
	cli.add("--float", "", "float value required" , ror::CommandLineParser::OptionType::type_float, false, [&output](std::any a_float){ output.append("--float"); output.append(std::to_string(std::any_cast<float32_t>(a_float)));});
	cli.add("--path", "", "", ror::CommandLineParser::OptionType::type_string, false, [&output](std::any a_path){ output.append("--path"); output.append(std::any_cast<std::string>(a_path));});

	cli.add("--untested", "", "", ror::CommandLineParser::OptionType::type_string, false, [&output](std::any a_path){ output.append("--untested"); output.append(std::any_cast<std::string>(a_path));});

	// cli.add("--req1", "", "", ror::CommandLineParser::OptionType::type_string, true, [](std::any ){});
	// cli.add("--req2", "", "", ror::CommandLineParser::OptionType::none, true, [](std::any ){});
	// cli.add("--req3", "", "", ror::CommandLineParser::OptionType::none, true, [](std::any ){});

	cli.execute();

	EXPECT_EQ(output, expected);


}
}        // namespace ror_test
