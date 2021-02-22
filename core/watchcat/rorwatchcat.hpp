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

#pragma once

#include "core/foundation/rormacros.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include <atomic>
#include <filesystem>
#include <functional>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

namespace ror
{
enum class WatchCatEventType : uint32_t
{
	add,
	remove,
	change,
	max
};

struct WatchCatEvent
{
	std::filesystem::path m_path;        // Where did it happen
	WatchCatEventType     m_type;        // What happened
	uint32_t              m_pad{0};

	WatchCatEvent(WatchCatEventType a_event_type, std::filesystem::path a_path) :
		m_path(a_path), m_type(a_event_type)
	{}
};

class PlatformData;

using watchcat_callback = std::function<void(std::vector<WatchCatEvent>)>;

/**
* Creates a watcher responsible for reporting filesystem events in specific paths
* You can watch a single file or a folder or a collection of either in any combination
* You will receive WatchCatEventType events for files in your specified locations
* Prefer to create few or even one instance of WatchCat because it creates a thread
* Many WatchCat objects means many threads which will result in contention for system resources
* WatchCat needs to be externally synchronised if used from multiple threads
*/

class ROAR_ENGINE_ITEM WatchCat final
{
  public:
	WatchCat(std::vector<std::filesystem::path> a_paths,
			 watchcat_callback                  a_callback,
			 float32_t                          a_latency = 1.0f);                             // Watches all the paths, and calls a_callback at a_latency frequency
	WatchCat(const WatchCat &a_other)     = delete;                   //! Copy constructor
	WatchCat(WatchCat &&a_other) noexcept = delete;                   //! Move constructor
	WatchCat &operator=(const WatchCat &a_other) = delete;            //! Copy assignment operator
	WatchCat &operator=(WatchCat &&a_other) noexcept = delete;        //! Move assignment operator
	~WatchCat() noexcept;                                             //! Destructor defined

	void                               add_path(std::filesystem::path a_path);
	std::vector<std::filesystem::path> get_paths() const;
	float32_t                          get_latency() const;
	void                               run();                                              // Start watching the paths
	void                               stop();                                             // Stop watching the paths
	void                               notify(std::vector<WatchCatEvent> a_events);        // Calls the callback with the events from platform specific callbacks, don't call directly. Its only public because the callback that calls it needs to see it

  protected:
  private:
	WatchCat() = default;                            //! Constructor private to prevent creation without paths
	void rerun(std::filesystem::path a_path);        // restart watching the paths

	watchcat_callback                  m_callback;                      // Use callback to forward events to // TODO: make this per path
	std::unique_ptr<std::thread>       m_runner_thread{nullptr};        // The thread that will be used to launch a monitor in
	std::unique_ptr<PlatformData>      m_platform_data{nullptr};        // Platform specific data should be defined in platform/os/watchcat.inc
	std::vector<std::filesystem::path> m_paths;                         // Copy of all files/directories to watch
	float32_t                          m_latency{1.0f};                 // Poll each second by default, but can be changed by user
	std::atomic<bool>                  m_run{true};                     // Should the loop be running capturing events or should it stop
public:
	uchar8_t                           m_pad[3];
};

}        // namespace ror
