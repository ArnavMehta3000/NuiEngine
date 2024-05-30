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
		// Advance the index
		++m_index;

		/* 
		Keep advancing the index as long as :
		- Entity index is under limit
		- AND EITHER
		- Entity is not nullptr
		- OR
		- Entity has required components
		- OR
		- Entity is pending destruction (or not depending on input conditions)
		*/

		while (m_index < m_context->GetEntityCount()                                // Entity index is under limit
			&& (GetEntity() == nullptr                                              // Current entity is nullptr
				|| !GetEntity()->template Has<Types...>()                           // Entity has required components
				|| (GetEntity()->IsPendingDestroy() && !m_includePendingDestroy)))  // Entity is pending destruction
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
		/*
		Ensure 'm_first' points to a valid entity by:
		- First entity is valid (not nullptr)
		- OR
		- First entity is pending destruction AND IncludePendingDestroy is false
		- OR
		- First entity has a all required components
		If th above conditions fail, (using the ++ operator) advance the EntityComponentIterator until we find a valid Entity or reaches end of range
		*/
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
		// Similar to EntityComponentIterator's ++ operator

		++m_index;
		while (m_index < m_context->GetEntityCount() 
			&& (GetEntity() == nullptr 
				|| (GetEntity()->IsPendingDestroy() && !m_includePendingDestroy)))
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
#pragma endregion

#pragma region Entity
	template<typename T, typename ...Args>
	inline ComponentHandle<T> Entity::Add(Args && ...args)
	{
		auto found = m_components.find(GetTypeIndex<T>());
		if (found != m_components.end())
		{
			// If found, update the existing component
			Internal::ComponentContainer<T>* container = reinterpret_cast<Internal::ComponentContainer<T>*>(found->second.get());
			container->m_data = T(std::forward<Args>(args)...);

			ComponentHandle<T> handle = ComponentHandle<T>(&container->m_data);
			m_context->EmitEvent<Events::OnComponentAdd<T>>({ this, handle });
			return handle;
		}
		else
		{
			// If not found, create a new component container
			std::unique_ptr<Internal::ComponentContainer<T>> container = std::make_unique<Internal::ComponentContainer<T>>(T(std::forward<Args>(args)...));
			
			Internal::ComponentContainer<T>* containerRawPtr = container.get();

			// Insert the new component container into the map
			m_components.insert({ GetTypeIndex<T>(), std::move(container) });

			// Now that the container is safely stored in the map, we can create the handle
			ComponentHandle<T> handle(&containerRawPtr->m_data);

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
			Internal::ComponentContainer<T>* container = reinterpret_cast<Internal::ComponentContainer<T>*>(found->second.get());
			return ComponentHandle<T>(&container->m_data);
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
	inline Context::Context() = default;

	inline Context::~Context()
	{
		// Shutdown systems
		for (std::unique_ptr<SystemBase>& system : m_systems)
		{
			system->OnShutdown(this);
		}

		// Clear entities
		for (std::unique_ptr<Entity>& ent : m_entities)
		{
			if (!ent->IsPendingDestroy())
			{
				ent->m_pendingDestroy = true;
				EmitEvent<Events::OnEntityDestroy>({ ent.get()});
			}

			ent.reset();
		}

		// Shutdown systems
		for (std::unique_ptr<SystemBase>& system : m_systems)
		{
			system.reset();
		}
	}

	inline Entity* Context::GetEntityById(U64 id)
	{
		if (id == Entity::InvalidId || id > m_lastEntityId)
			return nullptr;

		// We should likely store entities in a map of id -> entity so that this is faster.
		for (std::unique_ptr<Entity>& ent : m_entities)
		{
			if (ent->GetId() == id)
				return ent.get();
		}

		return nullptr;
	}

	inline Entity* Context::GetEntityByIndex(U64 index)
	{
		if (index >= m_entities.size())
			return nullptr;

		return m_entities[index].get();
	}

	inline Entity* Context::CreateEntity()
	{
		++m_lastEntityId;
		m_entities.push_back(std::make_unique<Entity>(this, m_lastEntityId));
		
		Entity* ent = m_entities.front().get();

		EmitEvent<Events::OnEntityCreate>({ ent });

		return ent;
	}

	inline void Context::DestroyEntity(Entity* e, bool immediate)
	{
		if (e == nullptr)
			return;

		if (e->IsPendingDestroy())
		{
			if (immediate)
			{
				m_entities.erase(
					std::remove_if(
						m_entities.begin(), m_entities.end(), 
						[e] (const std::unique_ptr<Entity>& ptr)
						{
							return ptr.get() == e; 
						}),
					m_entities.end());
			}

			return;
		}

		e->m_pendingDestroy = true;

		EmitEvent<Events::OnEntityDestroy>({ e });

		if (immediate)
		{
			m_entities.erase(
				std::remove_if(
					m_entities.begin(), m_entities.end(), 
					[e] (const std::unique_ptr<Entity>& ptr)
					{ 
						return ptr.get() == e;
					}), 
				m_entities.end());
		}
	}
	
	inline bool Context::ClearPending()
	{
		U64 count = 0;
		m_entities.erase(
			std::remove_if(
				m_entities.begin(), m_entities.end(),
				[&, this](std::unique_ptr<Entity>& ent)
				{
					if (ent->IsPendingDestroy())
					{
						++count;
						ent.reset();
						return true;
					}

					return false;
				}),
			m_entities.end()
		);

		return count > 0;
	}

	inline void Context::Reset()
	{
		for (std::unique_ptr<Entity>& ent : m_entities)
		{
			if (!ent->IsPendingDestroy()) 
			{
				ent->m_pendingDestroy = true;
				EmitEvent<Events::OnEntityDestroy>({ ent.get() });
			}
		}

		m_entities.clear();
		m_lastEntityId = 0;
	}

	inline void Context::UnregisterAllSystems()
	{
		// Shutdown all systems, and clear the vector
		for (std::unique_ptr<SystemBase>& system : m_systems)
		{
			system->OnShutdown(this);
		}

		m_systems.clear();
	}

	template <typename T, typename... Args>
	inline T* Context::RegisterSystem(Args&&... args) requires IsSystem<T>
	{
		// Check if a system of type T already exists
		for (const std::unique_ptr<SystemBase>& system : m_systems)
		{
			if (dynamic_cast<T*>(system.get()))
			{
				return nullptr;
			}
		}

		std::unique_ptr<SystemBase> system = std::make_unique<T>(std::forward<Args>(args)...);
		system->OnInit(this);

		m_systems.push_back(std::move(system));

		return dynamic_cast<T*>(m_systems.back().get());
	}

	template <typename T>
	inline void Context::UnregisterSystem() requires IsSystem<T>
	{
		// Remove the system from the vector
		auto it = std::remove_if(
			m_systems.begin(), m_systems.end(), 
			[&] (std::unique_ptr<SystemBase>& system)
			{
				if (dynamic_cast<T*>(system.get()) != nullptr) 
				{
					system->OnShutdown(this);
					return true; // Mark for removal
				}
				
				return false; // Keep the system
			});

		// Erase the removed systems from the vector
		if (it != m_systems.end())
		{
			m_systems.erase(it, m_systems.end());
		}
	}

	template<typename T>
	inline T* Context::GetSystem() requires IsSystem<T>
	{
		for (std::unique_ptr<SystemBase>& system : m_systems)
		{
			if (T* typedSystem = dynamic_cast<T*>(system.get()))
			{
				return typedSystem;
			}
		}
		return nullptr;
	}

	template <typename T>
	inline void Context::EnableSystem() requires IsSystem<T>
	{
		for (std::unique_ptr<SystemBase>& system : m_systems)
		{
			if (dynamic_cast<T*>(system.get()) != nullptr)
			{
				system->SetIsEnabled(true);
			}
		}
	}

	template <typename T>
	inline void Context::DisableSystem() requires IsSystem<T>
	{
		for (std::unique_ptr<SystemBase>& system : m_systems)
		{
			if (dynamic_cast<T*>(system.get()) != nullptr)
			{
				system->SetIsEnabled(false);
			}
		}
	}

	template <typename T>
	inline bool Context::IsSystmEnabled() requires IsSystem<T>
	{
		for (std::unique_ptr<SystemBase>& system : m_systems)
		{
			if (dynamic_cast<T*>(system.get()) != nullptr)
			{
				return system->IsEnabled();
			}
		}
	}

	template<typename T>
	inline void Context::SubscribeEvent(EventSubscriber<T>* subscriber)
	{
		TypeIndex index = GetTypeIndex<T>();
		auto found = m_subscribers.find(index);

		if (found == m_subscribers.end())
		{
			// No subscribers yet
			std::vector<Internal::EventSubscriberBase*> subList;
			subList.push_back(subscriber);
			m_subscribers.insert({ index, subList });
		}
		else
		{
			// Add subscriber
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

	inline void Context::UnsubscribeAll(void* subscriber)
	{
		for (auto& [typeIndex, subList] : m_subscribers)
		{
			subList.erase(
				std::remove(
					subList.begin(), subList.end(),
					subscriber
				), subList.end()
			);

			if (subList.size() == 0)
			{
				m_subscribers.erase(typeIndex);
			}
		}
	}

	inline Internal::EntityView Context::All(bool includePendingDestroy)
	{
		Internal::EntityIterator first(this, 0, false, includePendingDestroy);
		Internal::EntityIterator last(this, GetEntityCount(), true, includePendingDestroy);
		return Internal::EntityView(first, last);
	}

	inline void Context::All(std::function<void(Entity*)> viewFunc, bool includePendingDestroy)
	{
		for (auto* ent : All(includePendingDestroy))
		{
			viewFunc(ent);
		}
	}

	template<typename T>
	inline void Context::EmitEvent(const T& event)
	{
		TypeIndex index = GetTypeIndex<T>();
		auto found = m_subscribers.find(index);

		if (found != m_subscribers.end())
		{
			for (Internal::EventSubscriberBase* subBase : found->second)
			{
				EventSubscriber<T>* sub = reinterpret_cast<EventSubscriber<T>*>(subBase);
				sub->OnEvent(this, event);
			}
		}
	}

	template<typename ...Types>
	inline Internal::EntityComponentView<Types...> Nui::ECS::Context::Each(bool includePendingDestroy)
	{
		Internal::EntityComponentIterator<Types...> first(this, 0, false, includePendingDestroy);
		Internal::EntityComponentIterator<Types...> last(this, GetEntityCount(), true, includePendingDestroy);
		return Internal::EntityComponentView<Types...>(first, last);
	}

	template<typename ...Types>
	inline void Context::Each(typename std::common_type<std::function<void(Entity*, ComponentHandle<Types>...)>>::type viewFunc, bool includePendingDestroy)
	{
		for (Entity* ent : Each<Types...>(includePendingDestroy))
		{
			viewFunc(ent, ent->template Get<Types>()...);
		}
	}

	inline void Context::Tick(const F64 dt)
	{
		//ClearPending();

		for (std::unique_ptr<SystemBase>& system : m_systems)
		{
			system->OnUpdate(this, dt);
		}
	}
#pragma endregion
}