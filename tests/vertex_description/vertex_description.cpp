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
#include "foundation/rorhash.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/rormesh.hpp"
#include "include/gtest/gtest.h"
#include "profiling/rorlog.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_attribute.hpp"
#include "rhi/rorvertex_description.hpp"
#include "rhi/rorvertex_layout.hpp"
#include <cstddef>
#include <foundation/rortypes.hpp>
#include <iostream>

namespace ror_test
{
static auto position_only             = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position);
static auto position_uv               = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0);
static auto position_normal_uv        = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_normal);
static auto position_normal_uv_weight = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_normal) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_weight_0);

TEST(VertexDescriptionTest, automated_description)
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
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, sizeof(uint16_t) * 2, 0, 2, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::instance, sizeof(uint16_t) * 2 + sizeof(float32_t) * 4, 3, 1, position_normal_uv, __LINE__);
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
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::StepFunction::vertex,
								 rhi::BufferSemantic::vertex_normal, 2,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex,
								 rhi::BufferSemantic::vertex_weight_0,
								 rhi::BufferSemantic::mesh_index, rhi::StepFunction::instance, 2,
								 rhi::BufferSemantic::instance_transform, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, 1, &bp};

		auto semantics = position_normal_uv_weight |
						 ror::enum_to_type_cast(rhi::BufferSemantic::mesh_index) |
						 ror::enum_to_type_cast(rhi::BufferSemantic::instance_transform);

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 2, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 2, sizeof(float32_t) * 3, 0, 2, 0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, sizeof(float32_t) * 3, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::mesh_index, 4, 0, 0, 4, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::instance, sizeof(float32_t) * 3, 2, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);

		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::instance_transform, 5, 0, 0, 5, 0, rhi::VertexFormat::float32_16, rhi::StepFunction::instance, sizeof(float32_t) * 16, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::StepFunction::vertex,
								 rhi::BufferSemantic::vertex_normal, 2,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, &bp};

		auto semantics = position_normal_uv;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 2, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 2, sizeof(float32_t) * 3, 0, 2, 0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 1, 1, semantics, __LINE__);

		semantics = position_normal_uv_weight;

		vd.add(rhi::BufferSemantic::vertex_weight_0, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, sizeof(float32_t) * 3, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);

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
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::StepFunction::vertex,
								 rhi::BufferSemantic::vertex_normal, 2,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, &bp};

		auto semantics = position_normal_uv;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 2, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 2, sizeof(float32_t) * 3, 0, 2, 0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 1, 1, semantics, __LINE__);

		semantics = position_normal_uv_weight;

		rhi::VertexAttribute va0{3, 0, 0, 3, 0, rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_3};
		rhi::VertexLayout    vl0{3, sizeof(float32_t) * 6, 1};

		vd.add(va0, vl0, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, sizeof(float32_t) * 3, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);

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
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 2, sizeof(float32_t) * 3, 0, 2, 0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, sizeof(float32_t) * 3, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
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
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 2, sizeof(float32_t) * 3, 0, 2, 0, rhi::VertexFormat::uint16_2, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 2, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, sizeof(float32_t) * 3, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
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
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, sizeof(float32_t) * 2, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, sizeof(float32_t) * 2, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
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
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 2, sizeof(float32_t) * 3, 0, 2, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 3, sizeof(float32_t) * 2, 0, 3, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,
								 rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3, &bp};

		auto semantics = position_normal_uv;

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, sizeof(float32_t) * 2, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, sizeof(float32_t) * 2, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,
								 rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3,
								 rhi::BufferSemantic::vertex_index, rhi::VertexFormat::uint32_1, &bp};

		auto semantics = position_normal_uv | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_index);

		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 0, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 2, sizeof(float32_t) * 2, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 2 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
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
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 4, sizeof(float32_t) * 9, 0, 2, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 9 + sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_1, 5, 0, 0, 3, 0, rhi::VertexFormat::float32_9, rhi::StepFunction::vertex, sizeof(float32_t) * 9, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_2, 8, 0, 0, 4, 0, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2, 1, 1, semantics, __LINE__);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_index, 9, 0, 0, 5, 0, rhi::VertexFormat::uint32_1, rhi::StepFunction::vertex, sizeof(uint32_t), 1, 1, semantics, __LINE__);
		vd.add();
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_index, 9, 0, 0, 5, 0, rhi::VertexFormat::uint32_1, rhi::StepFunction::vertex, sizeof(uint32_t), 1, 1, semantics, __LINE__);
	}
}

TEST(VertexDescriptionTest, fox_attributes_description)
{
	const auto &bp = rhi::get_buffers_pack();

	// type, comps, offset, count, stride/byte_size data_size
	// f32,   vec3,      0,   1728,    12               20736
	// f32,   vec2,      0,   1728,    8                13824
	// r16u,  vec4,  13824,   1728,    8                13824
	// f32,   vec4,      0,   1728,    16               27648

	// rhi::BufferSemantic    semantic, uint32_t               location, uint32_t               offset, uint64_t               buffer_offset, uint32_t               binding, uint32_t               buffer_index, rhi::VertexFormat      format, rhi::StepFunction function, uint64_t          stride, rhi::Rate         rate, uint32_t          multiplier, uint64_t          semantic_type, uint32_t          line)
	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3,               // 20736, 12
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,        // 13824, 8
								 rhi::BufferSemantic::vertex_bone_id_0, rhi::VertexFormat::uint16_4,               // 13824, 8
								 rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_4,               // 27648, 16
								 &bp};
	}
	{
		rhi::VertexDescriptor vd{};

		auto semantics = position_only;

		// location, offset, buffer_offset, binding, buffer_index, format, function, stride, rate, multiplier

		vd.add(rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);

		semantics |= ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0);
		vd.add(rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 1, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2, 1, 1, semantics, __LINE__);

		semantics |= ror::enum_to_type_cast(rhi::BufferSemantic::vertex_bone_id_0);
		vd.add(rhi::BufferSemantic::vertex_bone_id_0, rhi::VertexFormat::uint16_4, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_bone_id_0, 2, sizeof(float32_t) * 3, 0, 2, 0, rhi::VertexFormat::uint16_4, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 4, 1, 1, semantics, __LINE__);

		semantics |= ror::enum_to_type_cast(rhi::BufferSemantic::vertex_weight_0);
		vd.add(rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_4, &bp);
		test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, sizeof(float32_t) * 3 + sizeof(uint16_t) * 4, 0, 3, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(uint16_t) * 4 + sizeof(float32_t) * 4, 1, 1, semantics, __LINE__);
	}
}

TEST(VertexDescriptionTest, attributes_descriptor_hasher)
{
	const auto &bp = rhi::get_buffers_pack();

	hash_64_t hash0{0};
	hash_64_t hash1{0};
	hash_64_t hash2{0};
	hash_64_t hash3{0};

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_4,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_4,
								 rhi::BufferSemantic::vertex_bone_id_0, rhi::VertexFormat::uint16_4,
								 rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_4,
								 &bp};

		// Calculating hash once outside to confirm it doesn't change
		const auto attribs = vd.attributes();
		const auto type    = vd.type();

		ror::hash_stream_64(nullptr, -1);                                                    // Init streamer
		ror::hash_stream_64(reinterpret_cast<const void *>(&type), sizeof(uint64_t));        // Lets start with the type

		for (auto &attrib : attribs)
		{
			const auto format = attrib.format();
			ror::hash_stream_64(reinterpret_cast<const void *>(&format), sizeof(uint32_t));        // Now lets add all the formats
		}

		auto temp_hash = ror::hash_stream_64(nullptr, 0);

		hash0       = vd.hash_64();
		auto hash00 = vd.hash_64();

		EXPECT_EQ(temp_hash, temp_hash);
		EXPECT_EQ(hash0, hash00);
	}

	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,
								 rhi::BufferSemantic::vertex_bone_id_0, rhi::VertexFormat::uint16_4,
								 rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_4,
								 &bp};

		hash1 = vd.hash_64();
	}
	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_4,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,
								 rhi::BufferSemantic::vertex_bone_id_0, rhi::VertexFormat::float32_4,
								 rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_4,
								 &bp};

		hash2 = vd.hash_64();
	}
	{
		rhi::VertexDescriptor vd{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3,
								 rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2,
								 rhi::BufferSemantic::vertex_bone_id_0, rhi::VertexFormat::uint16_4,
								 &bp};

		hash3 = vd.hash_64();
	}

	// Check there are no collisions
	EXPECT_NE(hash0, hash1);
	EXPECT_NE(hash0, hash2);
	EXPECT_NE(hash0, hash3);

	EXPECT_NE(hash1, hash2);
	EXPECT_NE(hash1, hash3);

	EXPECT_NE(hash3, hash2);
}

}        // namespace ror_test
