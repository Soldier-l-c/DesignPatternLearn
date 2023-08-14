#pragma once

template <class Node>
class RBTree
{
	using NodePtr = std::shared_ptr<Node>;

	struct InserLocation
	{
		NodePtr parent{ nullptr };
		Tree::TreeChildNode child_pos{ Tree::TreeChildNode::Right };
	};

	struct FindResult
	{
		NodePtr bound{ nullptr };
		InserLocation insert_location;
	};

public:
	void Insert(NodePtr node)
	{
		auto insert_location_dup = GetInsertLocation(node);
		if (!insert_location_dup.second)
		{
			return;
		}

		auto insert_location = insert_location_dup.first;

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

	template <class KeyOrValueOrNodePtr>
	void EraseEx(const KeyOrValueOrNodePtr& node)
	{
		using EreaseNode = KeyOrValueOrNodePtr;
		NodePtr erase_node{ nullptr };
		if constexpr (std::is_same_v<std::decay_t<KeyOrValueOrNodePtr>, NodePtr>)
		{
			erase_node = node;
		}
		else
		{
			erase_node = Find(node);
		}
		if (nullptr == erase_node)return;



	}

	void Erase(const NodePtr& node)
	{
		EraseEx<NodePtr>(node);
	}

	NodePtr GetRoot()
	{
		return root_;
	}

	template <class KeyOrValue>
	NodePtr Find(const KeyOrValue& key_value)
	{
		auto lowwer_bound = FindLowerBound(key_value);

		if (LowerBoundDuplicate(lowwer_bound.bound, key_value))
		{
			return lowwer_bound.bound;
		}

		return NodePtr();
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
		else if(p_node->parent->left == node)
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
		else if (p_node->parent->left == node)
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
			auto parent_child_type = (parent == pp_node->left) ? Tree::TreeChildNode::Left : Tree::TreeChildNode::Right;

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

	template <class KeyOrValue>
	FindResult FindLowerBound(const KeyOrValue& key_value)
	{
		FindResult res;
		auto p_node = root_;
		while (p_node)
		{
			res.insert_location.parent = p_node;
			//p_node的value或key小于key_value，说明key_value在p_node的右子树中或者应该插入到p_node的右子树中
			if (p_node->Compare(key_value) < 0)
			{
				p_node = p_node->right;
				res.insert_location.child_pos = Tree::TreeChildNode::Right;//插入位置
			}
			//p_node的value或key大于等于key_value，说明key_value在p_node以及p_node的左子树中或者应该插入到p_node的左子树中
			else
			{
				res.bound = p_node;
				res.insert_location.child_pos = Tree::TreeChildNode::Left;
				p_node = p_node->left;
			}
		}
		return res;
	}

	template <class KeyOrValue>
	FindResult FindUpperBound(const KeyOrValue& key_value)
	{
		FindResult res;
		auto p_node = root_;
		while (p_node)
		{
			res.insert_location.parent = p_node;
			//p_node的value或key小于key_value，说明key_value在p_node的右子树中或者应该插入到p_node的右子树中
			if (p_node->Compare(key_value) <= 0)
			{
				p_node = p_node->right;
				res.insert_location.child_pos = Tree::TreeChildNode::Right;//插入位置
			}
			//p_node的value或key大于等于key_value，说明key_value在p_node以及p_node的左子树中或者应该插入到p_node的左子树中
			else
			{
				res.bound = p_node;
				res.insert_location.child_pos = Tree::TreeChildNode::Left;
				p_node = p_node->left;
			}
		}
		return res;
	}

	template <class KeyOrValue>
	bool LowerBoundDuplicate(const NodePtr& bound, const KeyOrValue& key_value)
	{
		//当查找到的lowwerbound 的值等于 key_value时，其实就已经出现重复了。（小于的情况其实不会出现）
		return bound && !(bound->Compare(key_value) > 0);
	}

	std::pair<InserLocation, bool> GetInsertLocation(const NodePtr& node)
	{
		auto lowwer_bound = FindLowerBound(node);
		if (lowwer_bound.bound && LowerBoundDuplicate(lowwer_bound.bound, node))
		{
			return { lowwer_bound.insert_location, false };
		}

		return { lowwer_bound.insert_location , true };
	}

private:
	NodePtr root_{ nullptr };
};