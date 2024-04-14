#include "System.h"

namespace Nui::ECS
{
	void HierarchicalSystem::Update(F64 dt)
	{
		auto& root = GetRoot();
		auto& callback = GetCallback();

		OnInit();
		root.Visit(callback, dt);
		Shutdown();
	}
}