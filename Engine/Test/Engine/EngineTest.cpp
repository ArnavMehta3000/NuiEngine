#include "CppUnitTest.h"
#include <Core/App/AppBase.h>
#include <Core/Utils/Filesystem.h>
#include <thread>

class TestApp : public Nui::AppBase
{
};

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EngineTest
{
	TEST_CLASS(EngineTest)
	{
	public:
		
		TEST_METHOD(TestAppRunForFiveSeconds)
		{
			std::unique_ptr<TestApp> app;
			try
			{
				Nui::Log::Internal::OpenGlobalLogFile(Nui::Filesystem::GetCurrentWorkingDirectory() / "Saved" / "TestAppRunForFiveSeconds.log");

				app = std::make_unique<TestApp>();
				std::jthread runThread = std::jthread([&app]() 
				{
					app->Run();
				});

				std::this_thread::sleep_for(std::chrono::seconds(5));

				app->Quit();
				app.reset();

				Nui::Log::Internal::CloseGlobalLogFile();

			}
			catch (const std::exception& e)
			{
				NUI_LOG(Fatal, TestMain, e.what());
				Assert::Fail(L"TestApp run failed with exception");
			}

		}
	};
}