#include "TestECSStructures.h"

namespace NuiCoreTest
{
	TEST_CLASS(TestECS)
	{
	public:

		// Simply add and remove 1000 entities
		TEST_METHOD(TestECSEntityAddRemove100000)
		{
			using namespace Nui;

			std::unique_ptr<ECS::Context> ctx = std::make_unique<ECS::Context>();

			constexpr I32 count = 100000;

			// Add 10 entities
			for (I32 i = 0; i < count; i++)
			{
				ECS::Entity* e = ctx->CreateEntity();
			}

			Logger::WriteMessage("Created 100000 entities\n");

			Assert::IsTrue((ctx->GetEntityCount() == count), L"Add entity count mismatch");

			// Remove 10 entities
			// ID starts from 1 and goes up to 'count'
			for (I32 i = 1; i <= count; i++)
			{
				ECS::Entity* e = ctx->GetEntityById(i);
				ctx->DestroyEntity(e);
			}

			// Clean context to delete pending
			ctx->ClearPending();

			Logger::WriteMessage("Removed 100000 entities\n");

			Assert::IsTrue((ctx->GetEntityCount() ==  0), L"Remove entity count mismatch");
		}

		TEST_METHOD(TestECSEntitySystem)
		{
			using namespace Nui;

			std::unique_ptr<ECS::Context> ctx = std::make_unique<ECS::Context>();
			TestSystem* system = ctx->RegisterSystem<TestSystem>();

			// Make sure system is initialized
			Assert::IsNotNull(system, L"TestSystem is nullptr. Registeration failed");
			Assert::IsTrue(system->m_initialized, L"TestSystem initialization failed");


			constexpr I32 count = 100000;

			// Add 10 entities with TestComponent
			for (I32 i = 0; i < count; i++)
			{
				ECS::Entity* e = ctx->CreateEntity();
				e->Add<TestComponent>(TestComponent{ 0, 0 });
			}

			Logger::WriteMessage("Created 100000 entities\n");

			// Make sure 10 entities with TestComponents were added
			Assert::IsTrue((system->m_testComponentCount == count), L"TestSystem component count mismatch");


			// Update context
			ctx->Tick(0.0f);  // 0.0f delta time

			// Check if context ticked properly
			for (ECS::Entity* e : ctx->Each<TestComponent>())
			{
				ECS::ComponentHandle<TestComponent> comp = e->Get<TestComponent>();

				Assert::IsTrue(comp.IsValid(), L"Entity does not have TestComponent or component is not valid");

				Assert::AreEqual(1, comp->X, L"TestComponent::X is not equal to '1' after context Tick");
				Assert::AreEqual(1, comp->Y, L"TestComponent::Y is not equal to '1' after context Tick");
			}

			// Manually loop through context to change component values
			ctx->Each<TestComponent>(
				[&](Nui::ECS::Entity* e, Nui::ECS::ComponentHandle<TestComponent> comp)
				{
					comp->X = 2;
					comp->Y = 2;
				});

			// Check if components updated after 'Each()'
			for (ECS::Entity* e : ctx->Each<TestComponent>())
			{
				ECS::ComponentHandle<TestComponent> comp = e->Get<TestComponent>();

				Assert::IsTrue(comp.IsValid(), L"Entity does not have TestComponent or component is not valid");

				Assert::AreEqual(2, comp->X, L"TestComponent::X is not equal to '2' after context Tick");
				Assert::AreEqual(2, comp->Y, L"TestComponent::Y is not equal to '2' after context Tick");
			}

			// Send test event and check if event was processed
			constexpr I32 eventId = 5;
			ctx->EmitEvent<TestEvent>(TestEvent{ eventId });

			Assert::AreEqual(5, system->m_eventVal, L"TestSystem TestEvent ID value mismatch");
		}
	};
}