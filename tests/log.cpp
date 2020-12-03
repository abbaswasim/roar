#include "common.hpp"
#include "profiling/rorlog.hpp"
#include <gtest/gtest.h>

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace ror_test
{
uint16_t level_to_uint(ror::LogLevel a_level)
{
	switch (a_level)
	{
		case ror::LogLevel::trace:
			return 0;
		case ror::LogLevel::debug:
			return 1;
		case ror::LogLevel::info:
			return 2;
		case ror::LogLevel::warn:
			return 3;
		case ror::LogLevel::error:
			return 4;
		case ror::LogLevel::critical:
			return 5;
		default:
			return 6;
	}
}

void log_and_test(std::string a_log_file, ror::LogLevel a_level)
{
	{
		ror::Log logger(a_log_file);
		logger.set_level(a_level);

		if (level_to_uint(a_level) <= 0)
		{
			print_gtest_header(green);
		}

		logger.trace("This should {} a trace", "print");

		if (level_to_uint(a_level) <= 1)
		{
			print_gtest_header(green);
		}

		logger.debug("This should print a {}", "debug");

		if (level_to_uint(a_level) <= 2)
		{
			print_gtest_header(green);
		}

		logger.info("This will print {}, {}, {}", 1, 2.0f, 3.0);

		if (level_to_uint(a_level) <= 3)
		{
			print_gtest_header(green);
		}

		logger.warn("Print {} warning", 1);

		if (level_to_uint(a_level) <= 4)
		{
			print_gtest_header(green);
		}

		logger.error("Error {}, {}, {}", 1, 2.0f, 3.0);

		if (level_to_uint(a_level) <= 5)
		{
			print_gtest_header(green);
		}

		logger.critical("Critical message {}, {}, {}", 1, 2.0f, 3.0);
	}

	std::ifstream log_file(a_log_file);

	if (!log_file.is_open())
	{
		FAIL();
	}

	if (level_to_uint(a_level) <= 0)
	{
		std::string line;
		std::getline(log_file, line);
		line = line.substr(40);
		ASSERT_STREQ(line.c_str(), "] [trace] This should print a trace");
	}

	if (level_to_uint(a_level) <= 1)
	{
		std::string line;
		std::getline(log_file, line);
		line = line.substr(40);
		ASSERT_STREQ(line.c_str(), "] [debug] This should print a debug");
	}

	if (level_to_uint(a_level) <= 2)
	{
		std::string line;
		std::getline(log_file, line);
		line = line.substr(40);
		ASSERT_STREQ(line.c_str(), "] [info] This will print 1, 2, 3");
	}

	if (level_to_uint(a_level) <= 3)
	{
		std::string line;
		std::getline(log_file, line);
		line = line.substr(40);
		ASSERT_STREQ(line.c_str(), "] [warning] Print 1 warning");
	}

	if (level_to_uint(a_level) <= 4)
	{
		std::string line;
		std::getline(log_file, line);
		line = line.substr(40);
		ASSERT_STREQ(line.c_str(), "] [error] Error 1, 2, 3");
	}

	if (level_to_uint(a_level) <= 5)
	{
		std::string line;
		std::getline(log_file, line);
		line = line.substr(40);
		ASSERT_STREQ(line.c_str(), "] [critical] Critical message 1, 2, 3");
	}
}

TEST(LogTest, LogTest_console_output)
{
	log_and_test("./roar.log", ror::LogLevel::trace);

	log_and_test("./roar_trace.log", ror::LogLevel::trace);
	log_and_test("./roar_debug.log", ror::LogLevel::debug);
	log_and_test("./roar_info.log", ror::LogLevel::info);
	log_and_test("./roar_warn.log", ror::LogLevel::warn);
	log_and_test("./roar_error.log", ror::LogLevel::error);
	log_and_test("./roar_critical.log", ror::LogLevel::critical);
}

TEST(StreamRedirectTest, StreamRedirectTest_cout_capture)
{
	ror::StreamRedirect srdt_g{};
	std::cout << "Created some objects\n";
	{
		ror::StreamRedirect srdt{};
		std::cout << "Created some more objects\n";

		std::string msgs = srdt.m_messages.str();
		ASSERT_STREQ(msgs.c_str(), "Created some more objects\n");
	}

	{
		ror::StreamRedirect srdt{};
		std::cout << "Created lots of objects\n";

		std::string msgs = srdt.m_messages.str();
		ASSERT_STREQ(msgs.c_str(), "Created lots of objects\n");
	}

	std::cout << "Created a lot of objects\n";
	std::string msgs = srdt_g.m_messages.str();
	ASSERT_STREQ(msgs.c_str(), "Created some objects\nCreated a lot of objects\n");
}

}        // namespace ror_test
