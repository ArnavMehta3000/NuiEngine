#include "Universe.h"

namespace Nui::ECS
{
	Universe::Universe()
		: m_activeWorld(nullptr)
	{
	}

	Universe::~Universe()
	{
		DestroyActiveWorld();
	}

	void Universe::Tick(F64 dt)
	{
		if (m_activeWorld)
		{
			m_activeWorld->PreUpdate(dt);
			m_activeWorld->OnUpdate(dt);
			m_activeWorld->PostUpdate(dt);
		}
	}
}