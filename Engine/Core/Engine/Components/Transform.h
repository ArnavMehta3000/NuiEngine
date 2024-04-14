#pragma once
#include "Core/Engine/ECS/Component.h"

namespace Nui::Components
{
	class Transform : public ECS::Component
	{
	public:
		Transform();
		Transform(Math::Vector2 pos, F32 rot = 0, Math::Vector2 scale = Math::Vector2::Zero);

		inline bool IsUnique() override { return true; }

		void SetPosition(Math::Vector2 pos);
		Math::Vector2 GetPosition() const;
		void SetRotation(F32 rot);
		F32 GetRotation() const;
		void SetScale(F32 scale);
		void SetScale(Math::Vector2 scale);
		Math::Vector2 GetScale() const;
		void SetTransform(Math::Matrix transform);
		void SetParent(Math::Matrix parent);
		const Math::Matrix& GetParent() const;
		Math::Matrix GetLocalMatrix() const;
		Math::Matrix GetWorldMatrix() const;


	private:
		Math::Matrix m_local;
		Math::Matrix m_parent;		
	};
}