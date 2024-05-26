#include "TransformSystem.h"

namespace Nui::Systems
{
	void TransformSystem::OnInit(ECS::Context* ctx)
	{
		NUI_LOG(Debug, TransformSystem, "TransformSystem::OnInit");

		ctx->SubscribeEvent<ECS::Events::OnEntityCreate>(this);
		ctx->SubscribeEvent<ECS::Events::OnComponentAdd<Components::Transform>>(this);
	}

	void TransformSystem::OnUpdate(ECS::Context* ctx, const F64 dt)
	{
		NUI_LOG(Debug, TransformSystem, "TransformSystem::OnUpdate");

		ctx->Each<Components::Transform>(
			[&](ECS::Entity* e, ECS::ComponentHandle<Components::Transform> transform)
			{
				if (transform->IsDirty())
				{
					NUI_LOG(Debug, TransformSystem, "TransformSystem::OnUpdate - Recalculated world matrices");
					transform->RecalculateWorldMatrix();
				}
			});
	}

	void TransformSystem::OnShutdown(ECS::Context* ctx)
	{
		NUI_LOG(Debug, TransformSystem, "TransformSystem::OnShutdown");
		ctx->UnsubscribeAll(this);
	}

	void TransformSystem::OnEvent(ECS::Context* ctx, const ECS::Events::OnEntityCreate& event)
	{
		NUI_LOG(Debug, TransformSystem, "TransformSystem::OnEvent - OnEntityCreate - Added transfrom component");
		
		// Sanity check
		if (event.Entity->Has<Components::Transform>())
		{
			return;
		}

		// Add a transform to every entity on creation
		auto transform = event.Entity->Add<Components::Transform>(Components::Transform());

		NUI_ASSERT(transform.IsValid(), "TransformSystem::OnEvent<OnEntityCreate> - invalid transfrom added");

	}

	void TransformSystem::OnEvent(ECS::Context* ctx, const ECS::Events::OnComponentAdd<Components::Transform>& event)
	{
		NUI_LOG(Debug, TransformSystem, "TransformSystem::OnEvent - OnComponentAdd<Transform> - RecalculateWorldMatrix");
		
		// Recalculate world matrices on creation
		event.Component->RecalculateWorldMatrix();
	}
}
