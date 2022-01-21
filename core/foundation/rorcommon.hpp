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

#pragma once

#include "foundation/rorsystem.hpp"
#include <iostream>

namespace ror
{
static std::string get_os_string()
{
	OsType os_type = get_os();

	switch (os_type)
	{
		case OsType::os_mac:
			return std::string("OsType::os_mac");
		case OsType::os_linux:
			return std::string("OsType::os_linux");
		case OsType::os_ios:
			return std::string("OsType::os_ios");
		case OsType::os_ios_simulator:
			return std::string("OsType::os_ios_simulator");
		case OsType::os_android:
			return std::string("OsType::os_android");
		case OsType::os_windows:
			return std::string("OsType::os_windows");
	}

	return std::string("OsType::os_unknown");
}

static std::string get_arch_string()
{
	ArchType arch_type = get_arch();

	switch (arch_type)
	{
		case ArchType::arch_arm:
			return std::string("ArchType::arch_arm");
		case ArchType::arch_arm64:
			return std::string("ArchType::arch_arm64");
		case ArchType::arch_x86:
			return std::string("ArchType::arch_x86");
		case ArchType::arch_x86_64:
			return std::string("ArchType::arch_x86_64");
	}
	return std::string("ArchType::arch_unknown");
}

static std::string get_compiler_string()
{
	CompilerType compiler_type = get_compiler();

	switch (compiler_type)
	{
		case CompilerType::comp_gcc:
			return std::string("CompilerType::comp_gcc");
		case CompilerType::comp_clang:
			return std::string("CompilerType::comp_clang");
		case CompilerType::comp_msvc:
			return std::string("CompilerType::comp_msvc");
	}
	return std::string("CompilerType::comp_unknown");
}

static std::string get_build_string()
{
	BuildType build_type = get_build();

	switch (build_type)
	{
		case BuildType::build_release:
			return std::string("BuildType::build_release");
		case BuildType::build_debug:
			return std::string("BuildType::build_debug");
	}
	return std::string("BuildType::build_unknown");
}

}        // namespace ror
