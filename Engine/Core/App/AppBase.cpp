#include "AppBase.h"

namespace Nui
{
	AppBase::AppBase(StringViewW appName, Window::Style style, Window::Size size)
		: Window(style, appName, size)
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