#include "stdafx.h"
#include "DesignMgr.h"
#include <any>
int main()
{
    auto desmgr = std::make_shared<CDesignMgr>();
    desmgr->Init();
    desmgr->Test();
    system("pause");
    std::map<int, int>().lower_bound(1);
    using dc = std::decay_t<const int>;
    std::any a = 1;
}

void quick(std::vector<int>& v, int l, int r)
{
    if (r >= l)return;

    auto b = l;
    auto e = r;
    const auto& k = v[l];
    while (b > e)
    {
        while (e > b && v[e] >= k)--e;
        while (b < e && v[b] <= k)++b;
        std::swap(v[b], v[e]);
    }
    std::swap(v[l], v[b]);
    quick(v, l, b - 1);
    quick(v, b + 1, r);
}

void insert(std::vector<int>& v)
{
    for (auto i = 1; i < v.size(); ++i)
    {
        for (auto j = i; j>0 && v[j] < v[j-1]; --j)
        {
            std::swap(v[j], v[j - 1]);
        }
    }
}

void dubble(std::vector<int>& v)
{
    if (v.empty())return;
    bool swapped(true);
    for (auto i = 0; i < v.size() - 1; ++i)
    {
        swapped = false;
        for (auto j = v.size() - 1; j > i; --j)
        {
            if (v[j] < v[j - 1])
            {
                std::swap(v[j], v[j - 1]);
                swapped = true;
            }
        }
    }
}

void select(std::vector<int>& v)
{
    if (v.empty())return;
    for (auto i = 0; i < v.size() - 1; ++i)
    {
        auto index = i;
        for (auto j = i + 1; j < v.size(); ++j)
        {
            if (v[j] < v[index])
            {
                index = j;
            }
        }
        std::swap(v[index], v[i]);
    }
}