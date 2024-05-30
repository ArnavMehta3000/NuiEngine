#pragma once
#include "Core/Engine/ECS/Common.h"

namespace Nui::ECS
{
	/**
	 * @brief Base class for all ECS systems
	 */
	class SystemBase
	{
	public:
		/**
		 * @brief Default virtual destructor
		 */
		virtual ~SystemBase() = default;

		/**
		 * @brief Called when the system is initialized
		 * @note Subscribe to event callbacks here
		 * @param ctx Pointer to the ECS context
		 */
		virtual void OnInit(Context* ctx) {}

		/**
		 * @brief Called when the system is updated (every frame)
		 * @param ctx Pointer to the ECS context
		 * @param dt Time since last frame
		 */
		virtual void OnUpdate(Context* ctx, const F64 dt) {}

		/**
		 * @brief Called when the system is shutdown
		 * @note Unsubscribe from event callbacks here
		 * @param ctx Pointer to the ECS context
		 */
		virtual void OnShutdown(Context* ctx) {}

		/**
		 * @brief Check if the system is enabled
		 * @return True if the system is enabled
		 */
		bool IsEnabled() const noexcept { return m_enabled; }

		/**
		 * @brief Set if the system is enabled
		 * @param enabled True if the system is enabled
		 */
		void SetIsEnabled(bool enabled) noexcept { m_enabled = enabled; }

	private:
		/**
		 * @brief Enabled flag for the system
		 */
		bool m_enabled{ true };
	};
}