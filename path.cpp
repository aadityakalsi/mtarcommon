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
 * \file path.cpp
 * \date 2015
 */

#include <mtarcommon/path.hpp>

#include <cstdlib>
#include <list>

namespace mtar {
namespace detail {

    class mtar_allocator_vec
    {
        std::list<char*> segments_;
        char*            buf_;
        size_t           idx_;

        static const size_t BLOCK_SIZE = 10000000;

      public:
        mtar_allocator_vec()
          : segments_()
          , idx_(BLOCK_SIZE)
        { }

        MTAR_COMMON_INLINE
        void* malloc(size_t n)
        {
            if (idx_ + n > BLOCK_SIZE) {
                char* buf = new (std::nothrow) char[BLOCK_SIZE];
                if (buf == nullptr) { throw std::bad_alloc(); }
                buf_ = buf;
                segments_.push_back(buf);
                idx_ = 0;
            }
            void* p = buf_ + idx_;
            // always return aligned at 8 bytes
            size_t rem = n % 8;
            idx_ += n + ((rem == 0) ? 0 : (8-rem));
            return p;
        }

        MTAR_COMMON_INLINE
        void free(void* p, size_t n)
        {
            if ((buf_ + idx_ - n) == p) {
                idx_ -= n;
            }
        }

        ~mtar_allocator_vec()
        {
            for (auto& seg : segments_) {
                delete [] seg;
            }
            segments_.clear();
        }
    };

    mtar_allocator_vec& alloc()
    {
        static detail::mtar_allocator_vec ALLOC;
        return ALLOC;
    }

    void* allocate_pvt_vec(size_t num)
    {
        return alloc().malloc(num);
    }

    void deallocate_pvt_vec(void* p, size_t num)
    {
        return alloc().free((char*)p, num);
    }

}//namespace detail
}//namespace mtar
