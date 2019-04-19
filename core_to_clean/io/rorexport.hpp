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
class ROAR_ENGINE_ITEM Export
{
  public:
	FORCE_INLINE Export(void);
	FORCE_INLINE Export(uint32_t a_out, std::string a_file_name);
	~Export(void);

	FORCE_INLINE bool load_file();
	FORCE_INLINE void write(char8_t *a_bytes, uint32_t a_size);
	//		FORCE_INLINE void				Write(uint32_t a_Size);
	//		FORCE_INLINE void				Write(float32_t a_Size);
	FORCE_INLINE void read(float32_t *a_bytes, uint32_t a_size);
	FORCE_INLINE void read(uint32_t *a_bytes, uint32_t a_size);
	FORCE_INLINE void read(int32_t *a_bytes, uint32_t a_size);
	FORCE_INLINE void read(char8_t *a_bytes, uint32_t a_size);
	FORCE_INLINE void read(uint32_t *a_size);
	FORCE_INLINE void read(int32_t *a_size);
	FORCE_INLINE void read(rUsShort *a_size);
	FORCE_INLINE void read(rShort *a_size);
	FORCE_INLINE void read(float32_t *a_size);
	FORCE_INLINE void finish_write();

  private:
	std::fstream m_file;           // Handle to the File
	bool         m_input;          // whether is used for reading or writing file
	char8_t *    m_buffer;         // Data
	char8_t *    m_buffer1;        // Copy of the pointer to Data .which is used for deleting the buffer
};
}        // namespace ror

#include "rorexport.inl"
