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

#include "rorlog.hpp"

#if defined(__ANDROID__)
#	include "spdlog/sinks/android_sink.h"
#else
#	include "spdlog/sinks/stdout_color_sinks.h"
#endif

#include "spdlog/sinks/basic_file_sink.h"

namespace ror
{
StreamRedirect::StreamRedirect()
{
	this->m_stream_buffer_backup = std::cout.rdbuf();
	std::cout.rdbuf(this->m_messages.rdbuf());
}

StreamRedirect::~StreamRedirect()
{
	std::cout.rdbuf(this->m_stream_buffer_backup);
}

Log::Log(const std::string &a_log_file)
{
#if defined(__ANDROID__)
	const std::string tag = "ROAR";

	this->m_logger = spdlog::android_logger_mt("roar::android", tag);
#else
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto file_sink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>(((a_log_file == "") ? "./roar.log" : a_log_file), true);

	std::initializer_list<spdlog::sink_ptr> sink_list{console_sink, file_sink};

	this->m_logger = std::make_unique<spdlog::logger>("roar::desktop", sink_list);

	// How to set pattern
	// file_sink->set_pattern("[ROAR] [%^%l%$] %v");
#endif
}

spdlog::level::level_enum convert_to_spdlog_level(LogLevel a_level)
{
	switch (a_level)
	{
		case LogLevel::trace:
			return spdlog::level::trace;
		case LogLevel::debug:
			return spdlog::level::debug;
		case LogLevel::info:
			return spdlog::level::info;
		case LogLevel::warn:
			return spdlog::level::warn;
		case LogLevel::error:
			return spdlog::level::err;
		case LogLevel::critical:
			return spdlog::level::critical;
	}
	return spdlog::level::off;
}

void Log::set_level(LogLevel a_level)
{
	if (this->m_logger != nullptr)
	{
		this->m_logger->set_level(convert_to_spdlog_level(a_level));
	}
}

Log &get_logger()
{
	static Log logger;
	return logger;
}

std::mutex &get_logger_lock()
{
	static std::mutex sync;
	return sync;
}

void log_set_level(LogLevel a_level)
{
	add_sync();
	get_logger().set_level(a_level);
}

}        // namespace ror
