#include "AppBase.h"

namespace Nui
{
	AppBase::AppBase(WStringView appName, Window::Style style, Window::Size size)
		: Window(style, appName, size)
	{
	}

	AppBase::~AppBase()
	{

	}
}