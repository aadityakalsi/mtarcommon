/**
 * \file tString.cpp
 * \date 2015
 */

#include <mtarcommon/string.hpp>

#include <vector>

#if defined(_MSC_VER) && _MSC_VER >= 1800 // VS2013 with C++11 support
#define THREAD_SUPPORT 1
#include <thread>
#else

typedef void(*func)(void);

DWORD WINAPI RunFunction(LPVOID f)
{
    func pf = (func)f;
    pf();
    return 0;
}

namespace std {

    class thread
    {
        HANDLE thread_;
      public:
        thread(func f)
          : thread_()
        {
            thread_ = CreateThread(NULL, 0, RunFunction, f, 0, NULL);
        }

        void join() const
        {
            WaitForSingleObject(thread_, INFINITE);
        }

        ~thread() { CloseHandle(thread_); }
    };
}
#endif

#include "unittest.hpp"

CPP_TEST( noOp )
{
    // warm up allocator
    std::string s1;
    mtar::string s2;
}

#define TEST_ALLOC_GROW(ns, type) \
    typedef ns::type string_t; \
    { \
        static const size_t LOOP = 5002; \
        string_t str; \
        str.reserve(LOOP); \
        for (size_t i = 0; i != LOOP; ++i) { \
            str.push_back('A'); \
            TEST_TRUE(str[i] == 'A'); \
        } \
    }

#define TEST_ALLOC_VEC_GROW(ns, type) \
    typedef ns::type string_t; \
    { \
        static const size_t LOOP = 100002; \
        string_t STR(100, 'A'); \
        std::vector<string_t> vec; \
        for (size_t i = 0; i != LOOP; ++i) { \
            vec.push_back(STR); \
        } \
    }

#define TEST_CREATE_DESTROY(ns, type) \
    typedef ns::type string_t; \
    { \
        static const size_t LOOP = 5000; \
        for (size_t i = 0; i != LOOP; ++i) { \
            TEST_TRUE( !string_t(10, 'A').empty() ); \
        } \
    }

CPP_TEST( std_string )
{
    TEST_ALLOC_GROW(std, string)
}

CPP_TEST( mtar_string )
{
    TEST_ALLOC_GROW(mtar, string)
}

CPP_TEST( std_wstring )
{
    TEST_ALLOC_GROW(std, wstring)
}

CPP_TEST( mtar_wstring )
{
    TEST_ALLOC_GROW(mtar, wstring)
}

CPP_TEST( std_vec_string )
{
    TEST_ALLOC_VEC_GROW(std, string)
}

CPP_TEST( mtar_vec_string )
{
    TEST_ALLOC_VEC_GROW(mtar, string)
}

CPP_TEST( std_vec_wstring )
{
    TEST_ALLOC_VEC_GROW(std, wstring)
}

CPP_TEST( mtar_vec_wstring )
{
    TEST_ALLOC_VEC_GROW(mtar, wstring)
}

CPP_TEST( std_string_create_destroy )
{
    TEST_CREATE_DESTROY(std, string)
}

CPP_TEST( mtar_string_create_destroy )
{
    TEST_CREATE_DESTROY(mtar, string)
}

CPP_TEST( std_wstring_create_destroy )
{
    TEST_CREATE_DESTROY(std, wstring)
}

CPP_TEST( mtar_wstring_create_destroy )
{
    TEST_CREATE_DESTROY(mtar, wstring)
}

////////////////////////////

void test_vec_grow_std_string()
{
    TEST_ALLOC_VEC_GROW(std, string)
}

void test_vec_grow_mtar_string()
{
    TEST_ALLOC_VEC_GROW(mtar, string)
}

CPP_TEST( std_string_thread )
{
    std::thread t1(test_vec_grow_std_string);
    std::thread t2(test_vec_grow_std_string);
    t1.join();
    t2.join();
}

CPP_TEST( mtar_string_thread )
{
    std::thread t1(test_vec_grow_mtar_string);
    std::thread t2(test_vec_grow_mtar_string);
    t1.join();
    t2.join();
}
