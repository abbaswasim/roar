#include "common.hpp"
#include "core/rhi/rorbuffer.hpp"
#include "core/rhi/rorbuffers_format.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorprojectroot.hpp"
#include <cstddef>
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <iostream>

namespace ror_test
{
TEST(ConfigurationTest, BufferFormat)
{
	std::filesystem::path file_path{ror::get_project_root().path() / "assets/configs/simple_format.json"};

	ror::BuffersFormatConfig bfc{};
	// Read it in as resource
	bfc.load(file_path);

	auto                   bf  = bfc.buffers_format();
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

		if (fmt.first == rhi::BufferSemantic::vertex_weight)
			wi = i;

		if (fmt.first == rhi::BufferSemantic::vertex_bone_id)
			ji = i;
	}

	ASSERT_NE(pi, -1);
	ASSERT_NE(wi, -1);
	ASSERT_NE(ji, -1);

	EXPECT_EQ(b1.semantic(static_cast<size_t>(pi)).first, rhi::BufferSemantic::vertex_position);
	EXPECT_EQ(b1.semantic(static_cast<size_t>(wi)).first, rhi::BufferSemantic::vertex_weight);
	EXPECT_EQ(b1.semantic(static_cast<size_t>(ji)).first, rhi::BufferSemantic::vertex_bone_id);

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

}        // namespace ror_test
