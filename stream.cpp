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
 * \file stream.cpp
 * \date 2015
 */

#include <mtarcommon/allocator.hpp>
#include <mtarcommon/stream.hpp>

#include <algorithm>
#include <fstream>

namespace mtar {

    class stream_impl : public std::fstream
    {
      public:
#if defined(_WIN32)
        stream_impl(const wchar_t* fname, std::ios_base::openmode mode)
          : std::fstream(fname, mode)
        { }
#else//UNIX
        stream_impl(const char* fname, std::ios_base::openmode mode)
          : std::fstream(fname, mode)
        { }
#endif//defined(_WIN32)
    };

    static mtar::allocator<stream_impl> ALLOC;

    istream::istream(const path& p)
      : strm_(ALLOC.allocate(1))
    {
        new (strm_) stream_impl(
#if defined(_WIN32)
                      p.c_str(),
#else// UNIX
                      mtar::to_string(p).c_str(),
#endif//defined(WIN32)
                      std::ios_base::in | std::ios_base::binary);
    }

    istream::~istream()
    {
        strm_->~stream_impl();
        ALLOC.deallocate(strm_, 1);
    }

    size_t istream::read(char* buffer, size_t sz) const
    {
        return static_cast<size_t>(strm_->read(buffer, sz).gcount());
    }

    ostream::ostream(const path& p)
      : strm_(ALLOC.allocate(1))
    {
        new (strm_) stream_impl(
#if defined(_WIN32)
                      p.c_str(),
#else// UNIX
                      mtar::to_string(p).c_str(),
#endif//defined(WIN32)
                      std::ios_base::out | std::ios_base::binary);
    }

    ostream::~ostream()
    {
        strm_->~stream_impl();
        ALLOC.deallocate(strm_, 1);
    }

    void ostream::write(const char* buffer, size_t sz) const
    {
        strm_->write(buffer, sz);
    }

    void stream_copy(const ostream& os, const istream& is, size_t s)
    {
        char BUFF[65536];
        size_t c = s;
        size_t nRead = 65536;
        while ((c > 0) || (nRead != 0)) {
            size_t nToCopy = std::min<size_t>(c, 65536);
            nRead = is.read(BUFF, nToCopy);
            os.write(BUFF, nRead);
            c -= nRead;
        }
    }

}//namespace mtar
