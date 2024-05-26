#pragma once
#include "Core/Engine/ECS/Common.h"

namespace Nui::ECS
{
	namespace Internal
	{
		/**
		 * @brief Abstract base class for component containers
		 */
		struct ComponentContainerBase
		{
			/**
			 * @brief Virtual destructor
			 */
			virtual ~ComponentContainerBase() = default;

			/**
			 * @brief Pure virtual method called when an entity is removed
			 * @param entity The entity that was removed
			 */
			virtual void OnRemove(Entity* entity) = 0;
		};

		/**
		 * @brief Template class for component containers (concrete implementation of ComponentContainerBase)
		 * @tparam T The stored component data
		 */
		template <typename T>
		struct ComponentContainer : public ComponentContainerBase
		{
			T m_data;

			ComponentContainer() = default;
			ComponentContainer(const T& data) : m_data(data) {}

		protected:
			/**
			 * @brief Method called when an entity is removed
			 * @param entity The entity that was removed
			 */
			virtual void OnRemove(Entity* entity) override;
		};
	}

	/**
	 * @brief This class provides a handle to a component
	 * @tparam T The type of stored component data
	 */
	template <typename T>
	class ComponentHandle
	{
	public:
		ComponentHandle() : m_component(nullptr) {}
		ComponentHandle(T* component) : m_component(component) {}

		/**
		 * @brief Checks if the component is valid
		 * @return True if the component is valid
		 */
		bool IsValid() const noexcept { return m_component != nullptr; }

		/**
		 * @brief Get the component
		 * @return Reference to the component
		 */
		T& Get() const noexcept { return *m_component; }

		/**
		 * @brief Get the component
		 * @return Pointer to the component
		 */
		T* operator->() const { return m_component; }

		/**
		 * @brief Implicit conversion to bool to check if the component is valid
		 */
		operator bool() const { return IsValid(); }

	private:
		/**
		 * @brief Pointer to the component data from the container
		 */
		T* m_component;
	};
}