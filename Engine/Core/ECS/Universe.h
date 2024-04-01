#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/ECS/World.h"

namespace  Nui { class AppBase; }

namespace Nui::ECS
{

	class Universe
	{
		friend class ::Nui::AppBase;

	public:
		Universe();
		~Universe();

		template<typename T, typename... Args> requires std::is_base_of_v<World, T>
		T* CreateWorld(Args&&... args)
		{
			// If a world already exists then destroy it
			DestroyActiveWorld();
			NUI_LOG(Info, Universe, "Creating new world of type ", typeid(T).name());

			m_activeWorld = std::make_unique<T>(std::forward<Args>(args)...);
			m_activeWorld->OnConstruct();

			return dynamic_cast<T*>(m_activeWorld.get());
		}

		void DestroyActiveWorld()
		{
			if (m_activeWorld)
			{
				NUI_LOG(Info, Universe, "Destructing world of type ", typeid(*m_activeWorld).name());
				
				m_activeWorld->OnDestruct();
				m_activeWorld.reset();
			}
		}

		inline World* GetActiveWorld() const { return m_activeWorld.get(); }

		template <typename T> requires std::is_base_of_v<World, T>
		inline T* GetActiveWorld() const { return dynamic_cast<T*>(m_activeWorld.get()); }

	private:
		void Tick(F64 dt);

	private:
		std::unique_ptr<World> m_activeWorld;
	};
}