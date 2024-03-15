#include "GameEntity.h"
#include "Core/Component/TransformComponent.h"

namespace Nui::GameEntity
{
	namespace // Anonymous namespace
	{
		std::vector<Transform::Component> s_transforms;
		std::vector<Id::GenerationType>   s_generations;
		std::deque<EntityId>              s_freeIds;
	}  // Anonymous namespace

	Entity Create(const InitInfo& info)
	{
		NUI_ASSERT(info.Transform, "All entities must have a Transform component");
		
		if (!info.Transform)
		{
			return Entity();
		}

		EntityId id;
		if (s_freeIds.size() > Id::MinDeletedCount)
		{
			// Get the first free slot
			id = s_freeIds.front();
			NUI_ASSERT(!IsAlive(Entity(id)), "Entity (free id) is already alive");

			// Remove it from the free list and increment the generation
			s_freeIds.pop_front();
			id = EntityId(Id::NewGeneration(id));

			++s_generations[Id::Index(id)];
		}
		else
		{
			id = EntityId(Id::Type(s_generations.size()));
			s_generations.push_back(0);

			// Add default transform
			s_transforms.emplace_back();
		}

		const Entity entity = Entity(id);
		const Id::Type index = Id::Index(id);

		// Create transform component
		NUI_ASSERT(!s_transforms[index].IsValid(), "Transform component already exists");
		s_transforms[index] = Transform::Create(*info.Transform, entity);
		if (!s_transforms[index].IsValid())
		{
			// Invalid transform return invalid entity
			return Entity();
		}

		return entity;
	}

	void Destroy(Entity e)
	{
		const EntityId id = e.GetId();
		const Id::Type index = Id::Index(id);

		NUI_ASSERT(IsAlive(e), "Trying to destroy already destroyed entity");
		
		// Remove transform
		if (IsAlive(e))
		{
			Transform::Destroy(s_transforms[index]);
			s_transforms[index] = Transform::Component();  // Replace with default transform
			s_freeIds.push_back(id);
		}
	}

	bool IsAlive(Entity e)
	{
		NUI_ASSERT(e.IsValid(), "Invalid entity");

		const EntityId id = e.GetId();
		const Id::Type index = Id::Index(id);

		NUI_ASSERT(index < s_generations.size(), "Entity index out of generation bounds");
		NUI_ASSERT(s_generations[index] == Id::Generation(id), "Entity generation id mismatch");

		return s_generations[index] == Id::Generation(id)
			&& s_transforms[index].IsValid();
	}

	// Implemented from API/Entity.h
	Transform::Component Entity::GetTransform() const
	{
		NUI_ASSERT(IsAlive(*this), "Entity is not alive");
		const Id::Type index = Id::Index(m_id);
		NUI_ASSERT(s_generations[index] == Id::Generation(m_id), "Entity generation id mismatch");

		return s_transforms[index];
	}
}