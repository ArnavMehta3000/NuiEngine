#include "TestApp.h"
#include <Core/Utils/Singleton.h>
#include <Core/Engine/Engine.h>
//#include <Core/Engine/ECS/ECS.h>

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
	using namespace Nui;
}

void TestbenchApp::OnShutdown()
{
	NUI_LOG(Info, TestbenchApp, "Shutting down Testbench App");
}