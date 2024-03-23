#include <Core/EntryPoint.h>


class TestbenchApp : public Nui::AppBase
{
public:
	TestbenchApp()
		: Nui::AppBase(L"TestbenchApp", Nui::Window::Style::Borderless, { 1280, 720 })
	{

	}

	virtual ~TestbenchApp()
	{

	}
};

NUI_DECLARE_APP(TestbenchApp)