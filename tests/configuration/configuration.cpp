#include "common.hpp"
#include "core/rhi/rorbuffers_format.hpp"
#include <cstddef>
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <iostream>

namespace ror_test
{
TEST(ConfigurationTest, BufferFormat)
{
	std::string format_config{R"format(
{
	"buffer_pack_current":"optimal",
	"unit": 1024,
	"buffer_packs":[
		{
			"name": "optimal",
			"buffers":[
				{
					"size":150,
					"semantic":[
						{
							"vertex_position":100,
							"vertex_weight":25,
							"vertex_bone_id":25
						}
					]
				},
				{
					"size":200,
					"interleaved":"global",
					"semantic":[
						{
							"vertex_texture_coord_0":100,
							"vertex_normal":50,
							"vertex_tangent":50
						}
					]
				}
			]
		}
	]
})format"};

	auto root_dir = create_root_dir();

	ror::BuffersFormatConfig bfc{};

	std::filesystem::path file_path{root_dir / "buffers_format.json"};

	// First write out input to temp
	write_file(file_path, format_config);

	// Read it back in as resource
	bfc.load(file_path);

	auto                   bf  = bfc.buffers_format();
	const ror::BufferPack &bfp = bf->m_buffer_packs[0];

	EXPECT_EQ(bf->m_unit, 1024);
	EXPECT_EQ(bf->m_current_format, 0);

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

		if (fmt.first == rhi::ShaderSemantic::vertex_position)
			pi = i;

		if (fmt.first == rhi::ShaderSemantic::vertex_weight)
			wi = i;

		if (fmt.first == rhi::ShaderSemantic::vertex_bone_id)
			ji = i;
	}

	ASSERT_NE(pi, -1);
	ASSERT_NE(wi, -1);
	ASSERT_NE(ji, -1);

	EXPECT_EQ(b1.semantic(static_cast<size_t>(pi)).first, rhi::ShaderSemantic::vertex_position);
	EXPECT_EQ(b1.semantic(static_cast<size_t>(wi)).first, rhi::ShaderSemantic::vertex_weight);
	EXPECT_EQ(b1.semantic(static_cast<size_t>(ji)).first, rhi::ShaderSemantic::vertex_bone_id);

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
		if (b2.semantic(static_cast<size_t>(i)).first == rhi::ShaderSemantic::vertex_texture_coord_0)
			ti = i;

		if (b2.semantic(static_cast<size_t>(i)).first == rhi::ShaderSemantic::vertex_normal)
			ni = i;

		if (b2.semantic(static_cast<size_t>(i)).first == rhi::ShaderSemantic::vertex_tangent)
			gi = i;
	}

	ASSERT_NE(ti, -1);
	ASSERT_NE(ni, -1);
	ASSERT_NE(gi, -1);

	EXPECT_EQ(b2.semantic(static_cast<size_t>(ti)).first, rhi::ShaderSemantic::vertex_texture_coord_0);
	EXPECT_EQ(b2.semantic(static_cast<size_t>(ni)).first, rhi::ShaderSemantic::vertex_normal);
	EXPECT_EQ(b2.semantic(static_cast<size_t>(gi)).first, rhi::ShaderSemantic::vertex_tangent);

	EXPECT_EQ(b2.semantic(static_cast<size_t>(ti)).second, 100);
	EXPECT_EQ(b2.semantic(static_cast<size_t>(ni)).second, 50);
	EXPECT_EQ(b2.semantic(static_cast<size_t>(gi)).second, 50);

	teardown_environment();
}

}        // namespace ror_test
