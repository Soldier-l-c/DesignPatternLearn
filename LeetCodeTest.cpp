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


template <class Key, class Value>
class LFUCache
{
public:

    template <typename KEY, typename VALUE>
    struct DuNode
    {
        KEY key;
        VALUE value;

        using ThisNodePtr = std::shared_ptr<DuNode<KEY, VALUE>>;

        ThisNodePtr pre{ nullptr };
        ThisNodePtr suf{ nullptr };
        int reference{ 0 };
    };

    //双向链表
    template <class KEY, class VALUE>
    class DuLinkList
    {
    public:
        using Node = DuNode<KEY, VALUE>;
        using NodePtr = typename Node::ThisNodePtr;

        DuLinkList()
        {
            head_ = std::make_shared<DuNode<KEY, VALUE>>();
            tail_ = std::make_shared<DuNode<KEY, VALUE>>();
            head_->suf = tail_;
            tail_->pre = head_;
        };

        ~DuLinkList()
        {
            //主动析构，防止循环引用无法析构
            while (head_->suf != tail_)
            {
                RemoveLast();
            }

            head_->suf = nullptr;
            tail_->pre = nullptr;
        }

    public:
        void MoveToHead(NodePtr& node)
        {
            RemoveNode(node);
            AddNode(node);
        }

        void AddNode(NodePtr& node)
        {
            auto suf = head_->suf;
            node->suf = suf;
            suf->pre = node;

            head_->suf = node;
            node->pre = head_;
        }

        void RemoveNode(NodePtr& node)
        {
            auto pre = node->pre;
            auto suf = node->suf;
            pre->suf = suf;
            suf->pre = pre;
        }

        NodePtr RemoveLast()
        {
            auto node = tail_->pre;
            if (node == head_)return nullptr;

            RemoveNode(node);

            return node;
        }

        bool Empty()
        {
            return head_->suf == tail_;
        }

    private:
        NodePtr head_;
        NodePtr tail_;
    };

    //using Key = int;
    //using Value = int;

    using NodePtr = typename DuLinkList<Key, Value>::NodePtr;

    LFUCache(Key capacity)
    {
        max_size_ = capacity;
    }

    Value get(Key key)
    {
        auto iter = node_list_.find(key);
        if (iter == node_list_.end())return Value();

        auto& node = iter->second;
        RemoveFromTableList(node->reference++, node);
        AddToTableList(node->reference, node);

        return node->value;
    }

    void put(Key key, Value value)
    {
        NodePtr node;

        auto iter = node_list_.find(key);

        //务必插入前检查是否需要删除最少使用的节点
        if (cur_size_ == max_size_ && iter == node_list_.end())
        {
            //移除使用次数最少，最近未使用的节点
            auto node = RemoveLast(table_list_.begin()->first);
            --cur_size_;
            node_list_.erase(node->key);
        }

        if (iter == node_list_.end())
        {
            node = std::make_shared<DuLinkList<Key, Value>::Node>();
            node->key = key;
            node->value = value;
            node_list_.insert({ key, node });
            //新插入节点才需要增加大小
            ++cur_size_;
        }
        else
        {
            node = iter->second;
            node->value = value;
        }

        //从原有队列删除节点
        RemoveFromTableList(node->reference++, node);

        //添加到新队列
        AddToTableList(node->reference, node);
    }

private:
    void RemoveFromTableList(int reference, NodePtr& node)
    {
        if (reference != 0)
        {
            auto& cur_node_list = table_list_[reference];
            cur_node_list->RemoveNode(node);
            if (cur_node_list->Empty())
            {
                cur_node_list = nullptr;
                table_list_.erase(reference);
            }
        }
    }

    NodePtr RemoveLast(int reference)
    {
        auto& cur_node_list = table_list_[reference];
        auto node = cur_node_list->RemoveLast();
        if (cur_node_list->Empty())
        {
            cur_node_list = nullptr;
            table_list_.erase(reference);
        }

        return node;
    }

    void AddToTableList(int reference, NodePtr& node)
    {
        auto& new_node_list = table_list_[reference];

        if (!new_node_list)
            new_node_list = std::make_shared<DuLinkList<Key, Value>>();

        new_node_list->AddNode(node);
    }

private:
    using DuLinkListPtr = std::shared_ptr<DuLinkList<Key, Value>>;

    std::map<int, DuLinkListPtr>table_list_;
    std::unordered_map<Key, NodePtr>node_list_;

    int max_size_{ 0 };
    int cur_size_{ 0 };
};


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

    LFUCache<int32_t, std::string> lfu(2);
    lfu.put(1, "1sdsd");
    lfu.put(2, "2sdsd");
    lfu.get(1);
    lfu.put(3, "3sdsd");
    LOG(INFO) << "LFUCache get 1:[" << lfu.get(1) << "]";

    LOG(INFO) << "LFUCache get 2:[" << lfu.get(2) << "]";


}
