#include "RenderSystem.h"
#include "Graphics/Graphics.h"
using namespace Nui::Graphics;

namespace Nui::Systems
{
	void RenderSystem::OnInit(ECS::Context* ctx)
	{
		NUI_ASSERT(Graphics::IsInitialized(), 
			"RenderSystem::OnInit: Graphics not initialized. Initialize the graphics system before initializing the render system.");
	}

	void RenderSystem::OnUpdate(ECS::Context* ctx, const F64 dt)
	{
	}

	void RenderSystem::OnShutdown(ECS::Context* ctx)
	{
	}

	void RenderSystem::OnEvent(ECS::Context* ctx, const Events::EndFrame& event)
	{
	}
}