#pragma once
#include "Core/Engine/ECS/System.h"

namespace Nui::ECS
{
	class SystemManager final
	{
	public:
		SystemManager() = default;
		SystemManager(const SystemManager&) = delete;
		SystemManager(SystemManager&&) = default;
		~SystemManager() = default;

		SystemManager& operator=(const SystemManager&) = delete;
		SystemManager& operator=(SystemManager&&) = default;

		void AddSequentialSystem(std::unique_ptr<SequentialSystemBase> system);
		void AddHierarchicalSystem(std::unique_ptr<HierarchicalSystem> system);

		void Run(Components::Node& node);
		void Run(typename EntityManager::iterator begin, typename EntityManager::iterator end);

	private:
		std::vector<std::unique_ptr<ISystem>> m_systems;
		std::vector<std::unique_ptr<HierarchicalSystem>> m_hierarchicalSystems;
	};
}