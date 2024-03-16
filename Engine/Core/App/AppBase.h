#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/Window.h"

namespace Nui
{
	class AppBase : public Window
	{
	public:
		explicit AppBase(StringViewW appName, Window::Style style, Window::Size size, Window::Position position);
		virtual ~AppBase();

		void Run();
		void Quit();

	private:
		AppBase(const AppBase&) = delete;
		AppBase(AppBase&&) = delete;

	private:
		bool m_wantsToClose;
	};
}

