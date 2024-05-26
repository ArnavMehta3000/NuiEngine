#pragma once
#include "Core/Engine/Components/Common.h"

#define NUI_NODISCARD_TRANSFROM_GETTER [[nodiscard("Cannot discard return value of Transform getter functions")]]

namespace Nui::Components
{
	class Transform : public ComponentBase
	{
	public:
		Transform();
		Transform(Math::Vector3 position, Math::Quaternion rotation = Math::Quaternion::Identity, Math::Vector3 scale = Math::Vector3::One);
		Transform(const Transform&) = default;
		Transform(Transform&&) = default;
		~Transform() = default;

		Transform& operator=(const Transform& other) = default;
		Transform& operator=(Transform&& other) noexcept = default;
		auto operator<=>(const Transform&) const = default;
				
		NUI_NODISCARD_TRANSFROM_GETTER inline const Math::Quaternion& GetRotation() const { return m_rotation; }
		NUI_NODISCARD_TRANSFROM_GETTER inline Math::Quaternion GetRotation() { return m_rotation; }
		NUI_NODISCARD_TRANSFROM_GETTER inline const Math::Vector3& GetPosition() const { return m_position; }
		NUI_NODISCARD_TRANSFROM_GETTER inline Math::Vector3 GetPosition() { return m_position; }
		NUI_NODISCARD_TRANSFROM_GETTER inline const Math::Vector3& GetScale() const { return m_scale; }
		NUI_NODISCARD_TRANSFROM_GETTER inline Math::Vector3 GetScale() { return m_scale; }
		NUI_NODISCARD_TRANSFROM_GETTER inline Math::Matrix GetWorldMatrix() const { return m_worldMat; }

		[[noreturn]] void SetRotation(const Math::Quaternion& rotation);
		[[noreturn]] void SetPosition(const Math::Vector3& position);
		[[noreturn]] void SetScale(const Math::Vector3& scale);

		[[noreturn]] void RecalculateWorldMatrix();

	private:
		Math::Quaternion m_rotation;
		Math::Vector3    m_position;
		Math::Vector3    m_scale;
		Math::Matrix     m_worldMat;
	};
}

#undef NUI_NODISCARD_TRANSFROM_GETTER