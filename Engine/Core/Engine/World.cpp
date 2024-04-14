#include "World.h"
#include "Core/Engine/Systems/TransfromSystem.h"

namespace Nui
{
	World::World()
		: m_isActive(true)
	{
		GetSystemManager().AddSystem(std::make_unique<Systems::TransformSystem>());
	}

	void World::Update(F64 dt)
	{
		if (!m_isActive)
		{
			return;
		}

		ECS::Context::Update(dt);

		if (m_rootNode)
		{
			GetSystemManager().Update(*m_rootNode, dt);
		}
	}
}