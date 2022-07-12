#include "bounds/rorbounding.hpp"
#include "common.hpp"
#include "configuration/rorsettings_configuration.hpp"
#include "foundation/rorcommon.hpp"
#include "foundation/rorcrtp.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
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

#include "fox.h"
#include "graphics/rormodel.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rorrender_data.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_description.hpp"
#include "shader_system/rorshader_system.hpp"

#include "gltf.hpp"

namespace ror_test
{

rhi::BuffersPack           *GLTFTest::bp = nullptr;
std::shared_ptr<ror::Model> GLTFTest::fox_model;

void GLTFTest::SetUp()
{}

void GLTFTest::TearDown()
{}

TEST_F(GLTFTest, gltf_loader_test)
{
	this->fox_model->load_from_gltf_file("Fox/Fox.gltf");

	EXPECT_EQ(this->fox_model->meshes().size(), 1);
	auto &m = this->fox_model->meshes()[0];
	EXPECT_EQ(m.m_attribute_vertex_descriptors.size(), 1);
	EXPECT_EQ(m.m_morph_targets_vertex_descriptors.size(), 1);
	EXPECT_EQ(m.m_has_indices_states[0], false);
	EXPECT_EQ(m.m_primitive_types[0], rhi::PrimitiveTopology::triangles);

	auto &vd = m.m_attribute_vertex_descriptors[0];

	auto semantics = ror::enum_to_type_cast(rhi::BufferSemantic::vertex_position) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0) |
	                 ror::enum_to_type_cast(rhi::BufferSemantic::vertex_bone_id_0) | ror::enum_to_type_cast(rhi::BufferSemantic::vertex_weight_0);

	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_position, 0, 0, 0, 0, 0, rhi::VertexFormat::float32_3, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4 + sizeof(uint16_t) * 4, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_texture_coord_0, 1, 0, 0, 1, 1, rhi::VertexFormat::float32_2, rhi::StepFunction::vertex, sizeof(float32_t) * 2, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_bone_id_0, 2, sizeof(float32_t) * 3, 0, 2, 0, rhi::VertexFormat::uint16_4, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4 + sizeof(uint16_t) * 4, 1, 1, semantics, __LINE__);
	test_one_vertex_description(vd, rhi::BufferSemantic::vertex_weight_0, 3, sizeof(float32_t) * 3 + sizeof(uint16_t) * 4, 0, 3, 0, rhi::VertexFormat::float32_4, rhi::StepFunction::vertex, sizeof(float32_t) * 3 + sizeof(float32_t) * 4 + sizeof(uint16_t) * 4, 1, 1, semantics, __LINE__);

	{
		auto &attrib_pos = m.m_attribute_vertex_descriptors[0].attribute(rhi::BufferSemantic::vertex_position);
		EXPECT_EQ(attrib_pos.location(), 0);
		EXPECT_EQ(attrib_pos.offset(), 0);
		EXPECT_EQ(attrib_pos.buffer_offset(), 0);
		EXPECT_EQ(attrib_pos.binding(), 0);
		EXPECT_EQ(attrib_pos.buffer_index(), 0);
		EXPECT_EQ(attrib_pos.semantics(), rhi::BufferSemantic::vertex_position);
		EXPECT_EQ(attrib_pos.format(), rhi::VertexFormat::float32_3);
	}
	{
		auto &attrib_uv = m.m_attribute_vertex_descriptors[0].attribute(rhi::BufferSemantic::vertex_texture_coord_0);
		EXPECT_EQ(attrib_uv.location(), 1);
		EXPECT_EQ(attrib_uv.offset(), 0);
		EXPECT_EQ(attrib_uv.buffer_offset(), 0);
		EXPECT_EQ(attrib_uv.binding(), 1);
		EXPECT_EQ(attrib_uv.buffer_index(), 1);
		EXPECT_EQ(attrib_uv.semantics(), rhi::BufferSemantic::vertex_texture_coord_0);
		EXPECT_EQ(attrib_uv.format(), rhi::VertexFormat::float32_2);
	}
	{
		auto &attrib_bone = m.m_attribute_vertex_descriptors[0].attribute(rhi::BufferSemantic::vertex_bone_id_0);
		EXPECT_EQ(attrib_bone.location(), 2);
		EXPECT_EQ(attrib_bone.offset(), sizeof(float32_t) * 3);
		EXPECT_EQ(attrib_bone.buffer_offset(), 0);
		EXPECT_EQ(attrib_bone.binding(), 2);
		EXPECT_EQ(attrib_bone.buffer_index(), 0);
		EXPECT_EQ(attrib_bone.semantics(), rhi::BufferSemantic::vertex_bone_id_0);
		EXPECT_EQ(attrib_bone.format(), rhi::VertexFormat::uint16_4);
	}
	{
		auto &attrib_weight = m.m_attribute_vertex_descriptors[0].attribute(rhi::BufferSemantic::vertex_weight_0);
		EXPECT_EQ(attrib_weight.location(), 3);
		EXPECT_EQ(attrib_weight.offset(), sizeof(float32_t) * 3 + sizeof(uint16_t) * 4);
		EXPECT_EQ(attrib_weight.buffer_offset(), 0);
		EXPECT_EQ(attrib_weight.binding(), 3);
		EXPECT_EQ(attrib_weight.buffer_index(), 0);
		EXPECT_EQ(attrib_weight.semantics(), rhi::BufferSemantic::vertex_weight_0);
		EXPECT_EQ(attrib_weight.format(), rhi::VertexFormat::float32_4);
	}

	// Now lets see if the attributes data is copied correctly or not

	const uint8_t *pos_buff    = this->bp->buffer(rhi::BufferSemantic::vertex_position).data().data();
	const uint8_t *uv_buff     = this->bp->buffer(rhi::BufferSemantic::vertex_texture_coord_0).data().data();
	const uint8_t *bone_buff   = this->bp->buffer(rhi::BufferSemantic::vertex_bone_id_0).data().data();
	const uint8_t *weight_buff = this->bp->buffer(rhi::BufferSemantic::vertex_weight_0).data().data();

	const float epsilon = 0.000001f;
	{
		auto  semantic = rhi::BufferSemantic::vertex_position;
		auto &attrib   = m.m_attribute_vertex_descriptors[0].attribute(semantic);
		auto &layout   = m.m_attribute_vertex_descriptors[0].layout(semantic);

		uint32_t comp_count = 3;
		uint8_t *ptr        = const_cast<uint8_t *>(pos_buff) + attrib.buffer_offset() + attrib.offset();        // const_cast only allowed in tests
		for (size_t i = 0; i < fox_attrib_count; ++i)
		{
			for (size_t j = 0; j < comp_count; ++j)
			{
				EXPECT_NEAR(fox_positions[i * comp_count + j], reinterpret_cast<float32_t *>(ptr)[j], epsilon);
			}

			ptr += layout.stride();
		}
	}
	{
		auto     semantic   = rhi::BufferSemantic::vertex_texture_coord_0;
		auto    &attrib     = m.m_attribute_vertex_descriptors[0].attribute(semantic);
		auto    &layout     = m.m_attribute_vertex_descriptors[0].layout(semantic);
		uint32_t comp_count = 2;
		uint8_t *ptr        = const_cast<uint8_t *>(uv_buff) + attrib.buffer_offset() + attrib.offset();        // const_cast only allowed in tests
		for (size_t i = 0; i < fox_attrib_count; ++i)
		{
			for (size_t j = 0; j < comp_count; ++j)
			{
				EXPECT_NEAR(fox_uvs[i * comp_count + j], reinterpret_cast<float32_t *>(ptr)[j], epsilon);
			}

			ptr += layout.stride();
		}
	}
	{
		auto     semantic   = rhi::BufferSemantic::vertex_bone_id_0;
		auto    &attrib     = m.m_attribute_vertex_descriptors[0].attribute(semantic);
		auto    &layout     = m.m_attribute_vertex_descriptors[0].layout(semantic);
		uint32_t comp_count = 4;
		uint8_t *ptr        = const_cast<uint8_t *>(bone_buff) + attrib.buffer_offset() + attrib.offset();        // const_cast only allowed in tests
		for (size_t i = 0; i < fox_attrib_count; ++i)
		{
			for (size_t j = 0; j < comp_count; ++j)
			{
				EXPECT_EQ(fox_joint_ids[i * comp_count + j], reinterpret_cast<uint16_t *>(ptr)[j]);
			}

			ptr += layout.stride();
		}
	}
	{
		auto     semantic   = rhi::BufferSemantic::vertex_weight_0;
		auto    &attrib     = m.m_attribute_vertex_descriptors[0].attribute(semantic);
		auto    &layout     = m.m_attribute_vertex_descriptors[0].layout(semantic);
		uint32_t comp_count = 4;
		uint8_t *ptr        = const_cast<uint8_t *>(weight_buff) + attrib.buffer_offset() + attrib.offset();        // const_cast only allowed in tests
		for (size_t i = 0; i < fox_attrib_count; ++i)
		{
			for (size_t j = 0; j < comp_count; ++j)
			{
				EXPECT_NEAR(fox_weights[i * comp_count + j], reinterpret_cast<float32_t *>(ptr)[j], epsilon);
			}

			ptr += layout.stride();
		}
	}
	{
		EXPECT_EQ(this->fox_model->skins().size(), 1);
		auto &s = this->fox_model->skins()[0];
		EXPECT_EQ(s.m_inverse_bind_matrices.size(), fox_inverse_bind_matrices_count);
		EXPECT_EQ(s.m_joints.size(), fox_inverse_bind_matrices_count);

		for (size_t i = 0; i < s.m_inverse_bind_matrices.size(); ++i)
			for (size_t j = 0; j < 16; ++j)
				EXPECT_NEAR(s.m_inverse_bind_matrices[i].m_values[j], fox_inverse_bind_matrices[i * 16 + j], epsilon);

		for (size_t i = 0; i < s.m_joints.size(); ++i)
			EXPECT_EQ(s.m_joints[i], fox_joints[i]);

		EXPECT_EQ(s.m_root, 2);
		EXPECT_EQ(s.m_node_index, 1);
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

TEST_F(GLTFTest, gltf_assert_test)
{
	// Since I can only call "free()" once on buffer_pack otherwise it asserts (a good thing)
	// TODO: Check for asserting and catching in tests
	std::vector<ror::Mesh, rhi::BufferAllocator<ror::Mesh>> *alloc_buffer = new std::vector<ror::Mesh, rhi::BufferAllocator<ror::Mesh>>();
	alloc_buffer->reserve(2);        // This shouldn't assert
	ror::Mesh m1;
	ror::Mesh m2;
	alloc_buffer->push_back(m1);
	alloc_buffer->push_back(m2);

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

void GLTFTest::load_model(std::string path)
{
	auto        print_shader_source = settings_configs_copy.get<bool>("gltf_compile_print_shader");
	ror::Model *model2              = new ror::Model();
	model2->load_from_gltf_file(path);

	uint32_t         mesh_index = 0;
	std::vector<int> v;
	for (auto &ms : model2->meshes())
	{
		hashes_of_meshs[ms.m_hash]++;
		for (size_t j = 0; j < ms.m_attribute_vertex_descriptors.size(); ++j)
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
				auto vs = rhi::generate_primitive_vertex_shader(*model2, static_cast<uint32_t>(mesh_index), static_cast<uint32_t>(j));
				(void) vs;

				program_source = vs;

				auto hash = ror::hash_64(vs);
				hashes_of_vs[hash]++;
				unique_vs[hash]      = vs;
				unique_vattrib[hash] = std::make_pair(mesh_index, j);

				if (print_shader_source)
				{
					std::filesystem::path tmp_file = std::filesystem::temp_directory_path() / "tmp_shader.vert";
					tmp_file                       = std::filesystem::absolute(tmp_file);

					std::ios_base::openmode mode = std::ios::out;
					{
						std::ofstream of(tmp_file, mode);
						of << vs;
					}

					std::string command{"glslangValidator -V  "};
					command.append(tmp_file);

					auto res = ror_test::execute_command(command.c_str());
					std::cout << "glslangValidator says \n\t" << res.first << std::endl;
					if (res.second != EXIT_SUCCESS)
						ror::log_critical("Created bad shader source code\n{}", vs.c_str());
					else
						ror::log_warn("Vertex shader source code\n{}", vs.c_str());
				}
			}
			{
				auto fs = rhi::generate_primitive_fragment_shader(ms, model2->materials(), static_cast<uint32_t>(j));
				(void) fs;

				program_source.append(fs);

				auto hash = ror::hash_64(fs);
				hashes_of_fs[hash]++;
				unique_fs[hash] = fs;

				auto prog_hash = ror::hash_64(program_source);

				unique_ps[prog_hash] = program_source;
				hashes_of_ps[prog_hash]++;

				if (print_shader_source)
				{
					std::filesystem::path tmp_file = std::filesystem::temp_directory_path() / "tmp_shader.frag";
					tmp_file                       = std::filesystem::absolute(tmp_file);

					std::ios_base::openmode mode = std::ios::out;
					{
						std::ofstream of(tmp_file, mode);
						of << fs;
					}

					std::string command{"glslangValidator -V  "};
					command.append(tmp_file);

					auto res = ror_test::execute_command(command.c_str());
					if (res.second != EXIT_SUCCESS)
					{
						ror::log_critical("Created bad fragment shader source code\n");
						std::stringstream ss{fs};
						std::string       to;
						uint32_t          linnum = 1;
						while (std::getline(ss, to, '\n'))
						{
							std::cout << linnum++ << " : " << to << std::endl;
						}

						std::cout << "glslangValidator says \n\t" << res.first << std::endl;
					}
					else
						ror::log_warn("Fragment shader source code\n{}", fs.c_str());
				}
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
