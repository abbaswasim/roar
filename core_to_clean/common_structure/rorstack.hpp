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

#include "memory_manager/rormemory_check.h"

namespace ror
{
template <class _type>
class ROAR_ENGINE_ITEM Stack
{
  public:
	FORCE_INLINE Stack(void);
	FORCE_INLINE ~Stack(void);
	// Pushes an Element on the stack
	FORCE_INLINE void push(_type a_value);
	// Warning: Slow Version Uses A copy constructor
	// Returns and Erases the Top Element
	FORCE_INLINE _type pop(void);
	// Just returns the Top Element doesn't erase it from the top
	// Warning: No Safety Checks for Empty Vectors ,This Safety should be provided by client
	FORCE_INLINE _type top(void);
	// Check for Empty Stack
	FORCE_INLINE bool empty(void);

  private:
	// Data Needed for Stack
	std::vector<_type> *m_stack;        // Container to keep data for stack
};
}        // namespace ror

#include "rorstack.inl"
