#include "stdafx.h"
#include "Sort.hpp"
#include "SortTest.h"
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <map>
#include <deque>
#include <queue>
#include <forward_list>
#include <algorithm>

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
        rand_A(value_list, 1000); \
        SortFunc(value_list,std::greater<int32_t>()); \
        LOG(INFO)<<str<<":";\
        OutputList(value_list); \
    }\

void SortTest::StartTest()
{
    std::vector<int32_t> value_list;

    //TestSort(Bubble, "Bubble");
    //TestSort(Selection, "Selection");
    //TestSort(Insertion, "Insertion");
    TestSort(Sort::Quick, "Quick");
    TestSort(Sort::Shell, "Shell");
    TestSort(Sort::MergeSort, "MergeSort");
    TestSort(Sort::HeapSink, "HeapSink");
    //TestSort(HeapUpward, "HeapUpward");

    //std::priority_queue<int32_t> p;
    //p.pop();

    //std::list<int32_t>  l;
    //l.push_back(1); 
    //
    //std::unordered_map<int32_t, int32_t> s;
    //s[1] = 1;

    //std::forward_list<int32_t>;
    //std::stack<int32_t>;
    //std::queue<int32_t>sss;
    //std::vector<int32_t>sssssss;
    //std::sort(sssssss.begin(), sssssss.end());
    //std::stable_sort(sssssss.begin(), sssssss.end());

    //std::inplace_merge(sssssss.begin(), sssssss.begin() + 1, sssssss.end());

}
