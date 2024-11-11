#include "stdafx.h"
#include "LeetCodeTest.h"
#include <set>
#include <unordered_map>
#include <queue>
#include "TreeNode.h"
#include <helper/util_time.h>

template <typename KEY, typename VALUE>
struct DuNode
{
    KEY key;
    VALUE value;

    using ThisNodePtr = std::shared_ptr<DuNode<KEY, VALUE>>;
    //using ThisNodePtr = DuNode<KEY, VALUE>*;

    ThisNodePtr pre{ nullptr };
    ThisNodePtr suf{ nullptr };

    static ThisNodePtr CreateNode(const KEY& k, const VALUE& v)
    {
        //return new TreeNode<V>(v);
        return std::make_shared<DuNode<KEY, VALUE>>(k, v);
    }

    DuNode(const KEY& k, const VALUE& v) :key(k), value(v), pre(nullptr), suf(nullptr) {};

    int reference{ 0 };
};

template<class Value>
struct ListNode
{
    Value val;

    std::shared_ptr<ListNode<Value>> next;
    ListNode(const Value& v) :val(v), next(nullptr) {};
};

//https://leetcode.cn/problems/lru-cache/
template <class KEY, class VALUE>
class LRUCache
{
public:
    using NodePtr = typename DuNode<KEY, VALUE>::ThisNodePtr;

    LRUCache(int capacity) :max_num_(capacity)
    {
        head_ = DuNode<KEY, VALUE>::CreateNode(KEY(), VALUE());
        tail_ = DuNode<KEY, VALUE>::CreateNode(KEY(), VALUE());
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

        node = DuNode<KEY, VALUE>::CreateNode(key, value);
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

    //1.找出最小操作数，其实就是找到最多的不需要操作的数的个数。
    //2.要找出最多不需要操作的个数，首先需要排序去重，这样不需要操作的数据肯定都是在一个区间里的。
    //3.如何定义这样的区间：对于区间的data[end]- data[begin] < len时，这时候区间里的数字不需要操作，只操作区间外的数字即可，并且区间外的数字都需要操作。
    //4.使用滑动窗口找出所有满足3条件的区间，同时获取最小操作数。
    int minOperations(std::vector<int>& nums) 
    {
        std::set<int> sort_list;
        for (const auto i : nums)sort_list.insert(i);
        auto len = nums.size();
        auto res = len;
        auto iter_l = sort_list.begin(), iter_r = sort_list.begin();

        std::deque<int>window;

        while (iter_r != sort_list.end())
        {
            window.push_back(*(iter_r++));
            while (window.back() - window.front() >= len)
            {
                window.pop_front();
            }
            res = min(res, len - window.size());
        }
        return res;
    }

    //int Gcd
    int Gcd(int m, int n)
    {
        while (n != 0)
        {
            auto temp = m % n;
            m = n;
            n = temp;
        }
        return m;
    }

    void InitGcdList()
    {
        gcd_list_ = decltype(gcd_list_)(51, std::vector<int>());
        for (int i = 1; i <= 50; ++i)
        {
            for (int j = 1; j <= 50; ++j)
            {
                if (Gcd(i, j) == 1)
                {
                    gcd_list_[i].push_back(j);
                    gcd_list_[j].push_back(i);
                }
            }
        }
    }

    using NodeList = std::vector<std::vector<int>>;

    void DfsGetCoprimes(const NodeList& node_list, const std::vector<int>& nums, std::vector<int>& res,
        std::vector<int>&num_2node_index, std::vector<int>& deep, int root, int pre)
    {
        const auto& gcd = gcd_list_[nums[root]];
        for (const auto item : gcd)
        {
            const auto node_index = num_2node_index[item];
            if (node_index == -1)continue;
            if (res[root] == -1 || deep[res[root]] < deep[node_index])res[root] = node_index;
        }

        auto node = num_2node_index[nums[root]];
        num_2node_index[nums[root]] = root;

        const auto& childs = node_list[root];
        if (childs.empty())return;

        for (const auto child : childs)
        {
            if (child == pre)continue;
            deep[child] = deep[root] + 1;
            DfsGetCoprimes(node_list, nums, res, num_2node_index, deep, child, root);
        }
        num_2node_index[nums[root]] = node;
    }

    std::vector<int> getCoprimes(std::vector<int>& nums, NodeList& edges)
    {
        if (nums.empty())return {};
        NodeList node_list(nums.size(), std::vector<int>());

        std::vector<int>res(nums.size(), -1);

        for (const auto& item : edges)
        {
            if (item.size() != 2)continue;
            node_list[item.front()].push_back(item.back());
            node_list[item.back()].push_back(item.front());
        }

        InitGcdList();

        std::vector<int>num_2node_index(51, -1);
        std::vector<int> deep(nums.size(), 0);

        DfsGetCoprimes(node_list, nums, res, num_2node_index, deep,0, -1);

        return res;
    }

    /*
C++无相图dfs遍历。
分别让每个初始感染节点去感染，找出独立感染节点数（只有该节点能联通，其他感染节点联通的节点）最多的初始感染节点
    */

    struct NodeInfo
    {
        std::vector<int> malware_by;
        bool initial{ false };
        int reduce{ 0 };
    };

    void DfsSpread(const std::vector<std::vector<int>>& graph, std::vector<NodeInfo>& node_list, std::vector<bool>& visit, int cur_node, int start)
    {
        auto& node_info = node_list[cur_node];
        if (visit[cur_node] || (node_info.initial && start != cur_node))
        {
            return;
        }

        node_info.malware_by.push_back(start);
        visit[cur_node] = true;

        for (auto i = 0; i < graph.size(); ++i)
        {
            if (graph[cur_node][i] && i != cur_node)
            {
                DfsSpread(graph, node_list, visit, i, start);
            }
        }

        return;
    }

    int minMalwareSpread(std::vector<std::vector<int>>& graph, std::vector<int>& initial)
    {
        std::vector<NodeInfo> node_list(graph.size(), NodeInfo());

        int res{ (int)graph.size() }, max_reduce{ 0 };

        for (auto i : initial)
        {
            auto& node_info = node_list[i];
            node_info.initial = true;
        }

        for (auto i : initial)
        {
            std::vector<bool>visit(graph.size(), false);
            DfsSpread(graph, node_list, visit, i, i);
        }

        for (const auto& item : node_list)
        {
            if (item.malware_by.size() == 1)
            {
                auto& reduce = node_list[item.malware_by[0]].reduce;
                ++reduce;

                if (max_reduce <= reduce)
                {
                    res = ((max_reduce == reduce) && res < item.malware_by[0]) ? res : item.malware_by[0];
                    max_reduce = reduce;
                }
            }
        }

        return res;
    }

    std::vector<int> findOriginalArray(std::vector<int>& changed) 
    {
        const int changed_size = changed.size();
        const auto original_size = changed_size / 2;
        if (changed_size == 0 || changed_size % 2)return {};

        std::vector<int> original(original_size, 0);
        std::vector<bool> changed_list(changed_size, false);
        std::sort(changed.begin(), changed.end());

        auto low = changed.begin();
        auto fast = low + 1;
        auto index = 0;
        while (fast != changed.end())
        {
            while (low != fast && changed_list[low - changed.begin()])++low;
            if (low == fast && ++fast == changed.end())return {};

            fast = std::lower_bound(fast, changed.end(), (*low) * 2);
            if (fast == changed.end() || *fast != (*low) * 2)return {};

            changed_list[fast - changed.begin()] = true;

            original[index++] = *(low);
            low++; fast++;
        }

        if (index != original_size)return {};

        return original;
    }

    struct TreeNode {
        int val;
        TreeNode* left;
        TreeNode* right;
    };

    std::vector<std::vector<int>> zigzagLevelOrder(TreeNode* root) 
    {
        std::vector<std::vector<int>>res;
        if (!root)return res;

        std::stack<TreeNode*>nodes;
        nodes.push(root);

        bool rl{ false };
        while (!nodes.empty())
        {
            std::vector<int>temp;

            std::stack<TreeNode*>next_nodes;

            while (!nodes.empty())
            {
                auto node = nodes.top();
                nodes.pop();
                temp.push_back(node->val);
                if (rl)
                {
                    if(node->right)next_nodes.push(node->right);
                    if(node->left)next_nodes.push(node->left);
                }
                else
                {
                    if (node->left)next_nodes.push(node->left);
                    if (node->right)next_nodes.push(node->right);
                }
            }

            nodes = std::move(next_nodes);

            res.emplace_back(temp);
            rl = !rl;
        }
        return res;
    }

    int findValueOfPartition(std::vector<int>& nums) 
    {
        std::sort(nums.begin(), nums.end());

        auto res{ INT_MAX };
        for (int i = 1; i < nums.size(); ++i)
        {
            res = min(nums[i] - nums[i - 1], res);
        }
        return res;
    }

private:
    std::vector<std::vector<int>>gcd_list_;
};

using ListNodeInt = std::shared_ptr<ListNode<int>>;

ListNodeInt ReverseList(ListNodeInt head)
{
    decltype(head)pre = nullptr;

    while (head)
    {
        auto next = head->next;
        head->next = pre;
        pre = head;
        head = next;
    }
    return pre;
}

ListNodeInt reverseKGroup(ListNodeInt head, int k)
{
    auto length = 1;
    decltype(head) res_head = nullptr, last_head = head, last_tial = nullptr;

    while (head)
    {
        auto next = head->next;
        if ((length % k == 0))
        {
            head->next = nullptr;
            if (!res_head)
            {
                res_head = head;
            }
            if (!last_tial)
            {
                last_tial->next = head;
            }

            ReverseList(last_head);
            last_tial = last_head;
            last_tial->next = next;
            last_head = next;
        }

        head = next;
        ++length;
    }

    return res_head;
}


//双向链表
template <class KEY, class VALUE>
class DuLinkList
{
public:
    using Node = DuNode<KEY, VALUE>;
    using NodePtr = typename Node::ThisNodePtr;

    DuLinkList()
    {
        head_ = CreateNode(KEY(),VALUE());
        tail_ = CreateNode(KEY(), VALUE());
        head_->suf = tail_;
        tail_->pre = head_;
    };

    ~DuLinkList()
    {
        //主动析构，防止循环引用无法析构
        while (!Empty())
        {
            RemoveLast();
        }

        head_->suf = nullptr;
        tail_->pre = nullptr;
    }

    NodePtr Head() { return head_; };
    NodePtr Tail() { return tail_; };

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

    auto CreateNode(const KEY& key, const VALUE& value)->NodePtr
    {
        return DuNode<KEY, VALUE>::CreateNode(key, value);
    }

private:
    NodePtr head_;
    NodePtr tail_;
};

template <class Key, class Value>
class LFUCache
{
public:

    using NodePtr = typename DuLinkList<Key, Value>::NodePtr;

    LFUCache(int capacity)
    {
        max_size_ = capacity;
    }

    Value get(const Key& key)
    {
        auto iter = node_list_.find(key);
        if (iter == node_list_.end())return Value();

        auto& node = iter->second;
        RemoveFromTableList(node->reference++, node);
        AddToTableList(node->reference, node);

        return node->value;
    }

    void put(const Key& key, const Value& value)
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
            node = CreateNode(key, value);
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

    auto CreateNode(const Key& key, const Key& value)->NodePtr
    {
        return DuNode<Key, Value>::CreateNode(key, value);
    }

private:
    using DuLinkListPtr = std::shared_ptr<DuLinkList<Key, Value>>;

    std::map<int, DuLinkListPtr>table_list_;
    std::unordered_map<Key, NodePtr>node_list_;

    int max_size_{ 0 };
    int cur_size_{ 0 };
};

template <typename NodePtr>
auto DeleteDuplicates(NodePtr head)->decltype(head)
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

template<class NodePtr>
NodePtr ReverseDList(const NodePtr& head)
{
    auto cur_node = head;
    decltype(cur_node) pre_node = nullptr;

    while (cur_node)
    {
        std::swap(cur_node->pre, cur_node->suf);
        pre_node = cur_node;
        cur_node = cur_node->pre;
    }

    return pre_node;
}

template<class NodePtr>
void PrintDList(NodePtr head)
{
    auto&& logger = LOG(INFO);
    logger << "PrintDList:";
    while (head)
    {
        logger << head->value << " ";
        head = head->suf;
    }
}

template<class NodePtr>
void PrintList(NodePtr head)
{
    auto&& logger = LOG(INFO);
    logger << "PrintList:";
    while (head)
    {
        logger << head->val << " ";
        head = head->next;
    }
}

void TestList()
{
    auto head = std::make_shared<ListNode<int32_t>>(10);
    auto temp = head;
    for (int32_t i = 9; i >= 0; --i)
    {
        temp->next = std::make_shared<ListNode<int32_t>>(i);
        temp = temp->next;
    }

    PrintList(head);
    auto rev = ReverseList(head);
    PrintList(rev);
}

void TestDuList()
{
    DuLinkList<int, int> dlist;
    for (int i = 0; i < 10; ++i)
    {
        auto node = dlist.CreateNode(i, i);
        dlist.AddNode(node);
    }

    PrintDList(dlist.Head());
    ReverseDList(dlist.Head());
    PrintDList(dlist.Tail());
    ReverseDList(dlist.Tail());
}

void TestLRU()
{
    LRUCache<std::string, int32_t> l(10);
    l.put("1", 10);
    LOG(INFO) << "LRUCache get 1:[" << l.get("1") << "]";

    LRUCache<int32_t, int32_t> i(10);
    i.put(1, 10);
    LOG(INFO) << "LRUCache get 1:[" << i.get(1) << "]";
}

void TestLFU()
{
    LFUCache<std::string, std::string> lfu(2);
    lfu.put("q", "qqq");
    lfu.put("p", "ppp");
    lfu.get("q");
    lfu.put("m", "mmmm");

    LOG(INFO) << "LFUCache get q:[" << lfu.get("q") << "]";

    LOG(INFO) << "LFUCache get p:[" << lfu.get("p") << "]";

    LOG(INFO) << "LFUCache get m:[" << lfu.get("m") << "]";
}

void GetNextArray(const std::string& s2, std::vector<int32_t>& next)
{
    auto m = s2.length();
    next.resize(s2.length(),0);
    if (m < 2)return;

    next[0] = -1;
    next[1] = 0;

    size_t cn{ 0 }, i{ 2 };

    while (i < m)
    {
        if (s2[i-1] == s2[cn])
        {
            next[i++] = ++cn;
        }
        else if (cn > 0)
        {
            cn = next[cn];
        }
        else
        {
            next[i++] = 0;
        }
    }
}

size_t KMP(const std::string& s1, const std::string& s2)
{
    std::vector<int32_t>next;
    GetNextArray(s2, next);

    size_t n{ s1.length() }, m{s2.length()}, i{ 0 }, j{ 0 };
    while (i < n && j < m)
    {
        if (s1[i] == s2[j])
        {
            ++i; ++j;
        }
        else if (j == 0)
        {
            ++i;
        }
        else
        {
            j = next[j];
        }
    }

    return j == m ? i - j : std::string::npos;
}

void LeetCodeTest::StartTest()
{
    TestList();
    TestLRU();
    TestLFU();
    TestDuList();

    std::string s1 = "asdadasdasda123sdqwerdfdcefg0dkfguidjkfghdkdfsdklfjslfsdklfjsl;dkfjsldkfjslkfjskl;fjsldkfjsl;dkfjsl;kdfjsldkfjsdlfkjsdl;fkjslf";
    std::string s2 = "fkj";

    helper::time::Timer t;
    for (auto i  =0;i< 10000000;++i)
    {
        KMP(s1, s2);
    }

    LOG(INFO) << "KMP:[" << t.step() << "] "<< KMP(s1, s2);;

    for (auto i = 0; i < 10000000; ++i)
    {
        std::string s1 = "asdadasdasda123sdqwerdfdcefg0dkfguidjkfghdkdfsdklfjslfsdklfjsl;dkfjsldkfjslkfjskl;fjsldkfjsl;dkfjsl;kdfjsldkfjsdlfkjsdl;fkjslf";
        std::string s2 = "fkj";
        s1.find(s2);
        //LOG(INFO) << "find:[" << t.step() << "] " << s1.find(s2);;

    }

    LOG(INFO) << "find:[" << t.step() << "] " << s1.find(s2);;
}

class ThroneInheritance
{
public:

    template <class V>
    struct TreeNode
    {
        V val;
        using NodePtr = std::shared_ptr<TreeNode<V>>;
        //using NodePtr = TreeNode<V>*;
        std::vector<NodePtr> child_list;
        bool alive;
        TreeNode(const V& v) :val(v), alive(true) {};

        static NodePtr CreateNode(const V& v)
        {
            //return new TreeNode<V>(v);
            return std::make_shared<TreeNode<V>>(v);
        }
    };

    using Value = std::string;
    using NodePtr = typename TreeNode<Value>::NodePtr;

    template <class V>
    auto PreDfsTree(const NodePtr& root, std::vector<V>& res)->void
    {
        if (!root)return;
        if (root->alive)res.push_back(root->val);
        for (const auto& item : root->child_list)
        {
            PreDfsTree(item, res);
        }
    }

    ThroneInheritance(Value kingName)
    {
        root_ = TreeNode<Value>::CreateNode(kingName);
        node_list_.insert({ kingName, root_ });
    }

    void birth(Value parentName, Value childName)
    {
        auto parent_node = node_list_[parentName];
        auto child_node = TreeNode<Value>::CreateNode(childName);
        parent_node->child_list.emplace_back(child_node);
        node_list_.insert({ childName, child_node });
    }

    void death(Value name)
    {
        auto node = node_list_[name];
        node->alive = false;
    }

    std::vector<Value> getInheritanceOrder()
    {
        std::vector<Value> res;
        PreDfsTree(root_, res);
        return res;
    }

private:
    std::unordered_map<std::string, NodePtr> node_list_;
    NodePtr root_;
};

template<typename VALUE,
    typename container = std::vector<VALUE>,
    typename CMP = std::less<VALUE>>
    class PriorityQueue
{
public:
    PriorityQueue(CMP cmp) :cmp_(cmp) {};

    void pop()
    {
        std::swap(heap_[0], heap_[size_-- - 1]);
        heap_.pop_back();
        sink(0);
    }

    VALUE top()
    {
        return heap_[0];
    }

    bool empty()
    {
        return heap_.empty();
    }

    void push(const VALUE& value)
    {
        heap_.push_back(value);
        down(size_++);
    }

private:

    void sink(int index)
    {
        while (index < size_ / 2)
        {
            auto left = index * 2 + 1;
            auto right = index * 2 + 2;

            auto cur = (right < size_&& cmp_(heap_[right], heap_[left])) ? right : left;
            if (cmp_(heap_[cur], heap_[index]))
            {
                std::swap(heap_[cur], heap_[index]);
                index = cur;
                continue;
            }
            break;
        }
    }

    void down(int index)
    {
        while (index > 0)
        {
            auto parent = (index + 1) / 2 - 1;
            if (cmp_(heap_[index], heap_[parent]))
            {
                std::swap(heap_[index], heap_[parent]);
                index = parent;
                continue;
            }
            break;
        }
    }

private:
    container heap_;
    size_t size_{ 0 };
    CMP cmp_;
};