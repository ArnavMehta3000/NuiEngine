#pragma once
#include "Core/Engine/ECS/System.h"

namespace Nui::Systems
{
	class TransformSystem : public ECS::HierarchicalSystem
	{
	public:
		TransformSystem();
		bool operator()(Components::Node& node);
	};
}