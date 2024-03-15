#include "CppUnitTest.h"
#include <Core/Utils/Filesystem.h>
#include <Core/Component/GameEntity.h>
#include <Core/Component/TransformComponent.h>
#include <ctime>
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NuiCoreTest
{
	TEST_CLASS(TestEntityComponents)
	{
	public:
		TEST_METHOD_INITIALIZE(TestEntityComponentsInitialize)
		{
			// Create random seed
			srand((Nui::U32)time(nullptr));
		}

		TEST_METHOD_CLEANUP(TestEntityComponentsShutdown)
		{
			m_entities.clear();
			m_added = 0;
			m_removed = 0;
			m_entitiesCount = 0;
		}

		TEST_METHOD(CreateDestroyRandomEntities)
		{
			try
			{
				Logger::WriteMessage("TestEntityComponents: CreateDestroyRandomEntities started\n");
				for (Nui::U32 i = 0; i < 10000; i++)
				{
					CreateRandom();
					DestroyRandom();
					m_entitiesCount = (Nui::U32)m_entities.size();
					
					if (i % 50 == 0)
					{
						Nui::String iStr = "Iteration: " + std::to_string(i);
						Logger::WriteMessage(iStr.c_str());
					}
				}
				
				Nui::String msg = std::to_string(m_added) + " added, " + std::to_string(m_removed) + " removed, " + std::to_string(m_entitiesCount) + " alive\n";
				Logger::WriteMessage(msg.c_str());
			}
			catch (const std::exception&)
			{
				Assert::Fail(L"Test failed with exception");
			}
		}

	private:
		void CreateRandom()
		{
			Nui::U32 count = rand() % 20;
			if (m_entities.empty())
				count = 1000;

			// Create init info
			Nui::Transform::InitInfo transformInfo = {};
			Nui::GameEntity::InitInfo entityInfo = 
			{
				.Transform = &transformInfo
			};

			while (count > 0)
			{
				m_added++;

				Nui::GameEntity::Entity e(Nui::GameEntity::Create(entityInfo));
				Assert::IsTrue(e.IsValid() && Nui::Id::IsValid(e.GetId()), L"Entity should be valid");
				m_entities.push_back(e);
				Assert::IsTrue(Nui::GameEntity::IsAlive(e), L"Entity should be alive after creation");
				count--;
			}
		}

		void DestroyRandom()
		{
			Nui::U32 count = rand() % 20;
			if (m_entities.size() < 1000)
				return;

			while (count > 0)
			{
				const Nui::U32 index = (Nui::U32)rand() % (Nui::U32)m_entities.size();
				const Nui::GameEntity::Entity e = m_entities[index];
				Assert::IsTrue(e.IsValid() && Nui::Id::IsValid(e.GetId()), L"Entity should be valid");

				if (e.IsValid())
				{
					Nui::GameEntity::Destroy(e);
					m_entities.erase(m_entities.begin() + index);
					Assert::IsFalse(Nui::GameEntity::IsAlive(e), L"Entity should be dead after destroying");
					m_removed++;
				}
				count--;
			}
		}

	private:
		std::vector<Nui::GameEntity::Entity> m_entities;

		Nui::U32 m_added;
		Nui::U32 m_removed;
		Nui::U32 m_entitiesCount;
	};
}