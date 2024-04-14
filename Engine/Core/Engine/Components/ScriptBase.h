#pragma once
#include "Core/Engine/ECS/Component.h"

namespace Nui::Components
{
	class ScriptBase : public ECS::Component
	{
	public:
		ScriptBase() = default;
		virtual ~ScriptBase() = default;

		virtual void OnInit() = 0;
		virtual void Update(F64 dt) = 0;

		inline void SetName(StringView name) { m_name = name; }
		inline const String GetName() const noexcept { return m_name; }

	private:
		String m_name;
	};
}