#include "World.h"
#include "Core/Engine/ECS/ECS.h"

namespace Nui
{
	World::World()
		: m_isActive(true)
	{
	}

	World::~World()
	{
	}
	void World::Update(const F64 dt)
	{
		if (!m_isActive)
		{
			return;
		}
	}
}