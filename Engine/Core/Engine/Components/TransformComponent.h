#pragma once
#include "Core/Engine/Components/Common.h"

#define NUI_NODISCARD_TRANSFROM_GETTER [[nodiscard("Cannot discard return value of Transform getter functions")]]

namespace Nui::Components
{
	/**
	 * @brief Transform component
	 */
	class Transform : public ComponentBase
	{
	public:
		/**
		 * @brief Default constructor
		 */
		Transform();

		/**
		 * @brief Parametrized constructor
		 * @param position Position of the transform
		 * @param rotation Rotation of the transform (defaults to identity rotation)
		 * @param scale Scale of the transform (defaults to 1, 1, 1)
		 */
		Transform(Math::Vector3 position, Math::Quaternion rotation = Math::Quaternion::Identity, Math::Vector3 scale = Math::Vector3::One);

		/**
		 * @brief Copy constructor (defaulted)
		 */
		Transform(const Transform&) = default;

		/**
		 * @brief Move constructor (defaulted)
		 */
		Transform(Transform&&) = default;

		/**
		 * @brief Destructor (defaulted)
		 */
		~Transform() = default;

		/**
		 * @brief Copy assignment operator
		 * @return Reference to self
		 */
		Transform& operator=(const Transform&) = default;

		/**
		 * @brief Move assignment operator
		 * @return Reference to self
		 */
		Transform& operator=(Transform&&) noexcept = default;

		/**
		 * @brief Comparison operator
		 * @return 0 if equal
		 */
		auto operator<=>(const Transform&) const = default;
		
		/**
		 * @brief Const reference getter for rotation
		 * @return Rotation of the transform
		 */
		NUI_NODISCARD_TRANSFROM_GETTER inline const Math::Quaternion& GetRotation() const { return m_rotation; }

		/**
		 * @brief Getter for rotation
		 * @return Rotation of the transform
		 */
		NUI_NODISCARD_TRANSFROM_GETTER inline Math::Quaternion GetRotation() { return m_rotation; }

		/**
		 * @brief Const reference getter for position
		 * @return Position of the transform
		 */
		NUI_NODISCARD_TRANSFROM_GETTER inline const Math::Vector3& GetPosition() const { return m_position; }

		/**
		 * @brief Getter for position
		 * @return Position of the transform
		 */
		NUI_NODISCARD_TRANSFROM_GETTER inline Math::Vector3 GetPosition() { return m_position; }

		/**
		 * @brief Const reference getter for scale
		 * @return Scale of the transform
		 */
		NUI_NODISCARD_TRANSFROM_GETTER inline const Math::Vector3& GetScale() const { return m_scale; }

		/**
		 * @brief Getter for scale
		 * @return Scale of the transform
		 */
		NUI_NODISCARD_TRANSFROM_GETTER inline Math::Vector3 GetScale() { return m_scale; }

		/**
		 * @brief Getter for world matrix
		 * @return World matrix from position, rotation and scale
		 */
		NUI_NODISCARD_TRANSFROM_GETTER inline Math::Matrix GetWorldMatrix() const { return m_worldMat; }

		/**
		 * @brief Setter for rotation
		 * @param rotation New rotation
		 */
		[[noreturn]] void SetRotation(const Math::Quaternion& rotation);

		/**
		 * @brief Setter for position
		 * @param position New position
		 */
		[[noreturn]] void SetPosition(const Math::Vector3& position);

		/**
		 * @brief Setter for scale
		 * @param scale New scale
		 */
		[[noreturn]] void SetScale(const Math::Vector3& scale);

		/**
		 * @brief Recalculates world matrix (used when transform is dirty)
		 */
		[[noreturn]] void RecalculateWorldMatrix();

	private:
		/**
		 * @brief Rotation of the transform
		 */
		Math::Quaternion m_rotation;
		
		/**
		 * @brief Position of the transform
		 */
		Math::Vector3 m_position;

		/**
		 * @brief Scale of the transform
		 */
		Math::Vector3 m_scale;

		/**
		 * @brief World matrix from position, rotation and scale
		 */
		Math::Matrix m_worldMat;
	};
}

#undef NUI_NODISCARD_TRANSFROM_GETTER