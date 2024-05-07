#include "Context.h"
#include "Core/Engine/ECS/System.h"

namespace Nui::ECS
{
	void Context::Tick(const F64 dt)
	{
		//CleanUp();

		for (auto* system : m_systems)
		{
			system->OnUpdate(this, dt);
		}
	}
}