#pragma once

/*
1.�ڵ�Ǻ켴�ڡ�
2.���ڵ��Ǻ�ɫ��
3.����NULL����ΪҶ�ӽڵ㣬����Ϊ��ɫΪ�ڡ�
4.���к�ڵ���ӽڵ㶼Ϊ��ɫ��
5.����һ�ڵ㵽��Ҷ�ӽڵ������·���϶�������ͬ��Ŀ�ĺڽڵ㡣
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
		//�Ѿ������ˣ����ٲ���
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
		//p_node ��ת��node�ĸ��ڵ㡣node��Ϊ�����ӽڵ�
		auto p_node = node->right;
		//right_left p_node�����ӽڵ㣬��ת���Ϊnode�����ӽڵ�
		auto right_left = node->right->left;

		//��p_node��left��ֵΪnode������node�����ӽڵ㸳ֵΪp_node�����ӽڵ�
		p_node->left = node;
		node->right = right_left;

		//p_node�����ӽڵ㲻Ϊ�յĻ�����Ҫ���丸�ڵ�����Ϊnode
		if (right_left != nullptr)
		{
			right_left->parent = node;
		}

		//��p_node �ĸ��ӽڵ㸳ֵΪnode�ĸ��ӽڵ�
		p_node->parent = node->parent;

		//��node�ĸ��ӽڵ㸳ֵΪp_node
		node->parent = p_node;

		//������ת�ڵ�p_node���ڵ��ָ������
		//��תǰnode���Ǹ��ڵ㣬ֱ�ӽ����ڵ㸳ֵΪp_node,��Ϊ�µĸ��ڵ�
		if (node == root_)
		{
			root_ = p_node;
		}
		//node�ڵ�����ԭ���ڵ�����ӽڵ㣬��p_node �ĸ��ڵ�����ӽڵ㸳ֵΪp_node
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
		//p_node ��ת��node�ĸ��ڵ㡣node��Ϊ�����ӽڵ�
		auto p_node = node->left;
		//left_right p_node�����ӽڵ㣬��ת���Ϊnode�����ӽڵ�
		auto left_right = p_node->right;

		//��p_node��right��ֵΪnode������node�����ӽڵ㸳ֵΪp_node�����ӽڵ�
		p_node->right = node;
		node->left = left_right;

		//p_node�����ӽڵ㲻Ϊ�յĻ�����Ҫ���丸�ڵ�����Ϊnode
		if (left_right != nullptr)
		{
			left_right->parent = node;
		}

		//��p_node �ĸ��ӽڵ㸳ֵΪnode�ĸ��ӽڵ�
		p_node->parent = node->parent;

		//��node�ĸ��ӽڵ㸳ֵΪp_node
		node->parent = p_node;

		//������ת�ڵ㸸�ڵ��ָ������
		//��תǰnode���Ǹ��ڵ㣬ֱ�ӽ����ڵ㸳ֵΪp_node,��Ϊ�µĸ��ڵ�
		if (node == root_)
		{
			root_ = p_node;
		}
		//node�ڵ�����ԭ���ڵ�����ӽڵ�
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

			//��һ��������常�ڵ�Ϊ��ɫ��ֱ�ӰѸ��ڵ��Լ��常�ڵ���Ϊ��ɫ���游�ڵ���Ϊ��ɫ
			//node ��ֵΪ�游�ڵ㣬parent��Ϊ�游�ڵ�ĸ��ڵ㣬����ִ��ƽ��
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

			//�ڶ���������常�ڵ㲻���ڻ��ߴ��ڲ���Ϊ��ɫ
			if (parent_child_type == Tree::TreeChildNode::Left)
			{
				if (Tree::TreeChildNode::Right == insert_location.child_pos)
				{
					RotateL(parent);

					//����֮��node��parent��parent��֮��������ʱ���޸���ɫ��Ҫ�޸�node����ɫ
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

					//����֮��node����֮ǰparent��parent��֮��������ʱ���޸���ɫ��Ҫ�޸�node����ɫ
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

		//����ڵ������ƽ������
		MakeInsertBalance(insert_location.parent, node, insert_location);

		//�ؼ�һ��������ִ��ƽ������п��ܻὫ���ڵ��޸�Ϊ��ɫ�ڵ㣬��������Ϊ��ɫ
		root_->col = Tree::RBTree::NodeColor::Black;

		return node;
	}

	void InternalErase(iterator erase_iter)
	{
		NodePtr erase_node = erase_iter.node_;
		++erase_iter;

		//pnode ��erase_node�������ӽڵ�ʱ���滻Ҫɾ���ڵ�Ľڵ㣬����Ҫɾ���Ľڵ�
		auto pnode = erase_node;
		//Ҫ����ɾ���ڵ�����ӽڵ�������ӽڵ㣬Ҳ�����油ɾ���ڵ�λ�õĽڵ�
		NodePtr fix_node{ nullptr };
		//fix_node_parentҪ����ɾ���ڵ�ĸ��ڵ�
		NodePtr fix_node_parent{ nullptr };

		//���erase_node��һ���ڵ㣬��ô�ڵ���ɫһ���Ǻ�ɫ���ӽڵ���ɫһ���Ǻ�ɫ
		//��ʱfix_nodeΪ�ջ�����һ����ɫ�ڵ�
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
			//�Ƚ�ɾ���ڵ��������ӽڵ�ĵ������ת��Ϊɾ���ڵ������һ���ӽڵ�������Ĭ��ʹ����������С�ڵ�������丸�ڵ������������Сֵ�滻
			//erase_node�������ӽڵ㣬��ʱpnode��������������Сֵ
			//pnode������һ���ӽڵ㣬��Ϊ��Ϊ��Сֵ�������ܴ�����������ֻ���ܴ������ӽڵ㡣
			//pnode������������ӽڵ㣬��ôpnodeһ��Ϊ��ɫ�����ӽڵ�һ��Ϊ��ɫ��Ϊ����������5
			//��ʱfix_nodeΪ�գ�������һ����ɫ�ڵ�
			pnode = erase_iter.node_;
			fix_node = pnode->right;
		}

		//��ɾ���Ľڵ�ֻ��һ���ӽڵ�
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

			//������ɾ����������С�ڵ�
			//ɾ���ڵ����������С�ڵ���������ӽڵ�
			if (erase_node->right == pnode)
			{
				fix_node_parent = pnode;
			}
			else
			{
				fix_node_parent = pnode->parent;

				//pnodeһ�����丸�ڵ�����ӽڵ㣬��Ϊpnode����������Сֵ�����Բ����������ӽڵ�
				fix_node_parent->left = fix_node;
				pnode->right = erase_node->right;

				if (fix_node)
				{
					fix_node->parent = fix_node_parent;
				}
			}

			//ʹ����������С�ڵ��滻��ɾ���ڵ�
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

		//ɾ���ڵ��Ǻ�ɫ��û��Ӱ�죬ֱ�ӷ���
		//���ɾ���ڵ��Ǻ�ɫ���ýڵ㲻�����ֻ��һ���ڵ�������Ҫô���ӽڵ㶼Ϊ��ɫ��Ҫôû���ӽڵ㣬ɾ������Ӱ��߶�ƽ���Լ����������
		if (erase_node->col == Tree::RBTree::NodeColor::Red)
		{
			return;
		}

		//ɾ���ڵ���ɫ�Ǻ�ɫ��ɾ���ڵ����erse_node�������������������С�ڵ�����ڵ���ɫΪ��ɫ��
		do 
		{
			//�油�ڵ��Ǻ�ɫ��ɾ���ڵ��Ǻڵģ�ֱ�ӽ�����ɫ�޸�Ϊ��ɫ����
			if (root_ == fix_node || fix_node && fix_node->col == Tree::RBTree::NodeColor::Red)
			{
				break;
			}
			
			//���濼���油�ڵ�Ϊ�յ����
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
			//p_node��value��keyС��key_value��˵��key_value��p_node���������л���Ӧ�ò��뵽p_node����������
			if (p_node->Compare(key_value) < 0)
			{
				p_node = p_node->right;
				res.insert_location.child_pos = Tree::TreeChildNode::Right;//����λ��
			}
			//p_node��value��key���ڵ���key_value��˵��key_value��p_node�Լ�p_node���������л���Ӧ�ò��뵽p_node����������
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
			//p_node��value��keyС��key_value��˵��key_value��p_node���������л���Ӧ�ò��뵽p_node����������
			if (p_node->Compare(key_value) <= 0)
			{
				p_node = p_node->right;
				res.insert_location.child_pos = Tree::TreeChildNode::Right;//����λ��
			}
			//p_node��value��key���ڵ���key_value��˵��key_value��p_node�Լ�p_node���������л���Ӧ�ò��뵽p_node����������
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
		//�����ҵ���lowwerbound ��ֵ���� key_valueʱ����ʵ���Ѿ������ظ��ˡ���С�ڵ������ʵ������֣�
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

		//����ѭ�����õ�����������
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

		//root_��Ҫ����������һ��
		ReleaseNode(root_);
	}

private:
	NodePtr root_{ nullptr };
};