#include "TestbenchApp.h"

TestbenchApp::TestbenchApp()
	: Nui::AppBase(L"TestbenchApp", Nui::Window::Style::Windowed, { 1280, 720 })
{

}

TestbenchApp::~TestbenchApp()
{

}

void TestbenchApp::OnInit()
{
	NUI_LOG(Info, TestbenchApp, "Initializing Testbench App");
}

void TestbenchApp::OnShutdown()
{
	NUI_LOG(Info, TestbenchApp, "Shutting down Testbench App");
}
