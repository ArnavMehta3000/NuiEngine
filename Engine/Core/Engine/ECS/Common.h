#pragma once
#include "Core/Common/CommonHeaders.h"
#include <type_traits>
#include <typeinfo>
#include <typeindex>

// Contains ECS forward declarations

namespace Nui::ECS
{
	using TypeIndex = std::type_index;

	class Context;
	class Entity;
	class SystemBase;

	namespace Internal
	{
		class EntityView;
		class EventSubscriberBase;
		template<typename... Types> class EntityComponentView;
	}

	template <typename T> class ComponentHandle;
	template <typename T> class EventSubscriber;

	template <typename T>
	constexpr TypeIndex GetTypeIndex()
	{
		return TypeIndex(typeid(T));
	}
}