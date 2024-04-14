#include "SystemManager.h"

namespace Nui::ECS
{
	void SystemManager::AddSystem(std::unique_ptr<SequentialSystemBase> system)
	{
		m_systems.push_back(std::move(system));
	}

	void SystemManager::AddSystem(std::unique_ptr<HierarchicalSystem> system)
	{
		m_hierarchicalSystems.push_back(std::move(system));
	}

	void SystemManager::Update(Components::Node& node, F64 dt)
	{
		for (auto& sys : m_hierarchicalSystems)
		{
			HierarchicalSystem* system = static_cast<HierarchicalSystem*>(sys.get());
			system->BindRoot(node);
			system->Update(dt);
		}
	}

	void SystemManager::Update(typename EntityManager::iterator begin, typename EntityManager::iterator end, F64 dt)
	{
		for (auto& sys : m_systems)
		{
			SequentialSystem<>* seqSys = static_cast<SequentialSystem<>*>(sys.get());
			seqSys->BindIterator(begin, end);
			sys->Update(dt);
		}
	}
}