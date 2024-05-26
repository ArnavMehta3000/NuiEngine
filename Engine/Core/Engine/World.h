#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/Engine/ECS/ECS.h"
#include "Core/Engine/Components/TransformComponent.h"

namespace Nui
{
	namespace Systems { class TransformSystem; }

	class World final : public ECS::Context
	{
		friend class AppBase;

	public:
		World();
		World(const World&) = delete;
		World(World&&) = delete;
		~World();

		World operator=(const World&) = delete;
		World operator=(World&&) = delete;

	private:
		void OnInit();
		void Update(const F64 dt);
		void OnShutdown();

	private:
		Systems::TransformSystem* m_transformSystem;
	};
}