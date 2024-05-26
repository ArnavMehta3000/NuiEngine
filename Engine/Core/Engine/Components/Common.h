#pragma once
#include "Core/Common/CommonHeaders.h"
#include <bitset>

namespace Nui::Components
{
	/**
	 * @brief Component flags
	 */
	enum ComponentFlags
	{
		CF_None,
		CF_IsDirty,
		CF_IsActive,

		CF_COUNT
	};

	/**
	 * @brief Base class for all components
	 */
	class ComponentBase
	{
	public:
		/**
		 * @brief Constructor (sets dirty flag to true)
		 */
		ComponentBase()
		{
			SetDirty();  // All components are dirty by default and will need update on frame 1
		}

		/**
		 * @brief Checks if the component is dirty
		 * @return True if the component is dirty
		 */
		[[nodiscard]] inline bool IsDirty() const { return m_flags.test(ComponentFlags::CF_IsDirty); }

		/**
		 * @brief Checks if the component is active
		 * @return True if the component is active
		 */
		[[nodiscard]] inline bool IsActive() const { return m_flags.test(ComponentFlags::CF_IsActive); }

		/**
		 * @brief Sets the active flag
		 * @param isActive Active state to set
		 */
		[[noreturn]] inline void SetActive(bool isActive) { m_flags.set(ComponentFlags::CF_IsActive, isActive); }

	protected:
		/**
		 * @brief Sets the dirty flag
		 * @param isDirty Dirty state to set (defaults to true)
		 */
		[[noreturn]] inline void SetDirty(bool isDirty = true) { m_flags.set(ComponentFlags::CF_IsDirty, isDirty); }

	protected:
		/**
		 * @brief Bitset of component flags
		 */
		std::bitset<ComponentFlags::CF_COUNT> m_flags;
	};
}