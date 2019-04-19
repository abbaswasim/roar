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
template <class _type>
FORCE_INLINE Stack<_type>::Stack(void)
{
	m_stack = new std::vector<_type>;
}

template <class _type>
Stack<_type>::~Stack(void)
{
	rDELETE(m_stack);
}

template <class _type>
FORCE_INLINE _type Stack<_type>::pop(void)
{
	if (m_stack->empty())
	{
		// Null Pointer from the Front
		return *(m_stack->begin());
	}
	_type end_element = *(m_stack->end() - 1);
	m_stack->pop_back();
	return end_element;
}

template <class _type>
FORCE_INLINE void Stack<_type>::push(_type a_value)
{
	m_stack->push_back(a_value);
}

template <class _type>
FORCE_INLINE _type Stack<_type>::top(void)
{
	// Warning: No Safety Checks for Empty Vectors
	// This Safety should be provided by client
	return *(m_stack->end() - 1);
}

template <class _type>
FORCE_INLINE bool Stack<_type>::empty(void)
{
	return m_stack->empty();
}

}        // namespace ror
