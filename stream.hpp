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
 * \file stream.hpp
 * \date 2015
 */

#ifndef MTAR_COMMON_STREAM_HPP
#define MTAR_COMMON_STREAM_HPP

#include <mtarcommon/defs.hpp>
#include <mtarcommon/noncopyable.hpp>
#include <mtarcommon/string.hpp>
#include <mtarcommon/path.hpp>

#if defined(_WIN32)
typedef unsigned short mode_t;

#define S_IRUSR  0000400
#define S_IRGRP  0000040
#define S_IROTH  0000004
#define S_IWUSR  0000200
#define S_IWGRP  0000020
#define S_IWOTH  0000002
#define S_IXUSR  0000100
#define S_IXGRP  0000010
#define S_IXOTH  0000001
#define S_IRWXU  0000700
#define S_IRWXG  0000070
#define S_IRWXO  0000007

#endif//defined(_WIN32)

namespace mtar {

    class stream_impl;

    //!
    //!
    //!
    class MTAR_COMMON_API istream : public noncopyable
    {
      public:
        istream(const path& p);

        ~istream();

        size_t read(char* buffer, size_t sz) const;

      private:
        stream_impl* strm_;
    };

    //!
    //!
    //!
    class MTAR_COMMON_API ostream : public noncopyable
    {
      public:
        ostream(const path& p, mode_t m = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);

        ~ostream();

        void write(const char* buffer, size_t sz) const;

      private:
        stream_impl* strm_;
    };

    MTAR_COMMON_API
    //!
    //!
    //!
    void stream_copy(const ostream& os, const istream& is, size_t s);

}//namespace mtar

#endif//MTAR_COMMON_STREAM_HPP
