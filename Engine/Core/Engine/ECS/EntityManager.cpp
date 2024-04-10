#include "EntityManager.h"

namespace Nui::ECS
{
	Entity& EntityManager::CreateEntity()
	{
		Entity e;
		Entity::Id id = e.GetId();
		m_entities[id] = std::move(e);

		return m_entities[id];
	}

	void EntityManager::AddExisting(Entity&& e)
	{
		m_entities[e.GetId()] = std::move(e);
	}

	void EntityManager::Destroy(Entity::Id id)
	{
		if (!m_entities.contains(id))
			return;

		m_toDestroy.push_back(id);
	}

	void EntityManager::Destroy(const Entity& e)
	{
		Destroy(e.GetId());
	}

	Entity* EntityManager::operator[](Entity::Id id)
	{
		if (!m_entities.contains(id))
			return nullptr;

		return &m_entities[id];
	}

	void EntityManager::Clean()
	{
		if (!m_toDestroy.empty())
		{
			std::for_each(m_toDestroy.begin(), m_toDestroy.end(),
				[this](Entity::Id id)
				{
					m_entities.erase(id);
				});

			m_toDestroy.clear();
		}
	}
}