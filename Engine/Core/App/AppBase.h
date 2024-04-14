#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/Window.h"
#include "Core/Engine/World.h"
#include "Core/Engine/ECS/EntityManager.h"
#include "Core/Engine/ECS/SystemManager.h"

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

		void Tick(F64 dt);

	private:
		std::unique_ptr<World> m_world;
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
