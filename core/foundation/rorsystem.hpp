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

#if defined(__APPLE__) && defined(__MACH__)
#	include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR == 1
#		define OS_TYPE_IOS_SIMULATOR
#	elif TARGET_OS_IPHONE == 1
#		define OS_TYPE_IOS
#	elif TARGET_OS_MAC == 1
#		define OS_TYPE_MAC
#	endif
#elif defined(__ANDROID__) || defined(ANDROID)
#	define OS_TYPE_ANDROID
#elif defined(__linux__)
#	define OS_TYPE_LINUX
#elif defined(_WIN32)
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN        // This trims down the windows libraries.
#	endif
#	ifndef WIN32_EXTRA_LEAN
#		define WIN32_EXTRA_LEAN        // This Trims it even farther.
#	endif
#	include <windows.h>
#	define OS_TYPE_WINDOWS
#else
#	error "OS not supported"
#endif

#if defined(__i386__)
#	define ARCH_TYPE_X86
#elif defined(__x86_64__)
#	define ARCH_TYPE_X86_64
#elif defined(__arm__)
#	define ARCH_TYPE_ARM
#elif defined(__aarch64__)
#	define ARCH_TYPE_ARM64
#else
#	error "Architecture not supported"
#endif

#if defined(__clang__)
#	define COMPILER_TYPE_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#	define COMPILER_TYPE_GCC
#elif defined(_MSVC)
#	define COMPILER_TYPE_MSVC
#else
#	error "Compiler not supported"
#endif

#if defined(RORDEBUG)
#	define BUILD_TYPE_DEBUG
#else
#	define BUILD_TYPE_RELEASE
#endif

namespace ror
{
enum class OsType
{
	os_unknown,
	os_mac,
	os_android,
	os_ios,
	os_ios_simulator,
	os_linux,
	os_bsd,
	os_windows,
	os_max
};

enum class ArchType
{
	arch_unknown,
	arch_arm,
	arch_arm64,
	arch_x86,
	arch_x86_64,
	arch_max
};

enum class CompilerType
{
	comp_unknown,
	comp_gcc,
	comp_clang,
	comp_msvc,
	comp_max
};

enum class BuildType
{
	build_unknown,
	build_release,
	build_debug,
	build_max
};

constexpr OsType get_os()
{
	OsType os_type = OsType::os_unknown;

#ifdef OS_TYPE_MAC
	os_type = OsType::os_mac;
#elif defined OS_TYPE_ANDROID
	os_type = OsType::os_android;
#elif defined OS_TYPE_IOS
	os_type = OsType::os_ios;
#elif defined OS_TYPE_IOS_SIMULATOR
	os_type = OsType::os_ios_simulator;
#elif defined OS_TYPE_LINUX        // Linux means Linux desktop not Android linux
	os_type = OsType::os_linux;
#elif defined OS_TYPE_BSD
	os_type = OsType::os_bsd;
#elif defined OS_TYPE_WINDOWS
	os_type = OsType::os_windows;
#endif

	return os_type;
}

constexpr ArchType get_arch()
{
	ArchType arch_type = ArchType::arch_unknown;

#ifdef ARCH_TYPE_ARM
	arch_type = ArchType::arch_arm;
#elif defined ARCH_TYPE_ARM64
	arch_type = ArchType::arch_arm64;
#elif defined ARCH_TYPE_X86
	arch_type = ArchType::arch_x86;
#elif defined ARCH_TYPE_X86_64
	arch_type = ArchType::arch_x86_64;
#endif

	return arch_type;
}

constexpr CompilerType get_compiler()
{
	CompilerType compiler_type = CompilerType::comp_unknown;

#ifdef COMPILER_TYPE_GCC
	compiler_type = CompilerType::comp_gcc;
#elif defined COMPILER_TYPE_CLANG
	compiler_type = CompilerType::comp_clang;
#elif defined COMPILER_TYPE_MSVC
	compiler_type = CompilerType::comp_msvc;
#endif

	return compiler_type;
}

constexpr BuildType get_build()
{
	BuildType build_type = BuildType::build_unknown;

#ifdef BUILD_TYPE_RELEASE
	build_type = BuildType::build_release;
#elif defined BUILD_TYPE_DEBUG
	build_type = BuildType::build_debug;
#endif

	return build_type;
}

}        // namespace ror
