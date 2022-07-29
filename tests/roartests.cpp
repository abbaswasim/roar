// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2019
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
#include "common.hpp"
#include "geometry/geometry.hpp"
#include "math/math_tests.hpp"
#include "profiling/rorlog.hpp"
#include "project_setup.hpp"
#include "random.hpp"
#include "utilities.hpp"

#include <basisu_transcoder.h>
#include <filesystem>
#include <gtest/gtest.h>

ror::SettingsConfig ror_test::settings_configs_copy;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	std::string test_default_project{"test_roar_project"};
	std::string gltf_test_file{""};
	std::string gltf_tests_root{""};
	bool        gltf_compile_shader{false};
	bool        gltf_print_shader{false};
	bool        gltf_all{false};
	bool        gltf_write_shaders{false};

	ror::CommandLineParser cli{std::vector<std::string>{argv, argv + argc}};

	cli.add("--default-project", "-dp", "Default project name for the tests [default \"test_roar_project\"]", ror::CommandLineParser::OptionType::type_string, false,
	        [&test_default_project](std::any a_argument_value) { test_default_project = std::any_cast<std::string>(a_argument_value); });
	cli.add("--gltf-test", "-gt", "Can be used to test a single gltf file", ror::CommandLineParser::OptionType::type_string, false,
	        [&gltf_test_file](std::any a_argument_value) { gltf_test_file = std::any_cast<std::string>(a_argument_value); });
	cli.add("--gltf-compile-shader", "-gp", "Should compile generated shaders for the gltf file provided", ror::CommandLineParser::OptionType::none, false,
	        [&gltf_compile_shader](std::any) { gltf_compile_shader = true; });
	cli.add("--gltf-compile-print-shader", "-gtp", "Should compile and print generated shaders for the gltf file provided", ror::CommandLineParser::OptionType::none, false,
	        [&gltf_print_shader](std::any) { gltf_print_shader = true; });
	cli.add("--gltf-samples", "-gs", "Should test all the glTF 2.0 samples from khronos", ror::CommandLineParser::OptionType::none, false,
	        [&gltf_all](std::any) { gltf_all = true; });
	cli.add("--gltf-samples-root", "-gr", "Root folder where the glTF 2.0 samples are cloned", ror::CommandLineParser::OptionType::type_string, false,
	        [&gltf_tests_root](std::any a_argument_value) { gltf_tests_root = std::any_cast<std::string>(a_argument_value); });
	cli.add("--gltf-write-shaders", "-gws", "Can write generated shaders to a temporary folder", ror::CommandLineParser::OptionType::none, false,
	        [&gltf_write_shaders](std::any) { gltf_write_shaders = true; });

	if (!cli.execute())
		return 1;

	// Copy assets/configs required for tests
	ror::setup_project_root(test_default_project, "tests");

	ror_test::settings_configs_copy = ror::get_settings();

	if (gltf_test_file != "")
		ror_test::settings_configs_copy.add("gltf_input", gltf_test_file);

	ror_test::settings_configs_copy.add("gltf_compile_shader", gltf_compile_shader);
	ror_test::settings_configs_copy.add("gltf_compile_print_shader", gltf_print_shader);
	ror_test::settings_configs_copy.add("gltf_all", gltf_all);
	ror_test::settings_configs_copy.add("gltf_write_shaders", gltf_write_shaders);
	ror_test::settings_configs_copy.add("gltf_samples_root", gltf_tests_root);

	basist::basisu_transcoder_init();

	return RUN_ALL_TESTS();
}
