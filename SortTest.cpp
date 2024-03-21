#include "stdafx.h"
#include "Sort.hpp"
#include "SortTest.h"

void rand_A(std::vector<int32_t>& A, int32_t count)
{
    A.clear();
    srand((unsigned)time(NULL)); //提供随机种子即随机数开始数字
    for (int i = 0; i < count; ++i)
    {
        A.push_back(rand()); //默认rand()范围0~65536不包括边界即1—2^16-1
    }
}

template <class List>
void OutputList(const List& list)
{
    auto logger = LOG(INFO);
    for (const auto& item : list)
    {
        logger << item << ",";
    }
}

#define TestSort(SortFunc, str)\
    {\
        rand_A(value_list, 20); \
        Sort::SortFunc(value_list); \
        LOG(INFO)<<str<<":";\
        OutputList(value_list); \
    }\

void SortTest::StartTest()
{
    std::vector<int32_t> value_list;

    TestSort(Bubble, "Bubble");
    TestSort(Selection, "Selection");
    TestSort(Insertion, "Insertion");
    TestSort(Quick, "Quick");
    TestSort(Shell, "Shell");
    TestSort(MergeSort, "MergeSort");
}
