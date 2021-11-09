// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2019
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
#include "foundation/rorrandom.hpp"
#include "profiling/rorlog.hpp"
#include "roar.hpp"
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <forward_list>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

// Use ctpl
#include "CTPL/ctpl_stl.h"

namespace ror
{
FORCE_INLINE uint32_t get_hardware_threads()
{
	return std::max(1u, std::thread::hardware_concurrency() - 1);
}

/* Job system goals:
Must:
  Would like some queue to push jobs onto, deque is a better choice
  Pick and run jobs if dependencies are met and threads available
  If can't run now, push back onto the queue, or leave where it is move onto the next in the deque
  Per Job query, finished or running
  Per Job wait options
  Pair of job and future returned to caller
Could:
  If job waiting for other jobs to finish, don't hog onto a worker thread
*/

class ROAR_ENGINE_ITEM Job final
{
  public:
	FORCE_INLINE      Job()                       = default;              //! Default constructor
	FORCE_INLINE      Job(const Job &a_other)     = delete;               //! Copy constructor
	FORCE_INLINE      Job(Job &&a_other) noexcept = delete;               //! Move constructor
	FORCE_INLINE Job &operator=(const Job &a_other) = delete;             //! Copy assignment operator
	FORCE_INLINE Job &operator=(Job &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~Job() noexcept                        = default;        //! Destructor

	FORCE_INLINE Job(std::function<void(void)> a_function, std::vector<std::shared_ptr<Job>> a_dependencies) :
		m_payload(a_function), m_dependencies(a_dependencies)
	{}

	void operator()()
	{
		this->m_payload();
	}

	FORCE_INLINE void finish() noexcept
	{
		this->m_done.test_and_set();
	}

	FORCE_INLINE bool ready() noexcept
	{
		for (auto &d : this->m_dependencies)
		{
			if (d)
				if (!d->m_done.test())
					return false;
		}
		return true;
	}

	// TODO: Add debug code to check how many jobs are alive at one time. Are we actually deleting all those shared_ptrs or not

  protected:
  private:
	std::function<void(void)>         m_payload{};             // Function the job runs with its arguements
	std::vector<std::shared_ptr<Job>> m_dependencies{};        // All job dependencies, needs synchronisation before reading
	std::atomic_flag                  m_done{false};           // Cleared by default initializer to false
};

template <class _future_type>
class ROAR_ENGINE_ITEM JobHandle final
{
  public:
	FORCE_INLINE            JobHandle(const JobHandle &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE            JobHandle(JobHandle &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE JobHandle &operator=(const JobHandle &a_other) = delete;             //! Copy assignment operator
	FORCE_INLINE JobHandle &operator=(JobHandle &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~JobHandle() noexcept                              = default;        //! Destructor

	FORCE_INLINE JobHandle(std::shared_ptr<Job> a_job, std::future<_future_type> &&a_future) :
		m_job(a_job), m_future(std::move(a_future))
	{}

	std::function<_future_type> &future()
	{
		return this->m_future;
	}

	std::shared_ptr<Job> job()
	{
		return this->m_job;
	}

	_future_type data()
	{
		return this->m_future.get();        // TODO: Check if this is making a copy?
	}

  protected:
  private:
	FORCE_INLINE JobHandle() = default;        //! Default constructor

	std::shared_ptr<Job>      m_job;
	std::future<_future_type> m_future;        // Need to keep the copy here
};

class ROAR_ENGINE_ITEM JobSystem final
{
  public:
	// FORCE_INLINE            JobSystem()                             = default;        //! Default constructor
	FORCE_INLINE            JobSystem(const JobSystem &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE            JobSystem(JobSystem &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE JobSystem &operator=(const JobSystem &a_other) = delete;             //! Copy assignment operator
	FORCE_INLINE JobSystem &operator=(JobSystem &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~JobSystem() noexcept                              = default;        //! Destructor
	// FORCE_INLINE ~JobSystem() noexcept                                      //! Destructor
		// {
			// this->m_workers.clear();
			// for (auto & th : this->m_workers) {
			//	th.release();
			// }
		// }

  private:
	class ROAR_ENGINE_ITEM WorkerQueue final
	{
	  public:
		FORCE_INLINE              WorkerQueue()                               = default;        //! Default constructor
		FORCE_INLINE              WorkerQueue(const WorkerQueue &a_other)     = delete;         //! Copy constructor
		FORCE_INLINE              WorkerQueue(WorkerQueue &&a_other) noexcept = delete;         //! Move constructor
		FORCE_INLINE WorkerQueue &operator=(const WorkerQueue &a_other) = delete;               //! Copy assignment operator
		FORCE_INLINE WorkerQueue &operator=(WorkerQueue &&a_other) noexcept = delete;           //! Move assignment operator
		FORCE_INLINE ~WorkerQueue() noexcept                                = default;          //! Destructor

		void push(std::shared_ptr<Job> a_job)
		{
			std::unique_lock<std::mutex> lock{this->m_lock};
			this->m_worker_queue.emplace_back(a_job);
		}

		std::shared_ptr<Job> pop()
		{
			std::unique_lock<std::mutex> lock{this->m_lock};

			if (this->m_worker_queue.empty())
				return nullptr;

			auto front = this->m_worker_queue.front();
			this->m_worker_queue.pop_front();
			return front;
		}

		std::shared_ptr<Job> steal()
		{
			std::unique_lock<std::mutex> lock{this->m_lock};

			if (this->m_worker_queue.empty())
				return nullptr;

			auto back = this->m_worker_queue.front();
			this->m_worker_queue.pop_back();
			return back;
		}

		bool empty()
		{
			std::unique_lock<std::mutex> lock{this->m_lock};

			return this->m_worker_queue.empty();
		}

	  protected:
	  private:
		std::deque<std::shared_ptr<Job>> m_worker_queue{};        // WorkerQueue queues containing all the jobs for this worker
		std::mutex                       m_lock{};                // Used to lock shared variables in this worker
	};

  public:
	FORCE_INLINE JobSystem(uint32_t a_workers_count)
	{
		this->init(a_workers_count);
	}

	template <class _function, class... _arguments>
	decltype(auto) push_job(_function &&a_function, std::vector<std::shared_ptr<Job>> a_dependencies, _arguments &&...a_arguments)
	{
		using return_type = decltype(a_function(a_arguments...));
		auto pack_task    = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<_function>(a_function), std::forward<_arguments>(a_arguments)...));

		auto index = this->random_worker();

		std::shared_ptr<Job> job = std::make_shared<Job>([]() {}, a_dependencies);
		this->worker(index).push(job);

		std::unique_lock<std::mutex> lock(this->m_lock);
		this->m_condition_variable.notify_one();

		return JobHandle<return_type>{job, pack_task->get_future()};
	}

	FORCE_INLINE uint32_t random_worker()
	{
		std::unique_lock<std::mutex> lock(this->m_lock);
		return this->m_random_index.next();
	}

	// Utility function to access thread queue
	FORCE_INLINE WorkerQueue &worker(uint32_t a_index)
	{
		return *this->m_worker_queues[static_cast<size_t>(a_index)];
	}

	FORCE_INLINE void stop()
	{
		this->m_stop.test_and_set();
	}

  protected:
  private:
	std::shared_ptr<Job> any_job()
	{
		for (auto &q : this->m_worker_queues)
			if (auto j = q->steal())
				return j;

		return nullptr;
	}

	void init(uint32_t a_workers_count)
	{
#if 0
		auto payload_function = [](uint32_t) {};

#else
		auto payload_function = [this](uint32_t a_thread_id) {
			log_critical("Here we go ===================================================== ");
			auto &my_worker = this->worker(a_thread_id);

			while (true)
			{
				auto job = my_worker.pop();
				if (job != nullptr)
				{
					if (job->ready())
					{
						(*job)();        // Execute the job
						job->finish();
					}
					else
						my_worker.push(job);
				}
				else
				{
					// auto &other_worker = this->worker(this->random_worker());
					// TODO: Fix me, reset reandom worker
					auto &other_worker = this->worker(0);
					// Steal a job
					job = other_worker.steal();
					if (job != nullptr)
					{
						if (job->ready())
						{
							(*job)();        // Execute the job
							job->finish();
						}
						else
							other_worker.push(job);
					}
					else
					{
						// Look for jobs in all queues
						job = this->any_job();
						if (job != nullptr)
						{
							if (job->ready())
							{
								(*job)();        // Execute the job
								job->finish();
							}
							else
								my_worker.push(job);
						}
						else
						{
							// Sleep to be awaken
							std::unique_lock<std::mutex> lock{this->m_lock};
							this->m_condition_variable.wait(lock, [this, &a_thread_id]() {
								return !this->m_worker_queues[a_thread_id]->empty() || this->m_stop.test();
							});

							if (this->m_stop.test())
								return;
						}
					}
				}
			}
		};
#endif

		this->m_random_index.reset(0u, a_workers_count - 1);

		for (size_t i = 0; i < a_workers_count; ++i)
			this->m_worker_queues.emplace_back(std::make_unique<WorkerQueue>());

		// Kick of all the threads
		for (size_t i = 0; i < a_workers_count; ++i)
			this->m_workers.emplace_back(std::make_unique<std::thread>(payload_function, i));
	}

	std::vector<std::unique_ptr<std::thread>> m_workers{};                   // list of workers running jobs that needs running
	std::vector<std::unique_ptr<WorkerQueue>> m_worker_queues{};             // list of worker queues containing all the jobs for that worker
	ror::Random<uint32_t>                     m_random_index{};              // Random index generator from 0 - get_hardware_threads() - 1
	std::condition_variable                   m_condition_variable{};        // Used to signal workers to start working
	std::mutex                                m_lock{};                      // Used to lock shared variables in job system
	std::atomic_flag                          m_stop;                        // Used to check if should stop execution
};

}        // namespace ror

#include "rorjobsystem.hh"
