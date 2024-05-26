#include "World.h"
#include "Core/Engine/ECS/ECS.h"
#include "Core/Engine/Systems/TransformSystem.h"
#include "Core/Input/Input.h"

namespace Nui
{
	World::World()
		: Context()
		, m_transformSystem(nullptr)
	{
	}

	World::~World() = default;

	void World::OnInit()
	{
		m_transformSystem = Context::RegisterSystem<Systems::TransformSystem>();
		NUI_ASSERT(m_transformSystem, "Failed to regsiter transform system for world!");
	}

	void World::Update(const F64 dt)
	{
		if (Input::GetKeyState(Input::KeyCode::A).IsHeld)
		{
			NUI_LOG(Info, World, "Created Entity");
			Context::CreateEntity();
		}

		if (Input::GetKeyState(Input::KeyCode::B).IsHeld)
		{
			auto view = Context::Each<Components::Transform>();
			for (ECS::Entity* e : view)
			{
				if (auto tf = e->Get<Components::Transform>())
				{
					tf->SetPosition(Math::Vector3::One);
				}
			}
		}


		// Update ECS
		Context::ClearPending();
		Context::Tick(dt);
	}

	void World::OnShutdown()
	{
		Context::UnregisterSystem<Systems::TransformSystem>();
	}
}