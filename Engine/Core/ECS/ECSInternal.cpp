#include "ECSInternal.h"

namespace Nui::ECS
{
	ComponentId ECS::Internal::s_componentCounter = 0;

	EntityId Internal::CreateEntityId(EntityIndex index, EntityVersion version)
	{
		// Shift the index up 32 and put the version in the bottom
		return ((EntityId)index << 32) | ((EntityId)version);
	}

	EntityIndex Internal::GetEntityIndex(EntityId id)
	{
		// Shift down 32 so we lose the version and get our index
		return id >> 32;
	}

	EntityVersion Internal::GetEntityVersion(EntityId id)
	{
		// Cast to a 32 bit int to get our version number (loosing the top 32 bits)
		return (EntityVersion)id;
	}

	bool Internal::IsEntityIdValid(EntityId id)
	{
		// Check if the index is our invalid index
		return (id >> 32) != EntityIndex(-1);
	}
}