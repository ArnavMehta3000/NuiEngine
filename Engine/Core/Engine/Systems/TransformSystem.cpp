#include "TransformSystem.h"

namespace Nui::Systems
{
	void TransformSystem::OnInit(ECS::Context* ctx)
	{
		NUI_LOG(Debug, TransformSystem, "TransformSystem::OnInit");

		NUI_SUBSCRIBE_EVENT(ctx, ECS::Events::OnEntityCreate);
		NUI_SUBSCRIBE_EVENT(ctx, ECS::Events::OnComponentAdd<Components::Transform>);
		NUI_SUBSCRIBE_EVENT(ctx, Events::ForceTransformRecalculation);
	}

	void TransformSystem::OnUpdate(ECS::Context* ctx, const F64 dt)
	{
		ctx->Each<Components::Transform>(
			[&](ECS::Entity* e, ECS::ComponentHandle<Components::Transform> transform)
			{
				if (transform->IsDirty())
				{
					transform->RecalculateWorldMatrix();
				}
			});
	}

	void TransformSystem::OnShutdown(ECS::Context* ctx)
	{
		NUI_LOG(Debug, TransformSystem, "TransformSystem::OnShutdown");
		NUI_UNSUBSCRIBE_EVENTS_ALL(ctx);
	}

	NUI_DEFINE_EVENT(TransformSystem, ECS::Events::OnEntityCreate)
	{
		NUI_LOG(Debug, TransformSystem, "TransformSystem::OnEvent - OnEntityCreate");
		
		// Sanity check
		if (event.Entity->Has<Components::Transform>())
		{
			return;
		}

		// Add a transform to every entity on creation
		auto transform = event.Entity->Add<Components::Transform>(Components::Transform());

		NUI_ASSERT(transform.IsValid(), "TransformSystem::OnEvent - OnEntityCreate: Invalid transfrom added");

	}

	NUI_DEFINE_EVENT(TransformSystem, ECS::Events::OnComponentAdd<Components::Transform>)
	{
		NUI_LOG(Debug, TransformSystem, "TransformSystem::OnEvent - OnComponentAdd<Transform>");
		
		// Recalculate world matrices on creation
		event.Component->RecalculateWorldMatrix();
	}

	NUI_DEFINE_EVENT(TransformSystem, Events::ForceTransformRecalculation)
	{
		ctx->Each<Components::Transform>(
			[&](ECS::Entity* e, ECS::ComponentHandle<Components::Transform> transform)
			{
				if (event.IncludeDirty)  // Update all
				{
					transform->RecalculateWorldMatrix();
				}
				else  // Update only dirty
				{
					if (transform->IsDirty())
					{
						transform->RecalculateWorldMatrix();
					}
				}
			});
	}
}
