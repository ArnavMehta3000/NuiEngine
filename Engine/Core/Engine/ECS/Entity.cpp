#include "Entity.h"

namespace Nui::ECS
{
	void Entity::RemoveAll()
	{
		for (auto& [typeIndex, containerBase] : m_components)
		{
			containerBase->OnRemove(this);
			containerBase->OnDestroy(m_context);
		}

		m_components.clear();
	}
}