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

		template <class Key, class Value>
		struct RBKeyTreeNode
		{
			using RBKeyTreeNodePtr = std::shared_ptr<RBKeyTreeNode<Key, Value>>;
			RBKeyTreeNodePtr left;
			RBKeyTreeNodePtr right;
			RBKeyTreeNodePtr parent;
			Value value;
			Key key;
			NodeColor col;
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
			RBTreeNodePtr left;
			RBTreeNodePtr right;
			RBTreeNodePtr parent;
			Value value;
			NodeColor col;
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
			return left.value < right.value;
		}

		template <class Value>
		bool operator > (const std::shared_ptr<RBTreeNode<Value>>& left, const std::shared_ptr<RBTreeNode<Value>>& right)
		{
			return right < left;
		}

		template <class Value>
		bool operator == (const std::shared_ptr<RBTreeNode<Value>>& left, const std::shared_ptr<RBTreeNode<Value>>& right)
		{
			return left.value == right.value;
		}

		template <class Value>
		bool operator < (const std::shared_ptr<RBTreeNode<Value>>& left, const Value& right)
		{
			return left.value < right;
		}

		template <class Value>
		bool operator > (const std::shared_ptr<RBTreeNode<Value>>& left, const Value& right)
		{
			return  left.value > right;
		}

		template <class Value>
		bool operator == (const std::shared_ptr<RBTreeNode<Value>>& left, const Value& right)
		{
			return left.value == right;
		}

	}


}