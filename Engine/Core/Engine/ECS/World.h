#pragma once
#include "Core/Engine/ECS/EntityManager.h"
#include "Core/Engine/ECS/SystemManager.h"

namespace Nui::ECS
{
	class World
	{
	public:
		inline EntityManager& GetEntityManager() { return m_entityManager; }
		inline SystemManager& GetSystemManager() { return m_systemManager; }

		void Update();

	private:
		EntityManager m_entityManager;
		SystemManager m_systemManager;
	};
}