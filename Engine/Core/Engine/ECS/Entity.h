#pragma once
#include "Core/Engine/ECS/ComponentVector.h"
#include <set>

namespace Nui::ECS
{
	class Entity final
	{
	public:
		using Id = U64;
		using iterator = std::multiset<Component::Id>::iterator;

		Entity();
		Entity(const Entity& other);
		Entity(Entity&& other) = default;
		~Entity() = default;

		Entity& operator=(const Entity& other) = delete;
		Entity& operator=(Entity&& other) = default;

		inline Id GetId() const { return m_id; }

		template <typename C = Component, typename = std::enable_if_t<std::is_base_of_v<Component, C>>>
		Entity& AddComponent(std::shared_ptr<C> comp)
		{
			comp->m_entity = this;
			const Component::Id id = GetTypeId<C>();

			if (comp->IsUnique())
			{
				m_components[id].clear();
			}

			m_components[id].push_back(std::static_pointer_cast<Component>(comp));
			m_componentIds.insert(id);

			return *this;
		}

		void RemoveComponent(Component::Id compId);
		void RemoveAllComponentsWithId(Component::Id compId);

		template <typename C = Component, typename = std::enable_if_t<std::is_base_of_v<Component, C>>>
		ComponentVector<C> Query() const
		{
			const Component::Id id = GetTypeId<C>();
			return QueryInternal<C>(id);
		}

		iterator begin() { return m_componentIds.begin(); }
		iterator end() { return m_componentIds.end(); }

	private:

		template <typename C = Component, typename = std::enable_if_t<std::is_base_of_v<Component, C>>>
		ComponentVector<C> QueryInternal(Component::Id compId) const
		{
			ComponentVector<C> result;

			auto it = m_componentIds.find(compId);
			if (it != m_componentIds.end())
			{
				const ComponentVector<>& cv = this->m_components.at(compId);
				result = cv;
			}

			return result;
		}

	private:
		using ComponentMap = std::unordered_map<Component::Id, ComponentVector<>>;

		Id                           m_id;
		ComponentMap                 m_components;
		std::multiset<Component::Id> m_componentIds;
	};
}