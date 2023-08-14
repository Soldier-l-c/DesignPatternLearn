#pragma once

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
			while (node && node->tight)node = node->tight;
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

		NodePtr operator*()
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
	void EraseEx(const KeyOrValue& node)
	{
		NodePtr erase_node{ nullptr };
		erase_node = Find(node).node_;
		
		if (nullptr == erase_node)return;

		EraseInternal(iterator(erase_node));
	}

	void Erase(iterator iter)
	{
		EraseInternal(iter);
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
		return iterator(Min(root_));
	}

	iterator End()
	{
		return iterator(nullptr);
	}

	NodePtr GetRoot()
	{
		return root_;
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

	void EraseInternal(iterator erase_iter)
	{
		NodePtr erase_node = erase_iter.node_;
		++erase_iter;

		auto pnode = erase_node;
		NodePtr fix_node{ nullptr };
		NodePtr fix_node_parent{ nullptr };

		//�Ƚ�ɾ���ڵ��������ӽڵ�ĵ������ת��Ϊɾ���ڵ������һ���ӽڵ�������Ĭ��ʹ����������С�ڵ��滻
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
			pnode = erase_iter.node_;
			fix_node = pnode->right;
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

	static NodePtr Min(NodePtr node)
	{
		while (node && node->left)node = node->left;
		return node;
	}

private:
	NodePtr root_{ nullptr };
};