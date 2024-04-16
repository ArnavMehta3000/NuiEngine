#pragma once
#include "Core/Common/CommonHeaders.h"

namespace Nui
{
	class World final
	{
	public:
		World();
		World(const World&) = delete;
		World(World&&) = delete;
		~World();

		World operator=(const World&) = delete;
		World operator=(World&&) = delete;
		void Update(F64 dt);

		inline void SetActive(bool active) { m_isActive = active; }
		bool IsActive() const { return m_isActive; }

	private:
		bool m_isActive;
	};
}