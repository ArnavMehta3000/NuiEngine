#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/Window.h"
#include "Core/Engine/World.h"

namespace Nui
{
	class AppBase : public Window
	{
		friend class Engine;
	public:
		explicit AppBase(WStringView appName, Window::Style style, Window::Size size);
		virtual ~AppBase() = default;

		virtual void OnInit() {}
		virtual void OnShutdown() {}

		inline World* GetWorld() const { return m_world.get(); }

	private:
		AppBase(const AppBase&) = delete;
		AppBase(AppBase&&) = delete;

		void PreInit();
		void Tick(const F64 dt);
		void PostShutdown();

	private:
		std::unique_ptr<World> m_world;
	};

	namespace Internal
	{
		extern std::unique_ptr<Nui::AppBase> MakeApp();
	}
}

#define NUI_DECLARE_APP(app, ...)                 \
namespace Nui::Internal                           \
{                                                 \
	std::unique_ptr<Nui::AppBase> MakeApp()       \
	{                                             \
		return std::make_unique<app>(__VA_ARGS__);\
	}                                             \
}
