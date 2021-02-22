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

#include "roar.hpp"
#include "spdlog/spdlog.h"
#include <iostream>
#include <sstream>

#include "math/rormatrix4.hpp"

namespace ror
{
/**
* Redirects cout stream to internal buffer
*/
class ROAR_ENGINE_ITEM StreamRedirect
{
  public:
	StreamRedirect();                                                             //! Default constructor
	StreamRedirect(const StreamRedirect &a_other)     = delete;                   //! Copy constructor
	StreamRedirect(StreamRedirect &&a_other) noexcept = delete;                   //! Move constructor
	StreamRedirect &operator=(const StreamRedirect &a_other) = delete;            //! Copy assignment operator
	StreamRedirect &operator=(StreamRedirect &&a_other) noexcept = delete;        //! Move assignment operator
	virtual ~StreamRedirect();                                                    //! Destructor

	std::stringstream m_messages;        //! all cout messages are redicted here

  protected:
  private:
	std::streambuf *m_stream_buffer_backup = nullptr;        //! Backup of cout's existing buffer restored on destruction
};

enum class LogLevel
{
	trace,
	debug,
	info,
	warn,
	error,
	critical,
};

class ROAR_ENGINE_ITEM Log final
{
  public:
	Log(const std::string &a_log_file = "");                 //! Default constructor
	Log(const Log &a_other)     = delete;                    //! Copy constructor
	Log(Log &&a_other) noexcept = delete;                    //! Move constructor
	Log &operator=(const Log &a_other) = delete;             //! Copy assignment operator
	Log &operator=(Log &&a_other) noexcept = delete;         //! Move assignment operator
	~Log() noexcept                        = default;        //! Destructor

	template <typename... Args>
	void trace(const char *a_format, const Args &...a_args);

	template <typename... Args>
	void debug(const char *a_format, const Args &...a_args);

	template <typename... Args>
	void info(const char *a_format, const Args &...a_args);

	template <typename... Args>
	void warn(const char *a_format, const Args &...a_args);

	template <typename... Args>
	void error(const char *a_format, const Args &...a_args);

	template <typename... Args>
	void critical(const char *a_format, const Args &...a_args);

	// Can be used to filter out unimporant messages
	void set_level(LogLevel a_level);

  protected:
  private:
	std::unique_ptr<spdlog::logger> m_logger = nullptr;
};

void log_set_level(LogLevel a_level);

template <typename... Args>
void log_trace(const char *a_format, const Args &...a_args);

template <typename... Args>
void log_debug(const char *a_format, const Args &...a_args);

template <typename... Args>
void log_info(const char *a_format, const Args &...a_args);

template <typename... Args>
void log_warn(const char *a_format, const Args &...a_args);

template <typename... Args>
void log_error(const char *a_format, const Args &...a_args);

template <typename... Args>
void log_critical(const char *a_format, const Args &...a_args);

}        // namespace ror

#include "rorlog.hh"
