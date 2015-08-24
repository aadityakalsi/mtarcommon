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
 * \file parallel_for.cpp
 * \date 2015
 */

#include <mtarcommon/allocator.hpp>
#include <mtarcommon/parallel_for.hpp>
#include <mtarcommon/atomic.hpp>
#include <mtarcommon/thread.hpp>

#include <vector>
#include <algorithm>

#if !defined(NDEBUG)
#include <iostream>
#endif

namespace mtar {

    // Good:
    static const size_t INVOKER_SLEEP =  10 /*microsec*/;
    static const size_t WORKER_SLEEP  =   1 /*microsec*/;

    static allocator<std::thread> ALLOC;
    static const unsigned MAX_THREADS = std::thread::hardware_concurrency();

    int                       THREAD_INIT = 0;
    std::vector<std::thread*> PTHREADS(MAX_THREADS, NULL);

    static std::vector<size_t>     STARTS(MAX_THREADS, 0);
    static std::vector<size_t>     ENDS(MAX_THREADS, 0);
    static parallel_fcn_t          THREAD_FCN;
    static void*                   THREAD_DATA;
    static atomic_int              THREAD_START;
    static std::vector<atomic_int> THREAD_DONE(MAX_THREADS);

    atomic_int THREAD_NUM;
    void loop_runner(void)
    {
        int threadnum = -1;
        for (;;) {
            if (THREAD_START.load() == 1 && (threadnum == -1 || THREAD_DONE[threadnum].load() == 0)) {
                if (threadnum == -1) {
                    threadnum = THREAD_NUM++;
                }
                THREAD_FCN(STARTS[threadnum], ENDS[threadnum], THREAD_DATA);
                THREAD_DONE[threadnum].store(1);
            }
            sleep(WORKER_SLEEP);
        }
    }

    void parallel_for(size_t start, size_t end, parallel_fcn_t fcn, void* data)
    {
        using std::thread;

        size_t numitersperthread = (end - start) / MAX_THREADS;

        if (THREAD_INIT == 0) {
            THREAD_START.store(0);
            for (size_t i = 0; i != MAX_THREADS; ++i) {
                new (&PTHREADS[i]) thread(loop_runner);
            }
            THREAD_INIT = 1;
        }

        // set starts and ends
        size_t next = start;
        for (size_t i = 0; i != MAX_THREADS-1; ++i) {
            STARTS[i] = next;
            next += numitersperthread;
            ENDS[i] = next;
        }
        STARTS[MAX_THREADS-1] = next;
        ENDS[MAX_THREADS-1] = end;

        // set thread function
        THREAD_FCN  = fcn;
        THREAD_DATA = data;
        
        // set states to not done
        for (size_t i = 0; i != MAX_THREADS; ++i) {
            THREAD_DONE[i].store(0);
        }

        // START!
        THREAD_START.store(1);
        // check for done state
        for (size_t i = 0; i != MAX_THREADS; ++i) {
            while (THREAD_DONE[i].load() == 0) { sleep(INVOKER_SLEEP); }
        }

        THREAD_START.store(0);
        THREAD_FCN = NULL;
    }

}//namespace mtar
