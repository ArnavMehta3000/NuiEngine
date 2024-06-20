#pragma once
#include <Core/App/AppBase.h>

class TestbenchApp : public Nui::AppBase
{
public:
	TestbenchApp();
	virtual ~TestbenchApp();

	void OnInit() override;
	void OnShutdown() override;
};
