// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2020
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

#include "core/watchcat/rorwatchcat.hpp"
#include "foundation/rortypes.hpp"
#include "profiling/rorlog.hpp"
#include <atomic>

namespace ror
{
void WatchCat::notify(std::vector<WatchCatEvent> a_events)
{
	this->m_callback(a_events);
}

WatchCat::WatchCat(std::vector<std::filesystem::path> a_paths, watchcat_callback a_callback, float32_t a_latency) :
    m_callback(a_callback), m_paths(a_paths), m_latency(a_latency)
{
	this->run();
}

WatchCat::~WatchCat() noexcept
{
	this->stop();
}

void WatchCat::add_path(std::filesystem::path a_path)
{
	this->m_paths.emplace_back(a_path);

	// removes duplicate paths
	std::sort(this->m_paths.begin(), this->m_paths.end());
	this->m_paths.erase(std::unique(this->m_paths.begin(), this->m_paths.end()),
	                    this->m_paths.end());

	this->rerun(a_path);
}

std::vector<std::filesystem::path> WatchCat::get_paths() const
{
	return this->m_paths;
}

float32_t WatchCat::get_latency() const
{
	return this->m_latency;
}

void WatchCat::stop()
{
	this->m_run.store(false, std::memory_order_relaxed);

	if (this->m_runner_thread && this->m_runner_thread->joinable())
		this->m_runner_thread->join();
}

}        // namespace ror

// Defer platform specific code to the bellow

#if defined(ROR_OS_TYPE_MAC)
#	include "platform/macos/rorwatchcat.inc"
#elif defined(ROR_OS_TYPE_LINUX)
#	include "platform/linux/rorwatchcat.inc"
#elif defined(ROR_OS_TYPE_WINDOWS)
#	include "platform/windows/rorwatchcat.inc"
#else
#	error "Platform not implemented"
#endif
