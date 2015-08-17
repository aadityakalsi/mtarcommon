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
 * \file path.hpp
 * \date 2015
 */

#ifndef MTAR_COMMON_PATH_HPP
#define MTAR_COMMON_PATH_HPP

#include <mtarcommon/defs.hpp>
#include <mtarcommon/string.hpp>

namespace mtar {

#if defined(_WIN32)
    
    typedef wstring path;
    typedef wchar_t char_type;
    #define PATH_LIT(x) L##x

#else//UNIX

    typedef string  path;
    typedef char char_type;
    #define PATH_LIT(x) u8##x

#endif//defined(_WIN32)
    
    MTAR_COMMON_API
    //!
    //!
    //!
    void fill_path(path& p, const wstring& wstr);

    MTAR_COMMON_API
    //!
    //!
    //!
    void fill_path(path& p, const std::wstring& wstr);

    MTAR_COMMON_API
    //!
    //!
    //!
    void fill_path(path& p, const string& str);

    MTAR_COMMON_API
    //!
    //!
    //!
    void fill_path(path& p, const std::string& str);

    template <typename T>
    //!
    //!
    //!
    path create_path(const T& str)
    {
        path p;
        fill_path(p, str);
        return p;
    }

    //!
    //!
    //!
    path create_path(const char_type* cstr)
    {
        path p(cstr);
        return p;
    }

    MTAR_COMMON_API
    //!
    //!
    //!
    wstring to_wstring(const path& p);

    MTAR_COMMON_API
    //!
    //!
    //!
    string to_string(const path& p);

}//namespace mtar

#endif//MTAR_COMMON_PATH_HPP
