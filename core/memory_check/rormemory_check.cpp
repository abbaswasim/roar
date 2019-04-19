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

#include "memory_manager/rormemory_check.h"
#include <mutex>

std::vector<Allocation *> allocation_list;
std::mutex                allocation_mutex;

FORCE_INLINE void *operator new(size_t a_allocation_size, const char *a_file_name, int a_line_number)
{
	void *ptr = (void *) malloc(a_allocation_size);
	record_allocation(ptr, a_allocation_size, a_file_name, a_line_number);
	return ptr;
}

FORCE_INLINE void *operator new[](size_t a_allocation_size, const char *a_file_name, int a_line_number)
{
	void *ptr = (void *) malloc(a_allocation_size);
	record_allocation(ptr, a_allocation_size, a_file_name, a_line_number);
	return ptr;
}

FORCE_INLINE void operator delete(void *a_pointer_to_delete) throw()
{
	delete_allocation(a_pointer_to_delete);
	free(a_pointer_to_delete);
}

FORCE_INLINE void operator delete[](void *a_pointer_to_delete) throw()
{
	delete_allocation(a_pointer_to_delete);
	free(a_pointer_to_delete);
}

FORCE_INLINE void record_allocation(void *a_address, size_t a_size, const char *a_file_name, size_t a_line_number)
{
	Allocation *                info = new Allocation{(size_t) a_address, a_size, a_file_name, a_line_number};
	std::lock_guard<std::mutex> lock(allocation_mutex);
	allocation_list.push_back(info);
}

FORCE_INLINE void delete_allocation(void *a_address)
{
	std::lock_guard<std::mutex> lock(allocation_mutex);
	auto                        pointer = std::find_if(allocation_list.begin(), allocation_list.end(), [a_address](const Allocation *allocation) {
        return allocation->m_address_of_allocation == (size_t) a_address;
    });

	if (pointer != allocation_list.end())
		allocation_list.erase(pointer);
}

FORCE_INLINE void dump_potential_leaks()
{
	std::vector<Allocation *>::iterator index;

	for (auto i = allocation_list.begin(); i != allocation_list.end(); i++)
	{
		std::cout << "There might be a leak at address 0x" << std::hex << (*i)->m_address_of_allocation << " at line " << std::dec << (*i)->m_line_number << " in file " << (*i)->m_file_name << " of size " << (*i)->m_allocation_size << std::endl
		          << std::flush;
	}
}

#endif
#endif

#ifndef NDEBUG
#	ifdef USEROARMEMORYMANAGER
#		define new new (__FILE__, __LINE__)
#	endif
#endif
