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

#include <mtarcommon/stream.hpp>

#include <algorithm>
#include <fstream>

namespace mtar {

    class stream_impl : public std::fstream
    {
      public:
        stream_impl(const std::string& name, std::ios_base::openmode mode)
          : std::fstream(name, mode)
        { }
    };


    istream::istream(const path& p)
      : strm_(new stream_impl(to_string(p).c_str(), std::ios_base::in | std::ios_base::binary))
    { }

    istream::~istream()
    { delete strm_; }

    size_t istream::read(char* buffer, size_t sz) const
    {
        return strm_->read(buffer, sz).gcount();
    }

    ostream::ostream(const path& p)
      : strm_(new stream_impl(mtar::to_string(p).c_str(), std::ios_base::out | std::ios_base::binary))
    { }

    ostream::~ostream()
    { delete strm_; }

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
