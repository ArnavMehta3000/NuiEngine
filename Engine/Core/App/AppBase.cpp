#include "AppBase.h"

namespace Nui
{
	AppBase::AppBase()
		: Window(Window::Style::WindowedFullscreen, { 1280, 720}, { 500, 250 }, L"NuiApp")
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