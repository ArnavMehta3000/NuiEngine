#include "World.h"
#include "Core/Engine/ECS/ECS.h"

namespace Nui
{
	World::World()
		: Context()
		, m_isActive(true)
	{
	}

	World::~World() = default;

	void World::Update(const F64 dt)
	{
		// Update ECS
		Context::ClearPending();
		Context::Tick(dt);

		NUI_LOG(Debug, World, "Updated world in ", dt, " seconds");
	}
}