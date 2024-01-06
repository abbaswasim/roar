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
#include "project_setup.hpp"
#include "rorshader_update.hpp"
#include "settings/rorsettings.hpp"
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
	auto &setting = ror::settings();

	if (!setting.m_watch_shaders)
		return;

	if (setting.m_watch_shaders && setting.m_shaders_watch_path.empty())
		ror::log_critical("Trying to watch empty shaders folder, shader watching won't work");

	std::filesystem::path              m_starting_path{std::filesystem::path{roar_dir} / setting.m_shaders_watch_path};        // TODO: Think about how we give it path from within editor
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
	if (!ror::settings().m_watch_shaders)
		return;

	if (this->m_watcher)
		this->m_watcher->stop();
}

void ShaderUpdater::push_program_record(int32_t a_program_id, std::vector<rhi::Program> &a_programs, std::vector<rhi::Shader> &a_shaders, UpdateCallback a_callback, std::vector<std::string> *a_dependencies)
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
		}
	}

	if (a_dependencies)
		for (auto &dependency : *a_dependencies)
			this->m_shader_programs_mapping[dependency].emplace_back(record_id);

	this->m_program_records.emplace_back(a_program_id, a_programs, a_shaders, a_callback);
}

void ShaderUpdater::push_shader_update_record(int32_t a_counter, std::string a_shader)
{
	std::lock_guard<std::mutex> lock{this->m_mutex};

	this->m_shaders_update_records.emplace_back(a_counter, a_shader);
}

void ShaderUpdater::resolve_updates(rhi::Device &a_device, ror::JobSystem &a_job_system)
{
	std::lock_guard<std::mutex> lock{this->m_mutex};

	std::unordered_set<hash_64_t>     unique_records{};        // All unique record hashes, so we don't process same record again, this one is probably not needed
	std::unordered_set<hash_64_t>     unique_shaders{};        // All unique shader hashes, used for not having to build many shaders here than required
	std::unordered_set<hash_64_t>     shader_cache{};          // All unique shader hashes used for update callbacks
	std::vector<ror::JobHandle<bool>> job_handles;
	job_handles.reserve(this->m_shader_programs_mapping.size() * 2);        // Should be enough, still a conservative estimate

	log_set_level(LogLevel::error);        // Lets not create too much noise when this happenes

	for (auto &shader_record : this->m_shaders_update_records)
	{
		--shader_record.m_counter;

		if (shader_record.m_counter <= 0)
		{
			auto &program_record_ids = this->m_shader_programs_mapping[shader_record.m_shader];
			for (auto &program_record_id : program_record_ids)
			{
				auto &program_record = this->m_program_records[program_record_id];

				hash_64_t hash = hash_64(&program_record.m_program_id, sizeof(program_record.m_program_id));
				hash_combine_64(hash, reinterpret_cast<hash_64_t &>(program_record.m_programs));
				hash_combine_64(hash, reinterpret_cast<hash_64_t &>(program_record.m_shaders));

				auto res = unique_records.emplace(hash);
				if (res.second)
				{
					auto &programs = program_record.m_programs;
					auto &shaders  = program_record.m_shaders;
					auto &program  = programs[static_cast<size_t>(program_record.m_program_id)];

					std::vector<int32_t> shader_ids = {program.vertex_id(), program.fragment_id(), program.compute_id(), program.mesh_id(), program.tile_id()};

					for (auto shader_id : shader_ids)
					{
						if (shader_id != -1 && shader_id < static_cast<int32_t>(shaders.size()))
						{
							auto &shader = shaders[static_cast<size_t>(shader_id)];

							res = unique_shaders.emplace(shader.hash());
							if (res.second)
							{
								auto prog_job_handle = a_job_system.push_job([&shader, &shader_record, &a_device]() -> auto {
									std::string shader_name = shader.shader_path().filename();
									auto       &inc         = shader.includes();
									auto        include     = std::find(inc.begin(), inc.end(), shader_record.m_shader);

									// Simple case, shader exists on disk and it might or might not have includes
									if (shader_name == shader_record.m_shader || include != inc.end())
									{
										shader.reload();
										shader.compile();
										shader.upload(a_device);
									}
									// More complex case, its a dependency but not clear how to build the shader with this dependency,
									// probably needs a source creation like what scene does for each model part
									// This means the user takes responsibility and knows this shader is a dependency

									return true;
								});

								job_handles.emplace_back(std::move(prog_job_handle));        // Since we know these are unique shaders, no need to mutex lock them
							}
						}
					}
				}
			}

			for (auto &jh : job_handles)
				if (!jh.data())
					ror::log_critical("Can't update program required for the scene.");

			for (auto &program_record_id : program_record_ids)
			{
				auto &program_record = this->m_program_records[program_record_id];
				program_record.m_callback(a_device, &shader_cache);        // This must have a call to program.upload()
				                                                           // Incase of dependency it must have a call to shader.source(), shader.compile(), shader.upload() and program.upload()
			}
		}
	}

	this->m_shaders_update_records.remove_if([](ShaderUpdateRecord &shader_record) { return shader_record.m_counter <= 0; });

	log_set_level(LogLevel::trace);        // Reset the log levels
}
}        // namespace ror
