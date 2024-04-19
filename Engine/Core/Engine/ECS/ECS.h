#pragma once
#include "Core/Engine/ECS/Entity.h"
#include "Core/Engine/ECS/Component.h"
#include "Core/Engine/ECS/System.h"
#include "Core/Engine/ECS/Event.h"
#include "Core/Engine/ECS/Context.h"

namespace Nui::ECS
{
#pragma region Entity Component Iterator
	template<typename... Types>
	inline Internal::EntityComponentIterator<Types...>::EntityComponentIterator(Context* context, U64 index, bool isEnd, bool includePendingDestroy)
		: m_context(context)
		, m_index(index)
		, m_isEnd(isEnd)
		, m_includePendingDestroy(includePendingDestroy)
	{
		if (index >= context->GetEntityCount())
		{
			m_isEnd = true;
		}
	}

	template<typename... Types>
	inline bool Internal::EntityComponentIterator<Types...>::IsEnd() const noexcept
	{
		return m_isEnd || m_index >= m_context->GetEntityCount();
	}

	template<typename... Types>
	inline Entity* Internal::EntityComponentIterator<Types...>::GetEntity() const noexcept
	{
		if (IsEnd())
			return nullptr;

		return m_context->GetEntityByIndex(m_index);
	}

	template<typename... Types>
	inline Internal::EntityComponentIterator<Types...>& Internal::EntityComponentIterator<Types...>::operator++()
	{
		++m_index;
		while (m_index < m_context->GetEntityCount() && (GetEntity() == nullptr || !GetEntity()->template Has<Types...>() || (GetEntity()->IsPendingDestroy() && !m_includePendingDestroy)))
		{
			++m_index;
		}

		if (m_index >= m_context->GetEntityCount())
			m_isEnd = true;

		return *this;
	}
#pragma endregion

#pragma region Entity Component View
	template<typename... Types>
	inline Internal::EntityComponentView<Types...>::EntityComponentView(const Internal::EntityComponentIterator<Types...>& first, const Internal::EntityComponentIterator<Types...>& last)
		: m_first(first)
		, m_last(last)
	{
		if (m_first.GetEntity() == nullptr
			|| (m_first.GetEntity()->IsPendingDestroy() && !m_first.IncludePendingDestroy())
			|| !m_first.GetEntity()->template Has<Types...>())
		{
			++m_first;
		}
	}
#pragma endregion

#pragma region Entity Iterator
	inline Internal::EntityIterator::EntityIterator(Context* context, U64 index, bool isEnd, bool includePendingDestroy)
		: m_context(context)
		, m_index(index)
		, m_isEnd(isEnd)
		, m_includePendingDestroy(includePendingDestroy)
	{
		if (index >= context->GetEntityCount())
		{
			m_isEnd = true;
		}
	}

	inline bool Internal::EntityIterator::IsEnd() const noexcept
	{
		return m_isEnd || m_index >= m_context->GetEntityCount();
	}

	inline Entity* Internal::EntityIterator::GetEntity() const noexcept
	{
		if (IsEnd())
			return nullptr;

		return m_context->GetEntityByIndex(m_index);
	}

	inline Internal::EntityIterator& Internal::EntityIterator::operator++()
	{
		++m_index;
		while (m_index < m_context->GetEntityCount() && (GetEntity() == nullptr || (GetEntity()->IsPendingDestroy() && !m_includePendingDestroy)))
		{
			++m_index;
		}

		if (m_index >= m_context->GetEntityCount())
			m_isEnd = true;

		return *this;
	}
#pragma endregion

#pragma region Component Container
	template<typename T>
	inline void Internal::ComponentContainer<T>::OnRemove(Entity* entity)
	{
		auto handle = ComponentHandle<T>(&m_data);
		entity->GetContext()->EmitEvent<Events::OnComponentRemove<T>>({ entity, handle });
	}

	template<typename T>
	inline void Internal::ComponentContainer<T>::OnDestroy(Context* context)
	{
		using ComponentAllocator = std::allocator_traits<Context::EntityAllocator>::template rebind_alloc<ComponentContainer<T>>;

		ComponentAllocator alloc(context->GetPrimaryAllocator());
		std::allocator_traits<ComponentAllocator>::destroy(alloc, this);
		std::allocator_traits<ComponentAllocator>::deallocate(alloc, this, 1);
	}
#pragma endregion

#pragma region Entity
	template<typename T, typename ...Args>
	inline ComponentHandle<T> Entity::Add(Args && ...args)
	{
		using ComponentAllocator = std::allocator_traits<Context::EntityAllocator>::template rebind_alloc<Internal::ComponentContainer<T>>;

		auto found = m_components.find(GetTypeIndex<T>());
		if (found != m_components.end())
		{
			Internal::ComponentContainer<T>* container = reinterpret_cast<Internal::ComponentContainer<T>*>(found->second);
			container->m_data = T(std::forward<Args>(args)...);

			auto handle = ComponentHandle<T>(&container->m_data);
			m_context->EmitEvent<Events::OnComponentAdd<T>>({ this, handle });
			return handle;
		}
		else
		{
			ComponentAllocator alloc(m_context->GetPrimaryAllocator());

			Internal::ComponentContainer<T>* container = std::allocator_traits<ComponentAllocator>::allocate(alloc, 1);
			std::allocator_traits<ComponentAllocator>::construct(alloc, container, T(std::forward<Args>(args)...));

			m_components.insert({ GetTypeIndex<T>(), container });

			auto handle = ComponentHandle<T>(&container->m_data);
			m_context->EmitEvent<Events::OnComponentAdd<T>>({ this, handle });
			return handle;
		}
	}

	template<typename T>
	inline ComponentHandle<T> Entity::Get()
	{
		auto found = m_components.find(GetTypeIndex<T>());
		if (found != m_components.end())
		{
			return ComponentHandle<T>(&reinterpret_cast<Internal::ComponentContainer<T>*>(found->second)->m_data);
		}

		return ComponentHandle<T>();
	}

	template<typename T>
	inline bool Entity::Remove()
	{
		auto found = m_components.find(GetTypeIndex<T>());
		if (found != m_components.end())
		{
			found->second->OnRemove(this);
			found->second->OnDestroy(m_context);

			m_components.erase(found);

			return true;
		}

		return false;
	}
#pragma endregion

#pragma region Context
	Context* Context::Create()
	{
		Allocator_t alloc;  // Default allocator to create the context
		ContextAllocator ctxAlloc(alloc);

		Context* ctx = std::allocator_traits<ContextAllocator>::allocate(ctxAlloc, 1);
		std::allocator_traits<ContextAllocator>::construct(ctxAlloc, ctx, alloc);

		return ctx;
	}

	inline void Context::Destroy()
	{
		ContextAllocator alloc(m_entityAlloc);
		std::allocator_traits<ContextAllocator>::destroy(alloc, this);
		std::allocator_traits<ContextAllocator>::deallocate(alloc, this, 1);
	}

	Context::Context(Allocator_t allocator)
		: m_entityAlloc(allocator)
		, m_systemAlloc(allocator)
		, m_entities({}, EntityPtrAllocator(allocator))
		, m_systems({}, SystemPtrAllocator(allocator))
		, m_subscribers({}, std::hash<TypeIndex>(), std::equal_to<TypeIndex>(), SubscriberPtrAllocator(allocator))
	{
	}

	Context::~Context()
	{
		for (auto* system : m_systems)
		{
			system->OnShutdown(this);
		}

		for (auto* ent : m_entities)
		{
			if (!ent->IsPendingDestroy())
			{
				ent->m_pendingDestroy = true;
				EmitEvent<Events::OnEntityDestroy>({ ent });
			}

			std::allocator_traits<EntityAllocator>::destroy(m_entityAlloc, ent);
			std::allocator_traits<EntityAllocator>::deallocate(m_entityAlloc, ent, 1);
		}

		for (auto* system : m_systems)
		{
			std::allocator_traits<SystemAllocator>::destroy(m_systemAlloc, system);
			std::allocator_traits<SystemAllocator>::deallocate(m_systemAlloc, system, 1);
		}
	}

	Entity* Context::GetEntityById(U64 id)
	{
		if (id == Entity::InvalidId || id > m_lastEntityId)
			return nullptr;

		// We should likely store entities in a map of id -> entity so that this is faster.
		for (Entity* ent : m_entities)
		{
			if (ent->GetId() == id)
				return ent;
		}

		return nullptr;
	}

	Entity* Context::CreateEntity()
	{
		++m_lastEntityId;
		Entity* ent = std::allocator_traits<EntityAllocator>::allocate(m_entityAlloc, 1);
		std::allocator_traits<EntityAllocator>::construct(m_entityAlloc, ent, this, m_lastEntityId);
		m_entities.push_back(ent);

		EmitEvent<Events::OnEntityCreate>({ ent });

		return ent;
	}

	void Context::DestroyEntity(Entity* e, bool immediate)
	{
		if (e == nullptr)
			return;

		if (e->IsPendingDestroy())
		{
			if (immediate)
			{
				m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), e), m_entities.end());
				std::allocator_traits<EntityAllocator>::destroy(m_entityAlloc, e);
				std::allocator_traits<EntityAllocator>::deallocate(m_entityAlloc, e, 1);
			}

			return;
		}

		e->m_pendingDestroy = true;

		EmitEvent<Events::OnEntityDestroy>({ e });

		if (immediate)
		{
			m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), e), m_entities.end());
			std::allocator_traits<EntityAllocator>::destroy(m_entityAlloc, e);
			std::allocator_traits<EntityAllocator>::deallocate(m_entityAlloc, e, 1);
		}
	}

	SystemBase* Context::RegisterSystem(SystemBase* system)
	{
		m_systems.push_back(system);
		system->OnInit(this);

		return system;
	}

	void Context::UnregisterSystem(SystemBase* system)
	{
		m_systems.erase(std::remove(m_systems.begin(), m_systems.end(), system), m_systems.end());
		system->OnShutdown(this);
	}

	void Context::EnableSystem(SystemBase* system)
	{
		auto it = std::find(m_disabledSystems.begin(), m_disabledSystems.end(), system);
		if (it != m_disabledSystems.end())
		{
			m_disabledSystems.erase(it);
			m_systems.push_back(system);
		}
	}

	void Context::DisableSystem(SystemBase* system)
	{
		auto it = std::find(m_systems.begin(), m_systems.end(), system);
		if (it != m_systems.end())
		{
			m_systems.erase(it);
			m_disabledSystems.push_back(system);
		}
	}

	bool Context::CleanUp()
	{
		U64 count = 0;
		m_entities.erase(
			std::remove_if(
				m_entities.begin(), m_entities.end(),
				[&, this](Entity* ent)
				{
					if (ent->IsPendingDestroy())
					{
						std::allocator_traits<EntityAllocator>::destroy(m_entityAlloc, ent);
						std::allocator_traits<EntityAllocator>::deallocate(m_entityAlloc, ent, 1);
						++count;
						return true;
					}

					return false;
				}),
			m_entities.end()
		);

		return count > 0;
	}

	void Context::Reset()
	{
		for (auto* ent : m_entities)
		{
			if (!ent->IsPendingDestroy())
			{
				ent->m_pendingDestroy = true;
				EmitEvent<Events::OnEntityDestroy>({ ent });
			}
			std::allocator_traits<EntityAllocator>::destroy(m_entityAlloc, ent);
			std::allocator_traits<EntityAllocator>::deallocate(m_entityAlloc, ent, 1);
		}

		m_entities.clear();
		m_lastEntityId = 0;
	}
	
	template<typename T>
	inline void Context::SubscribeEvent(EventSubscriber<T>* subscriber)
	{
		TypeIndex index = GetTypeIndex<T>();
		auto found = m_subscribers.find(index);

		if (found == m_subscribers.end())
		{
			std::vector<Internal::EventSubscriberBase*, SubscriberPtrAllocator> subList(m_entityAlloc);
			subList.push_back(subscriber);

			m_subscribers.insert({ index, subList });
		}
		else
		{
			found->second.push_back(subscriber);
		}
	}

	template<typename T>
	inline void Context::UnsubscribeEvent(EventSubscriber<T>* subscriber)
	{
		TypeIndex index = GetTypeIndex<T>();
		auto found = m_subscribers.find(index);

		if (found != m_subscribers.end())
		{
			found->second.erase(
				std::remove(
					found->second.begin(), found->second.end(),
					subscriber),
				found->second.end()
			);

			if (found->second.size() == 0)
			{
				m_subscribers.erase(found);
			}
		}
	}

	void Context::UnsubscribeAll(void* subscriber)
	{
		for (auto kv : m_subscribers)
		{
			kv.second.erase(
				std::remove(
					kv.second.begin(), kv.second.end(),
					subscriber
				), kv.second.end()
			);

			if (kv.second.size() == 0)
			{
				m_subscribers.erase(kv.first);
			}
		}
	}

	template<typename T>
	inline void Context::EmitEvent(const T& event)
	{
		TypeIndex index = GetTypeIndex<T>();
		auto found = m_subscribers.find(index);
		if (found != m_subscribers.end())
		{
			for (auto* base : found->second)
			{
				auto* sub = reinterpret_cast<EventSubscriber<T>*>(base);
				sub->OnReceiveEvent(this, event);
			}
		}
	}

	template<typename ...Types>
	inline void Context::Each(typename std::common_type<std::function<void(Entity*, ComponentHandle<Types>...)>>::type viewFunc, bool includePendingDestroy)
	{
		for (auto* ent : Each<Types...>(includePendingDestroy))
		{
			viewFunc(ent, ent->template Get<Types>()...);
		}
	}

	template<typename ...Types>
	inline Internal::EntityComponentView<Types...> Context::Each(bool includePendingDestroy)
	{
		Internal::EntityComponentIterator<Types...> first(this, 0, false, includePendingDestroy);
		Internal::EntityComponentIterator<Types...> last(this, GetEntityCount(), true, includePendingDestroy);
		return Internal::EntityComponentView<Types...>(first, last);
	}

	inline void Context::All(std::function<void(Entity*)> viewFunc, bool includePendingDestroy)
	{
		for (auto* ent : All(includePendingDestroy))
		{
			viewFunc(ent);
		}
	}

	inline Internal::EntityView Context::All(bool includePendingDestroy)
	{
		Internal::EntityIterator first(this, 0, false, includePendingDestroy);
		Internal::EntityIterator last(this, GetEntityCount(), true, includePendingDestroy);
		return Internal::EntityView(first, last);
	}

#pragma endregion
}