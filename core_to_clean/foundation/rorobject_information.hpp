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
class ROAR_ENGINE_ITEM ObjectInformation
{
  public:
	// Ctors and Dtors
	FORCE_INLINE ObjectInformation(void);
	virtual ~ObjectInformation(void);

	// Accessors and Mutator Functions
	FORCE_INLINE std::string get_name() const;
	FORCE_INLINE void        set_name(std::string a_name);
	FORCE_INLINE uint32_t get_unique_id() const;
	FORCE_INLINE uint32_t get_id() const;

  protected:
	FORCE_INLINE void set_id(uint32_t a_id);        // Usually Should not allow to set ID Explicitly

	// Data for Objects
	std::string     m_name;             // Name of the Object
	uint32_t        m_id;               // Unique ID of the Object
	static uint32_t m_unique_id;        // Unique ID generator
};
}        // namespace ror

#include "rorobject_information.inl"
