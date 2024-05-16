#pragma once
#include "Core/Engine/ECS/Component.h"

namespace Nui::ECS
{
	class Entity
	{
		friend class Context;
		friend class Context2;
	public:
		constexpr static U64 InvalidId = 0;

	public:
		Entity(Context* context, U64 id) : m_context(context), m_id(id) {}
		~Entity() { RemoveAll(); }

		inline Context* GetContext() const noexcept { return m_context; }
		inline U64 GetId() const noexcept { return m_id; }
		inline bool IsPendingDestroy() const noexcept { return m_pendingDestroy; }

		template<typename T>
		bool Has() const
		{
			TypeIndex index = GetTypeIndex<T>();
			return m_components.find(index) != m_components.end();
		}

		template<typename T, typename V, typename... Types>
		bool Has() const
		{
			return Has<T>() && Has<V, Types...>();
		}

		template<typename T, typename... Args>
		ComponentHandle<T> Add(Args&&... args);

		template<typename T>
		ComponentHandle<T> Get();

		template<typename T>
		bool Remove();

		void RemoveAll();

		template<typename... Types>
		bool With(typename std::common_type<std::function<void(ComponentHandle<Types>...)>>::type view)
		{
			if (!Has<Types...>())
				return false;

			View(Get<Types>()...);

			return true;
		}

	private:
		std::unordered_map<TypeIndex, Internal::ComponentContainerBase*> m_components;
		Context*                                                         m_context;
		U64                                                              m_id;
		bool                                                             m_pendingDestroy{ false };
	};

	namespace Internal
	{
		template <typename... Types>
		class EntityComponentIterator
		{
		public:
			EntityComponentIterator(Context* context, U64 index, bool isEnd, bool includePendingDestroy);

			inline U64 GetIndex() const noexcept { return m_index; }
			inline bool IncludePendingDestroy() const noexcept { return m_includePendingDestroy; }
			inline Context* GetContext() const noexcept { return m_context; }

			bool IsEnd() const noexcept;

			Entity* GetEntity() const noexcept;

			Entity* operator*() const noexcept
			{
				return GetEntity();
			}

			bool operator==(const EntityComponentIterator<Types...>& other) const
			{
				if (m_context != other.m_context)
					return false;

				if (m_isEnd)
					return other.m_isEnd;

				return m_index == other.m_index;
			}

			bool operator!=(const EntityComponentIterator<Types...>& other) const
			{
				if (m_context != other.m_context)
					return true;

				if (m_isEnd)
					return !other.m_isEnd;

				return m_index != other.m_index;
			}

			EntityComponentIterator<Types...>& operator++();

		private:
			bool     m_isEnd;
			U64      m_index;
			Context* m_context;
			bool     m_includePendingDestroy;
		};

		template<typename... Types>
		class EntityComponentView
		{
		public:
			EntityComponentView(const EntityComponentIterator<Types...>& first, const EntityComponentIterator<Types...>& last);

			const EntityComponentIterator<Types...>& begin() const
			{
				return m_first;
			}

			const EntityComponentIterator<Types...>& end() const
			{
				return m_last;
			}

		private:
			EntityComponentIterator<Types...> m_first;
			EntityComponentIterator<Types...> m_last;
		};

		class EntityIterator
		{
		public:
			EntityIterator(Context* context, U64 index, bool isEnd, bool includePendingDestroy);

			bool IsEnd() const noexcept;
			inline U64 GetIndex() const noexcept { return m_index; }
			inline bool IncludePendingDestroy() const noexcept { return m_includePendingDestroy; }
			inline Context* GetContext() const noexcept { return m_context; }
			Entity* GetEntity() const noexcept;

			Entity* operator*() const noexcept { return GetEntity(); }

			bool operator==(const EntityIterator& other) const
			{
				if (m_context != other.m_context)
					return false;

				if (m_isEnd)
					return other.m_isEnd;

				return m_index == other.m_index;
			}

			bool operator!=(const EntityIterator& other) const
			{
				if (m_context != other.m_context)
					return true;

				if (m_isEnd)
					return !other.m_isEnd;

				return m_index != other.m_index;
			}

			EntityIterator& operator++();

		private:
			bool     m_isEnd;
			U64      m_index;
			Context* m_context;
			bool     m_includePendingDestroy;
		};

		class EntityView
		{
		public:
			EntityView(const EntityIterator& first, const EntityIterator& last)
				: m_first(first), m_last(last)
			{
				if (m_first.GetEntity() == nullptr || m_first.GetEntity()->IsPendingDestroy() && !m_first.IncludePendingDestroy())
				{
					++m_first;
				}
			}

			const EntityIterator& begin() const
			{
				return m_first;
			}

			const EntityIterator& end() const
			{
				return m_last;
			}

		private:
			EntityIterator m_first;
			EntityIterator m_last;
		};		
	}
}