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
#include <mutex>

namespace ror
{
template <typename... Args>
void Log::trace(const char *a_format, const Args &...a_args)
{
	this->m_logger->trace(a_format, a_args...);
}

template <typename... Args>
void Log::debug(const char *a_format, const Args &...a_args)
{
	this->m_logger->debug(a_format, a_args...);
}

template <typename... Args>
void Log::info(const char *a_format, const Args &...a_args)
{
	this->m_logger->info(a_format, a_args...);
}

template <typename... Args>
void Log::warn(const char *a_format, const Args &...a_args)
{
	this->m_logger->warn(a_format, a_args...);
}

template <typename... Args>
void Log::error(const char *a_format, const Args &...a_args)
{
	this->m_logger->error(a_format, a_args...);
}

template <typename... Args>
void Log::critical(const char *a_format, const Args &...a_args)
{
	this->m_logger->critical(a_format, a_args...);
}

// Have a global instance of the logger for easy access, its still thread safe if sync_logger is defined, defined by default
Log &       get_logger();
std::mutex &get_logger_lock();
void log_set_level(LogLevel a_level);

#ifndef sync_logger
#	define sync_logger
#endif

#ifdef sync_logger
#	define add_sync() std::lock_guard<std::mutex> lock{get_logger_lock()}
#else
#	define add_sync()
#endif


template <typename... Args>
void log_trace(const char *a_format, const Args &...a_args)
{
	add_sync();
	get_logger().trace(a_format, a_args...);
}

template <typename... Args>
void log_debug(const char *a_format, const Args &...a_args)
{
	add_sync();
	get_logger().debug(a_format, a_args...);
}

template <typename... Args>
void log_info(const char *a_format, const Args &...a_args)
{
	add_sync();
	get_logger().info(a_format, a_args...);
}

template <typename... Args>
void log_warn(const char *a_format, const Args &...a_args)
{
	add_sync();
	get_logger().warn(a_format, a_args...);
}

template <typename... Args>
void log_error(const char *a_format, const Args &...a_args)
{
	add_sync();
	get_logger().error(a_format, a_args...);
}

template <typename... Args>
void log_critical(const char *a_format, const Args &...a_args)
{
	add_sync();
	get_logger().critical(a_format, a_args...);
}

}        // namespace ror
