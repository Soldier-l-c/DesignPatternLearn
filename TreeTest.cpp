#include "stdafx.h"
#include "TreeNode.h"
#include "RBTree.h"
#include "TreeTest.h"

using IntIntKeyNode = Tree::RBTree::RBKeyTreeNode<int32_t, int32_t>;
using IntNode = Tree::RBTree::RBTreeNode<int32_t>;
using IntIntKeyNodePtr = IntIntKeyNode::RBKeyTreeNodePtr;
using IntNodePtr = IntNode::RBTreeNodePtr;

template <class TreeNodePtr>
void MidDfsPrintTree(const TreeNodePtr& root)
{
	if (nullptr == root)return;

	MidDfsPrintTree(root->left);
	LOG(INFO) << "node:[" << root->value << "]";
	MidDfsPrintTree(root->right);
}

void TestRBTreeNode()
{
	LOG(INFO) << "TestRBTreeNode begin ....";
	RBTree<IntNodePtr> rb_tree;
	for (int i = 0; i < 100; ++i)
	{
		auto node = std::make_shared<IntNode>();
		node->value = i;
		rb_tree.Insert(node);
	}
	MidDfsPrintTree(rb_tree.GetRoot());
	LOG(INFO) << "TestRBTreeNode end ....";
}

void TestRBKeyTreeNode()
{
	LOG(INFO) << "TestRBKeyTreeNode begin ....";
	RBTree<IntIntKeyNodePtr> rb_tree;
	for (int i = 0; i < 100; ++i)
	{
		auto node = std::make_shared<IntIntKeyNode>();
		node->value = i;
		node->key = i;
		rb_tree.Insert(node);
	}
	MidDfsPrintTree(rb_tree.GetRoot());
	LOG(INFO) << "TestRBKeyTreeNode end ....";
}

void CTreeTest::StartTest()
{
	TestRBTreeNode();
	TestRBKeyTreeNode();
}
