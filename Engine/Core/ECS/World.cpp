#include "World.h"

namespace Nui::ECS
{
    EntityId World::CreateEntity()
    {
        using namespace Nui::ECS::Internal;
        // If we have any free index, then create a new entity from that
        if (!m_freeEntities.empty())
        {
            EntityIndex newIndex = m_freeEntities.back();
            m_freeEntities.pop_back();

            EntityVersion newVersion = GetEntityVersion(m_entities[newIndex].Id);
            EntityId newEntity = CreateEntityId(newIndex, newVersion);

            m_entities[newIndex].Id = newEntity;
            return newEntity;
        }

        // No index was free, create a new entity
        m_entities.emplace_back(Internal::EntityInfo
        {
            Internal::CreateEntityId(EntityIndex(m_entities.size()), 0),
            ComponentMask(),
        });

        // Return the recently added entity id
        return m_entities.back().Id;
    }

    void World::MarkForDelete(EntityId id)
    {
        m_markedForDelete.push_back(id);
    }

    void World::DestroyEntities()
    {
        using namespace Nui::ECS::Internal;

        bool deletedAny = false;
        for (EntityId id : m_markedForDelete)
        {
            deletedAny = true;

            EntityIndex currentIndex = GetEntityIndex(id);
            // Invalidate current index and increment version number
            m_entities[currentIndex].Id = CreateEntityId(EntityIndex(-1), GetEntityVersion(id) + 1);
            // Clear component mask
            m_entities[currentIndex].Mask.reset();

            m_freeEntities.push_back(GetEntityIndex(id));
        }

        if (deletedAny)
        {
            m_markedForDelete.clear();
        }
    }
}