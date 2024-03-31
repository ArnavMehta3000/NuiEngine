#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/Window.h"

namespace Nui
{
	class AppBase : public Window
	{
	public:
		explicit AppBase(WStringView appName, Window::Style style, Window::Size size);
		virtual ~AppBase();

	private:
		AppBase(const AppBase&) = delete;
		AppBase(AppBase&&) = delete;
	};

}
#define NUI_DECLARE_APP(app, ...)                 \
namespace Nui::Internal                           \
{                                                 \
	std::unique_ptr<Nui::AppBase> MakeApp()       \
	{                                             \
		return std::make_unique<app>(__VA_ARGS__);\
	}                                             \
}
