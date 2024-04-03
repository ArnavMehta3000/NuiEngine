#include "Entity.h"

namespace Nui::ECS
{
	Entity::Entity()
		: m_id(entt::null)
		, m_world(nullptr)
	{
	}

	Entity::Entity(entt::entity id, World* world)
		: m_id(id)
		, m_world(world)
	{
	}

	bool Entity::IsValid() const
	{
		NUI_ASSERT(m_world, "Failed to check if entity is valid, world is nullptr");
		return m_world->m_registry.valid(m_id);
	}
}
