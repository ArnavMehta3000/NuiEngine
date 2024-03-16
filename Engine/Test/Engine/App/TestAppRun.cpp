#include "CppUnitTest.h"
#include "TestApp.h"
#include <Core/Utils/Filesystem.h>
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NuiCoreTest
{
	TEST_CLASS(TestAppRun)
	{
	public:
		
		TEST_METHOD(AppRunWindowedForFiveSeconds)
		{
			std::unique_ptr<TestApp> app;
			try
			{
				Nui::Log::Internal::OpenLogFile(Nui::Filesystem::GetCurrentWorkingDirectory() / "Saved" / "TestAppRunWindowedForFiveSeconds.log");

				app = std::make_unique<TestApp>(Nui::Window::Style::Windowed);
				std::jthread runThread = std::jthread([&app]() 
				{
					app->Run();
				});

				std::this_thread::sleep_for(std::chrono::seconds(5));

				app->Quit();
				app.reset();

				Nui::Log::Internal::CloseLogFile();

			}
			catch (const std::exception& e)
			{
				NUI_LOG(Fatal, TestMain, e.what());
				Assert::Fail(L"TestApp run failed with exception");
			}

		}

		TEST_METHOD(AppRunWindowedFullscreenForFiveSeconds)
		{
			std::unique_ptr<TestApp> app;
			try
			{
				Nui::Log::Internal::OpenLogFile(Nui::Filesystem::GetCurrentWorkingDirectory() / "Saved" / "TestAppRunWindowedFullscreenForFiveSeconds.log");

				app = std::make_unique<TestApp>(Nui::Window::Style::WindowedFullscreen);
				std::jthread runThread = std::jthread([&app]()
					{
						app->Run();
					});

				std::this_thread::sleep_for(std::chrono::seconds(5));

				app->Quit();
				app.reset();

				Nui::Log::Internal::CloseLogFile();

			}
			catch (const std::exception& e)
			{
				NUI_LOG(Fatal, TestMain, e.what());
				Assert::Fail(L"TestApp run failed with exception");
			}
		}

		TEST_METHOD(AppRunBorderlessForFiveSeconds)
		{
			std::unique_ptr<TestApp> app;
			try
			{
				Nui::Log::Internal::OpenLogFile(Nui::Filesystem::GetCurrentWorkingDirectory() / "Saved" / "TestAppRunBorderlessForFiveSeconds.log");

				app = std::make_unique<TestApp>(Nui::Window::Style::Borderless);
				std::jthread runThread = std::jthread([&app]()
					{
						app->Run();
					});

				std::this_thread::sleep_for(std::chrono::seconds(5));

				app->Quit();
				app.reset();

				Nui::Log::Internal::CloseLogFile();

			}
			catch (const std::exception& e)
			{
				NUI_LOG(Fatal, TestMain, e.what());
				Assert::Fail(L"TestApp run failed with exception");
			}
		}

		TEST_METHOD(AppRunBorderlessFullscreenForFiveSeconds)
		{
			std::unique_ptr<TestApp> app;
			try
			{
				Nui::Log::Internal::OpenLogFile(Nui::Filesystem::GetCurrentWorkingDirectory() / "Saved" / "TestAppRunBorderlessFullscreenForFiveSeconds.log");

				app = std::make_unique<TestApp>(Nui::Window::Style::BorderlessFullscreen);
				std::jthread runThread = std::jthread([&app]()
					{
						app->Run();
					});

				std::this_thread::sleep_for(std::chrono::seconds(5));

				app->Quit();
				app.reset();

				Nui::Log::Internal::CloseLogFile();

			}
			catch (const std::exception& e)
			{
				NUI_LOG(Fatal, TestMain, e.what());
				Assert::Fail(L"TestApp run failed with exception");
			}
		}
	};
}