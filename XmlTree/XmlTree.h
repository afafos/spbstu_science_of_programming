#pragma once

#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>

template<class T>
class XmlTree
{
	using ValuePtr = std::unique_ptr<T>;
	using TreePtr = std::unique_ptr<XmlTree<T>>;

	struct XmlNode
	{
		using NodePtr = std::shared_ptr<XmlNode>;
		using ParentPtr = std::weak_ptr<XmlNode>;

		XmlNode(ValuePtr value, ParentPtr parent) noexcept
			: m_value(std::move(value))
			, m_parent(parent)
		{
		}

		ValuePtr m_value;
		ParentPtr m_parent;
		std::vector<NodePtr> m_childs;
	};

	template<class U>
	class TreeIterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = U;
		using pointer = value_type*;
		using reference = value_type&;

		TreeIterator() = default;

		TreeIterator(const TreeIterator<T>& rhs) noexcept
			: m_node(rhs.m_node)
			, m_path(rhs.m_path)
		{
		}

		inline bool operator==(const TreeIterator& rhs) const noexcept
		{
			return m_node == rhs.m_node && m_path.size() == rhs.m_path.size()
				&& std::equal(m_path.cbegin(), m_path.cend(), rhs.m_path.cbegin());
		}

		inline bool operator!=(const TreeIterator& rhs) const noexcept
		{
			return !operator==(rhs);
		}

		inline reference operator*() const
		{
			if (!m_node)
			{
				throw std::runtime_error("End of tree is reached");
			}

			return *(m_node->m_value);
		}

		inline pointer operator->() const
		{
			if (!m_node)
			{
				throw std::runtime_error("End of tree is reached");
			}

			return m_node->m_value.get();
		}

		inline TreeIterator& operator++()
		{
			if (!m_node)
			{
				throw std::runtime_error("End of tree is reached");
			}

			if (!m_node->m_childs.empty())
			{
				auto childIt = m_node->m_childs.cbegin();
				m_node = *childIt;
				m_path.push_back(std::move(childIt));
			}
			else while (!m_path.empty())
			{
				auto& childIt = ++m_path.back();

				if (childIt != std::shared_ptr(m_node->m_parent)->m_childs.cend())
				{
					m_node = *childIt;
					break;
				}
				else
				{
					m_node = std::shared_ptr(m_node->m_parent);
					m_path.pop_back();
				}
			}

			if (m_path.empty())
			{
				m_node = nullptr;
			}

			return *this;
		}

		inline TreeIterator operator++(int)
		{
			auto it(*this);
			++(*this);
			return it;
		}

	private:
		friend class XmlTree;

		using NodePtrVectorIterator = std::vector<typename XmlNode::NodePtr>::const_iterator;

		explicit TreeIterator(
			XmlNode::NodePtr node,
			std::vector<NodePtrVectorIterator>&& path) noexcept
			: m_node(node)
			, m_path(path)
		{
		}

		XmlNode::NodePtr m_node;
		std::vector<NodePtrVectorIterator> m_path;
	};

public:
	XmlTree(const XmlTree&) = delete;
	XmlTree(XmlTree&&) = delete;

	XmlTree& operator=(const XmlTree&) = delete;
	XmlTree& operator=(XmlTree&&) = delete;

	static TreePtr Create() noexcept;

	using Iterator = TreeIterator<T>;
	Iterator begin() noexcept;
	Iterator end() noexcept;

	using ConstIterator = TreeIterator<const T>;
	ConstIterator cbegin() noexcept;
	ConstIterator cend() noexcept;

	Iterator Insert(ConstIterator pos, ValuePtr&& value);
	Iterator Find(const T& value) noexcept;
	bool Erase(ConstIterator pos);

private:
	XmlTree() = default;

	XmlNode::NodePtr m_rootNode;
};

template<class T>
inline XmlTree<T>::TreePtr XmlTree<T>::Create() noexcept
{
	return TreePtr(new XmlTree<T>());
}

template<class T>
inline XmlTree<T>::Iterator XmlTree<T>::begin() noexcept
{
	return Iterator{ m_rootNode, { } };
}

template<class T>
inline XmlTree<T>::Iterator XmlTree<T>::end() noexcept
{
	return { };
}

template<class T>
inline XmlTree<T>::ConstIterator XmlTree<T>::cbegin() noexcept
{
	return ConstIterator{ m_rootNode, { } };
}

template<class T>
inline XmlTree<T>::ConstIterator XmlTree<T>::cend() noexcept
{
	return { };
}

template<class T>
inline XmlTree<T>::Iterator XmlTree<T>::Insert(ConstIterator pos, ValuePtr&& value)
{
	Iterator result;

	if (!m_rootNode && pos == cend())
	{
		m_rootNode = std::make_shared<XmlTree<T>::XmlNode>(std::move(value), std::weak_ptr<XmlTree<T>::XmlNode>());
		result = Iterator{ m_rootNode, { } };
	}
	else
	{
		result = Iterator{ pos.m_node, std::move(pos.m_path) };
		result.m_node->m_childs.emplace_back(std::make_shared<XmlTree<T>::XmlNode>(std::move(value), pos.m_node));
		++result;
		result.m_node = std::shared_ptr(result.m_node->m_parent)->m_childs.back();
		result.m_path.back() = std::shared_ptr(result.m_node->m_parent)->m_childs.end() - 1;
	}

	return result;
}

template<class T>
inline XmlTree<T>::Iterator XmlTree<T>::Find(const T& value) noexcept
{
	auto result = end();

	for (auto it = begin(); it != end(); ++it)
	{
		if (*it == value)
		{
			result = it;
			break;
		}
	}

	return result;
}

template<class T>
inline bool XmlTree<T>::Erase(ConstIterator pos)
{
	if (pos == cend())
	{
		return false;
	}

	if (pos == cbegin())
	{
		m_rootNode = nullptr;
	}
	else
	{
		std::shared_ptr(pos.m_node->m_parent)->m_childs.erase(pos.m_path.back());
	}

	return true;
}
