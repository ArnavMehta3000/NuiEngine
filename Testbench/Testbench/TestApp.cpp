#include "TestApp.h"
#include <Core/Utils/Singleton.h>
#include <Core/Engine/Engine.h>
#include <Core/Engine/Components/Transform.h>
#include <Core/Engine/Components/ScriptBase.h>

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
	auto& e = GetWorld()->GetEntityManager().CreateEntity();
	GetWorld()->SetRoot(*e.Query<Nui::Components::Node>());
}

void TestbenchApp::OnShutdown()
{
	NUI_LOG(Info, TestbenchApp, "Shutting down Testbench App");
}