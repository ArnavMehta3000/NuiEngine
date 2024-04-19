#include "AppBase.h"

namespace Nui
{
	namespace // Anonymous namespace
	{
		static AppBase* s_currentApp = nullptr;
	}  // Anonymous namespace

	AppBase::AppBase(WStringView appName, Window::Style style, Window::Size size)
		: Window(style, appName, size)
	{
		if (s_currentApp)
		{
			NUI_ASSERT(false, "Cannot create multiple AppBase instances");
		}

		s_currentApp = this;

		m_world = std::make_unique<World>();
	}

	void AppBase::Tick(F64 dt)
	{
		if (m_world->IsActive())
		{
			m_world->Update(dt);
		}
	}
}