#include "RenderSystem.h"
#include "Graphics/Graphics.h"
using namespace Nui::Graphics;

namespace Nui::Systems
{
	void RenderSystem::OnInit(ECS::Context* ctx)
	{
		NUI_ASSERT(Graphics::IsInitialized(), 
			"RenderSystem::OnInit: Graphics not initialized. Initialize the graphics system before initializing the render system.");
		
		NUI_SUBSCRIBE_EVENT(ctx, Events::EndFrame);
	}

	void RenderSystem::OnUpdate(ECS::Context* ctx, const F64 dt)
	{
	}

	void RenderSystem::OnShutdown(ECS::Context* ctx)
	{
		NUI_UNSUBSCRIBE_EVENTS_ALL(ctx);
	}

	NUI_DEFINE_EVENT(RenderSystem, Events::EndFrame)
	{
		NUI_LOG(Info, RenderSystem, "RenderSystem::OnEvent - EndFrame");
	}
}