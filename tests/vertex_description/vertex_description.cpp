// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

#include "common.hpp"
#include "graphics/rormesh.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorvertex_description.hpp"
#include "foundation/rorutilities.hpp"
#include "rhi/rorvertex_attribute.hpp"
#include "rhi/rorvertex_layout.hpp"
#include "rhi/rortypes.hpp"
#include <cstddef>
#include <foundation/rortypes.hpp>
#include "include/gtest/gtest.h"
#include <iostream>

namespace ror_test
{
static auto position_only             = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position);
static auto position_uv               = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0);
static auto position_normal_uv        = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_normal);
static auto position_normal_uv_weight = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_normal) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_weight_0);

void test_one_vertex_description(rhi::VertexDescriptor &vd,
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

	ASSERT_TRUE(vd.complete());
	ASSERT_EQ(vd.type(), semantic_type);

	auto lp = vd.layout(semantic);
	auto at = vd.attribute(semantic);

	ASSERT_TRUE(lp.complete());
	ASSERT_EQ(lp.binding(), binding);
	ASSERT_EQ(lp.rate(), rate.m_value);
	ASSERT_EQ(lp.step_function(), function);
	ASSERT_EQ(lp.stride(), stride);

	ASSERT_TRUE(at.complete());
	ASSERT_EQ(at.binding(), binding);
	ASSERT_EQ(at.format(), format);
	ASSERT_EQ(at.location(), location);
	ASSERT_EQ(at.offset(), offset);
	ASSERT_EQ(at.semantics(), semantic);
}

TEST(VertexDescritionTest, automated_description)
{
	// TODO: Also check other ShaderSemantics added recently (Animation, texture etc)
	const auto &bp = rhi::get_buffers_pack();

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, &bp};
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, position_only, __LINE__);

		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, position_only, __LINE__);

		vd.add(rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::uint16_2, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, sizeof(uint16_t) * 2, 1, 1, position_uv, __LINE__);

		vd.add(rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_4, rhi::StepFunction::instance, 3, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, 0, 0, 2, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::instance, sizeof(uint16_t) * 2 + sizeof(float32_t) * 4, 3, 1, position_normal_uv, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::StepFunction::instance, &bp};

		auto semantics = position_only;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::instance, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::instance, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_4, &bp};

		auto semantics = position_only;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::vertex, sizeof(float32_t) * 4, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::vertex, sizeof(float32_t) * 4, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_4, rhi::StepFunction::instance, 3, &bp};

		auto semantics = position_only;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::instance, sizeof(float32_t) * 4, 3, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::instance, sizeof(float32_t) * 4, 3, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::StepFunction::instance, 3, &bp};

		auto semantics = position_only;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::instance, sizeof(float32_t) * 3, 3, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::instance, sizeof(float32_t) * 3, 3, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, 3, &bp};

		auto semantics = position_only;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 3, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 3, 1, semantics, __LINE__);
	}

	{
		// clang-format off
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::StepFunction::vertex,
			rhi::BufferSemantic::vertex_normal, 2,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex,
								 rhi::BufferSemantic::vertex_weight_0,
								 rhi::BufferSemantic::mesh_index, rhi::StepFunction::instance, 2,
								 rhi::BufferSemantic::instance_transform, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, 1, &bp};
		// clang-format on

		auto semantics = position_normal_uv_weight |
							 ror::enum_to_type_cast(rhi::BufferSemantic::mesh_index) |
							 ror::enum_to_type_cast(rhi::BufferSemantic::instance_transform);

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 2, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 2, 0, 0, 2, 0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, 0, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::mesh_index, 4, 0, 0, 4, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::instance, sizeof(float32_t) * 3, 2, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);

		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);
	}

	{
		// clang-format off
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::StepFunction::vertex,
			rhi::BufferSemantic::vertex_normal, 2,
			rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, &bp};
		// clang-format on

		auto semantics = position_normal_uv;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 2, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 2, 0, 0, 2, 0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 1, 1, semantics, __LINE__);

		semantics = position_normal_uv_weight;

		vd.add(rhi::BufferSemantic::vertex_weight_0, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, 0, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);

		semantics = position_normal_uv_weight | ror::enum_to_type_cast(rhi::BufferSemantic::mesh_index);
		vd.add(rhi::BufferSemantic::mesh_index, rhi::StepFunction::instance, 2, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::mesh_index, 4, 0, 0, 4, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::instance, sizeof(float32_t) * 3, 2, 1, semantics, __LINE__);

		semantics = position_normal_uv_weight |
					ror::enum_to_type_cast(rhi::BufferSemantic::mesh_index) |
					ror::enum_to_type_cast(rhi::BufferSemantic::instance_transform);
		vd.add(rhi::BufferSemantic::instance_transform, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, 1, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);

		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);
	}

	{
		// clang-format off
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::StepFunction::vertex,
			rhi::BufferSemantic::vertex_normal, 2,
			rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, &bp};
		// clang-format on

		auto semantics = position_normal_uv;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 2, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 2, 0, 0, 2, 0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 1, 1, semantics, __LINE__);

		semantics = position_normal_uv_weight;

		rhi::VertexAttribute va0{3, 0, 0, 3, 0, rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_3};
		rhi::VertexLayout    vl0{3, sizeof(float32_t) * 6, 1};

		vd.add(va0, vl0, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, 0, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);

		semantics = position_normal_uv_weight | ror::enum_to_type_cast(rhi::BufferSemantic::mesh_index);

		rhi::VertexAttribute va1{4, 0, 0, 4, 0, rhi::BufferSemantic::mesh_index, rhi::VertexFormat::float32_3};
		rhi::VertexLayout    vl1{3, sizeof(float32_t) * 3, 2, 1, rhi::StepFunction::instance};        // Notice the binding=3 isn't used

		vd.add(va1, vl1, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::mesh_index, 4, 0, 0, 4, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::instance, sizeof(float32_t) * 3, 2, 1, semantics, __LINE__);

		semantics = position_normal_uv_weight |
					ror::enum_to_type_cast(rhi::BufferSemantic::mesh_index) |
					ror::enum_to_type_cast(rhi::BufferSemantic::instance_transform);

		rhi::VertexAttribute va2{5, 0, 0, 5, 0, rhi::BufferSemantic::instance_transform, rhi::VertexFormat::float32_16};
		rhi::VertexLayout    vl2{5, sizeof(float32_t) * 16, 1, 1, rhi::StepFunction::instance};

		vd.add(va2, vl2, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);

		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{};

		rhi::VertexAttribute va{};
		rhi::VertexLayout    vl{};
		rhi::VertexAttribute va0{1, 0, 0, 1, 0, rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3};
		rhi::VertexLayout    vl0{0, 0, 2};
		rhi::VertexAttribute va1{2, 0, 0, 2, 0, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::uint16_2};
		rhi::VertexLayout    vl1{};
		rhi::VertexAttribute va2{3, 0, 0, 3, 0, rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_3};
		rhi::VertexLayout    vl2{};
		rhi::VertexAttribute va3{4, 0, 0, 4, 0, rhi::BufferSemantic::mesh_index, rhi::VertexFormat::float32_3};
		rhi::VertexLayout    vl3{3, 0, 2, 1, rhi::StepFunction::instance};        // Notice the binding=3 isn't used
		rhi::VertexAttribute va4{5, 0, 0, 5, 0, rhi::BufferSemantic::instance_transform, rhi::VertexFormat::float32_16};
		rhi::VertexLayout    vl4{5, 0, 1, 1, rhi::StepFunction::instance};

		std::vector<rhi::VertexAttribute> attributes{va, va0, va1, va2, va3, va4};
		std::vector<rhi::VertexLayout>    layouts{vl, vl0, vl1, vl2, vl3, vl4};

		auto semantics = position_normal_uv_weight |
							 ror::enum_to_type_cast(rhi::BufferSemantic::mesh_index) |
							 ror::enum_to_type_cast(rhi::BufferSemantic::instance_transform);

		vd.add(attributes, layouts, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 2, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 2, 0, 0, 2, 0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, 0, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::mesh_index, 4, 0, 0, 4, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::instance, sizeof(float32_t) * 3, 2, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);

		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{};

		rhi::VertexAttribute va{};
		rhi::VertexLayout    vl{};
		rhi::VertexAttribute va0{1, 0, 0, 1, 0, rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3};
		rhi::VertexLayout    vl0{0, 0, 2};
		rhi::VertexAttribute va1{2, 0, 0, 2, 0, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::uint16_2};
		rhi::VertexLayout    vl1{};
		rhi::VertexAttribute va2{3, 0, 0, 3, 0, rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_3};
		rhi::VertexLayout    vl2{};
		rhi::VertexAttribute va3{4, 0, 0, 4, 0, rhi::BufferSemantic::custom, rhi::VertexFormat::uint16_custom};
		rhi::VertexLayout    vl3{3, 24, 2, 3, rhi::StepFunction::instance};        // Notice the binding=3 isn't used
		rhi::VertexAttribute va4{5, 0, 0, 5, 0, rhi::BufferSemantic::instance_transform, rhi::VertexFormat::float32_16};
		rhi::VertexLayout    vl4{5, 0, 1, 1, rhi::StepFunction::instance};

		std::vector<rhi::VertexAttribute> attributes{va, va0, va1, va2, va3, va4};
		std::vector<rhi::VertexLayout>    layouts{vl, vl0, vl1, vl2, vl3, vl4};

		auto semantics = position_normal_uv_weight |
							 ror::enum_to_type_cast(rhi::BufferSemantic::custom) |
							 ror::enum_to_type_cast(rhi::BufferSemantic::instance_transform);

		vd.add(attributes, layouts, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 6, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 2, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 2, 0, 0, 2, 0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, 0, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::custom, 4, 0, 0, 4, 0, rhi::VertexFormat::uint16_custom, rhi::StepFunction::instance, sizeof(uint16_t) * 3, 2, 3, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{
			rhi::BufferSemantic::custom,
			rhi::VertexFormat::float32_custom,
			rhi::StepFunction::vertex,
			1, &bp};

		auto semantics = ror::enum_to_type_cast(rhi::BufferSemantic::custom);

		test_one_vertex_description(vd, rhi::BufferSemantic::custom, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_custom, rhi::StepFunction::vertex, sizeof(float32_t), 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::custom, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_custom, rhi::StepFunction::vertex, sizeof(float32_t), 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{
			rhi::BufferSemantic::custom,
			rhi::VertexFormat::float32_custom,
			rhi::StepFunction::vertex,
			(128 << 16) | 1, &bp};

		auto semantics = ror::enum_to_type_cast(rhi::BufferSemantic::custom);

		test_one_vertex_description(vd, rhi::BufferSemantic::custom, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_custom, rhi::StepFunction::vertex, sizeof(float32_t) * 128, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::custom, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_custom, rhi::StepFunction::vertex, sizeof(float32_t) * 128, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{
			rhi::BufferSemantic::custom,
			rhi::VertexFormat::float32_custom,
			rhi::StepFunction::instance,
			(128 << 16) | 2, &bp};

		auto semantics = ror::enum_to_type_cast(rhi::BufferSemantic::custom);

		test_one_vertex_description(vd, rhi::BufferSemantic::custom, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_custom, rhi::StepFunction::instance, sizeof(float32_t) * 128, 2, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::custom, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_custom, rhi::StepFunction::instance, sizeof(float32_t) * 128, 2, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,
								 rhi::BufferSemantic::vertex_normal, &bp};

		auto semantics = position_normal_uv;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, 0, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, 0, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2, &bp};

		auto semantics = position_normal_uv_weight;

		// Lets add the following now and test if older layout strides are updated or not
		vd.add(rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_4, &bp);
		vd.add(rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3, &bp);

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 2, 0, 0, 2, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 3, 0, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,
								 rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3, &bp};

		auto semantics = position_normal_uv;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, 0, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, 0, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,
								 rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3,
								 rhi::BufferSemantic::vertex_index, rhi::VertexFormat::uint32_1, &bp};

		auto semantics = position_normal_uv | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_index);

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, 0, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_index, 3, 0, 0, 3, 0, rhi::VertexFormat::uint32_1, rhi::StepFunction::vertex, sizeof(uint32_t), 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_index, 3, 0, 0, 3, 0, rhi::VertexFormat::uint32_1, rhi::StepFunction::vertex, sizeof(uint32_t), 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_9,
								 rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3,
								 rhi::BufferSemantic::vertex_texture_coord_1, rhi::VertexFormat::float32_9,
								 rhi::BufferSemantic::vertex_texture_coord_2, rhi::VertexFormat::float32_2,
								 rhi::BufferSemantic::vertex_index, rhi::VertexFormat::uint32_1, &bp};

		uint32_t semantics = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position) |
							 ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0) |
							 ror::enum_to_type_cast(rhi::BufferSemantic::vertex_normal) |
							 ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_1) |
							 ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_2) |
							 ror::enum_to_type_cast(rhi::BufferSemantic::vertex_index);

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_9, rhi::StepFunction::vertex, sizeof(float32_t) * 9 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 4, 0, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 9 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_1, 5, 0, 0, 3, 0, rhi::VertexFormat::float32_9, rhi::StepFunction::vertex, sizeof(float32_t) * 9, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_2, 8, 0, 0, 4, 0, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_index, 9, 0, 0, 5, 0, rhi::VertexFormat::uint32_1, rhi::StepFunction::vertex, sizeof(uint32_t), 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_index, 9, 0, 0, 5, 0, rhi::VertexFormat::uint32_1, rhi::StepFunction::vertex, sizeof(uint32_t), 1, 1, semantics, __LINE__);
	}
}


}        // namespace ror_test
