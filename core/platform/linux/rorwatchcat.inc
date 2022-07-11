#include "foundation/rortypes.hpp"
#include "profiling/rorlog.hpp"
#include "watchcat/rorwatchcat.hpp"
#include <bits/stdint-intn.h>
#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <experimental/filesystem>
#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>
#include <unordered_map>

namespace ror
{
constexpr size_t inotify_events_counts{1024};

class PlatformData final
{
  public:
	PlatformData(const PlatformData &a_other)                = delete;        //! Copy constructor
	PlatformData(PlatformData &&a_other) noexcept            = delete;        //! Move constructor
	PlatformData &operator=(const PlatformData &a_other)     = delete;        //! Copy assignment operator
	PlatformData &operator=(PlatformData &&a_other) noexcept = delete;        //! Move assignment operator
	~PlatformData() noexcept                                                  //! Destructor
	{
		for (auto &path_fs : this->m_watched_descriptors)
			inotify_rm_watch(this->m_inotify_instance, path_fs.first);

		close(this->m_inotify_instance);
	}

	PlatformData(WatchCat &a_watchcat)
	{
		this->m_watchcat = &a_watchcat;

		this->m_inotify_instance = inotify_init();
		if (this->m_inotify_instance < 0)
			ror::log_error("watchcat: Inotify init failed, can't create instance.");

		std::vector<std::filesystem::path> all_paths = this->m_watchcat->get_paths();

		for (auto &path : all_paths)
		{
			this->add_watcher(path);
		}

		this->m_notify_events_buffer.resize(inotify_events_counts, inotify_event{});
	}

	void add_watcher(std::filesystem::path a_path)
	{
		if (std::filesystem::exists(a_path))
		{
			std::vector<std::filesystem::path> paths;
			paths.emplace_back(a_path);

			if (std::filesystem::is_directory(a_path))
			{
				try
				{
					for (auto &p : std::filesystem::recursive_directory_iterator(a_path, std::filesystem::directory_options::follow_directory_symlink))
					{
						// Only add directories recursively not files
						if (std::filesystem::is_directory(p.path()))
							paths.emplace_back(p.path());
					}
				}
				catch (std::filesystem::filesystem_error &fse)
				{
					ror::log_error("WatchCat: Recursive directory iterator threw exception for {} and {}", fse.path1().c_str(), fse.path2().c_str());
				}
			}

			for (auto &p : paths)
			{
				auto search = this->m_watched_paths.find(p.c_str());
				if (search == this->m_watched_paths.end())
				{
					auto path_fd = inotify_add_watch(this->m_inotify_instance,
													 p.c_str(),
													 IN_CREATE | IN_DELETE | IN_MODIFY);

					this->m_watched_descriptors[path_fd] = p;
					this->m_watched_paths[p]             = path_fd;
				}
			}
		}
		else
		{
			ror::log_error("WatchCat: Requesting to watch {} but it doesn't exist", a_path.c_str());
		}
	}

	void handle_events(size_t a_length)
	{
		size_t i = 0;

		std::vector<WatchCatEvent> events;
		events.reserve(this->m_notify_events_buffer.size());        // Conservative reserve

		while (i < a_length)
		{
			struct inotify_event *event = static_cast<struct inotify_event *>(&this->m_notify_events_buffer[i]);
			if (!(event->mask & IN_ISDIR) && event->len)        // this means its a file instead of a directory
			{
				if (event->mask & IN_CREATE)
				{
					events.emplace_back(WatchCatEventType::add, this->m_watched_descriptors[event->wd] / std::filesystem::path(event->name));
				}
				else if (event->mask & IN_DELETE)
				{
					events.emplace_back(WatchCatEventType::remove, this->m_watched_descriptors[event->wd] / std::filesystem::path(event->name));
				}
				else if (event->mask & IN_MODIFY)
				{
					events.emplace_back(WatchCatEventType::change, this->m_watched_descriptors[event->wd] / std::filesystem::path(event->name));
				}
			}
			i += sizeof(inotify_event) + static_cast<size_t>(event->len);
		}

		this->m_watchcat->notify(events);
	}

	void read_events()
	{
		// The following is to workaround bug in select.h by using __i and __arr variable names
		typedef fd_fd_mask fd_mask;
#define __FD_ZERO_LOCAL(s)                                                 \
	do                                                                     \
	{                                                                      \
		unsigned int fd_i;                                                 \
		fd_set      *fd_arr = (s);                                         \
		for (fd_i = 0; fd_i < sizeof(fd_set) / sizeof(fd_fd_mask); ++fd_i) \
			__FDS_BITS(fd_arr)                                             \
			[fd_i] = 0;                                                    \
	} while (0)

#define FD_ZERO_LOCAL(fdsetp) __FD_ZERO_LOCAL(fdsetp)

		fd_set  rfds;
		timeval timeout;

		// wait for a very short amount of time
		timeout.tv_sec  = 0;
		timeout.tv_usec = 10;

		FD_ZERO_LOCAL(&rfds);
		FD_SET(this->m_inotify_instance, &rfds);

		// TODO: check whether epoll is better choice compared to select, this is use  before calling potentiall blocking read
		int rv = select(this->m_inotify_instance + 1, &rfds, nullptr, nullptr, &timeout);

		if (rv == -1)
		{
			ror::log_error("No data available to read from inotify file descriptor after select");
			return;
		}

		// In case of read timeout just repeat the loop.
		if (rv == 0)
			return;

		ssize_t length = read(this->m_inotify_instance, this->m_notify_events_buffer.data(), this->m_notify_events_buffer.size());

		if (length < 0)
			ror::log_error("No data available from inotify file descriptor");
		else
			handle_events(static_cast<size_t>(length));
	}

  protected:
  private:
	PlatformData() = default;                                               //! Default constructor
	WatchCat                                *m_watchcat{nullptr};           // Alias to the watchcat, non-owning
	int32_t                                  m_inotify_instance{-1};        //! inotify file descriptor
	std::unordered_map<int32_t, std::string> m_watched_descriptors{};
	std::unordered_map<std::string, int32_t> m_watched_paths{};
	std::vector<inotify_event>               m_notify_events_buffer{};
};

void WatchCat::rerun(std::filesystem::path a_path)
{
	// On linux don't need to rerun the watcher, just need to add the current path to inotify
	assert(this->m_platform_data != nullptr && "No data pointer exists in watcher before calling WatchCat::rerun.");
	this->m_platform_data->add_watcher(a_path);

	// If previously stopped, rerun
	if (!this->m_run.load(std::memory_order_relaxed))
		this->run();
}

void WatchCat::run()
{
	this->stop();        // Stop if already running

	this->m_platform_data = std::make_unique<PlatformData>(*this);
	this->m_run.store(true, std::memory_order_relaxed);        // Enable running of the thread incase we were stopped before

	this->m_runner_thread = std::make_unique<std::thread>([&]() {
		do
		{
			this->m_platform_data->read_events();
			std::this_thread::sleep_for(std::chrono::duration<float32_t>(this->m_latency));        // This should be sleeping for a second
		} while (this->m_run.load(std::memory_order_relaxed));
	});
}

}        // namespace ror
