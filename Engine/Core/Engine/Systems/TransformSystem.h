#pragma once
#include "Core/Engine/Components/TransformComponent.h"
#include "Core/Engine/ECS/ECS.h"

namespace Nui::Systems
{
	/**
	 * @brief ECS system that handles updating transform components
	 * @details This system adds the `Transform` component to every entity on creation and recalculates the world matrix
	 */
	class TransformSystem : public ECS::SystemBase,
							public ECS::EventSubscriber<ECS::Events::OnEntityCreate>,
							public ECS::EventSubscriber<ECS::Events::OnComponentAdd<Components::Transform>>
	{
	public:
		/**
		 * @brief Virtual default destructor
		 */
		virtual ~TransformSystem() = default;

		/**
		 * @brief Initializes the system and subscribes to events
		 * @param ctx Pointer to the ECS Context
		 */
		virtual void OnInit(ECS::Context* ctx) override;

		/**
		 * @brief Updates the system
		 * @param ctx Pointer to the ECS Context
		 * @param dt Delta time
		 */
		virtual void OnUpdate(ECS::Context* ctx, const F64 dt) override;

		/**
		 * @brief Shuts down the system and unsubscribes from events
		 * @param ctx Pointer to the ECS Context
		 */
		virtual void OnShutdown(ECS::Context* ctx) override;

		/**
		 * @brief Called when an entity is created
		 * @param ctx Pointer to the ECS Context
		 * @param event Event data
		 * @details Adds the `Transform` component to the entity
		 */
		virtual void OnEvent(ECS::Context* ctx, const ECS::Events::OnEntityCreate& event) override;

		/**
		 * @brief Called when a `Transform` component is added to an entity
		 * @param ctx Pointer to the ECS Context
		 * @param event Event data
		 */
		virtual void OnEvent(ECS::Context* ctx, const ECS::Events::OnComponentAdd<Components::Transform>& event) override;
	};
}