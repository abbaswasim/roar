#include "common.hpp"
#include "foundation/rorcommon.hpp"
#include "foundation/rorjobsystem.hpp"
#include <gtest/gtest.h>
#include <iostream>

namespace ror_test
{
void print_with_gtest_header(const char *txt, const std::string &color)
{
	printf("\033[0;3%sm", color.c_str());
	printf("%s", "[----------] ");
	printf("\033[m");        // Resets the terminal to default.
	printf("%s\n", txt);
}

void print_gtest_header(const std::string &color)
{
	printf("\033[0;3%sm", color.c_str());
	printf("%s", "[----------] ");
	printf("\033[m");        // Resets the terminal to default.
}

fs::path get_root_dir()
{
	fs::current_path(fs::temp_directory_path());
	std::filesystem::path root("sandbox");
	return fs::current_path() / root;
}

fs::path create_root_dir()
{
	auto rd = get_root_dir();
	fs::create_directories(rd);
	return rd;
}

void teardown_environment()
{
	auto root_dir = get_root_dir();
	fs::remove_all(root_dir);
}

void write_file(fs::path file, std::string data)
{
	std::ofstream f(file);
	if (!f.is_open())
	{
		FAIL();
	}

	f.write(&data[0], static_cast<long>(data.size()));

	f.close();
}

TEST(System, System_constants)
{
	auto os    = ror::get_os();
	auto arch  = ror::get_arch();
	auto comp  = ror::get_compiler();
	auto build = ror::get_build();

	switch (os)
	{
		case ror::OsType::os_mac:
			print_with_gtest_header("Building for mac", green);
			break;
		case ror::OsType::os_android:
			print_with_gtest_header("Building for android", green);
			break;
		case ror::OsType::os_ios:
			print_with_gtest_header("Building for ios", green);
			break;
		case ror::OsType::os_ios_simulator:
			print_with_gtest_header("Building for ios simulator", green);
			break;
		case ror::OsType::os_linux:
			print_with_gtest_header("Building for linux", green);
			break;
		case ror::OsType::os_bsd:
			print_with_gtest_header("Building for bsd", green);
			break;
		case ror::OsType::os_windows:
			print_with_gtest_header("Building for windows", green);
			break;
	}

	print_with_gtest_header(ror::get_os_string().c_str(), green);

	switch (arch)
	{
		case ror::ArchType::arch_arm:
			print_with_gtest_header("Building for arm", green);
			break;
		case ror::ArchType::arch_arm64:
			print_with_gtest_header("Building for arm64", green);
			break;
		case ror::ArchType::arch_x86:
			print_with_gtest_header("Building for x86", green);
			break;
		case ror::ArchType::arch_x86_64:
			print_with_gtest_header("Building for x86_64", green);
			break;
	}

	print_with_gtest_header(ror::get_arch_string().c_str(), green);

	switch (comp)
	{
		case ror::CompilerType::comp_gcc:
			print_with_gtest_header("Building with gcc", green);
			break;
		case ror::CompilerType::comp_clang:
			print_with_gtest_header("Building with clang", green);
			break;
		case ror::CompilerType::comp_msvc:
			print_with_gtest_header("Building with msvc", green);
			break;
	}

	print_with_gtest_header(ror::get_compiler_string().c_str(), green);

	switch (build)
	{
		case ror::BuildType::build_release:
			print_with_gtest_header("Building in release", green);
			break;
		case ror::BuildType::build_debug:
			print_with_gtest_header("Building in debug", green);
			break;
	}

	print_with_gtest_header(ror::get_build_string().c_str(), green);
}

TEST(JobSystem, JobSystem_hardware_threads)
{
	uint32_t threads          = ror::get_hardware_threads();
	uint32_t expected_threads = 15;
	EXPECT_EQ(threads, expected_threads) << "Not enough threads available";
}

}        // namespace ror_test
