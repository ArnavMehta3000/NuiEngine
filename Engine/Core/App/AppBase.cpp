#include "AppBase.h"

namespace Nui
{
	AppBase::AppBase(StringViewW appName, Window::Style style, Window::Size size, Window::Position position)
		: Window(style, appName, size, position)
		, m_wantsToClose(false)
	{
	}

	AppBase::~AppBase()
	{

	}

	void AppBase::Run()
	{
		while (!m_wantsToClose && !Window::WantsToClose())
		{
		}
	}
	void AppBase::Quit()
	{
		m_wantsToClose = true;
	}
}