#pragma once
#include "CppUnitTest.h"
#include <Core/Engine/ECS/ECS.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct TestComponent
{
	// Defualt values is '-1 for testing
	Nui::I32 X = -1;
	Nui::I32 Y = -1;
};

struct TestEvent
{
	Nui::I32 ID;
};

class TestSystem : public Nui::ECS::SystemBase,
				   public Nui::ECS::EventSubscriber<Nui::ECS::Events::OnEntityCreate>,
				   public Nui::ECS::EventSubscriber<Nui::ECS::Events::OnEntityDestroy>,
				   public Nui::ECS::EventSubscriber<Nui::ECS::Events::OnComponentAdd<TestComponent>>,
				   public Nui::ECS::EventSubscriber<Nui::ECS::Events::OnComponentRemove<TestComponent>>,
				   public Nui::ECS::EventSubscriber<TestEvent>
{
public:
	virtual ~TestSystem() {}

	virtual void OnInit(Nui::ECS::Context* ctx)
	{
		ctx->SubscribeEvent<Nui::ECS::Events::OnEntityCreate>(this);
		ctx->SubscribeEvent<Nui::ECS::Events::OnEntityDestroy>(this);
		ctx->SubscribeEvent<Nui::ECS::Events::OnComponentAdd<TestComponent>>(this);
		ctx->SubscribeEvent<Nui::ECS::Events::OnComponentRemove<TestComponent>>(this);
		ctx->SubscribeEvent<TestEvent>(this); 

		m_initialized = true;
	}

	virtual void OnUpdate(Nui::ECS::Context* ctx, const Nui::F64 dt)
	{
		// Change all entities with test component values to 1
		ctx->Each<TestComponent>(
			[&](Nui::ECS::Entity* e, Nui::ECS::ComponentHandle<TestComponent> comp)
			{
				comp->X = 1;
				comp->Y = 1;
			});
	}

	virtual void OnShutdown(Nui::ECS::Context* ctx)
	{
		ctx->UnsubscribeAll(this);
	}

	virtual void OnEvent(class Nui::ECS::Context* ctx, const Nui::ECS::Events::OnEntityCreate& event) override
	{
	}

	virtual void OnEvent(class Nui::ECS::Context* ctx, const Nui::ECS::Events::OnEntityDestroy& event) override
	{
	}

	virtual void OnEvent(class Nui::ECS::Context* ctx, const Nui::ECS::Events::OnComponentAdd<TestComponent>& event) override
	{
		m_testComponentCount++;
	}

	virtual void OnEvent(class Nui::ECS::Context* ctx, const Nui::ECS::Events::OnComponentRemove<TestComponent>& event) override
	{
		m_testComponentCount--;
	}

	virtual void OnEvent(class Nui::ECS::Context* ctx, const TestEvent& event) override
	{
		m_eventVal = event.ID;
	}

public:
	bool m_initialized{ false };
	Nui::I32 m_testComponentCount{ 0 };
	Nui::I32 m_eventVal{ 0 };
};