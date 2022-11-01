#include "profiling/rorlog.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rortypes.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest.h>
#include <string>
#include <tuple>
#include <unordered_map>

namespace ror_test
{

void test_entries(rhi::ShaderBufferTemplate &a_sb, std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> a_var_values, bool a_debug_messages = false)
{
	auto entries = a_sb.entries_structs();

	for (auto &e : entries)
	{
		if (a_debug_messages)
			std::cout << e->m_name << "=" << e->m_offset << ", " << e->m_stride << std::endl;

		EXPECT_EQ(std::get<0>(a_var_values[e->m_name]), e->m_offset);
		EXPECT_EQ(std::get<1>(a_var_values[e->m_name]), e->m_stride);
		EXPECT_EQ(std::get<2>(a_var_values[e->m_name]), e->m_type);
	}
}

void example0_test(rhi::Layout a_layout)
{
	rhi::ShaderBufferTemplate sb{"Example0", rhi::ShaderBufferType::ubo, a_layout};

	sb.add_entry("a", rhi::Format::float32_1, 1);
	sb.add_entry("b", rhi::Format::float32_2, 1);
	sb.add_entry("c", rhi::Format::float32_3, 1);
	sb.add_entry("d", rhi::Format::int32_1, 1);
	sb.add_entry("e", rhi::Format::bool32_2, 1);
	sb.add_entry("g", rhi::Format::float32_1, 1);
	sb.add_entry("h", rhi::Format::float32_1, 1);
	sb.add_entry("i", rhi::Format::float32_2x3, 1);
	sb.add_entry("j", rhi::Format::uint32_3, 1);
	sb.add_entry("k", rhi::Format::float32_2, 1);
	sb.add_entry("l", rhi::Format::float32_1, 1);
	sb.add_entry("m", rhi::Format::float32_2, 1);
	sb.add_entry("n", rhi::Format::float32_3x3, 1);

	rhi::ShaderBufferMetal sbm{"Example0", rhi::ShaderBufferType::ubo, a_layout, 0u, 0u,
	                           "a", rhi::VertexFormat::float32_1, 1u,
	                           "b", rhi::VertexFormat::float32_2, 1u,
	                           "c", rhi::VertexFormat::float32_3, 1u,
	                           "d", rhi::VertexFormat::int32_1, 1u,
	                           "e", rhi::VertexFormat::bool32_2, 1u,
	                           "g", rhi::VertexFormat::float32_1, 1u,
	                           "h", rhi::VertexFormat::float32_1, 1u,
	                           "i", rhi::VertexFormat::float32_2x3, 1u,
	                           "j", rhi::VertexFormat::uint32_3, 1u,
	                           "k", rhi::VertexFormat::float32_2, 1u,
	                           "l", rhi::VertexFormat::float32_1, 1u,
	                           "m", rhi::VertexFormat::float32_2, 1u,
	                           "n", rhi::VertexFormat::float32_3x3, 1u};

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, rhi::Format::float32_1}},
	    {"b", {8, 8, rhi::Format::float32_2}},
	    {"c", {16, 12, rhi::Format::float32_3}},
	    {"d", {28, 4, rhi::Format::int32_1}},
	    {"e", {32, 8, rhi::Format::bool32_2}},
	    {"g", {40, 4, rhi::Format::float32_1}},
	    {"h", {44, 4, rhi::Format::float32_1}},
	    {"i", {48, 32, rhi::Format::float32_2x3}},
	    {"j", {80, 12, rhi::Format::uint32_3}},
	    {"k", {96, 8, rhi::Format::float32_2}},
	    {"l", {104, 4, rhi::Format::float32_1}},
	    {"m", {112, 8, rhi::Format::float32_2}},
	    {"n", {128, 64, rhi::Format::float32_3x3}}};

	test_entries(sb, var_values);

	// std::cout << std::endl;
	// In flat structure
	// layout(std140) uniform Example1
	// {
	//	// Base types below consume 4 basic machine units
	//	//
	//	//                           base   base  align
	//	//                     rule  align  off.  off.  bytes used
	//	//                     ----  ------ ----  ----  -----------------------
	//	float a;           //  1       4     0    0    0..3
	//	vec2  b;           //  2       8     4    8    8..15
	//	vec3  c;           //  3      16    16   16    16..27
	//	int   d;           //  1       4    28   28    28..31
	//	bvec2 e;           //  2       8    32   32    32..39
	//	float g;           //  1       4    40   40    40..43
	//	float h;           //  4       4    44   44    44..47
	//	mat2x3 i;          // 5/4     16    48   48    48..59 (i, column 0)
	//					   //                    64    64..75 (i, column 1)
	//					   // 5/4     16    76   80    (pad end of i because next thing needs to be 16 bytes aligned
	//	uvec3 j;           //  3      16    76   80    80..91 (j)
	//	vec2  k;           //  2       8    92   96    96..103 (k)
	//	float l;           //  4       4   104  104    104..107 (l)
	//	vec2 m;            //  2       8   108  112    112..119 (m)
	//	mat3 n;            // 6/4     16   120  128    128..139 (n, column 0)
	//					   //                   144    144..155 (n, column 1)
	//					   //                   160    160..171 (n, column 2)
	//					   // 6/4     16   160  160    (pad end of n) confirm????
	// };

	auto output0 = sbm.to_glsl_string();
	auto output = sb.to_glsl_string();

	std::string layout{"layout("};
	if (a_layout == rhi::Layout::std140)
		layout.append("std140");
	else
		layout.append("std430");

	auto uniform_output{R"output(, set = 0, binding = 0) uniform Example0
{
	float a;
	vec2 b;
	vec3 c;
	int d;
	bvec2 e;
	float g;
	float h;
	mat2x3 i;
	uvec3 j;
	vec2 k;
	float l;
	vec2 m;
	mat3 n;
} in_Example0;
)output"};

	auto expected_output{layout};
	expected_output.append(uniform_output);

	EXPECT_EQ(output0, expected_output);
	EXPECT_EQ(output, expected_output);
}

TEST(ShaderBuffer, example0_test)
{
	example0_test(rhi::Layout::std140);
	example0_test(rhi::Layout::std430);
}

void example1_test(rhi::Layout a_layout)
{
	rhi::ShaderBufferTemplate sb{"Example1", rhi::ShaderBufferType::ubo, a_layout};

	sb.add_entry("a", rhi::Format::float32_1, 1);
	sb.add_entry("b", rhi::Format::float32_2, 1);
	sb.add_entry("c", rhi::Format::float32_3, 1);
	sb.add_entry("d", rhi::Format::int32_1, 1);
	sb.add_entry("e", rhi::Format::bool32_2, 1);
	sb.add_entry("g", rhi::Format::float32_1, 1);
	sb.add_entry("h", rhi::Format::float32_1, 1);
	sb.add_entry("i", rhi::Format::float32_2x3, 1);
	sb.add_entry("j", rhi::Format::uint32_1, 1);
	sb.add_entry("k", rhi::Format::float32_2, 1);
	sb.add_entry("l", rhi::Format::float32_1, 1);
	sb.add_entry("m", rhi::Format::float32_2, 1);
	sb.add_entry("n", rhi::Format::float32_3x3, 1);

	rhi::ShaderBufferMetal sbm{"Example1", rhi::ShaderBufferType::ubo, a_layout, 0u, 0u,
	                           "a", rhi::VertexFormat::float32_1, 1u,
	                           "b", rhi::VertexFormat::float32_2, 1u,
	                           "c", rhi::VertexFormat::float32_3, 1u,
	                           "d", rhi::VertexFormat::int32_1, 1u,
	                           "e", rhi::VertexFormat::bool32_2, 1u,
	                           "g", rhi::VertexFormat::float32_1, 1u,
	                           "h", rhi::VertexFormat::float32_1, 1u,
	                           "i", rhi::VertexFormat::float32_2x3, 1u,
	                           "j", rhi::VertexFormat::uint32_1, 1u,
	                           "k", rhi::VertexFormat::float32_2, 1u,
	                           "l", rhi::VertexFormat::float32_1, 1u,
	                           "m", rhi::VertexFormat::float32_2, 1u,
	                           "n", rhi::VertexFormat::float32_3x3, 1u};

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, rhi::Format::float32_1}},
	    {"b", {8, 8, rhi::Format::float32_2}},
	    {"c", {16, 12, rhi::Format::float32_3}},
	    {"d", {28, 4, rhi::Format::int32_1}},
	    {"e", {32, 8, rhi::Format::bool32_2}},
	    {"g", {40, 4, rhi::Format::float32_1}},
	    {"h", {44, 4, rhi::Format::float32_1}},
	    {"i", {48, 32, rhi::Format::float32_2x3}},
	    {"j", {80, 4, rhi::Format::uint32_1}},
	    {"k", {88, 8, rhi::Format::float32_2}},
	    {"l", {96, 4, rhi::Format::float32_1}},
	    {"m", {104, 8, rhi::Format::float32_2}},
	    {"n", {112, 64, rhi::Format::float32_3x3}}};

	test_entries(sb, var_values);

	// std::cout << std::endl;
	// In flat structure
	// layout(std140) uniform Example1
	// {
	//	// Base types below consume 4 basic machine units
	//	//
	//	//                           base   base  align
	//	//                     rule  align  off.  off.  bytes used
	//	//                     ----  ------ ----  ----  -----------------------
	//	float a;           //  1       4     0    0    0..3
	//	vec2  b;           //  2       8     4    8    8..15
	//	vec3  c;           //  3      16    16   16    16..27
	//	int   d;           //  1       4    28   28    28..31
	//	bvec2 e;           //  2       8    32   32    32..39
	//	float g;           //  1       4    40   40    40..43
	//	float h;           //  4       4    44   44    44..47
	//	mat2x3 i;          // 5/4     16    48   48    48..59 (i, column 0)
	//					   //                    64    64..75 (i, column 1)
	//					   // 5/4     16    76   80    (pad end of i because next thing needs to be 16 bytes aligned
	//	float j;           //  3       4    76   80    80..83 (j)
	//	vec2  k;           //  2       8    84   88    88..95 (k)
	//	float l;           //  4       4    96   96    94..99 (l)
	//	vec2 m;            //  2       8   100  104    104..111 (m)
	//	mat3 n;            // 6/4     16   112  112    112..123 (n, column 0)
	//					   //                   128    128..139 (n, column 1)
	//					   //                   144    144..159 (n, column 2)
	//					   // 6/4     16   160  160    (pad end of n) confirm????
	// };

	auto output0 = sbm.to_glsl_string();
	auto output = sb.to_glsl_string();

	std::string layout{"layout("};
	if (a_layout == rhi::Layout::std140)
		layout.append("std140");
	else
		layout.append("std430");

	auto uniform_output{R"output(, set = 0, binding = 0) uniform Example1
{
	float a;
	vec2 b;
	vec3 c;
	int d;
	bvec2 e;
	float g;
	float h;
	mat2x3 i;
	uint j;
	vec2 k;
	float l;
	vec2 m;
	mat3 n;
} in_Example1;
)output"};

	auto expected_output{layout};
	expected_output.append(uniform_output);

	EXPECT_EQ(output0, expected_output);
	EXPECT_EQ(output, expected_output);
}

TEST(ShaderBuffer, example1_test)
{
	example1_test(rhi::Layout::std140);
	example1_test(rhi::Layout::std430);
}

TEST(ShaderBuffer, std140_example2_test)
{
	rhi::ShaderBufferTemplate sb{"Example2", rhi::ShaderBufferType::ubo, rhi::Layout::std140};

	sb.add_entry("a", rhi::Format::float32_1, 1);
	sb.add_entry("b", rhi::Format::float32_2, 1);
	sb.add_entry("c", rhi::Format::float32_3, 1);
	sb.add_entry("d", rhi::Format::int32_1, 1);
	sb.add_entry("e", rhi::Format::bool32_2, 1);
	sb.add_entry("g", rhi::Format::float32_1, 2);
	sb.add_entry("h", rhi::Format::float32_1, 1);
	sb.add_entry("i", rhi::Format::float32_2x3, 1);
	sb.add_entry("j", rhi::Format::uint32_1, 1);
	sb.add_entry("k", rhi::Format::float32_2, 1);
	sb.add_entry("l", rhi::Format::float32_1, 2);
	sb.add_entry("m", rhi::Format::float32_2, 1);
	sb.add_entry("n", rhi::Format::float32_3x3, 1);

	rhi::ShaderBufferMetal sbm{"Example2", rhi::ShaderBufferType::ubo, rhi::Layout::std140, 0u, 0u,
	                           "a", rhi::VertexFormat::float32_1, 1u,
	                           "b", rhi::VertexFormat::float32_2, 1u,
	                           "c", rhi::VertexFormat::float32_3, 1u,
	                           "d", rhi::VertexFormat::int32_1, 1u,
	                           "e", rhi::VertexFormat::bool32_2, 1u,
	                           "g", rhi::VertexFormat::float32_1, 2u,
	                           "h", rhi::VertexFormat::float32_1, 1u,
	                           "i", rhi::VertexFormat::float32_2x3, 1u,
	                           "j", rhi::VertexFormat::uint32_1, 1u,
	                           "k", rhi::VertexFormat::float32_2, 1u,
	                           "l", rhi::VertexFormat::float32_1, 2u,
	                           "m", rhi::VertexFormat::float32_2, 1u,
	                           "n", rhi::VertexFormat::float32_3x3, 1u};

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, rhi::Format::float32_1}},
	    {"b", {8, 8, rhi::Format::float32_2}},
	    {"c", {16, 12, rhi::Format::float32_3}},
	    {"d", {28, 4, rhi::Format::int32_1}},
	    {"e", {32, 8, rhi::Format::bool32_2}},
	    {"g", {48, 16, rhi::Format::float32_1}},
	    {"h", {80, 4, rhi::Format::float32_1}},
	    {"i", {96, 32, rhi::Format::float32_2x3}},
	    {"j", {128, 4, rhi::Format::uint32_1}},
	    {"k", {136, 8, rhi::Format::float32_2}},
	    {"l", {144, 16, rhi::Format::float32_1}},
	    {"m", {176, 8, rhi::Format::float32_2}},
	    {"n", {192, 64, rhi::Format::float32_3x3}}};

	test_entries(sb, var_values);

	// std::cout << std::endl;
	// In flat structure
	// layout(std140) uniform Example1
	// {
	//	// Base types below consume 4 basic machine units
	//	//
	//	//                           base   base  align
	//	//                     rule  align  off.  off.  bytes used
	//	//                     ----  ------ ----  ----  -----------------------
	//	float a;           //  1       4     0    0    0..3
	//	vec2  b;           //  2       8     4    8    8..15
	//	vec3  c;           //  3      16    16   16    16..27
	//	int   d;           //  1       4    28   28    28..31
	//	bvec2 e;           //  2       8    32   32    32..39
	//	float g[2];        //  1      16    40   48    48..51, 64..67
	//	float h;           //  4       4    44   80    44..47
	//	mat2x3 i;          // 5/4     16    48   96    48..59 (i, column 0)
	//					   //                    64    64..75 (i, column 1)
	//					   // 5/4     16    76   80    (pad end of i because next thing needs to be 16 bytes aligned
	//	float j;           //  3       4    76  128    80..83 (j)
	//	vec2  k;           //  2       8    84  136    88..95 (k)
	//	float l[2];        //  4      16    96  144    94..99 (l)
	//	vec2 m;            //  2       8   100  176    104..111 (m)
	//	mat3 n;            // 6/4     16   112  192    112..123 (n, column 0)
	//					   //                   128    128..139 (n, column 1)
	//					   //                   144    144..159 (n, column 2)
	//					   // 6/4     16   160  160    (pad end of n) confirm????
	// };

	auto output0 = sbm.to_glsl_string();
	auto output = sb.to_glsl_string();

	auto expected_output{R"output(layout(std140, set = 0, binding = 0) uniform Example2
{
	float a;
	vec2 b;
	vec3 c;
	int d;
	bvec2 e;
	float g[2];
	float h;
	mat2x3 i;
	uint j;
	vec2 k;
	float l[2];
	vec2 m;
	mat3 n;
} in_Example2;
)output"};

	EXPECT_EQ(output0, expected_output);
	EXPECT_EQ(output, expected_output);
}

TEST(ShaderBuffer, std140_example3_test)
{
	rhi::ShaderBufferTemplate sb{"Example3", rhi::ShaderBufferType::ubo, rhi::Layout::std140};

	sb.add_entry("a", rhi::Format::float32_1, 1);
	sb.add_entry("b", rhi::Format::float32_2, 1);
	sb.add_entry("c", rhi::Format::float32_3, 1);
	sb.add_entry("d", rhi::Format::int32_1, 1);
	sb.add_entry("e", rhi::Format::bool32_2, 1);
	sb.add_entry("g", rhi::Format::float32_1, 2);
	sb.add_entry("h", rhi::Format::float32_1, 1);
	sb.add_entry("i", rhi::Format::float32_2x3, 2);
	sb.add_entry("j", rhi::Format::uint32_1, 1);
	sb.add_entry("k", rhi::Format::float32_2, 1);
	sb.add_entry("l", rhi::Format::float32_1, 2);
	sb.add_entry("m", rhi::Format::float32_2, 1);
	sb.add_entry("n", rhi::Format::float32_3x3, 1);

	rhi::ShaderBufferMetal sbm{"Example3", rhi::ShaderBufferType::ubo, rhi::Layout::std140, 0u, 0u,
	                           "a", rhi::VertexFormat::float32_1, 1u,
	                           "b", rhi::VertexFormat::float32_2, 1u,
	                           "c", rhi::VertexFormat::float32_3, 1u,
	                           "d", rhi::VertexFormat::int32_1, 1u,
	                           "e", rhi::VertexFormat::bool32_2, 1u,
	                           "g", rhi::VertexFormat::float32_1, 2u,
	                           "h", rhi::VertexFormat::float32_1, 1u,
	                           "i", rhi::VertexFormat::float32_2x3, 2u,
	                           "j", rhi::VertexFormat::uint32_1, 1u,
	                           "k", rhi::VertexFormat::float32_2, 1u,
	                           "l", rhi::VertexFormat::float32_1, 2u,
	                           "m", rhi::VertexFormat::float32_2, 1u,
	                           "n", rhi::VertexFormat::float32_3x3, 1u};

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, rhi::Format::float32_1}},
	    {"b", {8, 8, rhi::Format::float32_2}},
	    {"c", {16, 12, rhi::Format::float32_3}},
	    {"d", {28, 4, rhi::Format::int32_1}},
	    {"e", {32, 8, rhi::Format::bool32_2}},
	    {"g", {48, 16, rhi::Format::float32_1}},
	    {"h", {80, 4, rhi::Format::float32_1}},
	    {"i", {96, 32, rhi::Format::float32_2x3}},
	    {"j", {160, 4, rhi::Format::uint32_1}},
	    {"k", {168, 8, rhi::Format::float32_2}},
	    {"l", {176, 16, rhi::Format::float32_1}},
	    {"m", {208, 8, rhi::Format::float32_2}},
	    {"n", {224, 64, rhi::Format::float32_3x3}}};

	test_entries(sb, var_values);

	auto output0 = sbm.to_glsl_string();
	auto output = sb.to_glsl_string();

	auto expected_output{R"output(layout(std140, set = 0, binding = 0) uniform Example3
{
	float a;
	vec2 b;
	vec3 c;
	int d;
	bvec2 e;
	float g[2];
	float h;
	mat2x3 i[2];
	uint j;
	vec2 k;
	float l[2];
	vec2 m;
	mat3 n;
} in_Example3;
)output"};

	EXPECT_EQ(output0, expected_output);
	EXPECT_EQ(output, expected_output);
}

TEST(ShaderBuffer, std140_example4_test)
{
	rhi::ShaderBufferTemplate sb{"Example4", rhi::ShaderBufferType::ubo, rhi::Layout::std140};

	sb.add_entry("a", rhi::Format::float32_1, 1);
	sb.add_entry("b", rhi::Format::float32_2, 1);
	sb.add_entry("c", rhi::Format::float32_3, 1);

	// Lets create struct f
	rhi::ShaderBufferTemplate::Struct f("f", 1);
	f.add_entry("d", rhi::Format::int32_1, rhi::Layout::std140, 1);
	f.add_entry("e", rhi::Format::bool32_2, rhi::Layout::std140, 1);

	sb.add_struct(f);

	sb.add_entry("g", rhi::Format::float32_1, 1);
	sb.add_entry("h", rhi::Format::float32_1, 2);

	sb.add_entry("i", rhi::Format::float32_2x3, 1);

	// // Lets create struct o
	rhi::ShaderBufferTemplate::Struct o("o", 2);
	o.add_entry("j", rhi::Format::uint32_3, rhi::Layout::std140, 1);
	o.add_entry("k", rhi::Format::float32_2, rhi::Layout::std140, 1);
	o.add_entry("l", rhi::Format::float32_1, rhi::Layout::std140, 2);
	o.add_entry("m", rhi::Format::float32_2, rhi::Layout::std140, 1);
	o.add_entry("n", rhi::Format::float32_3x3, rhi::Layout::std140, 2);

	sb.add_struct(o);

	rhi::ShaderBufferMetal sbm{"Example4", rhi::ShaderBufferType::ubo, rhi::Layout::std140, 0u, 0u,
	                           "a", rhi::VertexFormat::float32_1, 1u,
	                           "b", rhi::VertexFormat::float32_2, 1u,
	                           "c", rhi::VertexFormat::float32_3, 1u,

	                           "f", rhi::VertexFormat::struct_1, 1u,

	                           "d", rhi::VertexFormat::int32_1, 1u,
	                           "e", rhi::VertexFormat::bool32_2, 1u,

	                           "f_end", rhi::VertexFormat::struct_0, 1u,

	                           "g", rhi::VertexFormat::float32_1, 1u,
	                           "h", rhi::VertexFormat::float32_1, 2u,

	                           "i", rhi::VertexFormat::float32_2x3, 1u,

	                           "o", rhi::VertexFormat::struct_1, 2u,
	                           "j", rhi::VertexFormat::uint32_3, 1u,
	                           "k", rhi::VertexFormat::float32_2, 1u,
	                           "l", rhi::VertexFormat::float32_1, 2u,
	                           "m", rhi::VertexFormat::float32_2, 1u,
	                           "n", rhi::VertexFormat::float32_3x3, 2u,
	                           "o_end", rhi::VertexFormat::struct_0};


	// Index of offset, stride, size, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, 4, rhi::Format::float32_1}},
	    {"b", {8, 8, 8, rhi::Format::float32_2}},
	    {"c", {16, 12, 12, rhi::Format::float32_3}},
	    {"d", {32, 4, 4, rhi::Format::int32_1}},
	    {"e", {40, 8, 8, rhi::Format::bool32_2}},
	    {"f", {32, 16, 48, rhi::Format::struct_1}},
	    {"g", {48, 4, 4, rhi::Format::float32_1}},
	    {"h", {64, 16, 4, rhi::Format::float32_1}},
	    {"i", {96, 32, 32, rhi::Format::float32_2x3}},
	    {"j", {128, 12, 12, rhi::Format::uint32_3}},
	    {"k", {144, 8, 8, rhi::Format::float32_2}},
	    {"l", {160, 16, 4, rhi::Format::float32_1}},
	    {"m", {192, 8, 8, rhi::Format::float32_2}},
	    {"n", {208, 64, 64, rhi::Format::float32_3x3}},
	    {"o", {128, 208, 544, rhi::Format::struct_1}}};

	auto entries = sb.entries_structs();

	for (auto &e : entries)
	{
		// std::cout << ", " << e->m_name << "=" << e->m_offset;
		EXPECT_EQ(std::get<0>(var_values[e->m_name]), e->m_offset);
		EXPECT_EQ(std::get<1>(var_values[e->m_name]), e->m_stride);
		EXPECT_EQ(std::get<2>(var_values[e->m_name]), e->m_size);
		EXPECT_EQ(std::get<3>(var_values[e->m_name]), e->m_type);
	}

	// layout(std140) buffer Example
	// {
	// Base types below consume 4 basic machine units
	//
	//                           base   base  align
	//                     rule  align  off.  off.  bytes used
	//                     ----  ------ ----  ----  -----------------------
	//	float a;            //  1       4     0    0    0..3
	//	vec2  b;            //  2       8     4    8    8..15
	//	vec3  c;            //  3      16    16   16    16..27
	//	struct
	//	{                   //  9      16    28   32    (align begin)
	//		int   d;        //  1       4    32   32    32..35
	//		bvec2 e;        //  2       8    36   40    40..47
	//	} f;                //  9      16    48   48    (pad end)
	//	float g;            //  1       4    48   48    48..51
	//	float h[2];         //  4      16    52   64    64..67 (h[0])
	//						//                    80    80..83 (h[1])
	//						//  4      16    84   96    (pad end of h)
	//	mat2x3 i;           // 5/4     16    96   96    96..107 (i, column 0)
	//						//                   112    112..123 (i, column 1)
	//						// 5/4     16   124  128    (pad end of i)
	//	struct
	//	{                   //  10     16   128  128    (align begin)
	//		uvec3 j;        //  3      16   128  128    128..139 (o[0].j)
	//		vec2  k;        //  2       8   140  144    144..151 (o[0].k)
	//		float l[2];     //  4      16   152  160    160..163 (o[0].l[0])
	//						//                   176    176..179 (o[0].l[1])
	//						//  4      16   180  192    (pad end of o[0].l)
	//		vec2 m;         //  2       8   192  192    192..199 (o[0].m)
	//		mat3 n[2];      // 6/4     16   200  208    208..219 (o[0].n[0], column 0)
	//						//                   224    224..235 (o[0].n[0], column 1)
	//						//                   240    240..251 (o[0].n[0], column 2)
	//						//                   256    256..267 (o[0].n[1], column 0)
	//						//                   272    272..283 (o[0].n[1], column 1)
	//						//                   288    288..299 (o[0].n[1], column 2)
	//						// 6/4     16   300  304    (pad end of o[0].n)
	//						//  9      16   304  304    (pad end of o[0])
	//						//  3      16   304  304    304..315 (o[1].j)
	//						//  2       8   316  320    320..327 (o[1].k)
	//						//  4      16   328  336    336..347 (o[1].l[0])
	//						//                   352    352..355 (o[1].l[1])
	//						//  4      16   356  368    (pad end of o[1].l)
	//						//  2       8   368  368    368..375 (o[1].m)
	//						// 6/4     16   376  384    384..395 (o[1].n[0], column 0)
	//						//                   400    400..411 (o[1].n[0], column 1)
	//						//                   416    416..427 (o[1].n[0], column 2)
	//						//                   432    432..443 (o[1].n[1], column 0)
	//						//                   448    448..459 (o[1].n[1], column 1)
	//						//                   464    464..475 (o[1].n[1], column 2)
	//						// 6/4     16   476  480    (pad end of o[1].n)
	//						//  9      16   480  480    (pad end of o[1])
	//	} o[2];
	// };

	auto output0 = sbm.to_glsl_string();
	auto output = sb.to_glsl_string();

	auto expected_output{R"output(struct F
{
	int d;
	bvec2 e;
};

struct O
{
	uvec3 j;
	vec2 k;
	float l[2];
	vec2 m;
	mat3 n[2];
};

layout(std140, set = 0, binding = 0) uniform Example4
{
	float a;
	vec2 b;
	vec3 c;
	F f;
	float g;
	float h[2];
	mat2x3 i;
	O o[2];
} in_Example4;
)output"};

	EXPECT_EQ(output0, expected_output);
	EXPECT_EQ(output, expected_output);
}

// TODO: Create std430 equvivalent of the following 3 tests
void add_bool_entries(rhi::ShaderBufferTemplate &sb)
{
	sb.add_entry("a", rhi::Format::bool32_1, 1);
	sb.add_entry("b", rhi::Format::bool32_1, 1);
	sb.add_entry("c", rhi::Format::bool32_1, 1);
	sb.add_entry("d", rhi::Format::bool32_1, 1);
	sb.add_entry("e", rhi::Format::bool32_1, 1);
	sb.add_entry("g", rhi::Format::bool32_1, 2);
	sb.add_entry("h", rhi::Format::bool32_1, 1);
	sb.add_entry("i", rhi::Format::bool32_1, 2);
	sb.add_entry("j", rhi::Format::bool32_1, 1);
	sb.add_entry("k", rhi::Format::bool32_1, 1);
	sb.add_entry("l", rhi::Format::bool32_1, 2);
	sb.add_entry("m", rhi::Format::bool32_1, 1);
	sb.add_entry("n", rhi::Format::bool32_1, 1);
}

TEST(ShaderBuffer, bool_test_std140)
{
	rhi::ShaderBufferTemplate sb{"ExampleBool", rhi::ShaderBufferType::ubo, rhi::Layout::std140};
	add_bool_entries(sb);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, rhi::Format::bool32_1}},
	    {"b", {4, 4, rhi::Format::bool32_1}},
	    {"c", {8, 4, rhi::Format::bool32_1}},
	    {"d", {12, 4, rhi::Format::bool32_1}},
	    {"e", {16, 4, rhi::Format::bool32_1}},
	    {"g", {32, 16, rhi::Format::bool32_1}},
	    {"h", {64, 4, rhi::Format::bool32_1}},
	    {"i", {80, 16, rhi::Format::bool32_1}},
	    {"j", {112, 4, rhi::Format::bool32_1}},
	    {"k", {116, 4, rhi::Format::bool32_1}},
	    {"l", {128, 16, rhi::Format::bool32_1}},
	    {"m", {160, 4, rhi::Format::bool32_1}},
	    {"n", {164, 4, rhi::Format::bool32_1}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, bool_test_std430)
{
	rhi::ShaderBufferTemplate sb{"ExampleBool", rhi::ShaderBufferType::ubo, rhi::Layout::std430};
	add_bool_entries(sb);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, rhi::Format::bool32_1}},
	    {"b", {4, 4, rhi::Format::bool32_1}},
	    {"c", {8, 4, rhi::Format::bool32_1}},
	    {"d", {12, 4, rhi::Format::bool32_1}},
	    {"e", {16, 4, rhi::Format::bool32_1}},
	    {"g", {20, 4, rhi::Format::bool32_1}},
	    {"h", {28, 4, rhi::Format::bool32_1}},
	    {"i", {32, 4, rhi::Format::bool32_1}},
	    {"j", {40, 4, rhi::Format::bool32_1}},
	    {"k", {44, 4, rhi::Format::bool32_1}},
	    {"l", {48, 4, rhi::Format::bool32_1}},
	    {"m", {56, 4, rhi::Format::bool32_1}},
	    {"n", {60, 4, rhi::Format::bool32_1}}};

	test_entries(sb, var_values);
}

void add_double_entries(rhi::ShaderBufferTemplate &sb)
{
	sb.add_entry("a", rhi::Format::float64_1, 1);
	sb.add_entry("b", rhi::Format::float64_1, 1);
	sb.add_entry("c", rhi::Format::float64_1, 1);
	sb.add_entry("d", rhi::Format::float64_1, 1);
	sb.add_entry("e", rhi::Format::float64_1, 1);
	sb.add_entry("g", rhi::Format::float64_1, 2);
	sb.add_entry("h", rhi::Format::float64_1, 1);
	sb.add_entry("i", rhi::Format::float64_1, 2);
	sb.add_entry("j", rhi::Format::float64_1, 1);
	sb.add_entry("k", rhi::Format::float64_1, 1);
	sb.add_entry("l", rhi::Format::float64_1, 2);
	sb.add_entry("m", rhi::Format::float64_1, 1);
	sb.add_entry("n", rhi::Format::float64_1, 1);
}

TEST(ShaderBuffer, double_test_std140)
{
	rhi::ShaderBufferTemplate sb{"ExampleDouble", rhi::ShaderBufferType::ubo, rhi::Layout::std140};
	add_double_entries(sb);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 8, rhi::Format::float64_1}},
	    {"b", {8, 8, rhi::Format::float64_1}},
	    {"c", {16, 8, rhi::Format::float64_1}},
	    {"d", {24, 8, rhi::Format::float64_1}},
	    {"e", {32, 8, rhi::Format::float64_1}},
	    {"g", {48, 16, rhi::Format::float64_1}},
	    {"h", {80, 8, rhi::Format::float64_1}},
	    {"i", {96, 16, rhi::Format::float64_1}},
	    {"j", {128, 8, rhi::Format::float64_1}},
	    {"k", {136, 8, rhi::Format::float64_1}},
	    {"l", {144, 16, rhi::Format::float64_1}},
	    {"m", {176, 8, rhi::Format::float64_1}},
	    {"n", {184, 8, rhi::Format::float64_1}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, double_test_std430)
{
	rhi::ShaderBufferTemplate sb{"ExampleDouble", rhi::ShaderBufferType::ubo, rhi::Layout::std430};
	add_double_entries(sb);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 8, rhi::Format::float64_1}},
	    {"b", {8, 8, rhi::Format::float64_1}},
	    {"c", {16, 8, rhi::Format::float64_1}},
	    {"d", {24, 8, rhi::Format::float64_1}},
	    {"e", {32, 8, rhi::Format::float64_1}},
	    {"g", {40, 8, rhi::Format::float64_1}},
	    {"h", {56, 8, rhi::Format::float64_1}},
	    {"i", {64, 8, rhi::Format::float64_1}},
	    {"j", {80, 8, rhi::Format::float64_1}},
	    {"k", {88, 8, rhi::Format::float64_1}},
	    {"l", {96, 8, rhi::Format::float64_1}},
	    {"m", {112, 8, rhi::Format::float64_1}},
	    {"n", {120, 8, rhi::Format::float64_1}}};

	test_entries(sb, var_values);
}

void add_double_vec_entries(rhi::ShaderBufferTemplate &sb)
{
	sb.add_entry("a", rhi::Format::float64_1, 1);
	sb.add_entry("b", rhi::Format::float64_2, 1);
	sb.add_entry("c", rhi::Format::float64_1, 1);
	sb.add_entry("d", rhi::Format::float64_1, 1);
	sb.add_entry("e", rhi::Format::float64_4, 1);
	sb.add_entry("g", rhi::Format::float64_1, 2);
	sb.add_entry("h", rhi::Format::float64_1, 1);
	sb.add_entry("i", rhi::Format::float64_1, 2);
	sb.add_entry("j", rhi::Format::float64_1, 1);
	sb.add_entry("k", rhi::Format::float64_4, 1);
	sb.add_entry("l", rhi::Format::float64_1, 2);
	sb.add_entry("m", rhi::Format::float64_3, 1);
	sb.add_entry("n", rhi::Format::float64_1, 1);
}

TEST(ShaderBuffer, double_vec_test_std140)
{
	rhi::ShaderBufferTemplate sb{"ExampleBool", rhi::ShaderBufferType::ubo, rhi::Layout::std140};
	add_double_vec_entries(sb);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 8, rhi::Format::float64_1}},
	    {"b", {16, 16, rhi::Format::float64_2}},
	    {"c", {32, 8, rhi::Format::float64_1}},
	    {"d", {40, 8, rhi::Format::float64_1}},
	    {"e", {64, 32, rhi::Format::float64_4}},
	    {"g", {96, 16, rhi::Format::float64_1}},
	    {"h", {128, 8, rhi::Format::float64_1}},
	    {"i", {144, 16, rhi::Format::float64_1}},
	    {"j", {176, 8, rhi::Format::float64_1}},
	    {"k", {192, 32, rhi::Format::float64_4}},
	    {"l", {224, 16, rhi::Format::float64_1}},
	    {"m", {256, 24, rhi::Format::float64_3}},
	    {"n", {280, 8, rhi::Format::float64_1}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, double_vec_test_std430)
{
	rhi::ShaderBufferTemplate sb{"ExampleBool", rhi::ShaderBufferType::ubo, rhi::Layout::std430};
	add_double_vec_entries(sb);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 8, rhi::Format::float64_1}},
	    {"b", {16, 16, rhi::Format::float64_2}},
	    {"c", {32, 8, rhi::Format::float64_1}},
	    {"d", {40, 8, rhi::Format::float64_1}},
	    {"e", {64, 32, rhi::Format::float64_4}},
	    {"g", {96, 8, rhi::Format::float64_1}},
	    {"h", {112, 8, rhi::Format::float64_1}},
	    {"i", {120, 8, rhi::Format::float64_1}},
	    {"j", {136, 8, rhi::Format::float64_1}},
	    {"k", {160, 32, rhi::Format::float64_4}},
	    {"l", {192, 8, rhi::Format::float64_1}},
	    {"m", {224, 24, rhi::Format::float64_3}},
	    {"n", {248, 8, rhi::Format::float64_1}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, std430_ssbo_example2_test)
{
	rhi::ShaderBufferTemplate sb{"Example2", rhi::ShaderBufferType::ubo, rhi::Layout::std430};

	sb.add_entry("a", rhi::Format::float32_1, 1);
	sb.add_entry("b", rhi::Format::float32_2, 1);
	sb.add_entry("c", rhi::Format::float32_3, 1);
	sb.add_entry("d", rhi::Format::int32_1, 1);
	sb.add_entry("e", rhi::Format::bool32_2, 1);
	sb.add_entry("g", rhi::Format::float32_1, 2);
	sb.add_entry("h", rhi::Format::float32_1, 1);
	sb.add_entry("i", rhi::Format::float32_2x3, 2);
	sb.add_entry("j", rhi::Format::float32_1, 1);
	sb.add_entry("k", rhi::Format::float32_2, 1);
	sb.add_entry("l", rhi::Format::float32_1, 2);
	sb.add_entry("m", rhi::Format::float32_2, 1);
	sb.add_entry("n", rhi::Format::float32_3x3, 1);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, rhi::Format::float32_1}},
	    {"b", {8, 8, rhi::Format::float32_2}},
	    {"c", {16, 12, rhi::Format::float32_3}},
	    {"d", {28, 4, rhi::Format::int32_1}},
	    {"e", {32, 8, rhi::Format::bool32_2}},
	    {"g", {40, 4, rhi::Format::float32_1}},
	    {"h", {48, 4, rhi::Format::float32_1}},
	    {"i", {64, 32, rhi::Format::float32_2x3}},
	    {"j", {128, 4, rhi::Format::float32_1}},
	    {"k", {136, 8, rhi::Format::float32_2}},
	    {"l", {144, 4, rhi::Format::float32_1}},
	    {"m", {152, 8, rhi::Format::float32_2}},
	    {"n", {160, 64, rhi::Format::float32_3x3}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, std430_ssbo_example3_test)
{
	rhi::ShaderBufferTemplate sb{"Example3", rhi::ShaderBufferType::ubo, rhi::Layout::std430};

	sb.add_entry("a", rhi::Format::float32_1, 1);
	sb.add_entry("b", rhi::Format::float32_1, 2);
	sb.add_entry("c", rhi::Format::float32_1, 1);
	sb.add_entry("d", rhi::Format::float32_1, 1);
	sb.add_entry("e", rhi::Format::float32_1, 1);
	sb.add_entry("g", rhi::Format::float32_1, 1);
	sb.add_entry("h", rhi::Format::float32_1, 1);
	sb.add_entry("i", rhi::Format::float32_1, 2);
	sb.add_entry("j", rhi::Format::float32_1, 1);
	sb.add_entry("k", rhi::Format::float32_1, 2);
	sb.add_entry("l", rhi::Format::float32_1, 1);
	sb.add_entry("m", rhi::Format::float32_1, 2);
	sb.add_entry("n", rhi::Format::float32_1, 1);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, rhi::Format::float32_1}},
	    {"b", {4, 4, rhi::Format::float32_1}},
	    {"c", {12, 4, rhi::Format::float32_1}},
	    {"d", {16, 4, rhi::Format::float32_1}},
	    {"e", {20, 4, rhi::Format::float32_1}},
	    {"g", {24, 4, rhi::Format::float32_1}},
	    {"h", {28, 4, rhi::Format::float32_1}},
	    {"i", {32, 4, rhi::Format::float32_1}},
	    {"j", {40, 4, rhi::Format::float32_1}},
	    {"k", {44, 4, rhi::Format::float32_1}},
	    {"l", {52, 4, rhi::Format::float32_1}},
	    {"m", {56, 4, rhi::Format::float32_1}},
	    {"n", {64, 4, rhi::Format::float32_1}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, std430_ssbo_example4_test)
{
	rhi::ShaderBufferTemplate sb{"Example4", rhi::ShaderBufferType::ubo, rhi::Layout::std430};

	sb.add_entry("a", rhi::Format::float32_1, 1);
	sb.add_entry("b", rhi::Format::float32_3, 1);
	sb.add_entry("c", rhi::Format::float32_3, 1);
	sb.add_entry("d", rhi::Format::float32_3, 1);
	sb.add_entry("e", rhi::Format::float32_3, 1);
	sb.add_entry("g", rhi::Format::float32_3, 1);
	sb.add_entry("h", rhi::Format::float32_3, 1);
	sb.add_entry("i", rhi::Format::float32_3, 1);
	sb.add_entry("j", rhi::Format::float32_3, 1);
	sb.add_entry("k", rhi::Format::float32_3, 2);
	sb.add_entry("l", rhi::Format::float32_3, 1);
	sb.add_entry("m", rhi::Format::float32_3, 2);
	sb.add_entry("n", rhi::Format::float32_3, 1);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, rhi::Format::float32_1}},
	    {"b", {16, 12, rhi::Format::float32_3}},
	    {"c", {32, 12, rhi::Format::float32_3}},
	    {"d", {48, 12, rhi::Format::float32_3}},
	    {"e", {64, 12, rhi::Format::float32_3}},
	    {"g", {80, 12, rhi::Format::float32_3}},
	    {"h", {96, 12, rhi::Format::float32_3}},
	    {"i", {112, 12, rhi::Format::float32_3}},
	    {"j", {128, 12, rhi::Format::float32_3}},
	    {"k", {144, 16, rhi::Format::float32_3}},
	    {"l", {176, 12, rhi::Format::float32_3}},
	    {"m", {192, 16, rhi::Format::float32_3}},
	    {"n", {224, 12, rhi::Format::float32_3}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, std430_ssbo_example5_test)
{
	rhi::ShaderBufferTemplate sb{"Example5", rhi::ShaderBufferType::ubo, rhi::Layout::std430};

	sb.add_entry("a", rhi::Format::float32_1, 1);
	sb.add_entry("b", rhi::Format::float32_3, 3);
	sb.add_entry("c", rhi::Format::float32_3, 4);
	sb.add_entry("d", rhi::Format::float32_3, 2);
	sb.add_entry("e", rhi::Format::float32_3, 2);
	sb.add_entry("g", rhi::Format::float32_3, 2);
	sb.add_entry("h", rhi::Format::float32_3, 2);
	sb.add_entry("i", rhi::Format::float32_3, 2);
	sb.add_entry("j", rhi::Format::float32_3, 2);
	sb.add_entry("k", rhi::Format::float32_3, 2);
	sb.add_entry("l", rhi::Format::float32_3, 2);
	sb.add_entry("m", rhi::Format::float32_3, 2);
	sb.add_entry("n", rhi::Format::float32_3, 2);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, rhi::Format::float32_1}},
	    {"b", {16, 16, rhi::Format::float32_3}},
	    {"c", {64, 16, rhi::Format::float32_3}},
	    {"d", {128, 16, rhi::Format::float32_3}},
	    {"e", {160, 16, rhi::Format::float32_3}},
	    {"g", {192, 16, rhi::Format::float32_3}},
	    {"h", {224, 16, rhi::Format::float32_3}},
	    {"i", {256, 16, rhi::Format::float32_3}},
	    {"j", {288, 16, rhi::Format::float32_3}},
	    {"k", {320, 16, rhi::Format::float32_3}},
	    {"l", {352, 16, rhi::Format::float32_3}},
	    {"m", {384, 16, rhi::Format::float32_3}},
	    {"n", {416, 16, rhi::Format::float32_3}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, std430_ssbo_example6_test)
{
	rhi::ShaderBufferTemplate sb{"Example6", rhi::ShaderBufferType::ubo, rhi::Layout::std430};

	sb.add_entry("a", rhi::Format::float32_2x3, 1);
	sb.add_entry("b", rhi::Format::float32_2x3, 1);
	sb.add_entry("c", rhi::Format::float32_2x3, 1);
	sb.add_entry("d", rhi::Format::float32_2x3, 1);
	sb.add_entry("e", rhi::Format::float32_2x3, 1);
	sb.add_entry("g", rhi::Format::float32_2x3, 2);
	sb.add_entry("h", rhi::Format::float32_2x3, 1);
	sb.add_entry("i", rhi::Format::float32_2x3, 2);
	sb.add_entry("j", rhi::Format::float32_2x3, 1);
	sb.add_entry("k", rhi::Format::float32_2x3, 1);
	sb.add_entry("l", rhi::Format::float32_2x3, 2);
	sb.add_entry("m", rhi::Format::float32_2x3, 1);
	sb.add_entry("n", rhi::Format::float32_2x3, 1);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 32, rhi::Format::float32_2x3}},
	    {"b", {32, 32, rhi::Format::float32_2x3}},
	    {"c", {64, 32, rhi::Format::float32_2x3}},
	    {"d", {96, 32, rhi::Format::float32_2x3}},
	    {"e", {128, 32, rhi::Format::float32_2x3}},
	    {"g", {160, 32, rhi::Format::float32_2x3}},
	    {"h", {224, 32, rhi::Format::float32_2x3}},
	    {"i", {256, 32, rhi::Format::float32_2x3}},
	    {"j", {320, 32, rhi::Format::float32_2x3}},
	    {"k", {352, 32, rhi::Format::float32_2x3}},
	    {"l", {384, 32, rhi::Format::float32_2x3}},
	    {"m", {448, 32, rhi::Format::float32_2x3}},
	    {"n", {480, 32, rhi::Format::float32_2x3}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, std430_ssbo_example7_test)
{
	rhi::ShaderBufferTemplate sb{"Example7", rhi::ShaderBufferType::ubo, rhi::Layout::std430};

	sb.add_entry("a", rhi::Format::float32_1, 1);
	sb.add_entry("b", rhi::Format::float32_2x3, 1);
	sb.add_entry("c", rhi::Format::float32_2x3, 1);
	sb.add_entry("d", rhi::Format::float32_2x3, 1);
	sb.add_entry("e", rhi::Format::float32_2x3, 1);
	sb.add_entry("g", rhi::Format::float32_2x3, 2);
	sb.add_entry("h", rhi::Format::float32_2x3, 1);
	sb.add_entry("i", rhi::Format::float32_2x3, 2);
	sb.add_entry("j", rhi::Format::float32_2x3, 1);
	sb.add_entry("k", rhi::Format::float32_2x3, 1);
	sb.add_entry("l", rhi::Format::float32_2x3, 2);
	sb.add_entry("m", rhi::Format::float32_2x3, 1);
	sb.add_entry("n", rhi::Format::float32_2x3, 1);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 4, rhi::Format::float32_1}},
	    {"b", {16, 32, rhi::Format::float32_2x3}},
	    {"c", {48, 32, rhi::Format::float32_2x3}},
	    {"d", {80, 32, rhi::Format::float32_2x3}},
	    {"e", {112, 32, rhi::Format::float32_2x3}},
	    {"g", {144, 32, rhi::Format::float32_2x3}},
	    {"h", {208, 32, rhi::Format::float32_2x3}},
	    {"i", {240, 32, rhi::Format::float32_2x3}},
	    {"j", {304, 32, rhi::Format::float32_2x3}},
	    {"k", {336, 32, rhi::Format::float32_2x3}},
	    {"l", {368, 32, rhi::Format::float32_2x3}},
	    {"m", {432, 32, rhi::Format::float32_2x3}},
	    {"n", {464, 32, rhi::Format::float32_2x3}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, std430_ssbo_example8_test)
{
	rhi::ShaderBufferTemplate sb{"Example8", rhi::ShaderBufferType::ubo, rhi::Layout::std430};

	sb.add_entry("a", rhi::Format::float32_3x2, 1);
	sb.add_entry("b", rhi::Format::float32_3x2, 1);
	sb.add_entry("c", rhi::Format::float32_3x2, 1);
	sb.add_entry("d", rhi::Format::float32_3x2, 1);
	sb.add_entry("e", rhi::Format::float32_3x2, 1);
	sb.add_entry("g", rhi::Format::float32_3x2, 2);
	sb.add_entry("h", rhi::Format::float32_3x2, 1);
	sb.add_entry("i", rhi::Format::float32_3x2, 2);
	sb.add_entry("j", rhi::Format::float32_3x2, 1);
	sb.add_entry("k", rhi::Format::float32_3x2, 1);
	sb.add_entry("l", rhi::Format::float32_3x2, 2);
	sb.add_entry("m", rhi::Format::float32_3x2, 1);
	sb.add_entry("n", rhi::Format::float32_3x2, 1);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 24, rhi::Format::float32_3x2}},
	    {"b", {24, 24, rhi::Format::float32_3x2}},
	    {"c", {48, 24, rhi::Format::float32_3x2}},
	    {"d", {72, 24, rhi::Format::float32_3x2}},
	    {"e", {96, 24, rhi::Format::float32_3x2}},
	    {"g", {120, 24, rhi::Format::float32_3x2}},
	    {"h", {168, 24, rhi::Format::float32_3x2}},
	    {"i", {192, 24, rhi::Format::float32_3x2}},
	    {"j", {240, 24, rhi::Format::float32_3x2}},
	    {"k", {264, 24, rhi::Format::float32_3x2}},
	    {"l", {288, 24, rhi::Format::float32_3x2}},
	    {"m", {336, 24, rhi::Format::float32_3x2}},
	    {"n", {360, 24, rhi::Format::float32_3x2}}};

	test_entries(sb, var_values);
}

void test_double_matrix_entries(rhi::ShaderBufferTemplate &sb)
{
	sb.add_entry("a", rhi::Format::float64_2x2, 1);
	sb.add_entry("b", rhi::Format::float64_2x2, 1);
	sb.add_entry("c", rhi::Format::float64_2x2, 1);
	sb.add_entry("d", rhi::Format::float64_2x2, 1);
	sb.add_entry("e", rhi::Format::float64_2x2, 1);
	sb.add_entry("g", rhi::Format::float64_2x2, 2);
	sb.add_entry("h", rhi::Format::float64_2x2, 1);
	sb.add_entry("i", rhi::Format::float64_2x2, 2);
	sb.add_entry("j", rhi::Format::float64_2x2, 1);
	sb.add_entry("k", rhi::Format::float64_2x2, 1);
	sb.add_entry("l", rhi::Format::float64_2x2, 2);
	sb.add_entry("m", rhi::Format::float64_2x2, 1);
	sb.add_entry("n", rhi::Format::float64_2x2, 1);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 32, rhi::Format::float64_2x2}},
	    {"b", {32, 32, rhi::Format::float64_2x2}},
	    {"c", {64, 32, rhi::Format::float64_2x2}},
	    {"d", {96, 32, rhi::Format::float64_2x2}},
	    {"e", {128, 32, rhi::Format::float64_2x2}},
	    {"g", {160, 32, rhi::Format::float64_2x2}},
	    {"h", {224, 32, rhi::Format::float64_2x2}},
	    {"i", {256, 32, rhi::Format::float64_2x2}},
	    {"j", {320, 32, rhi::Format::float64_2x2}},
	    {"k", {352, 32, rhi::Format::float64_2x2}},
	    {"l", {384, 32, rhi::Format::float64_2x2}},
	    {"m", {448, 32, rhi::Format::float64_2x2}},
	    {"n", {480, 32, rhi::Format::float64_2x2}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, std140_ubo_example9_test)
{
	rhi::ShaderBufferTemplate sb{"Example9", rhi::ShaderBufferType::ubo, rhi::Layout::std140};
	test_double_matrix_entries(sb);
}

TEST(ShaderBuffer, std430_ssbo_example9_test)
{
	rhi::ShaderBufferTemplate sb{"Example9", rhi::ShaderBufferType::ubo, rhi::Layout::std430};
	test_double_matrix_entries(sb);
}

void test_double_diff_matrix_entries(rhi::ShaderBufferTemplate &sb)
{
	sb.add_entry("a", rhi::Format::float64_2x2, 1);
	sb.add_entry("b", rhi::Format::float64_3x2, 1);
	sb.add_entry("c", rhi::Format::float64_2x2, 1);
	sb.add_entry("d", rhi::Format::float64_2x2, 1);
	sb.add_entry("e", rhi::Format::float64_2x3, 1);
	sb.add_entry("g", rhi::Format::float64_2x2, 2);
	sb.add_entry("h", rhi::Format::float64_4x2, 1);
	sb.add_entry("i", rhi::Format::float64_2x2, 2);
	sb.add_entry("j", rhi::Format::float64_2x4, 1);
	sb.add_entry("k", rhi::Format::float64_4x4, 1);
	sb.add_entry("l", rhi::Format::float64_4x4, 2);
	sb.add_entry("m", rhi::Format::float64_2x2, 1);
	sb.add_entry("n", rhi::Format::float64_2x2, 1);
}

TEST(ShaderBuffer, std140_ubo_example10_test)
{
	rhi::ShaderBufferTemplate sb{"Example10", rhi::ShaderBufferType::ubo, rhi::Layout::std140};
	test_double_diff_matrix_entries(sb);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 32, rhi::Format::float64_2x2}},
	    {"b", {32, 48, rhi::Format::float64_3x2}},
	    {"c", {80, 32, rhi::Format::float64_2x2}},
	    {"d", {112, 32, rhi::Format::float64_2x2}},
	    {"e", {160, 64, rhi::Format::float64_2x3}},
	    {"g", {224, 32, rhi::Format::float64_2x2}},
	    {"h", {288, 64, rhi::Format::float64_4x2}},
	    {"i", {352, 32, rhi::Format::float64_2x2}},
	    {"j", {416, 64, rhi::Format::float64_2x4}},
	    {"k", {480, 128, rhi::Format::float64_4x4}},
	    {"l", {608, 128, rhi::Format::float64_4x4}},
	    {"m", {864, 32, rhi::Format::float64_2x2}},
	    {"n", {896, 32, rhi::Format::float64_2x2}}};

	test_entries(sb, var_values);
}

TEST(ShaderBuffer, std430_ssbo_example10_test)
{
	rhi::ShaderBufferTemplate sb{"Example10", rhi::ShaderBufferType::ubo, rhi::Layout::std430};
	test_double_diff_matrix_entries(sb);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 32, rhi::Format::float64_2x2}},
	    {"b", {32, 48, rhi::Format::float64_3x2}},
	    {"c", {80, 32, rhi::Format::float64_2x2}},
	    {"d", {112, 32, rhi::Format::float64_2x2}},
	    {"e", {160, 64, rhi::Format::float64_2x3}},
	    {"g", {224, 32, rhi::Format::float64_2x2}},
	    {"h", {288, 64, rhi::Format::float64_4x2}},
	    {"i", {352, 32, rhi::Format::float64_2x2}},
	    {"j", {416, 64, rhi::Format::float64_2x4}},
	    {"k", {480, 128, rhi::Format::float64_4x4}},
	    {"l", {608, 128, rhi::Format::float64_4x4}},
	    {"m", {864, 32, rhi::Format::float64_2x2}},
	    {"n", {896, 32, rhi::Format::float64_2x2}}};

	test_entries(sb, var_values);
}

void test_double_diff2_matrix_entries(rhi::ShaderBufferTemplate &sb)
{
	sb.add_entry("a", rhi::Format::float32_3x4, 1);
	sb.add_entry("b", rhi::Format::float32_3x4, 1);
	sb.add_entry("c", rhi::Format::float32_3x4, 1);
	sb.add_entry("d", rhi::Format::float64_3x4, 1);
	sb.add_entry("e", rhi::Format::float64_3x4, 1);
	sb.add_entry("g", rhi::Format::float32_2x2, 2);
	sb.add_entry("h", rhi::Format::float32_4x2, 1);
	sb.add_entry("i", rhi::Format::float32_2x2, 2);
	sb.add_entry("j", rhi::Format::float32_2x4, 1);
	sb.add_entry("k", rhi::Format::float32_4x4, 1);
	sb.add_entry("l", rhi::Format::float32_4x4, 2);
	sb.add_entry("m", rhi::Format::float32_2x2, 1);
	sb.add_entry("n", rhi::Format::float32_2x2, 1);
}

TEST(ShaderBuffer, std140_ubo_example11_test)
{
	rhi::ShaderBufferTemplate sb{"Example11", rhi::ShaderBufferType::ubo, rhi::Layout::std140};
	test_double_diff2_matrix_entries(sb);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 48, rhi::Format::float32_3x4}},
	    {"b", {48, 48, rhi::Format::float32_3x4}},
	    {"c", {96, 48, rhi::Format::float32_3x4}},
	    {"d", {160, 96, rhi::Format::float64_3x4}},
	    {"e", {256, 96, rhi::Format::float64_3x4}},
	    {"g", {352, 16, rhi::Format::float32_2x2}},
	    {"h", {384, 32, rhi::Format::float32_4x2}},
	    {"i", {416, 16, rhi::Format::float32_2x2}},
	    {"j", {448, 32, rhi::Format::float32_2x4}},
	    {"k", {480, 64, rhi::Format::float32_4x4}},
	    {"l", {544, 64, rhi::Format::float32_4x4}},
	    {"m", {672, 16, rhi::Format::float32_2x2}},
	    {"n", {688, 16, rhi::Format::float32_2x2}}};
	test_entries(sb, var_values);
}

TEST(ShaderBuffer, std430_ssbo_example11_test)
{
	rhi::ShaderBufferTemplate sb{"Example11", rhi::ShaderBufferType::ubo, rhi::Layout::std430};
	test_double_diff2_matrix_entries(sb);

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"a", {0, 48, rhi::Format::float32_3x4}},
	    {"b", {48, 48, rhi::Format::float32_3x4}},
	    {"c", {96, 48, rhi::Format::float32_3x4}},
	    {"d", {160, 96, rhi::Format::float64_3x4}},
	    {"e", {256, 96, rhi::Format::float64_3x4}},
	    {"g", {352, 16, rhi::Format::float32_2x2}},
	    {"h", {384, 32, rhi::Format::float32_4x2}},
	    {"i", {416, 16, rhi::Format::float32_2x2}},
	    {"j", {448, 32, rhi::Format::float32_2x4}},
	    {"k", {480, 64, rhi::Format::float32_4x4}},
	    {"l", {544, 64, rhi::Format::float32_4x4}},
	    {"m", {672, 16, rhi::Format::float32_2x2}},
	    {"n", {688, 16, rhi::Format::float32_2x2}}};

	test_entries(sb, var_values);
}

void joint_transforms_test(uint32_t joints_count)
{
	// Needs to create something like the following
	/*
	  struct trs_transform
	  {
	      vec4 rotation;
	      vec3 translation;
	      vec3 scale;
	  };
	  layout(std140, set = 2, binding = 0) uniform joint_transform
	  {
	      trs_transform joint_transforms[24];
	  } in_joint_transforms;
	*/
	rhi::ShaderBufferTemplate::Struct trs_transform("joint_transforms", joints_count);

	trs_transform.add_entry("rotation", rhi::Format::float32_4, rhi::Layout::std140, 1);
	trs_transform.add_entry("translation", rhi::Format::float32_3, rhi::Layout::std140, 1);
	trs_transform.add_entry("scale", rhi::Format::float32_3, rhi::Layout::std140, 1);

	rhi::ShaderBufferTemplate joint_transform{"joint_transform", rhi::ShaderBufferType::ubo, rhi::Layout::std140, 2, 0};

	joint_transform.add_struct(trs_transform);

	rhi::ShaderBufferMetal joint_transform_sbm{"joint_transform", rhi::ShaderBufferType::ubo, rhi::Layout::std140, 2u, 0u,
	                           "joint_transforms", rhi::VertexFormat::struct_1, joints_count,
	                           "rotation", rhi::VertexFormat::float32_4, 1u,
	                           "translation", rhi::VertexFormat::float32_3, 1u,
	                           "scale", rhi::VertexFormat::float32_3, 1u};

	// Index of offset, stride, type
	std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, rhi::Format>> var_values{
	    {"rotation", {0, 16, rhi::Format::float32_4}},
	    {"translation", {16, 12, rhi::Format::float32_3}},
	    {"scale", {32, 12, rhi::Format::float32_3}},
	    {"joint_transforms", {0, 48, rhi::Format::struct_1}}};

	test_entries(joint_transform, var_values);

	auto        output0 = joint_transform_sbm.to_glsl_string();
	auto        output = joint_transform.to_glsl_string();
	std::string expected_output{R"output(struct Joint_transforms
{
	vec4 rotation;
	vec3 translation;
	vec3 scale;
};

layout(std140, set = 2, binding = 0) uniform joint_transform
{
	Joint_transforms joint_transforms[)output"};

	auto expected_output2{R"output(];
} in_joint_transform;
)output"};

	if (joints_count != 0)
		expected_output.append(std::to_string(joints_count));

	expected_output.append(expected_output2);

	EXPECT_EQ(output0, expected_output);
	EXPECT_EQ(output, expected_output);
}

TEST(ShaderBuffer, std140_joint_transforms_test)
{
	joint_transforms_test(0);
	joint_transforms_test(24);
}

}        // namespace ror_test
