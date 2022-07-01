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

#include <CoreServices/CoreServices.h>
#include <filesystem>
#include <memory>

namespace ror
{
void fsevents_callback(
	ConstFSEventStreamRef /*streamRef*/,
	void                         *clientCallBackInfo,
	size_t                        numEvents,
	void                         *eventPaths,
	const FSEventStreamEventFlags eventFlags[],
	const FSEventStreamEventId[] /*eventIds[]*/)
{
	char    **paths = static_cast<char **>(eventPaths);
	WatchCat *wc    = static_cast<WatchCat *>(clientCallBackInfo);

	assert(wc != nullptr);

	std::vector<WatchCatEvent> events;
	events.reserve(numEvents);

	for (size_t i = 0; i < numEvents; i++)
	{
		// Since all nsevents are coalesced, we report all events these can be later filtered by importance
		if ((eventFlags[i] & kFSEventStreamEventFlagItemCreated) == kFSEventStreamEventFlagItemCreated)
			events.emplace_back(WatchCatEventType::add, paths[i]);
		if ((eventFlags[i] & kFSEventStreamEventFlagItemRemoved) == kFSEventStreamEventFlagItemRemoved)
			events.emplace_back(WatchCatEventType::remove, paths[i]);
		if ((eventFlags[i] & kFSEventStreamEventFlagItemModified) == kFSEventStreamEventFlagItemModified)
			events.emplace_back(WatchCatEventType::change, paths[i]);
		// if ((eventFlags[i] & kFSEventStreamEventFlagItemInodeMetaMod) == kFSEventStreamEventFlagItemInodeMetaMod)
		//	events.emplace_back(WatchCatEventType::touch, paths[i]); // No handling of touch events for now
		//  Don't register event otherwise
	}

	wc->notify(events);
}

class PlatformData final
{
  public:
	PlatformData(const PlatformData &a_other)                = delete;        //! Copy constructor
	PlatformData(PlatformData &&a_other) noexcept            = delete;        //! Move constructor
	PlatformData &operator=(const PlatformData &a_other)     = delete;        //! Copy assignment operator
	PlatformData &operator=(PlatformData &&a_other) noexcept = delete;        //! Move assignment operator
	~PlatformData() noexcept                                                  //! Destructor
	{
		delete m_callbackInfo;
		CFRelease(m_paths_to_watch);
		FSEventStreamStop(m_stream);
		FSEventStreamInvalidate(m_stream);
		FSEventStreamRelease(m_stream);

		m_callbackInfo   = nullptr;
		m_paths_to_watch = nullptr;
		m_stream         = nullptr;
	}

	PlatformData(WatchCat &a_watchcat)
	{
		std::vector<CFStringRef> paths_strings;
		for (auto &p : a_watchcat.get_paths())
			paths_strings.push_back(CFStringCreateWithCString(kCFAllocatorDefault, p.c_str(), kCFStringEncodingUTF8));

		this->m_paths_to_watch = CFArrayCreate(kCFAllocatorDefault, reinterpret_cast<const void **>(&paths_strings[0]), static_cast<CFIndex>(paths_strings.size()), nullptr);
		this->m_latency        = static_cast<double>(a_watchcat.get_latency());        // Cast to double because that whats CFAbsoluteTime is
		this->m_callbackInfo   = new FSEventStreamContext();                           // Will put a pointer to this in here

		this->m_callbackInfo->info            = std::addressof(a_watchcat);        // Use WatchCat as this data pointer, dirty system hack really
		this->m_callbackInfo->release         = nullptr;
		this->m_callbackInfo->retain          = nullptr;
		this->m_callbackInfo->copyDescription = nullptr;
		this->m_callbackInfo->version         = 0;

		this->m_stream = FSEventStreamCreate(nullptr,
											 &fsevents_callback,
											 this->m_callbackInfo,
											 this->m_paths_to_watch,
											 kFSEventStreamEventIdSinceNow,
											 this->m_latency,
											 static_cast<uint32_t>(kFSEventStreamCreateFlagFileEvents) | static_cast<uint32_t>(kFSEventStreamEventFlagRootChanged) | static_cast<uint32_t>(kFSEventStreamEventFlagItemCreated));
	}

	FSEventStreamRef get_stream()
	{
		return this->m_stream;
	}

  protected:
  private:
	PlatformData() = default;        //! Default constructor
	CFArrayRef            m_paths_to_watch{nullptr};
	CFAbsoluteTime        m_latency{1.0};
	FSEventStreamContext *m_callbackInfo{nullptr};
	FSEventStreamRef      m_stream{nullptr};
};

void WatchCat::rerun(std::filesystem::path a_path)
{
	(void) a_path;        // Don't need path in mac os case

	this->run();
}

void WatchCat::run()
{
	this->stop();        // Stop if already running

	this->m_platform_data = std::make_unique<PlatformData>(*this);
	this->m_run.store(true, std::memory_order_relaxed);        // Enable running of the thread incase we were stopped before

	this->m_runner_thread = std::make_unique<std::thread>([&]() {
		FSEventStreamScheduleWithRunLoop(this->m_platform_data->get_stream(), CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);

		if (!FSEventStreamStart(this->m_platform_data->get_stream()))
		{
			ror::log_error("Can't create MacOS FSEventStream to watch paths\n");
		}

		int timeout = 5;        // Run RunLoop for 5 seconds and then check whether we should be runing or not

		do
		{
			CFRunLoopRunInMode(kCFRunLoopDefaultMode, timeout, true);
		} while (this->m_run.load(std::memory_order_relaxed));
	});
}

}        // namespace ror
