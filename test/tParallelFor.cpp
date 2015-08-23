
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
#include <cstdlib>
#include <chrono>

static const size_t NUMEL = 500000;
static const double EL = 2;
static const std::vector<double> NUMS(NUMEL, EL);
static const double* NUMSPTR = &NUMS[0];

mtar::atomic_int SUM;

void storeSum(size_t st, size_t end)
{
    double sum = 0;
    const double* num = NUMSPTR + st;
    const double* const numend = NUMSPTR + end;
    for (; num != numend;) { sum += *(num++); }
    SUM.add(static_cast<int>(sum));
}

#if defined(_WIN32)

    class timer
    { };

#else

    class timer
    {
        decltype(std::chrono::high_resolution_clock::now()) beg_;
      public:
        timer()
          : beg_(std::chrono::high_resolution_clock::now())
        { }

        ~timer()
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto us = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - beg_).count();
            std::cout << "Test took:" << us/1e6 << " sec." << std::endl;
        }
    };

#endif

CPP_TEST( serialMean )
{
    timer T;
    (void)T;
    {// serial
        SUM.store(0);
        TEST_TRUE( SUM.load() == 0 );
        storeSum(0, NUMS.size());
        TEST_TRUE( SUM.load() == NUMEL*EL );
    }
}

CPP_TEST( parallelMean )
{
    timer T;
    (void)T;
    {// parallel
        SUM.store(0);
        TEST_TRUE( SUM.load() == 0 );
        mtar::parallel_for(0, NUMS.size(), storeSum);
        TEST_TRUE( SUM.load() == NUMEL*EL );
    }
}
