#pragma once
#include "Core/Engine/ECS/EntityManager.h"
#include "Core/Engine/ECS/Recursor.h"
#include "Core/Engine/Components/Node.h"

namespace Nui::ECS
{
	class ISystem : public IRecursor
	{
	public:
		~ISystem() = default;
		virtual void OnInit() {};
		virtual void Shutdown() {};
	};

	class HierarchicalSystem : public ISystem, public HierarchicalRecursor<Components::Node, bool>
	{
	public:
		void Update(F64 dt) override;
	};

	class SequentialSystemBase : public ISystem {};

	template <typename C = Component>
	class SequentialSystem : public SequentialSystemBase, public SequentialRecursor<std::shared_ptr<C>, void, EntityManager::iterator>
	{
	public:
		void Update(F64 dt) override
		{
			using Rec = SequentialRecursor<std::shared_ptr<C>, void, EntityManager::iterator>;


			OnInit();

			for (auto it = this->begin(); it != this->end(); it++)
			{
				auto& compEntityPair = *it;
				Entity& e = compEntityPair.second;

				ComponentVector<C> vec = e.Query<C>();

				for (std::shared_ptr<Component> c : vec)
				{
					this->GetCallback()(std::static_pointer_cast<C>(c), dt);
				}
			}

			Shutdown();
		}
	};

	template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<ISystem, T>>>
	std::unique_ptr<T> CreateSequentialSystem(Args&&... args)
	{
		auto system = std::make_unique<T>(std::forward<Args>(args)...);
		return std::unique_ptr<ISystem>(static_cast<ISystem*>(system.release()));
	}
}