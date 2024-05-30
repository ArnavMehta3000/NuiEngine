#include "RenderSystem.h"
#include "Graphics/Graphics.h"
using namespace Nui::Graphics;

namespace Nui::Systems
{
	void RenderSystem::OnInit(ECS::Context* ctx)
	{
		NUI_ASSERT(Graphics::IsInitialized(),
			"RenderSystem::OnInit: Graphics not initialized. Initialize the graphics system before initializing the render system.");

		NUI_LOG(Debug, RenderSystem, "RenderSystem::OnInit");

		NUI_SUBSCRIBE_EVENT(ctx, Events::EndFrame);
	}

	void RenderSystem::OnUpdate(ECS::Context* ctx, const F64 dt)
	{
	}

	void RenderSystem::OnShutdown(ECS::Context* ctx)
	{
		NUI_LOG(Debug, RenderSystem, "RenderSystem::OnShutdown");
		NUI_UNSUBSCRIBE_EVENTS_ALL(ctx);
	}

	NUI_DEFINE_EVENT(RenderSystem, Events::EndFrame)
	{
		OnEndFrame();
	}

	void RenderSystem::OnEndFrame()
	{
		const D3DManager& d3d = Singleton<D3DManager>::Get();
		ID3D11DeviceContext* d3dCtx = d3d.GetImmediateContext();

		d3dCtx->ClearRenderTargetView(d3d.GetBackBuffer(), DirectX::Colors::CornflowerBlue);

		d3d.Present();
	}
}