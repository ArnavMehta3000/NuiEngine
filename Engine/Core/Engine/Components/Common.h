#pragma once
#include "Core/Common/CommonHeaders.h"
#include <bitset>

namespace Nui::Components
{
	enum ComponentFlags
	{
		CF_None,
		CF_IsDirty,
		CF_IsActive,

		CF_COUNT
	};

	class ComponentBase
	{
	public:
		ComponentBase()
		{
			SetDirty();  // All components are dirty by default and will need update on frame 1
		}

		[[nodiscard]] inline bool IsDirty() const { return m_flags.test(ComponentFlags::CF_IsDirty); }
		[[nodiscard]] inline bool IsActive() const { return m_flags.test(ComponentFlags::CF_IsActive); }
		[[noreturn]] inline void SetActive(bool isActive) { m_flags.set(ComponentFlags::CF_IsActive, isActive); }

	protected:
		[[noreturn]] inline void SetDirty(bool isDirty = true) { m_flags.set(ComponentFlags::CF_IsDirty, isDirty); }

	protected:
		std::bitset<ComponentFlags::CF_COUNT> m_flags;
	};
}