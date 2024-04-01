#include "Core/ECS/ECSInternal.h"

namespace Nui::ECS::Internal
{
	// Memory pool for components
	// (meant to be managed by the world to ensure type correctness)
	class ComponentPool
	{
	public:
		ComponentPool() = delete;
		explicit ComponentPool(U64 elementSize);
		~ComponentPool();

		template <typename T>
		T* Get(U64 index)
		{
			return reinterpret_cast<T*>(&m_data[index * m_elementSize]);
		}

	private:
		U64 m_elementSize{ 0 };
		std::array<Byte, MAX_ENTITIES * sizeof(Byte)> m_data;
	};
}