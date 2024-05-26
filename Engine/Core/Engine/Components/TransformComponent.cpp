#include "TransformComponent.h"

namespace Nui::Components
{
	Transform::Transform()
		: m_rotation(Math::Quaternion::Identity)
		, m_position(Math::Vector3::Zero)
		, m_scale(Math::Vector3::One)
	{
	}

	Transform::Transform(Math::Vector3 position, Math::Quaternion rotation, Math::Vector3 scale)
		: m_rotation(rotation)
		, m_position(position)
		, m_scale(scale)
	{
	}

	void Transform::SetRotation(const Math::Quaternion& rotation)
	{
		m_rotation = rotation;
		SetDirty();
	}

	void Transform::SetPosition(const Math::Vector3& position)
	{
		m_position = position;
		SetDirty();
	}

	void Transform::SetScale(const Math::Vector3& scale)
	{
		m_scale = scale;
		SetDirty();
	}

	void Transform::RecalculateWorldMatrix()
	{
		const Math::Matrix S = Math::Matrix::CreateScale(m_scale);
		const Math::Matrix R = Math::Matrix::CreateFromQuaternion(m_rotation);
		const Math::Matrix T = Math::Matrix::CreateTranslation(m_position);

		m_worldMat = S * R * T;

		SetDirty(false);
	}
}
