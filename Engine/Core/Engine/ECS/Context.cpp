#include "Context.h"

namespace Nui::ECS
{
	void Context::Update(F64 dt)
	{
		m_systemManager.Update(m_entityManager.begin(), m_entityManager.end(), dt);
	}
}
