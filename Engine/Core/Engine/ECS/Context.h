#pragma once
#include "Core/Engine/ECS/Common.h"

namespace Nui::ECS
{
	class Context
	{
	public:
		using Allocator_t = std::allocator<::Nui::ECS::Entity>;
		using ContextAllocator = std::allocator_traits<Allocator_t>::template rebind_alloc<Context>;
		using EntityAllocator = std::allocator_traits<Allocator_t>::template rebind_alloc<Entity>;
		using SystemAllocator = std::allocator_traits<Allocator_t>::template rebind_alloc<SystemBase>;
		using EntityPtrAllocator = std::allocator_traits<Allocator_t>::template rebind_alloc<Entity*>;
		using SystemPtrAllocator = std::allocator_traits<Allocator_t>::template rebind_alloc<SystemBase*>;
		using SubscriberPtrAllocator = std::allocator_traits<Allocator_t>::template rebind_alloc<Internal::EventSubscriberBase*>;
		using SubscriberPairAllocator = std::allocator_traits<Allocator_t>::template rebind_alloc<std::pair<const TypeIndex, std::vector<Internal::EventSubscriberBase*, SubscriberPtrAllocator>>>;
		using SubscriberMap = std::unordered_map<TypeIndex, std::vector<Internal::EventSubscriberBase*, SubscriberPtrAllocator>, std::hash<TypeIndex>, std::equal_to<TypeIndex>>;

	public:
		static Context* Create();
		void Destroy();

		Context(Allocator_t allocator);
		~Context();


		inline EntityAllocator& GetPrimaryAllocator() { return m_entityAlloc; }
		inline U64 GetEntityCount() const noexcept { return m_entities.size(); }
		inline Entity* GetEntityByIndex(U64 index) const
		{
			if (index >= m_entities.size())
				return nullptr;

			return m_entities[index];
		}
		Entity* GetEntityById(U64 id);

		Entity* CreateEntity();
		void DestroyEntity(Entity* e, bool immediate = false);
		SystemBase* RegisterSystem(SystemBase* system);
		void UnregisterSystem(SystemBase* system);
		void EnableSystem(SystemBase* system);
		void DisableSystem(SystemBase* system);
		bool CleanUp();
		void Reset();

		template <typename T>
		void SubscribeEvent(EventSubscriber<T>* subscriber);		

		template<typename T>
		void UnsubscribeEvent(EventSubscriber<T>* subscriber);		

		void UnsubscribeAll(void* subscriber);

		template<typename T>
		void EmitEvent(const T& event);

		template<typename... Types>
		void Each(typename std::common_type<std::function<void(Entity*, ComponentHandle<Types>...)>>::type viewFunc, bool includePendingDestroy = false);
		
		void All(std::function<void(Entity*)> viewFunc, bool includePendingDestroy = false);
		
		Internal::EntityView All(bool includePendingDestroy = false);

		template<typename... Types>
		Internal::EntityComponentView<Types...> Each(bool includePendingDestroy = false);
		
		void Tick(const F64 dt);


	protected:
		EntityAllocator                              m_entityAlloc;
		SystemAllocator                              m_systemAlloc;
		std::vector<Entity*, EntityPtrAllocator>     m_entities;
		std::vector<SystemBase*, SystemPtrAllocator> m_systems;
		std::vector<SystemBase*>                     m_disabledSystems;
		SubscriberMap                                m_subscribers;
		U64                                          m_lastEntityId{ 0 };
	};

	struct ContextDeleter
	{
		void operator()(Context* ptr) const
		{
			if (ptr)
			{
				// Perform cleanup before destroying ECS context
				ptr->CleanUp();
				ptr->Destroy();
			}
		}
	};

	using ContextPtr = std::unique_ptr<Context, ContextDeleter>;
}