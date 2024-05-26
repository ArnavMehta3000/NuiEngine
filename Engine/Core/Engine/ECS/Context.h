#pragma once
#include "Core/Engine/ECS/Common.h"
#include <concepts>

namespace Nui::ECS
{
	// Concept - is true when a class derives from SystemBase
	template <typename T>
	concept IsSystem = std::is_base_of<SystemBase, T>::value;

	/**
	* @brief The ECS context class manages entities, systems, and events in the ECS framework.
	*/
	class Context
	{
	public:
		/**
		 * @brief Unordered map of subscribers to array of events subscriber base pointers
		 */
		using SubscriberMap = std::unordered_map<TypeIndex, std::vector<Internal::EventSubscriberBase*>, std::hash<TypeIndex>, std::equal_to<TypeIndex>>;


	public:
		/**
		 * @brief Constructs a new ECS context.
		 */
		Context();

		/**
		 * @brief Destroys the ECS context.
		 */
		virtual ~Context();

		/**
		* @brief Gets the total count of entities in the ECS context.
		* @return The count of entities.
		*/
		inline U64 GetEntityCount() const noexcept { return m_entities.size(); }

		/**
		 * @brief Retrieves an entity by its unique identifier.
		 * @param id The unique identifier of the entity.
		 * @return A pointer to the entity, or nullptr if not found.
		 */
		Entity* GetEntityById(U64 id);

		/**
		 * @brief Retrieves an entity by its index.
		 * @param index The index of the entity.
		 * @return A pointer to the entity, or nullptr if out of bounds.
		 */
		Entity* GetEntityByIndex(U64 index);

		/**
		 * @brief Creates a new entity in the ECS context.
		 * @return A pointer to the newly created entity.
		 */
		Entity* CreateEntity();

		/**
		 * @brief Destroys an entity in the ECS context.
		 * @param e Pointer to the entity to destroy.
		 * @param immediate Flag indicating whether to destroy the entity immediately.
		 */
		void DestroyEntity(Entity* e, bool immediate = false);

		/**
		 * @brief Clears all entities pending destruction in the ECS context.
		 * @return True if any entities were cleared, false otherwise.
		 */
		bool ClearPending();

		/**
		 * @brief Resets the ECS context, clearing all entities but keeping the systems
		 */
		void Reset();

		/**
		 * @brief Registers a new system in the ECS context
		 * @tparam T Class derived from SystemBase to register as a system
		 * @tparam ...Args Arguments to pass to the constructor
		 * @param ...args Arguments to pass to the constructor
		 * @return A pointer to the newly created system
		 */
		template <typename T, typename... Args>
		T* RegisterSystem(Args&&... args) requires IsSystem<T>;

		/**
		 * @brief Unregisters a system from the ECS context
		 * @tparam T Class derived from SystemBase to unregister
		 */
		template <typename T>
		void UnregisterSystem() requires IsSystem<T>;

		template <typename T>
		T* GetSystem() requires IsSystem<T>;

		/**
		 * @brief Enables a system in the ECS context
		 * @tparam T Class derived from SystemBase to enable
		 */
		template <typename T>
		void EnableSystem() requires IsSystem<T>;

		/**
		 * @brief Disables a system in the ECS context
		 * @tparam T Class derived from SystemBase to disable
		 */
		template <typename T>
		void DisableSystem() requires IsSystem<T>;

		/**
		 * @brief Checks if a system is enabled in the ECS context
		 * @tparam T Class derived from SystemBase to check
		 * @return True if the system is enabled, false otherwise
		 */
		template <typename T>
		bool IsSystmEnabled() requires IsSystem<T>;

		/**
		 * @brief Subscribes to an event in the ECS context
		 * @tparam T Type of event to subscribe to
		 * @param subscriber A pointer to the subscriber (usually the system itself)
		 */
		template <typename T>
		void SubscribeEvent(EventSubscriber<T>* subscriber);

		/**
		 * @brief Unsubscribes from an event in the ECS context
		 * @tparam T Type of event to unsubscribe from
		 * @param subscriber A pointer to the subscriber (usually the system itself)
		 */
		template<typename T>
		void UnsubscribeEvent(EventSubscriber<T>* subscriber);

		/**
		 * @brief Un-subscribes from all events in the ECS context (usually called in the system destructor)
		 * @param subscriber A pointer to the subscriber (usually the system itself)
		 */
		void UnsubscribeAll(void* subscriber);

		/**
		 * @brief Emits an event in the ECS context to all subscribers
		 * @tparam T Type of event
		 * @param event Event data
		 */
		template<typename T>
		void EmitEvent(const T& event);

		/**
		 * @brief Iterates over all entities in the ECS context (used in ranged loops)
		 * @tparam ...Types Types of components
		 * @param includePendingDestroy Flag indicating whether to include entities pending destruction
		 * @return An entity component view
		 */
		template<typename... Types>
		Internal::EntityComponentView<Types...> Each(bool includePendingDestroy = false);

		/**
		 * @brief Iterates over all entities in the ECS context (using a lambda/function)
		 * @tparam ...Types Types of components
		 * @param viewFunc Function to call for each entity
		 * @param includePendingDestroy Flag indicating whether to include entities pending destruction
		 */
		template<typename... Types>
		void Each(typename std::common_type<std::function<void(Entity*, ComponentHandle<Types>...)>>::type viewFunc, bool includePendingDestroy = false);

		/**
		 * @brief Iterates over all entities in the ECS context (used in ranged loops)
		 * @param includePendingDestroy Flag indicating whether to include entities pending destruction
		 * @return An entity view
		 */
		Internal::EntityView All(bool includePendingDestroy = false);

		/**
		 * @brief Iterates over all entities in the ECS context (using a lambda/function)
		 * @param viewFunc Function to call for each entity
		 * @param includePendingDestroy Flag indicating whether to include entities pending destruction
		 */
		void All(std::function<void(Entity*)> viewFunc, bool includePendingDestroy = false);

		/**
		 * @brief Updates the ECS context (updates all the systems)
		 * @param dt Delta time
		 */
		void Tick(const F64 dt);

	private:
		/**
		 * @brief Array of unique pointers of entities in the ECS context
		 */
		std::vector<std::unique_ptr<Entity>> m_entities;

		/**
		 * @brief Array of unique pointers of systems in the ECS context
		 */
		std::vector<std::unique_ptr<SystemBase>> m_systems;

		/**
		 * @brief Map of subscribers in the ECS context
		 */
		SubscriberMap m_subscribers;

		/**
		 * @brief Last entity id
		 */
		U64 m_lastEntityId{ 0 };
	};
}