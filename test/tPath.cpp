
/**
 * \file tPath.cpp
 * \date 2015
 */

#include "unittest.hpp"

#include <mtarcommon/path.hpp>

CPP_TEST( createPath )
{
    {// from c string
        mtar::path p = mtar::create_path(PATH_LIT("."));
        TEST_TRUE( p.size() == 1 );
        TEST_TRUE( p[0] == '.' );
    }
    {// from string
        mtar::path p = mtar::create_path(std::string("."));
        TEST_TRUE( p.size() == 1 );
        TEST_TRUE( p[0] == '.' );
    }
    {// from wstring
        mtar::path p = mtar::create_path(std::wstring(L"."));
        TEST_TRUE( p.size() == 1 );
        TEST_TRUE( p[0] == '.' );
    }
    {// from mtar string
        mtar::path p = mtar::create_path(mtar::string("."));
        TEST_TRUE( p.size() == 1 );
        TEST_TRUE( p[0] == '.' );
    }
    {// from mtar wstring
        mtar::path p = mtar::create_path(mtar::wstring(L"."));
        TEST_TRUE( p.size() == 1 );
        TEST_TRUE( p[0] == '.' );
    }
}

const char NAME_UTF8[] = {
    0xEF, 0xBC, 0xAE, 0xEF, 0xBD, 0x81, 0xEF, 0xBD, 0x8D, 0xEF, 0xBD, 0x85, 0xE3, 0x80, 0x80, 0xEF, 0xBD, 
    0x97, 0xEF, 0xBD, 0x89, 0xEF, 0xBD, 0x94, 0xEF, 0xBD, 0x88, 0xE3, 0x80, 0x80, 0xEF, 0xBD, 0x93, 0xEF,
    0xBD, 0x90, 0xEF, 0xBD, 0x81, 0xEF, 0xBD, 0x83, 0xEF, 0xBD, 0x85, 0xEF, 0xBD, 0x93, 0x00 };

const int EXP_NAME_UTF32[] = {
    0xFF2E, 0xFF41, 0xFF4D, 0xFF45, 0x3000, 0xFF57, 0xFF49, 0xFF54, 0xFF48, 0x3000, 0xFF53, 0xFF50, 0xFF41,
    0xFF43, 0xFF45, 0xFF53, 0x0000 };

CPP_TEST( utf8Name )
{
    {// utf8 path
        mtar::string path(NAME_UTF8);
        mtar::path p(mtar::create_path(path));
        mtar::wstring wstr(mtar::to_wstring(p));
        TEST_TRUE( wstr.size() == 16 );
        for (size_t i = 0; i != 16; ++i) 
        {
            TEST_TRUE( wstr[i] == EXP_NAME_UTF32[i] );
        }
    }
}
