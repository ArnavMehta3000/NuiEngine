#pragma once
#include "Core/Engine/Components/TransformComponent.h"
#include "Core/Engine/ECS/ECS.h"

namespace Nui::Systems
{
	namespace Events
	{
		/**
		 * @brief Event that is triggered at the end of the frame
		 */
		struct EndFrame {};
	}

	class RenderSystem : public ECS::SystemBase,
						 NUI_EVENT_SUBSCRIBER(Events::EndFrame)
	{
	public:
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

	private:
		/**
		 * @brief Called when a `Nui::Systems::Events::EndFrame` event is triggered
		 * @details This function is called when a `Nui::Systems::Events::EndFrame` event is triggered at the end of the frame
		 * @param ctx Pointer to the ECS Context
		 * @param event Event data
		 */
		NUI_DECLARE_EVENT(Events::EndFrame);

		/**
		 * @brief This function is called at the end of the frame to present to the screen
		 */
		void OnEndFrame();
	};
}