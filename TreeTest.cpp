#include "stdafx.h"
#include "TreeNode.h"
#include "RBTree.h"
#include "TreeTest.h"

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
	RBTree<std::shared_ptr<Tree::RBTree::RBTreeNode<int32_t>>> rb_tree;
	for (int i = 0; i < 100; ++i)
	{
		auto node = std::make_shared<Tree::RBTree::RBTreeNode<int32_t>>();
		node->value = i;
		rb_tree.Insert(node);
	}
	MidDfsPrintTree(rb_tree.GetRoot());
	LOG(INFO) << "TestRBTreeNode end ....";
}

void TestRBKeyTreeNode()
{
	LOG(INFO) << "TestRBKeyTreeNode begin ....";
	RBTree<std::shared_ptr<Tree::RBTree::RBKeyTreeNode<int32_t, int32_t>>> rb_tree;
	for (int i = 0; i < 100; ++i)
	{
		auto node = std::make_shared<Tree::RBTree::RBKeyTreeNode<int32_t, int32_t>>();
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
