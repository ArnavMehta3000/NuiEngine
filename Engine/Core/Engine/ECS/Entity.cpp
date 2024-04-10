#include "Entity.h"

namespace Nui::ECS
{
	namespace  // Anonymous namespace
	{
		Entity::Id s_idCounter = 0;
	}  // Anonymous namespace

	Entity::Entity()
		: m_id(s_idCounter++)
	{
	}

	Entity::Entity(const Entity& other)
	{
		this->m_id = s_idCounter++;

		for (auto& [compId, vec] : other.m_components)
		{
			std::for_each(vec.begin(), vec.end(), 
			[this, compId](const std::shared_ptr<Component>& ptr)
			{
				this->m_components[compId].emplace_back(std::make_shared<Component>(*ptr.get()));
				this->m_components[compId].back()->m_entity = this;
			});
		}

		this->m_componentIds = other.m_componentIds;
	}

	void Entity::RemoveComponent(Component::Id compId)
	{
		for (auto& [id, vec] : m_components)
		{
			std::vector<ComponentVector<>::iterator> its;

			auto it = std::find_if(vec.begin(), vec.end(),
			[&compId](const std::shared_ptr<Component>& ptr)
			{
				return ptr->GetId() == compId;
			});

			if (it != vec.end())
			{
				(*it)->m_entity = nullptr;
				vec.erase(it);
				m_componentIds.erase(id);
			}
		}
	}

	void Entity::RemoveAllComponentsWithId(Component::Id compId)
	{
		if (m_components.contains(compId))
		{
			m_components[compId].clear();
		}
	}
}