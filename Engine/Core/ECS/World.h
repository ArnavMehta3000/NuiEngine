#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/External/Entt/entity/registry.hpp"

namespace Nui::ECS
{
	class Entity;
	class World
	{
		friend class Universe;
		friend class Entity;
	public:
		World() = default;
		virtual ~World();

		virtual void OnConstruct();
		virtual void OnUpdate(F64 dt);
		virtual void PostUpdate(F64 dt);
		virtual void OnDestruct();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void DestroyAllEntities();

		template <typename... Args>
		inline auto View()
		{
			return m_registry.view<Args...>();
		}

		inline auto AllEntities()
		{
			return m_registry.view<entt::entity>();
		}

	private:
		void PreUpdate(F64 dt);

	private:
		entt::registry m_registry;
		std::vector<entt::entity> m_entitiesToDestroy;
	};
}