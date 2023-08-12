#pragma once

template <class Node>
class RBTree
{
	using NodePtr = std::shared_ptr<Node>;
	using InserLocation = typename Node::InsLocaType;

public:
	void Insert(NodePtr node)
	{
		auto insert_location = GetInsertLocation(node);
		if(nullptr == insert_location.parent)
		{
			root_ = node;
			root_->col = Tree::RBTree::NodeColor::Black;
			return;
		}

		node->col = Tree::RBTree::NodeColor::Red;
		if (Tree::TreeChildNode::Left == insert_location.child_pos)
		{
			insert_location.parent->left = node;
		}
		else
		{
			insert_location.parent->right = node;
		}
		node->parent = insert_location.parent;

		//插入节点后重新平衡红黑树
		MakeInsertBalance(insert_location.parent, node, insert_location);

		//关键一步，避免执行平衡过程中可能会将根节点修改为红色节点，重新设置为黑色
		root_->col = Tree::RBTree::NodeColor::Black;
	}
	
	void Erease(const NodePtr& node)
	{
		//待实现
	}

	NodePtr GetRoot()
	{
		return root_;
	}

private:
	void RotateL(const NodePtr& node)
	{
		//p_node 旋转后，node的父节点。node变为其左子节点
		auto p_node = node->right;
		//right_left p_node的左子节点，旋转后变为node的右子节点
		auto right_left = node->right->left;

		//将p_node的left赋值为node，并将node的右子节点赋值为p_node的左子节点
		p_node->left = node;
		node->right = right_left;

		//p_node的左子节点不为空的话，需要将其父节点设置为node
		if (right_left != nullptr)
		{
			right_left->parent = node;
		}

		//将p_node 的父子节点赋值为node的父子节点
		p_node->parent = node->parent;

		//将node的父子节点赋值为p_node
		node->parent = p_node;

		//处理旋转节点p_node父节点的指向问题
		//旋转前node就是根节点，直接将根节点赋值为p_node,作为新的根节点
		if (node == root_)
		{
			root_ = p_node;
		}
		//node节点是其原父节点的左子节点，将p_node 的父节点的左子节点赋值为p_node
		else if(p_node->parent->left && p_node->parent->left == node)
		{
			p_node->parent->left = p_node;
		}
		else
		{
			p_node->parent->right = p_node;
		}
	}

	void RotateR(const NodePtr& node)
	{
		//p_node 旋转后，node的父节点。node变为其右子节点
		auto p_node = node->left;
		//left_right p_node的右子节点，旋转后变为node的左子节点
		auto left_right = p_node->right;

		//将p_node的right赋值为node，并将node的左子节点赋值为p_node的右子节点
		p_node->right = node;
		node->left = left_right;

		//p_node的右子节点不为空的话，需要将其父节点设置为node
		if (left_right != nullptr)
		{
			left_right->parent = node;
		}

		//将p_node 的父子节点赋值为node的父子节点
		p_node->parent = node->parent;

		//将node的父子节点赋值为p_node
		node->parent = p_node;

		//处理旋转节点父节点的指向问题
		//旋转前node就是根节点，直接将根节点赋值为p_node,作为新的根节点
		if (node == root_)
		{
			root_ = p_node;
		}
		//node节点是其原父节点的左子节点
		else if (p_node->parent->left && p_node->parent->left == node)
		{
			p_node->parent->left = p_node;
		}
		else
		{
			p_node->parent->right = p_node;
		}
	}

	void MakeInsertBalance(NodePtr parent, NodePtr node, const InserLocation& insert_location)
	{
		while (nullptr != parent && parent->col == Tree::RBTree::NodeColor::Red)
		{
			auto pp_node = parent->parent;
			auto parent_child_type = (pp_node->left && parent == pp_node->left) ? Tree::TreeChildNode::Left : Tree::TreeChildNode::Right;

			//第一种情况，叔父节点为红色，直接把父节点以及叔父节点置为黑色，祖父节点置为红色
			//node 赋值为祖父节点，parent赋为祖父节点的父节点，继续执行平衡
			NodePtr sib_node = parent_child_type == Tree::TreeChildNode::Left ? pp_node->right : pp_node->left;
			if (sib_node && Tree::RBTree::NodeColor::Red == sib_node->col)
			{
				sib_node->col = Tree::RBTree::NodeColor::Black;
				parent->col = Tree::RBTree::NodeColor::Black;
				pp_node->col = Tree::RBTree::NodeColor::Red;
				node = pp_node;
				parent = pp_node->parent;
				continue;
			}

			//第二种情况，叔父节点不存在或者存在并且为黑色
			if (parent_child_type == Tree::TreeChildNode::Left)
			{
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
		}
	}

	InserLocation GetInsertLocation(const NodePtr& node)
	{
		return node->GetInsertLocation(root_);
	}

private:
	NodePtr root_{ nullptr };
};