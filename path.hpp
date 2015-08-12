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

#include <limits>
#include <memory>
#include <string>

namespace mtar {

    namespace detail {

        MTAR_COMMON_API
        void* allocate_pvt(size_t n);

        MTAR_COMMON_API
        void deallocate_pvt(void* p, size_t n);

    }//namespace detail

    //! wchar_allocator for wstring
    template <typename T>
    class allocator : public std::allocator<T>
    {
      public:
        typedef T              value_type;
        typedef T*             pointer;
        typedef const T*       const_pointer;
        typedef T&             reference;
        typedef const T&       const_reference;
        typedef size_t         size_type;
        typedef std::ptrdiff_t difference_type;

        // rebind allocator to type U
        template <class U>
        struct rebind
        {
            typedef allocator<U> other;
        };

        // return address of values
        pointer address(reference value) const
        {
            return &value;
        }
        const_pointer address(const_reference value) const
        {
            return &value;
        }

        /* constructors and destructor
        * - nothing to do because the allocator has no state
        */
        allocator() throw()
        { }

        allocator(const allocator&) throw()
        { }

        template <typename U>
        allocator(const allocator<U>&) throw()
        { }

        ~allocator() throw()
        { }

        // return maximum number of elements that can be allocated
        size_type max_size() const throw()
        {
            return static_cast<size_t>(-1) / sizeof(T);
        }

        // allocate but don't initialize num elements of type T
        pointer allocate(size_type num, const void* = 0)
        {
            return static_cast<pointer>(detail::allocate_pvt(num*sizeof(T)));
        }

        // deallocate storage p of deleted elements
        void deallocate(pointer p, size_type num)
        {
            return detail::deallocate_pvt((void*)p, num*sizeof(T));
        }

        // initialize elements of allocated storage p with value value
        void construct(pointer p, const T& value)
        {
            new (p) T(value);
        }

        // destroy elements of initialized storage p
        void destroy(pointer p)
        {
            p->~T();
        }
    };

    // return that all specializations of this allocator are interchangeable
    template <typename T1, typename T2>
    bool operator== (const allocator<T1>&, const allocator<T2>&) throw()
    {
        return true;
    }

    template <typename T1, typename T2>
    bool operator!= (const allocator<T1>&, const allocator<T2>&) throw()
    {
        return false;
    }

    //! wstring typedef
    typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>> wstring;

}//namespace mtar

#endif//MTAR_COMMON_PATH_HPP
