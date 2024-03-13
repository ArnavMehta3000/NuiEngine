#include "AppBase.h"

namespace Nui
{
	AppBase::AppBase()
		:
		Window(Window::Style::WindowedFullscreen, { 1280, 720}, { 500, 250 }, L"NuiApp")
	{

	}

	AppBase::~AppBase()
	{

	}}