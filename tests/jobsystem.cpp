// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

#include "common.hpp"
#include "profiling/rorlog.hpp"
#include <atomic>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <foundation/rortypes.hpp>
#include <functional>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest.h>
#include <memory>
#include <mutex>
#include <profiling/rortimer.hpp>
#include <string>
#include <thread>
#include <vector>

#include "foundation/rorjobsystem.hpp"

namespace ror_test
{

// Functionality tests
TEST(JobSystemTest, create_many_jobs)
{
	ror::JobSystem js(ror::get_hardware_threads());

	using namespace std::chrono_literals;

	auto payload = []() {
		std::string tmp{""};

		for (uint32_t i = 0; i < 1; ++i)
		{
			tmp += "AA" + std::to_string(i + 0);
			tmp += "BB" + std::to_string(i + 1);
			tmp += "CC" + std::to_string(i + 2);
			tmp += "DD" + std::to_string(i + 3);
			tmp += "EE" + std::to_string(i + 4);
			tmp += "FF" + std::to_string(i + 5);
		}
		return tmp;
	};

	auto jh0 = js.push_job(payload);
	auto jh1 = js.push_job(payload);
	auto jh2 = js.push_job(payload);
	auto jh3 = js.push_job(payload);
	auto jh4 = js.push_job(payload);

	ror::Dependencies deps{jh0.job(), jh1.job(), jh2.job(), jh3.job(), jh4.job()};

	auto jh = js.push_job([&]() -> bool {
		auto &o0 = jh0.future();
		auto &o1 = jh1.future();
		auto &o2 = jh2.future();
		auto &o3 = jh3.future();
		auto &o4 = jh4.future();

		EXPECT_TRUE(o0.valid());
		EXPECT_TRUE(o1.valid());
		EXPECT_TRUE(o2.valid());
		EXPECT_TRUE(o3.valid());
		EXPECT_TRUE(o4.valid());

		auto res0 = o0.get();
		auto res1 = o1.get();
		auto res2 = o2.get();
		auto res3 = o3.get();
		auto res4 = o4.get();

		std::string res{"AA0BB1CC2DD3EE4FF5"};
		EXPECT_STREQ(res.c_str(), res0.c_str());
		EXPECT_STREQ(res.c_str(), res1.c_str());
		EXPECT_STREQ(res.c_str(), res2.c_str());
		EXPECT_STREQ(res.c_str(), res3.c_str());
		EXPECT_STREQ(res.c_str(), res4.c_str());

		return true;
	},
						  deps);

	auto res = jh.data();

	ASSERT_TRUE(res);

	js.stop();
}

TEST(JobSystemTest, create_ordered_jobs)
{
	ror::JobSystem js(ror::get_hardware_threads());

	using namespace std::chrono_literals;

	std::string result{};

	auto payload = [&result](uint32_t a_counter, std::string a_token) -> decltype(auto) {
		std::string tmp{""};
		tmp += std::to_string(a_counter);
		tmp += a_token;

		for (uint32_t i = 0; i < a_counter; ++i)
		{
			tmp += "AA" + std::to_string(i + 0);
			tmp += "BB" + std::to_string(i + 1);
			tmp += "CC" + std::to_string(i + 2);
			tmp += "DD" + std::to_string(i + 3);
			tmp += "EE" + std::to_string(i + 4);
			tmp += "FF" + std::to_string(i + 5);
		}

		result += tmp;

		return true;
	};

	auto jh0 = js.push_job(payload, 0, "zero");
	auto jh1 = js.push_job(payload, jh0.job(), 1, "one");
	auto jh2 = js.push_job(payload, jh1.job(), 2, "two");
	auto jh3 = js.push_job(payload, jh2.job(), 3, "three");
	auto jh4 = js.push_job(payload, jh3.job(), 4, "four");

	ror::Dependencies deps{jh4.job()};

	auto jh = js.push_job([&]() -> bool {
		auto &o0 = jh0.future();
		auto &o1 = jh1.future();
		auto &o2 = jh2.future();
		auto &o3 = jh3.future();
		auto &o4 = jh4.future();

		EXPECT_TRUE(o0.valid());
		EXPECT_TRUE(o1.valid());
		EXPECT_TRUE(o2.valid());
		EXPECT_TRUE(o3.valid());
		EXPECT_TRUE(o4.valid());

		auto res0 = o0.get();
		auto res1 = o1.get();
		auto res2 = o2.get();
		auto res3 = o3.get();
		auto res4 = o4.get();

		EXPECT_TRUE(res0);
		EXPECT_TRUE(res1);
		EXPECT_TRUE(res2);
		EXPECT_TRUE(res3);
		EXPECT_TRUE(res4);

		std::string res{"0zero1oneAA0BB1CC2DD3EE4FF52twoAA0BB1CC2DD3EE4FF5AA1BB2CC3DD4EE5FF63threeAA0BB1CC2DD3EE4FF5AA1BB2CC3DD4EE5FF6AA2BB3CC4DD5EE6FF74fourAA0BB1CC2DD3EE4FF5AA1BB2CC3DD4EE5FF6AA2BB3CC4DD5EE6FF7AA3BB4CC5DD6EE7FF8"};
		EXPECT_STREQ(res.c_str(), result.c_str());

		return true;
	},
						  deps);

	auto res = jh.data();

	ASSERT_TRUE(res);

	js.stop();
}

TEST(JobSystemTest, create_out_of_ordered_jobs)
{
	ror::JobSystem js(ror::get_hardware_threads());

	using namespace std::chrono_literals;

	std::string result{};
	std::mutex  mtx;

	auto payload = [&result, &mtx](uint32_t a_counter, std::string a_token) -> decltype(auto) {
		std::string tmp{""};
		tmp += std::to_string(a_counter);
		tmp += a_token;

		for (uint32_t i = a_counter; i < (a_counter * 2); ++i)
		{
			tmp += "AA" + std::to_string(i + 0);
			tmp += "BB" + std::to_string(i + 1);
			tmp += "CC" + std::to_string(i + 2);
			tmp += "DD" + std::to_string(i + 3);
			tmp += "EE" + std::to_string(i + 4);
			tmp += "FF" + std::to_string(i + 5);
		}

		std::lock_guard<std::mutex> lock(mtx);
		result += tmp;

		return true;
	};

	auto jh0 = js.push_job(payload, 0, "zero");
	auto jh1 = js.push_job(payload, 1, "one");
	auto jh2 = js.push_job(payload, 2, "two");
	auto jh3 = js.push_job(payload, 3, "three");
	auto jh4 = js.push_job(payload, 4, "four");

	ror::Dependencies deps{jh0.job(), jh1.job(), jh2.job(), jh3.job(), jh4.job()};

	auto jh = js.push_job([&]() -> bool {
		auto &o0 = jh0.future();
		auto &o1 = jh1.future();
		auto &o2 = jh2.future();
		auto &o3 = jh3.future();
		auto &o4 = jh4.future();

		EXPECT_TRUE(o0.valid());
		EXPECT_TRUE(o1.valid());
		EXPECT_TRUE(o2.valid());
		EXPECT_TRUE(o3.valid());
		EXPECT_TRUE(o4.valid());

		auto res0 = o0.get();
		auto res1 = o1.get();
		auto res2 = o2.get();
		auto res3 = o3.get();
		auto res4 = o4.get();

		EXPECT_TRUE(res0);
		EXPECT_TRUE(res1);
		EXPECT_TRUE(res2);
		EXPECT_TRUE(res3);
		EXPECT_TRUE(res4);

		std::vector<std::string> res{
			"0zero",
			"1oneAA1BB2CC3DD4EE5FF6",
			"2twoAA2BB3CC4DD5EE6FF7AA3BB4CC5DD6EE7FF8",
			"3threeAA3BB4CC5DD6EE7FF8AA4BB5CC6DD7EE8FF9AA5BB6CC7DD8EE9FF10",
			"4fourAA4BB5CC6DD7EE8FF9AA5BB6CC7DD8EE9FF10AA6BB7CC8DD9EE10FF11AA7BB8CC9DD10EE11FF12"};

		for (auto &r : res)
		{
			auto a = result.find(r);
			EXPECT_TRUE(a != std::string::npos);
		}

		return true;
	},
						  deps);

	auto res = jh.data();

	ASSERT_TRUE(res);

	js.stop();
}

// Performance tests
// From the excellent https://wickedengine.net/2018/11/24/simple-job-system-using-standard-c/
float spin(float milliseconds)
{
	milliseconds /= 1000.0f;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	double                                         ms = 0;
	while (ms < milliseconds)
	{
		std::chrono::high_resolution_clock::time_point t2        = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double>                  time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		ms                                                       = time_span.count();
	}

	return milliseconds;
}

void test_spin_ms(float a_ms)
{
	ror::Timer     timer;
	const uint32_t iters = 20;

	float32_t spin_res{0.0f};

	// Serial test
	timer.tick();

	for (size_t i = 0; i < iters; ++i)
		spin_res += spin(a_ms);

	auto time_serial = timer.tick();

	// Parallel test
	ror::JobSystem js{};

	std::vector<ror::JobHandle<float32_t>> handles;
	handles.reserve(iters);

	timer.tick();

	for (size_t i = 0; i < iters; ++i)
		handles.emplace_back(js.push_job(spin, a_ms));

	float32_t jobs_res{0.0f};

	for (size_t i = 0; i < iters; ++i)
		jobs_res += handles[i].data();

	auto time_parallel = timer.tick();

	EXPECT_EQ(jobs_res, spin_res);
	EXPECT_TRUE(time_parallel < time_serial);

	js.stop();
}

TEST(JobSystemTest, performance_test)
{
	for (float i = 0.01f; i < 1.0f; i += 0.1f)
		test_spin_ms(i);
}

TEST(JobSystemTest, job_group_test)
{
	struct Data
	{
		float                m[16];
		std::atomic_uint32_t index{0};

		void compute(uint32_t value)
		{
			for (uint32_t i = 0; i < 16; ++i)
			{
				m[i] += float(value + i * 0);
			}
			index.store(value);
		}
	};

	uint32_t              dataCount = 1000000;
	ror::Timer            timer;
	int64_t               loop_time{0};
	int64_t               threads_time{0};
	int64_t               group_time{0};
	uint32_t              threads_count = ror::get_hardware_threads();
	std::vector<uint32_t> res{999980, 999981, 999982, 999983, 999984, 999985, 999986, 999987, 999988, 999989, 999990, 999991, 999992, 999993, 999994, 999995, 999996, 999997, 999998, 999999};

	// Loop test
	{
		Data *dataSet = new Data[dataCount];

		timer.tick();

		for (uint32_t i = 0; i < dataCount; ++i)
			dataSet[i].compute(i);

		loop_time = timer.tick();

		delete[] dataSet;
	}

	// Threads test
	{
		ror::JobSystem js(threads_count);

		// Threads test:
		Data *dataSet = new Data[dataCount];
		auto  f       = [&dataSet](uint32_t j) {
			dataSet[j].compute(j);
		};

		timer.tick();

		for (uint32_t i = 0; i < dataCount; ++i)
			js.push_job(f, i);

		js.stop();

		threads_time = timer.tick();

		for (uint32_t i = dataCount - 20, j = 0; i < dataCount; ++i, ++j)
			EXPECT_EQ(res[j], dataSet[i].index);

		delete[] dataSet;
	}

	// Group test
	{
		ror::JobSystem js{threads_count};

		// Job group test
		Data *dataSet = new Data[dataCount];

		auto f = [&dataSet](uint32_t j) {
			dataSet[j].compute(j);
		};

		timer.tick();

		auto jh = js.push_job_group(f, dataCount, dataCount / threads_count);

		if (jh.future().valid())
			jh.future().get();

		group_time = timer.tick();

		for (uint32_t i = dataCount - 20, j = 0; i < dataCount; ++i, ++j)
			EXPECT_EQ(res[j], dataSet[i].index);

		delete[] dataSet;

		js.stop();
	}

	EXPECT_TRUE(group_time < loop_time);
	EXPECT_TRUE(group_time < threads_time);
	EXPECT_TRUE(threads_time > loop_time);
}
}        // namespace ror_test
