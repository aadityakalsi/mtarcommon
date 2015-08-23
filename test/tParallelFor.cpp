
/**
 * \file tParallelFor.cpp
 * \date 2015
 */

#include "unittest.hpp"

#include <mtarcommon/atomic.hpp>
#include <mtarcommon/thread.hpp>
#include <mtarcommon/parallel_for.hpp>

#include <vector>
#include <iostream>

static const size_t NUMEL = 8000000;
static const int EL = 1;
static const std::vector<int> NUMS(NUMEL, EL);

mtar::atomic_int SUM;

void storeSum(size_t st, size_t end)
{
    int sum = 0;
    const int* num = &NUMS[0] + st;
    const int* const numend = &NUMS[0] + end;
    for (; num != numend;) { sum += *(num++); }
    SUM.add(sum);
}

CPP_TEST( serialMean )
{
    {// serial
        SUM.store(0);
        TEST_TRUE( SUM.load() == 0 );
        storeSum(0, NUMS.size());
        TEST_TRUE( SUM.load() == NUMEL*EL );
    }
}

CPP_TEST( parallelMean )
{
    {// parallel
        SUM.store(0);
        TEST_TRUE( SUM.load() == 0 );
        mtar::parallel_for(0, NUMS.size(), storeSum);
        TEST_TRUE( SUM.load() == NUMEL*EL );
    }
}
