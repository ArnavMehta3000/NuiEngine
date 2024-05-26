#include "AppBase.h"

namespace Nui
{
	AppBase::AppBase(WStringView appName, Window::Style style, Window::Size size)
		: Window(style, appName, size)
	{
		m_world = std::make_unique<World>();
	}

	void AppBase::PreInit()
	{
		// Set up default world systems
		m_world->OnInit();
	}

	void AppBase::Tick(const F64 dt)
	{
		// Update world
		m_world->Update(dt);
	}

	void AppBase::PostShutdown()
	{
		// Clean up world
		m_world->OnShutdown();
	}
}