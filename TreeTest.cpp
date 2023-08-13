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
void MidDfsPrintTree(const TreeNodePtr& root)
{
	if (nullptr == root)return;

	MidDfsPrintTree(root->left);

	if constexpr (std::is_same_v<TreeNodePtr, IntIntKeyNodePtr> || std::is_same_v<TreeNodePtr, StringIntKeyNodePtr>)
	{
		LOG(INFO) << "node key:["<< root->key<<"] value:[" << root->value << "]";
	}
	else 
	{
		LOG(INFO) << "node:[" << root->value << "]";
	}

	MidDfsPrintTree(root->right);
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
	if (find_node)
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
	MidDfsPrintTree(nrb_tree.GetRoot());

	RBTree<StringIntKeyNode> srb_tree;
	for (int i = 0; i < 100; ++i)
	{
		auto node = std::make_shared<StringIntKeyNode>();
		node->value = i;
		node->key = std::to_string(100-i);
		srb_tree.Insert(node);
	}

	MidDfsPrintTree(srb_tree.GetRoot());

	auto find_node = srb_tree.Find(decltype(StringIntKeyNode::key)("1"));
	if (find_node)
	{
		LOG(INFO) << "Find node:[" << find_node->value << "]";
	}

	LOG(INFO) << "TestRBKeyTreeNode end ....";
}

void CTreeTest::StartTest()
{
	TestRBTreeNode();
	TestRBKeyTreeNode();
}
