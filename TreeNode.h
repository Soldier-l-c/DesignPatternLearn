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

			RBKeyTreeNodePtr left{ nullptr };
			RBKeyTreeNodePtr right{ nullptr };
			RBKeyTreeNodePtr parent{ nullptr };
			Value value;
			Key key;
			NodeColor col{ NodeColor::Red };
			int32_t Compare(const RBKeyTreeNodePtr& node)
			{
				return key > node->key ? 1 : (key == node->key ? 0 : -1);
			}

			int32_t Compare(const Key& ckey)
			{
				return key > ckey ? 1 : (key == ckey ? 0 : -1);
			}
		};

		template <class Value>
		struct RBTreeNode
		{
			using RBTreeNodePtr = std::shared_ptr<RBTreeNode<Value>>;

			RBTreeNodePtr left{ nullptr };
			RBTreeNodePtr right{ nullptr };
			RBTreeNodePtr parent{ nullptr };
			Value value;
			NodeColor col{ NodeColor::Red };

			int32_t Compare(const RBTreeNodePtr& node)
			{
				return value > node->value ? 1 : (value == node->value ? 0 : -1);
			}

			int32_t Compare(const Value& cvalue)
			{
				return value > cvalue ? 1 : (value == cvalue ? 0 : -1);
			}
		};
	}

}