#pragma once
#include "Core/Engine/ECS/Common.h"

namespace Nui::ECS
{
	namespace Internal
	{
		/**
		 * @brief Base class for all event subscribers usually systems
		 */
		class EventSubscriberBase
		{
		public:
			/**
			 * @brief Virtual default destructor
			 */
			virtual ~EventSubscriberBase() = default;
		};
	}

	/**
	 * @brief Base class for all event subscribers
	 * @tparam T Type of event
	 */
	template <typename T>
	class EventSubscriber : public Internal::EventSubscriberBase
	{
	public:
		/**
		 * @brief Virtual default destructor
		 */
		virtual ~EventSubscriber() = default;

		/**
		 * @brief Pure virtual function for handling event callback
		 * @param context Pointer to the ECS Context
		 * @param event Const reference to the type of event
		 */
		virtual void OnEvent(Context* context, const T& event) = 0;
	};

	/**
	 * @brief Namespace for all internal ECS events
	 */
	namespace Events
	{
		/**
		 * @brief Event for entity creation
		 */
		struct OnEntityCreate
		{
			/**
			 * @brief Pointer to the created entity
			 */
			Entity* Entity;
		};

		/**
		 * @brief Event for entity destruction
		 */
		struct OnEntityDestroy
		{
			/**
			 * @brief Pointer to the destroyed entity
			 */
			Entity* Entity;
		};

		/**
		 * @brief Event for when component is added
		 * @tparam T Type of component
		 */
		template <typename T>
		struct OnComponentAdd
		{
			/**
			 * @brief Pointer to the entity
			 */
			Entity* Entity;

			/**
			 * @brief Handle to the component
			 */
			ComponentHandle<T> Component;
		};

		/**
		 * @brief Event for when component is removed
		 * @tparam T Type of component
		 */
		template <typename T>
		struct OnComponentRemove
		{
			/**
			 * @brief Pointer to the entity
			 */
			Entity* Entity;

			/**
			 * @brief Handle to the component
			 */
			ComponentHandle<T> Component;
		};
	}
}