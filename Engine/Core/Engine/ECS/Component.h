#pragma once
#include "Core/Engine/ECS/Common.h"

namespace Nui::ECS
{
	namespace Internal
	{
		struct ComponentContainerBase
		{
			virtual ~ComponentContainerBase() = default;

			virtual void OnRemove(Entity* entity) = 0;
			virtual void OnDestroy(Context* context) = 0;
		};

		template <typename T>
		struct ComponentContainer : public ComponentContainerBase
		{
			T m_data;

			ComponentContainer() = default;
			ComponentContainer(const T& data) : m_data(data) {}

		protected:
			virtual void OnRemove(Entity* entity);
			virtual void OnDestroy(Context* context);
		};
	}

	template <typename T>
	class ComponentHandle
	{
	public:
		ComponentHandle() : m_component(nullptr) {}
		ComponentHandle(T* component) : m_component(component) {}

		bool IsValid() const noexcept { return m_component != nullptr; }
		T& Get() const noexcept { return *m_component; }

		T* operator->() const { return m_component; }
		operator bool() const { return IsValid(); }

	private:
		T* m_component;
	};
}