#include "common.hpp"
#include "core/rhi/rorbuffer.hpp"
#include "core/rhi/rorbuffers_format.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorprojectroot.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rortypes.hpp"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <iostream>
#include <utility>

namespace ror_test
{
TEST(ConfigurationTest, BufferFormat)
{
	std::filesystem::path file_path{ror::get_project_root().path() / "assets/configs/simple_format.json"};

	ror::BuffersFormatConfig &&bfc{};        // Hack test but this shouldn't happen anyways.
	// Read it in as resource
	bfc.load(file_path);

	// After the following line bfc is in undefined state so shouldn't be used further
	auto                   bf  = static_cast<decltype(bfc)>(bfc).move_buffers_format();        // This magic of static_cast<decltype(bfc)> is casting bfc to r-value reference, alternatively do ror::BuffersFormatConfig&& cast
	const ror::BufferPack &bfp = bf.m_buffer_packs[0];

	EXPECT_EQ(bf.m_unit, 1024);
	EXPECT_EQ(bf.m_current_format, 0);

	const rhi::Buffer &b1 = bfp.m_buffers[0];

	EXPECT_EQ(b1.size(), 1024 * 150);
	EXPECT_EQ(b1.interleaved(), true);

	// The order depends on how json loader loads these

	int pi = -1;
	int wi = -1;
	int ji = -1;

	for (int i = 0; i < 3; ++i)
	{
		auto fmt = b1.semantic(static_cast<size_t>(i));

		if (fmt.first == rhi::BufferSemantic::vertex_position)
			pi = i;

		if (fmt.first == rhi::BufferSemantic::vertex_weight_0)
			wi = i;

		if (fmt.first == rhi::BufferSemantic::vertex_bone_id_0)
			ji = i;
	}

	ASSERT_NE(pi, -1);
	ASSERT_NE(wi, -1);
	ASSERT_NE(ji, -1);

	EXPECT_EQ(b1.semantic(static_cast<size_t>(pi)).first, rhi::BufferSemantic::vertex_position);
	EXPECT_EQ(b1.semantic(static_cast<size_t>(wi)).first, rhi::BufferSemantic::vertex_weight_0);
	EXPECT_EQ(b1.semantic(static_cast<size_t>(ji)).first, rhi::BufferSemantic::vertex_bone_id_0);

	EXPECT_EQ(b1.semantic(static_cast<size_t>(pi)).second, 100);
	EXPECT_EQ(b1.semantic(static_cast<size_t>(wi)).second, 25);
	EXPECT_EQ(b1.semantic(static_cast<size_t>(ji)).second, 25);

	const rhi::Buffer &b2 = bfp.m_buffers[1];

	EXPECT_EQ(b2.size(), 1024 * 200);
	EXPECT_EQ(b2.interleaved(), false);

	// The order depends on how json loader loads these
	int ti = -1;
	int ni = -1;
	int gi = -1;

	for (int i = 0; i < 3; ++i)
	{
		if (b2.semantic(static_cast<size_t>(i)).first == rhi::BufferSemantic::vertex_texture_coord_0)
			ti = i;

		if (b2.semantic(static_cast<size_t>(i)).first == rhi::BufferSemantic::vertex_normal)
			ni = i;

		if (b2.semantic(static_cast<size_t>(i)).first == rhi::BufferSemantic::vertex_tangent)
			gi = i;
	}

	ASSERT_NE(ti, -1);
	ASSERT_NE(ni, -1);
	ASSERT_NE(gi, -1);

	EXPECT_EQ(b2.semantic(static_cast<size_t>(ti)).first, rhi::BufferSemantic::vertex_texture_coord_0);
	EXPECT_EQ(b2.semantic(static_cast<size_t>(ni)).first, rhi::BufferSemantic::vertex_normal);
	EXPECT_EQ(b2.semantic(static_cast<size_t>(gi)).first, rhi::BufferSemantic::vertex_tangent);

	EXPECT_EQ(b2.semantic(static_cast<size_t>(ti)).second, 100);
	EXPECT_EQ(b2.semantic(static_cast<size_t>(ni)).second, 50);
	EXPECT_EQ(b2.semantic(static_cast<size_t>(gi)).second, 50);

	teardown_environment();
}

TEST(ResourcesTest, resouce_load_upload_write)
{
	auto             &parent      = ror::create_resource("my_assets", ror::ResourceSemantic::shaders);
	auto             &temp      = ror::create_resource("my_assets/my_shader.v", ror::ResourceSemantic::shaders);
	auto              file_name = temp.absolute_path().string() + "s";
	std::string       data{"void main{ gl_position = vec4(1.0);}"};
	ror::bytes_vector bytes{data.begin(), data.end()};
	auto              mode = std::ios::ate | std::ios::in;

	{
		auto &shader = ror::create_resource("my_assets/my_shader.vs", ror::ResourceSemantic::shaders);
		shader.update({data.begin(), data.end()});
		if constexpr (ror::get_os() < ror::OsType::os_android)        // This means all Host OSes
		{
			{
				std::ifstream as_file(file_name, mode);
				EXPECT_TRUE(as_file.is_open());        // Although we haven't written this yet, the file should exist empty

				std::streampos bytes_count = as_file.tellg();
				as_file.seekg(0, std::ios::beg);

				ASSERT_TRUE(bytes_count == 0);
				as_file.close();
			}
			{
				shader.flush();
				std::ifstream as_file(file_name, mode);
				ASSERT_TRUE(as_file.is_open());

				std::streampos bytes_count = as_file.tellg();
				as_file.seekg(0, std::ios::beg);

				ASSERT_TRUE(bytes_count > 0);
				ASSERT_TRUE(static_cast<unsigned long>(bytes_count) == bytes.size());

				std::string tmp;
				tmp.resize(bytes.size());
				as_file.read(tmp.data(), bytes_count);        // Weird that int8_t is 'signed char' and can't be converted to 'char'

				EXPECT_EQ(tmp, data);

				as_file.close();
			}
			{
				auto &loaded_shader = ror::load_resource("my_assets/my_shader.vs", ror::ResourceSemantic::shaders);
				EXPECT_EQ(loaded_shader.data(), bytes);
			}
		}

		shader.remove();        // Lets remove the shader created
	}
	{
		auto &shader = ror::create_resource("my_assets/my_shader.vs", ror::ResourceSemantic::shaders);
		shader.update({data.begin(), data.end()});
		shader.flush();
	}

	{
		auto &loaded_shader = ror::load_resource("my_assets/my_shader.vs", ror::ResourceSemantic::shaders);
		EXPECT_EQ(loaded_shader.data(), bytes);
	}
	{
		auto &loaded_shader = ror::load_resource("my_assets/my_shader.vs", ror::ResourceSemantic::shaders);
		EXPECT_EQ(loaded_shader.data(), bytes);
		loaded_shader.update({data.begin(), data.end()}, true); // Append data
		loaded_shader.flush();
	}
	{
		auto &loaded_shader = ror::load_resource("my_assets/my_shader.vs", ror::ResourceSemantic::shaders);
		ror::bytes_vector bytes_local;
		bytes_local.resize(data.size() * 2);

		std::copy(data.begin(), data.end(), bytes_local.begin());
		std::copy(data.begin(), data.end(), bytes_local.begin() + static_cast<long>(data.size()));

		EXPECT_EQ(loaded_shader.data(), bytes_local);

		loaded_shader.remove();
	}

	// Do some cleanup
	temp.remove();
	parent.remove();
}

}        // namespace ror_test
