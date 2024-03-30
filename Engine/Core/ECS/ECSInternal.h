#pragma once
#include "Core/Common/CommonHeaders.h"
#include <bitset>
#include <type_traits>

namespace Nui::ECS
{
    namespace Internal // Early constants declaration
    {
        constexpr U64 MAX_COMPONENTS  = 64;
        constexpr U64 MAX_ENTITIES = 1000000;
    }

    using EntityIndex   = U32;
    using EntityVersion = U32;
    using EntityId      = U64;  // Top 32 bits have the index and the bottom 32 bits have the version
    using ComponentId   = U64;
    using ComponentMask = std::bitset<Internal::MAX_COMPONENTS>;

    namespace Internal
    {
        extern ComponentId s_componentCounter;

        template <typename T>
        ComponentId GetId()
        {
            static ComponentId s_componentId = Internal::s_componentCounter++;
            return s_componentId;
        }

        EntityId CreateEntityId(EntityIndex index, EntityVersion version);
        EntityIndex GetEntityIndex(EntityId id);
        EntityVersion GetEntityVersion(EntityId id);
        bool IsEntityIdValid(EntityId id);
    }

#define NUI_INVALID_ENTITY Nui::ECS::Internal::CreateEntityId(Nui::ECS::EntityIndex(-1), 0)
}