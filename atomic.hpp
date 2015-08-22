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
    * Neither the name of the organization nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * \file atomic.hpp
 * \date 2015
 */

#ifndef MTAR_COMMON_ATOMIC_HPP
#define MTAR_COMMON_ATOMIC_HPP

#include <mtarcommon/defs.hpp>

#if defined(_MSC_VER) && _MSC_VER < 1800 // VS2013 with C++11 support

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace mtar {

    class atomic_int
    {
        volatile LONG int_;
      public:
        atomic_int()
          : int_()
        { }

        MTAR_COMMON_INLINE
        void store(int val)
        {
            LONG v = val;
            InterlockedExchange(&int_, v);
        }

        MTAR_COMMON_INLINE
        void compare_exchange_until(int oldval, int newval)
        {
            LONG o = oldval;
            LONG n = newval;
            for (; InterlockedCompareExchange(&int_, o, n) != o; ) { }
        }

        MTAR_COMMON_INLINE
        int load() const
        {
            return int_;
        }
    };

}//namespace mtar

#else// atomic support available

#include <atomic>

namespace mtar {

    class atomic_int
    {
        std::atomic<int> int_;
      public:
        atomic_int()
          : int_()
        { }

        MTAR_COMMON_INLINE
        void store(int val)
        {
            int_.store(val);
        }

        MTAR_COMMON_INLINE
        void compare_exchange_until(int oldval, int newval)
        {
            for(; !atomic_compare_exchange_weak(&int_, &oldval, newval); ) { }
        }

        MTAR_COMMON_INLINE
        int load() const
        {
            return int_.load();
        }
    };

}//namespace mtar

#endif

#endif//MTAR_COMMON_ATOMIC_HPP

