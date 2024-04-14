#include "Transform.h"

namespace Nui::Components
{
	Transform::Transform()
		: m_local(Math::Matrix::Identity)
		, m_parent(Math::Matrix::Identity)
	{
	}

	Transform::Transform(Math::Vector2 pos, F32 rot, Math::Vector2 scale)
		: m_parent(Math::Matrix::Identity)
	{
		Math::Matrix scaleMat = Math::Matrix::CreateScale(Math::Vector3(scale));
		Math::Matrix rotationMat = Math::Matrix::CreateRotationZ(rot);
		Math::Matrix translationMat = Math::Matrix::CreateTranslation(Math::Vector3(pos));

		m_local = scaleMat * rotationMat * translationMat;
	}

	void Transform::SetPosition(Math::Vector2 pos)
	{
		Math::Vector3 position, scale;
		Math::Quaternion rotation;
		m_local.Decompose(scale, rotation, position);

		position = Math::Vector3(pos);
		m_local = Math::Matrix::CreateScale(scale) * Math::Matrix::CreateFromQuaternion(rotation) * Math::Matrix::CreateTranslation(position);
	}

	Math::Vector2 Transform::GetPosition() const
	{
		Math::Vector3 position, scale;
		Math::Quaternion rotation;

		auto local = m_local;
		local.Decompose(scale, rotation, position);
		return Math::Vector2(position.x, position.y);
	}

	void Transform::SetRotation(F32 rot)
	{
		Math::Vector3 position, scale;
		Math::Quaternion rotation;
		m_local.Decompose(scale, rotation, position);

		m_local = Math::Matrix::CreateScale(scale) * Math::Matrix::CreateRotationZ(rot) * Math::Matrix::CreateTranslation(position);
	}

	F32 Transform::GetRotation() const
	{
		Math::Vector3 position, scale;
		Math::Quaternion rotation;

		auto local = m_local;
		local.Decompose(scale, rotation, position);

		return rotation.ToEuler().z;
	}

	void Transform::SetScale(F32 scale)
	{
		Math::Vector3 position, scaleVec;
		Math::Quaternion rotation;

		m_local.Decompose(scaleVec, rotation, position);

		m_local = Math::Matrix::CreateScale(Math::Vector3(scale)) * Math::Matrix::CreateFromQuaternion(rotation) * Math::Matrix::CreateTranslation(position);
	}

	void Transform::SetScale(Math::Vector2 scale)
	{
		Math::Vector3 position, scaleVec;
		Math::Quaternion rotation;
		m_local.Decompose(scaleVec, rotation, position);

		m_local = Math::Matrix::CreateScale(scale.x, scale.y, scaleVec.z) * Math::Matrix::CreateFromQuaternion(rotation) * Math::Matrix::CreateTranslation(position);
	}

	Math::Vector2 Transform::GetScale() const
	{
		Math::Vector3 position, scale;
		Math::Quaternion rotation;

		auto local = m_local;
		local.Decompose(scale, rotation, position);

		return Math::Vector2(scale.x, scale.y);
	}

	void Transform::SetTransform(Math::Matrix transform)
	{
		m_local = std::move(transform);
	}

	void Transform::SetParent(Math::Matrix parent)
	{
		m_parent = std::move(parent);
	}

	const Math::Matrix& Transform::GetParent() const
	{
		return m_parent;
	}

	Math::Matrix Transform::GetLocalMatrix() const
	{
		return m_local;
	}

	Math::Matrix Transform::GetWorldMatrix() const
	{
		if (m_parent == Math::Matrix::Identity)
		{
			return m_local;
		}

		return m_parent * m_local;
	}
}
