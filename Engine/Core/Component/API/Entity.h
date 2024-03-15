#pragma once
#include "Core/Component/Common.h"
#include "Core/Component/API/Transform.h"

namespace Nui::GameEntity
{
	NUI_DEFINE_TYPED_ID(EntityId);

	class Entity
	{
	public:
		constexpr Entity() : m_id(Id::InvalidId) {}
		constexpr explicit Entity(EntityId id) : m_id(id) {}

		constexpr EntityId GetId() const { return m_id; }
		constexpr bool IsValid() const { return Id::IsValid(m_id); }

		Transform::Component GetTransform() const;
	
	private:
		EntityId m_id;
	};
}