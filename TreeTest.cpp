#include "stdafx.h"
#include "TreeNode.h"
#include "RBTree.h"
#include "TreeTest.h"

using IntIntKeyNode			= Tree::RBTree::RBKeyTreeNode<int32_t, int32_t>;
using StringIntKeyNode		= Tree::RBTree::RBKeyTreeNode<std::string, int32_t>;
using IntNode				= Tree::RBTree::RBTreeNode<int32_t>;
using StringNode			= Tree::RBTree::RBTreeNode<std::string>;

using IntIntKeyNodePtr		= IntIntKeyNode::RBKeyTreeNodePtr;
using IntNodePtr			= IntNode::RBTreeNodePtr;
using StringIntKeyNodePtr	= StringIntKeyNode::RBKeyTreeNodePtr;
using StringNodePtr			= StringNode::RBTreeNodePtr;

template <class TreeNodePtr>
void PrintNode(const TreeNodePtr& root)
{
	if (nullptr == root)return;

	if constexpr (std::is_same_v<TreeNodePtr, IntIntKeyNodePtr> || std::is_same_v<TreeNodePtr, StringIntKeyNodePtr>)
	{
		LOG(INFO) << "node key:[" << root->key << "] value:[" << root->value << "]";
	}
	else
	{
		LOG(INFO) << "node:[" << root->value << "]";
	}
}

template <class TreeNodePtr>
void MidDfsPrintTree(const TreeNodePtr& root)
{
	if (nullptr == root)return;

	std::stack<TreeNodePtr>nodes;
	auto pnode = root;
	while (nullptr != pnode || nodes.size() > 0)
	{
		while (nullptr != pnode)
		{
			nodes.push(pnode);
			pnode = pnode->left;
		}
		auto temp_node = nodes.top();
		nodes.pop();
		PrintNode(temp_node);

		pnode = temp_node->right;
	}
}

template <class TreeNodePtr>
void PreDfsPrintTree(const TreeNodePtr& root)
{
	if (nullptr == root)return;

	std::stack<TreeNodePtr>nodes;
	auto pnode = root;
	while (nullptr != pnode || nodes.size() > 0)
	{
		while (nullptr != pnode)
		{
			PrintNode(pnode);
			nodes.push(pnode);
			pnode = pnode->left;
		}
		pnode = nodes.top()->right;
		nodes.pop();
	}
}

template <class TreeNodePtr>
void SufixDfsPrintTree(const TreeNodePtr& root)
{
	if (nullptr == root)return;

	std::stack<TreeNodePtr>nodes;
	std::vector<TreeNodePtr>node_list;
	auto pnode = root;
	while (nullptr != pnode || nodes.size() > 0)
	{
		while (nullptr != pnode)
		{
			node_list.push_back(pnode);
			nodes.push(pnode);
			pnode = pnode->right;
		}
		pnode = nodes.top()->left;
		nodes.pop();
	}

	std::for_each(node_list.rbegin(), node_list.rend(), [](const TreeNodePtr& root) {PrintNode(root); });
}


void TestRBTreeNode()
{
	LOG(INFO) << "TestRBTreeNode begin ....";
	RBTree<IntNode> nrb_tree;
	for (int i = 0; i < 100; ++i)
	{
		auto node = std::make_shared<IntNode>();
		node->value = i;
		nrb_tree.Insert(node);
	}
	MidDfsPrintTree(nrb_tree.GetRoot());

	RBTree<StringNode> srb_tree;
	for (int i = 0; i < 100; ++i)
	{
		auto node = std::make_shared<StringNode>();
		node->value = std::to_string(i);
		srb_tree.Insert(node);
	}
	MidDfsPrintTree(srb_tree.GetRoot());

	auto find_node = srb_tree.Find(decltype(StringNode::value)("1"));
	if (find_node!= srb_tree.End())
	{
		LOG(INFO) << "Find node:[" << find_node->value << "]";
	}


	LOG(INFO) << "TestRBTreeNode end ....";
}

void TestRBKeyTreeNode()
{
	LOG(INFO) << "TestRBKeyTreeNode begin ....";
	RBTree<IntIntKeyNode> nrb_tree;
	for (int i = 0; i < 100; ++i)
	{
		auto node = std::make_shared<IntIntKeyNode>();
		node->value = i;
		node->key = i;
		nrb_tree.Insert(node);
	}

	const auto node_size = 100000;
	RBTree<StringIntKeyNode> srb_tree;
	for (int i = 0; i < node_size; ++i)
	{
		auto node = std::make_shared<StringIntKeyNode>();
		node->value = i;
		node->key = std::to_string(node_size -i);
		srb_tree.Insert(node);
	}

	auto find_node = srb_tree.Find(decltype(StringIntKeyNode::key)("1"));
	if (find_node != srb_tree.End())
	{
		LOG(INFO) << "Find node:[" << find_node->value << "]";
	}

	for (int i = 0; i < node_size; ++i)
	{
		auto erase_node = std::to_string(i);
		LOG(INFO) << "erase_node:[" << erase_node << "]";

		srb_tree.Erase(erase_node);
	}

	std::for_each(srb_tree.Begin(), srb_tree.End(), [](const StringIntKeyNodePtr& node) {PrintNode(node); });

	LOG(INFO) << "TestRBKeyTreeNode end ....";
}

void CTreeTest::StartTest()
{
	TestRBTreeNode();
	TestRBKeyTreeNode();
}
