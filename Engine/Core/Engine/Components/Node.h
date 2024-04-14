#pragma once
#include "Core/Engine/ECS/Component.h"

namespace Nui::Components
{
	class Node : public ECS::Component
	{
	public: 
		using NodePtr = std::shared_ptr<Node>;

	public:
		void AddChild(NodePtr child);
		void RemoveChild(NodePtr child);
		void RemoveAllChildren();
		void DetachFromParent();

		inline const Node* GetParent() const { return m_parent; }
		inline bool HasParent() const { return m_parent != nullptr; }
		inline bool ShouldPrune() const { return m_shouldPrune; }
		inline void Prune(bool enable) { m_shouldPrune = enable; }
		inline const std::vector<NodePtr> GetChildren() const { return m_children; }

		void Visit(std::function<bool(Node&, F64)> fn, F64 dt);

	private:
		std::vector<NodePtr> m_children;
		Node* m_parent = nullptr;
		bool m_shouldPrune{ false };
	};
}