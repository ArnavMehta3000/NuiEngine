#include "TransformComponent.h"

namespace Nui::Transform
{
	namespace // Anonymous namespace
	{
		std::vector<Math::Quaternion> s_rotations;
		std::vector<Math::Vector3>    s_positions;
		std::vector<Math::Vector3>    s_scales;
	}  // Anonymous namespace


	Component Create(const InitInfo& info, GameEntity::Entity owner)
	{
		NUI_ASSERT(owner.IsValid(), "Invalid entity");
		const Id::Type entityIndex = Id::Index(owner.GetId());

		if (s_positions.size() > entityIndex)
		{
			s_rotations[entityIndex] = Math::Quaternion(info.Rotation);
			s_positions[entityIndex] = Math::Vector3(info.Position);
			s_scales[entityIndex] = Math::Vector3(info.Scale);
		}
		else
		{
			NUI_ASSERT(s_positions.size() == entityIndex, "Entity index out of bounds");
			s_rotations.emplace_back(info.Rotation);
			s_positions.emplace_back(info.Position);
			s_scales.emplace_back(info.Scale);
		}

		return Component(TransformId(Id::Type(s_positions.size() - 1)));
	}

	void Destroy(Component id)
	{
		NUI_ASSERT(id.IsValid(), "Invalid transform component");
	}

	// Implemented from API/Transform.h
	const Math::Quaternion& Component::GetRotation() const
	{
		NUI_ASSERT(IsValid(), "Invalid transform component");
		return s_rotations[Id::Index(m_id)];
	}

	const Math::Vector3& Component::GetPosition() const
	{
		NUI_ASSERT(IsValid(), "Invalid transform component");
		return s_positions[Id::Index(m_id)];
	}

	const Math::Vector3& Component::GetScale() const
	{
		NUI_ASSERT(IsValid(), "Invalid transform component");
		return s_scales[Id::Index(m_id)];
	}
}

