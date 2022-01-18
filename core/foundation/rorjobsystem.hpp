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
#include <cassert>
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

// #define WORK_STEALING 1

namespace ror
{
/**
 * Returns maximum minus one possible threads available on the system
 */
FORCE_INLINE uint32_t get_hardware_threads()
{
	// TODO: Check what does different number of threads mean for performance and contention
	return std::max(1u, std::thread::hardware_concurrency() - 1);
}

// TODO: Make all the allocations go away to see if that brings it back to as fast as it can be
/**
 * Base job type used internally by JobSystem
 * A job is a simple function that will be executed along side a done flag
 * This job is ready from the time its created, unless its done
 */
class ROAR_ENGINE_ITEM Job
{
  public:
	FORCE_INLINE      Job()                       = default;              //! Default constructor
	FORCE_INLINE      Job(const Job &a_other)     = delete;               //! Copy constructor
	FORCE_INLINE      Job(Job &&a_other) noexcept = delete;               //! Move constructor
	FORCE_INLINE Job &operator=(const Job &a_other) = delete;             //! Copy assignment operator
	FORCE_INLINE Job &operator=(Job &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE virtual ~Job() noexcept                = default;        //! Destructor

	FORCE_INLINE explicit Job(std::function<void(void)> a_function) :
		m_payload(a_function)
	{}

	declare_translation_unit_vtable();

	void operator()()
	{
		this->m_payload();
	}

	FORCE_INLINE void finish() noexcept
	{
		this->m_done.test_and_set();
	}

	[[nodiscard]] FORCE_INLINE bool finished() noexcept
	{
		return this->m_done.test();
	}

	FORCE_INLINE virtual bool ready() noexcept
	{
		// Always ready because no dependencies
		return true;
	}

	// TODO: Add debug code to check how many jobs are alive at one time. Are we actually deleting all those shared_ptrs or not

  protected:
  private:
	std::function<void(void)> m_payload{};          // Function the job runs along side its arguments
	std::atomic_flag          m_done{false};        // Cleared by default initializer to false
};

/**
 * Job specialization with a vector of dependencies used internally by JobSystem
 * A job is a simple function that will be executed along side a done flag and vector of dependencies
 * This job is ready when all of its dependencies are done
 */
class ROAR_ENGINE_ITEM JobDepend final : public Job
{
  public:
	FORCE_INLINE            JobDepend()                             = default;        //! Default constructor
	FORCE_INLINE            JobDepend(const JobDepend &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE            JobDepend(JobDepend &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE JobDepend &operator=(const JobDepend &a_other) = delete;             //! Copy assignment operator
	FORCE_INLINE JobDepend &operator=(JobDepend &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~JobDepend() noexcept override                     = default;        //! Destructor

	FORCE_INLINE JobDepend(std::function<void(void)> a_function, std::vector<std::shared_ptr<Job>> a_dependencies) :
		Job(a_function), m_dependencies(std::move(a_dependencies))
	{}

	declare_translation_unit_vtable() override;

	FORCE_INLINE bool ready() noexcept override
	{
		// Ready when all dependencies are "done"
		for (auto &d : this->m_dependencies)
			if (d)
				if (!d->finished())
					return false;

		return true;
	}

  protected:
  private:
	std::vector<std::shared_ptr<Job>> m_dependencies{};        // All job dependencies, synchronisation performed via m_done in Job
};

/**
 * Job specialization with a single dependency used internally by JobSystem
 * A job is a simple function that will be executed along side a done flag and a dependency
 * This job is ready when its dependency is done
 */
class ROAR_ENGINE_ITEM JobDepend1 final : public Job
{
  public:
	FORCE_INLINE             JobDepend1()                              = default;        //! Default constructor
	FORCE_INLINE             JobDepend1(const JobDepend1 &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE             JobDepend1(JobDepend1 &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE JobDepend1 &operator=(const JobDepend1 &a_other) = delete;              //! Copy assignment operator
	FORCE_INLINE JobDepend1 &operator=(JobDepend1 &&a_other) noexcept = delete;          //! Move assignment operator
	FORCE_INLINE ~JobDepend1() noexcept override                      = default;         //! Destructor

	FORCE_INLINE JobDepend1(std::function<void(void)> a_function, std::shared_ptr<Job> a_dependency) :
		Job(a_function), m_dependency(a_dependency)
	{}

	declare_translation_unit_vtable() override;

	FORCE_INLINE bool ready() noexcept override
	{
		// Ready when dependency is "done"
		if (this->m_dependency)
			if (!this->m_dependency->finished())
				return false;

		return true;
	}

  protected:
  private:
	std::shared_ptr<Job> m_dependency{};        // A job dependency, needs synchronisation before reading
};

using Dependencies = std::vector<std::shared_ptr<Job>>;

/**
 * JobHandle is public interface to the job system.
 * A JobHandle consists of a Job and a future that clients can wait on for results to be available
 */
template <class _future_type>
class ROAR_ENGINE_ITEM JobHandle final
{
  public:
	FORCE_INLINE            JobHandle(const JobHandle &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE            JobHandle(JobHandle &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE JobHandle &operator=(const JobHandle &a_other) = delete;             //! Copy assignment operator
	FORCE_INLINE JobHandle &operator=(JobHandle &&a_other) noexcept = delete;         //! Move assignment operator PATL: Don't enable this segfaulting with this
	FORCE_INLINE ~JobHandle() noexcept                              = default;        //! Destructor

	FORCE_INLINE JobHandle(std::shared_ptr<Job> a_job, std::future<_future_type> &&a_future) :
		m_job(a_job), m_future(std::move(a_future))
	{}

	FORCE_INLINE std::future<_future_type> &future()
	{
		return this->m_future;
	}

	FORCE_INLINE std::shared_ptr<Job> job()
	{
		return this->m_job;
	}

	FORCE_INLINE _future_type data()
	{
		return this->m_future.get();        // NOTE: There is a move happening here, if move is expensive for the return type of the job, fix this
	}

  protected:
  private:
	FORCE_INLINE JobHandle() = default;        //! Default constructor

	std::shared_ptr<Job>      m_job;
	std::future<_future_type> m_future;        // Need to keep the copy here
};

/**
 * JobSystem is the main job system. You would need an instance of this somewhere to push jobs to
 * JobSystem internally uses deque to push jobs onto. Jobs are ran as they come along unless they have dependecies
 */
// TODO: Add multiple queues for each thread and add work stealing scheduling
class ROAR_ENGINE_ITEM JobSystem final
{
  public:
	FORCE_INLINE            JobSystem(const JobSystem &a_other)     = delete;        //! Copy constructor
	FORCE_INLINE            JobSystem(JobSystem &&a_other) noexcept = delete;        //! Move constructor
	FORCE_INLINE JobSystem &operator=(const JobSystem &a_other) = delete;            //! Copy assignment operator
	FORCE_INLINE JobSystem &operator=(JobSystem &&a_other) noexcept = delete;        //! Move assignment operator
	FORCE_INLINE ~JobSystem() noexcept                                               //! Destructor
	{
		this->stop();
	}

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

		FORCE_INLINE void push(std::shared_ptr<Job> a_job)
		{
			std::lock_guard<std::mutex> lock{this->m_lock};
			this->m_worker_queue.emplace_back(a_job);
		}

		FORCE_INLINE std::shared_ptr<Job> pop()
		{
			std::lock_guard<std::mutex> lock{this->m_lock};
			return this->_pop();
		}

		// TODO: Remove: Without stealing scheduling support this has no point
		FORCE_INLINE std::shared_ptr<Job> steal()
		{
			std::lock_guard<std::mutex> lock{this->m_lock};
			return this->_pop();
		}

		FORCE_INLINE bool empty()
		{
			std::lock_guard<std::mutex> lock{this->m_lock};
			return this->m_worker_queue.empty();
		}

	  protected:
	  private:
		FORCE_INLINE std::shared_ptr<Job> _pop()
		{
			if (this->m_worker_queue.empty())
				return nullptr;

			auto front = this->m_worker_queue.front();
			this->m_worker_queue.pop_front();
			return front;
		}

		std::deque<std::shared_ptr<Job>> m_worker_queue{};        // WorkerQueue queues containing all the jobs for this worker
		std::mutex                       m_lock{};                // Used to lock shared variables in this worker
	};

  public:
	FORCE_INLINE JobSystem()        //! Default constructor
	{
		this->init(get_hardware_threads());
	}

	FORCE_INLINE JobSystem(uint32_t a_workers_count)
	{
		this->init(a_workers_count);
	}

	template <class _function, class... _arguments>
	decltype(auto) push_job(_function &&a_function, _arguments &&...a_arguments)
	{
		auto                 pack_task = make_packaged(std::forward<_function>(a_function), std::forward<_arguments>(a_arguments)...);
		std::shared_ptr<Job> job       = std::make_shared<Job>([pack_task]() { (*pack_task)(); });
#if defined(WORK_STEALING)
		auto index = this->random_worker();
		this->worker(index).push(job);
#else
		this->m_worker_queue.push(job);            // WorkerQueue is thread safe by itself
#endif

		// std::lock_guard<std::mutex> lock(this->m_lock);        // Don't need to take a lock, condition variable doesn't need one. Keep here to debug issues
		this->m_condition_variable.notify_one();

		using return_type = decltype(a_function(a_arguments...));
		return JobHandle<return_type>{job, pack_task->get_future()};
	}

	template <class _function, class... _arguments>
	decltype(auto) push_job(_function &&a_function, Dependencies a_dependencies, _arguments &&...a_arguments)
	{
		auto                       pack_task = make_packaged(std::forward<_function>(a_function), std::forward<_arguments>(a_arguments)...);
		std::shared_ptr<JobDepend> job       = std::make_shared<JobDepend>([pack_task]() { (*pack_task)(); }, std::forward<Dependencies>(a_dependencies));
#if defined(WORK_STEALING)
		auto index = this->random_worker();
		this->worker(index).push(job);
#else
		this->m_worker_queue.push(job);            // WorkerQueue is thread safe by itself
#endif

		// std::lock_guard<std::mutex> lock(this->m_lock);        // Don't need to take a lock, condition variable doesn't need one. Keep here to debug issues
		this->m_condition_variable.notify_one();

		using return_type = decltype(a_function(a_arguments...));
		return JobHandle<return_type>{job, pack_task->get_future()};
	}

	template <class _function, class... _arguments>
	decltype(auto) push_job(_function &&a_function, std::shared_ptr<Job> a_dependency, _arguments &&...a_arguments)
	{
		auto pack_task = make_packaged(std::forward<_function>(a_function), std::forward<_arguments>(a_arguments)...);

		std::shared_ptr<JobDepend1> job = std::make_shared<JobDepend1>([pack_task]() { (*pack_task)(); }, std::forward<std::shared_ptr<Job>>(a_dependency));
#if defined(WORK_STEALING)
		auto index = this->random_worker();
		this->worker(index).push(job);
#else
		this->m_worker_queue.push(job);            // WorkerQueue is thread safe by itself
#endif

		// std::lock_guard<std::mutex> lock(this->m_lock);        // Don't need to take a lock, condition variable doesn't need one. Keep here to debug issues
		this->m_condition_variable.notify_one();

		using return_type = decltype(a_function(a_arguments...));
		return JobHandle<return_type>{job, pack_task->get_future()};
	}

	template <class _function>
	decltype(auto) push_job_group(_function &&a_function, uint32_t a_data_size, uint32_t a_group_size)
	{
		assert(a_group_size > 0 && a_data_size >= a_group_size && "Data and Group size mismatch in push_job_group");

		std::vector<std::shared_ptr<Job>> handles;
		handles.reserve(a_data_size / a_group_size);

		for (uint32_t i = 0; i < a_data_size; i += a_group_size)
		{
			std::shared_ptr<Job> job = std::make_shared<Job>([a_group_size, i, &a_function, a_data_size]() {
				for (uint32_t index = i; index < i + a_group_size && index < a_data_size; ++index)
				{
					a_function(index);
				}
			});

#if defined(WORK_STEALING)
			auto index = this->random_worker();
			this->worker(index).push(job);
#else
			this->m_worker_queue.push(job);        // WorkerQueue is thread safe by itself
#endif

			this->m_condition_variable.notify_one();

			handles.emplace_back(job);
		}

		return this->push_job([]() {}, handles);
	}

	FORCE_INLINE void stop()
	{
		this->m_stop.test_and_set();
		this->m_condition_variable.notify_all();

		for (auto &worker : this->m_workers)
			if (worker->joinable())
				worker->join();
	}

  protected:
  private:
	template <class _function, class... _arguments>
	FORCE_INLINE decltype(auto) make_packaged(_function &&a_function, _arguments &&...a_arguments)
	{
		using return_type = decltype(a_function(a_arguments...));
		return std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<_function>(a_function), std::forward<_arguments>(a_arguments)...));
	}

#if defined(WORK_STEALING)
	FORCE_INLINE uint32_t random_worker() const
	{
		return this->m_random_index.next();
	}

	FORCE_INLINE WorkerQueue &worker(uint32_t a_index)
	{
		return *this->m_worker_queues[static_cast<size_t>(a_index)];
	}

	FORCE_INLINE std::shared_ptr<Job> any_job()
	{
		for (auto &q : this->m_worker_queues)
			if (auto j = q->steal())
				return j;

		return nullptr;
	}

	FORCE_INLINE bool all_empty()
	{
		for (auto &q : this->m_worker_queues)
			if (!q->empty())
				return false;

		return true;
	}

	FORCE_INLINE void run_job(std::shared_ptr<Job> a_job, WorkerQueue &a_worker)
	{
		if (a_job->ready())
		{
			(*a_job)();        // Execute the job
			a_job->finish();
		}
		else
			// this->m_worker_queue.push(a_job);
			a_worker.push(a_job);
	}
#endif

#if !defined(WORK_STEALING)
	void init(uint32_t a_workers_count)
	{
		const auto payload_function = [this](uint32_t a_thread_id) {
			(void) a_thread_id;
			while (true)
			{
				auto job = this->m_worker_queue.pop();
				if (job != nullptr)
				{
					if (job->ready())
					{
						(*job)();        // Execute the job
						job->finish();
					}
					else
						this->m_worker_queue.push(job);
				}
				else
				{
					// Sleep to be awaken later
					std::unique_lock<std::mutex> lock{this->m_lock};        // Using unique_lock instead of lock_guard because it needs to be relocked in the wait next
					this->m_condition_variable.wait(lock, [this]() {
						return !this->m_worker_queue.empty() || this->m_stop.test();
					});

					if (this->m_stop.test())
						return;
				}
			}
		};

		this->m_random_index.reset(0u, a_workers_count - 1);

		// Kick of all the threads
		for (size_t i = 0; i < a_workers_count; ++i)
			this->m_workers.emplace_back(std::make_unique<std::thread>(payload_function, i));
	}
#else
	void init(uint32_t a_workers_count)
	{
		const auto payload_function = [this](uint32_t a_thread_id) {
			(void) a_thread_id;
			auto &my_worker = this->worker(a_thread_id);
			while (true)
			{
				// auto a_job = this->m_worker_queue.pop();
				auto job = my_worker.pop();
				if (job)
				{
					this->run_job(job, my_worker);
				}
				else
				{
					auto &other_worker = this->worker(this->random_worker());
					// Steal a job
					job = other_worker.steal();
					if (job != nullptr)
					{
						this->run_job(job, other_worker);
					}
					else
					{
						// Look for jobs in all queues
						job = this->any_job();
						if (job != nullptr)
						{
							this->run_job(job, my_worker);
						}
						else
						{
							// Sleep to be awaken later
							std::unique_lock<std::mutex> lock{this->m_lock};        // Using unique_lock instead of lock_guard because it needs to be relocked in the wait next
							this->m_condition_variable.wait(lock, [this, &a_thread_id]() {
								// return !this->m_worker_queue.empty() || this->m_stop.test();
								return !this->m_worker_queues[a_thread_id]->empty() || !this->all_empty() || this->m_stop.test();
							});

							if (this->m_stop.test())
								return;
						}
					}
				}
			}
		};

		this->m_random_index.reset(0u, a_workers_count - 1);

		for (size_t i = 0; i < a_workers_count; ++i)
			this->m_worker_queues.emplace_back(std::make_unique<WorkerQueue>());

		// Kick of all the threads
		for (size_t i = 0; i < a_workers_count; ++i)
			this->m_workers.emplace_back(std::make_unique<std::thread>(payload_function, i));
	}
#endif

#if !defined(WORK_STEALING)
	WorkerQueue                               m_worker_queue{};              // list of worker queues containing all the jobs for that worker
#else
	std::vector<std::unique_ptr<WorkerQueue>> m_worker_queues{};        // list of worker queues containing all the jobs for that worker
#endif
	ror::Random<uint32_t>                     m_random_index{};              // Random index generator from 0 - workers_count - 1
	std::condition_variable                   m_condition_variable{};        // Used to signal workers to start working
	std::mutex                                m_lock{};                      // Used to lock shared variables in job system
	std::atomic_flag                          m_stop{false};                 // Used to check if should stop execution
	std::vector<std::unique_ptr<std::thread>> m_workers{};                   // list of workers running jobs that needs running
};

/**
 * Returns a static global JobSystem object, this isn't the only way one can use JobSystem but
 * is the prefered way, because threads creation and destruction isn't for free
 */
FORCE_INLINE JobSystem &get_job_system()
{
	static JobSystem js;

	return js;
}
}        // namespace ror

#include "rorjobsystem.hh"
