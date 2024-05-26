#pragma once
#include "Core/Engine/Components/TransformComponent.h"
#include "Core/Engine/ECS/ECS.h"

namespace Nui::Systems
{

	class TransformSystem : public ECS::SystemBase,
							public ECS::EventSubscriber<ECS::Events::OnEntityCreate>,
							public ECS::EventSubscriber<ECS::Events::OnComponentAdd<Components::Transform>>
	{
	public:
		virtual ~TransformSystem() = default;

		virtual void OnInit(ECS::Context* ctx) override;
		virtual void OnUpdate(ECS::Context* ctx, const F64 dt) override;
		virtual void OnShutdown(ECS::Context* ctx) override;

		virtual void OnEvent(ECS::Context* ctx, const ECS::Events::OnEntityCreate& event) override;
		virtual void OnEvent(ECS::Context* ctx, const ECS::Events::OnComponentAdd<Components::Transform>& event) override;
	};
}