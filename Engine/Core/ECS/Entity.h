#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/ECS/World.h"
#include "Core/External/Entt/entity/registry.hpp"

namespace Nui::ECS
{
	class World;

	class Entity
	{
	public:
		explicit Entity();
		explicit Entity(entt::entity id, World* world);

		operator entt::entity() const { return m_id; }
		World* GetWorld() const;
		bool IsValid() const;

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			NUI_ASSERT(m_world, "Failed to add component, world is nullptr");
			return m_world->m_registry.emplace<T>(m_id, std::forward<Args>(args)...);
		}

		template <typename T>
		void RemoveComponent()
		{
			NUI_ASSERT(m_world, "Failed to remove component, world is nullptr");
			m_world->m_registry.remove<T>(m_id);
		}

		template <typename T>
		T& GetComponent()
		{
			NUI_ASSERT(m_world, "Failed to get component, world is nullptr");
			return m_world->m_registry.get<T>(m_id);
		}

	private:
		entt::entity m_id;
		World* m_world;
	};
}