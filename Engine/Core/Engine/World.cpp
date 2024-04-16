#include "World.h"
#include "Core/Engine/ECS/ECS.h"


struct Position
{
	Position(float x, float y) : x(x), y(y) {}
	Position() = default;

	float x;
	float y;
};

struct Rotation
{
	Rotation(float angle) : angle(angle) {}
	Rotation() = default;

	float angle;
};

struct SomeComponent
{
	SomeComponent() {}
};

struct SomeEvent
{
	int num;
};


class TestSystem : public Nui::ECS::SystemBase,
				   public Nui::ECS::EventSubscriber<Nui::ECS::Events::OnEntityCreate>,
				   public Nui::ECS::EventSubscriber<Nui::ECS::Events::OnEntityDestroy>,
				   public Nui::ECS::EventSubscriber<Nui::ECS::Events::OnComponentRemove<Position>>,
				   public Nui::ECS::EventSubscriber<Nui::ECS::Events::OnComponentRemove<Rotation>>,
				   public Nui::ECS::EventSubscriber<SomeEvent>
{
public:
	virtual ~TestSystem() {}

	virtual void OnInit(Nui::ECS::Context* ctx)
	{
		ctx->SubscribeEvent<Nui::ECS::Events::OnEntityCreate>(this);
		ctx->SubscribeEvent<Nui::ECS::Events::OnEntityDestroy>(this);
		ctx->SubscribeEvent<Nui::ECS::Events::OnComponentRemove<Position>>(this);
		ctx->SubscribeEvent<Nui::ECS::Events::OnComponentRemove<Rotation>>(this);
		ctx->SubscribeEvent<SomeEvent>(this);
	}

	virtual void OnUpdate(Nui::ECS::Context* ctx, Nui::F64 dt)
	{
		ctx->Each<Position, Rotation>(
			[&](Nui::ECS::Entity* e, Nui::ECS::ComponentHandle<Position> pos, Nui::ECS::ComponentHandle<Rotation> rot) -> void
			{
				pos->x += (float)dt;
				pos->y += (float)dt;
				rot->angle += (float)dt;
			}
		);
	}

	virtual void OnShutdown(Nui::ECS::Context* ctx)
	{
		ctx->UnsubscribeAll(this);
	}

	virtual void OnReceiveEvent(class Nui::ECS::Context* ctx, const Nui::ECS::Events::OnEntityCreate& event) override
	{
		NUI_LOG(Warn, TestECS, "An entity was created");
	}

	virtual void OnReceiveEvent(class Nui::ECS::Context* ctx, const Nui::ECS::Events::OnEntityDestroy& event) override
	{
		NUI_LOG(Warn, TestECS, "An entity was destroyed");
	}

	virtual void OnReceiveEvent(class Nui::ECS::Context* ctx, const Nui::ECS::Events::OnComponentRemove<Position>& event) override
	{
		NUI_LOG(Warn, TestECS, "A position component was removed");
	}

	virtual void OnReceiveEvent(class Nui::ECS::Context* ctx, const Nui::ECS::Events::OnComponentRemove<Rotation>& event) override
	{
		NUI_LOG(Warn, TestECS, "A rotation component was removed");
	}

	virtual void OnReceiveEvent(class Nui::ECS::Context* ctx, const SomeEvent& event) override
	{
		NUI_LOG(Info, TestECS, "Received some event with value ", event.num);
	}
};

namespace Nui
{
	Nui::ECS::Context* context{ nullptr };
	Nui::ECS::SystemBase* testSystem {nullptr};
	std::vector<Nui::ECS::Entity*> all;

	World::World()
		: m_isActive(true)
	{
		context = Nui::ECS::Context::Create();
		testSystem = context->RegisterSystem(new TestSystem());

		Nui::ECS::Entity* e = context->CreateEntity();
		e->Add<Position>(0.0f, 0.0f);
		e->Add<Rotation>(0.0f);
		all.push_back(e);

		for (int i = 0; i < 10; ++i)
		{
			e = context->CreateEntity();
			e->Add<SomeComponent>();
			all.push_back(e);
		}

		int count = 0;
		for (auto ent : context->Each<SomeComponent>())
		{
			count++;
			NUI_LOG(Warn, TestECS, "Found entity id: ", ent->GetId());
		}
	}

	World::~World()
	{
		context->Destroy();
	}
	static F64 time = 0.0;
	static bool doOnce = true;
	void World::Update(F64 dt)
	{
		if (!m_isActive)
		{
			return;
		}

		if (context)
		{
			time += dt;
			context->Tick(dt);

			if (time >= 5)
			{
				if (doOnce)
				{
					doOnce = false;

					NUI_LOG(Info, TestECS, "Before clean up entity count: ", context->GetEntityCount());

					for (auto& e : all)
					{
						context->DestroyEntity(e);
					}

					context->CleanUp();
					NUI_LOG(Info, TestECS, "After clean up entity count: ", context->GetEntityCount());

				}
			}
		}
	}
}