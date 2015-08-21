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

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif//defined(_WIN32)

#include <mtarcommon/allocator.hpp>
#include <mtarcommon/stream.hpp>

#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#if defined(_WIN32)
#include <io.h>
#endif//defined(_WIN32)


namespace mtar {

    class stream_impl
    {
      public:
#if defined(_WIN32)
        stream_impl(const wchar_t* fname, int openflag, mode_t mode)
          : handle_()
        {
            mode_t m = 0;
            m |= (mode & (S_IRUSR | S_IRGRP | S_IROTH)) ? _S_IREAD : 0;
            m |= (mode & (S_IWUSR | S_IWGRP | S_IWOTH)) ? _S_IWRITE: 0;
            _wsopen_s(&handle_, fname, openflag | O_BINARY, _SH_DENYNO, m);
        }
#else//UNIX
        stream_impl(const char* fname, int openflag, mode_t mode)
          : handle_()
        {
            handle_ = open(fname, flags, mode);
        }
#endif//defined(_WIN32)

        int handle() const { return handle_; }

        bool isopen() const { return handle_ < 0; }

#if defined(_WIN32)
        size_t read(char* const buff, const size_t sz)
        {
            return (size_t)_read(handle(), buff, static_cast<int>(sz));
        }

        void write(const char* const buff, size_t sz)
        {
            _write(handle(), buff, (size_t)sz);
        }
#else//UNIX
        size_t read(char* const buff, const size_t sz)
        {
            return (size_t)read(handle(), buff, sz);
        }

        void write(const char* const buff, size_t sz)
        {
            write(handle(), buff, (size_t)sz);
        }
#endif//defined(_WIN32)

        const char* errmsg() const
        {
            int err = errno;
            errno = 0;
            return strerror(err);
        }

        ~stream_impl()
        {
            const int h = handle();
            if (h >= 0) {
                _close(h);
            }
        }

      private:
        int handle_;
    };

    static mtar::allocator<stream_impl> ALLOC;

    istream::istream(const path& p)
      : strm_(ALLOC.allocate(1))
    {
        new (strm_) stream_impl(p.c_str(), O_RDONLY, S_IRUSR);
    }

    istream::~istream()
    {
        strm_->~stream_impl();
        ALLOC.deallocate(strm_, 1);
    }

    size_t istream::read(char* buffer, size_t sz) const
    {
        return strm_->read(buffer, sz);
    }

    ostream::ostream(const path& p, mode_t m)
      : strm_(ALLOC.allocate(1))
    {
        new (strm_) stream_impl(p.c_str(), O_CREAT | O_WRONLY, m);
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
