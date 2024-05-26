#include "TestApp.h"

TestApp::TestApp(Nui::Window::Style style)
	: Nui::AppBase(L"NuiTestApp", style, { 1280, 720 })
{
}

TestApp::~TestApp()
{
}

NUI_DECLARE_APP(TestApp, Nui::Window::Style::Windowed)