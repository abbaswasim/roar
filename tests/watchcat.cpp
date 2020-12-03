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

#include "common.hpp"
#include "profiling/rorlog.hpp"
#include <filesystem>
#include <fstream>
#include <functional>
#include <gtest/gtest.h>
#include <istream>
#include <memory>

#include "watchcat/rorwatchcat.hpp"

namespace fs = std::filesystem;

namespace ror_test
{
using callback_signature = std::function<void(fs::path a_path)>;

fs::path get_root_dir()
{
	fs::current_path(fs::temp_directory_path());
	std::filesystem::path root("sandbox");
	return fs::current_path() / root;
}

//Would have been better to have created a WatchCatTest class instead, but for this kind of setup_enviroment function
// Its much harder to make the SetUp and Teardown functions work that gtest provides
std::unique_ptr<ror::WatchCat> setup_environment(callback_signature &   created,
												 callback_signature &   removed,
												 callback_signature &   modified,
												 std::vector<fs::path> &paths, int path_count = 1)
{
	auto root_dir = get_root_dir();

	fs::remove_all(root_dir);
	fs::create_directories(root_dir / "1");
	fs::create_directories(root_dir / "2");
	fs::create_directories(root_dir / "3");

	// These are changed depending on the stage of test we are later in test_create_modify_remove
	created  = [](fs::path) {};
	removed  = [](fs::path) {};
	modified = [](fs::path) {};

	// Different modes
	if (path_count == 2)        // Watch folder root_dir/1 and root_dir/2
	{
		paths.push_back(root_dir / "1");
		paths.push_back(root_dir / "2");
	}
	else if (path_count == 3)        // Watch folder root_dir/1 and root_dir/2 will be added later
	{
		paths.push_back(root_dir / "1");
	}
	else
	{
		paths.push_back(root_dir);        // Watch folder root_dir only
	}

	std::unique_ptr<ror::WatchCat> wc = std::make_unique<ror::WatchCat>(
		paths,
		[&](std::vector<ror::WatchCatEvent> events) {
			for (auto e : events)
			{
				if (e.m_type == ror::WatchCatEventType::add)
				{
					created(e.m_path);
				}
				else if (e.m_type == ror::WatchCatEventType::remove)
				{
					removed(e.m_path);
				}
				else if (e.m_type == ror::WatchCatEventType::change)
				{
					modified(e.m_path);
				}
			}
		},
		0.00001f);

	return wc;
}

void teardown_environment()
{
	auto root_dir = get_root_dir();
	fs::remove_all(root_dir);
}

void test_create_modify_remove(std::function<void(fs::path)> &created,
							   std::function<void(fs::path)> &removed,
							   std::function<void(fs::path)> &modified,
							   fs::path                       file,
							   int                            calls_count_test = 3)
{
	// Wait for the watcher to fire up
	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::atomic<int> call_count = 0;

	created = [&](fs::path p) {
		std::string ps  = p;
		std::string psr = file;
		ASSERT_EQ(ps, psr);
		++call_count;
	};

	{
		auto ff = std::ofstream(file);
		ff << "the quick brown fox ";
		ff.close();
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));
	if (calls_count_test != 0)
		EXPECT_EQ(call_count, 1);
	else
		EXPECT_EQ(call_count, calls_count_test);
	created = [](fs::path) {};
	std::this_thread::sleep_for(std::chrono::seconds(2));

	modified = [&](fs::path p) {
		std::string ps  = p;
		std::string psr = file;
		ASSERT_EQ(ps, psr);

		auto        ff = std::ifstream(file);
		std::string data;
		std::string result("the quick brown fox jumps over the lazy dog!");
		std::getline(ff, data);
		ff.close();
		EXPECT_EQ(data, result);
		++call_count;
	};

	{
		auto ff = std::ofstream(file, std::ios::app);
		ff << "jumps over the lazy dog!";
		ff.close();
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));
	if (calls_count_test != 0)
		EXPECT_EQ(call_count, 2);
	else
		EXPECT_EQ(call_count, calls_count_test);
	created  = [](fs::path) {};
	modified = [](fs::path) {};
	std::this_thread::sleep_for(std::chrono::seconds(2));

	removed = [&](fs::path p) {
		std::string ps  = p;
		std::string psr = file;
		ASSERT_EQ(ps, psr);
		++call_count;
	};

	{
		std::filesystem::remove(file);
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));

	if (calls_count_test != 0)
		EXPECT_EQ(call_count, 3);
	else
		EXPECT_EQ(call_count, calls_count_test);
}

TEST(WatchCatTest, DISABLED_WatchCatTest_single_folder)
{
	std::vector<fs::path> root_dir;

	callback_signature created  = [](fs::path) {};
	callback_signature removed  = [](fs::path) {};
	callback_signature modified = [](fs::path) {};

	auto     wc   = setup_environment(created, removed, modified, root_dir, 1);
	fs::path file = root_dir[0] / "file3.txt";

	wc->run();
	test_create_modify_remove(created, removed, modified, file);
	wc->stop();

	teardown_environment();
}

TEST(WatchCatTest, DISABLED_WatchCatTest_single_folder_run_again)
{
	std::vector<fs::path> root_dir;

	callback_signature created  = [](fs::path) {};
	callback_signature removed  = [](fs::path) {};
	callback_signature modified = [](fs::path) {};

	auto     wc   = setup_environment(created, removed, modified, root_dir, 1);
	fs::path file = root_dir[0] / "file3.txt";

	wc->run();
	test_create_modify_remove(created, removed, modified, file);
	wc->stop();
	wc->run();
	test_create_modify_remove(created, removed, modified, file);
	wc->stop();

	teardown_environment();
}

TEST(WatchCatTest, DISABLED_WatchCatTest_multiple_folders)
{
	std::vector<fs::path> root_dir;

	callback_signature created  = [](fs::path) {};
	callback_signature removed  = [](fs::path) {};
	callback_signature modified = [](fs::path) {};

	auto     wc    = setup_environment(created, removed, modified, root_dir, 2);
	fs::path file1 = root_dir[0] / "file3.txt";
	fs::path file2 = root_dir[1] / "file3.txt";

	wc->run();
	test_create_modify_remove(created, removed, modified, file1);
	test_create_modify_remove(created, removed, modified, file2);
	wc->stop();

	teardown_environment();
}

// TEST(WatchCatTest, DISABLED_WatchCatTest_multiple_folders_run_again)
TEST(WatchCatTest, DISABLED_WatchCatTest_multiple_folders_run_again)
{
	std::vector<fs::path> root_dir;

	callback_signature created  = [](fs::path) {};
	callback_signature removed  = [](fs::path) {};
	callback_signature modified = [](fs::path) {};

	auto     wc    = setup_environment(created, removed, modified, root_dir, 2);
	fs::path file1 = root_dir[0] / "file3.txt";
	fs::path file2 = root_dir[1] / "file3.txt";

	wc->run();
	test_create_modify_remove(created, removed, modified, file1);
	test_create_modify_remove(created, removed, modified, file2);
	wc->stop();

	wc->run();
	test_create_modify_remove(created, removed, modified, file1);
	test_create_modify_remove(created, removed, modified, file2);
	wc->stop();

	teardown_environment();
}

// TEST(WatchCatTest, DISABLED_WatchCatTest_multiple_folders_run_again_add_path)
TEST(WatchCatTest, DISABLED_WatchCatTest_multiple_folders_run_again_add_path)
{
	std::vector<fs::path> root_dir;

	callback_signature created  = [](fs::path) {};
	callback_signature removed  = [](fs::path) {};
	callback_signature modified = [](fs::path) {};

	auto     wc    = setup_environment(created, removed, modified, root_dir, 3);
	fs::path file1 = root_dir[0] / "file3.txt";        // already has 1/ in the root_dir name
	fs::path file2 = get_root_dir() / "2/file3.txt";

	wc->run();
	test_create_modify_remove(created, removed, modified, file1);
	test_create_modify_remove(created, removed, modified, file2, 0);        // Here there shouldn't be any calls for 2/file3.txt
	wc->stop();

	wc->run();
	test_create_modify_remove(created, removed, modified, file1);
	test_create_modify_remove(created, removed, modified, file2, 0);        // Here there shouldn't be any calls for 2/file3.txt
	wc->stop();

	wc->add_path(get_root_dir() / "2");        // also does implicit stop and start
	test_create_modify_remove(created, removed, modified, file1);
	test_create_modify_remove(created, removed, modified, file2);        // Here there should be any 3 calls for 2/file3.txt which is default argument

	auto lat = wc->get_latency();
	EXPECT_DOUBLE_EQ(lat, 0.00001f);

	auto paths = wc->get_paths();

	ASSERT_EQ(paths.size(), 2);

	std::string p1 = get_root_dir() / "1";
	std::string p2 = get_root_dir() / "2";

	ASSERT_STREQ(std::string(paths[0]).c_str(), p1.c_str());
	ASSERT_STREQ(std::string(paths[1]).c_str(), p2.c_str());

	wc->stop();

	teardown_environment();
}

}        // namespace ror_test
