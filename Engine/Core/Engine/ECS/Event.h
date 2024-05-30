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

// Use this macro to inherit from Nui::ECS::EventSubscriber<Type>
#define NUI_EVENT_SUBSCRIBER(Type) public ECS::EventSubscriber<Type>

// Use this macro to declare an event function in header files (needs use of NUI_EVENT_SUBSCRIBER macro with same type)
#define NUI_DECLARE_EVENT(Type) virtual void OnEvent(Nui::ECS::Context* ctx, const Type& event) override

// Use this macro to define an event function in source files (needs use of NUI_DECLARE_EVENT macro)
#define NUI_DEFINE_EVENT(Class, Type) void Class::OnEvent(Nui::ECS::Context* ctx, const Type& event)

// Use this macro to subscribe to an event function (needs use of NUI_DECLARE_EVENT & NUI_EVENT_SUBSCRIBER macro)
#define NUI_SUBSCRIBE_EVENT(ECSContext, Type) ECSContext->SubscribeEvent<Type>(this)

// Use this macro to unsubscribe from an event function (needs use of NUI_DECLARE_EVENT & NUI_EVENT_SUBSCRIBER macro)
#define NUI_UNSUBSCRIBE_EVENT(ECSContext, Type) ECSContext->UnsubscribeEvent<Type>(this)

// Use this macro to unsubscribe from all events - use in destructor/shutdown functions
#define NUI_UNSUBSCRIBE_EVENTS_ALL(ECSContext) ECSContext->UnsubscribeAll(this)