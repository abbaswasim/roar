// Roar Source Code
// Wasim Abbas
// http://wazim.i8.com
// Copyright (c) 2009-2010
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Version: 1.0.0 (08 July 2008)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////NOT USED YET////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ror
{
FORCE_INLINE GeometryVBO::GeometryVBO(void) :
    m_id(0)
{}

FORCE_INLINE uint32_t GeometryVBO::get_id() const
{
	return this->m_id;
}

FORCE_INLINE void GeometryVBO::set_id(uint32_t a_id)
{
	this->m_id = a_id;
}

}        // namespace ror
