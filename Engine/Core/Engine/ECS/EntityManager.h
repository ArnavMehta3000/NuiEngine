#pragma once
#include "Core/Engine/ECS/Entity.h"

namespace Nui::ECS
{
	class EntityManager final
	{
	public:
		using iterator = std::unordered_map<Component::Id, Entity>::iterator;

		EntityManager() = default;
		EntityManager(const EntityManager&) = delete;
		EntityManager(EntityManager&&) = default;
		~EntityManager() = default;

		EntityManager& operator=(const EntityManager&) = delete;
		EntityManager& operator=(EntityManager&&) = default;

		Entity& CreateEntity();
		void AddExisting(Entity&& e);
		void Destroy(Entity::Id id);
		void Destroy(const Entity& e);

		Entity* operator[](Entity::Id id);

		void Clean();  // Destroyed queued entities

		iterator begin() { return m_entities.begin(); }
		iterator end() { return m_entities.end(); }

	private:
		std::vector<Entity::Id>                m_toDestroy;
		std::unordered_map<Entity::Id, Entity> m_entities;
	};
}