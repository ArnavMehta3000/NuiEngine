#include "CppUnitTest.h"
#include <ctime>
#include <Core/ECS/World.h>
#include <Core/Utils/Filesystem.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NuiCoreTest
{
	using namespace Nui;

	TEST_CLASS(TestECS)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			// Create random engine
			srand(static_cast<unsigned int>(time(nullptr)));
		}
		TEST_METHOD(AddRemoveRandom)
		{

			// Create random engine
			try
			{
				Nui::Log::Internal::OpenLogFile(Nui::Filesystem::GetCurrentWorkingDirectory() / "Saved" / "ECSAddRemoveRandom.log");

				// Create world
				ECS::World world;

				for (U32 i = 0; i < 1000; i++)
				{
					CreateRandom(world);
					RemoveRandom(world);
				}

				U64 alive = world.GetAliveCount();
				U64 pool = world.GetEntityPoolSize();
				U64 free = world.GetFreeEntityCount();
				Print(world);
				
				Assert::IsTrue(m_added - m_removed == alive, L"Added - Removed != World alive. AddRemoveRandomTest failed");
				Assert::IsTrue(pool - alive == free, L"Pool - World alive != Free. AddRemoveRandomTest failed");

				Nui::Log::Internal::CloseLogFile();
			}
			catch (const std::exception& e)
			{
				NUI_LOG(Fatal, TestMain, e.what());
				Assert::Fail(L"AddRemove50 failed with exception");
			}
		}

	private:
		void CreateRandom(ECS::World& world)
		{

			U32 count = rand() % 20;
			if (m_entities.empty())
				count = 10000;

			while (count > 0)
			{
				m_added++;
				
				ECS::EntityId id = world.CreateEntity();
				m_entities.push_back(id);
				
				Assert::IsTrue(world.IsValidEntity(id), L"Entity ID is invalid");
				count--;
			}
		}

		void RemoveRandom(ECS::World& world)
		{
			U32 count = rand() % 20;
			if (m_entities.size() < 10000)
				return;

			const U32 index = (U32)rand() % (U32)m_entities.size();
			const ECS::EntityId id = m_entities[index];
			
			// Check if the entity is valid
			bool valid = world.IsValidEntity(id);
			if (!valid)
			{
				Print(world);
			}
			Assert::IsTrue(valid, L"Entity ID is invalid before removing");
			

			
			while (count > 0)
			{
				m_removed++;

				world.MarkForDelete(id);
				world.Cleanup();
				if (world.IsValidEntity(id))
				{
					Nui::StringW msg = std::format(L"Entity ID {} is still valid after cleanup\nIndex: {}\nVersion: {}", 
						id, ECS::Internal::GetEntityIndex(id), ECS::Internal::GetEntityVersion(id));
					
					Print(world);
					Assert::IsFalse(world.IsValidEntity(id), L"Entity ID is valid after cleanup");
				}
				count--;
			}
		}

		void Print(ECS::World& world)
		{
			U64 count = m_entities.size();
			U64 alive = world.GetAliveCount();
			U64 pool = world.GetEntityPoolSize();
			U64 free = world.GetFreeEntityCount();

			Nui::String msg = Log::Format("Added: ", m_added,
				"\nRemoved: ", m_removed,
				"\nCount: ", count,
				"\nWorld alive: ", alive,
				"\nEntity Pool: ", pool,
				"\nFree: ", free
			);
			Logger::WriteMessage(msg.c_str());
		}

	private:
		U32 m_added;
		U32 m_removed;
		std::vector<ECS::EntityId> m_entities;
	};
}