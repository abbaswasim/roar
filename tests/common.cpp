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

void test_one_vertex_description(const rhi::VertexDescriptor &vd,
								 rhi::BufferSemantic    semantic,
								 uint32_t               location,
								 uint32_t               offset,
								 uint64_t               buffer_offset,
								 uint32_t               binding,
								 uint32_t               buffer_index,
								 rhi::VertexFormat      format,

								 rhi::StepFunction function,
								 uint64_t          stride,
								 rhi::Rate         rate,
								 uint32_t          multiplier,
								 uint64_t          semantic_type,
								 uint32_t          line)
{
	(void) buffer_offset;
	(void) buffer_index;
	(void) multiplier;

	// Uncomment to find out the error line
	(void) line;
	// std::string line_header{"Looking at like = "};
	// line_header += std::to_string(line);
	// print_with_gtest_header(line_header.c_str(), green);

	ASSERT_EQ(vd.type(), semantic_type);

	auto lp = vd.layout(semantic);
	auto at = vd.attribute(semantic);

	ASSERT_EQ(lp.binding(), binding);
	ASSERT_EQ(lp.rate(), rate.m_value);
	ASSERT_EQ(lp.step_function(), function);
	ASSERT_EQ(lp.stride(), stride);

	ASSERT_EQ(at.binding(), binding);
	ASSERT_EQ(at.format(), format);
	ASSERT_EQ(at.location(), location);
	ASSERT_EQ(at.offset(), offset);
	ASSERT_EQ(at.semantics(), semantic);
}
}        // namespace ror_test
