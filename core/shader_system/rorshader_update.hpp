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

#pragma once

#include "foundation/rormacros.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorprogram.hpp"
#include "watchcat/rorwatchcat.hpp"
#include <memory>
#include <vector>

namespace ror
{
class ROAR_ENGINE_ITEM ShaderUpdater final
{
  public:
	FORCE_INLINE                ShaderUpdater(const ShaderUpdater &a_other)     = delete;        //! Copy constructor
	FORCE_INLINE                ShaderUpdater(ShaderUpdater &&a_other) noexcept = delete;        //! Move constructor
	FORCE_INLINE ShaderUpdater &operator=(const ShaderUpdater &a_other)         = delete;        //! Copy assignment operator
	FORCE_INLINE ShaderUpdater &operator=(ShaderUpdater &&a_other) noexcept     = delete;        //! Move assignment operator

	ShaderUpdater();                  //! constructor
	~ShaderUpdater() noexcept;        //! Destructor

	void push_program_record(int32_t a_program_id, std::vector<rhi::Program> &a_programs, std::vector<rhi::Shader> &a_shaders, std::function<void(rhi::Device &)> a_callback, std::vector<std::string> *a_dependencies = nullptr);
	void resolve_updates(rhi::Device &a_device);

	struct ProgramRecord
	{
		FORCE_INLINE ProgramRecord(int32_t a_program_id, std::vector<rhi::Program> &a_programs, std::vector<rhi::Shader> &a_shaders, std::function<void(rhi::Device &)> a_callback) :
		    m_program_id(a_program_id), m_programs(a_programs), m_shaders(a_shaders), m_callback(a_callback)
		{}

		int32_t                            m_program_id{-1};        //! What's my program id in the list of programs in target m_programs
		std::vector<rhi::Program>         &m_programs;              //! Programs target in some list in clients
		std::vector<rhi::Shader>          &m_shaders;               //! Shaders target in some list in clients
		std::function<void(rhi::Device &)> m_callback{};            //! Callback that will be used to update the target program
	};

	struct ShaderUpdateRecord
	{
		FORCE_INLINE ShaderUpdateRecord(int32_t a_counter, const std::string &a_shader) :
		    m_counter(a_counter), m_shader(a_shader)
		{}

		int32_t     m_counter{2};        //! When this counter reaches 0 it will be executed, this is to defer it to later frames, by default gives 3 frames
		std::string m_shader;
	};

	using ShaderProgramMapping = std::unordered_map<std::string, std::vector<size_t>>;        // Shader name to ProgramRecords mapping by ProgramRecord index in the m_program_records
	using ProgramRecords       = std::vector<ProgramRecord>;                                  // Convenient name for many program records

  protected:
  private:
	void push_shader_update_record(int32_t a_counter, std::string a_shader);

	ShaderProgramMapping           m_shader_programs_mapping{};        //! Mapping of shaders by name to program record that needs updating
	ProgramRecords                 m_program_records{};                //! All program records we have seen so far
	std::list<ShaderUpdateRecord>  m_shaders_update_records{};         //! Shaders that were updated on disk recently and needs updating in the renderer, I keep this list for deferred processing
	std::unique_ptr<ror::WatchCat> m_watcher{nullptr};                 //! The watcher used to watch shader files
	std::mutex                     m_mutex{};                          //! Mutex to lock everything when used from multiple threads
};

FORCE_INLINE ShaderUpdater &shader_updater() noexcept
{
	static ShaderUpdater su{};        // Global shader_updater container
	return su;
}

}        // namespace ror
