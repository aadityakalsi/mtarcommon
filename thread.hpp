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
 * \file thread.hpp
 * \date 2015
 */

#ifndef MTAR_COMMON_THREAD_HPP
#define MTAR_COMMON_THREAD_HPP

#include <mtarcommon/defs.hpp>

#if (defined(_MSC_VER) && _MSC_VER >= 1800) || !defined(_MSC_VER)

#include <thread>

namespace mtar {

    void sleep(uint32_t millis)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(millis));
    }

}//namespace mtar

#else

#include <mtarcommon/atomic.hpp>

typedef void(*func)(void);

DWORD WINAPI RunFunction(LPVOID f)
{
    func pf = (func)f;
    pf();
    return 0;
}

namespace std {

    class thread
    {
        HANDLE thread_;
      public:
        thread(func f)
          : thread_(CreateThread(NULL, 0, RunFunction, f, 0, NULL))
        { }

        void join() const
        { WaitForSingleObject(thread_, INFINITE); }

        ~thread() { if (thread_ != INVALID_HANDLE_VALUE) CloseHandle(thread_); }

        static unsigned hardware_concurrency()
        {
            SYSTEM_INFO sysinfo;
            GetSystemInfo( &sysinfo );
            return sysinfo.dwNumberOfProcessors;
        }
    };
}//namespace std

namespace mtar {

    void sleep(UINT32 millis)
    {
        SleepEx(millis, FALSE);
    }

}//namespace mtar

#endif

#endif//MTAR_COMMON_THREAD_HPP
