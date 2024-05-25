#pragma once
#include "Core/Engine/ECS/Component.h"

namespace Nui::ECS
{
	/**
	 * @brief Class to represent an entity in an ECS Context
	 */
	class Entity
	{
		friend class Context;
	public:
		constexpr static U64 InvalidId = 0;
		using ComponentMap = std::unordered_map<TypeIndex, std::unique_ptr<Internal::ComponentContainerBase>>;

	public:
		/**
		 * @brief Constructs a new entity in the given ECS Context with the given id
		 * @param context Pointer to the ECS Context
		 * @param id ID of the entity
		 */
		Entity(Context* context, U64 id) : m_context(context), m_id(id) {}

		/**
		 * @brief Destroys the entity and all its components
		 */
		~Entity() { RemoveAll(); }

		/**
		 * @brief Gets the ECS context associated with the entity
		 * @return Pointer to the ECS context
		 */
		inline Context* GetContext() const noexcept { return m_context; }

		/**
		 * @brief Gets the ID of the entity
		 * @return ID of the entity
		 */
		inline U64 GetId() const noexcept { return m_id; }

		/**
		 * @brief Whether the entity has pending destruction
		 * @return True if the entity has pending destruction
		 */
		inline bool IsPendingDestroy() const noexcept { return m_pendingDestroy; }

		/**
		 * @brief Whether the entity has a component of the given type
		 * @tparam T Type of the component
		 * @return True if the entity has the component
		 */
		template<typename T>
		bool Has() const
		{
			TypeIndex index = GetTypeIndex<T>();
			return m_components.find(index) != m_components.end();
		}

		/**
		 * @brief Whether the entity has components of the given types
		 * @tparam T First Component
		 * @tparam V Second Component
		 * @tparam ...Types Remaining Components
		 * @return True if the entity has all the specified components
		 */
		template<typename T, typename U, typename... Types>
		bool Has() const
		{
			return Has<T>() && Has<U, Types...>();
		}

		/**
		 * @brief Adds a component to the entity
		 * @tparam T Type of the component
		 * @tparam ...Args Arguments to pass to the constructor
		 * @param ...args Arguments to pass to the constructor
		 * @return Handle to the added (or updated) component
		 */
		template<typename T, typename... Args>
		ComponentHandle<T> Add(Args&&... args);

		/**
		 * @brief Gets a component from the entity
		 * @tparam T Type of the component
		 * @return Handle to the component
		 */
		template<typename T>
		ComponentHandle<T> Get();

		/**
		 * @brief Gets a component from the entity
		 * @tparam T Type of the component
		 * @return Handle to the component
		 */
		template<typename T>
		bool Remove();

		/**
		 * @brief Removes all components from the entity
		 */
		void RemoveAll();

		/**
		 * @brief Executes a function on a set of component handles if the entity has components of the specified types
		 * @tparam ...Types Types of components
		 * @param view Function to execute
		 * @return True if the entity has all the specified components
		 */
		template<typename... Types>
		bool With(typename std::common_type<std::function<void(ComponentHandle<Types>...)>>::type func)
		{
			if (!Has<Types...>())
				return false;

			func(Get<Types>()...);

			return true;
		}

	private:
		/**
		 * @brief Maps component type indices to unique pointers to component containers
		 */
		ComponentMap m_components;

		/**
		 * @brief Pointer to the ECS context
		 */
		Context* m_context;

		/**
		 * @brief ID of the entity
		 */
		U64 m_id;

		/**
		 * @brief Flag indicating if the entity has pending destruction
		 */
		bool m_pendingDestroy{ false };
	};

	namespace Internal
	{
		/**
		 * @brief Iterator class to allow easy iteration over entities with components
		 * @tparam ...Types Types of components to include in the iteration
		 */
		template <typename... Types>
		class EntityComponentIterator
		{
		public:
			/**
			 * @brief Initializes the iterator with necessary information
			 * @param context Pointer to the ECS Context
			 * @param index Starting index for iteration
			 * @param isEnd Flag indicating if the iterator is at the end
			 * @param includePendingDestroy Flag indicating if the iterator should include pending destruction entities
			 */
			EntityComponentIterator(Context* context, U64 index, bool isEnd, bool includePendingDestroy);

			/**
			 * @brief Gets the index of the current entity in the iteration
			 * @return Index of the current entity
			 */
			inline U64 GetIndex() const noexcept { return m_index; }

			/**
			 * @brief Whether the iterator should include pending destruction entities
			 * @return True if the iterator should include pending destruction entities
			 */
			inline bool IncludePendingDestroy() const noexcept { return m_includePendingDestroy; }

			/**
			 * @brief Gets the ECS context associated with the iterator
			 * @return Pointer to the ECS context
			 */
			inline Context* GetContext() const noexcept { return m_context; }

			/**
			 * @brief Checks if the iterator is at the end
			 * @return True if the iterator is at the end
			 */
			bool IsEnd() const noexcept;

			/**
			 * @brief Gets the current entity held by the iterator
			 * @return Pointer to the current entity
			 */
			Entity* GetEntity() const noexcept;

			/**
			 * @brief Gets the current entity held by the iterator
			 * @return Pointer to the current entity
			 */
			Entity* operator*() const noexcept
			{
				return GetEntity();
			}

			/**
			 * @brief Checks if two iterators are equal
			 * @param other Other iterator
			 * @return True if the iterators are equal
			 */
			bool operator==(const EntityComponentIterator<Types...>& other) const
			{
				if (m_context != other.m_context)
					return false;

				if (m_isEnd)
					return other.m_isEnd;

				return m_index == other.m_index;
			}

			/**
			 * @brief Checks if two iterators are not equal
			 * @param other Other iterator
			 * @return True if the iterators are not equal
			 */
			bool operator!=(const EntityComponentIterator<Types...>& other) const
			{
				if (m_context != other.m_context)
					return true;

				if (m_isEnd)
					return !other.m_isEnd;

				return m_index != other.m_index;
			}

			/**
			 * @brief Moves the iterator to the next entity and skips over entities based on input criteria
			 * @return Reference to the iterator
			 */
			EntityComponentIterator<Types...>& operator++();

		private:
			/**
			 * @brief Flag indicating if the iterator is at the end
			 */
			bool m_isEnd;

			/**
			 * @brief Index of the current entity
			 */
			U64 m_index;

			/**
			 * @brief Pointer to the ECS context
			 */
			Context* m_context;

			/**
			 * @brief Flag indicating if the iterator should include pending destruction entities
			 */
			bool m_includePendingDestroy;
		};

		/**
		 * @brief Class to represent a view over a range of entities in an ECS Context
		 * @tparam ...Types Types of components to include in the iteration
		 */
		template<typename... Types>
		class EntityComponentView
		{
		public:
			/**
			 * @brief Initializes the view with iterator range
			 * @param first Iterator to the first entity
			 * @param last Iterator to the last entity
			 */
			EntityComponentView(const EntityComponentIterator<Types...>& first, const EntityComponentIterator<Types...>& last);

			/**
			 * @brief Gets the beginning of the view
			 * @return Begin iterator
			 */
			const EntityComponentIterator<Types...>& begin() const
			{
				return m_first;
			}

			/**
			 * @brief Gets the end of the view
			 * @return End iterator
			 */
			const EntityComponentIterator<Types...>& end() const
			{
				return m_last;
			}

		private:
			/**
			 * @brief First iterator of the view
			 */
			EntityComponentIterator<Types...> m_first;

			/**
			 * @brief Last iterator of the view
			*/
			EntityComponentIterator<Types...> m_last;
		};

		/**
		 * @brief Iterator class to allow easy iteration over entities
		 */
		class EntityIterator
		{
		public:
			/**
			 * @brief Initializes the iterator with necessary information
			 * @param context Pointer to the ECS Context
			 * @param index Starting index for iteration
			 * @param isEnd Flag indicating if the iterator is at the end
			 * @param includePendingDestroy Flag indicating if the iterator should include pending destruction entities
			 */
			EntityIterator(Context* context, U64 index, bool isEnd, bool includePendingDestroy);

			/**
			 * @brief Checks if the iterator is at the end
			 * @return True if the iterator is at the end
			 */
			bool IsEnd() const noexcept;

			/**
			 * @brief Gets the index of the current entity in the iteration
			 * @return Index of the current entity
			 */
			inline U64 GetIndex() const noexcept { return m_index; }

			/**
			 * @brief Whether the iterator should include pending destruction entities
			 * @return True if the iterator should include pending destruction entities
			 */
			inline bool IncludePendingDestroy() const noexcept { return m_includePendingDestroy; }

			/**
			 * @brief Gets the ECS context associated with the iterator
			 * @return Pointer to the ECS context
			 */
			inline Context* GetContext() const noexcept { return m_context; }

			/**
			 * @brief Gets the current entity held by the iterator
			 * @return Pointer to the current entity
			 */
			Entity* GetEntity() const noexcept;

			/**
			 * @brief Gets the current entity held by the iterator
			 * @return Pointer to the current entity
			 */
			Entity* operator*() const noexcept { return GetEntity(); }

			/**
			 * @brief Checks if two iterators are equal
			 * @param other Other iterator
			 * @return True if the iterators are equal
			 */
			bool operator==(const EntityIterator& other) const
			{
				if (m_context != other.m_context)
					return false;

				if (m_isEnd)
					return other.m_isEnd;

				return m_index == other.m_index;
			}

			/**
			 * @brief Checks if two iterators are not equal
			 * @param other Other iterator
			 * @return True if the iterators are not equal
			 */
			bool operator!=(const EntityIterator& other) const
			{
				if (m_context != other.m_context)
					return true;

				if (m_isEnd)
					return !other.m_isEnd;

				return m_index != other.m_index;
			}

			/**
			 * @brief Moves the iterator to the next entity and skips over entities based on input criteria
			 * @return Reference to the iterator
			 */
			EntityIterator& operator++();

		private:
			/**
			 * @brief Flag indicating if the iterator is at the end
			 */
			bool m_isEnd;

			/**
			 * @brief Index of the current entity
			 */
			U64 m_index;

			/**
			 * @brief Pointer to the ECS context
			 */
			Context* m_context;

			/**
			 * @brief Flag indicating if the iterator should include pending destruction entities
			 */
			bool m_includePendingDestroy;
		};

		/**
		 * @brief Class to represent a view over a range of entities in an ECS Context
		 */
		class EntityView
		{
		public:
			/**
			 * @brief Initializes the view with iterator range
			 * @param first Iterator to the first entity
			 * @param last Iterator to the last entity
			 */
			EntityView(const EntityIterator& first, const EntityIterator& last)
				: m_first(first), m_last(last)
			{
				if (m_first.GetEntity() == nullptr || m_first.GetEntity()->IsPendingDestroy() && !m_first.IncludePendingDestroy())
				{
					++m_first;
				}
			}

			/**
			 * @brief Gets the beginning of the view
			 * @return Begin iterator
			 */
			const EntityIterator& begin() const
			{
				return m_first;
			}

			/**
			 * @brief Gets the end of the view
			 * @return End iterator
			 */
			const EntityIterator& end() const
			{
				return m_last;
			}

		private:
			/**
			 * @brief First iterator of the view
			 */
			EntityIterator m_first;

			/**
			 * @brief Last iterator of the view
			 */
			EntityIterator m_last;
		};
	}
}