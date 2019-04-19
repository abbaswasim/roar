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

#include <foundation/rorsystem.hpp>
#include <foundation/rortypes.hpp>

#define ROAR_CREATE_SHARED_LIB        // Should Roar consider shared library building

#ifndef ROAR_IMPORTING_SYMBOLS
#	define ROAR_EXPORTING_SYMBOLS        // Are we exporting or being exported
#endif

// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>

#ifndef CONSERVATIVEOPTIMIZATION
#	define LESSCONSERVATIVEOPTIMIZATION
#endif

#if defined(COMPILER_TYPE_GCC) || defined(COMPILER_TYPE_CLANG)
#	define ROAR_HELPER_IMPORT __attribute__((visibility("default")))
#	define ROAR_HELPER_EXPORT __attribute__((visibility("default")))
#	define ROAR_HELPER_LOCAL __attribute__((visibility("hidden")))
#	define ROAR_HELPER_INLINE __inline__
#	define ROAR_HELPER_FORCE_INLINE __attribute__((__always_inline__)) __inline__
#	define ROAR_HELPER_NEVER_INLINE __attribute__((__noinline__))
#elif defined(COMPILER_TYPE_MSVC)
#	define ROAR_HELPER_IMPORT __declspec(dllimport)
#	define ROAR_HELPER_EXPORT __declspec(dllexport)
#	define ROAR_HELPER_LOCAL
#	define ROAR_HELPER_INLINE __inline
#	define ROAR_HELPER_FORCE_INLINE __forceinline
#	define ROAR_HELPER_NEVER_INLINE __declspec(noinline)
#else
#	define ROAR_HELPER_IMPORT
#	define ROAR_HELPER_EXPORT
#	define ROAR_HELPER_LOCAL
#	define ROAR_HELPER_INLINE
#	define ROAR_HELPER_FORCE_INLINE
#	define ROAR_HELPER_NEVER_INLINE
#endif

#ifdef ROAR_CREATE_SHARED_LIB
#	ifdef ROAR_EXPORTING_SYMBOLS        // defined if we are building the shared library instead of using it
#		define ROAR_ENGINE_ITEM ROAR_HELPER_EXPORT
#	else
#		define ROAR_ENGINE_ITEM ROAR_HELPER_IMPORT
#	endif
#	define ROAR_ENGINE_ITEM_LOCAL ROAR_HELPER_LOCAL
#else        // Static lib
#	define ROAR_ENGINE_ITEM
#	define ROAR_ENGINE_ITEM_LOCAL
#endif

#ifdef CONSERVATIVEOPTIMIZATION
#	define FORCE_INLINE ROAR_HELPER_FORCE_INLINE
#else
#	ifdef LESSCONSERVATIVEOPTIMIZATION
#		define FORCE_INLINE ROAR_HELPER_INLINE
#	else
#		define FORCE_INLINE ROAR_HELPER_NEVER_INLINE
#	endif
#endif
