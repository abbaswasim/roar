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

namespace ror
{
FORCE_INLINE ParticleSystemObject::ParticleSystemObject(void) :
    m_reference_count(1),
    m_system_pointer(nullptr)
{}

FORCE_INLINE uint32_t ParticleSystemObject::increment_reference_count()
{
	return ++this->m_reference_count;
}

FORCE_INLINE uint32_t ParticleSystemObject::decrement_reference_count()
{
	return --this->m_reference_count;
}

FORCE_INLINE uint32_t ParticleSystemObject::get_reference_count() const
{
	return this->m_reference_count;
}

FORCE_INLINE void ParticleSystemObject::set_reference_count(uint32_t &a_reference_count)
{
	this->m_reference_count = a_reference_count;
}

FORCE_INLINE ParticleSystemBase *ParticleSystemObject::get_system_pointer() const
{
	return this->m_system_pointer;
}

FORCE_INLINE void ParticleSystemObject::set_system_pointer(ParticleSystemBase *a_system_pointer)
{
	this->m_system_pointer = a_system_pointer;
}

}        // namespace ror
