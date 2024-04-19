#pragma once
#include "Core/Engine/ECS/Common.h"

namespace Nui::ECS
{
	namespace Internal
	{
		class EventSubscriberBase
		{
		public:
			virtual ~EventSubscriberBase() = default;
		};
	}

	template <typename T>
	class EventSubscriber : public Internal::EventSubscriberBase
	{
	public:
		virtual ~EventSubscriber() = default;
		virtual void OnReceiveEvent(Context* context, const T& event) = 0;
	};

	namespace Events
	{
		struct OnEntityCreate
		{
			Entity* Entity;
		};

		struct OnEntityDestroy
		{
			Entity* Entity;
		};

		template <typename T>
		struct OnComponentAdd
		{
			Entity* Entity;
			ComponentHandle<T> Component;
		};

		template <typename T>
		struct OnComponentRemove
		{
			Entity* Entity;
			ComponentHandle<T> Component;
		};
	}
}