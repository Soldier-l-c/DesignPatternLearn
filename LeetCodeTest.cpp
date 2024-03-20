#include "stdafx.h"
#include "LeetCodeTest.h"

template <class KEY, class VALUE>
struct DNode
{
    KEY key;
    VALUE value;
    std::shared_ptr<DNode<KEY, VALUE>> pre{ nullptr };
    std::shared_ptr<DNode<KEY, VALUE>> suf{ nullptr };
};

//https://leetcode.cn/problems/lru-cache/
template <class KEY, class VALUE>
class LRUCache
{
public:

    using ThisNode = DNode<KEY, VALUE>;
    using NodePtr = std::shared_ptr<ThisNode>;

    LRUCache(int capacity) :max_num_(capacity)
    {
        head_ = std::make_shared<ThisNode>();
        tail_ = std::make_shared<ThisNode>();
        head_->suf = tail_;
        tail_->pre = head_;
    }

    VALUE get(const KEY& key, const VALUE& default_v = VALUE())
    {
        auto& node = nums_[key];
        if (!node)return default_v;

        moveToHead(node);

        return node->value;
    }

    void put(const KEY& key, const VALUE& value)
    {
        auto& node = nums_[key];
        if (nullptr != node)
        {
            node->value = value;
            moveToHead(node);
            return;
        }

        node = std::make_shared<ThisNode>();
        node->value = value;
        node->key = key;
        addNode(node);
        ++cur_size_;

        if (cur_size_ > max_num_)
        {
            auto last_node = removeLast();
            nums_[last_node->key] = nullptr;
            --cur_size_;
        }
    }

private:
    void moveToHead(NodePtr& node)
    {
        removeNode(node);
        addNode(node);
    }

    void addNode(NodePtr& node)
    {
        auto suf = head_->suf;
        node->suf = suf;
        suf->pre = node;

        head_->suf = node;
        node->pre = head_;
    }

    void removeNode(NodePtr& node)
    {
        auto pre = node->pre;
        auto suf = node->suf;
        pre->suf = suf;
        suf->pre = pre;
    }

    NodePtr removeLast()
    {
        auto node = tail_->pre;
        if (node == head_)return nullptr;

        removeNode(node);

        return node;
    }

private:
    int max_num_{ 0 };
    std::map<KEY, NodePtr> nums_;

    NodePtr head_;
    NodePtr tail_;

    int cur_size_{ 0 };
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
//lru-cache

template<class Value>
struct ListNode
{
    Value val;

    std::shared_ptr<ListNode<Value>> next;
    ListNode(const Value& v) :val(v), next(nullptr) {};
};

using IntNodePtr = std::shared_ptr<ListNode<int32_t>>;

template<class NodePtr>
NodePtr ReverseList(const NodePtr& head)
{
    auto cur_node = head;
    decltype(cur_node) pre_node = nullptr;

    while (cur_node)
    {
        auto next_node = cur_node->next;
        cur_node->next = pre_node;
        pre_node = cur_node;
        cur_node = next_node;
    }

    return pre_node;
}

void TestList()
{
    auto head = std::make_shared<ListNode<int32_t>>(10);
    auto temp = head;
    for(int32_t i = 9;i>=0;--i)
    {
        temp->next  = std::make_shared<ListNode<int32_t>>(i);
        temp = temp->next;
    }
    auto rev = ReverseList(head);

    auto&& logger = LOG(INFO);
    while (rev)
    {
        logger << " " << rev->val;
        rev = rev->next;
    }
}

class Solution 
{
public:

    template <class T>
    int Count(const T& t1, const T& t2)
    {
        auto res = 0;
        auto b1 = t1.begin();
        auto b2 = t2.begin();
        while (b1 != t1.end() && b2 != t2.end())
        {
            if (*b1 < *b2)
                ++b1;
            else if (*b1 > * b2)
                ++b2;
            else
            {
                ++res;
                ++b1;
                ++b2;
            }
        }

        return res;
    }

    void CoverToSet(std::vector<std::string>& words)
    {
        if (words.size() < 2)
            return;

        std::sort(words.begin(), words.end());
        auto low = words.begin();
        auto fast = low + 1;

        while (fast != words.end())
        {
            while (fast != words.end() && *fast == *low)++fast;
            if (fast - low > 1)
            {
                fast = words.erase(low, fast);
            }

            if (fast == words.end())
                break;

            low = fast;
            ++fast;
        }
    }

    int countWords(std::vector<std::string>& words1, std::vector<std::string>& words2) 
    {
        CoverToSet(words1);
        CoverToSet(words2);
        return Count(words1, words2);
    }

public:

    //using IntNodePtr = ListNode*;

    auto deleteDuplicates(IntNodePtr head)->decltype(head)
    {
        if (!head || !head->next)
            return head;

        decltype(head) pre{ nullptr }, ret{ nullptr };
        
        auto low = head;
        auto fast = low->next;

        while (low)
        {
            while (fast && fast->val == low->val)fast = fast->next;

            if (fast == low->next)
            {
                if (pre)
                {
                    pre->next = low;
                }
                pre = low;
                pre->next = nullptr;
            }

            ret = ret ? ret : pre;

            low = fast;
            fast = (fast ? fast->next : fast);
        }

        return ret;
    }

};

void ExpandPath(std::wstring& path_ex)
{
    wchar_t buffer[1024]{ 0 };
    if (!path_ex.empty() && path_ex[0] == L'%')
    {
        ExpandEnvironmentStrings(path_ex.c_str(), buffer, 1024);
        path_ex = buffer;
    }
}

#include<atlstr.h>
void LeetCodeTest::StartTest()
{
    //TestList();

    LRUCache<std::string, int32_t> l(10);
    l.put("1", 10);
    LOG(INFO)<<"LRUCache get 1:[" << l.get("1")<<"]";
    

    LRUCache<int32_t, int32_t> i(10);
    i.put(1, 10);
    LOG(INFO) << "LRUCache get 1:[" << i.get(1) << "]";
}
