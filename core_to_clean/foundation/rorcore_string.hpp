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

#include "rorsingleton.h"
#include "rorstring_table.h"

namespace ror
{
typedef char8_t *std::strings;

class ROAR_ENGINE_ITEM CoreString
{
  public:
	FORCE_INLINE CoreString(void);
	FORCE_INLINE CoreString(const char8_t *a_string);
	FORCE_INLINE CoreString(CoreString &a_string);
	~CoreString(void);

	FORCE_INLINE operator const char8_t *() const;
	FORCE_INLINE CoreString &operator=(const CoreString &a_string);
	FORCE_INLINE CoreString &operator=(const char8_t *a_string);
	FORCE_INLINE bool        is_empty() const;
	FORCE_INLINE bool        equals(const char8_t *a_string) const;
	FORCE_INLINE bool        contains(const char8_t *a_string) const;
	FORCE_INLINE std::string get_string() const;

	friend bool operator==(const CoreString &a_string1, const CoreString &a_string2);
	friend bool operator!=(const CoreString &a_string1, const CoreString &a_string2);

	friend bool operator==(const CoreString &a_string1, const char8_t *a_string2);
	friend bool operator!=(const CoreString &a_string1, const char8_t *a_string2);

	friend bool operator==(const char8_t *a_string1, const CoreString &a_string2);
	friend bool operator!=(const char8_t *a_string1, const CoreString &a_string2);

  private:
	std::strings m_handle;        // Pointer to the StringTable String
};

}        // namespace ror

#include "rorcore_string.inl"
