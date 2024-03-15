#pragma once
#include "Core/Component/Common.h"

namespace Nui::Transform
{
	struct InitInfo
	{
		F32 Position[3] = { 0.0f, 0.0f, 0.0f };
		F32 Rotation[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		F32 Scale[3]    = { 1.0f, 1.0f, 1.0f };
	};

	Component Create(const InitInfo& info, GameEntity::Entity owner);
	void Destroy(Component id);
}