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
#include "configuration/rorsettings_configuration.hpp"
#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "graphics/rorscene.hpp"
#include "platform/rorapplication.hpp"
#include "project_setup.hpp"
#include "resources/rorprojectroot.hpp"
#include "roreditor.hpp"
#include <any>
#include <iostream>

template <typename _type>
void app_run(ror::Application<_type> &a_application)
{
	a_application.run();
}

int main(int argc, char *argv[])
{
	std::string editor_default_project{"editor_default_project"};
	std::string editor_default_scene{"default_scene.json"};

	ror::CommandLineParser cli{std::vector<std::string>{argv, argv + argc}};

	cli.add("--default-project", "-dp", "Default project name for the editor", ror::CommandLineParser::OptionType::type_string, true,
			[&editor_default_project](std::any a_argument_value) { editor_default_project = std::any_cast<std::string>(a_argument_value); });
	cli.add("--default-scene", "-ds", "Default scene to load from the project folder", ror::CommandLineParser::OptionType::type_string, false,
			[&editor_default_scene](std::any a_argument_value) { editor_default_scene = std::any_cast<std::string>(a_argument_value); });

	if (!cli.execute())
		return 1;


	ror::setup_project_root(editor_default_project, "editor");
	auto &bp = rhi::get_buffers_pack();

	ror::Scene scene{editor_default_scene};

	ror::EventSystem event_system;

	// TODO: Make sure this is platform agnostic
	// Need to create different type of apps and then
	// dispatch those to different threads if want to run at the same time
	// Otherwise just run one or the other
	ror::MacOSApp app1(event_system);
	// ror::MacOSApp app2;


	try
	{
		app_run(app1);
		// app_run(app2);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	bp.free();

	return 0;
}
