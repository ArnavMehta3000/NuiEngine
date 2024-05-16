#pragma once
#include "Core/Engine/ECS/Common.h"

namespace Nui::ECS
{
	class SystemBase
	{
	public:
		virtual ~SystemBase() = default;

		virtual void OnInit(Context* ctx) {}
		virtual void OnUpdate(Context* ctx, const F64 dt) {}
		virtual void OnShutdown(Context* ctx) {}

		bool IsEnabled() const noexcept { return m_enabled; }
		void SetIsEnabled(bool enabled) noexcept { m_enabled = enabled; }

	private:
		bool m_enabled;
	};
}