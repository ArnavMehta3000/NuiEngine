#include "Core/ECS/ECSInternal.h"
#include "Core/ECS/ComponentPool.h"

namespace Nui::ECS
{
	namespace Internal
	{
		struct EntityInfo
		{
			EntityId Id;
			ComponentMask Mask;
		};
	}

	class World
	{
		template <typename... Types>
		friend class WorldView;
	public:
		World() = default;

		EntityId CreateEntity();
		void MarkForDelete(EntityId id);

		template <typename ComponentType, typename... Args>
		ComponentType* AddComponent(EntityId id, Args&&... args)
			requires std::is_constructible_v<ComponentType>  &&
			(std::is_constructible_v<ComponentType, Args...> || std::is_default_constructible_v<ComponentType>)
		{
			using namespace Nui::ECS::Internal;

			// Get component id
			ComponentId componentId = GetId<ComponentType>();

			// Assert if provided entity does not exist
			NUI_ASSERT(m_entities[GetEntityIndex(id)].Id == id,
				"Trying to add a component on an entity that does not exist");

			// Assert if provided entity already has this component
			NUI_ASSERT(!m_entities[GetEntityIndex(id)].Mask[componentId],
				"Trying to add a component on an entity that already has it");

			if (m_entities[GetEntityIndex(id)].Id != id)
				return nullptr;

			// Increase component pool size by one if needed
			if (componentId >= m_componentPools.size())
				m_componentPools.resize(componentId + 1);

			// New component (id) make a new component pool
			if (m_componentPools[componentId] == nullptr)
				m_componentPools[componentId] = std::make_unique<ComponentPool>(sizeof(ComponentType));

			// https://en.cppreference.com/w/cpp/language/new#Placement_new - using 'placement new'
			// Construct the component and place it in the pool (where the memory is already pre-allocated)
			auto pool = m_componentPools[componentId]->Get<ComponentType>(GetEntityIndex(id));
			if constexpr (std::is_constructible_v<ComponentType, Args...>)
			{
				ComponentType* outComp = new(pool) ComponentType(std::forward<Args>(args)...);
				m_entities[GetEntityIndex(id)].Mask.set(componentId);
				return outComp;
			}
			else if constexpr (std::is_default_constructible_v<ComponentType>)
			{
				ComponentType* outComp = new(pool) ComponentType();
				m_entities[GetEntityIndex(id)].Mask.set(componentId);
				return outComp;
			}
			else
			{
				return nullptr;
			}
		}

		template <typename ComponentType>
		void RemoveComponent(EntityId id)
		{
			using namespace Nui::ECS::Internal;

			// Assert if provided entity does not exist
			NUI_ASSERT(m_entities[GetEntityIndex(id)].Id == id,
				"Trying to remove a component on an entity that does not exist");

			if (m_entities[GetEntityIndex(id)].Id != id)
				return;

			ComponentId componentId = GetId<ComponentType>();
			m_entities[GetEntityIndex(id)].Mask.reset(componentId);

			// Destruct the component
			if (componentId < m_componentPools.size() && m_componentPools[componentId])
			{
				m_componentPools[componentId]->Get<ComponentType>(GetEntityIndex(id))->~ComponentType();
			}
		}

		template <typename ComponentType>
		ComponentType* GetComponent(EntityId id)
		{
			using namespace Nui::ECS::Internal;

			ComponentId componentId = GetId<ComponentType>();

			// Assert if provided entity does not exist
			NUI_ASSERT(m_entities[GetEntityIndex(id)].Id == id,
				"Trying to get a component on an entity that does not exist");

			// Assert if provided entity does not have this component
			NUI_ASSERT(m_entities[GetEntityIndex(id)].Mask.test(componentId),
				"Trying to get a component on an entity that does not have it");

			// Assert if provided component does not exist
			NUI_ASSERT(m_componentPools[componentId].get(),
				"Component pools does not exist for current component");

			if (!m_entities[GetEntityIndex(id)].Mask.test(componentId))
				return nullptr;

			if (componentId < m_componentPools.size() && m_componentPools[componentId])
			{
				return m_componentPools[componentId]->Get<ComponentType>(componentId);
			}
			else
			{
				return nullptr;
			}
		}

		template <typename ComponentType>
		bool HasComponent(EntityId id)
		{
			using namespace Nui::ECS::Internal;

			ComponentId componentId = GetId<ComponentType>();

			// Assert if provided entity does not exist
			NUI_ASSERT(m_entities[GetEntityIndex(id)].Id == id,
				"Trying to check for a component on an entity that does not exist");

			if (!m_entities[GetEntityIndex(id)].Mask.test(componentId))
				return false;

			return true;
		}

		bool IsValidEntity(EntityId id);


		void Cleanup();

		U64 GetAliveCount() const;
		U64 GetEntityPoolSize() const;
		U64 GetFreeEntityCount() const;

	private:
		std::vector<Internal::EntityInfo>                     m_entities;
		std::vector<EntityIndex>                              m_freeEntities;
		std::vector<EntityId>                                 m_markedForDelete;
		std::vector<std::unique_ptr<Internal::ComponentPool>> m_componentPools;
	};


	template <typename... ComponentTypes>
	class WorldView
	{
	public:

		explicit WorldView(World& world)
			: m_index(0)
			, m_worldPtr(&world)
			, m_all(false)
		{
			using namespace Nui::ECS::Internal;

			// If no component types are there requested, then we want all the components to be included in this view
			if (sizeof...(ComponentTypes) == 0)
			{
				m_all = true;
			}
			else
			{
				// Unpack the template parameters into a sequence of calls to a function
				// The fold expression below expands the GetId<ComponentId>() calls into the array initializer
				ComponentId componentIds[] = { GetId<ComponentTypes>() ... };

				// Build the component mask from the component ids
				for (ComponentId componentId : componentIds)
				{
					m_componentMask.set(componentId);
				}
			}
		}

		inline World* GetWorld() { return m_worldPtr; }

		EntityId operator*() const
		{
			return m_worldPtr->m_entities[m_index].Id;
		}

		bool operator==(const WorldView& other) const
		{
			return m_index == other.m_index || m_index == m_worldPtr->m_entities.size();
		}

		bool operator!=(const WorldView& other) const
		{
			return m_index != other.m_index && m_index != m_worldPtr->m_entities.size();
		}

		WorldView& operator++()
		{
			do
			{
				m_index++;
			} while (m_index < m_worldPtr->m_entities.size() && !ValidIndex());

			return *this;
		}

		WorldView begin()
		{
			using namespace Nui::ECS::Internal;

			int firstIndex = 0;
			while (
				firstIndex < m_worldPtr->m_entities.size()
				&& (m_componentMask != (m_componentMask & m_worldPtr->m_entities[firstIndex].Mask)
				|| !IsEntityIdValid(m_worldPtr->m_entities[firstIndex].Id)))
			{
				firstIndex++;
			}

			return WorldView(*m_worldPtr, firstIndex, m_componentMask);
		}

		WorldView end()
		{
			return WorldView(*m_worldPtr, EntityIndex(m_worldPtr->m_entities.size()), m_componentMask);
		}

	private:
		WorldView(World& world, EntityIndex index, ComponentMask mask)
			: WorldView(world)
		{
			m_index = index;
			m_componentMask = mask;
		}

		bool ValidIndex()
		{
			using namespace Nui::ECS::Internal;

			bool validEntity = IsEntityIdValid(m_worldPtr->m_entities[m_index].Id);
			bool validComponentMask = (m_all || m_componentMask == (m_componentMask & m_worldPtr->m_entities[m_index].Mask));

			return validEntity && validComponentMask;
		}

	private:
		EntityIndex   m_index;
		World*        m_worldPtr;
		ComponentMask m_componentMask;
		bool          m_all;
	};
}