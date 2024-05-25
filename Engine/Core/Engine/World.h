#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/Engine/ECS/ECS.h"

namespace Nui
{
	class World final : public ECS::Context
	{
	public:
		World();
		World(const World&) = delete;
		World(World&&) = delete;
		~World();

		World operator=(const World&) = delete;
		World operator=(World&&) = delete;

		void Update(const F64 dt);

		inline void SetActive(bool active) { m_isActive = active; }
		bool IsActive() const { return m_isActive; }

	private:
		bool m_isActive;
	};
}