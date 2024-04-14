#pragma once
#include "Core/Engine/ECS/Context.h"

namespace Nui
{
	class World final : public ECS::Context
	{
	public:
		World();
		World(const World&) = delete;
		World(World&&) = delete;
		~World() = default;

		World operator=(const World&) = delete;
		World operator=(World&&) = delete;

		inline void SetRoot(Components::Node::NodePtr root) { m_rootNode = root; }
		inline Components::Node::NodePtr GetRoot() const { return m_rootNode; }

		void Update(F64 dt);

		inline void SetActive(bool active) { m_isActive = active; }
		bool IsActive() const { return m_isActive; }

	private:
		Components::Node::NodePtr m_rootNode;
		bool m_isActive;
	};
}