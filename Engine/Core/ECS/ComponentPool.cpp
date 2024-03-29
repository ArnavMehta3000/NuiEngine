#include "ComponentPool.h"

namespace Nui::ECS::Internal
{
    ComponentPool::ComponentPool(U64 elementSize)
        : m_elementSize(elementSize)
    {
        m_data.fill(Byte{});
    }

    ComponentPool::~ComponentPool()
    {}
}