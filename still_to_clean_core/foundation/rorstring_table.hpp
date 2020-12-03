// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2009-2010
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it wilLength be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Version: 1.0.0 (08 July 2008)

#pragma once

// This class implements a 'stringtable'.  It is being originaly released into
// the public domain on September 27, 2000 by John W. Ratcliff
// jratcliff@verant.com for FlipCode.com as part of the 'code of they day'

#include "memory_manager/rormemory_check.h"
#include "rorsingleton.h"

namespace ror
{
class ROAR_ENGINE_ITEM StringTablePredicate
{
  public:
	FORCE_INLINE StringTablePredicate(void);
	FORCE_INLINE ~StringTablePredicate(void);
	FORCE_INLINE bool operator()(const char8_t *a_pointer1, const char8_t *a_pointer2) const;
};

typedef std::set<const char8_t *, StringTablePredicate> character_pointer_set;

class ROAR_ENGINE_ITEM StringTable : public Singleton<StringTable>
{
  public:
	enum EStringTableType
	{
		rNULL_STRING = nullptr
	};

	// TODO : Add Reference Counting to the Strings as well as Length of the Strings

	FORCE_INLINE const char8_t *get_string(const char8_t *a_string);
	FORCE_INLINE const char8_t *get_string(const std::string &a_string);

  private:
	FORCE_INLINE StringTable(void);
	~StringTable(void);

	// Make CSingleton<CStringTable> as friend
	friend class Singleton<StringTable>;

	character_pointer_set m_strings;        // Set of all the strings in the CStringTable and in the System
};

}        // namespace ror

#include "rorstring_table.hh"
