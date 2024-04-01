#include "World.h"
#include "Core/ECS/Entity.h"

namespace Nui::ECS
{
	World::~World()
	{
		m_registry.clear();
	}
	void World::OnConstruct()
	{
	}

	void World::PreUpdate(F64 dt)
	{
		if (m_entitiesToDestroy.size() > 0)
		{
			// Delete all queued entities before frame
			for (auto& entity : m_entitiesToDestroy)
			{
				m_registry.destroy(entity);
			}

			m_entitiesToDestroy.clear();
		}
	}

	void World::OnUpdate(F64 dt)
	{
	}

	void World::PostUpdate(F64 dt)
	{
	}

	void World::OnDestruct()
	{
	}

	Entity World::CreateEntity()
	{
		return Entity(entt::null, this);
	}

	void World::DestroyEntity(Entity entity)
	{
		m_entitiesToDestroy.push_back(entity);
	}

	void World::DestroyAllEntities()
	{
		m_registry.clear();
	}
}
