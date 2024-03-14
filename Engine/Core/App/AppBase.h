#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/Window.h"

namespace Nui
{
	class AppBase : public Window
	{
	public:
		AppBase();
		virtual ~AppBase();

		void Run();
		void Quit();

	private:
		bool m_wantsToClose;
	};
}

