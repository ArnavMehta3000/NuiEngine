#include "World.h"

namespace Nui::ECS
{
	void World::Update()
	{
		m_systemManager.Run(m_entityManager.begin(), m_entityManager.end());
	}
}
