#include "stdafx.h"
#include "LeetCodeTest.h"

//https://leetcode.cn/problems/lru-cache/
class LRUCache
{
public:

    struct DNode
    {
        int key;
        int value;
        std::shared_ptr<DNode> pre{ nullptr };
        std::shared_ptr<DNode> suf{ nullptr };
    };
    using NodePtr = std::shared_ptr<DNode>;

    LRUCache(int capacity) :max_num_(capacity)
    {
        head_ = std::make_shared<DNode>();
        tail_ = std::make_shared<DNode>();
        head_->suf = tail_;
        tail_->pre = head_;
    }

    int get(int key)
    {
        auto& node = nums_[key];
        if (!node)return -1;

        moveToHead(node);

        return node->value;
    }

    void put(int key, int value)
    {
        auto& node = nums_[key];
        if (nullptr != node)
        {
            node->value = value;
            moveToHead(node);
            return;
        }

        node = std::make_shared<DNode>();
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
    std::map<int, NodePtr> nums_;

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

void LeetCodeTest::StartTest()
{
    LRUCache lru(1);
    lru.put(2, 1);
    lru.get(2);
}
