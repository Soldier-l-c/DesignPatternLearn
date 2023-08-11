#pragma once

template <class NodePtr>
class RBTree
{
	struct InserLocation
	{
		NodePtr parent;
		Tree::TreeChildNode child_pos;
	};

public:
	void Insert(NodePtr node)
	{
		if (root_ == nullptr)
		{
			root_ = node;
			root_->col = Tree::RBTree::NodeColor::Black;
			return;
		}

		node->col = Tree::RBTree::NodeColor::Red;
		auto insert_location = GetInsertParentNode(node);
		if (Tree::TreeChildNode::Left == insert_location.child_pos)
		{
			insert_location.parent->left = node;
		}
		else
		{
			insert_location.parent->right = node;
		}

		auto parent = insert_location.parent;
		while (parent && parent->col == Tree::RBTree::NodeColor::Red)
		{
			auto pp_node = parent->parent;

			if (parent == pp_node->left)
			{
				auto sib_node = pp_node->right;
				if (sib_node && Tree::RBTree::NodeColor::Red == sib_node->col)
				{
					sib_node->col = Tree::RBTree::NodeColor::Black;
					parent->col = Tree::RBTree::NodeColor::Black;
					pp_node->col = Tree::RBTree::NodeColor::Red;
					node = pp_node;
					parent = pp_node->parent;
					continue;
				}

				if (Tree::TreeChildNode::Right == insert_location.child_pos)
				{
					RotateL(parent);

					//左旋之后，node是parent的parent，之后右旋的时候修改颜色需要修改node的颜色
					parent = node;
				}

				pp_node->col = Tree::RBTree::NodeColor::Red;
				parent->col = Tree::RBTree::NodeColor::Black;
				RotateR(pp_node);
			}
			else
			{
				auto sib_node = pp_node->left;
				if (sib_node && Tree::RBTree::NodeColor::Red == sib_node->col)
				{
					sib_node->col = Tree::RBTree::NodeColor::Black;
					parent->col = Tree::RBTree::NodeColor::Black;
					pp_node->col = Tree::RBTree::NodeColor::Red;
					node = pp_node;
					parent = pp_node->parent;
					continue;
				}

				if (Tree::TreeChildNode::Left == insert_location.child_pos)
				{
					RotateR(parent);

					//右旋之后，node是其之前parent的parent，之后左旋的时候修改颜色需要修改node的颜色
					parent = node;
				}

				pp_node->col = Tree::RBTree::NodeColor::Red;
				parent->col = Tree::RBTree::NodeColor::Black;
				RotateL(pp_node);
			}

			root_->col = Tree::RBTree::NodeColor::Black;
		}
	}
	
	void Erease(const NodePtr& node)
	{

	}

	NodePtr GetRoot()
	{
		return root_;
	}

private:
	void RotateL(const NodePtr& node)
	{
		auto right = node->right;
		auto right_left = node->right->left;

		right->left = node;
		node->right = right_left;

		if (right_left != nullptr)
		{
			right_left->parent = node;
		}

		right->parent = node->parent;
		node->parent = right;
	}

	void RotateR(const NodePtr& node)
	{
		auto left = node->left;
		auto left_right = left->right;

		left->right = node;
		node->left = left_right;

		if (left_right != nullptr)
		{
			left_right->parent = node;
		}

		left->parent = node->parent;
		node->parent = left;
	}

	InserLocation GetInsertParentNode(const NodePtr& node)
	{
		InserLocation res;
		NodePtr cur_node = root_;
		while (nullptr != cur_node)
		{
			res.parent = cur_node;
			if (cur_node > node)
			{
				cur_node = cur_node->left;
				res.child_pos = Tree::TreeChildNode::Left;
			}
			else
			{
				cur_node = cur_node->right;
				res.child_pos = Tree::TreeChildNode::Right;
			}
		}

		return res;
	}

private:
	NodePtr root_;
};