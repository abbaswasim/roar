#include "common.hpp"
#include "core/rhi/rorrhi_vertex_description.hpp"
#include "foundation/rorutilities.hpp"
#include "rhi/rorrhi_types.hpp"
#include <cstddef>
#include <foundation/rortypes.hpp>
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <iostream>

namespace ror_test
{
TEST(VertexDescritionTest, automated_description)
{
	static uint32_t position_only             = ror::enum_to_type_cast(rhi::ShaderSemantic::vertex_position);
	static uint32_t position_normal_uv        = ror::enum_to_type_cast(rhi::ShaderSemantic::vertex_position) | ror::enum_to_type_cast(rhi::ShaderSemantic::vertex_texture_coord_0) | ror::enum_to_type_cast(rhi::ShaderSemantic::vertex_normal);
	static uint32_t position_normal_uv_weight = ror::enum_to_type_cast(rhi::ShaderSemantic::vertex_position) | ror::enum_to_type_cast(rhi::ShaderSemantic::vertex_texture_coord_0) | ror::enum_to_type_cast(rhi::ShaderSemantic::vertex_normal) | ror::enum_to_type_cast(rhi::ShaderSemantic::vertex_weight);

	{
		rhi::VertexDescriptor vd{rhi::ShaderSemantic::vertex_position};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), position_only);

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_position);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_position);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_position);
		}
	}

	{
		rhi::VertexDescriptor vd{rhi::ShaderSemantic::vertex_position, rhi::StepFunction::instance};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), position_only);

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_position);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_position);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::instance);
			ASSERT_NE(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_position);
		}
	}

	{
		rhi::VertexDescriptor vd{rhi::ShaderSemantic::vertex_position, rhi::VertexFormat::float32_4};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), position_only);

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_position);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_position);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 4);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_4);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_position);
		}
	}

	{
		rhi::VertexDescriptor vd{rhi::ShaderSemantic::vertex_position, rhi::VertexFormat::float32_4, rhi::StepFunction::instance, 3};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), position_only);

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_position);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_position);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 3);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::instance);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 4);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_4);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_position);
		}
	}

	{
		rhi::VertexDescriptor vd{rhi::ShaderSemantic::vertex_position, rhi::StepFunction::instance, 3};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), position_only);

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_position);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_position);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 3);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::instance);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_position);
		}
	}

	{
		rhi::VertexDescriptor vd{rhi::ShaderSemantic::vertex_position, 3};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), position_only);

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_position);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_position);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 3);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_position);
		}
	}
	{
		// clang-format off
		rhi::VertexDescriptor vd{
			rhi::ShaderSemantic::vertex_position, rhi::StepFunction::vertex,
			rhi::ShaderSemantic::vertex_normal, 2,
			rhi::ShaderSemantic::vertex_texture_coord_0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex,
			rhi::ShaderSemantic::vertex_weight,
			rhi::ShaderSemantic::mesh_index, rhi::StepFunction::instance, 2,
			rhi::ShaderSemantic::instance_transform, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, 1,
		};
		// clang-format on

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), position_normal_uv_weight | ror::enum_to_type_cast(rhi::ShaderSemantic::mesh_index) | ror::enum_to_type_cast(rhi::ShaderSemantic::instance_transform));

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_position);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_position);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3 + sizeof(float32_t) * 3);        // Position + Weight

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_position);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_normal);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_normal);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 1);
			ASSERT_EQ(lp.rate(), 2);        // TODO: This fails
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3 + sizeof(uint16_t) * 2);        // Normal + UV

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 1);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 1);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_normal);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_texture_coord_0);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_texture_coord_0);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 2);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3 + sizeof(uint16_t) * 2);        // Normal + UV

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 2);
			ASSERT_EQ(at.format(), rhi::VertexFormat::uint16_2);
			ASSERT_EQ(at.location(), 2);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_texture_coord_0);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_weight);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_weight);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 3);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3 + sizeof(float32_t) * 3);        // Position + Weight

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 3);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 3);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_weight);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::mesh_index);
			auto at = vd.attribute(rhi::ShaderSemantic::mesh_index);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 4);
			ASSERT_EQ(lp.rate(), 2);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::instance);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3);        // MeshIndex only because globally interleaved

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 4);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 4);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::mesh_index);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::instance_transform);
			auto at = vd.attribute(rhi::ShaderSemantic::instance_transform);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 5);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::instance);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 16);        // MeshIndex + Matrix4x4

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 5);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_16);
			ASSERT_EQ(at.location(), 5);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::instance_transform);
		}
	}

	{
		rhi::VertexDescriptor vd{
			rhi::ShaderSemantic::custom,
			rhi::VertexFormat::float32_custom,
			rhi::StepFunction::vertex,
			1,
		};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), ror::enum_to_type_cast(rhi::ShaderSemantic::custom));

		{
			auto lp = vd.layout(rhi::ShaderSemantic::custom);
			auto at = vd.attribute(rhi::ShaderSemantic::custom);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.format_multiplier(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t));

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_custom);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::custom);
		}
	}

	{
		rhi::VertexDescriptor vd{
			rhi::ShaderSemantic::custom,
			rhi::VertexFormat::float32_custom,
			rhi::StepFunction::vertex,
			(128 << 16) | 1,
		};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), ror::enum_to_type_cast(rhi::ShaderSemantic::custom));

		{
			auto lp = vd.layout(rhi::ShaderSemantic::custom);
			auto at = vd.attribute(rhi::ShaderSemantic::custom);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.format_multiplier(), 128);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 128);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_custom);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::custom);
		}
	}

	{
		rhi::VertexDescriptor vd{
			rhi::ShaderSemantic::custom,
			rhi::VertexFormat::float32_custom,
			rhi::StepFunction::instance,
			(128 << 16) | 2,
		};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), ror::enum_to_type_cast(rhi::ShaderSemantic::custom));

		{
			auto lp = vd.layout(rhi::ShaderSemantic::custom);
			auto at = vd.attribute(rhi::ShaderSemantic::custom);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 2);
			ASSERT_EQ(lp.format_multiplier(), 128);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::instance);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 128);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_custom);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::custom);
		}
	}

	{
		rhi::VertexDescriptor vd{
			rhi::ShaderSemantic::vertex_position,
			rhi::ShaderSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,
			rhi::ShaderSemantic::vertex_normal};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), position_normal_uv);

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_position);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_position);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_position);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_texture_coord_0);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_texture_coord_0);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 1);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 2 + sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 1);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_2);
			ASSERT_EQ(at.location(), 1);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_texture_coord_0);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_normal);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_normal);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 2);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 2 + sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 2);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 2);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_normal);
		}
	}

	{
		rhi::VertexDescriptor vd{
			rhi::ShaderSemantic::vertex_position, rhi::VertexFormat::float32_3,
			rhi::ShaderSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,
			rhi::ShaderSemantic::vertex_normal, rhi::VertexFormat::float32_3};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), position_normal_uv);

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_position);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_position);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_position);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_texture_coord_0);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_texture_coord_0);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 1);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 2 + sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 1);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_2);
			ASSERT_EQ(at.location(), 1);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_texture_coord_0);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_normal);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_normal);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 2);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 2 + sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 2);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 2);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_normal);
		}
	}

	{
		rhi::VertexDescriptor vd{
			rhi::ShaderSemantic::vertex_position, rhi::VertexFormat::float32_3,
			rhi::ShaderSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,
			rhi::ShaderSemantic::vertex_normal, rhi::VertexFormat::float32_3,
			rhi::ShaderSemantic::vertex_index, rhi::VertexFormat::uint32_1};

		ASSERT_TRUE(vd.complete());
		ASSERT_EQ(vd.type(), position_normal_uv | ror::enum_to_type_cast(rhi::ShaderSemantic::vertex_index));

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_position);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_position);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_position);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_texture_coord_0);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_texture_coord_0);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 1);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 2 + sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 1);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_2);
			ASSERT_EQ(at.location(), 1);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_texture_coord_0);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_normal);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_normal);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 2);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 2 + sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 2);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 2);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_normal);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_index);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_index);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 3);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(uint32_t));

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 3);
			ASSERT_EQ(at.format(), rhi::VertexFormat::uint32_1);
			ASSERT_EQ(at.location(), 3);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_index);
		}
	}

	{
		// Testing different formats that takes a location of more than 1
		rhi::VertexDescriptor vd{
			rhi::ShaderSemantic::vertex_position, rhi::VertexFormat::float32_3,
			rhi::ShaderSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_9,
			rhi::ShaderSemantic::vertex_normal, rhi::VertexFormat::float32_3,
			rhi::ShaderSemantic::vertex_texture_coord_1, rhi::VertexFormat::float32_9,
			rhi::ShaderSemantic::vertex_texture_coord_2, rhi::VertexFormat::float32_2,
			rhi::ShaderSemantic::vertex_index, rhi::VertexFormat::uint32_1};

		ASSERT_TRUE(vd.complete());
		// ASSERT_EQ(vd.type(), position_normal_uv | ror::enum_to_type_cast(rhi::ShaderSemantic::vertex_index));

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_position);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_position);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 0);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 0);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 0);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_position);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_texture_coord_0);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_texture_coord_0);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 1);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 9 + sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 1);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_9);
			ASSERT_EQ(at.location(), 1);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_texture_coord_0);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_normal);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_normal);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 2);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 9 + sizeof(float32_t) * 3);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 2);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_3);
			ASSERT_EQ(at.location(), 4);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_normal);
		}
		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_texture_coord_1);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_texture_coord_1);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 3);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 9);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 3);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_9);
			ASSERT_EQ(at.location(), 5);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_texture_coord_1);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_texture_coord_2);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_texture_coord_2);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 4);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(float32_t) * 2);

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 4);
			ASSERT_EQ(at.format(), rhi::VertexFormat::float32_2);
			ASSERT_EQ(at.location(), 8);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_texture_coord_2);
		}

		{
			auto lp = vd.layout(rhi::ShaderSemantic::vertex_index);
			auto at = vd.attribute(rhi::ShaderSemantic::vertex_index);

			ASSERT_TRUE(lp.complete());
			ASSERT_EQ(lp.binding(), 5);
			ASSERT_EQ(lp.rate(), 1);
			ASSERT_EQ(lp.step_function(), rhi::StepFunction::vertex);
			ASSERT_EQ(lp.stride(), sizeof(uint32_t));

			ASSERT_TRUE(at.complete());
			ASSERT_EQ(at.binding(), 5);
			ASSERT_EQ(at.format(), rhi::VertexFormat::uint32_1);
			ASSERT_EQ(at.location(), 9);
			ASSERT_EQ(at.offset(), 0);
			ASSERT_EQ(at.semantics(), rhi::ShaderSemantic::vertex_index);
		}
	}
}
}        // namespace ror_test
