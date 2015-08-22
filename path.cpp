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
 * \file path.cpp
 * \date 2015
 */

#include <mtarcommon/path.hpp>
#include "utf8/unchecked.h"

#include <algorithm>

namespace mtar {

    //!
    //!
    //!
    void fill_path(path& p, const wstring& wstr)
    {
#if defined(_WIN32)
        p.resize(wstr.size());
        std::copy(wstr.begin(), wstr.end(), &p[0]);
#else
        p.resize(wstr.size() * 4 + 1);
        auto itend = utf8::unchecked::utf32to8(wstr.begin(), wstr.end(), &p[0]);
        *itend = '\0';
        p.resize(itend - &p[0]);
#endif
    }

    //!
    //!
    //!
    void fill_path(path& p, const std::wstring& wstr)
    {
#if defined(_WIN32)
        p.resize(wstr.size());
        std::copy(wstr.begin(), wstr.end(), &p[0]);
#else
        p.resize(wstr.size() * 4 + 1);
        auto itend = utf8::unchecked::utf32to8(wstr.begin(), wstr.end(), &p[0]);
        *itend = '\0';
        p.resize(itend - &p[0]);
#endif
    }

    //!
    //!
    //!
    void fill_path(path& p, const string& str)
    {
#if defined(_WIN32)
        p.resize(str.size() + 1);
        wchar_t* itend = utf8::unchecked::utf8to16(str.begin(), str.end(), &p[0]);
        *itend = '\0';
        p.resize(itend - &p[0]);
#else
        p.resize(str.size());
        std::copy(str.begin(), str.end(), &p[0]);
#endif
    }

    //!
    //!
    //!
    void fill_path(path& p, const std::string& str)
    {
#if defined(_WIN32)
        p.resize(str.size() + 1);
        wchar_t* itend = utf8::unchecked::utf8to16(str.begin(), str.end(), &p[0]);
        *itend = '\0';
        p.resize(itend - &p[0]);
#else
        p.resize(str.size());
        std::copy(str.begin(), str.end(), &p[0]);
#endif
    }

    //!
    //!
    //!
    wstring to_wstring(const path& p)
    {
#if defined(_WIN32)
        return p;
#else
        wstring wstr;
        wstr.resize(p.size() + 1);
        auto itend = utf8::unchecked::utf8to32(p.begin(), p.end(), &wstr[0]);
        *itend = '\0';
        wstr.resize(itend - &wstr[0]);
        return wstr;
#endif
    }

    //!
    //!
    //!
    string to_string(const path& p)
    {
#if !defined(_WIN32)
        return p;
#else
        string str;
        str.resize(p.size() + 1);
        char* itend = utf8::unchecked::utf16to8(p.begin(), p.end(), &str[0]);
        *itend = '\0';
        str.resize(itend - &str[0]);
        return str;
#endif
    }

}//namespace mtar
