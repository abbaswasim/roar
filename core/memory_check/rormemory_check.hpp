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
#include <string>

#define USEROARMEMORYMANAGER

// TODO Make this use os specific log, not cout, also convert size_t to proper type

// Always use unique_ptr and shared_ptr but this is a fail safe

#if defined(RORDEBUG)
#	if defined(USEROARMEMORYMANAGER)

class Allocation
{
  public:
	size_t      m_address_of_allocation;        // Address at which the Memory is Allocated
	size_t      m_allocation_size;              // Size of Allocation
	std::string m_file_name;                    // File Name From Where its Generated
	size_t      m_line_number;                  // And Line Number in m_FileName
};

FORCE_INLINE void *operator new(size_t a_allocation_size, const char *a_file_name, int a_line_number);
FORCE_INLINE void *operator new[](size_t a_allocation_size, const char *a_file_name, int a_line_number);
void               operator delete(void *a_pointer_to_delete) throw();
void               operator delete[](void *a_pointer_to_delete) throw();

FORCE_INLINE void record_allocation(void *a_address, size_t a_size, const char *a_file_name, size_t a_line_number);
FORCE_INLINE void delete_allocation(void *a_address);
FORCE_INLINE void dump_potential_leaks();

#	endif
#endif
