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

#include <boost/pool/pool_alloc.hpp>

namespace mtar {
namespace detail {

    class mtar_allocator_vec
      : public boost::fast_pool_allocator<char, boost::default_user_allocator_malloc_free, boost::details::pool::null_mutex>
    { };


    mtar_allocator_vec* get_pvt_vec()
    {
        static detail::mtar_allocator_vec ALLOC;
        return &ALLOC;
    }

    void* allocate_pvt_vec(size_t num)
    {
        return (void*)get_pvt_vec()->allocate(num);
    }

    void deallocate_pvt_vec(void* p, size_t num)
    {
        return get_pvt_vec()->deallocate((char*)p, num);
    }

    class mtar_allocator_scl
      : public boost::fast_pool_allocator<char, boost::default_user_allocator_malloc_free, boost::details::pool::null_mutex>
    { };


    mtar_allocator_scl* get_pvt_scl()
    {
        static detail::mtar_allocator_scl ALLOC;
        return &ALLOC;
    }

    void* allocate_pvt_scl(size_t num)
    {
        return (void*)get_pvt_scl()->allocate(num);
    }

    void deallocate_pvt_scl(void* p, size_t num)
    {
        return get_pvt_scl()->deallocate((char*)p, num);
    }

}//namespace detail
}//namespace mtar
