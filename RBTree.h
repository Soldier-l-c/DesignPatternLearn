#pragma once

template <class NodePtr>
class RBTree
{
	struct InserLocation
	{
		NodePtr parent{ nullptr };
		Tree::TreeChildNode child_pos{ Tree::TreeChildNode::Left };
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
		auto insert_location = GetInsertLocation(node);
		if (Tree::TreeChildNode::Left == insert_location.child_pos)
		{
			insert_location.parent->left = node;
		}
		else
		{
			insert_location.parent->right = node;
		}
		node->parent = insert_location.parent;

		auto parent = insert_location.parent;
		while (nullptr != parent && parent->col == Tree::RBTree::NodeColor::Red)
		{
			auto pp_node = parent->parent;
			auto parent_child_type = (pp_node->left && parent == pp_node->left) ? Tree::TreeChildNode::Left : Tree::TreeChildNode::Right;

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

		//�ؼ�һ��������ִ��ƽ������п��ܻὫ���ڵ��޸�Ϊ��ɫ�ڵ㣬��������Ϊ��ɫ
		root_->col = Tree::RBTree::NodeColor::Black;
	}
	
	void Erease(const NodePtr& node)
	{
		//��ʵ��
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
		else if (p_node->parent->left && p_node->parent->left == node)
		{
			p_node->parent->left = p_node;
		}
		else
		{
			p_node->parent->right = p_node;
		}
	}

	InserLocation GetInsertLocation(const NodePtr& node)
	{
		InserLocation res;
		NodePtr cur_node = root_;
		while (nullptr != cur_node)
		{
			res.parent = cur_node;
			//����Ҫ��NodePtr��>���������أ���Ȼ�ͳ��˵���ָ��ıȽ���
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
	NodePtr root_{ nullptr };
};