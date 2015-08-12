
/**
 * \file tWstring.cpp
 * \date 2015
 */

#include <mtarcommon/path.hpp>

#include <vector>

#include "unittest.hpp"

CPP_TEST( noOp )
{ }


#define TEST_ALLOC(vecAlloc, stringAlloc) \
    typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, stringAlloc<wchar_t>> string_t; \
    typedef std::vector<string_t, vecAlloc<string_t>> vector_t; \
    { \
        string_t str; \
        TEST_TRUE(str.size() == 0); \
    } \
    { \
        static const size_t LOOP = 1000; \
        string_t str; \
        for (size_t i = 0; i != LOOP; ++i) { \
            str.push_back('A'); \
            TEST_TRUE(str[i] == 'A'); \
        } \
    } \
    { \
        static const size_t LOOP = 100000; \
        vector_t vec; \
        for (size_t i = 0; i != LOOP; ++i) { \
            vec.emplace_back(1, 'A'); \
        } \
    }

CPP_TEST( wstring_vec_vec )
{
    TEST_ALLOC(mtar::vector_allocator, mtar::vector_allocator)
}

CPP_TEST( wstring_vec_scl )
{
    TEST_ALLOC(mtar::vector_allocator, mtar::scalar_allocator)
}

CPP_TEST( wstring_scl_vec )
{
    TEST_ALLOC(mtar::scalar_allocator, mtar::vector_allocator)
}

CPP_TEST( wstring_scl_scl )
{
    TEST_ALLOC(mtar::scalar_allocator, mtar::scalar_allocator)
}

CPP_TEST( wstring_vec_std )
{
    TEST_ALLOC(mtar::vector_allocator, std::allocator)
}

CPP_TEST( wstring_std_vec )
{
    TEST_ALLOC(std::allocator, mtar::vector_allocator)
}

CPP_TEST( wstring_scl_std )
{
    TEST_ALLOC(mtar::scalar_allocator, std::allocator)
}

CPP_TEST( wstring_std_scl )
{
    TEST_ALLOC(std::allocator, mtar::scalar_allocator)
}

CPP_TEST( std_cont )
{
    TEST_ALLOC(std::allocator, std::allocator)
}
