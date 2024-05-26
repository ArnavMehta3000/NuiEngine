#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/Window.h"
#include "Core/Engine/World.h"

namespace Nui
{
	/**
	 * @brief Base class for all apps in Nui Engine
	 * @note Inherit from `Nui::Window`
	 * @note Derived class must implement `OnInit` and `OnShutdown`
	 */
	class AppBase : public Window
	{
		friend class Engine;
	public:
		/**
		 * @brief Construct a new AppBase object
		 * @param appName App name is also window title
		 * @param style Style of the application window
		 * @param size Size of the application window
		 */
		explicit AppBase(WStringView appName, Window::Style style, Window::Size size);

		/**
		 * @brief Virtual destructor
		 */
		virtual ~AppBase() = default;

		/**
		 * @brief Called when the app is initialized (to be implemented in derived class)
		 */
		virtual void OnInit() {}

		/**
		 * @brief Called when the app is shutdown (to be implemented in derived class)
		 */
		virtual void OnShutdown() {}

		/**
		 * @brief Get the world of the app
		 * @return A pointer to the app's world
		 */
		inline World* GetWorld() const { return m_world.get(); }

	private:
		AppBase(const AppBase&) = delete;
		AppBase(AppBase&&) = delete;

		/**
		 * @brief Called before the app is initialized
		 */
		void PreInit();

		/**
		 * @brief Called every frame
		 * @param dt Time since last frame (delta time)
		 */
		void Tick(const F64 dt);

		/**
		 * @brief Called after the app is shutdown
		 */
		void PostShutdown();

	private:
		/**
		 * @brief A unique pointer to the app's world
		 */
		std::unique_ptr<World> m_world;
	};

	namespace Internal
	{
		/**
		 * @brief External function to create an app (used by Engine)
		 * @return A unique pointer to the app
		 * @note User application must be declared using `NUI_DECLARE_APP` macro
		 */
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
