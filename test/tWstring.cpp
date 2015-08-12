
/**
 * \file tWstring.cpp
 * \date 2015
 */

#include <mtarcommon/path.hpp>

#include "unittest.hpp"

CPP_TEST( wstring )
{
    {
        mtar::wstring str;
        TEST_TRUE(str.size() == 0);
    }
    {
        static const size_t LOOP = 100;
        mtar::wstring str;
        str.resize(LOOP, 'A');
        TEST_TRUE(str.size() == LOOP);
        for (size_t i = 0; i != LOOP; ++i) {
            TEST_TRUE(str[i] == 'A');
        }
    }
    {
        static const size_t LOOP = 100000;
        for (size_t i = 0; i != LOOP; ++i) {
            mtar::wstring str(1, 'A');
        }
    }
}

CPP_TEST(std_wstring)
{
    {
        std::wstring str;
        TEST_TRUE(str.size() == 0);
    }
    {
        static const size_t LOOP = 100;
        std::wstring str;
        str.resize(LOOP, 'A');
        TEST_TRUE(str.size() == LOOP);
        for (size_t i = 0; i != LOOP; ++i) {
            TEST_TRUE(str[i] == 'A');
        }
    }
    {
        static const size_t LOOP = 100000;
        for (size_t i = 0; i != LOOP; ++i) {
            std::wstring str(1, 'A');
        }
    }
}
