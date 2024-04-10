#include "Node.h"

namespace Nui::Components
{
	void Node::AddChild(NodePtr child)
	{
		m_children.push_back(child);
		child->m_parent = this;
	}

	void Node::RemoveChild(NodePtr child)
	{
		m_children.erase(
			std::remove(m_children.begin(), m_children.end(), child), 
			m_children.end()
		);
	}

	void Node::RemoveAllChildren()
	{
		m_children.clear();
	}

	void Node::DetachFromParent()
	{
		Node* parent = m_parent;
		const auto& children = parent->m_children;

		std::shared_ptr<Node> self = nullptr;

		for (auto& ptr : children)
		{
			if (*ptr == *this)
			{
				self = ptr;
				break;
			}
		}

		if (self)
		{
			parent->RemoveChild(self);
		}
	}

	void Node::Visit(std::function<bool(Node&)> fn)
	{
		std::vector<Node*> nodes = { this };

		while (!nodes.empty())
		{
			Node* node = nodes.back();
			nodes.pop_back();

			bool continueTraversal = fn(*node);

			if (!continueTraversal || node->ShouldPrune())
			{
				continue;
			}

			auto& children = node->GetChildren();
			for (const NodePtr& child : children)
			{
				nodes.push_back(child.get());
			}
		}
	}
}