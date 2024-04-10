#include "System.h"

namespace Nui::ECS
{
	void HierarchicalSystem::Run()
	{
		auto& root = GetRoot();
		auto& result = GetCallback();

		Init();
		root.Visit(result);
		Shutdown();
	}
}