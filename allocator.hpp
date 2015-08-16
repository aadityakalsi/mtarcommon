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
 * \file allocator.hpp
 * \date 2015
 */

#ifndef MTAR_COMMON_ALLOCATOR_HPP
#define MTAR_COMMON_ALLOCATOR_HPP

#include <mtarcommon/defs.hpp>

#include <memory>
#include <list>
#include <atomic>

namespace mtar {

    class storage
    {
        static const size_t SEGMENT_SIZE = 1024 * 1024;

        std::list<char*>   segments_;
        size_t             idx_;
        std::atomic<int>   in_use_;
      public:
        storage() NOEXCEPT
          : segments_()
          , idx_(SEGMENT_SIZE)
          , in_use_(0)
        { }

        MTAR_COMMON_INLINE
        char* get(size_t s) NOEXCEPT
        {
            char* buf;
            size_t sz = ((s + 7)/8)*8; // align to 8 bytes

            int curstate = 0;
            while (!atomic_compare_exchange_weak(&in_use_, &curstate, 1)) { }
            // CRIT begin            

            if (sz + idx_ <= SEGMENT_SIZE) {
                buf = segments_.back() + idx_;
                idx_ += sz;
            } else {
                buf = new (std::nothrow) char[(sz > SEGMENT_SIZE) ? sz : SEGMENT_SIZE];
                if (buf) {
                    segments_.push_back(buf);
                    idx_ = sz;
                }
            }

            // CRIT end
            in_use_.store(0);

            return buf;
        }

        MTAR_COMMON_INLINE
        void put(void* ptr, size_t s) NOEXCEPT
        {
            char* p = (char*)ptr;
            s = ((s+7)/8) * 8;

            int curstate = 0;
            while (!atomic_compare_exchange_weak(&in_use_, &curstate, 1)) { }
            // CRIT begin
            
            if (p == (segments_.back() + idx_ - s)) {
                idx_ -= s;
            }
            
            // CRIT end
            in_use_.store(0);
        }

        ~storage() NOEXCEPT
        {
            for (auto seg : segments_) {
                delete [] seg;
            }
        }

        static MTAR_COMMON_API storage* get_storage();
    };

    //! allocator
    template <typename T>
    class allocator
    {
      private:
        storage* storage_;

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
        allocator() NOEXCEPT
          : storage_(storage::get_storage())
        { }

        allocator(const allocator& a) NOEXCEPT
          : storage_(a.storage_)
        { }

        template <typename U>
        allocator(const allocator<U>& a) NOEXCEPT
          : storage_(a.storage_)
        { }

        ~allocator() NOEXCEPT
        { }

        // return maximum number of elements that can be allocated
        size_type max_size() const NOEXCEPT
        {
            return static_cast<size_t>(-1) / sizeof(T);
        }

        // allocate but don't initialize num elements of type T
        MTAR_COMMON_INLINE
        pointer allocate(size_type num, const void* hint = 0)
        {
            pointer p = (pointer)(void*)storage_->get(num*sizeof(T));
            if (!p) { throw std::bad_alloc(); }
            return p;
        }

        // deallocate storage p of deleted elements
        MTAR_COMMON_INLINE
        void deallocate(pointer p, size_type num)
        {
            storage_->put((void*)p, num*sizeof(T));
        }

        // initialize elements of allocated storage p with value value
        MTAR_COMMON_INLINE
        void construct(pointer p, const T& value)
        {
            new (p) T(value);
        }

        // destroy elements of initialized storage p
        MTAR_COMMON_INLINE
        void destroy(pointer p)
        {
            p->~T();
        }

        // return that all specializations of this allocator are interchangeable
        template <typename T1>
        bool operator== (const mtar::allocator<T1>& rhs) const NOEXCEPT
        {
            return storage_ == rhs.storage_;
        }

        // return that all specializations of this allocator are interchangeable
        template <typename T1>
        bool operator!= (const mtar::allocator<T1>& rhs) const NOEXCEPT
        {
            return storage_ != rhs.storage_;
        }
    };

}//namespace mtar

#endif//MTAR_COMMON_ALLOCATOR_HPP
