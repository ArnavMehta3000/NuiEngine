#include "CppUnitTest.h"
#include "TestApp.h"
#include <Core/Engine/Engine.h>
#include <Core/Utils/Singleton.h>
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NuiCoreTest
{
	TEST_CLASS(TesEngineRun)
	{
	public:

		TEST_METHOD(RunEngineForFiveSeconds)
		{
			using namespace Nui;
			try
			{
				auto& engine = Singleton<Engine>::Get();

				Logger::WriteMessage("Running engine for 5 seconds\n");

				std::jthread runThread([&engine]()
				{
					std::this_thread::sleep_for(std::chrono::seconds(5));
					engine.Quit();
				});

				engine.Run();
				
				Singleton<Engine>::Destroy();
			}
			catch (const std::exception&)
			{
				Assert::Fail(L"TestApp run failed with exception");
			}

			Logger::WriteMessage("Engine stopped\n");
		}
	};
}