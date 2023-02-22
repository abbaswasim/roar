#include "bounds/rorbounding.hpp"
#include "camera/rorcamera.hpp"
#include "common.hpp"
#include "configuration/rorsettings_configuration.hpp"
#include "foundation/rorcommon.hpp"
#include "foundation/rorcrtp.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "graphics/rorlight.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormesh.hpp"
#include "math/rorquaternion.hpp"
#include "math/rortransform.hpp"
#include "math/rorvector3.hpp"
#include "profiling/rorlog.hpp"
#include "profiling/rortimer.hpp"
#include <atomic>
#include <cstdint>
#include <filesystem>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "couldron_0.hpp"
#include "fox.h"
#include "graphics/rormodel.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rorrender_data.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_attribute.hpp"
#include "rhi/rorvertex_description.hpp"
#include "shader_system/rorshader_system.hpp"

#include "gltf.hpp"

namespace ror_test
{

rhi::BuffersPack           *GLTFTest::bp = nullptr;
std::shared_ptr<ror::Model> GLTFTest::fox_model;
std::shared_ptr<ror::Model> GLTFTest::couldron0_model;

template <typename _type>
void compare_attribute_values(const rhi::VertexDescriptor &a_vd, rhi::BufferSemantic a_semantic, rhi::VertexFormat a_format, uint32_t a_components, const _type *a_reference, uint32_t a_attrib_count, rhi::BuffersPack *a_bp, uint32_t a_line)
{
	(void) a_components;
	(void) a_reference;
	(void) a_format;
	(void) a_line;

	// std::string line_header{"Looking at line = "};
	// line_header += std::to_string(a_line);
	// print_with_gtest_header(line_header.c_str(), green);

	const float epsilon = 0.0001f;
	{
		auto          &attrib        = a_vd.attribute(a_semantic);
		auto          &layout        = a_vd.layout(a_semantic);
		auto          &buffer        = a_bp->buffer(a_semantic);
		auto           stride        = layout.stride();
		const uint8_t *attrib_values = buffer.data().data();

		const uint8_t *ptr = attrib_values + attrib.buffer_offset() + attrib.offset();
		for (size_t i = 0; i < a_attrib_count; ++i)
		{
			for (size_t j = 0; j < a_components; ++j)
				EXPECT_NEAR(a_reference[i * a_components + j], reinterpret_cast<const _type *>(ptr)[j], epsilon);

			ptr += stride;
		}
	}
}

void GLTFTest::SetUp()
{}

void GLTFTest::TearDown()
{}

static bool loaded_fox = false;

TEST_F(GLTFTest, fox_gltf_loader_test)
{
	(void) fox_attrib_count;
	(void) fox_positions;
	(void) fox_uvs;
	(void) fox_joint_ids;
	(void) fox_weights;
	(void) fox_inverse_bind_matrices_count;
	(void) fox_inverse_bind_matrices;
	(void) fox_joints;
	(void) fox_sampler0_count;
	(void) fox_sampler0_input;
	(void) fox_sampler0_output;

	std::vector<ror::OrbitCamera> fox_cameras;
	std::vector<ror::Light>       fox_lights;

	this->fox_model->load_from_gltf_file("Fox/Fox.gltf", fox_cameras, fox_lights, true, *this->bp);
	loaded_fox = true;

	EXPECT_EQ(fox_cameras.size(), 0);

	EXPECT_EQ(this->fox_model->meshes().size(), 1);
	auto &m = this->fox_model->meshes()[0];
	EXPECT_EQ(m.primitives_count(), 1);
	EXPECT_EQ(m.target_descriptor(0).size(), 0);
	EXPECT_EQ(m.has_indices(0), false);
	EXPECT_EQ(m.primitive_type(0), rhi::PrimitiveTopology::triangles);

	auto &vd = m.vertex_descriptor(0);

	auto semantics = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0) |
	                 ror::enum_to_type_cast(rhi::BufferSemantic::vertex_bone_id_0) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_weight_0);

	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4 + sizeof(uint16_t) * 4, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 1, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_bone_id_0, 2, sizeof(float32_t) * 3, 0, 2, 0, rhi::VertexFormat::uint16_4, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4 + sizeof(uint16_t) * 4, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, sizeof(float32_t) * 3 + sizeof(uint16_t) * 4, 0, 3, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4 + sizeof(uint16_t) * 4, 1, 1, semantics, __LINE__);

	auto &attrib_pos = vd.attribute(rhi::BufferSemantic::vertex_position);
	test_one_vertex_attribute(attrib_pos, 0, 0, 1728, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3, __LINE__);
	auto &attrib_uv = vd.attribute(rhi::BufferSemantic::vertex_texture_coord_0);
	test_one_vertex_attribute(attrib_uv, 1, 0, 1728, 0, 1, 1, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2, __LINE__);
	auto &attrib_bone = vd.attribute(rhi::BufferSemantic::vertex_bone_id_0);
	test_one_vertex_attribute(attrib_bone, 2, sizeof(float32_t) * 3, 1728, 0, 2, 0, rhi::BufferSemantic::vertex_bone_id_0, rhi::VertexFormat::uint16_4, __LINE__);
	auto &attrib_weight = vd.attribute(rhi::BufferSemantic::vertex_weight_0);
	test_one_vertex_attribute(attrib_weight, 3, sizeof(float32_t) * 3 + sizeof(uint16_t) * 4, 1728, 0, 3, 0, rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_4, __LINE__);

	// Now lets see if the attributes data is copied correctly or not
	compare_attribute_values<float32_t>(vd, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3, 3, fox_positions, fox_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(vd, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2, 2, fox_uvs, fox_attrib_count, this->bp, __LINE__);
	compare_attribute_values<uint16_t>(vd, rhi::BufferSemantic::vertex_bone_id_0, rhi::VertexFormat::float32_4, 4, fox_joint_ids, fox_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(vd, rhi::BufferSemantic::vertex_weight_0, rhi::VertexFormat::float32_4, 4, fox_weights, fox_attrib_count, this->bp, __LINE__);

	const float epsilon = 0.000001f;
	{
		EXPECT_EQ(this->fox_model->skins().size(), 1);
		auto &s        = this->fox_model->skins()[0];
		auto &matrices = s.inverse_bind_matrices();
		auto &joints   = s.joints();

		EXPECT_EQ(matrices.size(), fox_inverse_bind_matrices_count);
		EXPECT_EQ(joints.size(), fox_inverse_bind_matrices_count);

		for (size_t i = 0; i < matrices.size(); ++i)
			for (size_t j = 0; j < 16; ++j)
				EXPECT_NEAR(matrices[i].m_values[j], fox_inverse_bind_matrices[i * 16 + j], epsilon);

		for (size_t i = 0; i < joints.size(); ++i)
			EXPECT_EQ(joints[i], fox_joints[i]);

		EXPECT_EQ(s.root(), 2);
		EXPECT_EQ(s.node_index(), 1);
	}
	{
		EXPECT_EQ(this->fox_model->animations().size(), 3);
		auto &a = this->fox_model->animations()[0];

		EXPECT_EQ(a.m_samplers.size(), 21);
		EXPECT_EQ(a.m_channels.size(), 21);

		auto &s = a.m_samplers[0];
		auto &i = s.m_input;
		auto *o = reinterpret_cast<const float32_t *>(s.m_output.data());

		EXPECT_EQ(i.size(), fox_sampler0_count);
		EXPECT_EQ(s.m_output.size(), fox_sampler0_count * 4 * 4);

		for (size_t j = 0; j < i.size(); ++j)
		{
			EXPECT_NEAR(i[j].m_value, fox_sampler0_input[j], epsilon);

			EXPECT_NEAR(o[j * 4 + 0], fox_sampler0_output[j * 4 + 0], epsilon);
			EXPECT_NEAR(o[j * 4 + 1], fox_sampler0_output[j * 4 + 1], epsilon);
			EXPECT_NEAR(o[j * 4 + 2], fox_sampler0_output[j * 4 + 2], epsilon);
			EXPECT_NEAR(o[j * 4 + 3], fox_sampler0_output[j * 4 + 3], epsilon);
		}
	}
}

TEST_F(GLTFTest, couldron0_gltf_loader_test)
{
	(void) fox_attrib_count;
	(void) fox_positions;
	(void) fox_uvs;
	(void) fox_joint_ids;
	(void) fox_weights;
	(void) fox_inverse_bind_matrices_count;
	(void) fox_inverse_bind_matrices;
	(void) fox_joints;
	(void) fox_sampler0_count;
	(void) fox_sampler0_input;
	(void) fox_sampler0_output;

	std::vector<ror::OrbitCamera> couldron0_cameras;
	std::vector<ror::Light> couldron0_lights;

	this->couldron0_model->load_from_gltf_file("baba_yagas_hut/scene.gltf", couldron0_cameras, couldron0_lights, true, *this->bp);

	EXPECT_EQ(couldron0_cameras.size(), 0);

	EXPECT_GT(this->couldron0_model->meshes().size(), 1);
	auto &m = this->couldron0_model->meshes()[0];
	EXPECT_EQ(m.primitives_count(), 1);
	EXPECT_EQ(m.target_descriptor(0).size(), 3);
	EXPECT_EQ(m.has_indices(0), true);
	EXPECT_EQ(m.primitive_type(0), rhi::PrimitiveTopology::triangles);

	auto &vd  = m.vertex_descriptor(0);
	auto &tvd = m.target_descriptor(0);

	auto semantics = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_normal) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position) |
	                 ror::enum_to_type_cast(rhi::BufferSemantic::vertex_tangent) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_index);

	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_normal, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 1, 0, 0, 1, 1, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_tangent, 2, 0, 0, 2, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::vertex, sizeof(float32_t) * 4, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 3, 0, 0, 3, 0, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2, 1, 1, semantics, __LINE__);

	auto &attrib_normal = vd.attribute(rhi::BufferSemantic::vertex_normal);
	test_one_vertex_attribute(attrib_normal, 0, 0, 948, (loaded_fox ? 36576 : 22752), 0, 1, rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3, __LINE__);
	auto &attrib_position = vd.attribute(rhi::BufferSemantic::vertex_position);
	test_one_vertex_attribute(attrib_position, 1, 0, 948, (loaded_fox ? 62208 : 0), 1, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3, __LINE__);
	auto &attrib_tangent = vd.attribute(rhi::BufferSemantic::vertex_tangent);
	test_one_vertex_attribute(attrib_tangent, 2, 0, 948, (loaded_fox ? 21408 : 7584), 2, 1, rhi::BufferSemantic::vertex_tangent, rhi::VertexFormat::float32_4, __LINE__);
	auto &attrib_uv = vd.attribute(rhi::BufferSemantic::vertex_texture_coord_0);
	test_one_vertex_attribute(attrib_uv, 3, 0, 948, (loaded_fox ? 13824 : 0), 3, 1, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2, __LINE__);

	// Now lets see if the attributes data is copied correctly or not
	compare_attribute_values<float32_t>(vd, rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3, 3, couldron0_vertex_NORMAL, couldron0_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(vd, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3, 3, couldron0_vertex_POSITION, couldron0_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(vd, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2, 2, couldron0_vertex_TEXCOORD_0, couldron0_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(vd, rhi::BufferSemantic::vertex_tangent, rhi::VertexFormat::float32_4, 4, couldron0_vertex_TANGENT, couldron0_attrib_count, this->bp, __LINE__);

	compare_attribute_values<uint32_t>(vd, rhi::BufferSemantic::vertex_index, rhi::VertexFormat::uint32_1, 1, couldron0_vertex_INDEX, couldron0_vertex_index_count, this->bp, __LINE__);

	compare_attribute_values<float32_t>(tvd[0], rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3, 3, couldron0_target_NORMAL0, couldron0_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(tvd[0], rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3, 3, couldron0_target_POSITION0, couldron0_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(tvd[0], rhi::BufferSemantic::vertex_tangent, rhi::VertexFormat::float32_3, 3, couldron0_target_TANGENT0, couldron0_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(tvd[1], rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3, 3, couldron0_target_NORMAL1, couldron0_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(tvd[1], rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3, 3, couldron0_target_POSITION1, couldron0_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(tvd[1], rhi::BufferSemantic::vertex_tangent, rhi::VertexFormat::float32_3, 3, couldron0_target_TANGENT1, couldron0_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(tvd[2], rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3, 3, couldron0_target_NORMAL2, couldron0_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(tvd[2], rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_3, 3, couldron0_target_POSITION2, couldron0_attrib_count, this->bp, __LINE__);
	compare_attribute_values<float32_t>(tvd[2], rhi::BufferSemantic::vertex_tangent, rhi::VertexFormat::float32_3, 3, couldron0_target_TANGENT2, couldron0_attrib_count, this->bp, __LINE__);
}

TEST_F(GLTFTest, gltf_assert_test)
{
	// Since I can only call "free()" once on buffer_pack otherwise it asserts (a good thing)
	// TODO: Check for asserting and catching in tests
	std::vector<ror::Mesh, rhi::BufferAllocator<ror::Mesh>> *alloc_buffer = new std::vector<ror::Mesh, rhi::BufferAllocator<ror::Mesh>>();
	alloc_buffer->reserve(2);        // This shouldn't assert
	ror::Mesh m1;
	ror::Mesh m2;
	alloc_buffer->push_back(std::move(m1));
	alloc_buffer->push_back(std::move(m2));

	// Also some random calls to make the compiler happy about [-Werror,-Wunused-function]
	auto os    = ror::get_os_string();
	auto comp  = ror::get_compiler_string();
	auto arch  = ror::get_arch_string();
	auto build = ror::get_build_string();

	(void) os;
	(void) comp;
	(void) arch;
	(void) build;
}

void compile_print_shader_result(const std::string &a_shader_source, const std::string &a_shader_name, bool a_print_shader_source)
{
	std::filesystem::path tmp_file = std::filesystem::temp_directory_path() / a_shader_name;
	tmp_file                       = std::filesystem::absolute(tmp_file);

	std::ios_base::openmode mode = std::ios::out;
	{
		std::ofstream of(tmp_file, mode);
		of << a_shader_source;
	}

	std::string command{"glslangValidator -V  "};
	command.append(tmp_file);

	auto res = ror_test::execute_command(command.c_str());
	if (res.second != EXIT_SUCCESS)
	{
		std::stringstream ss{a_shader_source};
		std::string       to;
		uint32_t          linnum = 1;
		while (std::getline(ss, to, '\n'))
		{
			std::cout << linnum++ << " : " << to << std::endl;
		}

		ror::log_critical("Created bad shader source code, glslangValidator says {}\n", res.first);
	}
	else
	{
		if (a_print_shader_source)
			ror::log_warn("Vertex shader source code\n{}", a_shader_source.c_str());
	}
}

void GLTFTest::load_model(std::string path)
{
	auto                          compile_shader_source = settings_configs_copy.get<bool>("gltf_compile_shader");
	auto                          print_shader_source   = settings_configs_copy.get<bool>("gltf_compile_print_shader");
	std::vector<ror::OrbitCamera> model_cameras;
	std::vector<ror::Light> model_lights;
	ror::Model                   *model2 = new ror::Model();

	model2->load_from_gltf_file(path, model_cameras, model_lights, true, *this->bp);

	if (print_shader_source)
		compile_shader_source = true;

	uint32_t         mesh_index = 0;
	std::vector<int> v;
	for (auto &ms : model2->meshes())
	{
		hashes_of_meshs[ms.hash()]++;
		for (size_t j = 0; j < ms.primitives_count(); ++j)
		{
			auto vad_hash = ms.vertex_hash(j);
			hashes_of_prims[vad_hash]++;

			auto mesh_vs = ms.vertex_hash(j);
			auto mesh_fs = ms.fragment_hash(j);
			auto mesh_ps = ms.program_hash(j);

			unique_mesh_vs[mesh_vs]++;
			unique_mesh_fs[mesh_fs]++;
			unique_mesh_ps[mesh_ps]++;

			std::string program_source{""};
			{
				auto vs = ror::generate_primitive_vertex_shader(*model2, static_cast<uint32_t>(mesh_index), static_cast<uint32_t>(j), rhi::RenderpassType::main);
				(void) vs;

				program_source = vs;

				auto hash = ror::hash_64(vs);

				auto mesh_vs_hash = ms.vertex_hash(j);

				auto res = unique_vs_attribs_to_source.emplace(mesh_vs_hash, hash);
				if (!res.second)
				{
					assert(res.first->second == hash && "Source hashes for non-inserted vertex shader aren't the same");
					assert(res.first->first == mesh_vs_hash && "Shouldn't happen");
				}

				hashes_of_vs[hash]++;
				unique_vs[hash]      = vs;
				unique_vattrib[hash] = std::make_pair(mesh_index, j);

				if (compile_shader_source)
					compile_print_shader_result(vs, "tmp_shader.vert", print_shader_source);
			}
			{
				auto fs = ror::generate_primitive_fragment_shader(ms, model2->materials(), static_cast<uint32_t>(j), rhi::RenderpassType::main, true);
				(void) fs;

				program_source.append(fs);

				auto hash = ror::hash_64(fs);

				auto mesh_fs_hash = ms.fragment_hash(j);
				auto res          = unique_fs_attribs_to_source.emplace(mesh_fs_hash, hash);
				if (!res.second)
				{
					assert(res.first->second == hash && "Source hashes for non-inserted fragment shader aren't the same");
					assert(res.first->first == mesh_fs_hash && "Shouldn't happen");
				}

				hashes_of_fs[hash]++;
				unique_fs[hash] = fs;

				auto prog_hash = ror::hash_64(program_source);

				unique_ps[prog_hash] = program_source;
				hashes_of_ps[prog_hash]++;

				if (compile_shader_source)
					compile_print_shader_result(fs, "tmp_shader.frag", print_shader_source);
			}
		}
		++mesh_index;
	}
	// Lets save all material hashes in this model
	for (auto &mat : model2->materials())
		hashes_of_mats[mat.m_hash]++;
}

void GLTFTest::print_hashes()
{
	std::cout << "\nAll vertex shader hashes are \n";
	for (auto &hash : hashes_of_vs)
		std::cout << hash.first << ":" << hash.second << std::endl;

	std::cout << "\nAll fragment shader hashes are \n";
	for (auto &hash : hashes_of_fs)
		std::cout << hash.first << ":" << hash.second << std::endl;

	std::cout << "\nAll program shader hashes are \n";
	for (auto &hash : hashes_of_ps)
		std::cout << hash.first << ":" << hash.second << std::endl;

	std::cout << "\nAll material shader hashes are \n";
	for (auto &hash : hashes_of_mats)
		std::cout << hash.first << ":" << hash.second << std::endl;

	std::cout << "\nAll model prims hashes\n"
	          << std::endl;
	for (auto &hash : hashes_of_prims)
		std::cout << hash.first << ":" << hash.second << std::endl;

	std::cout << "\nAll model mesh hashes\n"
	          << std::endl;
	for (auto &hash : hashes_of_meshs)
		std::cout << hash.first << ":" << hash.second << std::endl;

	std::cout << "\nSizes of hashes \nMeshes = " << hashes_of_meshs.size()
	          << "\nPrims = " << hashes_of_prims.size()
	          << "\nMats = " << hashes_of_mats.size()
	          << "\nVS = " << hashes_of_vs.size()
	          << "\nFS = " << hashes_of_fs.size()
	          << "\nPS = " << hashes_of_ps.size()
	          << "\nUVS = " << unique_vs.size()
	          << "\nUFS = " << unique_fs.size()
	          << "\nUPS = " << unique_ps.size()
	          << "\nUMeshVS = " << unique_mesh_vs.size()
	          << "\nUMeshFS = " << unique_mesh_fs.size()
	          << "\nUMeshPS = " << unique_mesh_ps.size()
	          << std::endl;
}

void GLTFTest::write_shaders()
{
	std::filesystem::path root_path = std::filesystem::absolute(std::filesystem::temp_directory_path() / "roar_shaders");

	std::filesystem::remove_all(root_path);

	std::filesystem::create_directory(root_path);

	for (auto &uvs : unique_vs)
	{
		std::filesystem::path tmp_file = root_path / (std::to_string(uvs.first) + ".vert");

		std::cout << "Writing out tmp_vs_file here " << tmp_file << std::endl;

		std::ios_base::openmode mode = std::ios::out;
		{
			std::ofstream of(tmp_file, mode);
			of << uvs.second;
		}
	}

	std::cout << std::endl;
	for (auto &ufs : unique_fs)
	{
		std::filesystem::path tmp_file = root_path / (std::to_string(ufs.first) + ".frag");

		std::cout << "Writing out tmp_fs_file here " << tmp_file << std::endl;

		std::ios_base::openmode mode = std::ios::out;
		{
			std::ofstream of(tmp_file, mode);
			of << ufs.second;
		}
	}
}

TEST_F(GLTFTest, gltf_single_commandline_test)
{
	auto path          = settings_configs_copy.get<std::string>("gltf_input");
	auto write_shaders = settings_configs_copy.get<bool>("gltf_write_shaders");

	if (path != "")
	{
		this->load_model(path);
		this->print_hashes();

		if (write_shaders)
			this->write_shaders();
	}
}

TEST_F(GLTFTest, gltf_load_all_test)
{
	auto                enable_test   = settings_configs_copy.get<bool>("gltf_all");
	auto                write_shaders = settings_configs_copy.get<bool>("gltf_write_shaders");
	auto                root_folder   = settings_configs_copy.get<std::string>("gltf_samples_root");
	ror::SettingsConfig gltf_tests{"gltf_tests.json"};

	if (enable_test)
	{
		if (root_folder == "")
			ror::log_critical("No root folder provided for the samples");
		else
		{
			std::string tests{"gltf_tests:t"};

			for (size_t ti = 0; ti < 202; ++ti)
			{
				auto key{tests + std::to_string(ti)};
				auto test = gltf_tests.get<std::string>(key);

				if (test != "")
				{
					auto file_path{root_folder + "/" + test};
					ror::log_warn("Loading glTF {}", file_path.c_str());
					this->load_model(file_path);
				}
			}

			this->print_hashes();

			if (write_shaders)
				this->write_shaders();
		}
	}
}

}        // namespace ror_test
