#include "Entity.h"

namespace Nui::ECS
{
	void Entity::RemoveAll()
	{
		for (auto& [typeIndex, containerBase] : m_components)
		{
			containerBase->OnRemove(this);
			containerBase.reset();
		}

		m_components.clear();
	}
}