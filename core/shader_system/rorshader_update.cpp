// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
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

#include "foundation/rortypes.hpp"
#include "profiling/rorlog.hpp"
#include "rorshader_update.hpp"
#include "shader_system/rorshader_system.hpp"
#include "watchcat/rorwatchcat.hpp"
#include <filesystem>
#include <memory>
#include <mutex>
#include <unordered_set>

namespace ror
{

ShaderUpdater::ShaderUpdater()
{
	std::filesystem::path              m_starting_path{"/System/Volumes/Data/personal/roar_engine/core/assets/shaders"};        // TODO: Think about how we give it path from within editor like "/System/Volumes/Data/personal/roar_engine/editor"
	std::vector<std::filesystem::path> paths{};
	paths.emplace_back(m_starting_path);

	this->m_watcher = std::make_unique<ror::WatchCat>(
	    paths, [this](const std::vector<ror::WatchCatEvent> &events) {
		    for (auto e : events)
			    if (e.m_type == ror::WatchCatEventType::change)
				    this->push_shader_update_record(2, e.m_path.filename());
	    },
	    0.00001f);
}

ShaderUpdater::~ShaderUpdater() noexcept
{
	if (this->m_watcher)
		this->m_watcher->stop();
}

void ShaderUpdater::push_program_record(int32_t a_program_id, std::vector<rhi::Program> &a_programs, std::vector<rhi::Shader> &a_shaders, std::function<void(rhi::Device &)> a_callback, std::vector<std::string> *a_dependencies)
{
	std::lock_guard<std::mutex> lock{this->m_mutex};

	assert(a_program_id >= 0 && "Invalid program id given to push_program_record");

	int32_t record_id = static_cast<int32_t>(this->m_program_records.size());
	auto   &program   = a_programs[static_cast<size_t>(a_program_id)];

	std::vector<int32_t> shader_ids = {program.vertex_id(), program.fragment_id(), program.compute_id(), program.mesh_id(), program.tile_id()};

	for (auto shader_id : shader_ids)
	{
		if (shader_id != -1 && shader_id < static_cast<int32_t>(a_shaders.size()))
		{
			auto       &shader      = a_shaders[static_cast<size_t>(shader_id)];
			std::string shader_name = shader.shader_path().filename();
			this->m_shader_programs_mapping[shader_name].emplace_back(record_id);

			// Also create mapping for all of its includes
			for (auto &incs : shader.includes())
				this->m_shader_programs_mapping[incs].emplace_back(record_id);

			if (a_dependencies)
				for (auto &dependency : *a_dependencies)
					this->m_shader_programs_mapping[dependency].emplace_back(record_id);
		}
	}

	this->m_program_records.emplace_back(a_program_id, a_programs, a_shaders, a_callback);
}

void ShaderUpdater::push_shader_update_record(int32_t a_counter, std::string a_shader)
{
	std::lock_guard<std::mutex> lock{this->m_mutex};

	this->m_shaders_update_records.emplace_back(a_counter, a_shader);
}

// TODO: Add some cacheing, at the moment it doesn't work for big scenes in debug mode
// All the programs using a shader needs to be updated there is no other way and its taking too long
void ShaderUpdater::resolve_updates(rhi::Device &a_device)
{
	// TODO: Maybe in the future could make each thread take an update, but this is for debugging purposes only anyways, not critical
	std::lock_guard<std::mutex> lock{this->m_mutex};
	const auto                 &dependencies = ror::mesh_shaders_dependencies();

	// std::unordered_set<hash_64_t> unique_records{};        // All unique record hashes

	for (auto &shader_record : this->m_shaders_update_records)
	{
		--shader_record.m_counter;

		if (shader_record.m_counter <= 0)
		{
			auto &program_record_ids = this->m_shader_programs_mapping[shader_record.m_shader];
			for (auto &program_record_id : program_record_ids)
			{
				auto &program_record = this->m_program_records[program_record_id];

				// hash_64_t hash = hash_64(&program_record.m_program_id, sizeof(program_record.m_program_id));
				// hash_combine_64(hash, reinterpret_cast<hash_64_t &>(program_record.m_programs));
				// hash_combine_64(hash, reinterpret_cast<hash_64_t &>(program_record.m_shaders));

				// auto res = unique_records.emplace(hash);
				// if (res.second)
				auto &programs = program_record.m_programs;
				auto &shaders  = program_record.m_shaders;
				auto &program  = programs[static_cast<size_t>(program_record.m_program_id)];

				std::vector<int32_t> shader_ids = {program.vertex_id(), program.fragment_id(), program.compute_id(), program.mesh_id(), program.tile_id()};

				for (auto shader_id : shader_ids)
				{
					if (shader_id != -1 && shader_id < static_cast<int32_t>(shaders.size()))
					{
						auto &shader = shaders[static_cast<size_t>(shader_id)];

						std::string shader_name = shader.shader_path().filename();
						auto       &inc         = shader.includes();
						auto        include     = std::find(inc.begin(), inc.end(), shader_record.m_shader);

						if (shader_name == shader_record.m_shader || include != inc.end())
						{
							shader.reload();
							shader.compile();
							shader.upload(a_device);

							program_record.m_callback(a_device);        // This must have a call to program.upload()
						}
						else        // This means the user takes responsibility and knows this shader is a dependency
						{
							auto dependency = std::find(dependencies.begin(), dependencies.end(), shader_record.m_shader);
							if (dependency != dependencies.end())
							{
								program_record.m_callback(a_device);        // This must have a call to shader.source(), shader.compile(), shader.upload() and program.upload()
							}
						}
					}
				}
			}
		}
	}

	this->m_shaders_update_records.remove_if([](ShaderUpdateRecord &shader_record) { return shader_record.m_counter <= 0; });
}
}        // namespace ror
