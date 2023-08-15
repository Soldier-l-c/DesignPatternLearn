#pragma once

/*
1.节点非红即黑。
2.根节点是黑色。
3.所有NULL结点称为叶子节点，且认为颜色为黑。
4.所有红节点的子节点都为黑色。
5.从任一节点到其叶子节点的所有路径上都包含相同数目的黑节点。
*/

namespace NRBTree
{
	template <class Node>
	struct IteratorBase
	{
		using NodePtr = std::shared_ptr<std::decay_t<Node>>;

		IteratorBase(const NodePtr& node) :node_(node) {};
		IteratorBase() = default;

		static NodePtr Min(NodePtr node)
		{
			while (node && node->left)node = node->left;
			return node;
		}

		static NodePtr Max(NodePtr node)
		{
			while (node && node->right)node = node->right;
			return node;
		}

		IteratorBase operator++(int)
		{
			auto temp = *this;
			++*this;
			return temp;
		}

		IteratorBase& operator++()
		{
			if (!node_)return *this;
			
			if (nullptr == node_->right)
			{
				NodePtr pnode = node_->parent;
				while (pnode && pnode->right == node_)
				{
					node_ = pnode;
					pnode = pnode->parent;
				}
				node_ = pnode;
			}
			else
			{
				node_ = Min(node_->right);
			}

			return *this;
		}

		IteratorBase operator--(int)
		{
			auto temp = *this;
			--*this;
			return temp;
		}

		IteratorBase& operator--()
		{
			if (!node_)return *this;

			if (nullptr == node_->left)
			{
				NodePtr pnode = node_->parent;
				while (pnode && pnode->left == node_)
				{
					node_ = pnode;
					pnode = pnode->parent;
				}
				node_ = pnode;
			}
			else
			{
				node_ = Max(node_->left);
			}

			return *this;
		}

		const NodePtr& operator->()
		{
			return node_;
		}

		bool operator==(const IteratorBase& right)
		{
			return this->node_ == right->node_;
		}

		bool operator!=(const IteratorBase& right)
		{
			return !(*this == right);
		}

		NodePtr node_{ nullptr };
	};

	template <class Node>
	struct Iterator : public IteratorBase<Node>
	{
		using NodePtr = std::shared_ptr<std::decay_t<Node>>;

		using MyBase = IteratorBase<std::decay_t<Node>>;

		Iterator(const NodePtr& node) :IteratorBase<Node>(node) {};
		Iterator() = default;

		Iterator operator++(int)
		{
			auto temp_node = *this;
			MyBase::operator++();
			return temp_node;
		};

		Iterator& operator++() 
		{
			(*this)++;
			return *this;
		};

		Iterator operator--(int)
		{
			auto temp_node = *this;
			MyBase::operator--();
			return temp_node;
		};

		Iterator& operator--()
		{
			(*this)--;
			return *this;
		};

		const NodePtr& operator->()
		{
			return MyBase::operator->();
		}

		bool operator==(const Iterator& right)
		{
			return this->node_ == right.node_;
		}

		bool operator!=(const Iterator& right)
		{
			return !(*this == right);
		}

		NodePtr& operator*()
		{
			return this->node_;
		}

	};
}

template <class Node>
class RBTree
{
	using NodePtr = std::shared_ptr<std::decay_t<Node>>;
	using iterator = NRBTree::Iterator<std::decay_t<Node>>;

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

	std::pair<iterator, bool> Insert(NodePtr node)
	{
		auto insert_location_dup = GetInsertLocation(node);
		//已经存在了，不再插入
		if (!insert_location_dup.second)
		{
			return { iterator(insert_location_dup.first.bound), false };
		}

		return { iterator(InternalInsert(insert_location_dup.first.insert_location, node)), true };
	}

	template <class KeyOrValue>
	void Erase(const KeyOrValue& node)
	{
		NodePtr erase_node{ nullptr };
		erase_node = Find(node).node_;
		
		if (nullptr == erase_node)return;

		InternalErase(iterator(erase_node));
	}

	void Erase(iterator iter)
	{
		InternalErase(iter);
	}

	template <class KeyOrValue>
	iterator Find(const KeyOrValue& key_value)
	{
		auto lowwer_bound = FindLowerBound(key_value);

		if (LowerBoundDuplicate(lowwer_bound.bound, key_value))
		{
			return iterator(lowwer_bound.bound);
		}

		return End();
	}

	template <class KeyOrValue>
	iterator LowerBound(const KeyOrValue& key_value)
	{
		auto bound = FindLowerBound(key_value);
		if (nullptr != bound)
		{
			return iterator(bound);
		}
		return End();
	}

	template <class KeyOrValue>
	iterator UpperBound(const KeyOrValue& key_value)
	{
		auto bound = FindUpperBound(key_value);
		if (nullptr != bound)
		{
			return iterator(bound);
		}
		return End();
	}

	iterator Begin()
	{
		return iterator(iterator::Min(root_));
	}

	iterator End()
	{
		return iterator(nullptr);
	}

	iterator RBegin()
	{
		return iterator(iterator::Max(root_));
	}

	iterator REnd()
	{
		return iterator(nullptr);
	}

	NodePtr GetRoot()
	{
		return root_;
	}

	void Clear()
	{
		InternalClear();
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

	NodePtr InternalInsert(const InserLocation& insert_location, NodePtr node)
	{
		if (nullptr == insert_location.parent)
		{
			root_ = node;
			root_->col = Tree::RBTree::NodeColor::Black;
			return node;
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

		return node;
	}

	void InternalErase(iterator erase_iter)
	{
		NodePtr erase_node = erase_iter.node_;
		++erase_iter;

		//pnode 当erase_node有两个子节点时，替换要删除节点的节点，真正要删除的节点
		auto pnode = erase_node;
		//要真正删除节点的右子节点或者左子节点，也就是替补删除节点位置的节点
		NodePtr fix_node{ nullptr };
		//fix_node_parent要真正删除节点的父节点
		NodePtr fix_node_parent{ nullptr };

		//如果erase_node有一个节点，那么节点颜色一定是黑色，子节点颜色一定是红色
		//此时fix_node为空或者是一个红色节点
		if (erase_node->left == nullptr)
		{
			fix_node = erase_node->right;
		}
		else if (erase_node->right == nullptr)
		{
			fix_node = erase_node->left;
		}
		else
		{
			//先将删除节点有两个子节点的的情况，转换为删除节点最多有一个子节点的情况。默认使用右子树最小节点或者是其父节点的右子树的最小值替换
			//erase_node有两个子节点，此时pnode是其右子树中最小值
			//pnode至多有一个子节点，因为其为最小值，不可能存在左子树，只可能存在右子节点。
			//pnode的如果存在右子节点，那么pnode一定为黑色，右子节点一定为红色。为了满足性质5
			//此时fix_node为空，或者是一个红色节点
			pnode = erase_iter.node_;
			fix_node = pnode->right;
		}

		//被删除的节点只有一个子节点
		if (pnode == erase_node)
		{
			fix_node_parent = erase_node->parent;
			if (erase_node == root_)
			{
				root_ = fix_node;
			}
			else if (fix_node_parent->right == erase_node)
			{
				fix_node_parent->right = fix_node;
			}
			else
			{
				fix_node_parent->left = fix_node;
			}

			if (fix_node)
			{
				fix_node->parent = fix_node_parent;
			}
		}
		else
		{
			erase_node->left->parent = pnode;
			pnode->left = erase_node->left;

			//右子树删除右子树最小节点
			//删除节点的右子树最小节点就是其右子节点
			if (erase_node->right == pnode)
			{
				fix_node_parent = pnode;
			}
			else
			{
				fix_node_parent = pnode->parent;

				//pnode一定是其父节点的左子节点，因为pnode是子树的最小值，所以不可能是右子节点
				fix_node_parent->left = fix_node;
				pnode->right = erase_node->right;

				if (fix_node)
				{
					fix_node->parent = fix_node_parent;
				}
			}

			//使用右子树最小节点替换被删除节点
			if (root_ == erase_node)
			{
				root_ = pnode;
			}
			else if (erase_node == erase_node->parent->left)
			{
				erase_node->parent->left = pnode;
			}
			else
			{
				erase_node->parent->right = pnode;
			}
			pnode->parent = erase_node->parent;

			std::swap(pnode->col, erase_node->col);
		}

		//删除节点是红色，没有影响，直接返回
		//如果删除节点是红色，该节点不会存在只有一个节点的情况，要么其子节点都为黑色，要么没有子节点，删除不会影响高度平衡以及红黑树性质
		if (erase_node->col == Tree::RBTree::NodeColor::Red)
		{
			return;
		}

		//删除节点颜色是黑色（删除节点就是erse_node本身或者是其右子树最小节点替代节点颜色为黑色）
		do 
		{
			//替补节点是红色，删除节点是黑的，直接将其颜色修改为黑色即可
			if (root_ == fix_node || fix_node && fix_node->col == Tree::RBTree::NodeColor::Red)
			{
				break;
			}
			
			//下面考虑替补节点为空的情况
			for (; root_ != fix_node && (fix_node == nullptr || fix_node->col == Tree::RBTree::NodeColor::Black) && fix_node_parent;)
			{
				if (fix_node == fix_node_parent->left)
				{
					pnode = fix_node_parent->right;
					if (pnode && pnode->col == Tree::RBTree::NodeColor::Red)
					{
						pnode->col = Tree::RBTree::NodeColor::Black;
						fix_node_parent->col = Tree::RBTree::NodeColor::Red;
						RotateL(fix_node_parent);
						pnode = fix_node_parent->right;
					}

					if (nullptr == pnode)
					{
						fix_node = fix_node_parent;
					}
					else if ((!pnode->right && !pnode->left) || (pnode->left && pnode->right)&&
						pnode->left->col == Tree::RBTree::NodeColor::Black && pnode->right->col == Tree::RBTree::NodeColor::Black)
					{
						pnode->col = Tree::RBTree::NodeColor::Red;
						fix_node = fix_node_parent;
					}
					else
					{
						if (!pnode->right || pnode->right->col == Tree::RBTree::NodeColor::Black)
						{
							if (pnode->left)
							{
								pnode->left->col = Tree::RBTree::NodeColor::Black;
								RotateR(pnode);
							}
							pnode->col = Tree::RBTree::NodeColor::Red;
							pnode = fix_node_parent->right;
						}
						pnode->col = fix_node_parent->col;
						fix_node_parent->col = Tree::RBTree::NodeColor::Black;;
						pnode->right->col = Tree::RBTree::NodeColor::Black;;
						RotateL(fix_node_parent);
						break;
					}
				}
				else
				{
					pnode = fix_node_parent->left;
					if (pnode && pnode->col == Tree::RBTree::NodeColor::Red)
					{
						pnode->col = Tree::RBTree::NodeColor::Black;
						fix_node_parent->col = Tree::RBTree::NodeColor::Red;
						RotateR(fix_node_parent);
						pnode = fix_node_parent->left;
					}

					if (!pnode)
					{
						fix_node = fix_node_parent;
					}
					else if ((!pnode->right && !pnode->left) || (pnode->left && pnode->right) &&
						pnode->left->col == Tree::RBTree::NodeColor::Black && pnode->right->col == Tree::RBTree::NodeColor::Black)
					{
						pnode->col = Tree::RBTree::NodeColor::Red;
						fix_node = fix_node_parent;
					}
					else
					{
						if (!pnode->left || pnode->left->col == Tree::RBTree::NodeColor::Black)
						{
							if (pnode->right)
							{
								pnode->right->col = Tree::RBTree::NodeColor::Black;
								RotateL(pnode);
							}

							pnode->col = Tree::RBTree::NodeColor::Red;
							pnode = fix_node_parent->left;
						}
						pnode->col = fix_node_parent->col;
						fix_node_parent->col = Tree::RBTree::NodeColor::Black;;
						pnode->left->col = Tree::RBTree::NodeColor::Black;;
						RotateR(fix_node_parent);
						break;
					}
				}
				fix_node_parent = (fix_node ? fix_node->parent : nullptr);
			}


		} while (false);
			
		if (fix_node)
		{
			fix_node->col = Tree::RBTree::NodeColor::Black;
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

	std::pair<FindResult, bool> GetInsertLocation(const NodePtr& node)
	{
		auto lowwer_bound = FindLowerBound(node);
		if (lowwer_bound.bound && LowerBoundDuplicate(lowwer_bound.bound, node))
		{
			return { lowwer_bound , false };
		}

		return { lowwer_bound , true };
	}

	void ReleaseNode(NodePtr& node)
	{
		if (nullptr == node)return;

		//避免循环引用导致析构不掉
		if (node->parent && node == node->parent->left)
		{
			node->parent->left.reset();
		}
		else if (node->parent)
		{
			node->parent->right.reset();
		}

		node->left.reset();
		node->right.reset();
		node->parent.reset();

		node = nullptr;
	}

	void InternalClear()
	{
		std::stack<NodePtr>nodes;
		auto pnode = root_;
		while (nullptr != pnode || nodes.size() > 0)
		{
			while (nullptr != pnode)
			{
				nodes.push(pnode);
				pnode = pnode->left;
			}
			auto temp_node = nodes.top();
			nodes.pop();
			pnode = temp_node->right;

			ReleaseNode(temp_node);
		}

		//root_需要单独再析构一次
		ReleaseNode(root_);
	}

private:
	NodePtr root_{ nullptr };
};