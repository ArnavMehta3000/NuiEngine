#pragma once
#include "Core/Engine/Components/TransformComponent.h"
#include "Core/Engine/ECS/ECS.h"

namespace Nui::Systems
{
	namespace Events
	{
		/**
		 * @brief Event for the `TransformSystem` to force a recalculation of the world matrix
		 */
		struct ForceTransformRecalculation
		{ 
			/**
			 * @brief Whether to include dirty components in the recalculation (default: `true`)
			 * @details If set to `false`, only dirty components will be recalculated
			 */
			bool IncludeDirty = true; 
		};
	}

	/**
	 * @brief ECS system that handles updating transform components
	 * @details This system adds the `Transform` component to every entity on creation and recalculates the world matrix
	 */
	class TransformSystem : public ECS::SystemBase,
							NUI_EVENT_SUBSCRIBER(ECS::Events::OnEntityCreate),
							NUI_EVENT_SUBSCRIBER(ECS::Events::OnComponentAdd<Components::Transform>),
							NUI_EVENT_SUBSCRIBER(Events::ForceTransformRecalculation)
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
		NUI_DECLARE_EVENT(ECS::Events::OnEntityCreate);

		/**
		 * @brief Called when a `Transform` component is added to an entity
		 * @param ctx Pointer to the ECS Context
		 * @param event Event data
		 */
		NUI_DECLARE_EVENT(ECS::Events::OnComponentAdd<Components::Transform>);

		/**
		 * @brief Called when a `Nui::Systems::Events::ForceTransformRecalculation` event is triggered
		 * @param ctx Pointer to the ECS Context
		 * @param event Event data
		 */
		NUI_DECLARE_EVENT(Events::ForceTransformRecalculation);
	};
}