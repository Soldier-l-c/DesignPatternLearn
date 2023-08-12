#pragma once
namespace Tree
{
	enum class TreeChildNode
	{
		Left = 0,
		Right
	};

	template <class Value>
	struct TreeNode
	{
		using NodePtr = std::shared_ptr<TreeNode<Value>>;
		NodePtr left;
		NodePtr right;
		Value value;
	};

	template <class Key, class Value>
	struct KeyTreeNode : TreeNode<Value>
	{
		using KeyNodePtr = std::shared_ptr<KeyTreeNode<Key, Value>>;
		Key key;
	};

	namespace RBTree
	{
		enum class NodeColor
		{
			Red = 0,
			Black
		};

		template<class NodePtr>
		struct InserLocation
		{
			NodePtr parent{ nullptr };
			TreeChildNode child_pos{ Tree::TreeChildNode::Left };
		};

		template <class Key, class Value>
		struct RBKeyTreeNode
		{
			using RBKeyTreeNodePtr = std::shared_ptr<RBKeyTreeNode<Key, Value>>;
			using InsLocaType = InserLocation<RBKeyTreeNodePtr>;

			RBKeyTreeNodePtr left{ nullptr };
			RBKeyTreeNodePtr right{ nullptr };
			RBKeyTreeNodePtr parent{ nullptr };
			Value value;
			Key key;
			NodeColor col{ NodeColor::Red };
			InsLocaType GetInsertLocation(const RBKeyTreeNodePtr& root)
			{
				RBKeyTreeNode<Key, Value>::InsLocaType res;
				auto cur_node = root;
				while (nullptr != cur_node)
				{
					res.parent = cur_node;
					if (cur_node->key > key)
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
		};

		template <class Key, class Value>
		bool operator < (const RBKeyTreeNode<Key, Value>& left, const RBKeyTreeNode<Key, Value>& right)
		{
			return left.key < right.key;
		}

		template <class Key, class Value>
		bool operator > (const RBKeyTreeNode<Key, Value>& left, const RBKeyTreeNode<Key, Value>& right)
		{
			return right < left;
		}

		template <class Key, class Value>
		bool operator == (const RBKeyTreeNode<Key, Value>& left, const RBKeyTreeNode<Key, Value>& right)
		{
			return left.key == right.key;
		}

		template <class Key, class Value>
		bool operator < (const std::shared_ptr<RBKeyTreeNode<Key, Value>>& left, const std::shared_ptr<RBKeyTreeNode<Key, Value>>& right)
		{
			return left->key < right->key;
		}

		template <class Key, class Value>
		bool operator > (const std::shared_ptr<RBKeyTreeNode<Key, Value>>& left, const std::shared_ptr<RBKeyTreeNode<Key, Value>>& right)
		{
			return right < left;
		}

		template <class Key, class Value>
		bool operator == (const std::shared_ptr<RBKeyTreeNode<Key, Value>>& left, const std::shared_ptr<RBKeyTreeNode<Key, Value>>& right)
		{
			return left->key == right->key;
		}

		template <class Key, class Value>
		bool operator < (const std::shared_ptr<RBKeyTreeNode<Key, Value>>& left, const Key& right)
		{
			return left->key < right;
		}

		template <class Key, class Value>
		bool operator > (const std::shared_ptr<RBKeyTreeNode<Key, Value>>& left, const Key& right)
		{
			return left->key > right;
		}

		template <class Key, class Value>
		bool operator == (const std::shared_ptr<RBKeyTreeNode<Key, Value>>& left, const Key& right)
		{
			return left->key == right;
		}

		template <class Value>
		struct RBTreeNode
		{
			using RBTreeNodePtr = std::shared_ptr<RBTreeNode<Value>>;
			using InsLocaType = InserLocation<RBTreeNodePtr>;

			RBTreeNodePtr left{ nullptr };
			RBTreeNodePtr right{ nullptr };
			RBTreeNodePtr parent{ nullptr };
			Value value;
			NodeColor col{ NodeColor::Red };

			InsLocaType GetInsertLocation(const RBTreeNodePtr& root)
			{
				InsLocaType res;
				auto cur_node = root;
				while (nullptr != cur_node)
				{
					res.parent = cur_node;
					if (cur_node->value > value)
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
		};

		template <class Value>
		bool operator < (const RBTreeNode<Value>& left, const RBTreeNode<Value>& right)
		{
			return left.value < right.value;
		}

		template <class Value>
		bool operator > (const RBTreeNode<Value>& left, const RBTreeNode<Value>& right)
		{
			return right < left;
		}

		template <class Value>
		bool operator == (const RBTreeNode<Value>& left, const RBTreeNode<Value>& right)
		{
			return left.value == right.value;
		}
		
		template <class Value>
		bool operator < (const std::shared_ptr<RBTreeNode<Value>>& left, const std::shared_ptr<RBTreeNode<Value>>& right)
		{
			return left->value < right->value;
		}

		template <class Value>
		bool operator > (const std::shared_ptr<RBTreeNode<Value>>& left, const std::shared_ptr<RBTreeNode<Value>>& right)
		{
			return right < left;
		}

		template <class Value>
		bool operator == (const std::shared_ptr<RBTreeNode<Value>>& left, const std::shared_ptr<RBTreeNode<Value>>& right)
		{
			return left->value == right->value;
		}

		template <class Value>
		bool operator < (const std::shared_ptr<RBTreeNode<Value>>& left, const Value& right)
		{
			return left->value < right;
		}

		template <class Value>
		bool operator > (const std::shared_ptr<RBTreeNode<Value>>& left, const Value& right)
		{
			return  left->value > right;
		}

		template <class Value>
		bool operator == (const std::shared_ptr<RBTreeNode<Value>>& left, const Value& right)
		{
			return left->value == right;
		}
	}

}