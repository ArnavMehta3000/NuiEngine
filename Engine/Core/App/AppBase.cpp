#include "AppBase.h"

namespace Nui
{
	AppBase::AppBase(WStringView appName, Window::Style style, Window::Size size)
		: Window(style, appName, size)
	{
		m_world = std::make_unique<World>();
	}

	void AppBase::Tick(const F64 dt)
	{
		if (m_world->IsActive())
		{
			m_world->Update(dt);
		}
	}
}