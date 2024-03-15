#pragma once
#include "Core/Component/Common.h"

namespace Nui::Transform
{
	NUI_DEFINE_TYPED_ID(TransformId);

	class Component
	{
	public:
		constexpr Component() : m_id(Id::InvalidId) {}
		constexpr explicit Component(TransformId id) : m_id(id) {}

		constexpr TransformId GetId() const { return m_id; }
		constexpr bool IsValid() const { return Id::IsValid(m_id); }

		const Math::Quaternion& GetRotation() const;
		const Math::Vector3& GetPosition() const;
		const Math::Vector3& GetScale() const;
	
	private:
		TransformId m_id;
	};
}