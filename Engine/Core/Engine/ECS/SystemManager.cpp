#include "SystemManager.h"

namespace Nui::ECS
{
	void SystemManager::AddSequentialSystem(std::unique_ptr<SequentialSystemBase> system)
	{
		m_systems.push_back(std::move(system));
	}

	void SystemManager::AddHierarchicalSystem(std::unique_ptr<HierarchicalSystem> system)
	{
		m_hierarchicalSystems.push_back(std::move(system));
	}

	void SystemManager::Run(Components::Node& node)
	{
		for (auto& sys : m_systems)
		{
			HierarchicalSystem* system = static_cast<HierarchicalSystem*>(sys.get());
			system->BindRoot(node);
			system->Run();
		}
	}

	void SystemManager::Run(typename EntityManager::iterator begin, typename EntityManager::iterator end)
	{
		for (auto& sys : m_systems)
		{
			SequentialSystem<>* seqSys = static_cast<SequentialSystem<>*>(sys.get());
			seqSys->BindIterator(begin, end);
			sys->Run();
		}
	}
}