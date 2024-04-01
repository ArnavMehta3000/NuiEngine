#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/Window.h"
#include "Core/ECS/Universe.h"


namespace Nui
{
	class AppBase : public Window
	{
		friend class Engine;
	public:
		explicit AppBase(WStringView appName, Window::Style style, Window::Size size);
		virtual ~AppBase();

		inline ECS::Universe* GetUniverse() { return m_universe.get(); }

	private:
		AppBase(const AppBase&) = delete;
		AppBase(AppBase&&) = delete;

		void Tick(F64 dt);

	private:
		std::unique_ptr<ECS::Universe> m_universe;
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
