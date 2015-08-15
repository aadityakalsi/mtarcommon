/*
Copyright (c) 2015, Aaditya Kalsi
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * \file defs.hpp
 * \date 2015
 */

#ifndef MTAR_COMMON_DEFS_HPP
#define MTAR_COMMON_DEFS_HPP

#if defined(MTAR_COMMON_BUILD)
#  ifdef _MSC_VER
#    define MTAR_COMMON_API __declspec(dllexport)
#  else
#    define MTAR_COMMON_API __attribute__((__visibility__("default")))
#  endif
#else
#  ifdef _MSC_VER
#    define MTAR_COMMON_API __declspec(dllimport)
#  else
#    define MTAR_COMMON_API
#  endif
#endif

#if defined(__cplusplus)
#  if defined(_MSC_VER)
#    define NOEXCEPT
#  else//GCC like compiler
#    define NOEXCEPT noexcept
#  endif//defined(_MSC_VER)
#endif//defined(__cplusplus)

#if !defined(MTAR_COMMON_INLINE)
#  if defined(_MSC_VER)
#    define MTAR_COMMON_INLINE __forceinline
#  else//GCC like compiler
#    define MTAR_COMMON_INLINE inline __attribute__((always_inline))
#  endif//defined(_MSC_VER)
#endif//!defined(MTAR_COMMON_INLINE)

#if defined (__GNUC__)
#  define MTAR_TLS __thread
#elif defined (_MSC_VER)
#  define MTAR_TLS __declspec(thread)
#else // !__GNUC__ && !_MSC_VER
#  define MTAR_TLS thread_local
#endif

#endif//MTAR_COMMON_DEFS_HPP
