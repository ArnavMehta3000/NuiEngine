#pragma once
#include "Core/Component/Common.h"
#include "Core/Component/API/Transform.h"

namespace Nui
{
#define NUI_INIT_INFO(name) namespace name { struct InitInfo; }
	NUI_INIT_INFO(Transform);
#undef NUI_INIT_INFO


	namespace GameEntity
	{
		struct InitInfo
		{
			Transform::InitInfo* Transform{ nullptr };
		};

		Entity Create(const InitInfo& info);
		void Destroy(Entity e);
		bool IsAlive(Entity e);
	}
}