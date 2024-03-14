#include <Core/EntryPoint.h>


class TestbenchApp : public Nui::AppBase
{
public:
	TestbenchApp()
		: Nui::AppBase(L"TestbenchApp", Nui::Window::Style::Windowed, { 1280, 720 }, { 500, 250 })
	{

	}

	virtual ~TestbenchApp()
	{

	}
};

NUI_DECLARE_APP(TestbenchApp)