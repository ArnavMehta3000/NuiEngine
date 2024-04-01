#include "AppBase.h"

namespace Nui
{
	AppBase::AppBase(WStringView appName, Window::Style style, Window::Size size)
		: Window(style, appName, size)
	{
		m_universe = std::make_unique<ECS::Universe>();
	}

	AppBase::~AppBase()
	{

	}
	void AppBase::Tick(F64 dt)
	{
		Input::Internal::Update();
		m_universe->Tick(dt);
	}
}